#include "core.h"

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

		if(!vecExternal.empty()) {
			pExt = vecExternal[0];// Only get the first one in this version
			intvTime = pTAMInterval->time;
			timePair = make_pair(intvTime, intvTime+pExt->getLength());
			pExt->getExecTime()->push_back(timePair);

			TAMStat.insertInterval(pExt);
			
			count++;
		}
		else {
			TAMStat.deleteTop();
		}
	}
}

// Accroding to one interval(TAM with least time), select possible Tests
vector<External*> System::possibleExternal(TAMInterval *pTAMInterval)
{
  vector<External*> vecExternal;
	map<string, External*> inerExtList;
	map<string, External*>::iterator itExt;
	External *pExt;
	//vector< pair<int, int> >::iterator itTAMRange;
	//int begin, end;

	inerExtList = ext_list;
	for(itExt = inerExtList.begin(); itExt != inerExtList.end(); itExt++) {
		pExt = itExt->second;	
		// check this test done or not
		if(pExt->checkDone())
			break;
		// check if all precedence done	
		if(!pExt->checkPreDone())
			break;
		// check power will not exceed limit	
		int extPower = pExt->getPower();
		if((extPower + TAMStat.totPower) > tot_power)
			break;
		// if there are empty TAM can used by this ext test
		/*
		for(itTAMRange = pExt->core->TAM_range->begin(); 
				itTAMRange != pExt->core->TAM_range->end(); itTAMRange++) {
			begin = itTAMRange->first;
			end = itTAMRange->second;
		}
		*/
		if(pExt->getCore()->getTAM_range().size() == 1) {
			if((pExt->getCore()->getTAMBegin() >= pTAMInterval->TAMBegin) && 
					(pExt->getCore()->getTAMEnd() <= pTAMInterval->TAMEnd))
				vecExternal.push_back(pExt);
		}
		else { // for the tests who are seperate into several TAM assignment
			vector<TAMInterval*> storeTAMIntv;
			bool *flag, flagExt;
		
			storeTAMIntv.push_back(pTAMInterval);
			TAMStat.pqTAM.pop();

			while(TAMStat.pqTAM.top()->time == pTAMInterval->time) {
				storeTAMIntv.push_back(TAMStat.pqTAM.top());
				TAMStat.pqTAM.pop();
			}

			for(int i=0; i < (int)storeTAMIntv.size(); i++) {
				TAMStat.pqTAM.push(storeTAMIntv[i]);
			}

			//////////////////////////////////////////////
			// check this ext can be candidate or not   //
			//////////////////////////////////////////////
			flag = new bool [pExt->getCore()->getTAM_range().size()];
			for(int i=0; i < (int)pExt->getCore()->getTAM_range().size(); i++) {
				flag[i] = false;
			}
			
			for(int i=0; i < (int)pExt->getCore()->getTAM_range().size(); i++) {
				for(int j=0; j < (int)storeTAMIntv.size(); j++) {
					if((pExt->getCore()->getTAM_range()[i].first >= storeTAMIntv[j]->TAMBegin) && 
							(pExt->getCore()->getTAM_range()[i].second <= storeTAMIntv[j]->TAMEnd)) {
						vecExternal.push_back(pExt);
						flag[i] = true;
						break;
					}
				}
			}

			flagExt = true;
			for(int i=0; i < (int)pExt->getCore()->getTAM_range().size(); i++) {
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
