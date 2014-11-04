#ifndef DYNARR_H
#define DYNARR_H

#include <string.h>
#include <stdlib.h>

struct DynArr
{
    void *array;
    int size_array; 
    int cap_array;
    int elt_size;
};

struct DynArr *create_dyn_array(int initial_cap, int item_size);
void free_dyn_array(struct DynArr **dyn_arr);

void add_dyn_array(struct DynArr *dyn_arr, void *item);
void *get_dyn_array(struct DynArr *dyn_arr, int index);
//void remove_at(struct DynArr *dyn_arr, int index);

#endif
