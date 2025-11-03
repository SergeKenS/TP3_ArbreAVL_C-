# Makefile pour INF3105 / TP3 - Arbres AVL
OPTIONS = -O2 -Wall

all : testtp3

testtp3 : testtp3.cpp arbreavl.h
	g++ ${OPTIONS} -o testtp3 testtp3.cpp

run : testtp3
	./testtp3

clean:
	rm -rf testtp3 *~ *.o