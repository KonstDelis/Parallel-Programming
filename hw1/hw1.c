#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void print_csv(graph_t* graph, FILE* fcsv){
    fprintf(fcsv, "node,pagerank");
    for(long int i=0; i<=graph->max_used_node; i++){
        if(!graph->nodes[i].active) continue;
        fprintf(fcsv, "\n%ld,%.5f", i, graph->nodes[i].value);
    }
}

int main(int argc, char* argv[]){
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
    graph_t* g = init_graph(50000);
    fill_graph(g, fin);

    /*Parallel pagerank*/

    /*End of parallel pagerank*/

    /*Print in files*/
    if(argc==6){
        FILE *fout = fopen("graph_representation.txt", "w");
        if (fout == NULL){
            fprintf(stderr,"Error: Cannot open graph_representation.txt file\n");
            return -1;
        }
        print_graph(g, fout);
    }

    print_csv(g, fcsv);

    free_graph(g);
    return 0;
}