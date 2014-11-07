#include "DynArr.h"

/* Prototypes */
void __resize_array(struct DynArr *array, int new_size);


void __resize_array(struct DynArr *array, int new_size)
{
    void *tmp = malloc(new_size * array->elt_size);
    memcpy(tmp, array->array, array->size_array * array->elt_size);
    free(array->array);
    array->array = tmp;
    array->cap_array = new_size;
}

struct DynArr *create_dyn_array(int initial_cap, int item_size)
{
    struct DynArr *dyn_arr = malloc(sizeof(struct DynArr));
    dyn_arr->array = malloc(item_size * initial_cap);
    dyn_arr->size_array = 0;
    dyn_arr->elt_size = item_size;
    dyn_arr->cap_array = initial_cap;

    return dyn_arr;
}

void free_dyn_array(struct DynArr **dyn_arr)
{
    free((*dyn_arr)->array);
    free(*dyn_arr);
    *dyn_arr = NULL;
}


void add_dyn_array(struct DynArr *dyn_arr, void *item)
{
    int write_pos = dyn_arr->size_array * dyn_arr->elt_size;
    memcpy(dyn_arr->array + write_pos, item, dyn_arr->elt_size);
    dyn_arr->size_array++;

    if(dyn_arr->size_array == dyn_arr->cap_array)
        __resize_array(dyn_arr, dyn_arr->size_array * 2);
}

void *get_dyn_array(struct DynArr *dyn_arr, int index)
{
    return (void *)dyn_arr->array + index * dyn_arr->elt_size; 
}
