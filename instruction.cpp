#include "instruction.h"

Instruction::Instruction(const std::string& line) {
  std::string temp = "";
  std::string operation = "";
  bool firstInstance = true;
  for (int i = 0; i < line.size(); i++) {
    //Get the current operation and store it in instruction object
    if (i == 0) {
      while (line[i] != ' ') {
        operation += line[i];
        if (line[i + 1] == ':') break; //handling "loop:" case
        i++;
      }
    }
    setType(operation); //assign operation to object type
    if (operation == "loop") {
      break;
    }
    //Get all registers in the line
    if (line[i] == '$' && line[i + 1] != 'z') { //if register (and not $zero)
      temp += line[i];
      temp += line[i + 1];
      temp += line[i + 2];
      addToRegNums(temp[2] - '0');
      if (!firstInstance) { //if we're not on the first register in the line
      addToDependencies(temp);
      }
    temp.clear(); //clear to keep reading new registers
    firstInstance = false;
    }
  }
}

//Add string to dependencies vector
void Instruction::addToDependencies(const std::string & reg) {
  dependencies.push_back(reg);
}

//Add integer to regNum vector
void Instruction::addToRegNums(const int num) {
  regNum.push_back(num);
}

//Set the type of the instruction
void Instruction::setType(const std::string & newType) {
  type = newType;
}

//Helper function to print the Instruction info
void Instruction::printInstInfo() {
  std::cout << "DEPENDENCIES: ";
  for (int i = 0; i < dependencies.size(); i++) {
    if (i != dependencies.size() - 1) std::cout << dependencies[i] << ", ";
    else std::cout << dependencies[i];
  }
  std::cout << "\nREGISTER NUMBERS: ";
  for (int i = 0; i < regNum.size(); i++) {
    if (i != regNum.size() - 1) std::cout << regNum[i] << ", ";
    else std::cout << regNum[i];
  }
  std::cout << "\nTYPE: " << type << std::endl;
}
