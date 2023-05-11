FLAGS = 

all: BHtree

clean: 
	rm -rf BHtree *.o *.out

run: BHtree
	./BHtree

BHtree: BHtree.cpp
	g++ $^ $(FLAGS) -o $@