// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) 2026 Poyraz Bakırtaş

#ifndef GLOBALS_H
#define GLOBALS_H



#define NODE_COUNT 100

#define MIN_EDGE_COUNT_PER_NODE 4
#define MAX_EDGE_COUNT_PER_NODE 16

#define MIN_DELAY 7
#define MAX_DELAY 24




#define DEBUG_MODE

#ifdef DEBUG_MODE
#include<stdio.h>
#define LOG_PRINT(x) printf("%s\n", x)
#define LOG_PRINT_INT(x) printf("%d\n", x)
#else
#define LOG_PRINT(x)
#define LOG_PRINT_INT(x) 
#endif


#endif
