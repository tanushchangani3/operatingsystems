CC=gcc
CFLAGS=-g -lm

problem1 : problem1.o utils.o
	$(CC) -o problem1 utils.o problem1.o -pthread

.PHONY : clean

clean : 
	rm *.o $(objects) problem1