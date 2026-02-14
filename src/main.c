// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) 2026 Poyraz Bakırtaş

#include <stdio.h>

#include "nn_errors.h"
#include "neural_network.h"

int main(){
    NeuralNetwork* network = init_neural_network();
    if(network == NULL) return 0; 

    for(int t = 0;t < 5;t++){
        update_neural_network(network, t);
    }
    delete_neural_network(network);
    return 0;
}
