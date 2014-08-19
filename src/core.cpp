#include "core.h"
#include "stdio.h"

void System::initTAM() {
	while(!TAM.empty())
		TAM.pop();
	TAM_Interval *p_TAM_Interval = new TAM_Interval(tot_TAM_width);
	TAM.push(p_TAM_Interval);
}

void System::printTAM() {
	TAM_Interval *p_TAM;

	while(!TAM.empty()) {
		p_TAM = TAM.top();	
		TAM.pop();

		printf("Time for this state: %d\n", p_TAM->time);
		printf("Range: %d ~ %d\n", p_TAM->begin, p_TAM->end);
		printf("Power used by this test: %d\n", p_TAM->power);
		if(p_TAM->occupyExternal != NULL)
			printf("External test name: %s\n", (p_TAM->occupyExternal->getName()).c_str());
		else
			printf("External test name: This TAM interval is not used!\n");
	}
}

TAM_Interval::TAM_Interval(int front, int back, int timeStat, int energy,
													 External *p_External){
	begin = front;
	end = back;
	time = timeStat;
	power = energy;
	width = begin - end;
	occupyExternal = p_External;
}
