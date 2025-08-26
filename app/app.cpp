#include "cli.h"
#include "cpr_http_client.h"
#include "moveit_client.h"
#include <cxxopts.hpp>
#include <iostream>
#include <string>
#include <string_view>

namespace {

enum class app_mode { SERVER, CLI, UNKNOWN };

void run_cli(const std::string &user, const std::string &url,
             const std::string &file) {
  std::unique_ptr<core::moveit_client> mv_client =
      std::make_unique<core::moveit_client>(
          std::make_unique<network::cpr_http_client>(), url);
  core::cli cli{std::move(mv_client)};
  cli.run(user, file);
}

void start_server([[maybe_unused]] int port) {}

} // namespace

int main(int argc, char *argv[]) {
  try {
    cxxopts::Options options("moveit_client", "Server/CLI dual mode app");

    options.add_options()("mode", "Mode: server or cli",
                          cxxopts::value<std::string>())(
        "user", "Username (CLI only)",
        cxxopts::value<std::string>()->default_value(""))(
        "url", "Server URL (CLI only)",
        cxxopts::value<std::string>()->default_value(""))(
        "file", "File to upload (CLI only)",
        cxxopts::value<std::string>()->default_value(""))(
        "port", "Port to run server on (Server only)",
        cxxopts::value<int>()->default_value("8080"))("h,help", "Print usage");

    auto result = options.parse(argc, argv);

    if (result.count("help") || !result.count("mode")) {
      std::cout << options.help() << "\n";
      return 0;
    }

    std::string mode_str = result["mode"].as<std::string>();
    app_mode mode = app_mode::UNKNOWN;

    if (mode_str == "server") {
      mode = app_mode::SERVER;
    } else if (mode_str == "cli") {
      mode = app_mode::CLI;
    }

    switch (mode) {
    case app_mode::SERVER: {
      int port = result["port"].as<int>();
      std::cout << "Note: Server is not implemented. Starting server on port " << port << "...\n";
      // TODO (samuil) eventually implement this using crow
      start_server(port);
      break;
    }
    case app_mode::CLI: {
      std::string user = result["user"].as<std::string>();
      std::string url = result["url"].as<std::string>();
      std::string file = result["file"].as<std::string>();

      if (user.empty() || url.empty() || file.empty()) {
        std::cerr << "CLI mode requires --user, --url, and --file\n";
        return 1;
      }

      std::cout << "Running CLI:\n"
                << "  User: " << user << "\n"
                << "  URL:  " << url << "\n"
                << "  File: " << file << "\n";

      run_cli(user, url, file);
      break;
    }
    default:
      std::cerr << "Unknown mode. Use --mode=server or --mode=cli\n";
      return 1;
    }
  } catch (const cxxopts::exceptions::parsing &e) {
    std::cerr << "Error parsing options: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
