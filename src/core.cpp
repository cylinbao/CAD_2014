#include "core.h"
#include "stdio.h"
#include "iostream"

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
// ------------ Work on this function ---------------------------------------//
void TAMContainer::insertInterval(External *pExt)
{
/*
	TAMInterval *pTAMInterval;

	pTAMInterval = TAMStat.pqTAM.top()
	TAMStat.pqTAM.pop();

	if(pExt->)
*/
}

void TAMContainer::deleteTop()
{

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

