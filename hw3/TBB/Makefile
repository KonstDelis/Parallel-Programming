EXECS = parallel_for_test reduce_test parallel_sort_test fib_parallel_invoke dynamic_programming_fib

all: $(EXECS)

parallel_for_test: parallel_for_test.cpp
	g++ $^ -ltbb -o $@

reduce_test: reduce_test.cpp
	g++ $^ -ltbb -o $@

parallel_sort_test: sort_test.cpp
	g++ $^ -ltbb -o $@

fib_parallel_invoke: fib_parallel_invoke.cpp
	g++ $^ -ltbb -o $@

dynamic_programming_fib: dynamic_programming_fib.cpp
	g++ $^ -ltbb -o $@

clean: 
	@rm -rf *.exe *.o *.out $(EXECS)
	@echo "Cleaned all executables and output files!"

