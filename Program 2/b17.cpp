/************************************************************************
Program: B17 Emulator
Author: Jake Davidson
Class: Computer Organization and Architecture
Instructor: Dr. Karlsson
Date: December 5th, 2016
Description: This program emulates the B17 chip to the specifications in the handout.
It reads in instructions from an object file and executes them one by one, printing
a trace line at each execution.

The program begins by reading in the .obj file passed in from the command line that
contains the instructions and program information. Each line of the obj file starts with the 
beginning memory address of the instructions on that line (if there are more than one instructions
on the line, each subsequent instruction will have a value of 1 + the previous address).
The next number is the number of instructions on that line. Following that is the instructions,
inputted as 6 digit hex numbers. These numbers, in binary, contain all the information needed for
the instruction. The bits are numbered from right to left, with bit 0 being the least significant
and bit 23 being the most significant. Bits 5-0 contain information about the addressing mode, the
way data is pulled from memory. Bits 11-6 contain the operation code, split into a 2 bit category
specifier and a 4 bit operation specifier. Bits 23-12 contain the address to use in the operation
(or if the addressing mode is immediate, the immediate value to use in the operation).

After each line is read in, the instructions are stored in a vector chronologically. This vector 
contains structs that represent each instruction, containing the instruction address, the addressing 
mode, the operation code, the operand address, and the EA (which is the final memory address after 
addressing mode calculations are done). The last line of the object file contains the memory location 
to start execution. This is stored to the Instruction Register, which stores the current instruction 
being executed. The Instruction Register is a pointer to the location in the instruction vector 
that we are currently executing.  

After the instructions vector is loaded in, we print the beginning of the trace line. We do not print 
the register contents until the instruction is finished executing. Next, the program uses the 
ExecuteInstruction class to call the function specified by the operation code in the current instruction. 
The ExecuteInstruction is a container for all the instruction functions, and acts as the ALU for arithmetic 
instructions. Once the function is finished running, the program prints the end of the trace line, which 
is the contents of the Accumulator and the three index registers. After this, the Instruction Register is  
incremented to the next instruction, if we are not at the end of the list. If we are at the end of the list,  
the program halts the machine and exits. The only time the Instruction Register is not incremented is 
if the current instruction performs a jump operation. A jump operation sets the next instruction to be 
different then the one next in line in the vector. If we jump, the ExecuteInstruction function for jumps
sets the Instruction Register to point to the instruction in the vector located at the memory address 
specified in the instruction. In this case, we do not increment the Instruction register after execution,
as the jump function has set if for us. 

This instruction execution loop lasts until either an invalid instruction is executed, or a Halt instruction
is executed. 

Input: instructions.obj as a command line argument
Output: trace line of each instruction and the contents of the registers after its execution
Compilation instructions: run "make" in program directory
Usage: ./b17 <object file>
Known bugs/missing features: In the example object files and output on the handout, it appears that program memory is 
already populated. In this program, all memory starts at 0, so many of the accumulator values do not match. This is not
technically a bug, since it is just a difference of implementation, but it is important to note nonetheless.
************************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "ExecuteInstruction.h"
#include "globals.h"
#include "const.h"

using namespace std;

void readInstructions(string file);
void execute();
vector<string> splitString(string s);
int getIndexRegister(string s);
unsigned int getOperandAddress(string s);
addrModes getAddrMode(string s);
opCodes getOpCode(string s);
string convertToBin(string s);
string pad(string s);

/************************************************************************
Function: main
Author: Jake Davidson
Description: Entry point for the program. Calls the instruction reading 
routine and contains the execution loop. 
Parameters: argc - number of cmd line args
			argv - array of cmd line args
Returns: 0 - End of program
************************************************************************/
int main(int argc, char* argv[]) {
	//verify command line arguments
	if (argc != 2) {
		cout << "Please only supply the program with the object file as cmd args." << endl;
		return 0;
	}
	//read instructions from object file
	//this function populates the instructions vector
	readInstructions(argv[1]); 
	//done reading in instructions
	//start executing instructions
	if(!instructions.empty())
		execute();
	else 
		cout << "No instructions loaded, ensure object file is not empty." << endl;
	return 0;
}

/************************************************************************
Function: readInstructions
Author: Jake Davidson
Description: Loops through each line of input object file and decodes
each instruction. Inserts each instruction into instructions vector.
Parameters: file - the object file to read from
************************************************************************/
void readInstructions(string file) {
	ifstream fin; //stream to read file from
	vector<string> instructionList; //list of hex instructions on the current line
	string currentLine, //current line being read in
		instructionString; //current instruction being decoded
	instruction currentInstruction; //instruction to build as we decode instructionString
	int num; //number of instructions on the current line
	unsigned int startAddress; //address of the current instruction

	//check that the file was opened successfully
	fin.open(file);
	if (!fin) {
		cout << "Could not open object file, ensure the path is correct." << endl;
		exit(0);
	}

	//read in the object file and add instructions to the list
	while (getline(fin, currentLine)) {
		instructionList = splitString(currentLine);
		//if we are not at the last line (last line only contains start address)
		if (instructionList.size() != 1) {
			//get the number of instructions on this line
			num = stoi(instructionList.at(1));
			//get the start address of the first instruction of the line
			startAddress = stol(instructionList[0], nullptr, 16);
			//loop through instructions on the current line adding them to the program
			for (int i = 0; i < num; i++) {
				//build current instruction (offset by 2 because of first two items not being instructions)
				instructionString = instructionList.at(i + 2);
				//store the hex value of the instruction to print in trace line
				currentInstruction.instructionHexString = instructionString;
				//convert the string to binary to extract bits to decode instruction
				instructionString = convertToBin(instructionString);
				//pad with 0s on left if too short
				instructionString = pad(instructionString);
				//read in current instruction
				//get instruction address
				currentInstruction.instructionAddress = startAddress;
				//get the 2-bit index register number
				currentInstruction.indexRegister = getIndexRegister(instructionString);
				//get the addressing mode (bits 5-2)
				currentInstruction.addressMode = getAddrMode(instructionString);
				//get the opcode (bits 11-6)
				currentInstruction.opCode = getOpCode(instructionString);
				//get the operand address (bits 23-12)
				currentInstruction.operandAddress = getOperandAddress(instructionString);

				//calculate the EA of the instruction
				if (currentInstruction.addressMode == Direct) {
					currentInstruction.EA = currentInstruction.operandAddress;
				}
				//technically there is no EA, but I set it to the immediate value to 
				//not have to have another variable in the struct only used with IMM
				else if (currentInstruction.addressMode == Immediate) {
					currentInstruction.EA = currentInstruction.operandAddress;
				}
				//for indexed mode, the ea is the memory location at operandAddress + register contents
				else if (currentInstruction.addressMode == Indexed) {
					//get the index register to add to the operand address
					switch (currentInstruction.indexRegister)
					{
					case 0:
						currentInstruction.EA = currentInstruction.operandAddress + X0;
					case 1:
						currentInstruction.EA = currentInstruction.operandAddress + X1;
					case 2:
						currentInstruction.EA = currentInstruction.operandAddress + X2;
					case 3:
						currentInstruction.EA = currentInstruction.operandAddress + X3;
					default:
						break;
					}
				}
				//Indirect addressing mode
				else {
					//get EA from memory address
					currentInstruction.EA = memory[currentInstruction.operandAddress];
				}

				//add to instruction vector
				instructions.push_back(currentInstruction);
				//increment the starting address for next loop
				startAddress = startAddress + 1;
			}
		}
		//we are at the last line, which contains the location to start execution
		else {
			if (!instructions.empty()) {
				startAddress = stol(instructionList[0], nullptr, 16);
				//look through instructions to find the one with startAddress
				for (vector<instruction>::iterator it = instructions.begin(); it != instructions.end(); it++) {
					//when we find the start address in the instructions vector, set our instruction register to its location
					if (it->instructionAddress == startAddress) {
						instructionRegister = it;
						break;
					}
					//if there was no instruction location at the end of the file to start at
					if (it == instructions.end()) {
						cout << "Machine Halted - no instruction at start address" << endl;
						exit(0);
					}
				}

			}
			else {
				//no instructions read in
				cout << "Machine Halted - No instructions to execute";
				exit(0);
			}
		}
	}
}



void execute() {
	ExecuteInstruction ins; //container class for instructions and ALU operations
	bool jump; //bool to keep track of whether or not we have jumped or not
	//run instructions until we hit halt or 
	//have an error
	while (true) {
		jump = false;
		instruction i = *instructionRegister;
		//print current instructions and all related data
		ins.printInstruction(i);

		//execute the instruction based on op code
		switch (instructionRegister->opCode)
		{
		case HALT:
			ins.halt();
			break;
		case NOP:
			//do nothing
			break;
		case LD:
			ins.LD(i);
			break;
		case ST:
			ins.ST(i);
			break;
		case EM:
			ins.EM(i);
			break;
		case LDX:
			ins.LDX(i);
			break;
		case STX:
			ins.STX(i);
			break;
		case EMX:
			ins.EMX(i);
			break;
		case ADD:
			ins.ADD(i);
			break;
		case SUB:
			ins.SUB(i);
			break;
		case CLR:
			ins.CLR();
			break;
		case COM:
			ins.COM();
			break;
		case AND:
			ins.AND(i);
			break;
		case OR:
			ins.OR(i);
			break;
		case XOR:
			ins.XOR(i);
			break;
		case ADDX:
			ins.ADDX(i);
			break;
		case SUBX:
			ins.SUBX(i);
			break;
		case CLRX:
			ins.CLRX(i);
			break;
		case J:
			jump = ins.J(i);
			break;
		case JZ:
			jump = ins.JZ(i);
			break;
		case JN:
			jump = ins.JN(i);
			break;
		case JP:
			jump = ins.JP(i);
			break;
		default:
			ins.printRegisters();
			cout << "Machine Halted - undefined opcode" << endl;
			exit(0);
			break;
		}
		//print contents of registers after instruction is executed
		ins.printRegisters();

		//if we do not jump, we need to point instructionRegister to the 
		//next instruction in the list
		if (!jump) {
			//if we are not at the end of the list
			if (instructionRegister + 1 != instructions.end()) {
				instructionRegister++;
			}
			//we have executed the last instruction and there was no jump
			//end the program
			else {
				cout << "Machine Halted - no more instructions to execute" << endl;
				exit(0);
			}
		}
	}
}

//get the address mode from a binary string (bits 5-2)
addrModes getAddrMode(string s) {
	addrModes a;
	string addressMode;
	map<string, addrModes>::iterator it;
	//extract address mode from bitstring
	addressMode = s.substr(18, 4);
	//match address mode bits to address mode
	it = addressMap.find(addressMode);
	//if we found a match in the map, assign it to a
	if (it != addressMap.end()) {
		a = it->second;
	}
	//otherwise, make a illegal so it outputs error during execution
	else {
		a = Illegal;
	}
	//return the address mode for the instruction
	return a;
}
//gets the op code from a binary string (bits 11-6)
opCodes getOpCode(string s) {
	opCodes op = UNDEFINED;
	//strings to hold the category bits and the specifier bits
	string category = "", specifier = "";
	//extract category bits
	category = s.substr(12, 2);
	//extract specifier bits
	specifier = s.substr(14, 4);
	//match bitstrings to opcode
	if (category == MISC) {
		if (specifier == S_HALT)
			op = HALT;
		else if (specifier == S_NOP)
			op = NOP;
	}
	else if (category == MEM) {
		if (specifier == S_LD)
			op = LD;
		else if (specifier == S_ST)
			op = ST;
		else if (specifier == S_EM)
			op = EM;
		else if (specifier == S_LDX)
			op = LDX;
		else if (specifier == S_STX)
			op = STX;
		else if (specifier == S_EMX)
			op = EMX;
	}
	else if (category == ALU) {
		if (specifier == S_ADD)
			op = ADD;
		else if (specifier == S_SUB)
			op = SUB;
		else if (specifier == S_CLR)
			op = CLR;
		else if (specifier == S_COM)
			op = COM;
		else if (specifier == S_AND)
			op = AND;
		else if (specifier == S_OR)
			op = OR;
		else if (specifier == S_XOR)
			op = XOR;
		else if (specifier == S_ADDX)
			op = ADDX;
		else if (specifier == S_SUBX)
			op = SUBX;
		else if (specifier == S_CLRX)
			op = CLRX;
		}
	else if (category == TRANS) {
		if (specifier == S_J)
			op = J;
		else if (specifier == S_JZ)
			op = JZ;
		else if (specifier == S_JN)
			op = JN;
		else if (specifier == S_JP)
			op = JP;
	}
	else {
		//op code was undefined
		op = UNDEFINED;
	}

	return op;
}
//get the two bit index register number
int getIndexRegister(string s) {
	string registerString = "";
	int indexRegister;
	registerString = s.substr(22, 2);
	if (registerString == R_0)
		indexRegister = 0;
	else if (registerString == R_1)
		indexRegister = 1;
	else if (registerString == R_2)
		indexRegister = 2;
	else if (registerString == R_3)
		indexRegister = 3;
	else {
		//for some reason, you got here. Should be impossible, as we check all
		//possible 2 bit combinations
		cout << "Invalid register number, something went horribly wrong." << endl;
		exit(0);
	}
	return indexRegister;
}
//get the address from the ADDR field (bits 23-12)
unsigned int getOperandAddress(string s) {
	string addressString = "";
	int address = 0;
	addressString = s.substr(0, 12);
	//convert to an int
	address = stoi(addressString, nullptr, 2);
	return address;
}
//convert a string of hex to a string of binary for bit extraction
string convertToBin(string s) {
	string binString = "";
	for (char c : s) {
		switch (c) {
		case '0':
			binString.append("0000");
			break;
		case '1':
			binString.append("0001");
			break;
		case '2':
			binString.append("0010");
			break;
		case '3':
			binString.append("0011");
			break;
		case '4':
			binString.append("0100");
			break;
		case '5':
			binString.append("0101");
			break;
		case '6':
			binString.append("0110");
			break;
		case '7':
			binString.append("0111");
			break;
		case '8':
			binString.append("1000");
			break;
		case '9':
			binString.append("1001");
			break;
		case 'a':
			binString.append("1010");
			break;
		case 'b':
			binString.append("1011");
			break;
		case 'c':
			binString.append("1100");
			break;
		case 'd':
			binString.append("1101");
			break;
		case 'e':
			binString.append("1110");
			break;
		case 'f':
			binString.append("1111");
			break;
		default:
			cout << "You have an incorrect character in your hex string" << endl;
			break;
		}
	}
	return binString;
}

//split a string into tokens by whitespace and return in vector
vector<string> splitString(string s) {
	stringstream stream; //stream to use
	vector<string> words; //container for strings
	string curr = ""; //current string
	stream.str(s); //add s to our stream
				   //read until end of stream, splitting on spaces
	while (getline(stream, curr, ' ')) {
		words.push_back(curr); //when we have our word, add it to the vector
	}
	return words;
}
//pad a string with 0s on left, up to 24 (0-23)
string pad(string s) {
	string padString = s;
	while (padString.length() < 24)
		padString = "0" + padString;
	return padString;
}