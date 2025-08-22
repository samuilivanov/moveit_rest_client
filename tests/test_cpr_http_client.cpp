#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "cpr_http_client.h"
#include "doctest/doctest.h"
#include <map>
#include <string>
// NOTE (samuil) not really unit tests these are just for my sanity that the cpr
// library works
// NOTE (samuil) will dissable after a while since these are slow or move them
// somewhere else
// NOTE (samuil) this is also a bit flaky

TEST_CASE("cpr_http_client GET request" * doctest::skip()) {
  network::cpr_http_client client;
  std::map<std::string, std::string> headers{{"User-Agent", "doctest-client"}};

  std::string response = client.get("https://httpbin.org/get", headers);

  CHECK(response.find("\"User-Agent\": \"doctest-client\"") !=
        std::string::npos);
}

TEST_CASE("cpr_http_client POST request" * doctest::skip()) {
  network::cpr_http_client client;
  std::map<std::string, std::string> headers{
      {"Content-Type", "application/json"}};
  std::string body = R"({"message":"hello"})";

  std::string response = client.post("https://httpbin.org/post", body, headers);

  CHECK(response.find("\"message\": \"hello\"") != std::string::npos);
}

TEST_CASE("cpr_http_client PUT request" * doctest::skip()) {
  network::cpr_http_client client;
  std::map<std::string, std::string> headers{
      {"Content-Type", "application/json"}};
  std::string body = R"({"update":"value"})";

  std::string response = client.put("https://httpbin.org/put", body, headers);

  CHECK(response.find("\"update\": \"value\"") != std::string::npos);
}

TEST_CASE("cpr_http_client DELETE request" * doctest::skip()) {
  network::cpr_http_client client;
  std::map<std::string, std::string> headers{{"X-Test", "delete"}};

  std::string response = client.del("https://httpbin.org/delete", headers);

  CHECK(response.find("\"X-Test\": \"delete\"") != std::string::npos);
}
