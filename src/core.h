#ifndef _CORE_H_
#define _CORE_H_

#include <map>
#include <string>
#include <vector>
#include <queue>
#include <iostream>

using namespace std;

class Core;
class Test;
class External;
class BIST;
class Resource;

class TAMInterval{
  public:
    TAMInterval(int tot_TAM_width){ TAMBegin=0; TAMEnd=tot_TAM_width-1; timeBegin=0;
                                    timeEnd=0; power=0; width=TAMEnd; occupyExternal=NULL;}
    TAMInterval(int front, int back, int time1, int time2, int energy,
                 External *p_External);
    int TAMBegin;
    int TAMEnd;
    int timeBegin;
		int timeEnd;
    int power;
    int width;
    External *occupyExternal;
};

class cmp_TAM {
  public:
    bool operator()(TAMInterval* lhs, TAMInterval* rhs) {
      return lhs->timeEnd > rhs->timeEnd;
    }
};

class TAMContainer {
  public:
    void initTAM(int tot_TAM_width);
    void printTAM();
		void printPowerStat();
		void insertInterval(External* pExt);
		void insertBist(BIST* pBist, map<string, Resource*>* p_res_list);
		void deleteTop();
		void computePower(External *pExt, map<string, External*>* pDoneExtList, 
											BIST *pBist, map<string, BIST*>* pDoneBistList);
		bool checkPower(TAMInterval *pTAMInterval, External *pExt, BIST *pBist, int limit, 
									 	map<string, External*>* pDoneExtList, map<string, BIST*>* pDoneBistList);
		vector<TAMInterval*> getTopIntvWithSameTime();

    priority_queue<TAMInterval*, vector<TAMInterval*>, cmp_TAM> pqTAM;
		map<int ,int> powerStat; // use time as key, power as value
};

class System{
	public:
		System(){tot_power = 0;tot_TAM_width = 0;};

		void setSysPower(int power){tot_power = power;};
		void setSysTW(int TAM_width){tot_TAM_width = TAM_width;};
		void setWaitExtList(){wait_ext_list = ext_list;};
		void setWaitBistList(){wait_bist_list = bist_list;};
		void setTAMAvg(int avg){this->avg = avg;};
		void initTAM(){TAM = new int[tot_TAM_width];for(int i = 0; i < tot_TAM_width; i++)TAM[i] = 0;};
		void modTAM(int begin, int end, int val){for(int i = begin; i <= end; i++)TAM[i]+=val;};

		int getTAMAvg(){return avg;};
		int getSysPower(){return tot_power;};
		int getSysTW(){return tot_TAM_width;};

		void fillTest();
		void printTAMAssignment();
		void printTest();
		void printExtList();
		void printBistList();
		void printResult();

		vector<External*> possibleExternal(TAMInterval *pTAMInterval);
		vector<BIST*> possibleBIST(TAMInterval *pTAMInterval);

		vector<Core*> core;
		map<string, Test*> tot_list;
		map<string, External*> ext_list;
		map<string, External*> wait_ext_list;
		map<string, External*> done_ext_list;
		map<string, BIST*> bist_list;
		map<string, BIST*> wait_bist_list;
		map<string, BIST*> done_bist_list;
		map<string, Resource*> res_list;
		TAMContainer TAMStat;

		int* TAM;
	private:
		int tot_power;
		int tot_TAM_width;
		int avg;
};

class Resource{
	public:
		Resource(){name == ""; usable = true; time=0;};

		void setName(string name){this->name = name;};
		void setTure(){usable = true;};
		void setFalse(){usable = false;};
		void setTime(int number){time = number;};

		string getName(){return name;};
		bool getUsable(){return usable;};
		int getTime(){return time;};
	private:
		string name;
		bool usable;
		int time;
};

class Core{
	public:
		Core(){sys = NULL; name = ""; TAM_width = 0; num_test = 0; ext_length = 0; same_ext_length = NULL; done = false;};

		void setSystem(System* sys){this->sys = sys;};
		void setName(string name){this->name = name;};
		void setCoreTW(int TAM_width){this->TAM_width = TAM_width;};
		void setNumTest(int num_test){this->num_test = num_test;};
		void setExtLength(int ext_length){this->ext_length = ext_length;};
		void setTAMRange(int begin, int end){TAM_range.push_back(make_pair(begin, end));};
		void setSameExtLength(Core* same_ext_length){this->same_ext_length = same_ext_length;};
		void setDone(bool done){this->done = done;};

		System* getSystem(){return sys;};
		string getName(){return name;};
		int getCoreTW(){return TAM_width;};
		int getNumTest(){return num_test;};
		int getExtLength(){return ext_length;};
		int getTAMBegin(){return TAM_range[0].first;};
		int getTAMEnd(){return TAM_range[0].second;};
		vector<pair<int, int> > getTAM_range() {return TAM_range;};
		Core* getSameExtLength(){return same_ext_length;};
		bool getDone(){return done;};

		int initTAM(){return TAM_range.size();};

		void printTAM_range();

		map<string, External*> ext_list;
		map<string, BIST*> bist_list;
	private: 
		System* sys;
		string name;
		Core* same_ext_length;
		vector<pair<int, int> >  TAM_range;
		int TAM_width;
		int num_test;
		int ext_length;
		bool done;
};

class Test{
	public: 
		Test(){core = NULL; name = ""; power = 0; length = 0;done = false;
					 startTime = 0; endTime = 0;};

		void setCore(Core* core){this->core = core;};
		void setName(string name){this->name = name;};
		void setPower(int power){this->power = power;};
		void setLength(int length){this->length = length;};
		void addPre(Test* pre){this->pre.push_back(pre);};

		Core* getCore(){return core;};
		string getName(){return name;};
		int getPower(){return power;};
		int getLength(){return length;};
		vector<Test*>* getPre(){return &pre;};
		vector< pair<int, int> >* getExecTime(){return &execTime;};

		bool checkDone(){return done;};
		bool checkPreDone();

		void setDone(){done = true;};
	private: 
		Core* core;
		string name;
		int power;
		int length;
		int startTime, endTime;
		bool done;
		vector<Test*> pre;
		vector< pair<int, int> > execTime;
};

class External: public Test{
	public: 
		External(){partition = 0;};

		void setPartition(int partition){this->partition = partition;};

		int getPartition(){return partition;};

		void printInfo();
	private: 
		int partition;
		
};

class BIST: public Test{
	public:
		BIST(){res = "";};

		void setRes(string res){this->res = res;};

		string getRes(){return res;};
	private:
		string res;
};

class Interval{
	public:
		Interval();
		Interval(int begin, int end, int length){this->begin = begin; this->end = end; this->length = length; width = end - begin + 1;done = false;};
		int begin;
		int end;
		int width;
		int length;
		bool done;
};

#endif
