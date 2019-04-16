#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <vector>
#include <iostream>
#include <string>

class Instruction {
public:
  Instruction(const std::string& line);
  void addToDependencies(const std::string & reg);
  void addToRegNums(const int num);
  void setType(const std::string & newType);
  void printInstInfo();

private:
  std::vector<std::string> dependencies;
  std::vector<int> regNum;
  std::string type;
  std::string output;
  int stage;
};

#endif
