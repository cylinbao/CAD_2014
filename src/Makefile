CC = g++

LIB = -O3 -Wall -g3

SOURCE = core.cpp testRelated.cpp TAM_assign.cpp parser.cpp printRelated.cpp main.cpp

TARGET = execute

all:
	${CC} ${SOURCE} ${LIB} -o ${TARGET}

cp:
	cp ${TARGET} ../benchmark/

run1:
	./${TARGET} bench_1.info

run2:
	./${TARGET} bench_2.info

clean:
	rm -rf ${TARGET}
