#include <stdlib.h>
#ifndef GRAPH_H
#define GRAPH_H
/*Note: We consider the neighbors of the node all the nodes that have edges pointed towards the said node*/
struct g_node{
    float value;
    long int capacity;
    long int neighbors_no;
    long int* neighbors;
};

typedef struct graph{
    long int size;
    struct g_node* nodes;
} graph_t;

/*Initializes a graph_t that can contain 'size' nodes and returns it*/
graph_t* init_graph(long int size);

/*Frees the graph*/
void free_graph(graph_t graph);

/*Adds in node with 'id' a neighbor with neighbor_id*/
int add_neighbor(long int id, long int neighbor_id);

/*Checks if 'neighbor_id' already exists in node with 'id'*/
int neighbor_exists(long int id, long int neighbor_id);

/*Calculates the new page rank and saves it in the value of the node with id*/
void rank(long int id);
#endif