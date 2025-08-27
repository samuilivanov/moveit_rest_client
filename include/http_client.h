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

struct url {
  std::string value;
  explicit url(std::string v) : value(std::move(v)) {}
  [[nodiscard]] operator std::string() const { return value; }
};

struct body {
  std::string value;
  body() = default;
  explicit body(std::string v) : value(std::move(v)) {}
  [[nodiscard]] operator std::string() const { return value; }
};

using DataProvider = std::function<size_t(char *buffer, size_t maxLength)>;

class http_client {
public:
  virtual ~http_client() = default;

  [[nodiscard]] virtual generic_response
  get(const url &u,
      const std::map<std::string, std::string> &headers = {}) const = 0;

  [[nodiscard]] virtual generic_response
  post(const url &u, const body &b = body{},
       const std::map<std::string, std::string> &headers = {},
       DataProvider data_provider = nullptr) const = 0;

  [[nodiscard]] virtual generic_response
  put(const url &u, const body &b = body{},
      const std::map<std::string, std::string> &headers = {}) const = 0;

  [[nodiscard]] virtual generic_response
  del(const url &u,
      const std::map<std::string, std::string> &headers = {}) const = 0;
};
} // namespace moveit::network

#endif