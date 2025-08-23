#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "mock_http_client.h"
#include "moveit_client.h"
#include <memory>

TEST_CASE("moveit_client login success returns token") {
  std::string fakeLoginResponse = R"({ "access_token": "abc123" })";
  auto mock = std::make_unique<network::mock_http_client>();

  mock->set_response("https://your-transfer-server/api/v1/token",
                     fakeLoginResponse);

  core::moveit_client client(std::move(mock), "https://your-transfer-server");

  auto token = client.authenticate("user", "pass");
  CHECK(token.access_token == "abc123");
}

TEST_CASE("moveit_client get_home_folder returns folderId") {
  std::string fakeFolderResponse = R"({ "homeFolderID": 42 })";

  auto mock = std::make_unique<network::mock_http_client>();

  mock->set_response("https://your-transfer-server/api/v1/users/self",
                     fakeFolderResponse);

  core::moveit_client client(std::move(mock), "https://your-transfer-server");

  int folderId = client.get_home_folder("abc123");
  CHECK(folderId == 42);
}

TEST_CASE("moveit_client uploadFile returns fileId" *
          doctest::skip("not implemented")) {
  //
}

// NOTE (samuil) at the moment I am not 100% sold on this error handling and the
// usefulness of these tests
TEST_CASE("moveit_client authenticate - HTTP error") {
  auto mock = std::make_unique<network::mock_http_client>();
  mock->mode = network::mock_http_client::mode::http_error;
  core::moveit_client client(std::move(mock), "https://fake-server/api/v1");

  CHECK_THROWS_AS(client.authenticate("alice", "secret"), std::runtime_error);
}

TEST_CASE("moveit_client authenticate - API error") {
  auto mock = std::make_unique<network::mock_http_client>();
  mock->mode = network::mock_http_client::mode::api_error;
  core::moveit_client client(std::move(mock), "https://fake-server/api/v1");

  CHECK_THROWS_AS(client.authenticate("alice", "wrongpass"),
                  std::runtime_error);
}

TEST_CASE("moveit_client getUserFolder - HTTP error") {
  auto mock = std::make_unique<network::mock_http_client>();
  mock->mode = network::mock_http_client::mode::http_error;
  core::moveit_client client(std::move(mock), "https://fake-server/api/v1");

  CHECK_THROWS_AS(client.get_home_folder("FAKE_TOKEN"), std::runtime_error);
}

TEST_CASE("moveit_client getUserFolder - API error") {
  auto mock = std::make_unique<network::mock_http_client>();
  mock->mode = network::mock_http_client::mode::api_error;
  core::moveit_client client(std::move(mock), "https://fake-server/api/v1");

  CHECK_THROWS_AS(client.get_home_folder("FAKE_TOKEN"), std::runtime_error);
}