#include "core.h"
#include "stdio.h"

//#define debug1
//#define debug2

void System::printResult()
{
	printTAMAssignment();
	printTest();
}

void System::printTAMAssignment()
{
	int i, j, first, second;
	vector<pair<int, int> >  range;

	for(i=0; i<(int)core.size(); i++) {
		cout << "\nTAM_assignment " << core[i]->getName();
		range = core[i]->getTAM_range();

		for(j=0; j<(int)range.size(); j++) {
			first = range[j].first;	
			second = range[j].second;	

			cout << " [" << first << ":" << second << "]";
		}
		cout << endl;
	}
}

void System::printTest()
{
	map<string, External*>::iterator itExt;
	map<string, BIST*>::iterator itBist;
	Core *pCore;
	int i, j, first, second;
	vector< pair<int, int> > *tempExecTime;

	for(i=0; i<(int)core.size(); i++) {
		pCore = core[i];
		for(itExt = pCore->ext_list.begin(); itExt != pCore->ext_list.end(); itExt++) {
			printf("\nExternal %s %s", pCore->getName().c_str(), itExt->second->getName().c_str());
			tempExecTime = itExt->second->getExecTime();
			for(j = 0; j < (int)tempExecTime->size(); j++) {
				first = (*tempExecTime)[j].first;
				second = (*tempExecTime)[j].second;

				printf(" (%d,%d)", first, second);
			}
			printf("\n");
		}

		for(itBist = pCore->bist_list.begin(); itBist != pCore->bist_list.end(); itBist++) {
			printf("\nBIST %s %s", pCore->getName().c_str(), itBist->second->getName().c_str());
			tempExecTime = itBist->second->getExecTime();
			for(j = 0; j < (int)tempExecTime->size(); j++) {
				first = (*tempExecTime)[j].first;
				second = (*tempExecTime)[j].second;

				printf(" (%d,%d)", first, second);
			}
			printf("\n");
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

TAMInterval::TAMInterval(int front, int back, int time1, int time2, int energy,
													 External *p_External){
	TAMBegin = front;
	TAMEnd = back;
	timeBegin = time1;
	timeEnd = time2;
	power = energy;
	width = TAMBegin - TAMEnd;
	occupyExternal = p_External;
}

void TAMContainer::initTAM(int tot_TAM_width) {
	while(!pqTAM.empty())
		pqTAM.pop();
	TAMInterval *pTAMInterval = new TAMInterval(tot_TAM_width);
	pqTAM.push(pTAMInterval);
	//powerStat[0] = 0;
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
			printf("External test name: %s\n", (pTAM->occupyExternal->getName()).c_str());
		else
			printf("External test name: This TAM interval is not used!\n");
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

///*
void TAMContainer::insertInterval(External *pExt)
{
	priority_queue<TAMInterval*, vector<TAMInterval*>, cmp_TAM> tempPqTAM;
	vector<pair<int, int> >  TAM_range;
	TAMInterval *pTAMInterval, *newTAMInterval;
	int i, j, TAM_range_size;

	TAM_range = pExt->getCore()->getTAM_range();
	TAM_range_size = TAM_range.size();
	for(i = 0; i < TAM_range_size; i++) {
		vector<TAMInterval*> storeTAMIntv; 
		while(1) {
			pTAMInterval = pqTAM.top();
			pqTAM.pop();
			
			if((TAM_range[i].first >= pTAMInterval->TAMBegin) &&
				 (TAM_range[i].second <= pTAMInterval->TAMEnd))
			{
				// conform the conditions, insert it
				newTAMInterval = new TAMInterval(TAM_range[i].first, TAM_range[i].second, 
																				 pTAMInterval->timeEnd, (pTAMInterval->timeEnd)
																				 +(pExt->getLength()), pExt->getPower(), pExt);
				pqTAM.push(newTAMInterval);

				if(TAM_range[i].first > pTAMInterval->TAMBegin) {
					newTAMInterval = new TAMInterval(pTAMInterval->TAMBegin, (TAM_range[i].first-1), 
																					 pTAMInterval->timeBegin, pTAMInterval->timeEnd, 
																					 pTAMInterval->power, pTAMInterval->occupyExternal);
					pqTAM.push(newTAMInterval);
				}

				if(TAM_range[i].second < pTAMInterval->TAMEnd) {
					newTAMInterval = new TAMInterval((TAM_range[i].second+1), pTAMInterval->TAMEnd,
																					 pTAMInterval->timeBegin, pTAMInterval->timeEnd,
																					 pTAMInterval->power, pTAMInterval->occupyExternal);
					pqTAM.push(newTAMInterval);
				}
			}
			else {
				storeTAMIntv.push_back(pTAMInterval);
			}

			if((pqTAM.empty()) || (pqTAM.top()->timeEnd != pTAMInterval->timeEnd)) {
				for(j = 0; j < (int)storeTAMIntv.size(); j++) {
					pqTAM.push(storeTAMIntv[j]);	
				}
				#ifdef debug1
				printTAM();
				char stop;
				cin >> stop;
				#endif
				break;
			}
		}
	}
}
//*/

void TAMContainer::insertBist(BIST *pBist, map<string, Resource*>* p_res_list)
{
	if(pBist->getRes() != "") {
		(*p_res_list)[pBist->getRes()]->setTime(pBist->getExecTime()->back().second);
	}
}

void TAMContainer::deleteTop()
{
	TAMInterval *pTAMInterval1, *pTAMInterval2, *newTAMInterval;
	vector<TAMInterval*> storeTAMIntv;
	int j;

	pTAMInterval1 = pqTAM.top();
	pqTAM.pop();
	while((!pqTAM.empty()) && (pqTAM.top()->timeEnd == pTAMInterval1->timeEnd)) {
		if((pTAMInterval1->TAMEnd+1 != pqTAM.top()->TAMBegin) && 
				(pqTAM.top()->TAMEnd+1 != pTAMInterval1->TAMBegin)) 
		{
			storeTAMIntv.push_back(pqTAM.top());
			pqTAM.pop();
		}
		else {
			break;
		}
	}

	pTAMInterval2 = pqTAM.top();

	if(pTAMInterval1->TAMEnd+1 == pTAMInterval2->TAMBegin) {
		newTAMInterval = new TAMInterval(pTAMInterval1->TAMBegin, pTAMInterval2->TAMEnd,
																		 pTAMInterval2->timeBegin, pTAMInterval2->timeEnd,
																		 pTAMInterval2->power, pTAMInterval2->occupyExternal);
		pqTAM.pop();
		pqTAM.push(newTAMInterval);
	}
	else if(pTAMInterval2->TAMEnd+1 == pTAMInterval1->TAMBegin) {
		newTAMInterval = new TAMInterval(pTAMInterval2->TAMBegin, pTAMInterval1->TAMEnd,
																		 pTAMInterval2->timeBegin, pTAMInterval2->timeEnd,
																		 pTAMInterval2->power, pTAMInterval2->occupyExternal);
		pqTAM.pop();
		pqTAM.push(newTAMInterval);
	}
	else {
		newTAMInterval = new TAMInterval(pTAMInterval1->TAMBegin, pTAMInterval1->TAMEnd,
																		 pTAMInterval2->timeBegin, pTAMInterval2->timeEnd,
																		 0, NULL);
		pqTAM.push(newTAMInterval);
	}

	for(j = 0; j < (int)storeTAMIntv.size(); j++) {
		pqTAM.push(storeTAMIntv[j]);	
	}

	#ifdef debug1
	printTAM();
	char stop;
	cin >> stop;
	#endif
}

void TAMContainer::computePower(External *pExt, map<string, External*>* pDoneExtList,
																BIST *pBist, map<string, BIST*>* pDoneBistList)
{
	map<int ,int>::iterator powerIt;	
	map<string, External*>::iterator doneItExt;
	map<string, BIST*>::iterator doneItBist;
	int count;

	if(pExt != NULL) {
		for(powerIt = powerStat.begin(); powerIt != powerStat.end(); powerIt++) {
			if((powerIt->first <= pExt->getExecTime()->back().second) && 
					(powerIt->first > pExt->getExecTime()->back().first))
			{
				powerIt->second += pExt->getPower();
			}
		}

		if(powerStat.find(pExt->getExecTime()->back().second) == powerStat.end()) {
			count = 0;
			for(doneItExt = pDoneExtList->begin(); doneItExt != pDoneExtList->end(); doneItExt++) {
				if((pExt->getExecTime()->back().second > doneItExt->second->getExecTime()->back().first) && 
						(pExt->getExecTime()->back().second <= doneItExt->second->getExecTime()->back().second))
				{
					count += doneItExt->second->getPower();
				}
			}

			for(doneItBist = pDoneBistList->begin(); doneItBist != pDoneBistList->end(); doneItBist++) {
				if((pExt->getExecTime()->back().second > doneItBist->second->getExecTime()->back().first) && 
						(pExt->getExecTime()->back().second <= doneItBist->second->getExecTime()->back().second))
				{
					count += doneItBist->second->getPower();
				}
			}

			powerStat[pExt->getExecTime()->back().second] = count + pExt->getPower();
		}
	}
	else if(pBist != NULL) {
		for(powerIt = powerStat.begin(); powerIt != powerStat.end(); powerIt++) {
			if((powerIt->first <= pBist->getExecTime()->back().second) && 
					(powerIt->first > pBist->getExecTime()->back().first))
			{
				powerIt->second += pBist->getPower();
			}
		}

		if(powerStat.find(pBist->getExecTime()->back().second) == powerStat.end()) {
			count = 0;
			for(doneItExt = pDoneExtList->begin(); doneItExt != pDoneExtList->end(); doneItExt++) {
				if((pBist->getExecTime()->back().second > doneItExt->second->getExecTime()->back().first) && 
						(pBist->getExecTime()->back().second <= doneItExt->second->getExecTime()->back().second))
				{
					count += doneItExt->second->getPower();
				}
			}

			for(doneItBist = pDoneBistList->begin(); doneItBist != pDoneBistList->end(); doneItBist++) {
				if((pBist->getExecTime()->back().second > doneItBist->second->getExecTime()->back().first) && 
						(pBist->getExecTime()->back().second <= doneItBist->second->getExecTime()->back().second))
				{
					count += doneItBist->second->getPower();
				}
			}

			powerStat[pBist->getExecTime()->back().second] = count + pBist->getPower();
		}
	}

	#ifdef debug2
	printPowerStat();
	#endif
}

bool TAMContainer::checkPower(TAMInterval *pTAMInterval, External *pExt, BIST *pBist, int limit, 
															map<string, External*>* pDoneExtList, map<string, BIST*>* pDoneBistList)
{
	map<string, External*>::iterator doneItExt;
	map<string, BIST*>::iterator doneItBist;
	bool flag;
	int timeBegin, timeEnd, count;

	count = 0;
	timeBegin = pTAMInterval->timeEnd;
	if((pExt != NULL) && (pBist == NULL)) 
		timeEnd = timeBegin + pExt->getLength();
	else if((pExt == NULL) && (pBist != NULL)) 
		timeEnd = timeBegin + pBist->getLength();

	for(doneItExt = pDoneExtList->begin(); doneItExt != pDoneExtList->end(); doneItExt++) {
		if((timeEnd <= doneItExt->second->getExecTime()->back().first) ||
			 (timeBegin >= doneItExt->second->getExecTime()->back().second))
		{
		}
		else
			count += doneItExt->second->getPower();
	}

	for(doneItBist = pDoneBistList->begin(); doneItBist != pDoneBistList->end(); doneItBist++) {
		if((timeEnd <= doneItBist->second->getExecTime()->back().first) ||
			 (timeBegin >= doneItBist->second->getExecTime()->back().second))
		{
		}
		else
			count += doneItBist->second->getPower();
	}

	flag = true;
	if((pExt != NULL) && (pBist == NULL)) {
		if((count + pExt->getPower()) > limit)
			flag = false;
	}
	else if((pExt == NULL) && (pBist != NULL)) {
		if((count + pBist->getPower()) > limit)
			flag = false;
	}

	return flag;
}

vector<TAMInterval*> TAMContainer::getTopIntvWithSameTime()
{
	priority_queue<TAMInterval*, vector<TAMInterval*>, cmp_TAM> tempPqTAM;
	vector<TAMInterval*> storeTAMIntv; 
	TAMInterval *pTAMInterval;

	tempPqTAM = pqTAM;
	pTAMInterval = tempPqTAM.top();
	storeTAMIntv.push_back(pTAMInterval);
	tempPqTAM.pop();
	// store those intervals which have the same time
	while((!tempPqTAM.empty()) && 
				(tempPqTAM.top()->timeEnd == pTAMInterval->timeEnd))
	{
		pTAMInterval = tempPqTAM.top();
		storeTAMIntv.push_back(pTAMInterval);
		tempPqTAM.pop();
	}

	return storeTAMIntv;
}

bool Test::checkPreDone()
{
	unsigned int i;
	bool flag;

	flag = true;
	for(i=0; i<pre.size(); i++) {
		if(!pre[i]->checkDone()) {
			flag = false;
			break;
		}
	}

	return flag;
}

void External::printInfo()
{
	cout << "\nThis is info about an External test!\n";
	cout << "Test name: " << getName() << endl;	
	cout << "Power: " << getPower() << endl;
	cout << "Length: " << getLength() << endl;
	cout << "Belong to Core: " << getCore()->getName() << endl;
	printf("Interval: (%d,%d)\n", getExecTime()->back().first, getExecTime()->back().second);
	getCore()->printTAM_range();
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
