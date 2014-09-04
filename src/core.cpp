#include "core.h"
#include "stdio.h"

//#define debug1

void System::printTAMAssignment()
{
	int i, j, first, second;
	vector<pair<int, int> >  range;

	for(i=0; i<(int)core.size(); i++) {
		cout << "Core name: " << core[i]->getName() << endl;
		range = core[i]->getTAM_range();

		for(j=0; j<(int)range.size(); j++) {
			first = range[j].first;	
			second = range[j].second;	

			cout << j << ". [" << first << ", " << second << "]\n";
		}
	}
}

void System::printExtList()
{
	map<string, External*>::iterator it;
	vector< pair<int, int> >* tempExecTime;
	int i, first, second;

	for(it = ext_list.begin(); it != ext_list.end(); it++) {
		printf("External %s ", it->second->getName().c_str());
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

TAMInterval::TAMInterval(int front, int back, int timeStat, int energy,
													 External *p_External){
	TAMBegin = front;
	TAMEnd = back;
	time = timeStat;
	power = energy;
	width = TAMBegin - TAMEnd;
	occupyExternal = p_External;
}

void TAMContainer::initTAM(int tot_TAM_width) {
	while(!pqTAM.empty())
		pqTAM.pop();
	TAMInterval *pTAMInterval = new TAMInterval(tot_TAM_width);
	pqTAM.push(pTAMInterval);
}

void TAMContainer::printTAM() {
	cout << "\nThis is info about TAM!\n";
	TAMInterval *pTAM;
	priority_queue<TAMInterval*, vector<TAMInterval*>, cmp_TAM> tempTAM;

	tempTAM = pqTAM;
	while(!tempTAM.empty()) {
		pTAM = tempTAM.top();	
		tempTAM.pop();

		printf("Time for this state: %d\n", pTAM->time);
		printf("Range: %d ~ %d\n", pTAM->TAMBegin, pTAM->TAMEnd);
		printf("Power used by this test: %d\n", pTAM->power);
		if(pTAM->occupyExternal != NULL)
			printf("External test name: %s\n", (pTAM->occupyExternal->getName()).c_str());
		else
			printf("External test name: This TAM interval is not used!\n");
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
																				(pTAMInterval->time)+(pExt->getLength()), 
																				 pExt->getPower(), pExt);
				pqTAM.push(newTAMInterval);

				if(TAM_range[i].first > pTAMInterval->TAMBegin) {
					newTAMInterval = new TAMInterval(pTAMInterval->TAMBegin, (TAM_range[i].first-1), 
																					 pTAMInterval->time, pTAMInterval->power,
																					 pTAMInterval->occupyExternal);
					pqTAM.push(newTAMInterval);
				}

				if(TAM_range[i].second < pTAMInterval->TAMEnd) {
					newTAMInterval = new TAMInterval((TAM_range[i].second+1), pTAMInterval->TAMEnd,
																					 pTAMInterval->time, pTAMInterval->power,
																					 pTAMInterval->occupyExternal);
					pqTAM.push(newTAMInterval);
				}
			}
			else {
				storeTAMIntv.push_back(pTAMInterval);
			}

			if((pqTAM.empty()) || (pqTAM.top()->time != pTAMInterval->time)) {
				for(j = 0; j < (int)storeTAMIntv.size(); j++) {
					pqTAM.push(storeTAMIntv[j]);	
				}
				#ifdef debug1
				printTAM();
				#endif
				break;
			}
		}
	}
}
//*/

void TAMContainer::deleteTop()
{
	TAMInterval *pTAMInterval1, *pTAMInterval2, *newTAMInterval;
	vector<TAMInterval*> storeTAMIntv;
	int j;

	pTAMInterval1 = pqTAM.top();
	pqTAM.pop();
	while((!pqTAM.empty()) && (pqTAM.top()->time == pTAMInterval1->time)) {
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
																		 pTAMInterval2->time, pTAMInterval2->power,
																		 pTAMInterval2->occupyExternal);
		pqTAM.pop();
		pqTAM.push(newTAMInterval);
	}
	else if(pTAMInterval2->TAMEnd+1 == pTAMInterval1->TAMBegin) {
		newTAMInterval = new TAMInterval(pTAMInterval2->TAMBegin, pTAMInterval1->TAMEnd,
																		 pTAMInterval2->time, pTAMInterval2->power,
																		 pTAMInterval2->occupyExternal);
		pqTAM.pop();
		pqTAM.push(newTAMInterval);
	}
	else {
		newTAMInterval = new TAMInterval(pTAMInterval1->TAMBegin, pTAMInterval1->TAMEnd,
																		 pTAMInterval2->time, 0, NULL);
		pqTAM.push(newTAMInterval);
	}

	for(j = 0; j < (int)storeTAMIntv.size(); j++) {
		pqTAM.push(storeTAMIntv[j]);	
	}

	#ifdef debug1
	printTAM();
	#endif
}

void TAMContainer::computeTotPower()
{
	priority_queue<TAMInterval*, vector<TAMInterval*>, cmp_TAM> tempPqTAM;
	map<string, External*> computedExt;
	TAMInterval *pTAMInterval;
	int count;

	tempPqTAM = pqTAM;
	count = 0;

	while(!tempPqTAM.empty()) {
		pTAMInterval = tempPqTAM.top();
		tempPqTAM.pop();

		if((pTAMInterval->occupyExternal != NULL) && 
			 (computedExt.find(pTAMInterval->occupyExternal->getName()) == computedExt.end())) 
		{
			count += pTAMInterval->power;
			computedExt[pTAMInterval->occupyExternal->getName()] = pTAMInterval->occupyExternal;
		}
	}

	totPower = count;
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
				(tempPqTAM.top()->time == pTAMInterval->time))
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
