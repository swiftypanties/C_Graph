#pragma once

#include <stdio.h>

#include "edges.h"

struct single_node {
    int number;

    int pointers_count;
    int pointers_length;
    edge_t** pointers_array;
};

typedef struct single_node single_node_t;

struct nodes {

    int nodes_count;
    int nodes_length;
    single_node_t** nodes_array;

    int edges_count;
    int edges_length;
    edge_t** edges_array;

};

typedef struct nodes nodes_t;

nodes_t* nodes_create(int count);

void nodes_destroy(nodes_t* nodes);

single_node_t* nodes_add_node(nodes_t* nodes, int node);

void nodes_delete_node(nodes_t* nodes, int node);

void nodes_add_edge(nodes_t* nodes, const edge_t* edge);

void nodes_replace_node(nodes_t* nodes, int node);

single_node_t* nodes_find(nodes_t* nodes, int number);

void debug_present_nodes(nodes_t* nodes, FILE* output_file);
