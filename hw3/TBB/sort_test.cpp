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

int main(int argc, char** argv){
    assert(argc==2);
    assert(argv[0] && argv[1]);
    int num_threads = atoi(argv[1]);
    assert(num_threads);
    srand(getpid());
    int array[ARRAY_SIZE] = {0};
    for(int i=0; i<ARRAY_SIZE; i++){
        array[i] = rand();
    }

    std::cout<<"Running with '"<<num_threads<<"' threads"<<std::endl;

    global_control control(global_control::max_allowed_parallelism, num_threads);
    
    //Parralel sorting
    parallel_sort(array, array+ARRAY_SIZE);

    //Comment out to see difference in speed
    //----------------------------------------------- 
    std::fstream fout;
    fout.open("sort_result.txt", std::ios::out);

    for(int i=0; i<ARRAY_SIZE; i++){
        fout<<array[i]<<std::endl;
    }
    //-----------------------------------------------
    
    
}