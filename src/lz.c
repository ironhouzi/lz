#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include "lz.h"

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

void print_i(const int i, const char c)
{
    if (i == 0)
        printf("      ");

    printf("%c", c);

    if (i < 8)
        printf("  ");
    else if (i == 8)
        printf("   ");
    else if (i == 9)
        printf("   ");
    else
        printf("  ");
}

void print_j(const int j, const int val)
{
    printf("%d", val);
    if (j < 10)
        printf("  ");
    else
        printf(" ");
}

int first_match(int *list, int length)
{
    for (int i = 0; i < length; i++)
        if (list[i])
            return i;
    return -1;
}

int calculate_penalty(int j, int *matches, int *previous_matches, int length)
{
    int skip = 1;
    int found = 0;
    int penalty = 0;
    printf("matches %d\n", matches[j]);

    for (int k = j+1; k < length || previous_matches[k]; k++) {
        if (matches[k]) {
            found++;
            break;
        }
        skip++;
    }

    j += skip;

    if (!found)
        penalty += skip * length * 2;
    else
        penalty += skip - (found * length / 4);

    return penalty;
}

int edit_distance(const Entry* str_s, const Entry* str_t)
{
    /* if (strcmp(str_s->string, str_t->string) == 0) */
    /*     return 0; */

    if (str_s->length == 0)
        return str_t->length;

    if (str_t->length == 0)
        return str_s->length;

    int line_above[str_t->length + 1];
    int current_line[str_t->length + 1];
    int matches[str_t->length];
    /* int *tmp = matches; */
    int previous_matches[str_t->length];
    /* int *tmpp = previous_matches; */
    int i, j, cost;
    char chr_s;
    char chr_t;
    int penalty = 0;

    for (i = 0; i < str_t->length + 1; i++) {
        line_above[i] = i;
        previous_matches[i] = 0;
        print_i(i, str_t->string[i]);
    }
    printf("\n   ");

    for (i = 0; i < str_t->length + 1; i++) {
        print_j(i, line_above[i]);
        matches[i] = 0;
        if (i == 10)
            printf(" ");
    }

    printf("\n");
    for (i = 0; i < str_s->length; i++) {
        printf("%c  %d  ", str_s->string[i], i + 1);
        current_line[0] = i + 1;

        for (j = 0; j < str_t->length; j++) {
            chr_s = str_s->string[i];
            chr_t = str_t->string[j];
            cost = (strncmp(&chr_s, &chr_t, sizeof(char)) == 0) ? 0 : 1;
            /* printf("%d\t%c == %c, cost: %d, ==> ", j, chr_s, chr_t, cost); */
            matches[j] = !cost;
            current_line[j+1] = min(current_line[j] + 1,
                                    line_above[j+1] + 1,
                                    line_above[j] + cost);
            print_j(j, current_line[j+1]);
        }
        printf("\n");

        for (j = 0; j < str_t->length + 1; j++) {
            line_above[j] = current_line[j];

            if (j == str_t->length)
                continue;

            if (previous_matches[j])
                penalty = calculate_penalty(j,
                                            matches,
                                            previous_matches,
                                            str_t->length);
        }

        for (int j = 0; j < str_t->length; j++)
            previous_matches[j] = matches[j];

    }

    return current_line[str_t->length] + penalty;
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
    Entry* input = new_entry("lzh");
    printf("input: %s\n", input->string);

    entry_from_stdin(entries, &entry_count);

    /* printf("input: %s\n", entries[1]->string); */
    /* printf("dist: %d\n", edit_distance(entries[0], entries[1])); */
    for (i = 0; i < entry_count; i++) {
        str = entries[i]->string;
        int rank = edit_distance(input, entries[i]);
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
