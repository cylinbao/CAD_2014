rm -fr output

mkdir output

./cadb102 bench_1.info
mv bench_1.scheduling output

./cadb102 bench_2.info
mv bench_2.scheduling output

./cadb102 bench_3.info
mv bench_3.scheduling output

./cadb102 bench_4.info
mv bench_4.scheduling output

./cadb102 bench_5.info
mv bench_5.scheduling output

./cadb102 bench_6.info
mv bench_6.scheduling output

./cadb102 bench_7.info
mv bench_7.scheduling output

./cadb102 bench_8.info
mv bench_8.scheduling output

#./cadb102 bench_10.info
#mv bench_10.scheduling output

#./cadb102 bench_11.info
#mv bench_11.scheduling output

#./cadb102 bench_12.info
#mv bench_12.scheduling output
