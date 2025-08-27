#include "cli.h"
#include "cpr_http_client.h"
#include "moveit_client.h"
#include "util.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <variant>

namespace {
// Masked password input (Linux-only)
[[nodiscard]] std::string prompt_password() {
  std::cout << "Enter password: ";
  termios oldt{};
  tcgetattr(STDIN_FILENO, &oldt);

  termios newt = oldt;
  newt.c_lflag &= ~ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  std::string password;
  std::getline(std::cin, password);

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  std::cout << "\n";
  return password;
}
} // namespace

namespace moveit::core {

void cli::run(const std::string &username,
              const std::filesystem::path &filePath) {

  // Prompt password securely
  std::string password = prompt_password();
  try {
    std::string access_token{};
    auto token = m_moveit_client->authenticate(username, password);

    if (std::holds_alternative<network::auth_response>(token)) {
      const auto &resp = std::get<network::auth_response>(token);
      access_token = resp.access_token;
    } else {
      throw std::runtime_error("Authentication failed: " +
                               std::get<network::auth_error>(token).error);
    }

    auto user_info = m_moveit_client->get_home_folder(access_token);
    int user_folder_id{-1};
    if (std::holds_alternative<network::user_info_response>(user_info)) {
      const auto &resp = std::get<network::user_info_response>(user_info);
      user_folder_id = resp.homeFolderID;
    } else {
      throw std::runtime_error(
          "User folder info request failed (status " +
          std::get<network::error_response>(user_info).detail);
    }

    auto result_from_upload =
        m_moveit_client->upload_file(filePath, access_token, user_folder_id);

    if (std::holds_alternative<network::upload_response>(result_from_upload)) {
      std::cout << "File: " << filePath << " was uploaded\n";
    } else {
      throw std::runtime_error(
          "Upload failed " +
          std::get<network::error_response>(result_from_upload).detail);
    }
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
  }
}

} // namespace moveit::core