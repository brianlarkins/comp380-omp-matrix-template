#CC=clang
CC=gcc
#CFLAGS=-Wall -Wpedantic -O3
CFLAGS=-g -I${PAPI_INC} -fopenmp -Wall -Wpedantic -O3 -msse4.2
LDFLAGS=-L${PAPI_LIB}

lab2: lab2.o wctimer.o
	$(CC) $(LDFLAGS) -fopenmp -o $@ $^ -lm -lpapi

hello: hello.o
	$(CC) $(LDFLAGS) -fopenmp -o $@ $^ -lm

clean:
	rm -f *.o lab2 hello
