#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graph.h"

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

int main(){
    FILE *f = fopen("datasets/mytest.txt", "r");
    FILE *fout = fopen("out.txt", "w");
    if (f == NULL){
        fprintf(stderr,"Error: Cannot open file\n");
        return -1;
    }
    graph_t* g = init_graph(50);
    fill_graph(g, f);
    for(long int i=0; i<g->size; i++){
        rank(g,i);
    }
    print_graph(g, fout);
    free_graph(g);
    return 0;
}