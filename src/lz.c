#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include "lz.h"
#include <ncurses.h>

#define MAXBUF 21

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
    /* if (strcmp(str_s->string, str_t->string) == 0) */
    /*     return 0; */

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
        /* print_i(i, str_t->string[i]); */
    }
    /* printf("\n   "); */

    /* for (i = 0; i < str_t->length + 1; i++) { */
    /*     print_j(i, line_above[i]); */
    /*     if (i == 10) */
    /*         printf(" "); */
    /* } */

    /* printf("\n"); */
    for (i = 0; i < len_s; i++) {
        /* printf("%c  %d  ", str_s[i], i + 1); */
        current_line[0] = i + 1;

        for (j = 0; j < str_t->length; j++) {
            chr_s = str_s[i];
            chr_t = str_t->string[j];
            cost = (strncmp(&chr_s, &chr_t, sizeof(char)) == 0) ? 0 : 1;
            /* printf("%d\t%c == %c, cost: %d, ==> ", j, chr_s, chr_t, cost); */
            matches[j] = !cost;
            current_line[j+1] = min(current_line[j] + 1,
                                    line_above[j+1] + 1,
                                    line_above[j] + cost);
            /* print_j(j, current_line[j+1]); */
        }
        /* printf("\n"); */

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

void query(const int entry_count, const char *s, Entry **entries)
{
    char *str;

    for (int i = 0; i < entry_count; i++) {
        str = entries[i]->string;
        int rank = edit_distance(s, entries[i]);
        entries[i]->rank = rank;
    }

    qsort(entries, entry_count, sizeof(Entry *), entry_cmp);
}

int main()
{
    Entry** entries = malloc(2000 * sizeof(Entry *));
    int i;
    int entry_count = 0;
    char buf[MAXBUF] = {0};
    int row_count, col_count;
    int pos = 0;
    int delete = FALSE;

    entry_from_stdin(entries, &entry_count);

    freopen("/dev/tty", "rw", stdin);
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    getmaxyx(stdscr, row_count, col_count);
    int limit = (entry_count < row_count - 2) ? entry_count : row_count - 2;

    for (i = 0; i < limit; i++) {
        move(i, 0);
        wprintw(stdscr, entries[i]->string);
    }

    move(row_count - 1, 0);
    refresh();

    while (TRUE) {
        if ((buf[pos] = getch()) == ERR) {
            move(row_count - 1, 0);
            wprintw(stdscr, "ERROR");
            getch();
            break;
        }

        if (buf[pos] == '\n') {
            buf[pos] = 0;
            break;
        }

        else if (buf[pos] == 127) {
            buf[pos--] = 0;
            delete = TRUE;
        }

        else if (pos >= MAXBUF - 1)
            continue;

        else {
            move(row_count - 1, pos);
            waddch(stdscr, buf[pos++]);
        }

        erase();
        query(entry_count, buf, entries);
        limit = (entry_count < row_count - 2) ? entry_count : row_count - 2;

        for (i = 0; i < limit; i++) {
            move(i, 0);
            wprintw(stdscr, entries[i]->string);
        }

        move(row_count - 1, 0);
        wprintw(stdscr, buf);

        if (delete) {
            move(row_count - 1, pos);
            delch();
            delete = FALSE;
        }

        refresh();
    }

    keypad(stdscr, FALSE);
    endwin();
    printf("%s\n", buf);

    free_all(entries, entry_count);
    return 0;
}
