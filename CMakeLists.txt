# SPDX-License-Identifier: AGPL-3.0-only
# Copyright (C) 2026 Poyraz Bakırtaş

cmake_minimum_required(VERSION 3.16)

project(NeuralNetwork LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(NeuralNetwork
    src/main.c
    src/neural_network.c
)

target_include_directories(NeuralNetwork
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
