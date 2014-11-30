#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "dynarr.h"

#define FACTOR 2
#define INIT_ARRAY_SIZE 256


Entry* new_entry(const char* string)
{
    int length = strlen(string);
    Entry *entry = malloc(sizeof (Entry));
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

int grow_array(Array *array)
{
    Entry **new = realloc(array->elements,
                          FACTOR * array->size * sizeof (Entry *));

    // TODO
    if(NULL == new) {
        printf("realloc error!\n");
        return 0;
    }

    array->elements = new;
    array->size *= FACTOR;

    return array->size;
}

int needs_doubling(Array *array)
{
    return array->size <= array->index;
}

void free_array(Array *array)
{
    free_all(array->elements, array->index);
    free(array);
}

Array *new_array(void)
{
    Array *array = malloc(sizeof (Array));
    array->index = 0;
    array->size = INIT_ARRAY_SIZE;
    array->elements = malloc(array->size * sizeof (Entry *));

    return array;
}

int add_element(Array *array, const char *string)
{
    if (needs_doubling(array) && !grow_array(array))
        return 0;

    array->elements[array->index++] = new_entry(string);

    return array->index;
}
