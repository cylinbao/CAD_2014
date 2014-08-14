#include "parser.h"
#include <iostream>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

extern System sys;
vector<vector<string> > pre;

void readInputFile(char* fileName)
{
	ifstream ifs(fileName);
	string line, catagory, type, name;
	int data, num_test;
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
					sys.setSysTM(data);
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
							new_ext->setLength(data);
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
					ss.clear();
					break;
				}
			}
			cout<<sys.core[sys.core.size() - 1]->getName()<<": "<<sys.core[sys.core.size() - 1]->getNumTest()<<endl;
		}
	}
	cout<<"Num of Resource: "<<sys.res_list.size()<<endl;
	cout<<"Num of Pre: "<<pre.size()<<endl;
	cout<<"Num of Core: "<<sys.core.size()<<endl;
}

void setPrecedence()
{	
	int i , j;
	for(i = 0; i < (int)pre.size(); i++){
		for(j = 0; j < (int)pre[i].size() - 1; j++)
			sys.tot_list[pre[i][j + 1]]->addPre(sys.tot_list[pre[i][j]]);
	}
}
