CC = gcc

all: hw1.c graph.c
	$(CC) $^ -o hw1

cleantxt: 
	rm -rf graph_representation.txt
	rm -rf pagerank.csv

clean:
	rm -rf *.exe
	rm -rf hw1

cleanall: cleantxt clean