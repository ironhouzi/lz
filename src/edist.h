#ifndef INCLUDE_EDIST_HEADER
#define INCLUDE_EDIST_HEADER

typedef struct _entry {
    int rank;
    int length;
    char *string;
} Entry;

int entry_cmp(const void *a, const void *b);
int edit_distance(const char* str_s, const Entry* str_t);
void free_all(Entry** all_entries, const int entry_count);

#endif
