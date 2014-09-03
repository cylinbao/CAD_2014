#include "TAM_assign.h"

void coreAssign(Core* core, System& sys, Complement& complement)
{
	map<int, map<int, Interval*> >::iterator it_int_1;
	map<int, Interval*>::iterator it_int_2;
	while(core != NULL && core->getDone() == false){
		int ext_length = core->getExtLength();
		if(!bottomAssign(core, sys, complement, ext_length)){
			while(ext_length != 0){
				bool find = false;
				for(it_int_1 = complement.list.begin(); it_int_1 != complement.list.end(); it_int_1++){
					for(it_int_2 = it_int_1->second.begin(); it_int_2 != it_int_1->second.end(); it_int_2++){
						if(it_int_1->first >= core->getCoreTW() && (core->initTAM() == 0 || it_int_1->second.begin()->second->begin == core->getTAMBegin())){
							if(it_int_1 != complement.list.end() && it_int_1->first == core->getCoreTW()){
								if(matchedAssign(core, it_int_1, it_int_2, sys, complement, ext_length)){
									find = true;
									break;
								}
							}
							else{
								if(noMatchedAssign(core, it_int_1, it_int_2, sys, complement, ext_length)){
									find = true;
									break;
								}
							}
						}
					}
					if(find == true)
						break;
				}
				if(!find){
					if(!randomAssign(core, sys, complement, ext_length))
						fixedAssign(core, sys, complement, ext_length);
				}
			}
			core = core->getSameExtLength();
		}
	}
}

bool noMatchedAssign(Core* core, map<int, map<int, Interval*> >::iterator it_int_1, map<int, Interval*>::iterator it_int_2, System& sys, Complement& complement, int& ext_length)
{
	bool fit_avg = true;
	if(it_int_1 != complement.list.end()){
		for(int i = it_int_2->second->begin; i < it_int_2->second->begin + core->getCoreTW(); i++){
			if(ext_length + sys.TAM[i] > sys.getTAMAvg()){
				fit_avg = false;
				break;
			}
		}
	}
	if(it_int_1 != complement.list.end() && fit_avg){
		core->setTAMRange(it_int_2->second->begin, it_int_2->second->begin + core->getCoreTW() - 1);
		if(ext_length - it_int_2->second->length > 0){
			sys.modTAM(it_int_2->second->begin, it_int_2->second->begin + core->getCoreTW() - 1, it_int_2->second->length);
			cout<<"Split: "<<core->getCoreTW()<<endl;
			cout<<"Begin: "<<it_int_2->second->begin<<endl;
			cout<<"End: "<<it_int_2->second->begin + core->getCoreTW() - 1<<endl;
			cout<<"Width: "<<core->getCoreTW()<<endl;
			cout<<"Length Added: "<<it_int_2->second->length<<endl;
			cout<<endl;
			Interval* tmp = it_int_2->second;
			ext_length -= it_int_2->second->length;
			complement.complementErase(it_int_1->first, it_int_2);
			complement.complementAdd(tmp->width - core->getCoreTW(), tmp->begin + core->getCoreTW(), tmp->end, tmp->length);
			delete tmp;
			return true;
		}
		else if(ext_length - it_int_2->second->length < 0){
			sys.modTAM(it_int_2->second->begin, it_int_2->second->begin + core->getCoreTW() - 1, ext_length);
			cout<<"Split: "<<core->getCoreTW()<<endl;
			cout<<"Begin: "<<it_int_2->second->begin<<endl;
			cout<<"End: "<<it_int_2->second->begin + core->getCoreTW() - 1<<endl;
			cout<<"Width: "<<core->getCoreTW()<<endl;
			cout<<"Length Added: "<<ext_length<<endl;
			cout<<endl;
			Interval* tmp = it_int_2->second;
			complement.complementErase(it_int_1->first, it_int_2);
			complement.complementAdd(core->getCoreTW(), tmp->begin, tmp->begin + core->getCoreTW() - 1, tmp->length - ext_length);
			complement.complementAdd(tmp->width - core->getCoreTW(), tmp->begin + core->getCoreTW(), tmp->end, tmp->length);
			ext_length = 0;
			core->setDone(true);
			delete tmp;
			return true;
		}
		else if(ext_length - it_int_2->second->length == 0){
			sys.modTAM(it_int_2->second->begin, it_int_2->second->begin + core->getCoreTW() - 1, ext_length);
			cout<<"Split: "<<core->getCoreTW()<<endl;
			cout<<"Begin: "<<it_int_2->second->begin<<endl;
			cout<<"End: "<<it_int_2->second->begin + core->getCoreTW() - 1<<endl;
			cout<<"Width: "<<core->getCoreTW()<<endl;
			cout<<"Length Added: "<<ext_length<<endl;
			cout<<endl;
			Interval* tmp = it_int_2->second;
			complement.complementErase(it_int_1->first, it_int_2);
			complement.complementAdd(tmp->width - core->getCoreTW(), tmp->begin + core->getCoreTW(), tmp->end, tmp->length);
			ext_length = 0;
			core->setDone(true);
			delete tmp;
			return true;
		}
	}
	return false;
}

bool matchedAssign(Core* core, map<int, map<int, Interval*> >::iterator it_int_1, map<int, Interval*>::iterator it_int_2, System& sys, Complement& complement, int& ext_length)
{
	bool fit_avg = true;
	for(int i = it_int_2->second->begin; i < it_int_2->second->begin + core->getCoreTW(); i++){
		if(ext_length + sys.TAM[i] > sys.getTAMAvg()){
			fit_avg = false;
			break;
		}
	}
	if(fit_avg){
		core->setTAMRange(it_int_2->second->begin, it_int_2->second->begin + core->getCoreTW() - 1);
		if(ext_length - it_int_2->second->length > 0){
			cout<<"Macted: "<<core->getCoreTW()<<endl;
			sys.modTAM(it_int_2->second->begin, it_int_2->second->end, it_int_2->second->length);
			cout<<"Begin: "<<it_int_2->second->begin<<endl;
			cout<<"End: "<<it_int_2->second->end<<endl;
			cout<<"Width: "<<it_int_2->second->width<<endl;
			cout<<"Length Added: "<<it_int_2->second->length<<endl;
			cout<<endl;
			ext_length -= it_int_2->second->length;
			complement.complementErase(core->getCoreTW(), it_int_2);
			return true;

		}
		else if(ext_length - it_int_2->second->length < 0){
			cout<<"Macted: "<<core->getCoreTW()<<endl;
			sys.modTAM(it_int_2->second->begin, it_int_2->second->end, ext_length);
			cout<<"Begin: "<<it_int_2->second->begin<<endl;
			cout<<"End: "<<it_int_2->second->end<<endl;
			cout<<"Width: "<<it_int_2->second->width<<endl;
			cout<<"Length Added: "<<ext_length<<endl;
			cout<<endl;
			complement.complementUpdate(core->getCoreTW(), it_int_2, ext_length);
			ext_length = 0;
			core->setDone(true);
			return true;
		}
		else if(ext_length - it_int_2->second->length == 0){
			cout<<"Macted: "<<core->getCoreTW()<<endl;
			sys.modTAM(it_int_2->second->begin, it_int_2->second->end, ext_length);
			cout<<"Begin: "<<it_int_2->second->begin<<endl;
			cout<<"End: "<<it_int_2->second->end<<endl;
			cout<<"Width: "<<it_int_2->second->width<<endl;
			cout<<"Length Added: "<<ext_length<<endl;
			cout<<endl;
			complement.complementErase(core->getCoreTW(), it_int_2);
			ext_length = 0;
			core->setDone(true);
			return true;
		}
	}
	return false;
}

bool bottomAssign(Core* core, System& sys, Complement& complement, int& ext_length)
{
	int begin = 0, end = core->getCoreTW() - 1;
	if(core->initTAM() == 0){

		bool fit_avg = true;
		for(int i = 0; i < core->getCoreTW(); i++){
			if(ext_length + sys.TAM[i] > sys.getTAMAvg()){
				fit_avg = false;
				break;
			}
		}

		if(fit_avg){
			core->setTAMRange(begin, end);
			complement.complementAdd(sys.getSysTW() - core->getCoreTW(), end + 1, sys.getSysTW() - 1, ext_length);
			sys.modTAM(begin, end, ext_length);
			cout<<"Bottom Assign"<<endl;
			cout<<"Begin: "<<core->getTAMBegin()<<endl;
			cout<<"End: "<<core->getTAMEnd()<<endl;
			cout<<"Width: "<<core->getCoreTW()<<endl;
			cout<<"Length Added: "<<ext_length<<endl;
			cout<<endl;
			core->setDone(true);
			ext_length = 0;
			return true;
		}
		else
			return false;
	}
	return false;
}

void fixedAssign(Core* core, System& sys, Complement& complement, int& ext_length)
{
	int begin = core->getTAMBegin();
	int end = core->getTAMEnd();
	sys.modTAM(begin, end, ext_length);
	cout<<"Fixed Assign"<<endl;
	cout<<"Begin: "<<core->getTAMBegin()<<endl;
	cout<<"End: "<<core->getTAMEnd()<<endl;
	cout<<"Width: "<<core->getCoreTW()<<endl;
	cout<<"Length Added: "<<ext_length<<endl;
	cout<<endl;
	core->setDone(true);
	ext_length = 0;
}

struct cmp
{
        bool operator()(int& l, int& r)
        {
                return l > r;
        }
};

bool randomAssign(Core* core, System& sys, Complement& complement, int& ext_length)
{

	if(core->initTAM() == 0){
		map<int, vector<int> > split;
		map<int, vector<int> >::iterator it;
		for(int i = 0; i < sys.getSysTW(); i++){
			if(split.find(sys.TAM[i]) != split.end())
				split[sys.TAM[i]].push_back(i);
			else{
				vector<int> tmp;
				tmp.push_back(i);
				split[sys.TAM[i]] = tmp;
			}
		}

		int i = 0, begin = -1, end = -1;
		map<int, int> tmp_range;
		map<int, int>::iterator tmp_it, tmp_it_2;
		map<int, map<int, Interval*> >::iterator it_int_1;
		map<int, Interval*>::iterator it_int_2;
		priority_queue<int, vector<int>, cmp> TAM;

		it = split.begin();
		while(i != core->getCoreTW()){
			for(int j = 0; j < (int)it->second.size(); j++){
				sys.modTAM(it->second[j], it->second[j], ext_length);
				TAM.push(it->second[j]);
				i++;
				if(i == core->getCoreTW())
					break;
			}
			if(i == core->getCoreTW())
				break;
			it++;
		}

		cout<<"Random Assign"<<endl;
		while(!TAM.empty()){
				if(begin == -1){
					begin = TAM.top();
					end = TAM.top();
				}
				else if(end != TAM.top() - 1){
					core->setTAMRange(begin, end);
					cout<<begin<<", "<<end<<endl;
					cout<<"Width: "<<end - begin + 1<<endl;
					begin = TAM.top();
				}
				if(TAM.size() == 1){
					end = TAM.top();
					core->setTAMRange(begin, end);
					cout<<begin<<", "<<end<<endl;
					cout<<"Width: "<<end - begin + 1<<endl;
				}
				end = TAM.top();
				TAM.pop();
		}
		cout<<endl;
		cout<<"Total Width: "<<core->getCoreTW()<<endl;
		cout<<"Length Added: "<<ext_length<<endl;
		cout<<endl;
		core->setDone(true);
		ext_length = 0;
		return true;
	}
	return false;
}

