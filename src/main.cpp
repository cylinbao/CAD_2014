#include "stdio.h"
#include "stdlib.h"
#include "core.h"
#include "parser.h"
#include "possibleTests.h"

using namespace std;

extern void readInputFile(char*);
extern void possibleExternal();
extern void possibleBIST();
System sys;

int main(int argc, char** argv)
{
	readInputFile(argv[1]);
	possibleExternal();
	possibleBIST();
	return 0;
}
