#include "core.h"
#include "stdio.h"
#include "string.h"

void System::printResult(char *str)
{
	char *outName;
	FILE *pFile;

	#ifdef debug3
	printf("original = %s\n", str);
	#endif
	outName = strtok(str,".");
	strcat(outName, ".scheduling");
	#ifdef debug3
	printf("outName = %s\n", outName);
	#endif
	pFile = fopen(outName, "w");

	fprintf(pFile, "Schedule\nbegin\n");
	printTestTime(pFile);
	printTAMAssignment(pFile);
	printTest(pFile);
	fprintf(pFile, "\nend");
}

void System::printTestTime(FILE *pFile)
{
	map<int ,int>::iterator it;
	it = TAMStat.powerStat.end();
	it--;
	fprintf(pFile, "\n\tTest_time %d\n", it->first);
}

void System::printTAMAssignment(FILE *pFile)
{
	int i, j, first, second;
	vector<pair<int, int> >  range;

	for(i=0; i<(int)core.size(); i++) {
		//cout << "\n\tTAM_assignment " << core[i]->getName();
		fprintf(pFile, "\n\tTAM_assignment %s", core[i]->getName().c_str());
		range = core[i]->getTAM_range();

		for(j=0; j<(int)range.size(); j++) {
			first = range[j].first;	
			second = range[j].second;	

			//cout << " [" << first << ":" << second << "]";
			fprintf(pFile, " [%d:%d]", first, second);
		}
		//cout << endl;
		fprintf(pFile, "\n");
	}
}

void System::printTest(FILE *pFile)
{
	map<string, External*>::iterator itExt;
	map<string, BIST*>::iterator itBist;
	Core *pCore;
	int i, j, first, second;
	vector< pair<int, int> > *tempExecTime;

	for(i=0; i<(int)core.size(); i++) {
		pCore = core[i];
		for(itExt = pCore->ext_list.begin(); itExt != pCore->ext_list.end(); itExt++) {
			fprintf(pFile, "\n\tExternal %s %s", pCore->getName().c_str(), itExt->second->getName().c_str());
			tempExecTime = itExt->second->getExecTime();
			for(j = 0; j < (int)tempExecTime->size(); j++) {
				first = (*tempExecTime)[j].first;
				second = (*tempExecTime)[j].second;

				fprintf(pFile, " (%d,%d)", first, second-1);
			}
			fprintf(pFile, "\n");
		}

		for(itBist = pCore->bist_list.begin(); itBist != pCore->bist_list.end(); itBist++) {
			fprintf(pFile, "\n\tBIST %s %s", pCore->getName().c_str(), itBist->second->getName().c_str());
			tempExecTime = itBist->second->getExecTime();
			for(j = 0; j < (int)tempExecTime->size(); j++) {
				first = (*tempExecTime)[j].first;
				second = (*tempExecTime)[j].second;

				fprintf(pFile, " (%d,%d)", first, second-1);
			}
			fprintf(pFile, "\n");
		}
	}
}

void System::printExtList()
{
	map<string, External*>::iterator it;
	vector< pair<int, int> >* tempExecTime;
	int i, first, second;

	printf("\nTotal state of External Test List\n");
	for(it = ext_list.begin(); it != ext_list.end(); it++) {
		printf("External %s ", it->second->getName().c_str());
		printf("Power %d ", it->second->getPower());
		tempExecTime = it->second->getExecTime();
		for(i = 0; i < (int)tempExecTime->size(); i++) {
			first = (*tempExecTime)[i].first;
			second = (*tempExecTime)[i].second;

			if(i != (int)tempExecTime->size()-1)
				printf("(%d,%d) ", first, second);
			else
				printf("(%d,%d)\n", first, second);
		}
	}
}

void System::printBistList()
{
	map<string, BIST*>::iterator it;
	vector< pair<int, int> >* tempExecTime;
	int i, first, second;

	printf("\nTotal state of BIST Test List\n");
	for(it = bist_list.begin(); it != bist_list.end(); it++) {
		printf("BIST %s ", it->second->getName().c_str());
		printf("Power %d ", it->second->getPower());
		tempExecTime = it->second->getExecTime();
		for(i = 0; i < (int)tempExecTime->size(); i++) {
			first = (*tempExecTime)[i].first;
			second = (*tempExecTime)[i].second;

			if(i != (int)tempExecTime->size()-1)
				printf("(%d,%d) ", first, second);
			else
				printf("(%d,%d)\n", first, second);
		}
	}
}

void TAMContainer::printTAM() {
	cout << "\nThis is info about TAM!\n";
	TAMInterval *pTAM;
	priority_queue<TAMInterval*, vector<TAMInterval*>, cmp_TAM> tempTAM;
	int count=0;

	tempTAM = pqTAM;
	while(!tempTAM.empty()) {
		pTAM = tempTAM.top();	
		tempTAM.pop();
		
		count += 1;
		printf("%d.\n", count);
		printf("Time start for this state: %d\n", pTAM->timeBegin);
		printf("Time end for this state: %d\n", pTAM->timeEnd);
		printf("Range: %d ~ %d\n", pTAM->TAMBegin, pTAM->TAMEnd);
		printf("Power used by this test: %d\n", pTAM->power);
		if(pTAM->occupyExternal != NULL)
			printf("Test name: %s\n", (pTAM->occupyExternal->getName()).c_str());
		else
			printf("Test name: This TAM interval is not used!\n");
	}
}
 
void TAMContainer::printPowerStat()
{
	map<int ,int>::iterator powerIt;

	printf("\nThis is power state!\n");
	for(powerIt = powerStat.begin(); powerIt != powerStat.end(); powerIt++) {
		printf("Time: %d, Power: %d\n", powerIt->first, powerIt->second);	
	}
}

void External::printInfo()
{
	cout << "\nExternal Test Info\n";
	cout << "Test name: " << getName() << endl;	
	cout << "Power: " << getPower() << endl;
	cout << "Length: " << getLength() << endl;
	cout << "Belong to Core: " << getCore()->getName() << endl;
	printf("Interval: (%d,%d)\n", getExecTime()->back().first, getExecTime()->back().second);
	getCore()->printTAM_range();
}

void BIST::printInfo()
{
	cout << "\nBIST Test Info\n";
	cout << "Test name: " << getName() << endl;	
	cout << "Power: " << getPower() << endl;
	cout << "Length: " << getLength() << endl;
	cout << "Belong to Core: " << getCore()->getName() << endl;
	cout << "Resource: " << getRes() << endl;
	printf("Interval: (%d,%d)\n", getExecTime()->back().first, getExecTime()->back().second);
}

void Core::printTAM_range()
{
	int i, first, second;

	cout << "TAM width info\n";	
	for(i = 0; i < (int)TAM_range.size(); i++) {
		first = TAM_range[i].first;
		second = TAM_range[i].second;

		cout << i << ". [" << first << ", " << second << "]\n";
	}
}

void System::printPrecedence()
{
	map<string, Test*>::iterator it;
	int i;
	printf("\nThis is info about Precedence!\n");

	for(it = tot_list.begin(); it != tot_list.end(); it++) {
		if(it->second->getPre()->size() > 0) {
			printf("Precedence\t");
			for(i = 0; i < (int)it->second->getPre()->size(); i++) {
				printf(" %s >", (*it->second->getPre())[i]->getName().c_str());
			}
			printf(" %s\n", it->first.c_str());
		}
	}
}
