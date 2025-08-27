#ifndef MOVEIT_MOCK_HTTP_CLIENT_H
#define MOVEIT_MOCK_HTTP_CLIENT_H
#include <map>
#include <string>
#include <unordered_map>

#include "http_client.h"

namespace moveit::network {

class mock_http_client : public http_client {
public:
  ~mock_http_client() = default;
  enum class mode { success, http_error, api_error };
  mode mode = mode::success;
  mutable std::string last_url;
  mutable std::string last_body;

  mutable std::map<std::string, std::string> last_headers;

  mutable std::unordered_map<std::string, std::string> fake_responses;

  void set_response(const std::string &url, const std::string &response) {
    fake_responses[url] = response;
  }

  generic_response
  get(const url &u,
      const std::map<std::string, std::string> &headers) const override {
    last_url = u;
    last_headers = headers;
    if (mode == mode::http_error) {
      throw std::runtime_error("HTTP request failed: simulated network error");
    }
    if (mode == mode::api_error) {
      throw std::runtime_error("HTTP request failed: simulated api error");
    }

    return generic_response{true,
                            fake_responses.count(u) ? fake_responses[u] : "{}"};
  }

  generic_response post(const url &u, const body &b,
                        const std::map<std::string, std::string> &headers,
                        DataProvider data_provider = nullptr) const override {
    last_url = u;
    last_body = b;
    last_headers = headers;
    if (mode == mode::http_error) {
      throw std::runtime_error("HTTP request failed: simulated network error");
    }
    if (mode == mode::api_error) {
      throw std::runtime_error("HTTP request failed: simulated api error");
    }
    return generic_response{true,
                            fake_responses.count(u) ? fake_responses[u] : "{}"};
  }

  generic_response
  put(const url &u, const body &b,
      const std::map<std::string, std::string> &headers) const override {
    last_url = u;
    last_body = b;
    last_headers = headers;
    // TODO (samuil) add the other modes that simulate issues
    return generic_response{true,
                            fake_responses.count(u) ? fake_responses[u] : "{}"};
  }

  generic_response
  del(const url &u,
      const std::map<std::string, std::string> &headers) const override {
    last_url = u;
    last_headers = headers;
    // TODO (samuil) add the other modes that simulate issues
    return generic_response{true,
                            fake_responses.count(u) ? fake_responses[u] : "{}"};
  }
};
} // namespace moveit::network

#endif