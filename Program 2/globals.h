//Global variable definitions. These are used throughout the program for global storage
#ifndef GLOBALS_H
#define GLOBALS_H

#include "const.h"
#include <vector>

//list of instructions
extern vector<instruction> instructions;
//register definitions
extern int X0, X1, X2, X3;
extern int MAR; //memory address register
extern vector<instruction>::iterator instructionRegister; //instructionRegister, contains pointer to current instruction
extern int ABUS; //Address bus
extern int MDR; //Memory data register
extern int AC; //accumulator
extern int DBUS; //Data Bus
//main memory, holds 4096 words
extern int memory[4096];

#endif