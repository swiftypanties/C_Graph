#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

#include "nodes.h"
#include "algo.h"

#include <stdio.h>

struct dijkstra_node {
    single_node_t* single_node;
    struct dijkstra_node* back;
    int weight;
};

typedef struct dijkstra_node dijkstra_node_t;

int algo_dijkstra(const dijkstra_t* data_dijkstra) {

    /* debug_present_nodes(nodes, stderr); */

    nodes_t* nodes = data_dijkstra->nodes;
    int node_a = data_dijkstra->node_a;
    int node_b = data_dijkstra->node_b;

    single_node_t** nodes_array = nodes->nodes_array;
    int nodes_count = nodes->nodes_count;

    int dijkstra_length = nodes_count;
    dijkstra_node_t** dijkstra_nodes = calloc(
        dijkstra_length,
        sizeof(*dijkstra_nodes)
    );

    for (int index = 0; index < dijkstra_length; index += 1) {

        dijkstra_node_t* node = malloc(sizeof(*node));

        node->single_node = nodes_array[index];
        node->weight = INT_MAX;
        node->back = NULL;

        if (nodes_array[index]->number == node_a) {

            node->weight = 0;

        }

        dijkstra_nodes[index] = node;

    }

    dijkstra_node_t* dijkstra_final = NULL;

    int sum_weights = (-1);

    while (true) {

        int index_last = dijkstra_length - 1;

        int weight_lowest = INT_MAX;
        int index_lowest = (-1);

        /* searches for the node with the least weight as starting node */
        for (int index = 0; index < dijkstra_length; index += 1) {

            dijkstra_node_t* dijkstra_node = dijkstra_nodes[index];

            int weight_node = dijkstra_node->weight;

            if (weight_node < weight_lowest) {

                weight_lowest = weight_node;
                index_lowest = index;

            }

        }

        /* no proper node found, failure */
        if (index_lowest < 0) {

            sum_weights = (-1);
            goto dijkstra_failure;

        }

        /* erases node from array */
        dijkstra_node_t* dijkstra_last = dijkstra_nodes[index_last];
        dijkstra_node_t* dijkstra_lowest = dijkstra_nodes[index_lowest];

        dijkstra_nodes[index_last] = dijkstra_lowest;
        dijkstra_nodes[index_lowest] = dijkstra_last;

        dijkstra_length -= 1;

        /* breaks if found last node, completing the path */
        if (dijkstra_lowest->single_node->number == node_b) {

            dijkstra_final = dijkstra_lowest;
            break;

        }

        edge_t** edges_array = dijkstra_lowest->single_node->pointers_array;
        int edges_count = dijkstra_lowest->single_node->pointers_count;

        /* calculates new dijkstra weights for nodes using edges */
        for (int index_a = 0; index_a < edges_count; index_a += 1) {

            edge_t* edge = edges_array[index_a];

            for (int index_b = 0; index_b < dijkstra_length; index_b += 1) {

                dijkstra_node_t* dijkstra_other = dijkstra_nodes[index_b];
                single_node_t* node_other = dijkstra_other->single_node;

                /* found edge from lowest node to another node */
                if (node_other->number == edge->node_b) {

                    int new_weight = dijkstra_lowest->weight + edge->weight;

                    if (new_weight < dijkstra_other->weight) {

                        dijkstra_other->weight = new_weight;
                        dijkstra_other->back = dijkstra_lowest;

                    }

                }

            }

        }

    }

    sum_weights = dijkstra_final->weight;

    /* use this code to traverse back the shortest path if needed */

    // while (dijkstra_final != NULL) {
    //
    //     dijkstra_final = dijkstra_final->back;
    //
    // }

dijkstra_failure:

    dijkstra_length = nodes_count;

     for (int index = 0; index < dijkstra_length; index += 1) {

        free(dijkstra_nodes[index]);

    }

    free(dijkstra_nodes);

    return sum_weights;

}

static bool permutation_next(int* numbers, int count) {

    /* arrays of one element or less don't have a next permutation */
    if (count <= 1) {

        return false;

    }

	int last_index = count - 1;

    int index_a = last_index;

    while (index_a > 0) {

        int integer_a = numbers[index_a - 0];
        int integer_b = numbers[index_a - 1];

        if (integer_b < integer_a) {

            break;

        }

        index_a -= 1;

    }

	/* the array is sorted in descending order, no next permutation */
	if (index_a == 0) {

		return 0;

	}

	int value_reference = numbers[index_a - 1];

	int index_b = last_index;

	while (true) {

		int value_other = numbers[index_b];

		if (value_other > value_reference) {
			break;
		}

		index_b -= 1;

	}

	int swap_a = numbers[index_a - 1];
	int swap_b = numbers[index_b - 0];
	numbers[index_a - 1] = swap_b;
	numbers[index_b - 0] = swap_a;

	int index_c = last_index;

	while (index_a < index_c) {

		int swap_a = numbers[index_a];
		int swap_c = numbers[index_c];
		numbers[index_a] = swap_c;
		numbers[index_c] = swap_a;

		index_a += 1;
		index_c -= 1;

	}

	return true;

}

static void debug_present_permutation(
    int* numbers,
    int count,
    FILE* output_file
) {

    fprintf(output_file, "[PERMUTATION]:\n");

    for (int index = 0; index < count; index += 1) {

        fprintf(output_file, "%d ", numbers[index]);

    }

    fprintf(output_file, "\n");
    
}

int algo_travelling_salesman(const travelling_salesman_t* data_travelling_salesman) {

    nodes_t* nodes = data_travelling_salesman->nodes;
    int travel_count = data_travelling_salesman->travel_count;
    int* travel_nodes = data_travelling_salesman->travel_nodes;

    /* debug_present_nodes(nodes, stderr); */

    /* sort values in ascending order using bubble sort */
    while (true) {

        int swaps = 0;

        for (int index = 1; index < travel_count; index += 1) {

            int node_a = travel_nodes[index - 0];
            int node_b = travel_nodes[index - 1];

            if (node_a < node_b) {

                travel_nodes[index - 0] = node_b;
                travel_nodes[index - 1] = node_a;
                swaps += 1;

            }

        }

        if (swaps == 0) {

            break;

        }

    }

    int lowest_value = INT_MAX;

    while (true) {

        /* debug_present_permutation(travel_nodes, travel_count, stderr); */

        /* brute-force by searching through all permutation paths */
        for (int index_a = 0; index_a < travel_count; index_a += 1) {

            int other_value = 0;

            for (int index_b = 1; index_b < travel_count; index_b += 1) {

                int node_a = travel_nodes[index_b - 1];
                int node_b = travel_nodes[index_b - 0];

                dijkstra_t data_dijkstra;
                data_dijkstra.nodes = nodes;
                data_dijkstra.node_a = node_a;
                data_dijkstra.node_b = node_b;

                int lowest_weight = algo_dijkstra(&data_dijkstra);

                /* the path is broken, must try another permutation */
                if (lowest_weight < 0) {

                    goto broken_path;

                }

                other_value += lowest_weight;

            }

            if (other_value < lowest_value) {

                lowest_value = other_value;

            }

        }

        bool new_permutation;

        broken_path:

        new_permutation = permutation_next(travel_nodes, travel_count);

        if (!new_permutation) {

            break;

        }

    }

    if (lowest_value == INT_MAX) {

        return (-1);

    }

    return lowest_value;

}