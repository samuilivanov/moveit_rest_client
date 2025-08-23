#include "cli.h"
#include "cpr_http_client.h"
#include "moveit_client.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>
namespace core {

void cli::run(int argc, const char *argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <username> <file-path>\n";
    exit(1);
  }

  std::string username = argv[1];
  std::string filePath = argv[2];

  // Prompt password securely
  std::string password = prompt_password();
  // TODO (samuil) this needs to be refactored it's as is just for a fast test
  std::unique_ptr<network::http_client> t =
      std::make_unique<network::cpr_http_client>();
  moveit_client client{
      std::move(t),
      "https://testserver.moveitcloud.com/"}; // TODO (samuil) the base url
                                              // should be passed with on start
  try {
    auto token = client.authenticate(username, password);
    auto id = client.get_home_folder(token.access_token);
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