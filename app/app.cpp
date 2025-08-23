#include "cli.h"
#include <iostream>
#include <string>

int main(int argc, char const *argv[]) {
  // TODO (samuil) creates the CLI and calls run
  // TODO (samuil) maybe add a web interface 
  // it should parse the arguments on start and run either cli or start a server
  core::cli app;
  app.run(argc, argv);
  return 0;
}
