#ifndef MOVEIT_MOCK_HTTP_CLIENT_H
#define MOVEIT_MOCK_HTTP_CLIENT_H
#include <map>
#include <string>
#include <unordered_map>

#include "http_client.h"

namespace network {

class mock_http_client : public http_client {
public:
  ~mock_http_client() = default;
  enum class mode { success, http_error, api_error };
  mode mode = mode::success;
  std::string last_url;
  std::string last_body;

  std::map<std::string, std::string> last_headers;

  std::unordered_map<std::string, std::string> fake_responses;

  void set_response(const std::string &url, const std::string &response) {
    fake_responses[url] = response;
  }

  std::string get(const std::string &url,
                  const std::map<std::string, std::string> &headers) override {
    last_url = url;
    last_headers = headers;
    if (mode == mode::http_error) {
      throw std::runtime_error("HTTP request failed: simulated network error");
    }
    if (mode == mode::api_error) {
      throw std::runtime_error("HTTP request failed: simulated api error");
    }

    return fake_responses.count(url) ? fake_responses[url] : "{}";
  }

  std::string post(const std::string &url, const std::string &body,
                   const std::map<std::string, std::string> &headers) override {
    last_url = url;
    last_body = body;
    last_headers = headers;
    if (mode == mode::http_error) {
      throw std::runtime_error("HTTP request failed: simulated network error");
    }
    if (mode == mode::api_error) {
      throw std::runtime_error("HTTP request failed: simulated api error");
    }
    return fake_responses.count(url) ? fake_responses[url] : "{}";
  }

  std::string put(const std::string &url, const std::string &body,
                  const std::map<std::string, std::string> &headers) override {
    last_url = url;
    last_body = body;
    last_headers = headers;
    // TODO (samuil) add the other modes that simulate issues
    return fake_responses.count(url) ? fake_responses[url] : "{}";
  }

  std::string del(const std::string &url,
                  const std::map<std::string, std::string> &headers) override {
    last_url = url;
    last_headers = headers;
    // TODO (samuil) add the other modes that simulate issues
    return fake_responses.count(url) ? fake_responses[url] : "{}";
  }
};
} // namespace network

#endif