#include "neural_network.h"
#include "globals.h"
#include "nn_errors.h"
#include "nn_math.h"
#include <stdlib.h>

struct NeuralNetwork{
    EdgeSoA* edges;
    NodeSoA* nodes;
};
struct EdgeSoA {
    int* from_id;
    int* target_id;
    float* weight;
    int* delay;

    int edge_count;
};
struct NodeSoA {
    int* edge_start_idx;
    int* edge_count;

    int node_count;
};

NeuralNetwork* init_neural_network() {
    //srand(time(NULL)); // Disabled for easy debugging (same network same result)
    int error = NN_SUCCESS;
    int max_edge_count = 0;

    NeuralNetwork* network = malloc(sizeof(NeuralNetwork));
    NodeSoA* nodeSoA = NULL;
    EdgeSoA* edgeSoA = NULL;
    network->nodes = NULL;
    network->edges = NULL;
    do {
        // Init NodeSoA
        nodeSoA = malloc(sizeof(NodeSoA));
        network->nodes = nodeSoA;
        if(nodeSoA == NULL){error = NN_FAILED_TO_CREATE_NODE_SOA;break;}
        nodeSoA->node_count = 0;

        nodeSoA->edge_start_idx = NULL;
        nodeSoA->edge_count = NULL;

        nodeSoA->edge_start_idx = malloc(sizeof(int)*NODE_COUNT);
        if(nodeSoA->edge_start_idx == NULL) {error = NN_FAILED_TO_CREATE_NODE_SOA;break;}
        nodeSoA->edge_count = malloc(sizeof(int)*NODE_COUNT);
        if(nodeSoA->edge_count == NULL) {error = NN_FAILED_TO_CREATE_NODE_SOA;break;}

        // Create Nodes
        {
            int current_edge_count = 0;
            for(int i = 0;i < NODE_COUNT;i++) {
                int edge_count = GET_RANDOM_RANGE(MIN_EDGE_COUNT_PER_NODE, MAX_EDGE_COUNT_PER_NODE);
                new_node(nodeSoA, edge_count, current_edge_count);   
                current_edge_count += edge_count;
            }
            max_edge_count = current_edge_count;
        }
        // Init EdgeSoA
        edgeSoA = malloc(sizeof(EdgeSoA));
        network->edges = edgeSoA;
        if(edgeSoA == NULL){error = NN_FAILED_TO_CREATE_EDGE_SOA;break;}
        edgeSoA->edge_count = 0;

        edgeSoA->from_id = NULL;
        edgeSoA->target_id = NULL;
        edgeSoA->weight = NULL;
        edgeSoA->delay = NULL;

        edgeSoA->from_id = malloc(sizeof(int)*max_edge_count);
        if(edgeSoA->from_id == NULL) {error = NN_FAILED_TO_CREATE_EDGE_SOA;break;}
        edgeSoA->target_id = malloc(sizeof(int)*max_edge_count);
        if(edgeSoA->target_id == NULL) {error = NN_FAILED_TO_CREATE_EDGE_SOA;break;}
        edgeSoA->weight = malloc(sizeof(float)*max_edge_count);
        if(edgeSoA->weight == NULL) {error = NN_FAILED_TO_CREATE_EDGE_SOA;break;}
        edgeSoA->delay = malloc(sizeof(int)*max_edge_count);
        if(edgeSoA->delay == NULL) {error = NN_FAILED_TO_CREATE_EDGE_SOA;break;}
    }while(0);

    if(error != NN_SUCCESS) {LOG_PRINT_INT(error);delete_neural_network(network) ;return NULL;}

    // Create Edges
    for(int i = 0;i < NODE_COUNT;i++) {
        for(int ed = 0;ed < nodeSoA->edge_count[i];ed++) {
            int target_id = -1;
            int delay = GET_RANDOM_RANGE(MIN_DELAY, MAX_DELAY); 
            float weight = (float) (rand() % 100) / 100;

            do {
                target_id = rand() % NODE_COUNT;
            }while (target_id == i);

            new_edge(edgeSoA,max_edge_count, i, target_id, weight, delay);
        }
    }

    return network;
}

void update_neural_network(NeuralNetwork* network, int t) {
    LOG_PRINT("update");
    LOG_PRINT_INT(t);
}

int new_edge(EdgeSoA* edgeSoA, int max_edge_count, int from_id, int target_id, float weight, int delay) {
    if(edgeSoA->edge_count >= max_edge_count) return NN_FAILED_TO_CREATE_EDGE;
    int new_edge_id = edgeSoA->edge_count;

    edgeSoA->delay[new_edge_id] = delay;
    edgeSoA->weight[new_edge_id] = weight;
    edgeSoA->from_id[new_edge_id] = from_id;
    edgeSoA->target_id[new_edge_id] = target_id;

    edgeSoA->edge_count++;
    return NN_SUCCESS;
} 

int new_node(NodeSoA* nodeSoA, int edge_count, int edge_start_idx) {
    if(nodeSoA->node_count >= NODE_COUNT) return NN_FAILED_TO_CREATE_NODE;
    int new_node_id = nodeSoA->node_count;

    nodeSoA->edge_count[new_node_id] = edge_count;
    nodeSoA->edge_start_idx[new_node_id] = edge_start_idx;

    nodeSoA->node_count++;
    return NN_SUCCESS;
}

int delete_edgeSoA(EdgeSoA* edgeSoA) {
    if(edgeSoA->target_id){free(edgeSoA->target_id); edgeSoA->target_id = NULL;}
    if(edgeSoA->weight){free(edgeSoA->weight); edgeSoA->weight = NULL;}
    if(edgeSoA->from_id){free(edgeSoA->from_id); edgeSoA->from_id = NULL;}
    if(edgeSoA->delay){free(edgeSoA->delay); edgeSoA->delay = NULL;}
    if(edgeSoA){free(edgeSoA); edgeSoA = NULL;}
    return NN_SUCCESS;
}

int delete_nodeSoA(NodeSoA* nodeSoA) {
    if(nodeSoA->edge_count){free(nodeSoA->edge_count); nodeSoA->edge_count = NULL;}
    if(nodeSoA->edge_start_idx){free(nodeSoA->edge_start_idx);nodeSoA->edge_start_idx = NULL;}
    if(nodeSoA)free(nodeSoA);
    return NN_SUCCESS;
}

NeuralNetwork* delete_neural_network(NeuralNetwork* neural_network) {
    if(!neural_network) return neural_network;

    if(neural_network->edges)delete_edgeSoA(neural_network->edges);
    if(neural_network->nodes)delete_nodeSoA(neural_network->nodes);
    free(neural_network);
    return NULL;
}
