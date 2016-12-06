#ifndef INSTRUCTIONS_H
#define INSTRUCTIIONS_H

#include <string>
#include <iostream>
#include <iomanip>
#include "globals.h"
#include "const.h"

using namespace std;

class ExecuteInstruction {
public:
	void halt();
	void LD(instruction i);
	void ST(instruction i);
	void EM(instruction i);
	void LDX(instruction i);
	void STX(instruction i);
	void EMX(instruction i);
	void ADD(instruction i);
	void SUB(instruction i);
	void CLR();
	void COM();
	void AND(instruction i);
	void OR(instruction i);
	void XOR(instruction i);
	void ADDX(instruction i);
	void SUBX(instruction i);
	void CLRX(instruction i);
	bool J(instruction i);
	bool JZ(instruction i);
	bool JN(instruction i);
	bool JP(instruction i);
	void printInstruction(instruction i);
	void printRegisters();
};

#endif