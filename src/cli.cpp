#include "cli.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>

void cli::run(int argc, const char *argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <username> <file-path>\n";
    exit(1);
  }

  std::string username = argv[1];
  std::string filePath = argv[2];

  // Prompt password securely
  std::string password = prompt_password();

  try {
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