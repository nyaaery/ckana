#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <curses.h>

#include "ckana_lib.h"

float randf() {
    return ((float) rand()) / ((float) RAND_MAX);
}

void trim_end_in_place(char* str) {
    int len = strlen(str);
    int i = len - 1;
    //          ^^^ - 1 to skip null terminator.
    while (isspace(str[i])) {
        str[i--] = 0;
    }
}

// Does not call wrefresh.
WINDOW* new_win_center(int h, int w) {
    int y = (LINES - h) / 2;
    int x = (COLS - w) / 2;

    WINDOW* win = newwin(h, w, y, x);
    box(win, 0, 0);

    return win;
}
