all: tbb_test

tbb_test: tbb_test.cpp
	g++ $^ -ltbb -o $@

clean: 
	@rm -rf *.exe *.o *.out tbb_test
	@echo "Cleaned all executables!"

