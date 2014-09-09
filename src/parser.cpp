#include "parser.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

extern System sys;
vector<vector<string> > pre;
int avg = 0;

void readInputFile(char* fileName)
{
	ifstream ifs(fileName);
	string line, catagory, type, name;
	int data, num_test, ext_length, TAM_sum = 0;
	stringstream ss;
	if(ifs == NULL){
		perror("Couldn't Open the Input File!");
		exit(1);
	}
	while(!ifs.eof()){
		getline(ifs, line);
		if(line == "")
			continue;
		ss << line;
		ss >> catagory;
		ss.clear();
		if(catagory == "System"){
			ss.clear();
			while(true){
				getline(ifs, line);
				ss << line;
				ss >> type;
				if(type == "TAM_width"){
					ss >> data;
					sys.setSysTW(data);
					ss.clear();
					continue;
				}
				else if(type == "Power"){
					ss >> data;
					sys.setSysPower(data);
					ss.clear();
					continue;
				}
				else if(type == "Precedence"){
					vector<string> new_pre;
					while(!ss.eof()){
						ss >> name;
						if(name != ">"){
							if(new_pre.size() == 0)
								new_pre.push_back(name);
							else if(name != new_pre.back())
								new_pre.push_back(name);
						}
					}
					pre.push_back(new_pre);
					ss.clear();
					continue;
				}
				else if(type == "Resource"){
					while(!ss.eof()){
						ss >> name;
						Resource* new_res = new Resource;
						sys.res_list[name] = new_res;
						new_res->setName(name);
					}
					ss.clear();
					continue;
				}
				else if(type == "begin"){
					ss.clear();
					continue;
				}
				else if(type == "end"){
					ss.clear();
					break;
				}
			}
		}
		else if(catagory == "Core"){
			num_test = 0;
			ext_length = 0;
			ss >> name;

			Core* new_core = new Core;
			sys.core.push_back(new_core);
			new_core->setSystem(&sys);
			new_core->setName(name);

			ss.clear();
			while(true){
				getline(ifs, line);
				ss << line;
				ss >> type;
				if(type == "TAM_width"){
					ss >> data;
					new_core->setCoreTW(data);
					ss.clear();
					continue;
				}
				else if(type == "External"){
					ss >> name;

					External* new_ext = new External;
					new_ext->setName(name);
					sys.tot_list[name] = new_ext;
					sys.ext_list[name] = new_ext;
					new_core->ext_list[name] = new_ext;
					new_ext->setCore(new_core);

					while(!ss.eof()){
						ss >> type;
						if(type == "length"){
							ss >> data;
							TAM_sum += data * new_core->getCoreTW();
							new_ext->setLength(data);
							ext_length += data;
						}
						else if(type == "power"){
							ss >> data;
							new_ext->setPower(data);
						}
						else if(type == "partition"){
							ss >> data;
							new_ext->setPartition(data);
						}
					}
					ss.clear();
					num_test++;
					continue;
				}
				else if(type == "BIST"){
					ss >> name;

					BIST* new_bist = new BIST;
					new_bist->setName(name);
					sys.tot_list[name] = new_bist;
					sys.bist_list[name] = new_bist;
					new_core->bist_list[name] = new_bist;
					new_bist->setCore(new_core);

					while(!ss.eof()){
						ss >> type;
						if(type == "length"){
							ss >> data;
							new_bist->setLength(data);
						}
						else if(type == "power"){
							ss >> data;
							new_bist->setPower(data);
						}
						else if(type == "resource"){
							ss >> name;
							new_bist->setRes(name);
						}
					}
					num_test++;
					ss.clear();
					continue;
				}
				else if(type == "begin"){
					ss.clear();
					continue;
				}
				else if(type == "end"){
					new_core->setNumTest(num_test);
					new_core->setExtLength(ext_length);
					ss.clear();
					break;
				}
			}
		}
	}
	sys.setTAMAvg(TAM_sum/sys.getSysTW());
}

void setPrecedence()
{	
	int i , j;
	for(i = 0; i < (int)pre.size(); i++){
		for(j = 0; j < (int)pre[i].size() - 1; j++)
			sys.tot_list[pre[i][j + 1]]->addPre(sys.tot_list[pre[i][j]]);
	}
}

struct cmp
{
        bool operator()(Core* l, Core* r)
        {
                return l->getExtLength() < r->getExtLength();
        }
};
					
void TAMwidthAssign()
{
	int i, random = 0;
	sys.initTAM();
	map<int, map<int, Core*> > set_core_list; // 1. TAM width 2. Length
	map<int, map<int, Core*> >::iterator it_1;
	map<int, Core*>::iterator it_2;
//	map<int, Core*> pre_arrange;
	priority_queue<Core*, vector<Core*>, cmp> pre_arrange;
	Complement complement;

	for(i = 0 ; i < (int)sys.core.size(); i++){
		if(sys.core[i]->getCoreTW() != 0){
			if(set_core_list.find(sys.core[i]->getCoreTW()) == set_core_list.end()){
				map<int, Core*> tmp;
				cout<<sys.core[i]->getExtLength()<<endl;
				tmp[sys.core[i]->getExtLength()] = sys.core[i];
				set_core_list[sys.core[i]->getCoreTW()] = tmp;
			}
			else{
				if(set_core_list[sys.core[i]->getCoreTW()].find(sys.core[i]->getExtLength()) == set_core_list[sys.core[i]->getCoreTW()].end())
					set_core_list[sys.core[i]->getCoreTW()].insert(pair<int, Core*>(sys.core[i]->getExtLength(), sys.core[i]));
				else{
					Core* tmp = set_core_list[sys.core[i]->getCoreTW()].find(sys.core[i]->getExtLength())->second;
					while(tmp->getSameExtLength() != NULL)
						tmp = tmp->getSameExtLength();
					tmp->setSameExtLength(sys.core[i]);
				}
				cout<<sys.core[i]->getExtLength()<<endl;
			}
			pre_arrange.push(sys.core[i]);
		}
	}

	cout<<"Previous Work Begin: "<<endl;
	/*while(!pre_arrange.empty()){
		if(pre_arrange.top()->getExtLength() >= sys.getTAMAvg() / 2){
			Core* core = pre_arrange.top();
			random+=coreAssign(core, sys, complement);
		}
		pre_arrange.pop();
	}*/
	cout<<"Previous Work End"<<endl;

	for(i = 0; i < sys.getSysTW(); i++)
		cout<<"TAM["<<i<<"]: "<<sys.TAM[i]<<endl;
	cout<<endl;

	it_1 = set_core_list.end();
	for(it_1--;; it_1--){
		it_2 = it_1->second.end();
		for(it_2--;; it_2--){
			Core* core = it_2->second;
			random+=coreAssign(core, sys, complement);
			if(it_2 == it_1->second.begin())
				break;
		}
		if(it_1 == set_core_list.begin())
			break;

	}

	int tot_1 = 0;
	for(i = 0; i < sys.getSysTW(); i++){
		tot_1 += sys.TAM[i];
		cout<<"TAM["<<i<<"]: "<<sys.TAM[i]<<endl;
	}
	cout<<"Random Count: "<<random<<endl;
	cout<<"Core Count: "<<sys.core.size()<<endl;
	cout<<"TAM avg: "<<tot_1/sys.getSysTW()<<endl;
	cout<<"TAM avg(correct): "<<sys.getTAMAvg()<<endl;
}
