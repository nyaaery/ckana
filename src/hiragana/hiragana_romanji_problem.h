#pragma once

#include <stdbool.h>

#include <curses.h>
#include <menu.h>

typedef struct {
    WINDOW* win;
    ITEM** items;
    MENU* menu;
    WINDOW* sub_win;
} HiraganaRomanjiProblemWin;

void hiragana_romanji_problem_win_free(HiraganaRomanjiProblemWin* self);

void hiragana_romanji_problem(bool* quit);
