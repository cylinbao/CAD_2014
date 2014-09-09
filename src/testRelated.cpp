#include "core.h"
#include "stdio.h"
#include <iostream>

#define debug1
#define debug2

void System::fillTest()
{
	int countExt, countBist, ext_list_size, bist_list_size, intvTime, vecSize;
  vector<External*> vecExternal;
  vector<BIST*> vecBist;
	TAMInterval *pTAMInterval;
	External *pExt;
	BIST *pBist;
	pair<int, int> timePair;
	
	countExt = 0; 
	ext_list_size = ext_list.size();
	countBist = 0;
	bist_list_size = bist_list.size();

	while((countExt < ext_list_size) || (countBist < bist_list_size)) {
		pTAMInterval = TAMStat.pqTAM.top();
		if(countExt != ext_list_size) {
			vecExternal = possibleExternal(pTAMInterval);
			#ifdef debug1
			vecSize = vecExternal.size();
			printf("\nvecExternal size = %d\n", vecSize);
			#endif
			// for external
			if(!vecExternal.empty()) {
				pExt = vecExternal[0];// Only get the first one in this version
				wait_ext_list.erase( pExt->getName());// delete it from wait list

				intvTime = pTAMInterval->timeEnd;
				timePair = make_pair(intvTime, intvTime+pExt->getLength());
				pExt->getExecTime()->push_back(timePair);
				#ifdef debug2
				pExt->printInfo();
				#endif

				TAMStat.insertInterval(pExt);
				pExt->setDone();		

				TAMStat.computePower(pExt, &done_ext_list, NULL, &done_bist_list);
				done_ext_list[pExt->getName()] = pExt;
				countExt++;
			}
			else {
				TAMStat.deleteTop(&done_bist_list);
			}
		}
		#ifdef debug2
		char stop;
		cin >> stop;
		#endif
		// for BIST
		if(countBist != bist_list_size) {
			vecBist = possibleBIST(pTAMInterval);
			#ifdef debug1
			vecSize = vecBist.size();
			printf("\nvecBist size = %d\n", vecSize);
			#endif
			if(!vecBist.empty()) {
				pBist = vecBist[0];
				wait_bist_list.erase( pBist->getName());// delete it from wait list

				intvTime = pTAMInterval->timeEnd;
				timePair = make_pair(intvTime, intvTime+pBist->getLength());
				pBist->getExecTime()->push_back(timePair);
				#ifdef debug2
				pBist->printInfo();
				#endif

				TAMStat.insertBist(pBist, &res_list);
				pBist->setDone();

				TAMStat.computePower(NULL, &done_ext_list, pBist, &done_bist_list);
				done_bist_list[pBist->getName()] = pBist;
				countBist++;
			}
			else{
				if(countExt == ext_list_size)
					TAMStat.deleteTop(&done_bist_list);
			}
		}
		#ifdef debug2
		cin >> stop;
		#endif
	}
}

// Accroding to one interval(TAM with least time), select possible Tests
vector<External*> System::possibleExternal(TAMInterval *pTAMInterval)
{
  vector<External*> vecExternal;
	map<string, External*>::iterator itExt;
	map<string, BIST*>::iterator itBist;
	External *pExt;
	int TAM_range_size;
	bool flag1;

	for(itExt = wait_ext_list.begin(); itExt != wait_ext_list.end(); itExt++) {
		pExt = itExt->second;	
		// check this test done or not
		if(pExt->checkDone())
			continue;
		// check if all precedence done	
		if(!pExt->checkPreDone(pTAMInterval))
			continue;
		// check power will not exceed limit	
		if(!TAMStat.checkPower(pTAMInterval, pExt, NULL, tot_power, &done_ext_list, &done_bist_list))
			continue;

		flag1 = true;
		int timeBegin = pTAMInterval->timeEnd;
		int timeEnd = timeBegin + pExt->getLength();

		for(itBist = itExt->second->getCore()->bist_list.begin(); 
				itBist != itExt->second->getCore()->bist_list.end(); itBist++)
		{
			if(itBist->second->checkDone()) {
				if((timeEnd <= itBist->second->getExecTime()->back().first) ||
						(timeBegin >= itBist->second->getExecTime()->back().second))
				{
				}
				else {
					flag1 = false;
					break;
				}
			}
		}	
		if(flag1 == false)
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
						(tempTAMStat.pqTAM.top()->timeEnd == pTAMInterval->timeEnd))
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

vector<BIST*> System::possibleBIST(TAMInterval *pTAMInterval)
{
  vector<BIST*> vecBist;
	map<string, BIST*>::iterator itBist;
	map<string, External*>::iterator itExt;
	BIST *pBist;
	bool flag;
	
	for(itBist = wait_bist_list.begin(); itBist != wait_bist_list.end(); itBist++) {
		pBist = itBist->second;

		// check this test done or not
    if(pBist->checkDone())
      continue;
    // check if all precedence done
    if(!pBist->checkPreDone(pTAMInterval))
      continue;
		// check resource is useable or not
		if(pBist->getRes() != "") {
			if(res_list[pBist->getRes()]->getTime() > pTAMInterval->timeEnd)
				continue;
		}
    // check power will not exceed limit
    if(!TAMStat.checkPower(pTAMInterval, NULL, pBist, tot_power, &done_ext_list, &done_bist_list))
      continue;
		
		flag = true;
		int timeBegin = pTAMInterval->timeEnd;
		int timeEnd = timeBegin + pBist->getLength();

		for(itExt = itBist->second->getCore()->ext_list.begin(); 
				itExt != itBist->second->getCore()->ext_list.end(); itExt++)
		{
			if(itExt->second->checkDone()) {
				if((timeEnd <= itExt->second->getExecTime()->back().first) ||
						(timeBegin >= itExt->second->getExecTime()->back().second))
				{
				}
				else {
					flag = false;
					break;
				}
			}
		}	

		if(flag == true)
			vecBist.push_back(pBist);
	}

  return vecBist;
}
