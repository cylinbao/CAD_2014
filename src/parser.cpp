#include "parser.h"
#include "core.h"
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
	int data;
	stringstream ss;
	if(ifs == NULL){
		perror("Couldn't Open the Input File!");
		exit(1);
	}
	while(!ifs.eof()){
		getline(ifs, line);
		ss << line;
		ss >> catagory;
		ss.clear();
		if(catagory == "System"){
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
				if(type == "Power"){
					ss >> data;
					sys.setSysPower(data);
					ss.clear();
					continue;
				}
				if(type == "Precedence"){
					while(!ss.eof())
						ss >> name;
					ss.clear();
					continue;
				}
				if(type == "Resource"){
					while(!ss.eof())
						ss >> name;
					ss.clear();
					continue;
				}
				if(type == "end"){
					ss.clear();
					break;
				}
				ss.clear();
			}
		}
	}
}
