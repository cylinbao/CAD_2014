#include "stdio.h"
#include "stdlib.h"
#include "core.h"
#include "parser.h"
#include <new>

//#define debug1
//#define debug2
//#define debug3
//#define debug4

using namespace std;

extern void readInputFile(char*);
System* getFasterSys(System *sys1, System *sys2);
System **sys;

int main(int argc, char** argv)
{
	int round1, index;
	float round2;
	System *recSys;

	sys = new System* [10 * 11];

	for(round1 = 1; round1 < 11; round1++) {
		for(round2 = 1.00; round2 < 1.10; round2+=0.01) {
			index = ((round1-1) * 11) + ((round2 - 1.00) / 0.01);
			sys[index] = new System;

			#ifdef debug3
			printf("Round %d\n", round1+1);
			printf("File name: %s\n", argv[1]);
			#endif
			
			readInputFile(argv[1], sys[index]);
			setPrecedence(sys[index]);
			TAMwidthAssign(round1, round2, sys[index]);
			//sys[index].printPrecedence();
			sys[index]->setWaitExtList();
			sys[index]->setWaitBistList();
			sys[index]->TAMStat.initTAM(sys[index]->getSysTW());

			#ifdef debug1
			sys[index]->TAMStat.printTAM();
			printf("External List Size: %lu\n", sys[index]->ext_list.size());
			printf("Wiat External List Size: %lu\n", sys[index]->wait_ext_list.size());
			#endif

			sys[index]->fillTest(); //test this function
			//sys[index]->printResult(argv[1]);

			#ifdef debug2
			printf("Power Limit: %d\n", sys[index]->getSysPower());
			sys[index]->printExtList();
			sys[index]->printBistList();
			//sys[index]->TAMStat.printPowerStat();
			#endif 

			///*
			if(round1 == 1 && round2 == 1.00)
				recSys = sys[index];
			else
				recSys = getFasterSys(sys[index], recSys);
			//*/ 

			//delete sys;

			#ifdef debug3
			printf("Round1 %d & Round2 %f End!\n", round1, round2);
			#endif
		}
	}
	recSys->printResult(argv[1]);

	for(index = 0; index < 110; index++)
		delete sys[index];

	delete [] sys;

	return 0;
}

System* getFasterSys(System *sys1, System *sys2)
{
	System *reSys;
	map<int ,int>::iterator it1, it2;

	it1 = sys1->TAMStat.powerStat.end();
	it1--;

	it2 = sys2->TAMStat.powerStat.end();
	it2--;

	if(it1->first < it2->first)
		reSys = sys1;
	else
		reSys = sys2;

	return reSys;
}
