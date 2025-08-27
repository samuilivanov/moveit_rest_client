#ifndef MOVEIT_CPR_HTTP_CLIENT_H
#define MOVEIT_CPR_HTTP_CLIENT_H

#include "http_client.h"
#include <cpr/api.h>
#include <stdexcept>

namespace moveit::network {

class cpr_http_client : public http_client {
public:
  ~cpr_http_client() override = default;

  [[nodiscard]] generic_response
  get(const url &u,
      const std::map<std::string, std::string> &headers = {}) const override;

  [[nodiscard]] generic_response
  post(const url &u, const body &b = body{},
       const std::map<std::string, std::string> &headers = {},
       DataProvider data_provider = nullptr) const override;

  [[nodiscard]] generic_response
  put(const url &u, const body &b = body{},
      const std::map<std::string, std::string> &headers = {}) const override;

  [[nodiscard]] generic_response
  del(const url &u,
      const std::map<std::string, std::string> &headers = {}) const override;
};
} // namespace moveit::network

#endif