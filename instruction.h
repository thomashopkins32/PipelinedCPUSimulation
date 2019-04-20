#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <vector>
#include <iostream>
#include <string>

class Instruction {
public:
  //constructor
  Instruction(const std::string& line);
  //debugging helper
  void printInstInfo();
  //member variable (keep public)
  std::vector<std::string> dependencies;
  std::string type;
  std::string output;
  std::string line;
  int skip;
  int stage;
  int value;
  bool isNop;
  bool isLabel;
};

bool operator==(const Instruction& a, const Instruction& b);

#endif
