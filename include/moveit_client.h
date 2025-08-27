#ifndef MOVEIT_MOVEIT_CLIENT_HPP
#define MOVEIT_MOVEIT_CLIENT_HPP

#include "http_client.h"
#include "responses.h"
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

namespace moveit::core {

class moveit_client {
public:
  moveit_client(std::unique_ptr<network::http_client> http_client,
                const std::string &baseUrl);
  ~moveit_client() = default;

  network::auth_result authenticate(
      const std::string &username,
      const std::string &password); // TODO (samuil) this password should not be
                                    // in this form it's a security issue

  network::user_info_result get_home_folder(const std::string &token);

  network::upload_result upload_file(const std::string &file_path,
                                     const std::string &token, int id);

private:
  std::unique_ptr<network::http_client> m_http_client;
  std::string m_base_url;
  std::string m_auth_token;

  //   std::map<std::string, std::string> defaultHeaders() const;
};
} // namespace moveit::core

#endif