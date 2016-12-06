#include "ExecuteInstruction.h"

//halts machine and ends processing
void ExecuteInstruction::halt() {
	this->printRegisters();
	cout << "Machine Halted - HALT instruction executed" << endl;
	exit(0);
}

//Load the accumulator from memory
void ExecuteInstruction::LD(instruction i) {
	//if the addressing mode is IMM, take the immediate value
	if (i.addressMode == Immediate)
		AC = i.EA;
	//otherwise, load value from memory
	else 
		AC = memory[i.EA];
}

void ExecuteInstruction::ST(instruction i) {
	//check for legal addressing mode
	if (i.addressMode == Immediate) {
		this->printRegisters();
		cout << "Machine Halted - illegal addressing mode" << endl;
		exit(0);
	}
	//store AC into memory location
	else
		memory[i.EA] = AC;
}

void ExecuteInstruction::EM(instruction i) 
{
	int tmp; //used for swap
	//check for illegal addressing mode
	if (i.addressMode == Immediate) {
		this->printRegisters();
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

void ExecuteInstruction::LDX(instruction i)
{
	int x; //holds value to store to register
	//check for illegal addressing modes
	if (i.addressMode == Indexed || i.addressMode == Indirect) {
		this->printRegisters();
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

void ExecuteInstruction::STX(instruction i)
{
	//check for illegal addressing mode
	if (i.addressMode != Direct) {
		this->printRegisters();
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

void ExecuteInstruction::EMX(instruction i)
{
	int tmp; //temp value used for swap
	//check for illegal addressing mode
	if (i.addressMode != Direct) {
		this->printRegisters();
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

void ExecuteInstruction::ADD(instruction i)
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

void ExecuteInstruction::SUB(instruction i)
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

void ExecuteInstruction::CLR()
{
	//set value of accumulator to 0
	AC = 0;
}

void ExecuteInstruction::COM()
{
	//take complement of the accumulator
	AC = ~AC;
}

void ExecuteInstruction::AND(instruction i)
{
	//bitwise AND a memory location and the accumulator
	//for IMM address mode, and the immediate value in the instruction
	if (i.addressMode == Immediate)
		AC = AC & i.EA;
	//for all other addressing modes, AND the value at the memory location i.EA
	else
		AC = AC & memory[i.EA];
}

void ExecuteInstruction::OR(instruction i)
{
	//bitwise OR a memory location and the accumulator
	//for IMM address mode, OR the immediate value in the instruction
	if (i.addressMode == Immediate)
		AC = AC | i.EA;
	//for all other addressing modes, OR the value at the memory location i.EA
	else
		AC = AC | memory[i.EA];
}

void ExecuteInstruction::XOR(instruction i)
{
	//bitwise XOR a memory location and the accumulator
	//for IMM address mode, XOR the immediate value in the instruction
	if (i.addressMode == Immediate)
		AC = AC ^ i.EA;
	//for all other addressing modes, XOR the value at the memory location i.EA
	else
		AC = AC ^ memory[i.EA];
}

void ExecuteInstruction::ADDX(instruction i)
{
	int addVal; //value to add to register
	//check for ilegal addressing modes
	if (i.addressMode == Indexed || i.addressMode == Indirect) {
		this->printRegisters();
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

void ExecuteInstruction::SUBX(instruction i)
{
	int subVal; //value to sub from register
	//check for ilegal addressing modes
	if (i.addressMode == Indexed || i.addressMode == Indirect) {
		this->printRegisters();
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

void ExecuteInstruction::CLRX(instruction i)
{
	//switch on index register specified in the instruction
	switch (i.indexRegister)
	{
	//0 out the specified register
	case 0:
		X0 = 0;
		break;
	case 1:
		X1 = 0;
		break;
	case 2:
		X2 = 0;
		break;
	case 3:
		X3 = 0;
		break;
	default:
		cout << "Machine Halted - illegal register specifier (somehow)" << endl;
		exit(0);
		break;
	}
}

bool ExecuteInstruction::J(instruction i)
{
	//need to set instructionRegister to point to
	//the instruction with the address specified in i

	//check that we do not have an illegal addressing mode
	if (i.addressMode == Immediate) {
		this->printRegisters();
		cout << "Machine Halted - illegal addressing mode" << endl;
		exit(0);
	}
	else {
		for (vector<instruction>::iterator it = instructions.begin(); it != instructions.end(); it++) {
			//loop through instruction list
			if (it->instructionAddress == i.EA) {
				//if we find the address in our instruction list, set our instruction register to it
				instructionRegister = it;
				//tell main loop that we are taking the jump
				return true;
			}
		}
		//if we end up here, the address was not valid
		cout << "Machine Halted - invalid jump address" << endl;
		exit(0);
	}
	//we should never get here, but we need to return a default value
	return false;
}

bool ExecuteInstruction::JZ(instruction i)
{
	bool jump;
	if (i.addressMode == Immediate) {
		this->printRegisters();
		cout << "Machine Halted, invalid address mode" << endl;
		exit(0);
	}
	//jump if the accumulator is a zero
	if (AC == 0) {
		//call the jump function, and return what it returns. this will not return false,
		//since if the address is not found we halt the machine
		jump = this->J(i);
		return jump;
	}
	else {
		//if the AC is not 0, then we return false. The check for a valid jump address will not occur
		return false;
	}
}

bool ExecuteInstruction::JN(instruction i)
{
	bool jump;
	if (i.addressMode == Immediate) {
		this->printRegisters();
		cout << "Machine Halted, invalid address mode" << endl;
		exit(0);
	}
	//jump if the accumulator is negative
	if (AC < 0) {
		//call the jump function, and return what it returns. this will not return false,
		//since if the address is not found we halt the machine
		jump = this->J(i);
		return jump;
	}
	else {
		//if the AC is not negative, then we return false. The check for a valid jump address will not occur
		return false;
	}
}

bool ExecuteInstruction::JP(instruction i)
{
	bool jump;
	if (i.addressMode == Immediate) {
		this->printRegisters();
		cout << "Machine Halted, invalid address mode" << endl;
		exit(0);
	}
	//jump if the accumulator is positive
	if (AC > 0) {
		//call the jump function, and return what it returns. this will not return false,
		//since if the address is not found we halt the machine
		jump = this->J(i);
		return jump;
	}
	else {
		//if the AC is not positive, then we return false. The check for a valid jump address will not occur
		return false;
	}
}

//print out trace line of an instruction
void ExecuteInstruction::printInstruction(instruction i) {
	//print address of the instruction
	cout << hex << setw(3) << setfill('0') << i.instructionAddress << ":  ";
	//print instruction itself in hex
	cout << i.instructionHexString << "   ";
	//print instruction mnemonic
	cout << opCodesPrintMap[i.opCode] << "   ";
	//print EA used by this instruction (or IMM is Immediate addressing mode)
	if (i.addressMode == Direct)
		cout << hex << setw(3) << setfill('0') << i.EA << "    ";
	else if (i.addressMode == Immediate)
		cout << "IMM    ";
	else if (i.addressMode == HALT)
		cout << "   ";
		
	
}

//print contents of registers after execution of instruction
void ExecuteInstruction::printRegisters() {
	//print formatted contents of the AC and the 4 X registers
	cout << "AC[" << hex << setw(6) << setfill('0') << AC << "]   " << "X0[" << hex << setw(3) << setfill('0') << X0 << "]   " 
		<< "X1[" << hex << setw(3) << setfill('0') << X1 << "]   " <<"X2[" << hex << setw(3) << setfill('0') << X2 << "]   " 
		<< "X3[" << hex << setw(3) << setfill('0') << X3 << "]" << endl;
}
