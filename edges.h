#pragma once

struct edge {
    int node_a;
    int node_b;
    int weight;
};

typedef struct edge edge_t;

edge_t* edge_create(
    int node_a,
    int node_b,
    int weight
);

void edge_destroy(edge_t* edge);