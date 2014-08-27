#ifndef _CORE_H_
#define _CORE_H_

#include <map>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Core;
class Test;
class External;
class BIST;
class Resource;

class System{
	public:
		System(){tot_power = 0;tot_TAM_width = 0;};

		void setSysPower(int power){tot_power = power;};
		void setSysTW(int TAM_width){tot_TAM_width = TAM_width;};
		void setTAMAvg(int avg){this->avg = avg;};
		void initTAM(){TAM = new int[tot_TAM_width];for(int i = 0; i < tot_TAM_width; i++)TAM[i] = 0;};
		void modTAM(int begin, int end, int val){for(int i = begin; i <= end; i++)TAM[i]+=val;};

		int getTAMAvg(){return avg;};
		int getSysPower(){return tot_power;};
		int getSysTW(){return tot_TAM_width;};

		vector<Core*> core;
		map<string, Test*> tot_list;
		map<string, External*> ext_list;
		map<string, BIST*> bist_list;
		map<string, Resource*> res_list;
		int* TAM;
	private:
		int tot_power;
		int tot_TAM_width;
		int avg;
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
		Core(){sys = NULL; name = ""; TAM_width = 0; num_test = 0; ext_length = 0; TAM_begin = -1; TAM_end = -1;same_ext_length = NULL; done = false;};

		void setSystem(System* sys){this->sys = sys;};
		void setName(string name){this->name = name;};
		void setCoreTW(int TAM_width){this->TAM_width = TAM_width;};
		void setNumTest(int num_test){this->num_test = num_test;};
		void setExtLength(int ext_length){this->ext_length = ext_length;};
		void setTAMRange(int begin, int end){this->TAM_begin = begin; this->TAM_end = end;};
		void setSameExtLength(Core* same_ext_length){this->same_ext_length = same_ext_length;};
		void setDone(bool done){this->done = done;};

		System* getSystem(){return sys;};
		string getName(){return name;};
		int getCoreTW(){return TAM_width;};
		int getNumTest(){return num_test;};
		int getExtLength(){return ext_length;};
		int getTAMBegin(){return TAM_begin;};
		int getTAMEnd(){return TAM_end;};
		Core* getSameExtLength(){return same_ext_length;};
		bool getDone(){return done;};

		map<string, External*> ext_list;
		map<string, BIST*> bist_list;
	private: 
		System* sys;
		string name;
		Core* same_ext_length;
		int TAM_width;
		int num_test;
		int ext_length;
		int TAM_begin;
		int TAM_end;
		bool done;
};

class Test{
	public: 
		Test(){core = NULL; name = ""; power = 0; length = 0;done = false;};

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
	private: 
		Core* core;
		string name;
		int power;
		int length;
		bool done;
		vector<Test*> pre;
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
		Interval(int begin, int end, int length){this->begin = begin; this->end = end; this->length = length; width = end - begin + 1;done = false;};
		int begin;
		int end;
		int width;
		int length;
		bool done;
};

#endif
