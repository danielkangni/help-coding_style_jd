#include <stdio.h>
#include <stdlib.h>
#include "array.h"
int main(void)
{
    array_t my_array;
    array_init(&my_array, 5);
    for (size_t i = 0; i < my_array.size(&my_array); i++) {
        int *value = malloc(sizeof(int));
        *value = (int)(i * 10);
        my_array.set(&my_array, i, value);
    }
    for (size_t i = 0; i < my_array.size(&my_array); i++) {
        int *value = (int *)my_array.get(&my_array, i);
        if (value)
            printf("Element at index %zu: %d\n", i, *value);
    }
    array_destroy(&my_array);
    return 0;
}