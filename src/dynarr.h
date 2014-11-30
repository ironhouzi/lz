#ifndef INCLUDE_DYNARR_HEADER
#define INCLUDE_DYNARR_HEADER

#include "edist.h"

typedef struct _strarray {
    size_t size;
    size_t index;
    Entry **elements;
} Array;

Array *new_array(void);
Entry* new_entry(const char* string);
int add_element(Array *array, const char *string);
void free_array(Array *array);

#endif
