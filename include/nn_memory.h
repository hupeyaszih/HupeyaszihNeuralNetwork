#ifndef NN_MEMORY_H
#define NN_MEMORY_H

#include <stdbool.h>

typedef struct EventList EventList;

int add_event(EventList* event_list, int target_time, int neuron_id);
int delete_event(EventList* event_list,int event_id);

int delete_event_list(EventList* event_list);
EventList* create_event_list(int capacity);

#endif
