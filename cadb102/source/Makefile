CC = g++

LIB = -O3 -Wall -g3

SOURCE = core.cpp testRelated.cpp TAM_assign.cpp parser.cpp printRelated.cpp main.cpp

TARGET = cadb102

all:
	${CC} ${SOURCE} ${LIB} -o ${TARGET}

cp:
	cp ${TARGET} ../benchmark/

run1:
	./cadb102 ../benchmark/bench_1.info
run2:
	./cadb102 ../benchmark/bench_2.info
run3:
	./cadb102 ../benchmark/bench_3.info
run4:
	./cadb102 ../benchmark/bench_4.info
run5:
	./cadb102 ../benchmark/bench_5.info
run6:
	./cadb102 ../benchmark/bench_6.info
run7:
	./cadb102 ../benchmark/bench_7.info
run8:
	./cadb102 ../benchmark/bench_8.info
run10:
	./cadb102 ../benchmark/bench_10.info
run11:
	./cadb102 ../benchmark/bench_11.info
run12:
	./cadb102 ../benchmark/bench_12.info

Time: 
	../benchmark/./getTime.sh

clean:
	rm -rf ${TARGET}
