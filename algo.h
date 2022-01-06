#pragma once

#include "edges.h"
#include "nodes.h"

struct dijkstra {
    nodes_t* nodes;
    int node_a;
    int node_b;
};

typedef struct dijkstra dijkstra_t;

int algo_dijkstra(const dijkstra_t* data_dijkstra);

struct travelling_salesman {
    nodes_t* nodes;
    int travel_count;
    int* travel_nodes;
};

typedef struct travelling_salesman travelling_salesman_t;

int algo_travelling_salesman(const travelling_salesman_t* data_travelling_salesman);