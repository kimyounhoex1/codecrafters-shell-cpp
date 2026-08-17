#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> split(std::string& input, char delimiter = ' ') {
  std::vector<std::string> tokens;
  std::istringstream iss(input);
  std::string token;
  while (std::getline(iss, token, delimiter)) {
    if (!token.empty()) {
      tokens.push_back(token);
    }
  }

  return tokens;
}

enum class Command { UNKNOWN, EXIT, ECHO, TYPE };

Command getCommand(std::string cmd_str) {
  if (cmd_str == "exit") {
    return Command::EXIT;
  } else if (cmd_str == "echo") {
    return Command::ECHO;
  } else if (cmd_str == "type") {
    return Command::TYPE;
  } else {
    return Command::UNKNOWN;
  }
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::string printErr = "command not found";
  uint64_t idx;
  // TODO: Uncomment the code below to pass the first stage
  while (true) {
    std::cout << "$ ";
    std::string input;
    idx = 0;
    if (!std::getline(std::cin, input)) break;

    if (input.empty()) continue;

    auto args = split(input);

    std::string builtin = args[idx++];

    Command cmd = getCommand(builtin);

    if (cmd == Command::EXIT) {
      if (args.size() > 1) {
        if (args[1] == "1") {
          return 1;
        } else if (args[1] == "0") {
          return 0;
        }
      } else {
        return 0;
      }
    }

    else if (cmd == Command::ECHO) {
      std::vector<std::string> printEcho;
      for (; idx < args.size(); idx++) {
        std::cout << args[idx] << " ";
      }
    } else if (cmd == Command::TYPE) {
      std::string cmd_var = args[idx];
      Command tar_cmd = getCommand(cmd_var);
      if (tar_cmd != Command::UNKNOWN) {
        std::cout << cmd_var << " is a shell builtin";
      } else {
        std::cout << cmd_var << ": not found";
      }
    } else {
      std::cout << input << ": " << printErr;
    }
    std::cout << std::endl;
  }
}
