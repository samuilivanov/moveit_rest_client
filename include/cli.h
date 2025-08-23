#ifndef MOVEIT_CLI_H
#define MOVEIT_CLI_H
#include "moveit_client.h"
#include <string>

namespace core {

class cli {
private:
  std::string
  prompt_password(); // TODO (samuil) this is fine for now but when asking for
                     // password make sure to lock memory and clear it after
                     // possible to make it a void return and out param
  std::unique_ptr<moveit_client> m_moveit_client;

public:
  explicit cli(std::unique_ptr<moveit_client> moveit_client)
      : m_moveit_client(std::move(moveit_client)) {}

  void run(const std::string &username,
           const std::string &filePath); // TODO (samuil) the pamareters might
                                         // be changed think of a dep injection
};
} // namespace core

#endif