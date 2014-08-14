CC = g++

LIB = -O3 -Wall -g3

SOURCE = parser.cpp possibleTests.cpp main.cpp

TARGET = execute

all:
	${CC} ${SOURCE} ${LIB} -o ${TARGET}

run:
	./${TARGET}

clean:
	rm -rf ${TARGET}
