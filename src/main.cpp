#include <sys/wait.h>
#include <unistd.h>

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

enum class Command { UNKNOWN, EXIT, ECHO, TYPE, EXTERNAL };

std::vector<std::string> split(std::string& input, char delimiter);
Command getCommand(std::string cmd_str);
bool ProcessCommandPath(const std::string& cmd_str, std::string& fullPath);
void execCommand(const std::string& fullPath,
                 const std::vector<std::string> args);

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

Command getCommand(std::string cmd_str) {
  std::string str;
  if (cmd_str == "exit") {
    return Command::EXIT;
  } else if (cmd_str == "echo") {
    return Command::ECHO;
  } else if (cmd_str == "type") {
    return Command::TYPE;
  } else if (ProcessCommandPath(cmd_str, str)) {
    return Command::EXTERNAL;
  }
  return Command::UNKNOWN;
}

bool ProcessCommandPath(const std::string& cmd_str, std::string& fullPath) {
  const char* envPath = std::getenv("PATH");

  if (envPath == nullptr) return false;

  std::string pathvar = envPath;

  auto paths = split(pathvar, ':');

  for (const auto& path : paths) {
    fullPath = path + "/" + cmd_str;
    if (access(fullPath.c_str(), F_OK | X_OK) == 0) return true;
  }

  return false;
}

void execCommand(const std::string& fullPath,
                 const std::vector<std::string> args) {
  pid_t pid = fork();

  if (pid == 0) {
    std::vector<char*> argv;

    for (const std::string& arg : args) {
      argv.push_back(const_cast<char*>(arg.c_str()));
    }

    argv.push_back(nullptr);
    execv(fullPath.c_str(), argv.data());

    _exit(1);
  } else {
    waitpid(pid, nullptr, 0);
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
    std::string fullPath;
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
    } else if (cmd == Command::ECHO) {
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
        if (ProcessCommandPath(cmd_var, fullPath)) {
          std::cout << cmd_var << " is " << fullPath;
        } else {
          std::cout << cmd_var << ": not found";
        }
      }
    } else if (cmd == Command::EXTERNAL) {
      if (ProcessCommandPath(args[0], fullPath)) {
        execCommand(fullPath, args);
      }
    } else {
      std::cout << input << ": " << printErr;
    }
    std::cout << std::endl;
  }
}
