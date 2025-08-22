#ifndef MOVEIT_HTTP_CLIENT_H
#define MOVEIT_HTTP_CLIENT_H

#include <map>
#include <string>

namespace network {

class http_client {
public:
  virtual ~http_client() = default;

  virtual std::string
  get(const std::string &url,
      const std::map<std::string, std::string> &headers = {}) = 0;

  virtual std::string
  post(const std::string &url, const std::string &body = "",
       const std::map<std::string, std::string> &headers = {}) = 0;

  virtual std::string
  put(const std::string &url, const std::string &body = "",
      const std::map<std::string, std::string> &headers = {}) = 0;

  virtual std::string
  del(const std::string &url,
      const std::map<std::string, std::string> &headers = {}) = 0;
};
} // namespace moveit_client

#endif