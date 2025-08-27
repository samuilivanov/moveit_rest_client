#include "cpr_http_client.h"
#include <cpr/cpr.h>
#include <cstring>
#include <iostream>

namespace {

cpr::Header mapToHeader(const std::map<std::string, std::string> &headers) {
  cpr::Header h;
  for (const auto &[key, value] : headers) {
    h[key] = value;
  }
  return h;
}

moveit::network::generic_response check_response(const cpr::Response &r) {
  if (r.error.code != cpr::ErrorCode::OK) {
    throw std::runtime_error("HTTP request failed: " + r.error.message);
  }
  bool ok = r.status_code > 201;

  return {!ok, r.text};
}
} // namespace

namespace moveit::network {

generic_response
cpr_http_client::get(const std::string &url,
                     const std::map<std::string, std::string> &headers) {
  auto r = cpr::Get(cpr::Url{url}, mapToHeader(headers));
  return check_response(r);
}

generic_response
cpr_http_client::post(const std::string &url, const std::string &body,
                      const std::map<std::string, std::string> &headers,
                      DataProvider data_provider) {
  if (!data_provider) {
    auto r = cpr::Post(cpr::Url{url}, cpr::Body{body}, mapToHeader(headers));
    return check_response(r);
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
    return check_response(r);
  }
}

generic_response
cpr_http_client::put(const std::string &url, const std::string &body,
                     const std::map<std::string, std::string> &headers) {
  auto r = cpr::Put(cpr::Url{url}, cpr::Body{body}, mapToHeader(headers));
  return check_response(r);
}

generic_response
cpr_http_client::del(const std::string &url,
                     const std::map<std::string, std::string> &headers) {
  auto r = cpr::Delete(cpr::Url{url}, mapToHeader(headers));
  return check_response(r);
}
} // namespace moveit::network
