CC = g++

LIB = -O3 -Wall -g3

SOURCE = core.cpp testRelated.cpp TAM_assign.cpp parser.cpp main.cpp

TARGET = execute

all:
	${CC} ${SOURCE} ${LIB} -o ${TARGET}

run1:
	./${TARGET} ../benchmark/bench_1.info

clean:
	rm -rf ${TARGET}
