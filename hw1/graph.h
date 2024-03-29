#include <stdlib.h>
#ifndef GRAPH_H
#define GRAPH_H
/*Note: We consider the neighbors of the node all the nodes that have edges pointed towards the said node*/
struct g_node{
    float value;
    float pending_incoming;
    long int capacity;
    long int neighbors_no;
    long int outgoing_edges;
    long int* neighbors;
    int active;
};

typedef struct graph{
    long int size;
    long int max_used_node;
    struct g_node* nodes;
} graph_t;

/*Initializes a graph_t that can contain 'size' nodes and returns it*/
graph_t* init_graph(long int size);

/*Expands the graph so that it can fit graph->size+extra_size*/
int expand_graph(graph_t* graph, long int extra_size);

/*Frees the graph*/
void free_graph(graph_t* graph);

/*Adds in node with 'id' a neighbor with neighbor_id*/
int add_neighbor(graph_t* graph, long int id, long int neighbor_id);

/*Checks if 'neighbor_id' already exists in node with 'id'*/
int neighbor_exists(graph_t* graph, long int id, long int neighbor_id);

/*Prints the entire graph in a file*/
void print_graph(graph_t* graph, FILE* stream);

/*Calculates the new page rank and saves it in the value of the node with id*/
void rank(graph_t* graph, long int id);

/*Calculate the incoming value of the node for the specific iteration*/
void calculate_incoming(graph_t* graph, long int id);
#endif