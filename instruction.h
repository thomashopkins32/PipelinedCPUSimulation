#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <vector>
#include <string>

class instruction {
public:
  instruction(const std::string& line);

private:
  std::vector<char> dependencies;
  std::vector<int> regNum;
  std::string type;
  std::string output;
  int stage;
};

#endif
