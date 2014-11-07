#include <stdio.h>
#include "DynArr.h"

void print_stats(struct DynArr *arr)
{
    int i = 0;

    printf("DynArr size: %d\n", arr->size_array);
    printf("DynArr cap: %d\n", arr->cap_array);

    for(i = 0; i < arr->size_array; i++)
    {
        printf("element %d value: %d\n", i, *(int*)(get_dyn_array(arr, i)));    
    }
}

int main(int argc, char **argv)
{
    struct DynArr *arr = NULL;
    int i = 0;

    arr = create_dyn_array(5, sizeof(int));
    
    int vals[100];
    for(i=0; i < 100; i++)
    {
        vals[i] = i;
    }

    for(i=0; i < 100; i++)
    {
        add_dyn_array(arr, vals + i);    
    }

    print_stats(arr);
    return 0;
}
