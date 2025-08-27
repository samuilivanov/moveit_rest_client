#ifndef MOVEIT_RESPONSES_H
#define MOVEIT_RESPONSES_H

#include <nlohmann/json.hpp>
#include <string>
#include <variant>

namespace moveit::network {
struct auth_response {
  std::string access_token;
  std::string token_type;
  int expires_in;
  std::string refresh_token;

  [[nodiscard]] static auth_response fromJson(const nlohmann::json &j) {
    auth_response resp{};
    if (j.contains("access_token")) {
      resp.access_token = j.at("access_token").get<std::string>();
    } else {
      throw std::runtime_error("JSON does not contain access_token");
    }

    return resp;
  }
};

struct auth_error {
  std::string error;
  std::string error_description;
  [[nodiscard]] static auth_error fromJson(const nlohmann::json &j) {
    auth_error resp{};
    // TODO (samuil) this needs to be in a if statment
    resp.error = j.at("error").get<std::string>();
    resp.error_description = j.at("error_description").get<std::string>();
    // NOTE (samuil) for now I am skipping the other json fields

    return resp;
  }
};

using auth_result = std::variant<auth_response, auth_error>;

struct user_info_response {
  int homeFolderID = 0;

  [[nodiscard]] static user_info_response fromJson(const nlohmann::json &j) {
    user_info_response info;
    if (j.contains("homeFolderID")) {
      info.homeFolderID = j.at("homeFolderID").get<int>();
    } else {
      throw std::runtime_error("JSON does not contain homeFolderID");
    }
    return info;
  }
};

struct error_response {
  std::string detail;
  int errorCode = 0;

  [[nodiscard]] static error_response fromJson(const nlohmann::json &j) {
    error_response info;
    if (j.contains("detail")) {
      info.detail = j.at("detail").get<std::string>();
    } else {
      throw std::runtime_error("JSON does not contain detail");
    }
    if (j.contains("errorCode")) {
      info.errorCode = j.at("errorCode").get<int>();
    } else {
      throw std::runtime_error("JSON does not contain errorCode");
    }
    return info;
  }
};

using user_info_result = std::variant<user_info_response, error_response>;

struct upload_response {
  bool success = true;
};
using upload_result = std::variant<upload_response, error_response>;
} // namespace moveit::network

#endif