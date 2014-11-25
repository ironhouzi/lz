typedef struct _entry {
    int rank;
    int length;
    char *string;
} Entry;

Entry* new_entry(const char* string);
