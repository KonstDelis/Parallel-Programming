#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "graph.h"

#define ITERATIONS 50

graph_t* g;
pthread_barrier_t barrier;
pthread_mutex_t print_lock;
int check=0;


struct thread_args{
    long int start;
    long int end;
};

void free_all(){
    free_graph(g);
    pthread_mutex_destroy(&print_lock);
    pthread_barrier_destroy(&barrier);
}

void fill_graph(graph_t *graph, FILE *file){
    char c;
    long int num1, num2;

    while((c = fgetc(file))!=EOF) {
        if(c=='#'){
            while((c = fgetc(file))!='\n') {}
            continue;
        }
        fseek(file, -1, SEEK_CUR);
        ;
        if (fscanf(file, "%ld  %ld\n", &num1, &num2) != 2){
            fprintf(stderr, "Error: unexpected input, fscanf failed\n");
            return ;
        }
        int rt = add_neighbor(graph, num2, num1);
        if(rt==1){
            printf("neighbor %ld for node %ld already exists\n", num1, num2);
        }
    }
}

void print_csv(graph_t* graph, FILE* fcsv){
    fprintf(fcsv, "node,pagerank");
    for(long int i=0; i<=graph->max_used_node; i++){
        if(!graph->nodes[i].active) continue;
        fprintf(fcsv, "\n%ld,%.5f", i, graph->nodes[i].value);
    }
}

void *parallel_page_rank(void* args){
    struct thread_args* a = (struct thread_args*) args;
    long int start = a->start;
    long int end = a->end;
    for(int i=0; i<ITERATIONS; i++){
        for(int j=start; j<=end; j++){
            rank(g,j);
        }
        pthread_barrier_wait(&barrier);
    }
}

int main(int argc, char* argv[]){
    pthread_t thread_table[4];
    struct thread_args args[4];
    /*Check arguments*/
    if((argc!=5 && argc!=6)|| strcmp(argv[1],"-in")!=0 || strcmp(argv[3],"-t")!=0){
        fprintf(stderr, "Error: invalid parameters. Run program as: ./executable -in input_file.txt -t thead_number(values:1,2,3,4)\n");
        exit(1);
    }
    int threads = atoi(argv[4]);
    if(threads < 1 || threads >4){
        fprintf(stderr, "Error: '%d' is not an acceptable number of threads. Try values 1,2,3,4\n",threads);
        exit(1);
    }
    if(argc==6 && strcmp(argv[5],"-graph_representation")!=0){
        fprintf(stderr, "Error: 5th argument should be '-graph_representation' to print file with all nodes and their info\n");
        exit(1);
    }
    /*open files*/
    FILE *fin = fopen(argv[2], "r");
    FILE* fcsv = fopen("pagerank.csv","w");
    if (fin == NULL){
        fprintf(stderr,"Error: Cannot open input file\n");
        return -1;
    }
    if(fcsv==NULL){
        fprintf(stderr, "Error: couldn't open pagerank.csv file\n");
        return -1;
    }
    /*Fill graph from input file*/
    g = init_graph(5000);
    fill_graph(g, fin);

    pthread_mutex_init(&print_lock,NULL);
    pthread_barrier_init(&barrier, NULL, threads);

    /*Parallel pagerank*/
    if(threads>g->max_used_node){
        free_all();
        fprintf(stderr, "Error: Array size too small to be parallelized, make sure that thread_no>=ArrSize\n");
        exit(1);
    }
    long int array_slice, start=0, end;
    array_slice=(g->max_used_node+1)/threads;
    for(int i=1; i<=threads; i++){
        args[i-1].start=start;
        if(i==threads){
            args[i-1].end=g->max_used_node;
        }
        else{
            args[i-1].end=start+array_slice-1;
            start+=array_slice;
        }
        pthread_create(&thread_table[i-1], NULL, parallel_page_rank, &args[i-1]);
    }
    for(int i=1; i<=threads; i++){
        pthread_join(thread_table[i-1], NULL);
    }
    /*End of parallel pagerank*/

    /*Print in files*/
    if(argc==6){
        FILE *fout = fopen("graph_representation.txt", "w");
        if (fout == NULL){
            fprintf(stderr,"Error: Cannot open graph_representation.txt file\n");
            free_all();
            return -1;
        }
        print_graph(g, fout);
    }

    print_csv(g, fcsv);

    free_all();
    return 0;
}