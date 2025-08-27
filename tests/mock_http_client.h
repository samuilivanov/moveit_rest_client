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
  std::string last_url;
  std::string last_body;

  std::map<std::string, std::string> last_headers;

  std::unordered_map<std::string, std::string> fake_responses;

  void set_response(const std::string &url, const std::string &response) {
    fake_responses[url] = response;
  }

  generic_response
  get(const std::string &url,
      const std::map<std::string, std::string> &headers) override {
    last_url = url;
    last_headers = headers;
    if (mode == mode::http_error) {
      throw std::runtime_error("HTTP request failed: simulated network error");
    }
    if (mode == mode::api_error) {
      throw std::runtime_error("HTTP request failed: simulated api error");
    }

    return generic_response{
        true, fake_responses.count(url) ? fake_responses[url] : "{}"};
  }

  generic_response post(const std::string &url, const std::string &body,
                        const std::map<std::string, std::string> &headers,
                        DataProvider data_provider = nullptr) override {
    last_url = url;
    last_body = body;
    last_headers = headers;
    if (mode == mode::http_error) {
      throw std::runtime_error("HTTP request failed: simulated network error");
    }
    if (mode == mode::api_error) {
      throw std::runtime_error("HTTP request failed: simulated api error");
    }
    return generic_response{
        true, fake_responses.count(url) ? fake_responses[url] : "{}"};
  }

  generic_response
  put(const std::string &url, const std::string &body,
      const std::map<std::string, std::string> &headers) override {
    last_url = url;
    last_body = body;
    last_headers = headers;
    // TODO (samuil) add the other modes that simulate issues
    return generic_response{
        true, fake_responses.count(url) ? fake_responses[url] : "{}"};
  }

  generic_response
  del(const std::string &url,
      const std::map<std::string, std::string> &headers) override {
    last_url = url;
    last_headers = headers;
    // TODO (samuil) add the other modes that simulate issues
    return generic_response{
        true, fake_responses.count(url) ? fake_responses[url] : "{}"};
  }
};
} // namespace moveit::network

#endif