#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h>
#include <ctype.h>

#include <curses.h>
#include <form.h>

#define DIM(arr) (sizeof(arr) / sizeof(arr[0]))

const char* hiragana[][2] = {
    { u8"ん", "n" },
    { u8"あ", "a" },  { u8"い", "i" },   { u8"う", "u" },   { u8"え", "e" },  { u8"お", "o" },
    { u8"か", "ka" }, { u8"き", "ki" },  { u8"く", "ku" },  { u8"け", "ke" }, { u8"こ", "ko" },
    { u8"さ", "sa" }, { u8"し", "shi" }, { u8"す", "su" },  { u8"せ", "se" }, { u8"そ", "so" },
    { u8"た", "ta" }, { u8"ち", "chi" }, { u8"つ", "tsu" }, { u8"て", "te" }, { u8"と", "to" },
    { u8"な", "na" }, { u8"に", "ni" },  { u8"ぬ", "nu" },  { u8"ね", "ne" }, { u8"の", "no" },
    { u8"は", "ha" }, { u8"ひ", "hi" },  { u8"ふ", "fu" },  { u8"へ", "he" }, { u8"ほ", "ho" },
    { u8"ま", "ma" }, { u8"み", "mi" },  { u8"む", "mu" },  { u8"め", "me" }, { u8"も", "mo" },
    { u8"や", "ya" },                    { u8"ゆ", "yu" },                    { u8"よ", "yo" },
    { u8"ら", "ra" }, { u8"り", "ri" },  { u8"る", "ru" },  { u8"れ", "re" }, { u8"ろ", "ro" },
    { u8"わ", "wa" },                                                         { u8"を", "wo" }
};

typedef enum {
    ResultErr,
    ResultOk
} ResultVariant;

typedef struct {
    ResultVariant var;
    void* val;
} Result;

bool result_is_ok(Result* res) {
    bool is_ok = res->var == ResultOk;
    return is_ok;
}

typedef struct {
    int kana_len;
    int romanji_len;
} MaxCharLensVal;

MaxCharLensVal linear_search_max_char_lens(const char* kana[][2], int len) {
    int max_kana_len = 0;
    int max_romanji_len = 0;
    
    // Linear search. Find maximums.
    for (int i = 0; i < len; i++) {
        const char* kanachar = kana[i][0];
        int kana_len = strlen(kanachar);
        if (kana_len > max_kana_len) {
            max_kana_len = kana_len;
        }

        const char* romanjichar = kana[i][1];
        int romanji_len = strlen(romanjichar);
        if (romanji_len > max_romanji_len) {
            max_romanji_len = romanji_len;
        }
    }

    return (MaxCharLensVal) {
        .kana_len = max_kana_len,
        .romanji_len = max_romanji_len
    };
}

typedef struct {
    char* kanastr;
    char* romanjistr;
} RandKanaStrVal;

RandKanaStrVal rand_kana_str(const char* kana[][2], int kana_len, MaxCharLensVal* lens, int str_len) {
    //                                              + 1 for null terminator.
    char* kanastr = calloc(lens->kana_len * str_len + 1, sizeof(char));
    char* romanjistr = calloc(lens->romanji_len * str_len + 1, sizeof(char));

    for (int i = 0; i < str_len; i++) {
        int j = rand() % kana_len;

        const char* kanachar = kana[j][0];
        const char* romanjichar = kana[j][1];
        
        if (i == 0) {
            strcpy(kanastr, kanachar);
            strcpy(romanjistr, romanjichar);
        } else {
            strcat(kanastr, kanachar);
            strcat(romanjistr, romanjichar);
        }
    }

    return (RandKanaStrVal) {
        .kanastr = kanastr,
        .romanjistr = romanjistr
    };
}

void rand_kana_str_val_free(RandKanaStrVal* self) {
    free(self->kanastr);
    free(self->romanjistr);
}

void trim_end_in_place(char* str) {
    int len = strlen(str);
    int i = len - 1;
    //          ^^^ - 1 to skip null terminator.
    while (isspace(str[i])) {
        str[i--] = 0;
    }
}

void kana_problem(MaxCharLensVal* hiragana_lens) {
    int char_count = 5;

    RandKanaStrVal randstr = rand_kana_str(hiragana, DIM(hiragana), hiragana_lens, char_count);

    int h = 9;
    int w = 48;
    int y = (LINES - h) / 2;
    int x = (COLS - w) / 2;

    WINDOW* problem_win = newwin(h, w, y, x);
    box(problem_win, 0, 0);

    mvwprintw(problem_win, 2, 4, "write in romanji");
    mvwprintw(problem_win, 4, 4, randstr.kanastr);
    wmove(problem_win, 6, 4);
    wrefresh(problem_win);

    int in_len = hiragana_lens->romanji_len * char_count;
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

    WINDOW* sub_win = newwin(subh, subw, y + 6, x + 4);
    set_form_win(form, problem_win);
    set_form_sub(form, sub_win);
    post_form(form);

    wrefresh(sub_win);
    
    bool done = false;
    while (!done) {
        int ch = getch();

        switch (ch) {
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

    free_field(field);
    free_form(form);
    delwin(sub_win);

    delwin(problem_win);

    int res = strcmp(randstr.romanjistr, in);

    if (res == 0) {
        h = 9;
    } else {
        h = 10;
    }
    w = w;
    y = (LINES - h) / 2;
    x = (COLS - w) / 2;

    WINDOW* result_win = newwin(h, w, y, x);
    box(result_win, 0, 0);

    if (res == 0) {
        mvwprintw(result_win, 2, 4, "CORRECT!");
        mvwprintw(result_win, 4, 4, randstr.kanastr);
        mvwprintw(result_win, 6, 4, in);
    } else {
        mvwprintw(result_win, 2, 4, "incorrect");
        mvwprintw(result_win, 4, 4, randstr.kanastr);
        
        mvwprintw(result_win, 6, 4, in);
        mvwprintw(result_win, 6, w - 9 - 4, "you wrote");
        mvwprintw(result_win, 7, 4, randstr.romanjistr);
        mvwprintw(result_win, 7, w - 14 - 4, "correct answer");
        
        int len = strlen(in);
        wmove(result_win, 6, 4 + len);
    }

    wrefresh(result_win);

    getch();

    delwin(result_win);

    // wclear(problem_win);
    // box(problem_win, 0, 0);
    // int res = strcmp(randstr.romanjistr, &in);
    // if (res == 0) {
    //     mvwprintw(problem_win, 1, 1, "CORRECT!");
    // } else {
    //     mvwprintw(problem_win, 1, 1, "INCORRECT");
    // }
    // mvwprintw(problem_win, 2, 1, randstr.kanastr);
    // mvwprintw(problem_win, 3, 1, randstr.romanjistr);
    // mvwprintw(problem_win, 4, 1, &in);
    // wrefresh(problem_win);
}

int main() {
    setlocale(LC_ALL, "");
    srand(time(0));

    MaxCharLensVal hiragana_lens = linear_search_max_char_lens(hiragana, DIM(hiragana));

    initscr();
    keypad(stdscr, TRUE);
    noecho();

    refresh();

    kana_problem(&hiragana_lens);

    endwin();

    return 0;
}
