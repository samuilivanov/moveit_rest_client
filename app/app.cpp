#include "cli.h"
#include <iostream>
#include <string>

int main(int argc, char const *argv[]) {
  // TODO (samuil) creates the CLI and calls run
  core::cli app;
  app.run(argc, argv);
  return 0;
}
