#include "instruction.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstring>

Instruction::Instruction(const std::string& line) {
  this->type = "";
  this->output = "";
  this->stage = 0;
  this->skip = 0;
  this->line = line;
  this->value = 0;
  this->isNop = false;
  this->isLabel = false;
  // Get the current operation and store it in instruction object
  // and assign operation to instruction type
  int i = 0;
  while (line[i] != ' ' && line[i] != ':') {
    this->type += line[i];
    i++;
  }
  // handles tag case
  if(line[i] == ':') {
    this->isLabel = true;
    return;
  }
  while(line[i] == ' ') ++i; // continue along line
  //Get all registers in the line
  std::string tmp = "";
  for(unsigned j = i; j < line.length(); ++j) {
    if(line[j] == ',' || line[j] == '\n' || line[j] == '\r') {
      this->dependencies.push_back(tmp);
      tmp = "";
    }
    else
      tmp += line[j];
  }
  this->dependencies.push_back(tmp);
  //Generate output line
  std::ostringstream stringstream;
  stringstream << std::left << std::setw(20) << line;
  for(int i = 0; i < 16; ++i) {
    if(i == 15)
      stringstream << ".";
    else
      stringstream << std::setw(4) << ".";
  }
  stringstream << "\n";
  this->output =  stringstream.str();
}

Instruction::Instruction(const Instruction& instr) {
  this->output = instr.output;
  this->dependencies = instr.dependencies;
  this->type = instr.type;
  this->line = instr.line;
  this->skip = instr.skip;
  this->stage = instr.stage;
  this->value = instr.value;
  this->isNop = instr.isNop;
  this->isLabel = instr.isLabel;
}

//Helper function to print the Instruction info
void Instruction::printInstInfo() {
  std::cout << "OUTPUT: " << this->output;
  std::cout << "DEPENDENCIES: ";
  for (unsigned i = 0; i < this->dependencies.size(); i++)
    std::cout << this->dependencies[i] << " ";
  std::cout << "\nTYPE: " << this->type << std::endl;
  std::cout << "STAGE: " << this->stage << std::endl;
  std::cout << "SKIP: " << this->skip << std::endl;
}

bool operator==(const Instruction& a, const Instruction& b) {
  return strcmp(a.line.c_str(), b.line.c_str()) == 0;
}
