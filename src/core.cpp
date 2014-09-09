#include "core.h"
#include "stdio.h"
#include "string.h"

//#define debug1
//#define debug2

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
}

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
				#endif
				break;
			}
		}
	}
}

void TAMContainer::insertBist(BIST *pBist, map<string, Resource*>* p_res_list)
{
	if(pBist->getRes() != "") {
		(*p_res_list)[pBist->getRes()]->setTime(pBist->getExecTime()->back().second);
	}
}

void TAMContainer::deleteTop(map<string, BIST*> *done_bist_list)
{
	TAMInterval *pTAMInterval1, *pTAMInterval2, *newTAMInterval;
	vector<TAMInterval*> storeTAMIntv;
	int j;

	pTAMInterval1 = pqTAM.top();
	pqTAM.pop();
	if(!pqTAM.empty()) {
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
	}
	else {
		map<string, BIST*>::iterator itBist;
		int min;
		bool flag;

		flag = true;
		for(itBist = done_bist_list->begin(); itBist != done_bist_list->end(); itBist++) {
			if(itBist->second->getExecTime()->back().second > pTAMInterval1->timeEnd) {
				if(flag == true) {
					min = itBist->second->getExecTime()->back().second;
					flag = false;
				}
				else {
					if(min > itBist->second->getExecTime()->back().second)
						min = itBist->second->getExecTime()->back().second;
				}
			}
		}

		newTAMInterval = new TAMInterval(pTAMInterval1->TAMBegin, pTAMInterval1->TAMEnd,
																		 pTAMInterval1->timeEnd, min, 0, NULL);
		pqTAM.push(newTAMInterval);
	}
	#ifdef debug1
	printTAM();
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

bool Test::checkPreDone(TAMInterval *pTAMInterval)
{
	unsigned int i;
	bool flag;

	flag = true;
	for(i=0; i<pre.size(); i++) {
		if(!pre[i]->checkDone()){
			flag = false;
			break;
		}
		else if(pTAMInterval->timeEnd < pre[i]->execTime.back().second) {
			flag = false;
			break;
		}
	}

	return flag;
}

