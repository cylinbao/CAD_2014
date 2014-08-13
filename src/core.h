#include <map>
#include <string>
#include <vector>

using namespace std;

class Core;
class Test;
class External;
class BIST;

class System{
	public:
		System(){tot_power = 0;tot_TAM_width = 0;};
		void setSysPower(int power){tot_power = power;};
		void setSysTM(int TAM_width){tot_TAM_width = TAM_width;};
		int getSysPower(){return tot_power;};
		int getSysTM(){return tot_TAM_width;};
		vector<Core*> core;
		map<string, External*> ext_sys_list;
		map<string, BIST*> bist_sys_list;
	private:
		int tot_power;
		int tot_TAM_width;
};

class Core{
	public:
		Core(){sys = NULL; core = -1; TAM_width = 0; num_test = 0;};
		void setCore(int core){this->core = core;};
		void setCoreTW(int TAM_width){this->TAM_width = TAM_width;};
		void setNumTest(int num_test){this->num_test = num_test;};
		int getCore(){return core;};
		int getCoreTW(){return TAM_width;};
		int getNumTest(){return num_test;};
		map<string, External*> ext_core_list;
		map<string, BIST*> bist_core_list;
	private: 
		System* sys;
		int core;
		int TAM_width;
		int num_test;
};

class Test{
	public: 
		Test(){core = NULL; name = ""; power = 0; length = 0; pre = NULL;};
		void setName(string name){this->name = name;};
		void setPower(int power){this->power = power;};
		void setLength(int length){this->length = length;};
		void setPre(Test* pre){this->pre = pre;};
		string getName(){return name;};
		int getPower(){return power;};
		int getLength(){return length;};
		Test* getPre(){return pre;};
	private: 
		Core* core;
		string name;
		int power;
		int length;
		Test *pre;

	
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
