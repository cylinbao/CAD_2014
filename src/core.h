#ifndef _CORE_H_
#define _CORE_H_

#include <map>
#include <string>
#include <vector>
#include <queue>

using namespace std;

class Core;
class Test;
class External;
class BIST;
class Resource;

class TAMInterval{
  public:
    TAMInterval(int tot_TAM_width){ TAMBegin=0; TAMEnd=tot_TAM_width-1; time=0;
                                     power=0; width=TAMEnd; occupyExternal=NULL;}
    TAMInterval(int front, int back, int timeStat, int energy,
                 External *p_External);
    int TAMBegin;
    int TAMEnd;
    int time;
    int power;
    int width;
    External *occupyExternal;
};

class cmp_TAM {
  public:
    bool operator()(TAMInterval* lhs, TAMInterval* rhs) {
      return lhs->time > rhs->time;
    }
};

class TAMContainer {
  public:
    void initTAM(int tot_TAM_width);
    void printTAM();
		void insertInterval(External* pExt);
		void deleteTop();

    priority_queue<TAMInterval*, vector<TAMInterval*>, cmp_TAM> pqTAM;
		int totPower;
};

class System{
	public:
		System(){tot_power = 0;tot_TAM_width = 0;};

		void setSysPower(int power){tot_power = power;};
		void setSysTW(int TAM_width){tot_TAM_width = TAM_width;};
		void setWaitExtList(){wait_ext_list = ext_list;};

		int getSysPower(){return tot_power;};
		int getSysTW(){return tot_TAM_width;};

		void fillTest();
		vector<External*> possibleExternal(TAMInterval *pTAMInterval);
		vector<BIST*> possibleBIST();

		vector<Core*> core;
		map<string, Test*> tot_list;
		map<string, External*> ext_list;
		map<string, External*> wait_ext_list;
		map<string, BIST*> bist_list;
		map<string, Resource*> res_list;
		TAMContainer TAMStat;

	private:
		int tot_power;
		int tot_TAM_width;
};

class Resource{
	public:
		Resource(){name == ""; usable = true;};

		void setName(string name){this->name = name;};
		void setTure(){usable = true;};
		void setFalse(){usable = false;};

		string getName(){return name;};
		bool getUsable(){return usable;};
	private:
		string name;
		bool usable;
};

class Core{
	public:
		Core(){sys = NULL; name = ""; TAM_width = 0; num_test = 0; ext_length = 0; TAM_begin = 0; TAM_end = 0;};

		void setSystem(System* sys){this->sys = sys;};
		void setName(string name){this->name = name;};
		void setCoreTW(int TAM_width){this->TAM_width = TAM_width;};
		void setNumTest(int num_test){this->num_test = num_test;};
		void setExtLength(int ext_length){this->ext_length = ext_length;};
		void setTAMRange(int begin, int end){this->TAM_begin = begin; this->TAM_end = end;};

		System* getSystem(){return sys;};
		string getName(){return name;};
		int getCoreTW(){return TAM_width;};
		int getNumTest(){return num_test;};
		int getExtLength(){return ext_length;};
		int getTAMBegin(){return TAM_begin;};
		int getTAMEnd(){return TAM_end;};
		vector<pair<int, int> > getTAM_range() {return TAM_range};

		map<string, External*> ext_list;
		map<string, BIST*> bist_list;
	private: 
		System* sys;
		string name;
		vector<pair<int, int> >  TAM_range;
		int TAM_width;
		int num_test;
		int ext_length;
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
		bool checkDone(){return done;};
		bool checkPreDone();
		vector<Test*>* getPre(){return &pre;};
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
		Interval(int begin, int end, int weight){this->begin = begin; this->end = end; this->weight = weight; width = end - begin + 1;};
		int begin;
		int end;
		int width;
		int weight;
};

#endif
