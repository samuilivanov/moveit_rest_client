#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "mock_http_client.h"

// NOTE (samuil) this test is more to verify the mock is working properly and
// can be used in the other tests

TEST_CASE("mock_http_client basic behavior") {
  moveit::network::mock_http_client client;
  client.set_response("http://test/get", R"({"status":"ok"})");
  client.set_response("http://test/post", R"({"result":"created"})");

  SUBCASE("GET request returns fake response") {
    auto res = client.get(moveit::network::url{"http://test/get"}, {});
    CHECK(res.response == R"({"status":"ok"})");
    CHECK(client.last_url == "http://test/get");
  }

  SUBCASE("POST request returns fake response and stores body") {
    auto res = client.post(moveit::network::url{"http://test/post"},
                           moveit::network::body{"data=123"}, {});
    CHECK(res.response == R"({"result":"created"})");
    CHECK(client.last_body == "data=123");
  }

  SUBCASE("PUT returns default if not configured") {
    auto res = client.put(moveit::network::url{"http://test/put"},
                          moveit::network::body{"update"}, {});
    CHECK(res.response == "{}");
  }

  SUBCASE("DELETE works with preconfigured response") {
    client.set_response("http://test/delete", R"({"deleted":true})");
    auto res = client.del(moveit::network::url{"http://test/delete"}, {});
    CHECK(res.response == R"({"deleted":true})");
  }
}
