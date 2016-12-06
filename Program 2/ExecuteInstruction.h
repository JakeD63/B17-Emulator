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
	//public functions, one per opcode
	void halt(); //halts execution
	void LD(instruction i); //load
	void ST(instruction i); //store
	void EM(instruction i); //exchange memory
	void LDX(instruction i); //load into register
	void STX(instruction i); //store into register
	void EMX(instruction i); //exchange register with memory
	void ADD(instruction i); //add to AC
	void SUB(instruction i); //sub from AC
	void CLR(); //clear the AC
	void COM(); //complement the AC
	void AND(instruction i); //and the AC
	void OR(instruction i); //or the AC
	void XOR(instruction i); //xor the AC
	void ADDX(instruction i); //add to ac from memory
	void SUBX(instruction i); //sub memory from ac
	void CLRX(instruction i); //clear specified register
	bool J(instruction i); //jump
	bool JZ(instruction i);//jump if ac is 0
	bool JN(instruction i); //jump if ac is negative
	bool JP(instruction i); //jump if ac is positive
	void printInstruction(instruction i); //print details of instruction for trace
	void printRegisters(); //print contents of AC and 4 index registers
};

#endif