#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include "edist.h"

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

int calculate_penalty(const int *matches, const int *previous_matches,
                      const int len_s, const int len_t, const int penalty, int *j)
{
    // TODO: maintain skip count accross all calculations
    int skip = 1;
    int found = 0;
    /* printf("matches %d\n", matches[*j]); */

    for (int k = *j+1; k < len_t || previous_matches[k]; k++) {
        if (matches[k]) {
            found++;
            break;
        }
        skip++;
    }

    *j += skip;

    if (!found)
        return penalty + (skip * len_t * len_t);

    int diff = (len_s + skip);
    // Use power function to exagerate differences in skip distances
    diff = diff * diff;
    /* int new = penalty + (diff - (len_t / found)); */
    int new = penalty + (diff - (found * len_t));
    /* printf("j=%d\told: %d, new: %d\n", *j, penalty, new); */

    return new;
}

int edit_distance(const char* str_s, const Entry* str_t)
{
    int len_s = strlen(str_s);
    if (len_s == 0)
        return str_t->length;

    if (str_t->length == 0)
        return len_s;

    int line_above[str_t->length + 1];
    int current_line[str_t->length + 1];
    int matches[str_t->length + 1];
    int previous_matches[str_t->length + 1];
    /* int *matches = malloc(str_t->length * sizeof(int)); */
    /* int *previous_matches = malloc(str_t->length * sizeof(int)); */
    int i, j, cost;
    char chr_s;
    char chr_t;
    int penalty = 0;

    for (i = 0; i < str_t->length + 1; i++) {
        matches[i] = 0;
        line_above[i] = i;
        *(previous_matches + i) = 0;
    }

    for (i = 0; i < len_s; i++) {
        current_line[0] = i + 1;

        for (j = 0; j < str_t->length; j++) {
            chr_s = str_s[i];
            chr_t = str_t->string[j];
            cost = (strncmp(&chr_s, &chr_t, sizeof(char)) == 0) ? 0 : 1;
            matches[j] = !cost;
            current_line[j+1] = min(current_line[j] + 1,
                                    line_above[j+1] + 1,
                                    line_above[j] + cost);
        }

        for (j = 0; j < str_t->length + 1; j++)
            if (previous_matches[j])
                penalty = calculate_penalty(matches,
                                            previous_matches,
                                            len_s,
                                            str_t->length,
                                            penalty,
                                            &j);

        for (int j = 0; j < str_t->length; j++) {
            line_above[j] = current_line[j];
            previous_matches[j] = matches[j];
        }

        /* free(matches); */
        /* free(previous_matches); */
    }

    return current_line[str_t->length] + penalty;
}

int entry_cmp(const void *a, const void *b)
{
    const Entry *ia = *(const Entry **)a;
    const Entry *ib = *(const Entry **)b;
    return ia->rank - ib->rank;
}


