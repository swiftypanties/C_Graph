#include <stdlib.h>

#include "edges.h"

edge_t* edge_create(
    int node_a,
    int node_b,
    int weight
) {

    edge_t* edge = malloc(sizeof(*edge));

    edge->node_a = node_a;
    edge->node_b = node_b;
    edge->weight = weight;

    return edge;

}

void edge_destroy(edge_t* edge) {

    free(edge);

}