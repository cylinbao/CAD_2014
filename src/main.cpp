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
	sys.setWaitExtList();
	sys.TAMStat.initTAM(sys.getSysTW());
	sys.TAMStat.printTAM();
	//sys.fillTest(); //test this function
	return 0;
}
