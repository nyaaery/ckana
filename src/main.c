#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>

#include <curses.h>

#include "lib/MaxLens.h"
#include "hiragana/hiragana.h"
#include "hiragana/hiragana_kana_problem.h"
#include "hiragana/hiragana_romanji_problem.h"

int main() {
    setlocale(LC_ALL, "");
    srand(time(0));

    MaxLens hiragana_lens = hiragana_max_lens();

    initscr();

    // Assert that terminal supports color.
    assert(has_colors() == TRUE);

    start_color();
    keypad(stdscr, TRUE);
    noecho();
    
    printw("q to quit");
    refresh();
    
    bool quit = false;
    while (!quit) {
        hiragana_kana_problem(&quit, &hiragana_lens);
        if (quit) {
            break;
        }
        hiragana_romanji_problem(&quit);
    }

    endwin();

    return 0;
}
