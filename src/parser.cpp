#include "parser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <stack>
#include <utility>

using namespace std;

extern System sys;
vector<vector<string> > pre;
int avg = 0;

void readInputFile(char* fileName)
{
	ifstream ifs(fileName);
	string line, catagory, type, name;
	int data, num_test, ext_length;
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
					sys.tot_list[name] = new_ext;
					sys.ext_list[name] = new_ext;
					new_core->ext_list[name] = new_ext;
					new_ext->setCore(new_core);

					while(!ss.eof()){
						ss >> type;
						if(type == "length"){
							ss >> data;
							avg += data * new_core->getCoreTW();
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
//			cout<<sys.core[sys.core.size() - 1]->getName()<<":"<<endl<<"TAM width: "<<sys.core[sys.core.size() - 1]->getCoreTW()<<endl<<"Num of Test: "<<sys.core[sys.core.size() - 1]->getNumTest()<<endl<<"Total length of External: "<<sys.core[sys.core.size() - 1]->getExtLength()<<endl;
		}
	}
	avg = avg/sys.getSysTW();
	cout<<"Avg of each TAM width: "<<avg<<endl;
//	cout<<"Num of Resource: "<<sys.res_list.size()<<endl;
//	cout<<"Num of Pre: "<<pre.size()<<endl;
//	cout<<"Num of Core: "<<sys.core.size()<<endl;
}

void setPrecedence()
{	
	int i , j;
	for(i = 0; i < (int)pre.size(); i++){
		for(j = 0; j < (int)pre[i].size() - 1; j++)
			sys.tot_list[pre[i][j + 1]]->addPre(sys.tot_list[pre[i][j]]);
	}
}

int TAM_avg(int begin, int end){
	int tot = 0;
	for(int i = begin; i < end; i++)
		tot += sys.TAM[i];
	return tot/(end - begin);
	
}

void TAMwidthAssign()
{

	int i, j;
	sys.initTAM(sys.getSysTW());
	map<int, map<int, Core*> > set_core_list;
	map<int, map<int, Core*> >::iterator it_1;
	map<int, Core*>::iterator it_2;
	for(i = 0 ; i < sys.core.size(); i++){
		if(set_core_list.find(sys.core[i]->getCoreTW()) == set_core_list.end()){
			map<int, Core*> tmp;
			tmp[sys.core[i]->getExtLength()] = sys.core[i];
			set_core_list[sys.core[i]->getCoreTW()] = tmp;
		}
		else
			set_core_list[sys.core[i]->getCoreTW()].insert(pair<int, Core*>(sys.core[i]->getExtLength(), sys.core[i]));
	}
	it_1 = set_core_list.end();
	for(it_1--;; it_1--){
		if(it_1->first >= sys.getSysTW()/2){
			for(it_2 = it_1->second.begin(); it_2 != it_1->second.end(); it_2++)
				sys.modTAM(0, it_2->second->getCoreTW(), it_2->second->getExtLength());
		}

		if(it_1 == set_core_list.begin())
			break;
	}
	for(i = 0; i < sys.getSysTW(); i++)
		cout<<"TAM["<<i<<"]: "<<sys.TAM[i]<<endl;
}
