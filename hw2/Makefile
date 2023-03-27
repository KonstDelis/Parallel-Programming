CC = gcc
CFLAGS = 

all: hw2.out hw2faster.out hw2animated.out

hw2faster.out: hw2faster.c
	$(CC) $^ -fopenmp -o $@

hw2.out: hw2.c
	$(CC) $^ -fopenmp -o $@

hw2animated.out: hw2animated.c
	$(CC) $^ -fopenmp -o $@
	
clean:
	rm -rf *.exe *.o *.out