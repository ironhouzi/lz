#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "dynarr.h"

#define DOUBLE 2.0
#define  HALVE 0.5


Entry* new_entry(const char* string)
{
    int length = strlen(string);
    Entry* entry = malloc(sizeof(Entry));
    entry->string = malloc(length + 1);
    strncpy(entry->string, string, length + 1);
    entry->rank = INT_MAX;
    entry->length = length;
    return entry;
}

void del_entry(Entry * entry)
{
    free(entry->string);
    free(entry);
}

void free_all(Entry** all_entries, const int entry_count)
{
    for (int i = 0; i < entry_count; i++)
        del_entry(all_entries[i]);

    free(all_entries);
}

int modify_array_size (Array* array, const float factor)
{
    // TODO
    if (factor != DOUBLE || factor != HALVE)
        return -2;

    // TODO
    if (factor == DOUBLE && (array->size * 2) > INT_MAX)
        return -3;

    Entry **new = realloc(array->elements,
                          factor * array->size * sizeof(Entry *));

    // TODO
    if(NULL == new) {
        printf("realloc error!\n");
        return -1;
    }

    array->elements = new;
    array->size *= factor;

    return 1;
}

int needs_doubling(Array *array)
{
    return array->size <= array->index;
}

void free_array(Array* array)
{
    for (int i = 0; i < array->index; i++)
        free(array->elements[i]);

    free(array);
}

int add_element(Array* array, Entry* entry)
{
    if (needs_doubling(array))
        modify_array_size(array, DOUBLE);

    array->elements[array->size] = malloc(sizeof(Entry *));
    array->elements[array->size] = entry;
    array->size++;

    return 1;
}
