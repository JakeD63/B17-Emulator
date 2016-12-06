#include "Instructions.h"

//halts machine and ends processing
void Instructions::halt() {
	cout << "Machine Halted - HALT instruction executed" << endl;
	exit(0);
}

//Load the accumulator from memory
void Instructions::LD(instruction i) {
	//if the addressing mode is IMM, take the immediate value
	if (i.addressMode == Immediate)
		AC = i.EA;
	//otherwise, load value from memory
	else 
		AC = memory[i.EA];
}

void Instructions::ST(instruction i) {
	//check for legal addressing mode
	if (i.addressMode == Immediate) {
		cout << "Machine Halted - illegal addressing mode" << endl;
		exit(0);
	}
	//store AC into memory location
	else
		memory[i.EA] = AC;
}

void Instructions::EM(instruction i) 
{
	int tmp; //used for swap
	//check for illegal addressing mode
	if (i.addressMode == Immediate) {
		cout << "Machine Halted - illegal addressing mode" << endl;
		exit(0);
	}
	//swap memory with AC
	else {
		tmp = memory[i.EA];
		i.EA = AC;
		AC = tmp;
	}
}

void Instructions::LDX(instruction i)
{
	int x; //holds value to store to register
	//check for illegal addressing modes
	if (i.addressMode == Indexed || i.addressMode == Indirect) {
		cout << "Machine Halted - illegal addressing mode" << endl;
		exit(0);
	}
	else {
		x = memory[i.EA]; //load value from memory
		//store it to specified register
		switch (i.indexRegister)
		{
		case 0:
			X0 = x;
			break;
		case 1:
			X1 = x;
			break;
		case 2:
			X2 = x;
			break;
		case 3:
			X3 = x;
			break;
		default:
			cout << "Machine Halted - illegal index register (somehow)" << endl;
			exit(0);
			break;
		}
	}
}

void Instructions::STX(instruction i)
{
	//check for illegal addressing mode
	if (i.addressMode != Direct) {
		cout << "Machine Halted - illegal addressing mode" << endl;
		exit(0);
	}
	//store specified register into memory location in EA
	else {
		switch (i.indexRegister)
		{
		case 0:
			memory[i.EA] = X0;
			break;
		case 1:
			memory[i.EA] = X1;
			break;
		case 2:
			memory[i.EA] = X2;
			break;
		case 3:
			memory[i.EA] = X3;
			break;
		default:
			cout << "Machine Halted - illegal index register (somehow)" << endl;
			exit(0);
			break;
		}
	}
}

void Instructions::EMX(instruction i)
{
	int tmp; //temp value used for swap
	//check for illegal addressing mode
	if (i.addressMode != Direct) {
		cout << "Machine Halted - illegal addressing mode" << endl;
		exit(0);
	}
	//store specified register into memory location in EA
	else {
		tmp = memory[i.EA];
		switch (i.indexRegister)
		{
		case 0:
			memory[i.EA] = X0;
			X0 = tmp;
			break;
		case 1:
			memory[i.EA] = X1;
			X1 = tmp;
			break;
		case 2:
			memory[i.EA] = X2;
			X2 = tmp;
			break;
		case 3:
			memory[i.EA] = X3;
			X3 = tmp;
			break;
		default:
			cout << "Machine Halted - illegal index register (somehow)" << endl;
			exit(0);
			break;
		}
	}
}

void Instructions::ADD(instruction i)
{
	//if IMM addressing mode, add immediate value to AC
	if (i.addressMode == Immediate) {
		AC += i.EA;
	}
	//otherwise add memory location to AC
	else {
		AC += memory[i.EA];
	}
}

void Instructions::SUB(instruction i)
{
	//if IMM addressing mode, subtract immediate value from AC
	if (i.addressMode == Immediate) {
		AC -= i.EA;
	}
	//otherwise subtract memory location from AC
	else {
		AC -= memory[i.EA];
	}
}

void Instructions::CLR()
{
	//set value of accumulator to 0
	AC = 0;
}

void Instructions::COM()
{
	//take complement of the accumulator
	AC = ~AC;
}

void Instructions::AND(instruction i)
{
	//bitwise AND a memory location and the accumulator
	//for IMM address mode, and the immediate value in the instruction
	if (i.addressMode == Immediate)
		AC = AC & i.EA;
	//for all other addressing modes, AND the value at the memory location i.EA
	else
		AC = AC & memory[i.EA];
}

void Instructions::OR(instruction i)
{
	//bitwise OR a memory location and the accumulator
	//for IMM address mode, OR the immediate value in the instruction
	if (i.addressMode == Immediate)
		AC = AC | i.EA;
	//for all other addressing modes, OR the value at the memory location i.EA
	else
		AC = AC | memory[i.EA];
}

void Instructions::XOR(instruction i)
{
	//bitwise XOR a memory location and the accumulator
	//for IMM address mode, XOR the immediate value in the instruction
	if (i.addressMode == Immediate)
		AC = AC ^ i.EA;
	//for all other addressing modes, XOR the value at the memory location i.EA
	else
		AC = AC ^ memory[i.EA];
}

void Instructions::ADDX(instruction i)
{
	int addVal; //value to add to register
	//check for ilegal addressing modes
	if (i.addressMode == Indexed || i.addressMode == Indirect) {
		cout << "Machine Halted - illegal addressing mode" << endl;
		exit(0);
	}
	//for IMM, add the EA value directly
	else if (i.addressMode == Immediate) {
		addVal = i.EA;
	}
	//for Direct, add value located at memory location EA
	else {
		addVal = memory[i.EA];
	}
	//add addVal to specified index register
	switch (i.indexRegister) {
	case 0:
		X0 += addVal;
		break;
	case 1:
		X1 += addVal;
		break;
	case 2:
		X2 += addVal;
		break;
	case 3:
		X3 += addVal;
		break;
	default:
		cout << "Machine Halted - illegal index register (somehow)" << endl;
		exit(0);
	}
}

void Instructions::SUBX(instruction i)
{
	int subVal; //value to sub from register
	//check for ilegal addressing modes
	if (i.addressMode == Indexed || i.addressMode == Indirect) {
		cout << "Machine Halted - illegal addressing mode" << endl;
		exit(0);
	}
	//for IMM, sub the EA value directly
	else if (i.addressMode == Immediate) {
		subVal = i.EA;
	}
	//for Direct, sub value located at memory location EA
	else {
		subVal = memory[i.EA];
	}
	//sub addVal from specified index register
	switch (i.indexRegister) {
	case 0:
		X0 -= subVal;
		break;
	case 1:
		X1 -= subVal;
		break;
	case 2:
		X2 -= subVal;
		break;
	case 3:
		X3 -= subVal;
		break;
	default:
		cout << "Machine Halted - illegal index register (somehow)" << endl;
		exit(0);
	}
}

void Instructions::CLRX(instruction i)
{

}

//print out trace line of an instruction
void Instructions::printInstruction(instruction i) {
	//print address of the instruction
	cout << i.instructionAddress << ":  ";
	//print instruction itself in hex
	cout << i.instructionHexString << "   ";
	//print instruction mnemonic
	cout << opCodesPrintMap[i.opCode] << "   ";
	//print EA used by this instruction (or IMM is Immediate addressing mode)
	if (i.opCode == Immediate)
		cout << "IMM    ";
	else if (i.opCode == HALT)
		cout << "   ";
	else {
		cout << hex << i.EA << "   ";
	}

}

//print contents of registers after execution of instruction
void Instructions::printRegisters() {

}
