#include "gate.h"
#include <fstream>

GateList::GateList(char* fileName)
{
	ifstream s(fileName);
	try { for (;;) gates.insert(s); }
	catch (EOFException_gate) {}
}
