#include <iostream>
#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
#include <tbb/tbb.h>
#include <tbb/task_group.h>
#include <cassert>

#define ARRAY_SIZE 1000000

using namespace tbb;

void parallel_change_array(int array[], size_t size, const int change){
    parallel_for(
        blocked_range<size_t>(0,size),
        [=](const blocked_range<size_t>& r)->void {
            for(size_t i = r.begin(); i!= r.end(); i++){
                array[i] += change;
            }
            for(int i=0; i<100000; i++){
                int r = 12/change;
            }
        }
    );
}

int main(int argc, char** argv){
    assert(argc==2);
    assert(argv[0] && argv[1]);
    int num_threads = atoi(argv[1]);
    assert(num_threads);

    std::cout<<"Running with '"<<num_threads<<"' threads"<<std::endl;

    global_control control(global_control::max_allowed_parallelism, num_threads);
    int array[ARRAY_SIZE] = {0};
    parallel_change_array(array, ARRAY_SIZE, 24);

    std::cout<<"First 20 cells\n";
    for(int i=0; i<20; i++){
        std::cout<<array[i]<<std::endl;
    }
    
}
