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
        printf("edge from %ld to %ld\n", num1, num2);
    }
}

int main(){
    FILE *f = fopen("datasets/Email-Enron.txt", "r");
    if (f == NULL){
        fprintf(stderr,"Error: Cannot open file\n");
        return -1;
    }
    graph_t* g = init_graph(10);
    fill_graph(g, f);
    // printf("Graph size = %d, max_node = %d\n", g->size, g->max_node);

    // for (int i =0; i < g->size; i++){
    //     printf("Id = %d || Value = %d || neighbor_no = %d || capacity = %d\n",i,g->nodes[i].value,g->nodes[i].neighbors_no ,g->nodes[i].capacity);
    // }
    free_graph(g);
    return 0;
}