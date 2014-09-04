#include "core.h"
#include "stdio.h"
#include <iostream>

//#define debug1
//#define debug2

void System::fillTest()
{
	int count, ext_list_size, intvTime;
  vector<External*> vecExternal;
	TAMInterval *pTAMInterval;
	External *pExt;
	pair<int, int> timePair;
	
	count = 0;
	ext_list_size = ext_list.size();

	while(count < ext_list_size) {
		pTAMInterval = TAMStat.pqTAM.top();
		vecExternal = possibleExternal(pTAMInterval);
		#ifdef debug1
		int vecSize = vecExternal.size();
		printf("vecExternal size = %d\n", vecSize);
		#endif

		if(!vecExternal.empty()) {
			pExt = vecExternal[0];// Only get the first one in this version
			wait_ext_list.erase( pExt->getName());// delete it from wait list

			intvTime = pTAMInterval->time;
			timePair = make_pair(intvTime, intvTime+pExt->getLength());
			pExt->getExecTime()->push_back(timePair);

			TAMStat.insertInterval(pExt);
			pExt->setDone();		
			
			#ifdef debug2
			pExt->printInfo();
			#endif
			count++;
		}
		else {
			TAMStat.deleteTop();
		}
		#ifdef debug2
		char stop;
		//TAMStat.printTAM();
		cin >> stop;
		#endif

		TAMStat.computeTotPower();
	}
}

// Accroding to one interval(TAM with least time), select possible Tests
vector<External*> System::possibleExternal(TAMInterval *pTAMInterval)
{
  vector<External*> vecExternal;
	map<string, External*>::iterator itExt;
	External *pExt;
	int TAM_range_size;

	for(itExt = wait_ext_list.begin(); itExt != wait_ext_list.end(); itExt++) {
		pExt = itExt->second;	
		// check this test done or not
		if(pExt->checkDone())
			continue;
		// check if all precedence done	
		if(!pExt->checkPreDone())
			continue;
		// check power will not exceed limit	
		int extPower = pExt->getPower();
		if((extPower + TAMStat.totPower - pTAMInterval->power) > tot_power)
			continue;

		TAM_range_size = pExt->getCore()->getTAM_range().size();
		if(TAM_range_size == 1) {
			if((pExt->getCore()->getTAMBegin() >= pTAMInterval->TAMBegin) && 
					(pExt->getCore()->getTAMEnd() <= pTAMInterval->TAMEnd))
				vecExternal.push_back(pExt);
		}
		else { // for the tests who are seperate into several TAM assignment
			vector<TAMInterval*> storeTAMIntv;
			bool *flag, flagExt;
			TAMContainer tempTAMStat;
		
			tempTAMStat = TAMStat;
			storeTAMIntv.push_back(pTAMInterval);
			tempTAMStat.pqTAM.pop();

			while((!tempTAMStat.pqTAM.empty()) && 
						(tempTAMStat.pqTAM.top()->time == pTAMInterval->time))
			{
				storeTAMIntv.push_back(TAMStat.pqTAM.top());
				tempTAMStat.pqTAM.pop();
			}

			//////////////////////////////////////////////
			// check this ext can be candidate or not   //
			//////////////////////////////////////////////
			flag = new bool [TAM_range_size];
			for(int i=0; i < TAM_range_size; i++) {
				flag[i] = false;
			}
			
			for(int i=0; i < TAM_range_size; i++) {
				for(int j=0; j < (int)storeTAMIntv.size(); j++) {
					if((pExt->getCore()->getTAM_range()[i].first >= storeTAMIntv[j]->TAMBegin) && 
							(pExt->getCore()->getTAM_range()[i].second <= storeTAMIntv[j]->TAMEnd)) {
						flag[i] = true;
						break;
					}
				}
			}

			flagExt = true;
			for(int i=0; i < TAM_range_size; i++) {
				if(flag[i] == false)
					flagExt = false;
			}

			if(flagExt == true)
				vecExternal.push_back(pExt);
		}
	}

  return vecExternal;
}

vector<BIST*> System::possibleBIST()
{
  vector<BIST*> vecBIST;

  return vecBIST;
}
