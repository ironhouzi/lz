typedef struct _entry {
    int rank;
    int length;
    char *string;
} Entry;

Entry* new_entry(const char* string);
int entry_cmp(const void *a, const void *b);
int edit_distance(const char* str_s, const Entry* str_t);
void free_all(Entry** all_entries, const int entry_count);
