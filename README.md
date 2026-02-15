# Hupeyaszih Neural Network (HNN)
Graph-based spiking neural network simulator with dopamine-modulated plasticity written in C.

⚠️This project is under active development.

## Research Direction

HNN aims to explore biologically plausible learning mechanisms
such as STDP and neuromodulator-driven plasticity instead of
backpropagation.

## Current Status
- Configurable neuron allocation
- Directed synapse graph construction

## What It Does (Currently)
Currently constructs configurable neural graphs with explicit neuron and synapse allocation. Learning dynamics are not yet implemented.

## Planned Features
- [ ] STDP
- [ ] Dopamine-modulated synaptic weight updates
- [ ] Local and global dopamine diffusion models
- [ ] Discrete-time simulation loop

## Build
```bash
    git clone https://github.com/hupeyaszih/HupeyaszihNeuralNetwork.git
    cd HupeyaszihNeuralNetwork
    cd build
    cmake build ..
    cmake --build .

```

## Architecture

### Current

- Fully graph-based topology
- No predefined input/hidden/output layers
- Explicit neuron and synapse allocation

### Planned

- Event-driven spike propagation
- Timestamped spike event queue
- Pluggable neuron model (custom spike dynamics)
- Dopamine-modulated synaptic plasticity
