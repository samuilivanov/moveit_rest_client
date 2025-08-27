#ifndef MOVEIT_CPR_HTTP_CLIENT_H
#define MOVEIT_CPR_HTTP_CLIENT_H

#include "http_client.h"
#include <cpr/api.h>
#include <stdexcept>

namespace moveit::network {

class cpr_http_client : public http_client {
public:
  ~cpr_http_client() override = default;

  std::string
  get(const std::string &url,
      const std::map<std::string, std::string> &headers = {}) override;

  std::string post(const std::string &url, const std::string &body = "",
                   const std::map<std::string, std::string> &headers = {},
                   DataProvider data_provider = nullptr) override;

  std::string
  put(const std::string &url, const std::string &body = "",
      const std::map<std::string, std::string> &headers = {}) override;

  std::string
  del(const std::string &url,
      const std::map<std::string, std::string> &headers = {}) override;
};
} // namespace network

#endif