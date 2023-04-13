#include <iostream>
#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
#include <tbb/tbb.h>
#include <tbb/task_group.h>
#include <tbb/parallel_reduce.h>
#include <tbb/parallel_sort.h>
#include <cassert>
#include <unistd.h>
#include <fstream>

unsigned long long int dynamic_programming_fib(int number){
    unsigned long long int tmp, num_minus1=0, num_minus2=1;
    for(int i=2; i<=number; i++){
        tmp = num_minus2;
        num_minus2 = num_minus1+num_minus2;
        num_minus1 = tmp;
    }
    return num_minus2;
}

int main(int argc, char** argv){
    assert(argc==2);
    assert(argv[0] && argv[1]);
    int number = atoi(argv[1]);

    unsigned long long int result = dynamic_programming_fib(number);
    std::cout<<"Fibonacci of number "<<number<< " is "<<result<<std::endl;
}