// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) 2026 Poyraz Bakırtaş

#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include "globals.h"

typedef struct NeuralNetwork NeuralNetwork;
typedef struct EdgeSoA EdgeSoA;
typedef struct NodeSoA NodeSoA;

NeuralNetwork* init_neural_network();
void update_neural_network(NeuralNetwork* network, int t);

int new_edge(EdgeSoA* edgeSoA, int max_edge_count, int from_id, int target_id, float weight, int delay); 
int new_node(NodeSoA* nodeSoA, int edge_count, int edge_start_idx);

int delete_edgeSoA(EdgeSoA* edgeSoA); 
int delete_nodeSoA(NodeSoA* nodeSoA); 
NeuralNetwork* delete_neural_network(NeuralNetwork* neural_network);

#endif
