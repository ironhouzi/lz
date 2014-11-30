#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include "edist.h"
#include "dynarr.h"

#define MAXBUF 21

int entry_from_stdin(Array* array)
{
    char buf[1024];
    int len;

    // TODO: Error check if buffer is exceeded
    while(NULL != fgets(buf, 1024, stdin)) {
        len = strlen(buf) - 1;

        if ('\n' == buf[len])
            buf[len] = '\0';

        if (!add_element(array, buf))
            return 0;
    }

    return 1;
}

void query(const char *input, Array  *array)
{
    char *str;

    for (size_t i = 0; i < array->index; i++) {
        str = array->elements[i]->string;
        int rank = edit_distance(input, array->elements[i]);
        array->elements[i]->rank = rank;
    }

    qsort(array->elements, array->index, sizeof (Entry *), entry_cmp);
}

void print_entries(const Array *array, const size_t row_count)
{
    int limit = (array->index < row_count - 2) ? array->index : row_count - 2;

    for (int i = 0; i < limit; i++) {
        move(i, 0);
        wprintw(stdscr, "%s", array->elements[i]->string);
    }
}

int main()
{
    Array *array = new_array();
    char buf[MAXBUF] = {0};
    size_t row_count, col_count;
    int pos = 0;
    int delete = FALSE;

    if (!entry_from_stdin(array))
        return 1;

    freopen("/dev/tty", "rw", stdin);
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    getmaxyx(stdscr, row_count, col_count);
    print_entries(array, row_count);

    move(row_count - 1, 0);
    refresh();

    while (TRUE) {
        if (ERR == (buf[pos] = getch())) {
            move(row_count - 1, 0);
            wprintw(stdscr, "ERROR");
            getch();
            break;
        }

        if ('\n' == buf[pos]) {
            buf[pos] = 0;
            break;
        }

        else if (127 == buf[pos]) {
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
        query(buf, array);
        print_entries(array, row_count);

        move(row_count - 1, 0);
        wprintw(stdscr, "%s", buf);

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

    free_array(array);
    return 0;
}
