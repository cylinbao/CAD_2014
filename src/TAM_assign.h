#include "core.h"

class Complement{
	public:
		map<int, map<int, Interval*> > list; //1. TAM width 2. Length
		void complementErase(int TAM, map<int, Interval*>::iterator it)
		{
			list[TAM].erase(it);
			if(list[TAM].empty())
				list.erase(list.find(TAM));
		};
		void complementAdd(int TAM, int begin, int end, int length)
		{
			Interval* new_interval = new Interval(begin, end, length);
			list[TAM].insert(pair<int, Interval*>(new_interval->length, new_interval));
		};
		void complementUpdate(int TAM, map<int, Interval*>::iterator it, int val)
		{
			Interval* tmp = it->second;
			complementErase(TAM, it);
			tmp->length -= val;
			complementAdd(tmp->width, tmp->begin, tmp->end, tmp->length);
		};
};


void coreAssign(Core* core, System& sys, Complement& complement);
bool noMatchedAssign(Core* core, map<int, map<int, Interval*> >::iterator it_int_1, map<int, Interval*>::iterator it_int_2, System& sys, Complement& complement, int& ext_length);
bool matchedAssign(Core* core, map<int, map<int, Interval*> >::iterator it_int_1, map<int, Interval*>::iterator it_int_2, System& sys, Complement& complement, int& ext_length);
bool bottomAssign(Core* core, System& sys, Complement& complement, int& ext_length);
void fixedAssign(Core* core, System& sys, Complement& complement, int& ext_length);
bool randomAssign(Core* core, System& sys, Complement& complement, int& ext_length);


