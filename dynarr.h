typedef struct _strarray {
    size_t size;
    size_t index;
    char **head;
} Array;

int add_element(Array* array, char* strptr);
void free_elements(Array* array);
