#include "ExecuteInstruction.h"

/************************************************************************
Function: halt
Author: Jake Davidson
Description: Halts execution
************************************************************************/
void ExecuteInstruction::halt() {
	this->printRegisters();
	cout << "Machine Halted - HALT instruction executed" << endl;
	exit(0);
}

/************************************************************************
Function: LD
Author: Jake Davidson
Description: Loads AC from memory
Parameters: i - current instruction
************************************************************************/
void ExecuteInstruction::LD(instruction i) {
	//if the addressing mode is IMM, take the immediate value
	if (i.addressMode == Immediate)
		AC = i.EA;
	//otherwise, load value from memory
	else 
		AC = memory[i.EA];
}

/************************************************************************
Function: ST
Author: Jake Davidson
Description: Stores AC to memory
Parameters: i - current instruction
************************************************************************/
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

/************************************************************************
Function: EM
Author: Jake Davidson
Description: Exchanges AC with memory location
Parameters: i - current instruction
************************************************************************/
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

/************************************************************************
Function: LDX
Author: Jake Davidson
Description: loads a memory location into an index register
Parameters: i - current instruction
************************************************************************/
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

/************************************************************************
Function: STX
Author: Jake Davidson
Description: stores an index register to memory
Parameters: i - current instruction
************************************************************************/
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

/************************************************************************
Function: EMX
Author: Jake Davidson
Description: swap memory location with index register
Parameters: i - current instruction
************************************************************************/
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

//ALU FUNCTIONS

/************************************************************************
Function: ADD
Author: Jake Davidson
Description: Adds to the AC from memory or immediate value
Parameters: i - current instruction
************************************************************************/
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

/************************************************************************
Function: SUB
Author: Jake Davidson
Description: subtracts from the  AC (either from memory or immediate value)
Parameters: i - current instruction
************************************************************************/
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

/************************************************************************
Function: CLR
Author: Jake Davidson
Description: sets AC to 0
Parameters: i - current instruction
************************************************************************/
void ExecuteInstruction::CLR()
{
	//set value of accumulator to 0
	AC = 0;
}

/************************************************************************
Function: COM
Author: Jake Davidson
Description: sets AC to its complement
Parameters: i - current instruction
************************************************************************/
void ExecuteInstruction::COM()
{
	//take complement of the accumulator
	AC = ~AC;
}

/************************************************************************
Function: AND
Author: Jake Davidson
Description: ands a value with the AC
Parameters: i - current instruction
************************************************************************/
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

/************************************************************************
Function: OR
Author: Jake Davidson
Description: OR a value and the AC
Parameters: i - current instruction
************************************************************************/
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

/************************************************************************
Function: XOR
Author: Jake Davidson
Description: XOR a value with AC
Parameters: i - current instruction
************************************************************************/
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

/************************************************************************
Function: ADDX
Author: Jake Davidson
Description: add value to specified index register
Parameters: i - current instruction
************************************************************************/
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

/************************************************************************
Function: SUBX
Author: Jake Davidson
Description: subtract value from specified index register
Parameters: i - current instruction
************************************************************************/
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

/************************************************************************
Function: CLRX
Author: Jake Davidson
Description: 0 out an index register
Parameters: i - current instruction
************************************************************************/
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

/************************************************************************
Function: J
Author: Jake Davidson
Description: jump execution to specified memory address
Parameters: i - current instruction
************************************************************************/
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
/************************************************************************
Function: JZ
Author: Jake Davidson
Description: jumps if AC is 0
Parameters: i - current instruction
************************************************************************/
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

/************************************************************************
Function: JN
Author: Jake Davidson
Description: jumps if AC is negative
Parameters: i - current instruction
************************************************************************/
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

/************************************************************************
Function: JP
Author: Jake Davidson
Description: jumps if AC is positive
Parameters: i - current instruction
************************************************************************/
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

/************************************************************************
Function: printInstruction
Author: Jake Davidson
Description: prints out information about the current instruction
for the trace line.
Parameters: i - current instruction
************************************************************************/
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

/************************************************************************
Function: printRegisters
Author: Jake Davidson
Description: prints the values of the AC and 4 index registers
Parameters: i - current instruction
************************************************************************/
void ExecuteInstruction::printRegisters() {
	//print formatted contents of the AC and the 4 X registers
	cout << "AC[" << hex << setw(6) << setfill('0') << AC << "]   " << "X0[" << hex << setw(3) << setfill('0') << X0 << "]   " 
		<< "X1[" << hex << setw(3) << setfill('0') << X1 << "]   " <<"X2[" << hex << setw(3) << setfill('0') << X2 << "]   " 
		<< "X3[" << hex << setw(3) << setfill('0') << X3 << "]" << endl;
}
