#include "stdio.h"
#include "stdlib.h"
#include "core.h"
#include "parser.h"

//#define debug1
#define debug2

using namespace std;

extern void readInputFile(char*);
System sys;

int main(int argc, char** argv)
{
	readInputFile(argv[1]);
	setPrecedence();
	TAMwidthAssign();
	sys.setWaitExtList();
	sys.setWaitBistList();
	sys.TAMStat.initTAM(sys.getSysTW());
	#ifdef debug1
	sys.TAMStat.printTAM();
	printf("External List Size: %lu\n", sys.ext_list.size());
	printf("Wiat External List Size: %lu\n", sys.wait_ext_list.size());
	#endif
	sys.fillTest(); //test this function
	#ifdef debug2
	//printf("Power Limit: %d\n", sys.getSysPower());
	sys.printResult();
	//sys.printTAMAssignment();
	//sys.printTest();
	//sys.printExtList();
	//sys.printBistList();
	//sys.TAMStat.printPowerStat();
	#endif 
	return 0;
}
