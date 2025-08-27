#include "moveit_client.h"
#include "util.h"
#include <algorithm>
#include <chrono>
#include <cstddef> // for size_t
#include <fstream>
#include <iostream>

namespace {

size_t copy_chunk(const std::string &src, size_t &offset, char *buffer,
                  size_t max_length) {
  if (offset >= src.size()) {
    return 0;
  }
  size_t copySize = std::min(max_length, src.size() - offset);
  memcpy(buffer, src.data() + offset, copySize);
  offset += copySize;
  return copySize;
}

struct upload_state {
  std::ifstream &file;
  const std::string preamble;
  const std::string hash_header;
  const std::string hashtype_header;
  const std::string ending;

  size_t preamble_offset = 0;
  size_t hash_header_offset = 0;
  size_t hash_value_offset = 0;
  size_t hashtype_header_offset = 0;
  size_t hashtype_value_offset = 0;
  size_t ending_offset = 0;
  size_t uploaded = 0;
  bool file_done = false;
  bool hash_ready = false;

  std::unique_ptr<EVP_MD_CTX, decltype(&EVP_MD_CTX_free)> mdctx;
  std::string hash_hex;

  upload_state(std::ifstream &f, std::string pre, std::string hash_hdr,
               std::string hashtype_hdr, std::string end)
      : file(f), preamble(std::move(pre)), hash_header(std::move(hash_hdr)),
        hashtype_header(std::move(hashtype_hdr)), ending(std::move(end)),
        mdctx(moveit::util::make_evp_md_ctx()) {
    EVP_DigestInit_ex(mdctx.get(), EVP_sha256(), nullptr);
  }

  void update_hash(const char *data, size_t len) {
    EVP_DigestUpdate(mdctx.get(), data, len);
  }

  void finalize_hash() {
    if (hash_ready)
      return;
    unsigned int len = 0;
    std::vector<unsigned char> digest(EVP_MAX_MD_SIZE);
    EVP_DigestFinal_ex(mdctx.get(), digest.data(), &len);

    std::ostringstream oss;
    for (unsigned int i = 0; i < len; ++i)
      oss << std::hex << std::setw(2) << std::setfill('0')
          << static_cast<int>(digest[i]);
    hash_hex = oss.str();
    hash_ready = true;
  }
};

size_t stream_file_chunk(upload_state &state, char *buffer, size_t max_length,
                         size_t total_size) {
  if (state.file_done) {
    return 0;
  }
  state.file.read(buffer, max_length);
  size_t read_bytes = state.file.gcount();
  state.uploaded += read_bytes;
  if (read_bytes > 0) {
    state.update_hash(buffer, read_bytes);
  }

  // progress
  double percent = static_cast<double>(state.uploaded) / total_size * 100;
  std::cout << "\rUploaded " << state.uploaded << "/" << total_size
            << " bytes (" << static_cast<int>(percent) << "%)";
  std::cout.flush();

  if (state.file.eof()) {
    state.file_done = true;
    state.finalize_hash();
    std::cout << "\nSHA-256: " << state.hash_hex << "\n";
  }

  return read_bytes;
}

auto make_upload_callback = [](std::ifstream &file, size_t total_size,
                               const std::string &boundary,
                               const std::string &filename) {
  auto state = std::make_shared<upload_state>(
      file,
      "--" + boundary +
          "\r\nContent-Disposition: form-data; name=\"file\"; filename=\"" +
          filename + "\"\r\nContent-Type: application/octet-stream\r\n\r\n",
      "\r\n--" + boundary +
          "\r\nContent-Disposition: form-data; name=\"hash\"\r\n\r\n",
      "\r\n--" + boundary +
          "\r\nContent-Disposition: form-data; name=\"hashtype\"\r\n\r\n",
      "\r\n--" + boundary + "--\r\n");

  return [state, total_size](char *buffer, size_t max_length) -> size_t {
    // 1. Preamble
    if (auto n = copy_chunk(state->preamble, state->preamble_offset, buffer,
                            max_length)) {
      return n;
    }
    // 2. File
    if (auto n = stream_file_chunk(*state, buffer, max_length, total_size))
      return n;

    // 3. Hash header
    if (auto n = copy_chunk(state->hash_header, state->hash_header_offset,
                            buffer, max_length)) {

      return n;
    }

    // 4. Hash value
    if (state->hash_ready) {
      const std::string &h = state->hash_hex;
      if (auto n = copy_chunk(h, state->hash_value_offset, buffer, max_length))
        return n;
    }

    // 5. Hashtype header
    if (auto n = copy_chunk(state->hashtype_header,
                            state->hashtype_header_offset, buffer, max_length))
      return n;

    // 6. hashtype value
    static const std::string hashtype_value = "sha-256";
    if (auto n = copy_chunk(hashtype_value, state->hashtype_value_offset,
                            buffer, max_length))
      return n;

    // 7. ending
    if (auto n =
            copy_chunk(state->ending, state->ending_offset, buffer, max_length))
      return n;

    // 8. Done
    return 0;
  };
};

} // namespace

namespace moveit::core {
moveit_client::moveit_client(std::unique_ptr<network::http_client> http_client,
                             const std::string &baseUrl)
    : m_http_client(std::move(http_client)), m_base_url(baseUrl) {}

// TODO (samuil) the hard coded string need to be refactored
network::auth_result moveit_client::authenticate(const std::string &username,
                                                 const std::string &password) {
  std::string body =
      "grant_type=password&username=" + username + "&password=" + password;

  std::map<std::string, std::string> headers = {
      {"Content-Type", "application/x-www-form-urlencoded"}};

  auto raw_response =
      m_http_client->post(m_base_url + "/api/v1/token", body, headers);
  nlohmann::json j = nlohmann::json::parse(raw_response.response);
  if (raw_response.success) {
    return network::auth_response::fromJson(j);
  } else {
    return network::auth_error::fromJson(j);
  }
}

// TODO (samuil) the hard coded string need to be refactored
network::user_info_result
moveit_client::get_home_folder(const std::string &token) {
  std::map<std::string, std::string> headers = {
      {"Authorization", "Bearer " + token}};

  auto raw_response =
      m_http_client->get(m_base_url + "/api/v1/users/self", headers);

  nlohmann::json j = nlohmann::json::parse(raw_response.response);
  if (raw_response.success) {
    return network::user_info_response::fromJson(j);
  } else {
    return network::error_response::fromJson(j);
  }
}
// TODO (samuil) a future improvment might be to check if the file is already
// there because at the moment it will fail with error that the file exists in
// this folder
network::upload_result moveit_client::upload_file(const std::string &file_path,
                                                  const std::string &token,
                                                  int id) {

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

  auto raw_response = m_http_client->post(m_base_url + "/api/v1/folders/" +
                                              std::to_string(id) + "/files",
                                          "", headers, provider);

  nlohmann::json j = nlohmann::json::parse(raw_response.response);
  if (raw_response.success) {
    return network::upload_response{};
  } else {
    return network::error_response::fromJson(j);
  }
}
} // namespace moveit::core
