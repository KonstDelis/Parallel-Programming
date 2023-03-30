CC = gcc
CFLAGS = 

all: hw2parallel.out hw2serial.out hw2arrayOfStructs.out hw2animated.out

hw2arrayOfStructs.out: hw2arrayOfStructs.c
	$(CC) $^ -fopenmp -o $@

hw2parallel.out: hw2.c
	$(CC) $^ -fopenmp -o $@

hw2serial.out: hw2.c
	$(CC) $^ -o $@

hw2animated.out: hw2animated.c
	$(CC) $^ -o $@
	
clean:
	rm -rf *.exe *.o *.out