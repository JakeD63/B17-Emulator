#pragma once
#include <map>
#include <string>
using namespace std;

string TEST_BIN_INSTRUCTION = "000001010000010000000100";

enum addrModes {
	Direct,
	Immediate,
	Indexed,
	Indirect,
	Indexed_Indrect,
	Illegal
};

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
string MISC = "00", MEM = "01", ALU = "10", TRANS = "11";
//op code specifiers (NOTE: many bitstrings here are equal, this is because their category indicators are different)
string S_HALT = "0000", S_LD = "0000", S_ADD = "0000", S_J = "0000";
string S_NOP = "0001", S_ST = "0001", S_SUB = "0001", S_JZ = "0001";
string S_EM = "0010", S_CLR = "0010", S_JN = "0010";
string S_COM = "0011", S_JP = "0011";
string S_AND = "0100";
string S_OR = "0101";
string S_XOR = "0110";
string S_LDX = "1000", S_ADDX = "1000";
string S_STX = "1001", S_SUBX = "1001";
string S_EMX = "1010", S_CLRX = "1010";

//register binary values
string R_0 = "00", R_1 = "01", R_2 = "10", R_3 = "11";

struct instruction {
	unsigned int instructionAddress;
	int indexRegister;
	addrModes addressMode;
	opCodes opCode;
	unsigned int operandAddress;
};

//map of bitstrings to addressing modes
map<string, addrModes> addressMap = { {"0000", Direct}, {"0001", Immediate}, {"0010", Indexed}, {"0110", Indexed_Indrect} };