#include "parser.h"
//#include "core.h"
#include <iostream>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

System sys;

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
					while(!ss.eof())
						ss >> name;
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
	cout<<"Num of Core: "<<sys.core.size()<<endl;
}
