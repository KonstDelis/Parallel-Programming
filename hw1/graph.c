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
    graph->nodes = (struct g_node*)malloc(sizeof(struct g_node)*size);
    if(graph->nodes==NULL){
        fprintf(stderr, "Error: memory allocation for graph nodes failed\n");
        free(graph);
        return NULL;
    }
    graph->size=size;
    graph->max_used_node=-1;
    for(long int i=0; i<graph->size; i++){
        graph->nodes[i].value=1.0;
        graph->nodes[i].neighbors_no=0;
        graph->nodes[i].outgoing_edges=0;
        graph->nodes[i].active=FALSE;
        graph->nodes[i].capacity=100;
        graph->nodes[i].neighbors = (long int*)malloc(sizeof(long int)*100);
        if(graph->nodes[i].neighbors==NULL){
            fprintf(stderr, "Error: memory allocation for neighbor array of node %ld failed\n",i);
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

    for(long int i=prv_size; i<graph->size; i++){
        graph->nodes[i].value=1.0;
        graph->nodes[i].neighbors_no=0;
        graph->nodes[i].outgoing_edges=0;
        graph->nodes[i].active=FALSE;
        graph->nodes[i].capacity=100;
        graph->nodes[i].neighbors = (long int*)malloc(sizeof(long int)*100);
        if(graph->nodes[i].neighbors==NULL){
            fprintf(stderr, "Error: memory allocation for neighbor array of node %ld failed\n",i);
            return -2;
        }
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

    if(neighbor_exists(graph,id, neighbor_id)) return 1; /*Check if neighbor already exists*/

    if(graph->nodes[id].neighbors_no+1>=graph->nodes[id].capacity){
        graph->nodes[id].neighbors = realloc(graph->nodes[id].neighbors, graph->nodes[id].capacity*2*sizeof(long int));
        if(graph->nodes[id].neighbors==NULL){
            fprintf(stderr, "Error: memory reallocation for neighbor array of node %ld failed\n",id);
            return 2;
        }
        graph->nodes[id].capacity*=2;
    }
    graph->nodes[id].neighbors[graph->nodes[id].neighbors_no++]=neighbor_id;
    graph->nodes[id].active=TRUE;
    graph->nodes[neighbor_id].active=TRUE;
    graph->nodes[neighbor_id].outgoing_edges+=1;
    if(id>graph->max_used_node || neighbor_id>graph->max_used_node)
        graph->max_used_node = id>neighbor_id ? id : neighbor_id;
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
    fprintf(stream, "Graph: size=%ld, max_used_node=%ld\n", graph->size, graph->max_used_node);
    for(long int i=0; i<graph->size; i++){
        if(!graph->nodes[i].active) continue;
        fprintf(stream, "id: %10ld || value: %8.3f || number of neighbors: %10ld || outgoing_edges: %10ld || neighbors: ",i, graph->nodes[i].value, graph->nodes[i].neighbors_no, graph->nodes[i].outgoing_edges);
        for(long int j=0; j<graph->nodes[i].neighbors_no; j++){
            fprintf(stream,"%ld ", graph->nodes[i].neighbors[j]);
        }
        fprintf(stream,"\n");
    }
}

/*Calculates the new page rank and saves it in the value of the node with id*/
void rank(graph_t* graph, long int id){
    float sum=0.0;
    long int outgoing_edges, neighbor_id;
    if(!graph->nodes[id].active) return;
    for(long int i=0; i<graph->nodes[id].neighbors_no; i++){
        neighbor_id=graph->nodes[id].neighbors[i];
        outgoing_edges=graph->nodes[neighbor_id].outgoing_edges;
        if(outgoing_edges==0) outgoing_edges++;
        sum += (graph->nodes[neighbor_id].value /  (float)outgoing_edges);
    }
    graph->nodes[id].value = 0.15 + (sum*0.85);
}