#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <curses.h>
#include <menu.h>

#include "../lib/ckana_def.h"
#include "../lib/ckana_lib.h"
#include "hiragana.h"

#include "hiragana_romanji_problem.h"

void hiragana_romanji_problem_win_free(HiraganaRomanjiProblemWin* self) {
    free_menu(self->menu);
    for (int i = 0; self->items[i] != NULL; i++) {
        free_item(self->items[i]);
    }

    delwin(self->sub_win);
    wclear(self->win);
    wrefresh(self->win);
    delwin(self->win);
}

void hiragana_romanji_problem(bool* quit) {
    int option_count = 15;

    PairPtr problempair = hiragana_rand_pair();

    int h = 11;
    int w = 48;
    WINDOW* problem_win = new_win_center(h, w);

    int correct_i = rand() % option_count;

    //                       + 1 for null terminator.
    ITEM* items[option_count + 1];
    items[option_count] = NULL;
    PairPtr item_pairs[option_count];

    for (int i = 0; i < option_count; i++) {
        if (i == correct_i) {
            items[i] = new_item(problempair[0], 0);
            item_pairs[i] = problempair;
        } else {
            PairPtr randpair = hiragana_rand_pair();
            items[i] = new_item(randpair[0], 0);
            item_pairs[i] = randpair;
        }
    }

    MENU* menu = new_menu(items);
    set_menu_format(menu, 3, 5);
    set_menu_mark(menu, " ");
    menu_opts_off(menu, O_SHOWDESC);

    int subh;
    int subw;
    scale_menu(menu, &subh, &subw);

    WINDOW* sub_win = derwin(problem_win, subh, subw, 6, 4);
    set_menu_win(menu, problem_win);
    set_menu_sub(menu, sub_win);
    post_menu(menu);

    HiraganaRomanjiProblemWin romanji_problem_win = {
        .win = problem_win,
        .items = items,
        .menu = menu,
        .sub_win = sub_win
    };

    mvwprintw(problem_win, 2, 4, "select in hiragana");
    mvwprintw(problem_win, 4, 4, "%s", problempair[1]);
    wrefresh(problem_win);

    keypad(problem_win, TRUE);

    bool done = false;
    while (!done) {
        int ch = wgetch(problem_win);

        switch (ch) {
            case 'q':
                *quit = true;
                unpost_menu(menu);
                hiragana_romanji_problem_win_free(&romanji_problem_win);
                return;
            case '\n':
            case '\r':
            case EOF:
                done = true;
                break;
            case 'j':
            case KEY_DOWN:
                menu_driver(menu, REQ_DOWN_ITEM);
                break;
            case 'k':
            case KEY_UP:
                menu_driver(menu, REQ_UP_ITEM);
                break;
            case 'l':
            case KEY_RIGHT:
                menu_driver(menu, REQ_RIGHT_ITEM);
                break;
            case 'h':
            case KEY_LEFT:
                menu_driver(menu, REQ_LEFT_ITEM);
                break;
        }

        wrefresh(problem_win);
    }

    unpost_menu(menu);

    int selected_i = item_index(current_item(menu));
    PairPtr selected_pair = item_pairs[selected_i];

    hiragana_romanji_problem_win_free(&romanji_problem_win);

    int res = strcmp(problempair[1], selected_pair[1]);

    if (res == 0) {
        h = 9;
    } else {
        h = 10;
    }
    w = w;

    WINDOW* result_win = new_win_center(h, w);

    if (res == 0) {
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        wattron(result_win, COLOR_PAIR(1) | A_BOLD);
        mvwprintw(result_win, 2, 4, "CORRECT!");
        wstandend(result_win);
        mvwprintw(result_win, 4, 4, "%s", problempair[1]);
        mvwprintw(result_win, 6, 4, "%s", selected_pair[0]);
    } else {
        init_pair(1, COLOR_RED, COLOR_BLACK);
        wattron(result_win, COLOR_PAIR(1) | A_BOLD);
        mvwprintw(result_win, 2, 4, "incorrect");
        wstandend(result_win);
        mvwprintw(result_win, 4, 4, "%s", problempair[1]);

        mvwprintw(result_win, 6, 4, "%s", selected_pair[0]);
        mvwprintw(result_win, 6, w - 12 - 4, "you selected");
        mvwprintw(result_win, 7, 4, "%s", problempair[0]);
        mvwprintw(result_win, 7, w - 14 - 4, "correct answer");

        int len = strlen(selected_pair[0]);
        wmove(result_win, 6, 4 + len);
    }
    wrefresh(result_win);

    int ch = getch();
    if (ch == 'q') {
        *quit = true;
    }

    wclear(result_win);
    wrefresh(result_win);
    delwin(result_win);
}
