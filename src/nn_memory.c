// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) 2026 Poyraz Bakırtaş

#include "nn_memory.h"
#include "nn_errors.h"
#include <stddef.h>
#include <stdlib.h>


struct EventList{
   unsigned int* time;
   int* neuron_id;
   bool* done;

   size_t capacity; 
   size_t size;
};

struct TimeTable{
    int size;
    TimeWindow* time_windows;
};

struct TimeWindow{
    int event_start_idx;
    int target_time; // ex: t = 10, t = 9..
    int event_count;
};

int add_event(EventList* event_list, int target_time, int neuron_id) {
    if(event_list->size+1 > event_list->capacity) return NN_ERR_EVENT_LIST_CAPACITY;
    int id = event_list->size + 1;
    event_list->time[id] = target_time;
    event_list->neuron_id[id] = neuron_id;
    event_list->done[id] = false;
    return NN_SUCCESS;
}
int delete_event(EventList* event_list,int event_id) {
    if(event_id >= event_list->size) return NN_FAILED_TO_DELETE_EVENT;
    event_list->time[event_id] = -1;
    event_list->neuron_id[event_id] = -1;
    event_list->done[event_id] = false;
    return NN_SUCCESS;
}

int delete_event_list(EventList* event_list) {
    if(event_list->time){free(event_list->time); event_list->time = NULL;}
    if(event_list->neuron_id){free(event_list->neuron_id); event_list->neuron_id = NULL;}
    if(event_list->done){free(event_list->done); event_list->done = NULL;}
    if(event_list){free(event_list); event_list = NULL;}
    return NN_SUCCESS;
}
EventList* create_event_list(int capacity) {
    EventList* event_list = malloc(sizeof(EventList));
    event_list->time = malloc(sizeof(unsigned int) * capacity);
    event_list->neuron_id = malloc(sizeof(int) * capacity);
    event_list->done = malloc(sizeof(bool) * capacity);
    return event_list;
}
