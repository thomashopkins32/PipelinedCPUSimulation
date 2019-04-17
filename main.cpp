#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "instruction.h"

void outputRegisters(const std::map<std::string, int>& regs) {

}

int main(int argc, char* argv[]) {
  std::vector<Instruction> lines; // contains each instruction (including nops)
  std::map<std::string, int> registers; // contains registers and their values
  bool forwarding = false;
  if(argv[1] == "F")
    forwarding = true;
  // Starting simulation assuming that all instructions are ready
  std::cout << "START OF SIMULATION";
  if(forwarding)
    std::cout << " (forwarding)" << std::endl;
  else
    std::cout << " (no forwarding)" << std::endl;
  int pc = 1; // Program counter, used for reading one line at a time
  for(int i = 0; i < 16; ++i) {
    for(int j = 0; j < pc; ++j) {
      int k = 0; // index at where to insert stage value
      int value = 0; // value of executed instruction
      int tmp = lines[j].skip; // used to determine k value
      while(tmp != 0) {
        ++k;
        if(lines[j].output[k] == '.')
          --tmp;
      }
      // IF stage
      // instructions are fetched and created here
      if(lines[j].stage == 0) {
        lines[j].output.insert(k, "IF");
        lines[j].output.erase(k, 1);
      }
      // ID stage
      // data and control hazards are resolved here
      else if(lines[j].stage == 1) {
        lines[j].output.insert(k, "ID");
        lines[j].output.erase(k, 1);
      }
      // EX stage
      // computation is made here
      else if(lines[j].stage == 2) {
        lines[j].output.insert(k, "EX");
        lines[j].output.erase(k, 1);
        std::string r1 = lines[j].dependencies[1];
        std::string r2 = lines[j].dependencies[2];
        if(lines[j].type == "add")
          value = registers[r1] + registers[r2];
        else if(lines[j].type == "addi")
          value = registers[r1] + stoi(r2);
        else if(lines[j].type == "or")
          value = registers[r1] | registers[r2];
        else if(lines[j].type == "ori")
          value = registers[r1] | stoi(r2);
        else if(lines[j].type == "and")
          value = registers[r1] & registers[r2];
        else if(lines[j].type == "andi")
          value = registers[r1] & stoi(r2);
        else if(lines[j].type == "slt") {
          value = 0;
        }
        else if(lines[j].type == "slti") {
          value = 0;
        }
        else if(lines[j].type == "beq") {
          value = 0;
        }
      }
      // MEM stage
      // memory is written here
      else if(lines[j].stage == 3) {
        lines[j].output.insert(k, "MEM");
        lines[j].output.erase(k, 1);
      }
      // WB stage
      // registers are written here
      else if(lines[j].stage == 4) {
        lines[j].output.insert(k, "WB");
        lines[j].output.erase(k, 1);
        lines[j].dependencies[0] = value;
      }
    }
    // output all registers and their values
    outputRegisters(registers);
  }
  std::cout << "END OF SIMULATION" << std::endl;
  return EXIT_SUCCESS;
}
