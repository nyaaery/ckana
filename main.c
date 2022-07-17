#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h>

#include <curses.h>

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

typedef struct {
    char* kana;
    char* romanji;
} RandKanaVal;

bool result_is_ok(Result* res) {
    bool is_ok = res->var == ResultOk;
    return is_ok;
}

RandKanaVal randkanastr(int len) {
    int kanasizetotal = 0;
    int romanjisizetotal = 0;
    int* indices = calloc(len, sizeof(int));
    for (int i = 0; i < len; i++) {
        int j = rand() % DIM(hiragana);
        int kanasize = strlen(hiragana[j][0]);
        int romanjisize = strlen(hiragana[j][1]);
        kanasizetotal += kanasize;
        romanjisizetotal += romanjisize;
        indices[i] = j;
    }
    // + 1 for null terminator
    char* kanastr = calloc(kanasizetotal + 1, sizeof(char));
    char* romanjistr = calloc(romanjisizetotal + 1, sizeof(char));
    for (int i = 0; i < len; i++) {
        int j = indices[i];
        const char* kana = hiragana[j][0];
        const char* romanji = hiragana[j][1];
        if (i == 0) {
            strcpy(kanastr, kana);
            strcpy(romanjistr, romanji);
        } else {
            strcat(kanastr, kana);
            strcat(romanjistr, romanji);
        }
    }
    RandKanaVal val = {
        .kana = kanastr,
        .romanji = romanjistr
    };
    return val;
}

int main() {
    setlocale(LC_ALL, "");
    srand(time(0));

    initscr();
    noecho();
    refresh();
    
    int h = 6;
    int w = 32;
    int y = (LINES - h) / 2;
    int x = (COLS - w) / 2;

    WINDOW* win = newwin(h, w, y, x);
    box(win, 0, 0);
    wrefresh(win);

    RandKanaVal randkanaval = randkanastr(5);

    mvwprintw(win, 1, 1, randkanaval.kana);
    mvwprintw(win, 3, 1, "write in romanji:");
    wmove(win, 4, 1);
    wrefresh(win);
    
    char in[31];
    for (int i = 0; i < 30; i++) {
        int ch = getch();

        if (ch == '\n' || ch == '\r' || ch == EOF) {
            break;
        }

        in[i] = (char) ch;
        mvwaddch(win, 4, i + 1, (char) ch);
        wrefresh(win);
    }

    wclear(win);
    box(win, 0, 0);
    int res = strcmp(randkanaval.romanji, &in);
    if (res == 0) {
        mvwprintw(win, 1, 1, "CORRECT!");
    } else {
        mvwprintw(win, 1, 1, "INCORRECT");
    }
    mvwprintw(win, 2, 1, randkanaval.kana);
    mvwprintw(win, 3, 1, randkanaval.romanji);
    mvwprintw(win, 4, 1, &in);
    wrefresh(win);

    getch();

    endwin();
}
