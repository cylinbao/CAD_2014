#include "stdio.h"
#include "stdlib.h"
#include "core.h"
#include "parser.h"

using namespace std;

extern void readInputFile(char*);
System sys;

int main(int argc, char** argv)
{
	readInputFile(argv[1]);
	setPrecedence();
	TAMwidthAssign();
	sys.TAM.initTAM(sys.getSysTW());
	sys.TAM.printTAM();
	//sys.fillTest();
	return 0;
}
