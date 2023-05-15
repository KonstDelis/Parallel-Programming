FLAGS =  -ltbb

all: barnes_hut.out

clean: 
	rm -rf *.o *.out

cleantxt:
	rm -rf *.txt

barnes_hut.out: BHtree.cpp main.cpp
	g++ BHtree.cpp -c
	g++ main.cpp BHtree.o $(FLAGS) -o $@