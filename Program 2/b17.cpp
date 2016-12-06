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
vector<string> splitString(string s);
int getIndexRegister(string s);
unsigned int getOperandAddress(string s);
addrModes getAddrMode(string s);
opCodes getOpCode(string s);
string convertToBin(string s);
string pad(string s);


int main(int argc, char* argv[]) {
	ExecuteInstruction ins;
	readInstructions(argv[1]);
	bool jump;

	//done reading in instructions
	//start executing instructions

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
			if (instructionRegister++ != instructions.end()) {
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

//Read instructions from the 
//passed in command line argument (argv[1])
void readInstructions(string file) {
	ifstream fin;
	vector<string> instructionList;
	string currentLine, instructionString;
	instruction currentInstruction;
	int num;
	unsigned int startAddress;

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
				if (currentInstruction.opCode == Direct) {
					currentInstruction.EA = currentInstruction.operandAddress;
				}
				//technically there is no EA, but I set it to the immediate value to 
				//not have to have another variable in the struct only used with IMM
				else if (currentInstruction.opCode == Immediate) {
					currentInstruction.EA = currentInstruction.operandAddress;
				}
				else if (currentInstruction.opCode == Indexed) {
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
					if (it->instructionAddress == startAddress) {
						instructionRegister = it;
						break;
					}

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