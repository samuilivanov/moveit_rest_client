#ifndef MOVEIT_CLI_H
#define MOVEIT_CLI_H
#include "moveit_client.h"
#include <string>

class cli {
private:
  std::string
  prompt_password(); // TODO (samuil) this is fine for now but when asking for
                     // password make sure to lock memory and clear it after
                     // possible to make it a void return and out param

public:
  cli(/* args */) {} // TODO (samuil) add the rest of the impl
  void run(int argc, const char *argv[]); // TODO (samuil) the pamareters might
                                          // be changed think of a dep injection
};

#endif