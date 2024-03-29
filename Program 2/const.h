//Constants declarations. These constants are used throughout the program
#ifndef CONST_H
#define CONST_H

#include <map>
#include <string>
using namespace std;

//enum of different supported addressing modes
enum addrModes {
	Direct,
	Immediate,
	Indexed,
	Indirect,
	Indexed_Indrect, //not implemented, as per instructions
	Illegal
};

//enum of all implemented op codes
enum opCodes {
	HALT,
	NOP,
	LD,
	ST,
	EM,
	LDX,
	STX,
	EMX,
	ADD,
	SUB,
	CLR,
	COM,
	AND,
	OR,
	XOR,
	ADDX,
	SUBX,
	CLRX,
	J,
	JZ,
	JN,
	JP,
	UNDEFINED
};

//op code category indicators
extern string MISC, MEM, ALU, TRANS;
//op code specifiers
extern string S_HALT, S_LD, S_ADD, S_J;
extern string S_NOP, S_ST, S_SUB, S_JZ;
extern string S_EM, S_CLR, S_JN;
extern string S_COM, S_JP;
extern string S_AND;
extern string S_OR;
extern string S_XOR;
extern string S_LDX, S_ADDX;
extern string S_STX, S_SUBX;
extern string S_EMX, S_CLRX;

//register binary values
extern string R_0, R_1, R_2, R_3;
//struct for a single instruction
struct instruction {
	unsigned int instructionAddress; //address of this instruction
	int indexRegister; //the index register the instruction specifies
	addrModes addressMode; //the addressing mode of the instruction
	opCodes opCode; //the op code of the instruction
	unsigned int operandAddress; //address of the instruction operand
	string instructionHexString; //hex value of the whole instruction
	int EA; //final memory value to load from (or immediate value to use)
};
//map of bitstrings to addressing modes
extern map<string, addrModes> addressMap;
//map of opcode enums to name of opcode for printing trace line
extern map<opCodes, string> opCodesPrintMap;

#endif