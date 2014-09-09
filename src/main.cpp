#include "stdio.h"
#include "stdlib.h"
#include "core.h"
#include "parser.h"

//#define debug1

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
	sys.printTAMAssignment();
	#ifdef debug1
	printf("External List Size: %lu\n", sys.ext_list.size());
	printf("Wiat External List Size: %lu\n", sys.wait_ext_list.size());
	#endif
	sys.fillTest(); //test this function
	sys.printExtList();
	return 0;
}
