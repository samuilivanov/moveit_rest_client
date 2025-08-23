#ifndef MOVEIT_MOVEIT_CLIENT_HPP
#define MOVEIT_MOVEIT_CLIENT_HPP

#include "http_client.h"
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>

namespace core {

struct auth_response {
  std::string access_token;
  std::string token_type;
  int expires_in;
  std::string refresh_token;

  static auth_response fromJson(const nlohmann::json &j) {
    auth_response resp;
    // TODO (samuil) this needs to be in a if statment
    resp.access_token = j.at("access_token").get<std::string>();
    // NOTE (samuil) for now I am skipping the other json fields
    
    return resp;
  }
};

struct user_info_response {
  int homeFolderID = 0;

  static user_info_response fromJson(const nlohmann::json &j) {
    user_info_response info;
    if (j.contains("homeFolderID")) {
      info.homeFolderID = j.at("homeFolderID").get<int>();
    } else {
      throw std::runtime_error("JSON does not contain homeFolderID");
    }
    return info;
  }
};

// TODO (samuil) finish this it's a mockup for now not the real response from moveit
struct upload_response {
  bool success;
  std::string fileId;
  std::string message;

  static upload_response fromJson(const std::string &json) {
    return upload_response{};
  }
};

class moveit_client {
public:
  moveit_client(std::unique_ptr<network::http_client> httpClient,
                const std::string
                    &baseUrl) /* TODO (samuil) remove this parameter and move it to cli */;
  ~moveit_client() = default;

  auth_response authenticate(
      const std::string &username,
      const std::string &password); // TODO (samuil) this password should not be
                                    // in this form it's a security issue

  int get_home_folder(const std::string &token);

  upload_response upload_file(const std::string &filePath);

private:
  std::unique_ptr<network::http_client> m_http_client;
  std::string m_base_url;
  std::string m_auth_token;

  //   std::map<std::string, std::string> defaultHeaders() const;
};
} // namespace core

#endif