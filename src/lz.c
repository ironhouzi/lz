#include <stdlib.h>
#include "edist.h"
#include <ncurses.h>

#define MAXBUF 21

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
