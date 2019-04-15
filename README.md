Repository for CSCI 2500 Final Project.

#To-do list (put your name next to what you want to work on):

Please comment your code so we know what everything does.
Put

- Handling Input (currently working on this: no one)
  ! parse registers into an std::map<std::string, int>
  ! create an instruction object for each line in the input file
    (including labels)
  ! put each instruction in a vector of instructions
  ! set up instruction object with useful variables for storing information in
    a line
- Pipelining structure (currently working on this: Tom)
  ! create basic structure for pipeline
  ! implement each step of the pipeline (IF, ID, EX, MEM, WB)
- Handling Hazards (currently working on this: Jason)
  ! Data Hazards (exactly like homework 5)
  ! Control Hazards (jumping and branching)
  ! inserting nop's in correct places
- Implementing forwarding (currently working on this: no one)
  ! Used for data hazards to not require a stall
  ! easier than you think
- Output Formatting (currently working on this: no one)
  ! match output format with examples on pdf

Replace ! with + when you are done with these steps
! means unfinished
+ means finished

Design suggestions: