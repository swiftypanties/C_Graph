#include <stdlib.h>

#include "nodes.h"

#include <stdio.h>

#define BASESIZE_EDGES_CHUNK           (16)
#define BASESIZE_NODES_CHUNK           (16)
#define BASESIZE_POINTERS_CHUNK        (16)

static single_node_t* single_node_create(int number);

static void single_node_destroy(single_node_t* single_node);

static void single_node_add_edge(single_node_t* single_node, edge_t* edge);

static void single_node_delete_edge(single_node_t* single_node, edge_t* edge);

static single_node_t* single_node_create(int number) {

    single_node_t* single_node;

    single_node = malloc(sizeof(*single_node));

    single_node->number = number;

    single_node->pointers_count = 0;
    single_node->pointers_length = BASESIZE_POINTERS_CHUNK;
    single_node->pointers_array = calloc(
        single_node->pointers_length,
        sizeof(*single_node->pointers_array)
    );

    return single_node;

}

static void single_node_destroy(single_node_t* single_node) {

    if (single_node != NULL) {

        free(single_node->pointers_array);
        free(single_node);

    }

}

static void single_node_add_edge(single_node_t* single_node, edge_t* edge) {

    if (single_node->pointers_count == single_node->pointers_length) {

        single_node->pointers_length += BASESIZE_POINTERS_CHUNK;

        single_node->pointers_array = reallocarray(
            single_node->pointers_array,
            sizeof(*single_node->pointers_array),
            single_node->pointers_length
        );

    }

    int last_index = single_node->pointers_count;

    single_node->pointers_array[last_index] = edge;

    single_node->pointers_count += 1;

}

static void single_node_delete_edge(single_node_t* single_node, edge_t* edge) {

    int edges_count = single_node->pointers_count;
    edge_t** edges_array = single_node->pointers_array;

    int last_index = edges_count - 1;

    for (int index = 0; index < edges_count; index += 1) {

        if (edges_array[index] == edge) {

            edges_array[index] = edges_array[last_index];
            edges_count -= 1;

            break;

        }

    }

    single_node->pointers_count = edges_count;

}

nodes_t* nodes_create(int length) {

    nodes_t* nodes;

    nodes = malloc(sizeof(*nodes));

    if (nodes == NULL) {

        return NULL;

    }

    nodes->nodes_count = 0;
    nodes->nodes_length = BASESIZE_NODES_CHUNK;
    nodes->nodes_array = calloc(
        nodes->nodes_length,
        sizeof(*nodes->nodes_array)
    );

    nodes->edges_count = 0;
    nodes->edges_length = BASESIZE_EDGES_CHUNK;
    nodes->edges_array = calloc(
        nodes->edges_length,
        sizeof(*nodes->edges_array)
    );

    return nodes;

}

void nodes_destroy(nodes_t* nodes) {

    if (nodes != NULL) {

        for (int index = 0; index < nodes->nodes_count; index += 1) {

            single_node_destroy(nodes->nodes_array[index]);

        }

        for (int index = 0; index < nodes->edges_count; index += 1) {

            edge_destroy(nodes->edges_array[index]);

        }

        free(nodes->nodes_array);
        free(nodes->edges_array);
        free(nodes);

    }

}

single_node_t* nodes_add_node(nodes_t* nodes, int node) {

    if (nodes->nodes_count == nodes->nodes_length) {

        nodes->nodes_length += BASESIZE_NODES_CHUNK;

        nodes->nodes_array = reallocarray(
            nodes->nodes_array,
            sizeof(*nodes->nodes_array),
            nodes->nodes_length
        );

    }

    single_node_t* new_node = single_node_create(node);

    int last_index = nodes->nodes_count;

    nodes->nodes_array[last_index] = new_node;

    nodes->nodes_count += 1;

    return new_node;

}

single_node_t* nodes_find(nodes_t* nodes, int number) {

    single_node_t* single_node;

    single_node = NULL;

    for (int index = 0; index < nodes->nodes_count; index += 1) {

        single_node_t* other_single_node = nodes->nodes_array[index];

        if (other_single_node->number == number) {

            single_node = other_single_node;
            break;

        }

    }

    return single_node;

}

void nodes_add_edge(nodes_t* nodes, const edge_t* edge) {

    single_node_t* node_a = nodes_find(nodes, edge->node_a);

    if (node_a == NULL) {

        node_a = nodes_add_node(nodes, edge->node_a);

    }

    single_node_t* node_b = nodes_find(nodes, edge->node_b);

    if (node_b == NULL) {

        node_b = nodes_add_node(nodes, edge->node_b);

    }

    if (nodes->edges_count == nodes->edges_length) {

        nodes->edges_length += BASESIZE_EDGES_CHUNK;

        nodes->edges_array = reallocarray(
            nodes->edges_array,
            sizeof(*nodes->edges_array),
            nodes->nodes_length
        );

    }

    edge_t* new_edge = edge_create(
        edge->node_a,
        edge->node_b,
        edge->weight
    );

    int last_index = nodes->edges_count;

    nodes->edges_array[last_index] = new_edge;

    nodes->edges_count += 1;

    single_node_add_edge(node_a, new_edge);

}

void nodes_replace_node(nodes_t* nodes, int node) {

    int last_index = nodes->nodes_count - 1;

    single_node_t* single_node;
    int single_index;

    single_node = NULL;
    single_index = (-1);

    for (int index = 0; index < nodes->nodes_count; index += 1) {

        single_node_t* other_single_node = nodes->nodes_array[index];

        if (other_single_node->number == node) {

            single_index = index;
            single_node = other_single_node;
            break;

        }

    }

    if (single_node == NULL) {

        return;

    }

    nodes->nodes_array[single_index] = nodes->nodes_array[last_index];
    nodes->nodes_count -= 1;

    single_node_destroy(single_node);

}

void nodes_delete_node(nodes_t* nodes, int node) {

    int last_index = nodes->nodes_count - 1;

    single_node_t* single_node;
    int single_index;

    single_node = NULL;
    single_index = (-1);

    for (int index = 0; index < nodes->nodes_count; index += 1) {

        single_node_t* other_single_node = nodes->nodes_array[index];

        if (other_single_node->number == node) {

            single_index = index;
            single_node = other_single_node;
            break;

        }

    }

    if (single_node == NULL) {

        return;

    }

    nodes->nodes_array[single_index] = nodes->nodes_array[last_index];
    nodes->nodes_count -= 1;

    for (int index = 0; index < nodes->edges_count; index += 1) {

        edge_t* edge = nodes->edges_array[index];

        if (edge->node_b == node) {

            single_node_t* other_node = nodes_find(nodes, edge->node_a);

            single_node_delete_edge(other_node, edge);

            int last_edge_index = nodes->edges_count - 1;

            nodes->edges_array[index] = nodes->edges_array[last_edge_index];

            nodes->edges_count -= 1;

            edge_destroy(edge);

        }

    }

    single_node_destroy(single_node);

}

void debug_present_nodes(nodes_t* nodes, FILE* output_file) {

    fprintf(output_file, "[NODES]\n\n");

    for (int index_a = 0; index_a < nodes->nodes_count; index_a += 1) {

        single_node_t* node = nodes->nodes_array[index_a];

        fprintf(output_file,"[NODE %d]\n", node->number);

        for (int index_b = 0; index_b < node->pointers_count; index_b += 1) {

            edge_t* edge = node->pointers_array[index_b];

            fprintf(
                output_file,
                "[EDGE FROM %d TO %d - WEIGHT %d]\n",
                edge->node_a,
                edge->node_b,
                edge->weight
            );

        }

    }

    fprintf(output_file, "\n");

}