#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edges.h"
#include "nodes.h"
#include "algo.h"

#define TOKEN_NEW_GRAPH                "A"
#define TOKEN_NEW_EDGES                "n"
#define TOKEN_DELETE_NODE              "D"
#define TOKEN_TRAVELLING_SALESMAN      "T"
#define TOKEN_SHORTEST_PATH            "S"
#define TOKEN_NEW_NODE                 "B"

/* SITUATION: A | n | D | T | S | B */
#define PARSER_SITUATION_ZERO          ( 0)

/* SITUATION: "A" decimal  */
#define PARSER_SITUATION_A0            ( 1)
#define PARSER_SITUATION_A1            ( 2)

/* SITUATION: "n" decimal continuation */
#define PARSER_SITUATION_B0            ( 3)
#define PARSER_SITUATION_B1            ( 4)
#define PARSER_SITUATION_B2            ( 5)
#define PARSER_SITUATION_B3            ( 6)

/* SITUATION: "D" decimal */
#define PARSER_SITUATION_C0            ( 8)
#define PARSER_SITUATION_C1            ( 9)

/* SITUATION: "S" decimal decimal */
#define PARSER_SITUATION_D0            (10)
#define PARSER_SITUATION_D1            (11)
#define PARSER_SITUATION_D2            (12)

/* SITUATION: "T" decimal continuation */
#define PARSER_SITUATION_E0            (13)
#define PARSER_SITUATION_E1            (14)
#define PARSER_SITUATION_E2            (15)

/* SITUATION: "B" decimal continuation */
#define PARSER_SITUATION_F0            (16)
#define PARSER_SITUATION_F1            (17)
#define PARSER_SITUATION_F2            (18)
#define PARSER_SITUATION_F3            (19)

#define FORMAT_DIJKSTRA                "Dijsktra shortest path: %d \n"
#define FORMAT_TRAVELLING_SALESMAN     "TSP shortest path: %d \n"

bool decimal_string(const char* input) {

    char* iterator = (char*) input;

    while (true) {

        char letter = *iterator;

        if (letter == '\0') {

            break;

        }

        if (letter < '0' || letter > '9') {

            return false;

        }

        iterator = &iterator[1];

    }

    return true;

}

int main(int argc, char* argv[]) {

    char next_token[100];    

    edge_t scratchpad_edge;

    dijkstra_t scratchpad_dijkstra;

    int travel_count;
    int* travel_nodes;
    travelling_salesman_t scratchpad_travelling_salesman;

    int present_situation;
    nodes_t* present_nodes;

    travel_nodes = NULL;
    scratchpad_travelling_salesman.travel_nodes = travel_nodes;

    present_situation = PARSER_SITUATION_ZERO;
    present_nodes = NULL;

    while (true) {

        int result = fscanf(stdin, "%s", next_token);

        /* break if nothing received from input */
        if (result < 0) {
            break;
        }

        switch (present_situation) {

            situation_zero:
            case PARSER_SITUATION_ZERO: {

                /* fprintf(stderr, "[SITUATION ZERO] '%s'\n", next_token); */

                if (strcmp(next_token, TOKEN_NEW_GRAPH) == 0) {

                    present_situation = PARSER_SITUATION_A0;
                    goto situation_a0;

                }

                if (strcmp(next_token, TOKEN_NEW_EDGES) == 0) {

                    present_situation = PARSER_SITUATION_B0;
                    goto situation_b0;

                }

                if (strcmp(next_token, TOKEN_DELETE_NODE) == 0) {

                    present_situation = PARSER_SITUATION_C0;
                    goto situation_c0;

                }

                if (strcmp(next_token, TOKEN_SHORTEST_PATH) == 0) {

                    present_situation = PARSER_SITUATION_D0;
                    goto situation_d0;

                }

                if (strcmp(next_token, TOKEN_TRAVELLING_SALESMAN) == 0) {

                    present_situation = PARSER_SITUATION_E0;
                    goto situation_e0;

                }

                if (strcmp(next_token, TOKEN_NEW_NODE) == 0) {

                    present_situation = PARSER_SITUATION_F0;
                    goto situation_f0;

                }

                /* the code should not reach this point */
                assert(false);

                break;

            }

            situation_a0:
            case PARSER_SITUATION_A0: {

                /* fprintf(stderr, "[SITUATION A0] '%s'\n", next_token); */

                present_situation = PARSER_SITUATION_A1;
                break;

            }

            case PARSER_SITUATION_A1: {

                /* fprintf(stderr, "[SITUATION A1] '%s'\n", next_token); */

                if (present_nodes != NULL) {

                    nodes_destroy(present_nodes);

                }

                int nodes_count = atoi(next_token);

                present_nodes = nodes_create(nodes_count);

                present_situation = PARSER_SITUATION_ZERO;
                break;

            }

            situation_b0:
            case PARSER_SITUATION_B0: {

                /* fprintf(stderr, "[SITUATION B0] '%s'\n", next_token); */

                present_situation = PARSER_SITUATION_B1;
                break;

            }


            case PARSER_SITUATION_B1: {

                /* fprintf(stderr, "[SITUATION B1] '%s'\n", next_token); */

                scratchpad_edge.node_a = atoi(next_token);

                present_situation = PARSER_SITUATION_B2;
                break;

            }

            situation_b2:
            case PARSER_SITUATION_B2: {

                /* fprintf(stderr, "[SITUATION B2] '%s'\n", next_token); */

                if (!decimal_string(next_token)) {

                    present_situation = PARSER_SITUATION_ZERO;
                    goto situation_zero;

                }

                scratchpad_edge.node_b = atoi(next_token);

                present_situation = PARSER_SITUATION_B3;
                break;

            }

            case PARSER_SITUATION_B3: {

                /* fprintf(stderr, "[SITUATION B3] '%s'\n", next_token); */

                scratchpad_edge.weight = atoi(next_token);

                nodes_add_edge(present_nodes, &scratchpad_edge);

                present_situation = PARSER_SITUATION_B2;
                break;

            }

            situation_c0:
            case PARSER_SITUATION_C0: {

                /* fprintf(stderr, "[SITUATION C0] '%s'\n", next_token); */

                present_situation = PARSER_SITUATION_C1;
                break;

            }

            case PARSER_SITUATION_C1: {

                /* fprintf(stderr, "[SITUATION C1] '%s'\n", next_token); */

                int deleted_node = atoi(next_token);

                nodes_delete_node(present_nodes, deleted_node);

                present_situation = PARSER_SITUATION_ZERO;
                break;

            }

            situation_d0:
            case PARSER_SITUATION_D0: {

                /* fprintf(stderr, "[SITUATION D0] '%s'\n", next_token); */

                scratchpad_dijkstra.nodes = present_nodes;

                present_situation = PARSER_SITUATION_D1;
                break;

            }

            case PARSER_SITUATION_D1: {

                /* fprintf(stderr, "[SITUATION D1] '%s'\n", next_token); */

                scratchpad_dijkstra.node_a = atoi(next_token);

                present_situation = PARSER_SITUATION_D2;
                break;

            }

            case PARSER_SITUATION_D2: {

                /* fprintf(stderr, "[SITUATION D2] '%s'\n", next_token); */

                scratchpad_dijkstra.node_b = atoi(next_token);

                int result_dijkstra = algo_dijkstra(&scratchpad_dijkstra);

                fprintf(stdout, FORMAT_DIJKSTRA, result_dijkstra);

                present_situation = PARSER_SITUATION_ZERO;
                break;

            }

            situation_e0:
            case PARSER_SITUATION_E0: {

                /* fprintf(stderr, "[SITUATION E0] '%s'\n", next_token); */

                scratchpad_travelling_salesman.nodes = present_nodes;

                present_situation = PARSER_SITUATION_E1;
                break;

            }

            case PARSER_SITUATION_E1: {

                /* fprintf(stderr, "[SITUATION E1] '%s'\n", next_token); */

                travel_nodes = scratchpad_travelling_salesman.travel_nodes;

                if (travel_nodes != NULL) {

                    free(travel_nodes);
                    travel_nodes = NULL;

                }

                travel_count = atoi(next_token);
                travel_nodes = calloc(travel_count, sizeof(*travel_nodes));

                scratchpad_travelling_salesman.travel_count = travel_count;
                scratchpad_travelling_salesman.travel_nodes = travel_nodes;

                present_situation = PARSER_SITUATION_E2;
                break;

            }

            case PARSER_SITUATION_E2: {

                /* fprintf(stderr, "[SITUATION E2] '%s'\n", next_token); */

                *travel_nodes = atoi(next_token);

                travel_nodes = &travel_nodes[1];
                travel_count = travel_count - 1;

                if (travel_count == 0) {

                    int result_travelling_salesman = algo_travelling_salesman(
                        &scratchpad_travelling_salesman
                    );

                    fprintf(
                        stdout,
                        FORMAT_TRAVELLING_SALESMAN,
                        result_travelling_salesman
                    );

                    present_situation = PARSER_SITUATION_ZERO;

                }

                break;

            }

            situation_f0:
            case PARSER_SITUATION_F0: {

                /* fprintf(stderr, "[SITUATION F0] '%s'\n", next_token); */

                present_situation = PARSER_SITUATION_F1;
                break;

            }

            case PARSER_SITUATION_F1: {

                /* fprintf(stderr, "[SITUATION F1] '%s'\n", next_token); */

                scratchpad_edge.node_a = atoi(next_token);

                nodes_replace_node(present_nodes, scratchpad_edge.node_a);

                nodes_add_node(present_nodes, scratchpad_edge.node_a);

                present_situation = PARSER_SITUATION_F2;
                break;

            }

            situation_f2:
            case PARSER_SITUATION_F2: {

                /* fprintf(stderr, "[SITUATION F2] '%s'\n", next_token); */

                if (!decimal_string(next_token)) {

                    present_situation = PARSER_SITUATION_ZERO;
                    goto situation_zero;

                }

                scratchpad_edge.node_b = atoi(next_token);

                present_situation = PARSER_SITUATION_F3;
                break;

            }

            case PARSER_SITUATION_F3: {

                /* fprintf(stderr, "[SITUATION F3] '%s'\n", next_token); */

                scratchpad_edge.weight = atoi(next_token);

                nodes_add_edge(present_nodes, &scratchpad_edge);

                present_situation = PARSER_SITUATION_F2;
                break;

            }

        }

    }

    travel_nodes = scratchpad_travelling_salesman.travel_nodes;

    if (travel_nodes != NULL) {

        free(travel_nodes);

    }

    if (present_nodes != NULL) {

        nodes_destroy(present_nodes);

    }

    return 0;

}
