#include "const.h"
#include <vector>

//map of memory addresses to their instructions
vector<instruction> instructions;
//register definitions
unsigned int X0 = 0, X1 = 0, X2 = 0, X3 = 0;
//pointer to instruction to begin execution at
vector<instruction>::iterator startInstruction;