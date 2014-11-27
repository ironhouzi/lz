#include "edist.h"


typedef struct _strarray {
    size_t size;
    size_t index;
    Entry **elements;
} Array;

int add_element(Array* array, Entry* entry);
void free_elements(Array* array);
