#include "cpr_http_client.h"
#include <cpr/cpr.h>
#include <cstring>

namespace {

cpr::Header mapToHeader(const std::map<std::string, std::string> &headers) {
  cpr::Header h;
  for (const auto &[key, value] : headers) {
    h[key] = value;
  }
  return h;
}

void check_response(const cpr::Response &r) {
  if (r.error.code != cpr::ErrorCode::OK) {
    throw std::runtime_error("HTTP request failed: " + r.error.message);
  }
  // TODO (samuil) this will mess up with the moveit status codes
  // del this
  if (r.status_code < 200 || r.status_code >= 300) {
    throw std::runtime_error("HTTP request returned status code: " +
                             std::to_string(r.status_code));
  }
}

} // namespace

namespace moveit::network {

std::string
cpr_http_client::get(const std::string &url,
                     const std::map<std::string, std::string> &headers) {
  auto r = cpr::Get(cpr::Url{url}, mapToHeader(headers));
  check_response(r);
  return r.text;
}

std::string
cpr_http_client::post(const std::string &url, const std::string &body,
                      const std::map<std::string, std::string> &headers,
                      DataProvider data_provider) {
  if (!data_provider) {
    auto r = cpr::Post(cpr::Url{url}, cpr::Body{body}, mapToHeader(headers));
    check_response(r);
    return r.text;
  } else {
    struct Context {
      DataProvider provider;
    } ctx{data_provider};

    auto r = cpr::Post(
        cpr::Url{url}, mapToHeader(headers),
        cpr::ReadCallback{
            [](char *buffer, size_t &length, intptr_t userdata) -> bool {
              Context *ctx_ = reinterpret_cast<Context *>(userdata);
              size_t written = ctx_->provider(buffer, length);
              length = written;
              return true;
            },
            reinterpret_cast<intptr_t>(&ctx)});
    return r.text;
  }
}

std::string
cpr_http_client::put(const std::string &url, const std::string &body,
                     const std::map<std::string, std::string> &headers) {
  auto r = cpr::Put(cpr::Url{url}, cpr::Body{body}, mapToHeader(headers));
  check_response(r);
  return r.text;
}

std::string
cpr_http_client::del(const std::string &url,
                     const std::map<std::string, std::string> &headers) {
  auto r = cpr::Delete(cpr::Url{url}, mapToHeader(headers));
  check_response(r);
  return r.text;
}
} // namespace moveit::network
