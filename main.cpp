#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <map>
#include <algorithm>
#include <fstream>
#include <cstring>
#include "instruction.h"

void insertNop(std::vector<Instruction> &lines, int i, int skip, int stage)
{
  Instruction nop("nop");
  nop.skip = skip;
  nop.stage = stage;
  nop.isNop = true;
  lines.insert(lines.begin() + i - 1, nop);
}
//finds register index, -1 if not in array
int findRegister(const std::vector<std::string> inUse, std::string reg)
{
  for (unsigned int i = 0; i < inUse.size(); i++)
  {
    if (inUse[i] == reg)
    {
      return i;
    }
  }
  return -1;
}
/* Outputs the registers in a table like format */
void outputRegisters(std::map<std::string, int>& regs) {
  std::cout << "\n";
  for(int i = 0; i < 8; ++i) {
    std::string tmp = "$s" + std::to_string(i);
    std::ostringstream ss;
    ss << tmp << " = " << regs[tmp];
    std::cout << std::left << std::setw(20) << ss.str();
    if((i+1) % 4 == 0)
      std::cout << std::endl;
  }
  for(int i = 0; i < 10; ++i) {
    std::string tmp = "$t" + std::to_string(i);
    std::ostringstream ss;
    ss << tmp << " = " << regs[tmp];
    std::cout << std::left << std::setw(20) << ss.str();
    if((i+1) % 4 == 0)
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
  regs["$zero"] = 0;
}

int main(int argc, char* argv[]) {
  std::vector<Instruction> lines; // contains each instruction (including nops)
  std::map<std::string, int> registers; // contains registers and their values
  std::vector<std::string> inUse;
  // Take input from file
  if (argc != 3) {
    std::cerr << "Usage: ./a.out [mode] [filename]" << std::endl;
    return 1;
  }
  std::ifstream inputFile(argv[2]);
  if(!inputFile) {
    std::cerr << "Error with input file" << std::endl;
    return 1;
  }
  for (std::string line; std::getline(inputFile, line);) {
    Instruction tempInst(line); //Initialize instruction object with line info
    lines.push_back(tempInst);
  }

  // initialize register map
  initRegisters(registers);
  // Decide forwarding argument
  bool forwarding = false;
  if(strcmp(argv[1], "F") == 0)
    forwarding = true;
  // initialize stages
  int count = 0;
  for(unsigned i = 0; i < lines.size(); ++i) {
    if(!lines[i].isLabel)
      ++count;
    // lines[i].stage -= count-1;
    lines[i].skip += count-1;
  }
  for(unsigned i = 0; i < lines.size(); ++i) {
    lines[i].printInstInfo();
  }
  // Starting simulation assuming that all instructions are ready
  std::cout << "START OF SIMULATION";
  if(forwarding)
    std::cout << " (forwarding)" << std::endl;
  else
    std::cout << " (no forwarding)" << std::endl;
  //
  unsigned pc = 1; // Program counter, used for reading one line at a time
  for(int i = 0; i < 16; ++i) {
    initialPrint();
    // Loop up to current instruction
    for(unsigned j = 0; j < pc; ++j) {
      int k = 0; // index at where to insert stage value
      int tmp = lines[j].skip; // used to determine k lines[j].value
      while(tmp != -1) {
        ++k;
        if(lines[j].output[k] == '.')
          --tmp;
      }
      // IF stage
      // instructions are fetched and created here
      if(lines[j].stage == 0) {
        lines[j].output[k] = 'I';
        lines[j].output[k+1] = 'F';
      }
      // ID stage
      // data and control hazards are resolved here
      else if(lines[j].stage == 1) {
        lines[j].output[k] = 'I';
        lines[j].output[k+1] = 'D';

        //check for dependencies
        bool dependFound = false;
        for (unsigned int l = 0; l < inUse.size(); l++)
        {
          if (inUse[l] == lines[j].dependencies[1] || inUse[l] == lines[j].dependencies[2])
          {
            dependFound = true;
          }
        }
        if (dependFound)
        {
          insertNop(lines, i, lines[j].skip, 2);
          lines[j].stage--;
        }
      }
      // EX stage
      // computation is made here
      else if(lines[j].stage == 2) {
        //add product register to use to potential dependencies
        if(lines[j].type != "beq") 
        {
          inUse.push_back(lines[j].dependencies[0]);
        }
        if(!lines[j].isNop) {
          lines[j].output[k] = 'E';
          lines[j].output[k+1] = 'X';
          std::string r1 = lines[j].dependencies[1];
          std::string r2 = lines[j].dependencies[2];
          if(lines[j].type == "add")
            lines[j].value = registers[r1] + registers[r2];
          else if(lines[j].type == "addi")
            lines[j].value = registers[r1] + stoi(r2);
          else if(lines[j].type == "or")
            lines[j].value = registers[r1] | registers[r2];
          else if(lines[j].type == "ori")
            lines[j].value = registers[r1] | stoi(r2);
          else if(lines[j].type == "and")
            lines[j].value = registers[r1] & registers[r2];
          else if(lines[j].type == "andi")
            lines[j].value = registers[r1] & stoi(r2);
          else if(lines[j].type == "slt") {
            if(registers[r1] < registers[r2])
              lines[j].value = 1;
          }
          else if(lines[j].type == "slti") {
            if(registers[r1] < stoi(r2))
              lines[j].value = 1;
          }
          else if(lines[j].type == "beq") {
            r1 = lines[j].dependencies[0];
            r2 = lines[j].dependencies[1];
            if(registers[r1] == registers[r2])
              lines[j].value = 1;
          }
        }
        else // is a nop instruction
          lines[j].output[k] = '*';
      }
      // MEM stage
      // memory is written here
      else if(lines[j].stage == 3) {
        if(!lines[j].isNop) {
          lines[j].output[k] = 'M';
          lines[j].output[k+1] = 'E';
          lines[j].output[k+2] = 'M';
        }
        else
          lines[j].output[k] = '*';
      }
      // WB stage
      // registers are written here
      else if(lines[j].stage == 4) {
        if(!lines[j].isNop) {
          lines[j].output[k] = 'W';
          lines[j].output[k+1] = 'B';
          if(lines[j].type != "beq")
            registers[lines[j].dependencies[0]] = lines[j].value;
          else { // execute jump

          }
        }
        else
          lines[j].output[k] = '*';
        //remove product register from inUse
        int regIndex = findRegister(inUse, lines[j].dependencies[0]);
        //if this line causes a seg fault it's because it's trying to remove 
        //somthing that's not there, that shouldn't happen so I didn't make an if to check
        //so if it does we know that something fucked up
        inUse.erase(inUse.begin() + regIndex);
      }
      std::cout << lines[j].output;
      ++lines[j].stage;
    }
    // output all registers and their values
    if(pc < lines.size())
      ++pc;
    outputRegisters(registers);
    if(lines[lines.size()-1].stage == 5)
      break;
  }
  std::cout << "----------------------------------------------------------------------------------\n";
  std::cout << "END OF SIMULATION" << std::endl;
  return EXIT_SUCCESS;
}
