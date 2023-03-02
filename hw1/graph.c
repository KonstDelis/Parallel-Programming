#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "graph.h"

#define TRUE 1
#define FALSE 0

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
    graph->nodes = (struct g_node*)malloc(sizeof(struct g_node)*(size));
    if(graph->nodes==NULL){
        fprintf(stderr, "Error: memory allocation for graph nodes failed\n");
        free(graph);
        return NULL;
    }
    graph->size=size;
    for(long int i=0; i<graph->size; i++){
        graph->nodes[i].value=1.0;
        graph->nodes[i].neighbors_no=0;
        graph->nodes[i].active=FALSE;
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
    printf("Initing from %ld to %ld\n",prv_size, graph->size);
    for(long int i=prv_size; i<graph->size; i++){
        graph->nodes[i].value=1.0;
        graph->nodes[i].neighbors_no=0;
        graph->nodes[i].active=FALSE;
        graph->nodes[i].capacity=50;
        graph->nodes[i].neighbors = (long int*)malloc(sizeof(long int)*50);
        if(graph->nodes[i].neighbors==NULL){
            fprintf(stderr, "Error: memory allocation for neighbor array of node %d failed\n",i);
            return -2;
        }
        //printf("node id=%ld has neighbors_no=%ld\n", i, graph->nodes[i].neighbors_no);
    }
    return 0;
}

/*Frees the graph*/
void free_graph(graph_t* graph){
    /*Free neighbor arrays*/
    for(long int i=0; i<graph->size; i++){
        free(graph->nodes[i].neighbors);
    }
    /*Free node array and graph*/
    free(graph->nodes);
    free(graph);
}

/*Adds in node with 'id' a neighbor with neighbor_id*/
int add_neighbor(graph_t* graph, long int id, long int neighbor_id){
    /*Check if we have enough nodes*/
    while(neighbor_id>=graph->size || id>=graph->size){
        if(expand_graph(graph, graph->size)!=0)
            return 2;
    }
    if(neighbor_exists(graph,id, neighbor_id)) return 1;

    if(graph->nodes[id].neighbors_no+1>=graph->nodes[id].capacity){
        graph->nodes[id].neighbors = realloc(graph->nodes[id].neighbors, graph->nodes[id].capacity*2);
        if(graph->nodes[id].neighbors==NULL){
            fprintf(stderr, "Error: memory reallocation for neighbor array of node %d failed\n",id);
            return 2;
        }
        graph->nodes[id].capacity*=2;
    }
    printf("Adding neighbor %ld to id %ld, node has neighbors_no %ld\n", neighbor_id, id, graph->nodes[id].neighbors_no);
    graph->nodes[id].neighbors[graph->nodes[id].neighbors_no++]=neighbor_id;
    graph->nodes[id].active=TRUE;
    graph->nodes[neighbor_id].active=TRUE;
}

/*Checks if 'neighbor_id' already exists in node with 'id'*/
int neighbor_exists(graph_t* graph, long int id, long int neighbor_id){
    for(long int i=0; i<graph->nodes[id].neighbors_no;i++){
        if(graph->nodes[id].neighbors[i]==neighbor_id)
            return TRUE;
    }
    return FALSE;
}

void print_graph(graph_t* graph, FILE* stream){
    for(long int i=0; i<graph->size; i++){
        if(!graph->nodes[i].active) continue;
        fprintf(stream, "id: %10ld || value: %1.3f || number of neighbors: %10ld || neighbors: ",i, graph->nodes[i].value, graph->nodes[i].neighbors_no);
        for(long int j=0; j<graph->nodes[i].neighbors_no; j++){
            fprintf(stream,"%ld ", graph->nodes[i].neighbors[j]);
        }
        fprintf(stream,"\n");
    }
    fprintf(stream, "Graph: size=%ld\n", graph->size);
}

/*Calculates the new page rank and saves it in the value of the node with id*/
void rank(graph_t* graph, long int id);