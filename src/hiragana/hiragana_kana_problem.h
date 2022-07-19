#pragma once

#include <stdbool.h>

#include "../lib/MaxLens.h"

#include <curses.h>
#include <form.h>

typedef struct {
    WINDOW* win;
    FIELD* field;
    FORM* form;
    WINDOW* sub_win;
} HiraganaKanaProblemWin;

void hiragana_kana_problem_win_free(HiraganaKanaProblemWin* self);

void hiragana_kana_problem(bool* quit, MaxLens* lens);
