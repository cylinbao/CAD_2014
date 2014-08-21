#include "core.h"

// Accroding to one interval(TAM with least time), select possible Tests
vector<External*> System::possibleExternal(TAMInterval *pTAMInterval)
{
  vector<External*> vecExternal;
	map<string, External*> inerExtList;
	map<string, External*>::iterator it;
	External *pExt;

	inerExtList = ext_list;
	for(it=inerExtList.begin(); it!=inerExtList.end(); it++) {
		pExt = it->second;	
		// check this test done or not
		if(pExt->checkDone())
			break;
		// check if all precedence done	
		if(!pExt->checkPreDone())
			break;
		// check power will not exceed limit	
		int extPower=pExt->getPower();
		if((extPower + TAM.totPower) > tot_power)
			break;
		// if there are empty TAM can used by this ext test
		if((pExt->getCore()->getTAMBegin() >= pTAMInterval->TAMBegin) && 
			 (pExt->getCore()->getTAMEnd() <= pTAMInterval->TAMEnd))
			vecExternal.push_back(pExt);
	}

  return vecExternal;
}

vector<BIST*> System::possibleBIST()
{
  vector<BIST*> vecBIST;

  return vecBIST;
}
