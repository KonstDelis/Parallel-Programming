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

#define ARRAY_SIZE 1000000

using namespace tbb;

long long int serial_fib(int number){
    if(number<2)
        return number;
    
    long long int x,y;
    x = serial_fib(number-1);
    y = serial_fib(number-2);

    return x+y;
}

void parallel_fib(int number, long long int& sum){
    if(number<2)
        sum=number;
    else if(number<20){
        sum = serial_fib(number);
    }
    else{
        long long int x,y;
        parallel_invoke([&](){parallel_fib(number-1, x);},
                        [&](){parallel_fib(number-2, y);}
        );
        sum = x+y;
    }
    
     
}

int main(int argc, char** argv){
    assert(argc==3);
    assert(argv[0] && argv[1] && argv[2]);
    int num_threads = atoi(argv[1]);
    assert(num_threads);
    int number = atoi(argv[2]);
    assert(number);

    std::cout<<"Running with '"<<num_threads<<"' threads"<<std::endl;

    global_control control(global_control::max_allowed_parallelism, num_threads);
    
    long long int result=0;;
    parallel_fib(number, result);

    std::cout<<"Fibonacci of number "<<number<<" is "<<result<<std::endl;
    
}