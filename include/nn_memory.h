// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) 2026 Poyraz Bakırtaş

#ifndef NN_MEMORY_H
#define NN_MEMORY_H

#include <stdbool.h>

typedef struct EventList EventList;

typedef struct TimeTable TimeTable;
typedef struct TimeWindow TimeWindow;

/// Time Table
int clear_time_window(TimeTable* time_table,int time_window_id, int new_target_time);

int delete_time_table(TimeTable* time_table);
TimeTable* create_time_table(int capacity);

/// Time Window
int free_time_window(TimeWindow* time_window);


/// Event List
int add_event(EventList* event_list, int target_time, int neuron_id);
int delete_event(EventList* event_list,int event_id);

int delete_event_list(EventList* event_list);
EventList* create_event_list(int capacity);

#endif
