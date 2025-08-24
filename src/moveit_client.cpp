#include "moveit_client.h"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>

namespace {

struct upload_state {
  std::ifstream &file;
  const std::string preamble;
  const std::string ending;
  size_t preamble_offset = 0;
  size_t ending_offset = 0;
  size_t uploaded = 0;
  bool file_done = false;
};

auto make_upload_callback = [](std::ifstream &file, size_t total_size,
                               const std::string &boundary,
                               const std::string &filename) {
  auto state = std::make_shared<upload_state>(upload_state{
      file,
      "--" + boundary +
          "\r\nContent-Disposition: form-data; name=\"file\"; filename=\"" +
          filename + "\"\r\nContent-Type: application/octet-stream\r\n\r\n",
      "\r\n--" + boundary + "--\r\n", 0, 0, 0, false});

  return [state, total_size](char *buffer, size_t max_length) -> size_t {
    // 1. Preamble
    if (state->preamble_offset < state->preamble.size()) {
      size_t copySize =
          std::min(max_length, state->preamble.size() - state->preamble_offset);
      memcpy(buffer, state->preamble.data() + state->preamble_offset, copySize);
      state->preamble_offset += copySize;
      return copySize;
    }

    // 2. File
    if (!state->file_done) {
      state->file.read(buffer, max_length);
      size_t read_bytes = state->file.gcount();
      state->uploaded += read_bytes;

      double percent = static_cast<double>(state->uploaded) / total_size * 100;
      std::cout << "\rUploaded " << state->uploaded << "/" << total_size
                << " bytes (" << static_cast<int>(percent) << "%)";
      std::cout.flush();

      if (state->file.eof())
        state->file_done = true;

      return read_bytes;
    }

    // 3. Ending
    if (state->ending_offset < state->ending.size()) {
      size_t copySize =
          std::min(max_length, state->ending.size() - state->ending_offset);
      memcpy(buffer, state->ending.data() + state->ending_offset, copySize);
      state->ending_offset += copySize;
      return copySize;
    }

    // 4. Done
    return 0;
  };
};

} // namespace

namespace core {
moveit_client::moveit_client(std::unique_ptr<network::http_client> http_client,
                             const std::string &baseUrl)
    : m_http_client(std::move(http_client)), m_base_url(baseUrl) {}

// TODO (samuil) the hard coded string need to be refactored
auth_response moveit_client::authenticate(const std::string &username,
                                          const std::string &password) {
  std::string body =
      "grant_type=password&username=" + username + "&password=" + password;

  std::map<std::string, std::string> headers = {
      {"Content-Type", "application/x-www-form-urlencoded"}};

  std::string response =
      m_http_client->post(m_base_url + "/api/v1/token", body, headers);
  nlohmann::json j = nlohmann::json::parse(response);

  return auth_response::fromJson(j);
}

// TODO (samuil) the hard coded string need to be refactored
int moveit_client::get_home_folder(const std::string &token) {
  std::map<std::string, std::string> headers = {
      {"Authorization", "Bearer " + token}};

  std::string response =
      m_http_client->get(m_base_url + "/api/v1/users/self", headers);
  nlohmann::json j = nlohmann::json::parse(response);
  return user_info_response::fromJson(j).homeFolderID;
}

bool moveit_client::upload_file(const std::string &file_path,
                                const std::string &token, int id) {

  std::ifstream file(file_path, std::ios::binary);
  if (!file.is_open())
    throw std::runtime_error("Cannot open file");
  file.seekg(0, std::ios::end);
  size_t total_size = file.tellg();
  file.seekg(0, std::ios::beg);

  std::string boundary = "----Boundary12345";
  std::string filename = std::filesystem::path(file_path).filename().string();

  network::DataProvider provider =
      make_upload_callback(file, total_size, boundary, filename);

  std::map<std::string, std::string> headers{
      {"Authorization", "Bearer " + token},
      {"Content-Type", "multipart/form-data; boundary=" + boundary},
      {"Transfer-Encoding", "chunked"}};

  auto r = m_http_client->post(m_base_url + "/api/v1/folders/" +
                                   std::to_string(id) + "/files",
                               "", headers, provider);
  std::cout << r << '\n';
  return true;
}
} // namespace core
