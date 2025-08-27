#ifndef MOVEIT_HTTP_CLIENT_H
#define MOVEIT_HTTP_CLIENT_H

#include <functional>
#include <map>
#include <string>

namespace moveit::network {

struct generic_response {
  bool success;
  std::string response;
};

using DataProvider = std::function<size_t(char *buffer, size_t maxLength)>;

class http_client {
public:
  virtual ~http_client() = default;

  virtual generic_response
  get(const std::string &url,
      const std::map<std::string, std::string> &headers = {}) = 0;

  virtual generic_response
  post(const std::string &url, const std::string &body = "",
       const std::map<std::string, std::string> &headers = {},
       DataProvider data_provider = nullptr) = 0;

  virtual generic_response
  put(const std::string &url, const std::string &body = "",
      const std::map<std::string, std::string> &headers = {}) = 0;

  virtual generic_response
  del(const std::string &url,
      const std::map<std::string, std::string> &headers = {}) = 0;
};
} // namespace moveit::network

#endif