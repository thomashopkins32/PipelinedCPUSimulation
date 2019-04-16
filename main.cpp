#include <iostream>
#include <vector>
#include "instruction.h"

void insert_and_replace(std::string& str, const std::string& sub, int i, int j) {
  memmove(&str[i], &str[i + j], strlen(str) - i);
  char tmp[128];
  strncpy(tmp, str, i);
  tmp[i] = '\0';
  strcat(tmp, sub);
  strcat(tmp, str + i);
  strcpy(str, tmp);
}

int main(int argc, char* argv[]) {
  std::vector<instruction> lines;

  // Starting simulation assuming that all instructions are ready
  int pc = 1; // Program counter, used for reading one line at a time
  for(int i = 0; i < 16; ++i) {
    for(int j = 0; j < pc; ++j) {
      int k = 0; // index at where to insert stage value
      int value = 0; // value of executed instruction
      int tmp = lines[j].skip; // used to determine k value
      while(tmp != 0) {
        ++k;
        if(list[j]->output[k] == '.')
          --tmp;
      }
      // IF stage
      // instructions are fetched and created here
      if(lines[j].getStage() == 0) {
        lines[j].output.insert(k, "IF");
        lines[j].output.erase(k+2, 1);
      }
      // ID stage
      // data and control hazards are resolved here
      else if(lines[j].getStage() == 1) {
        lines[j].output.insert(k, "ID");
        lines[j].output.erase(k+2, 1);
      }
      // EX stage
      // computation is made here
      else if(lines[j].getStage() == 2) {
        lines[j].output.insert(k, "EX");
        lines[j].output.erase(k+2, 1);
        std::string r1 = lines[j].dependencies[1];
        std::string r2 = lines[j].dependencies[2];
        switch(lines[j].type) {
          case "add":
            value = registers[r1] + registers[r2]; break;
          case "addi":
            value = registers[r1] + atoi(r2); break;
          case "or":
            value = registers[r1] | registers[r2]; break;
          case "ori":
            value = registers[r1] | atoi(r2); break;
          case "and":
            value = registers[r1] & registers[r2]; break;
          case "andi":
            value = registers[r1] & atoi(r2); break;
          case "slt":
            break;
          case "slti":
            break;
          case "beq":
            break;
          default:
            break;
        }
        if(lines[j].type == "add") {
          value = registers[r1] + regsiters[r2];
        }
        else if(lines[j].type == "addi") {
          value = registers[r1] + atoi(r2);
        }
        else if(lines[j].type == "or") {
          value = registers[r1] | registers[r2];
        }
        else if(lines[j].type == "ori") {
          value = registers[r1] | atoi(r2);
        }
        else if(lines[j].type == "and") {
          value = registers[r1] & registers[r2];
        }
        else if(lines[j].type == "andi") {
          value = registers[r1] & atoi(r2)
        }
        else if(lines[j].type == "slt") {

        }
        else if(lines[j].type == "slti") {

        }
        else if(lines[j].type == "beq") {

        }
      }
      // MEM stage
      // memory is written here
      else if(lines[j].getStage() == 3) {
        lines[j].output.insert(k, "MEM");
        lines[j].output.erase(k+2, 1);
      }
      // WB stage
      // registers are written here
      else if(lines[j].getStage() == 4) {
        lines[j].output.insert(k, "WB");
        lines[j].output.erase(k+2, 1);
        lines[j].dependencies[0] = value;
      }
    }
    // output all registers and their values
    outputRegisters(registers);
  }
  return EXIT_SUCCESS;
}
