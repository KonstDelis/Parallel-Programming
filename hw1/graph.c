#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "graph.h"

/*Initializes a graph_t that can contain 'size' nodes and returns it*/
graph_t* init_graph(long int size){
    if(size<1){
        fprintf(stderr, "Error: Graph cannot be of size<=0\n");
        return NULL;
    }
    graph_t* graph = (graph_t*)malloc(sizeof(graph_t));
    if(graph==NULL){
        fprintf(stderr, "Error: memory allocation for graph failed\n");
        return NULL;
    }
    graph->nodes = (struct g_node*)malloc(sizeof(struct g_node)*(size+1));
    if(graph->nodes==NULL){
        fprintf(stderr, "Error: memory allocation for graph failed\n");
        return NULL;
    }
    graph->size=size+1;
    for(int i=0; i<graph->size; i++){
        graph->nodes[i].value=1.0;
        graph->nodes[i].neighbors_no=0;
        graph->nodes[i].capacity=50;
        graph->nodes[i].neighbors = (int*)malloc(sizeof(int)*50);
        if(graph->nodes[i].neighbors==NULL){
            fprintf(stderr, "Error: memory allocation for graph failed\n");
            return NULL;
        }
    }
}

/*Frees the graph*/
void free_graph(graph_t graph);

/*Adds in node with 'id' a neighbor with neighbor_id*/
int add_neighbor(long int id, long int neighbor_id);

/*Checks if 'neighbor_id' already exists in node with 'id'*/
int neighbor_exists(long int id, long int neighbor_id);

/*Calculates the new page rank and saves it in the value of the node with id*/
void rank(long int id);