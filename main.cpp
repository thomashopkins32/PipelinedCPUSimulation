#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <map>
#include "instruction.h"

void insertNop(std::vector<Instruction> &lines, int i, int skip, int stage)
{
  Instruction nop("nop");
  nop.skip = skip;
  nop.stage = stage;
  nop.isNop = true;
  lines.insert(i, nop);
}
/* Outputs the registers in a table like format */
void outputRegisters(std::map<std::string, int>& regs) {
  std::cout << "\n";
  for(int i = 0; i < 8; ++i) {
    std::string tmp = "$s" + std::string(i);
    std::ostringstream ss;
    ss << tmp << " = " << regs[tmp];
    std::cout << std::left << std::setw(20) << ss.str();
    if(i % 4 == 0)
      std::cout << std::endl;
  }
  for(int i = 0; i < 10; ++i) {
    std::string tmp = "$t" + std::string(i);
    std::ostringstream ss;
    ss << tmp << " = " << regs[tmp];
    std::cout << std::left << std::setw(20) << ss.str();
    if(i % 4 == 0)
      std::cout << std::endl;
  }
  std::cout << std::endl;
}

/* Prints the header of each CPU cycle */
void initialPrint() {
  std::cout << "----------------------------------------------------------------------------------\n";
  std::cout << std::left << std::setw(20) << "CPU Cycles ===>";
  for(int j = 1; j <= 16; ++j)
    std::cout << std::setw(4) << j;
  std::cout << std::endl;
}

void initRegisters(std::map<std::string, int>& regs) {
  for(int i = 0; i < 8; ++i) {
    std::string tmp = "$s" + std::to_string(i);
    regs[tmp] = 0;
  }
  for(int i = 0; i < 10; ++i) {
    std::string tmp = "$t" + std::to_string(i);
    regs[tmp] = 0;
  }
}

int main(int argc, char* argv[]) {
  std::vector<Instruction> lines; // contains each instruction (including nops)
  std::map<std::string, int> registers; // contains registers and their values
  // Take input from file
  

  // initialize register map
  initRegisters(registers);
  // Decide forwarding argument
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
    initialPrint();
    // Loop up to current instruction
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
        lines[j].output.erase(k, 2);
      }
      // ID stage
      // data and control hazards are resolved here
      else if(lines[j].stage == 1) {
        lines[j].output.insert(k, "ID");
        lines[j].output.erase(k, 2);
      }
      // EX stage
      // computation is made here
      else if(lines[j].stage == 2) {
        lines[j].output.insert(k, "EX");
        lines[j].output.erase(k, 2);
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
          if(registers[r1] < registers[r2])
            value = 1;
        }
        else if(lines[j].type == "slti") {
          if(registers[r1] < stoi(r2))
            value = 1;
        }
        else if(lines[j].type == "beq") {
          r1 = lines[j].dependencies[0];
          r2 = lines[j].dependencies[1];
          if(registers[r1] == registers[r2])
            value = 1;
        }
      }
      // MEM stage
      // memory is written here
      else if(lines[j].stage == 3) {
        lines[j].output.insert(k, "MEM");
        lines[j].output.erase(k, 3);
      }
      // WB stage
      // registers are written here
      else if(lines[j].stage == 4) {
        lines[j].output.insert(k, "WB");
        lines[j].output.erase(k, 2);
        if(lines[j].type != "beq")
          lines[j].dependencies[0] = value;
        else { // execute jump

        }
      }
    }
    // output all registers and their values
    outputRegisters(registers);
  }
  std::cout << "END OF SIMULATION" << std::endl;
  return EXIT_SUCCESS;
}
