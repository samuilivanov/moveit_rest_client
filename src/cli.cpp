#include "cli.h"
#include "cpr_http_client.h"
#include "moveit_client.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>
namespace core {

void cli::run(const std::string &username, const std::string &filePath) {

  // Prompt password securely
  std::string password = prompt_password();
  // TODO (samuil) this needs to be refactored it's as is just for a fast test
  try {
    auto token = m_moveit_client->authenticate(username, password);
    auto id = m_moveit_client->get_home_folder(token.access_token);
    m_moveit_client->upload_file(filePath);
    std::cout << token.access_token << " : " << id << "\n";

    // TODO (samuil) this goes the rest logic
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
  }
}

// Masked password input (Linux-only)
std::string cli::prompt_password() {
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
} // namespace core