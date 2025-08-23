#include "moveit_client.h"

namespace core {
moveit_client::moveit_client(std::unique_ptr<network::http_client> httpClient,
                             const std::string &baseUrl)
    : m_http_client(std::move(httpClient)), m_base_url(baseUrl) {}

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

upload_response moveit_client::upload_file(const std::string &filePath) {
  // open file
  // check if all is good
  // in a while loop load in memory parts of it and
  // upload_chunk();
  return upload_response{};
}
} // namespace core
