#include <string.h>
#include <stdbool.h>

#include <curses.h>
#include <form.h>

#include "../lib/ckana_lib.h"
#include "../lib/MaxLens.h"
#include "../lib/RandStr.h"
#include "hiragana.h"

#include "hiragana_kana_problem.h"

void hiragana_kana_problem_win_free(HiraganaKanaProblemWin* self) {
    free_form(self->form);
    free_field(self->field);

    delwin(self->sub_win);
    wclear(self->win);
    wrefresh(self->win);
    delwin(self->win);
}

void hiragana_kana_problem(bool* quit, MaxLens* lens) {
    int char_count = 5;

    RandStr randstr = hiragana_rand_str(lens, char_count);

    int h = 9;
    int w = 48;
    WINDOW* problem_win = new_win_center(h, w);

    int in_len = lens->romanji_len * char_count;
    FIELD* field = new_field(1, in_len, 0, 0, 0, 0);
    set_field_back(field, A_UNDERLINE);
    field_opts_off(field, O_AUTOSKIP);

    FIELD* fields[2] = {
        field,
        NULL
    };

    FORM* form = new_form(fields);

    int subh;
    int subw;
    scale_form(form, &subh, &subw);

    WINDOW* sub_win = derwin(problem_win, subh, subw, 6, 4);
    set_form_win(form, problem_win);
    set_form_sub(form, sub_win);
    post_form(form);

    HiraganaKanaProblemWin kana_problem_win = {
        .win = problem_win,
        .field = field,
        .form = form,
        .sub_win = sub_win
    };

    mvwprintw(problem_win, 2, 4, "write in romanji");
    mvwprintw(problem_win, 4, 4, "%s", randstr.kanastr);
    wmove(problem_win, 6, 4);
    wrefresh(problem_win);
    wrefresh(sub_win);
  
    bool done = false;
    while (!done) {
        int ch = getch();

        switch (ch) {
            case 'q':
                *quit = true;
                unpost_form(form);
                hiragana_kana_problem_win_free(&kana_problem_win);
                randstr_free(&randstr);
                return;
            case '\n':
            case '\r':
            case EOF:
                form_driver(form, REQ_NEXT_FIELD);
                done = true;
                break;
            case KEY_BACKSPACE:
                form_driver(form, REQ_DEL_PREV);
                break;
            default:
                form_driver(form, ch);
                break;
        }

        wrefresh(sub_win);
    }

    unpost_form(form);

    //             + 1 for null terminator.
    char in[in_len + 1];
    memset(&in, 0, in_len + 1);

    char* buf = field_buffer(field, 0);
    strcpy(in, buf);
    trim_end_in_place(in);

    hiragana_kana_problem_win_free(&kana_problem_win);

    int res = strcmp(randstr.romanjistr, in);

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
        mvwprintw(result_win, 4, 4, "%s", randstr.kanastr);
        mvwprintw(result_win, 6, 4, "%s", in);
    } else {
        init_pair(1, COLOR_RED, COLOR_BLACK);
        wattron(result_win, COLOR_PAIR(1) | A_BOLD);
        mvwprintw(result_win, 2, 4, "incorrect");
        wstandend(result_win);
        mvwprintw(result_win, 4, 4, "%s", randstr.kanastr);
      
        mvwprintw(result_win, 6, 4, "%s", in);
        mvwprintw(result_win, 6, w - 9 - 4, "you wrote");
        mvwprintw(result_win, 7, 4, "%s", randstr.romanjistr);
        mvwprintw(result_win, 7, w - 14 - 4, "correct answer");
      
        int len = strlen(in);
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
    randstr_free(&randstr);
}
