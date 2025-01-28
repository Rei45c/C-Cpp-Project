#include "type.h"
#include <fstream>

TypeList::TypeList(char* fileName)
{
	ifstream s(fileName);
	try { for (;;) types.insert(s); }
	catch (EOFException_type) {}
}
