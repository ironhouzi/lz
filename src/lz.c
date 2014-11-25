#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include "lz.h"

Entry* new_entry(char* string)
{
    int length = strlen(string) + 1;
    Entry* entry = malloc(sizeof(Entry));
    entry->string = malloc(length);
    strncpy(entry->string, string, length);
    entry->rank = INT_MAX;
    return entry;
}

void del_entry(Entry * entry)
{
    free(entry->string);
    free(entry);
}

void free_all(Entry** all_entries, int entry_count)
{
    for (int i = 0; i < entry_count; i++)
        del_entry(all_entries[i]);

    free(all_entries);
}

void entry_from_stdin(Entry** entries, int* entry_count)
{
    char buf[1024];
    int len;

    while(NULL != fgets(buf, 1024, stdin)) {
        len = strlen(buf) - 1;

        if (buf[len] == '\n')
            buf[len] = '\0';

        entries[*entry_count] = new_entry(buf);
        *entry_count += 1;
    }
}

int min(const int x, const int y, const int z)
{
    int result = x;
    result = (x < y) ? x : y;
    result = (result < z) ? result : z;
    /* printf("previous + 1: %d, above + 1: %d, diag + cost: %d -> %d\n", x, y, z, result); */
    /* printf("%d\n", result); */
    return result;
}

int ldist(const Entry* str_s, const Entry* str_t)
{
    int len = 0;
    int s_len = strlen(str_s->string);
    int t_len = strlen(str_t->string);

    if (s_len == t_len)
        len = s_len;
    else if (s_len < t_len)
        len = s_len;
    else
        len = t_len;

    if (strcmp(str_s->string, str_t->string) == 0)
        return 0;

    if (s_len == 0)
        return t_len;

    if (t_len == 0)
        return s_len;

    int line_above[t_len + 1];
    int current_line[t_len + 1];
    int i, j, cost;
    char chr_s;
    char chr_t;

    /* current_line[t_len] = 0; */
    for (i = 0; i < t_len + 1; i++) {
        line_above[i] = i;
        /* current_line[i] = 0; */
    }

    for (i = 0; i < s_len; i++) {
        current_line[0] = i + 1;

        for (j = 0; j < t_len; j++) {
            chr_s = str_s->string[i];
            chr_t = str_t->string[j];
            cost = (strncmp(&chr_s, &chr_t, sizeof(char)) == 0) ? 0 : 1;
            /* printf("%d\t%c == %c, cost: %d, ==> ", j, chr_s, chr_t, cost); */
            current_line[j+1] = min(current_line[j] + 1,
                              line_above[j+1] + 1,
                              line_above[j] + cost);
        }
        /* printf("\n"); */

        for (j = 0; j < t_len + 1; j++)
            line_above[j] = current_line[j];
    }

    return current_line[t_len];
}

int entry_cmp(const void *a, const void *b)
{
    const Entry *ia = *(const Entry **)a;
    const Entry *ib = *(const Entry **)b;
    /* printf("%d - %d = %d\n", ia->rank, ib->rank, ia->rank - ib->rank); */
    return ia->rank - ib->rank;
}

int main()
{
    Entry** entries = malloc(2000 * sizeof(Entry *));
    int i;
    int entry_count = 0;
    char *str;
    /* int entry_count = 2; */
    /* entries[0] = new_entry("doapp"); */
    /* entries[1] = new_entry("that some of its noisiest aut"); */
    /* Entry* i1 = new_entry("bob"); */
    /* Entry* i2 = new_entry("bomy"); */
    /* Entry* i1 = new_entry("Saturday"); */
    /* Entry* i2 = new_entry("Sunday"); */
    /* Entry* i1 = new_entry("kitten"); */
    /* Entry* i2 = new_entry("sitting"); */
    /* Entry* input = new_entry("obligdenoisergbp"); */
    Entry* input = new_entry("lzc");
    printf("input: %s\n", input->string);

    entry_from_stdin(entries, &entry_count);

    /* printf("input: %s\n", entries[1]->string); */
    /* printf("dist: %d\n", ldist(entries[0], entries[1])); */
    for (i = 0; i < entry_count; i++) {
        str = entries[i]->string;
        int rank = ldist(input, entries[i]);
        /* printf("%s - %d\n", str, rank); */
        entries[i]->rank = rank;
    }

    qsort(entries, entry_count, sizeof(Entry *), entry_cmp);

    /* printf("\n"); */
    for (i = 0; i < entry_count; i++)
        printf("%d\t%s - %d\n", i, entries[i]->string, entries[i]->rank);

    free_all(entries, entry_count);
    del_entry(input);
    return 0;
}
