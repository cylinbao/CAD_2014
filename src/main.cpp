#include "stdio.h"
#include "stdlib.h"
#include "parser.h"

using namespace std;

extern void readInputFile(char*);
System sys;

int main(int argc, char** argv)
{
	readInputFile(argv[1]);
	setPrecedence();
	return 0;
}
