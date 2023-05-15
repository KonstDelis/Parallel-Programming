#include <iostream>
#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
#include <tbb/tbb.h>
#include <tbb/task_group.h>
#include <tbb/parallel_reduce.h>
#include <cassert>
#include <unistd.h>
#include <fstream>

#define ARRAY_SIZE 1000000

using namespace tbb;

size_t parallel_min_index(int array[], size_t size){
    size_t min;
    min = parallel_reduce(
        blocked_range<size_t>(0,size), size_t(0),
        [=](const blocked_range<size_t>& r, size_t min_index)->size_t{
            for(size_t i= r.begin(); i!=r.end(); i++){
                min_index = array[i]<array[min_index]? i:min_index;
            }
            for(int j=0; j<100000; j++){
                int l = 12/6;
            }
            return min_index;
        },
        [=](size_t s1, size_t s2)->size_t{
            return s1<s2? s1:s2;
        }
    );
    return min;
}

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
    size_t min_index = parallel_min_index(array, ARRAY_SIZE);

    std::cout<<"Min index is in position '"<<min_index<<"' and has value '"<<array[min_index]<<"'\n";
    
}