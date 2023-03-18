CC = gcc
CFLAGS = 

all: hw2.out

run: hw2.out
	clear
	./hw2.out -in tests/test2.txt

hw2.out: hw2.c
	$(CC) $^ -o $@
	
clean:
	rm -rf *.exe *.o *.out

cleanall: cleantxt clean