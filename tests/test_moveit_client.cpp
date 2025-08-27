#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "mock_http_client.h"
#include "moveit_client.h"
#include <memory>

TEST_CASE("moveit_client login success returns token") {
  std::string fakeLoginResponse = R"({ "access_token": "abc123" })";
  auto mock = std::make_unique<moveit::network::mock_http_client>();

  mock->set_response("https://your-transfer-server/api/v1/token",
                     fakeLoginResponse);

  moveit::core::moveit_client client(std::move(mock),
                                     "https://your-transfer-server");

  auto response = client.authenticate("user", "pass");
  auto token = std::get<moveit::network::auth_response>(response);
  CHECK(token.access_token == "abc123");
}

TEST_CASE("moveit_client get_home_folder returns folderId") {
  std::string fakeFolderResponse = R"({ "homeFolderID": 42 })";

  auto mock = std::make_unique<moveit::network::mock_http_client>();

  mock->set_response("https://your-transfer-server/api/v1/users/self",
                     fakeFolderResponse);

  moveit::core::moveit_client client(std::move(mock),
                                     "https://your-transfer-server");

  auto response = client.get_home_folder("abc123");
  auto folderId =
      std::get<moveit::network::user_info_response>(response).homeFolderID;
  CHECK(folderId == 42);
}

TEST_CASE("moveit_client uploadFile returns fileId" *
          doctest::skip("not implemented")) {
  //
}

// NOTE (samuil) at the moment I am not 100% sold on this error handling and the
// usefulness of these tests
TEST_CASE("moveit_client authenticate - HTTP error") {
  auto mock = std::make_unique<moveit::network::mock_http_client>();
  mock->mode = moveit::network::mock_http_client::mode::http_error;
  moveit::core::moveit_client client(std::move(mock),
                                     "https://fake-server/api/v1");

  CHECK_THROWS_AS(client.authenticate("alice", "secret"), std::runtime_error);
}

// TEST_CASE("moveit_client authenticate - API error") {
//   auto mock = std::make_unique<moveit::network::mock_http_client>();
//   mock->mode = moveit::network::mock_http_client::mode::api_error;
//   moveit::core::moveit_client client(std::move(mock),
//                                      "https://fake-server/api/v1");

//   CHECK_THROWS_AS(client.authenticate("alice", "wrongpass"),
//                   std::runtime_error);
// }

TEST_CASE("moveit_client getUserFolder - HTTP error") {
  auto mock = std::make_unique<moveit::network::mock_http_client>();
  mock->mode = moveit::network::mock_http_client::mode::http_error;
  moveit::core::moveit_client client(std::move(mock),
                                     "https://fake-server/api/v1");

  CHECK_THROWS_AS(client.get_home_folder("FAKE_TOKEN"), std::runtime_error);
}

// TEST_CASE("moveit_client getUserFolder - API error") {
//   auto mock = std::make_unique<moveit::network::mock_http_client>();
//   mock->mode = moveit::network::mock_http_client::mode::api_error;
//   moveit::core::moveit_client client(std::move(mock),
//                                      "https://fake-server/api/v1");

//   CHECK_THROWS_AS(client.get_home_folder("FAKE_TOKEN"), std::runtime_error);
// }