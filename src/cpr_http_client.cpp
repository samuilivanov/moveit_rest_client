#include "cpr_http_client.h"
#include <cpr/cpr.h>
#include <cstring>
#include <iostream>

namespace {

[[nodiscard]] cpr::Header
mapToHeader(const std::map<std::string, std::string> &headers) {
  cpr::Header h;
  for (const auto &[key, value] : headers) {
    h[key] = value;
  }
  return h;
}

[[nodiscard]] moveit::network::generic_response
check_response(const cpr::Response &r) {
  if (r.error.code != cpr::ErrorCode::OK) {
    throw std::runtime_error("HTTP request failed: " + r.error.message);
  }
  bool ok = r.status_code > 201;

  return {!ok, r.text};
}
} // namespace

namespace moveit::network {

[[nodiscard]] generic_response
cpr_http_client::get(const url &u,
                     const std::map<std::string, std::string> &headers) const {
  auto r = cpr::Get(cpr::Url{u}, mapToHeader(headers));
  return check_response(r);
}

[[nodiscard]] generic_response
cpr_http_client::post(const url &u, const body &b,
                      const std::map<std::string, std::string> &headers,
                      DataProvider data_provider) const {
  if (!data_provider) {
    auto r = cpr::Post(cpr::Url{u}, cpr::Body{b}, mapToHeader(headers));
    return check_response(r);
  } else {
    struct Context {
      DataProvider provider;
    } ctx{data_provider};

    auto r = cpr::Post(
        cpr::Url{u}, mapToHeader(headers),
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

[[nodiscard]] generic_response
cpr_http_client::put(const url &u, const body &b,
                     const std::map<std::string, std::string> &headers) const {
  auto r = cpr::Put(cpr::Url{u}, cpr::Body{b}, mapToHeader(headers));
  return check_response(r);
}

[[nodiscard]] generic_response
cpr_http_client::del(const url &u,
                     const std::map<std::string, std::string> &headers) const {
  auto r = cpr::Delete(cpr::Url{u}, mapToHeader(headers));
  return check_response(r);
}
} // namespace moveit::network
