#include <iostream>
#include <string>
#include "pstreambuf.h"

int main(int argc, char **argv) {
  if(argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <command>\n";
    return(127);
  }

  pstreambuf b { argv[1] };
  std::string line;
  std::istream is(&b);
  while(std::getline(is,line)) {
    std::cout << line << "\n";
  }

  return(0);
}

