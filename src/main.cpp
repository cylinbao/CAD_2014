#include "stdio.h"
#include "stdlib.h"
#include "parser.h"
//#include "parser.cpp"

using namespace std;

extern void readInputFile(char*);
extern System sys;

int main(int argc, char** argv)
{
	readInputFile(argv[1]);
	return 0;
}
