#include "stdio.h"
#include "stdlib.h"
#include "core.h"
#include "parser.h"

using namespace std;

extern void readInputFile(char*);
//extern void possibleExternal();
//extern void possibleBIST();
System sys;

int main(int argc, char** argv)
{
	readInputFile(argv[1]);
	setPrecedence();
	TAMwidthAssign();
	sys.initTAM();
	sys.printTAM();
	//sys.fillTest();
	return 0;
}
