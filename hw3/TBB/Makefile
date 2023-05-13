FLAGS = 

all: barnes_hut.out

clean: 
	rm -rf *.o *.out

barnes_hut.out: BHtree.cpp main.cpp
	g++ BHtree.cpp -c
	g++ main.cpp BHtree.o $(FLAGS) -o $@