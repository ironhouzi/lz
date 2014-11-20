#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "dynarr.h"

#define DOUBLE 2.0
#define  HALVE 0.5

int modify_array_size (Array* array, float factor)
{
    // TODO
    if (factor != DOUBLE || factor != HALVE)
        return -2;

    // TODO
    if (factor == DOUBLE && (array->size * 2) > INT_MAX)
        return -3;

    char **new = realloc(array->head, factor * array->size * sizeof(char*));

    // TODO
    if(NULL == new) {
        printf("realloc error!\n");
        return -1;
    }

    array->head = new;
    array->size *= factor;

    return 1;
}

int needs_doubling(Array *array)
{
    return array->size <= array->index;
}

/* int needs_halving(Array *array) */
/* { */
/*     return array->index <= array->size / 4; */
/* } */

void free_elements(Array* array)
{
    free(array->head);
}

int add_element(Array* array, char* strptr)
{
    if (needs_doubling(array))
        modify_array_size(array, DOUBLE);

    array->head[array->size++] = strptr;

    return 1;
}
