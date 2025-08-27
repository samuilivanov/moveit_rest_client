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
  moveit::network::cpr_http_client client;
  std::map<std::string, std::string> headers{{"User-Agent", "doctest-client"}};

  auto response =
      client.get(moveit::network::url{"https://httpbin.org/get"}, headers);

  CHECK(response.response.find("\"User-Agent\": \"doctest-client\"") !=
        std::string::npos);
}

TEST_CASE("cpr_http_client POST request" * doctest::skip()) {
  moveit::network::cpr_http_client client;
  std::map<std::string, std::string> headers{
      {"Content-Type", "application/json"}};
  std::string b = R"({"message":"hello"})";

  auto response = client.post(moveit::network::url{"https://httpbin.org/post"},
                              moveit::network::body{b}, headers);

  CHECK(response.response.find("\"message\": \"hello\"") != std::string::npos);
}

TEST_CASE("cpr_http_client PUT request" * doctest::skip()) {
  moveit::network::cpr_http_client client;
  std::map<std::string, std::string> headers{
      {"Content-Type", "application/json"}};
  std::string b = R"({"update":"value"})";

  auto response = client.put(moveit::network::url{"https://httpbin.org/put"},
                             moveit::network::body{b}, headers);

  CHECK(response.response.find("\"update\": \"value\"") != std::string::npos);
}

TEST_CASE("cpr_http_client DELETE request" * doctest::skip()) {
  moveit::network::cpr_http_client client;
  std::map<std::string, std::string> headers{{"X-Test", "delete"}};

  auto response =
      client.del(moveit::network::url{"https://httpbin.org/delete"}, headers);

  CHECK(response.response.find("\"X-Test\": \"delete\"") != std::string::npos);
}
