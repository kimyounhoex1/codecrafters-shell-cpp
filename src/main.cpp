#include <iostream>
#include <string>
#include <vector>
#include <sstream>

std::vector<std::string> split(std::string& input, char delimiter = ' ') {
  std::vector<std::string> tokens;
  std::istringstream iss(input);
  std::string token;
  while(std::getline(iss, token, delimiter)) {
    if(!token.empty()) {
      tokens.push_back(token);
    }
  }

  return tokens;
}


int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::string printErr = "command not found";
  uint64_t idx;
  // TODO: Uncomment the code below to pass the first stage
  while(true){
    std::cout << "$ ";
    std::string input;
    idx = 0;
    if(!std::getline(std::cin, input))
      break;
    
    if(input.empty())
      continue;
    
    auto args = split(input);

    std::string builtin = args[idx++];

    if(builtin == "exit" && args.size() > 1) {
      if(args[1] == "1") {
        return 1;
      }
      else if(args[1] == "0") {
        return 0;
      } 
    }

    else if(builtin == "echo") {
      std::vector<std::string> printEcho;
      for(; idx < args.size(); idx++) {
        std::cout << args[idx] << " ";
      }
    }
    else 
      std::cout << input << ": " << printErr;
    
    std::cout << std::endl;
  }
}
