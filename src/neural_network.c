// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) 2026 Poyraz Bakırtaş

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
    float* membrane_potential;
    float* threshold;
    int* last_spike_time;
    int* last_update_time;

    int* edge_start_idx;
    //int* input_edges; //I will add this
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

        nodeSoA->membrane_potential = NULL;
        nodeSoA->threshold = NULL;
        nodeSoA->last_spike_time = NULL;
        nodeSoA->last_update_time = NULL;

        nodeSoA->edge_start_idx = NULL;
        nodeSoA->edge_count = NULL;

        nodeSoA->edge_start_idx = malloc(sizeof(int)*NODE_COUNT);
        if(nodeSoA->edge_start_idx == NULL) {error = NN_FAILED_TO_CREATE_NODE_SOA;break;}
        nodeSoA->edge_count = malloc(sizeof(int)*NODE_COUNT);
        if(nodeSoA->edge_count == NULL) {error = NN_FAILED_TO_CREATE_NODE_SOA;break;}
        nodeSoA->membrane_potential = malloc(sizeof(float)*NODE_COUNT);
        if(nodeSoA->membrane_potential == NULL) {error = NN_FAILED_TO_CREATE_NODE_SOA;break;}
        nodeSoA->threshold = malloc(sizeof(int)*NODE_COUNT);
        if(nodeSoA->threshold == NULL) {error = NN_FAILED_TO_CREATE_NODE_SOA;break;}
        nodeSoA->last_update_time = malloc(sizeof(int)*NODE_COUNT);
        if(nodeSoA->last_update_time == NULL) {error = NN_FAILED_TO_CREATE_NODE_SOA;break;}
        nodeSoA->last_spike_time = malloc(sizeof(int)*NODE_COUNT);
        if(nodeSoA->last_spike_time == NULL) {error = NN_FAILED_TO_CREATE_NODE_SOA;break;}

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
    // Simple Update - I'll add event-based update 
    LOG_PRINT("UPDATE");
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
    if(nodeSoA->membrane_potential){free(nodeSoA->membrane_potential);nodeSoA->membrane_potential = NULL;}
    if(nodeSoA->threshold){free(nodeSoA->threshold);nodeSoA->threshold = NULL;}
    if(nodeSoA->last_spike_time){free(nodeSoA->last_spike_time);nodeSoA->last_spike_time = NULL;}
    if(nodeSoA->last_update_time){free(nodeSoA->last_update_time);nodeSoA->last_update_time = NULL;}
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

/// NODE GETTERS
static inline float get_membrane_potential(NodeSoA* nodeSoA,int index) {return nodeSoA->membrane_potential[index];}
static inline float get_threshold(NodeSoA* nodeSoA,int index) {return nodeSoA->threshold[index];}

static inline int get_last_spike_time(NodeSoA* nodeSoA,int index) {return nodeSoA->last_spike_time[index];}
static inline int get_last_update_time(NodeSoA* nodeSoA,int index) {return nodeSoA->last_update_time[index];}

static inline int get_edge_start_idx(NodeSoA* nodeSoA,int index) {return nodeSoA->edge_start_idx[index];}
static inline int get_edge_count(NodeSoA* nodeSoA,int index) {return nodeSoA->edge_count[index];}

/// NODE SETTERS
static inline void set_membrane_potential(NodeSoA* nodeSoA,int index, float value) {nodeSoA->membrane_potential[index] = value;}
static inline void set_threshold(NodeSoA* nodeSoA,int index, float value) {nodeSoA->threshold[index] = value;}

static inline void set_last_spike_time(NodeSoA* nodeSoA,int index, int value) {nodeSoA->last_spike_time[index] = value;}
static inline void set_last_update_time(NodeSoA* nodeSoA,int index, int value) {nodeSoA->last_update_time[index] = value;}

//static inline void set_edge_start_idx(NodeSoA nodeSoA,int index, int value) {nodeSoA.edge_start_idx[index] = value;}
//static inline void set_edge_count(NodeSoA nodeSoA,int index, int value) {nodeSoA.edge_count[index] = value;}

/// EDGE GETTERS
static inline int get_from_id(EdgeSoA* edgeSoA, int index) {return edgeSoA->from_id[index];}
static inline int get_target_id(EdgeSoA* edgeSoA, int index) {return edgeSoA->target_id[index];}
static inline int get_weight(EdgeSoA* edgeSoA, int index) {return edgeSoA->weight[index];}
static inline int get_delay(EdgeSoA* edgeSoA, int index) {return edgeSoA->delay[index];}

/// EDGE SETTERS
static inline void set_from_id(EdgeSoA* edgeSoA, int index, int value) {edgeSoA->from_id[index] = value;}
static inline void set_target_id(EdgeSoA* edgeSoA, int index, int value) {edgeSoA->target_id[index] = value;}
static inline void set_weight(EdgeSoA* edgeSoA, int index, int value) {edgeSoA->weight[index] = value;}
static inline void set_delay(EdgeSoA* edgeSoA, int index, int value) {edgeSoA->delay[index] = value;}


/// OTHER HELPER FUNCTIONS
static inline int get_edge_id(NodeSoA* nodeSoA, int node_id, int edge) {return nodeSoA->edge_start_idx[node_id] + edge;}

