#include "instruction.h"
#include <iostream>
#include <sstream>
#include <iomanip>

Instruction::Instruction(const std::string& line) {
  this->type = "";
  this->output = "";
  this->stage = 0;
  this->skip = 0;
  this->isNop = false;
  // Get the current operation and store it in instruction object
  // and assign operation to instruction type
  int i = 0;
  while (line[i] != ' ' && line[i] != ':') {
    this->type += line[i];
    i++;
  }
  if(line[i] == ':') return; // handles tag case
  while(line[i] == ' ') ++i; // continue along line
  while(i < line.length()) {
    std::string tmp = "";
    //Get all registers in the line
    while(line[i] != ' ' && line[i] != '\n' && line[i] != ',') {
        tmp += line[i];
        ++i;
    }
    this->dependencies.push_back(tmp);
    ++i;
  }
  std::ostringstream stringstream;
  stringstream << std::left << std::setw(20) << line;
  for(int i = 0; i < 16; ++i)
    stringstream << std::setw(4) << ".";
  stringstream << "\n";
  this->output =  stringstream.str();
}

//Helper function to print the Instruction info
void Instruction::printInstInfo() {
  std::cout << "OUTPUT: " << this->output << std::endl;
  std::cout << "DEPENDENCIES: ";
  for (int i = 0; i < this->dependencies.size(); i++) {
    if (i != this->dependencies.size() - 1)
      std::cout << this->dependencies[i] << ", ";
    else
      std::cout << this->dependencies[i];
  }
  std::cout << "\nTYPE: " << this->type << std::endl;
  std::cout << "STAGE: " << this->stage << std::endl;
  std::cout << "SKIP: " << this->skip << std::endl;
}
