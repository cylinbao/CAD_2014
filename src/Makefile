CC = g++

LIB = -O3 -Wall -g3

SOURCE = main.cpp parser.cpp TAM_assign.cpp possibleTests.cpp

TARGET = execute

all:
	${CC} ${SOURCE} ${LIB} -o ${TARGET}

run:
	./${TARGET}

clean:
	rm -rf ${TARGET}
