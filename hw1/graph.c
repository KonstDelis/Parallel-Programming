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
        fprintf(stderr, "Error: memory allocation for graph nodes failed\n");
        free(graph);
        return NULL;
    }
    graph->size=size+1;
    graph->max_node=0;
    for(int i=0; i<graph->size; i++){
        graph->nodes[i].value=1.0;
        graph->nodes[i].neighbors_no=0;
        graph->nodes[i].capacity=50;
        graph->nodes[i].neighbors = (long int*)malloc(sizeof(long int)*50);
        if(graph->nodes[i].neighbors==NULL){
            fprintf(stderr, "Error: memory allocation for neighbor array of node %d failed\n",i);
            free(graph->nodes);
            free(graph);
            return NULL;
        }
    }
    return graph;
}

/*Expands the graph so that it can fit graph->size+extra_size*/
int expand_graph(graph_t* graph, long int extra_size){
    /*Realloc node array*/
    long int prv_size=graph->size;
    graph->nodes = (struct g_node*)realloc(graph->nodes,sizeof(struct g_node)*(prv_size+extra_size));
    if(graph->nodes==NULL){
        fprintf(stderr, "Error: memory allocation for graph failed\n");
        return -1;
    }
    /*Change size var and initialize the new nodes*/
    graph->size=prv_size+extra_size;
    for(int i=prv_size; i<graph->size; i++){
        graph->nodes[i].value=1.0;
        graph->nodes[i].neighbors_no=0;
        graph->nodes[i].capacity=50;
        graph->nodes[i].neighbors = (long int*)malloc(sizeof(long int)*50);
        if(graph->nodes[i].neighbors==NULL){
            fprintf(stderr, "Error: memory allocation for neighbor array of node %d failed\n",i);
            return -2;
        }
    }
}

/*Frees the graph*/
void free_graph(graph_t* graph){
    /*Free neighbor arrays*/
    for(int i=0; i<graph->size; i++){
        free(graph->nodes[i].neighbors);
    }
    /*Free node array and graph*/
    free(graph->nodes);
    free(graph);
}

/*Adds in node with 'id' a neighbor with neighbor_id*/
int add_neighbor(graph_t* graph, long int id, long int neighbor_id);

/*Checks if 'neighbor_id' already exists in node with 'id'*/
int neighbor_exists(graph_t* graph, long int id, long int neighbor_id);

/*Calculates the new page rank and saves it in the value of the node with id*/
void rank(graph_t* graph, long int id);