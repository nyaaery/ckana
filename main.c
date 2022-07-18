#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h>
#include <ctype.h>
#include <assert.h>

#include <curses.h>
#include <form.h>

#define DIM(arr) (sizeof(arr) / sizeof(arr[0]))

#define DIACRITIC_FREQ 0.3
#define DIAGRAPH_FREQ 0.1
#define SOKUON_FREQ 0.05

typedef const char const* Pair[2];
typedef const char const* const* PairPtr;

const Pair hiragana_monographs[] = {
    { u8"あ", "a" },  { u8"い", "i" },   { u8"う", "u" },   { u8"え", "e" },  { u8"お", "o" },
    { u8"か", "ka" }, { u8"き", "ki" },  { u8"く", "ku" },  { u8"け", "ke" }, { u8"こ", "ko" },
    { u8"さ", "sa" }, { u8"し", "shi" }, { u8"す", "su" },  { u8"せ", "se" }, { u8"そ", "so" },
    { u8"た", "ta" }, { u8"ち", "chi" }, { u8"つ", "tsu" }, { u8"て", "te" }, { u8"と", "to" },
    { u8"な", "na" }, { u8"に", "ni" },  { u8"ぬ", "nu" },  { u8"ね", "ne" }, { u8"の", "no" },
    { u8"は", "ha" }, { u8"ひ", "hi" },  { u8"ふ", "fu" },  { u8"へ", "he" }, { u8"ほ", "ho" },
    { u8"ま", "ma" }, { u8"み", "mi" },  { u8"む", "mu" },  { u8"め", "me" }, { u8"も", "mo" },
    { u8"や", "ya" },                    { u8"ゆ", "yu" },                    { u8"よ", "yo" },
    { u8"ら", "ra" }, { u8"り", "ri" },  { u8"る", "ru" },  { u8"れ", "re" }, { u8"ろ", "ro" },
    { u8"わ", "wa" },                                                         { u8"を", "wo" },
    { u8"ん", "n" }
};

const Pair hiragana_diacritics[] = {
    { u8"かﾞ", "ga" }, { u8"きﾞ", "gi" }, { u8"くﾞ", "gu" }, { u8"けﾞ", "ge" }, { u8"こﾞ", "go" },
    { u8"さﾞ", "za" }, { u8"しﾞ", "ji" }, { u8"すﾞ", "zu" }, { u8"せﾞ", "ze" }, { u8"そﾞ", "zo" },
    { u8"たﾞ", "da" }, { u8"ちﾞ", "ji" }, { u8"つﾞ", "zu" }, { u8"てﾞ", "de" }, { u8"とﾞ", "do" },
    { u8"はﾞ", "ba" }, { u8"ひﾞ", "bi" }, { u8"ふﾞ", "bu" }, { u8"へﾞ", "be" }, { u8"ほﾞ", "bo" },
    { u8"はﾟ", "pa" }, { u8"ひﾟ", "pi" }, { u8"ふﾟ", "pu" }, { u8"へﾟ", "pe" }, { u8"ほﾟ", "po" }
};

const Pair hiragana_diagraphs[] = {
    { u8"きゃ", "kya" }, { u8"きゅ", "kyu" }, { u8"きょ", "kyo" },
    { u8"しゃ", "sha" }, { u8"しゅ", "shu" }, { u8"しょ", "sho" },
    { u8"ちゃ", "cha" }, { u8"ちゅ", "chu" }, { u8"ちょ", "cho" },
    { u8"にゃ", "nya" }, { u8"にゅ", "nyu" }, { u8"にょ", "nyo" },
    { u8"ひゃ", "hya" }, { u8"ひゅ", "hyu" }, { u8"ひょ", "hyo" },
    { u8"みゃ", "mya" }, { u8"みゅ", "myu" }, { u8"みょ", "myo" },
    { u8"りゃ", "rya" }, { u8"りゅ", "ryu" }, { u8"りょ", "ryo" }
};

const Pair hiragana_diagraphs_diacritics[] = {
    { u8"きﾞゃ", "gya" }, { u8"きﾞゅ", "gyu" }, { u8"きﾞょ", "gyo" },
    { u8"しﾞゃ", "ja" },  { u8"しﾞゅ", "ju" },  { u8"しﾞょ", "jo" },
    { u8"ちﾞゃ", "ja" },  { u8"ちﾞゅ", "ju" },  { u8"ちﾞょ", "jo" },
    { u8"ひﾞゃ", "bya" }, { u8"ひﾞゅ", "byu" }, { u8"ひﾞょ", "byo" },
    { u8"ひﾟゃ", "pya" }, { u8"ひﾟゅ", "pyu" }, { u8"ひﾟょ", "pyo" }
};

const char const* hiragana_sokuon = u8"っ";

const PairPtr hiragana_sokuon_monographs[] = {
    // K
    hiragana_monographs[5],  hiragana_monographs[6],  hiragana_monographs[7],  hiragana_monographs[8],  hiragana_monographs[9],
    // S
    hiragana_monographs[10], hiragana_monographs[11], hiragana_monographs[12], hiragana_monographs[13], hiragana_monographs[14],
    // T
    hiragana_monographs[15],                          hiragana_monographs[17], hiragana_monographs[18], hiragana_monographs[19]
};

const Pair hiragana_sokuon_monographs_irregular[] = {
    // T
    { u8"っち", "tchi" }
};

const PairPtr hiragana_sokuon_diacritics[] = {
    // P
    hiragana_diacritics[20], hiragana_diacritics[21], hiragana_diacritics[22], hiragana_diacritics[23], hiragana_diacritics[24]
};

const PairPtr hiragana_sokuon_diagraphs[] = {
    // K
    hiragana_diagraphs[0], hiragana_diagraphs[1], hiragana_diagraphs[2], 
    // S
    hiragana_diagraphs[3], hiragana_diagraphs[4], hiragana_diagraphs[5]
};

const Pair hiragana_sokuon_diagraphs_irregular[] = {
    // T
    { u8"っちゃ", "tcha" }, { u8"っちゅ", "tchu" }, { u8"っちょ", "tcho" }
};

const PairPtr hiragana_sokuon_diagraphs_diacritics[] = {
    // P
    hiragana_diagraphs_diacritics[12],  hiragana_diagraphs_diacritics[13], hiragana_diagraphs_diacritics[14]
};

float randf() {
    return ((float) rand()) / ((float) RAND_MAX);
}

typedef struct {
    int kana_len;
    int romanji_len;
} MaxCharLens;

void linear_search_max_char_lens(MaxCharLens* lens, const Pair const* pairs, int len) {
    // Linear search for maximums.
    for (int i = 0; i < len; i++) {
        const char const* kanachar = pairs[i][0];
        int kana_len = strlen(kanachar);
        if (kana_len > lens->kana_len) {
            lens->kana_len = kana_len;
        }

        const char const* romanjichar = pairs[i][1];
        int romanji_len = strlen(romanjichar);
        if (romanji_len > lens->romanji_len) {
            lens->romanji_len = romanji_len;
        }
    }
}

MaxCharLens hiragana_max_char_lens() {
    MaxCharLens lens = {
        .kana_len = 0,
        .romanji_len = 0
    };
    
    linear_search_max_char_lens(&lens, hiragana_monographs, DIM(hiragana_monographs));
    linear_search_max_char_lens(&lens, hiragana_diacritics, DIM(hiragana_diacritics));
    linear_search_max_char_lens(&lens, hiragana_diagraphs, DIM(hiragana_diagraphs));
    linear_search_max_char_lens(&lens, hiragana_diagraphs_diacritics, DIM(hiragana_diagraphs_diacritics));

    int sokuon_len = strlen(hiragana_sokuon);
    if (sokuon_len > lens.kana_len) {
        lens.kana_len = sokuon_len;
    }

    return lens;
}

typedef struct {
    char* kanastr;
    char* romanjistr;
} RandStr;

void randstr_append_rand_char(RandStr* randstr, const Pair const* pairs, int pairs_len) {
    int i = rand() % pairs_len;

    const char* kanachar = pairs[i][0];
    const char* romanjichar = pairs[i][1];

    if (randstr->kanastr[0] == 0) {
        // kanastr is empty.
        strcpy(randstr->kanastr, kanachar);
    } else {
        strcat(randstr->kanastr, kanachar);
    }

    if (randstr->romanjistr[0] == 0) {
        // romanjistr is empty.
        strcpy(randstr->romanjistr, romanjichar);
    } else {
        strcat(randstr->romanjistr, romanjichar);
    }
}

void randstr_append_rand_diagraph(RandStr* randstr,
                                  const Pair const* pairs, int pairs_len,
                                  const Pair const* diacritic_pairs, int diacritic_pairs_len) {
    float x = randf();

    if (x <= DIACRITIC_FREQ) {
        // Diagraph with diacritic.
        randstr_append_rand_char(randstr, diacritic_pairs, diacritic_pairs_len);
    } else {
        // Diagraph
        randstr_append_rand_char(randstr, pairs, pairs_len);
    }
}

typedef struct {
    bool regular;
    PairPtr pair;
} RandSokuonPair;

RandSokuonPair rand_sokuon_pair(PairPtr const* regular_pairs, size_t regular_pairs_len,
                                Pair const* irregular_pairs, size_t irregular_pairs_len) {
    PairPtr pair;
    bool regular;
    size_t i = rand() % (regular_pairs_len + irregular_pairs_len);

    if (i < regular_pairs_len) {
        pair = regular_pairs[i];
        regular = true;
    } else {
        pair = irregular_pairs[i - regular_pairs_len];
        regular = false;
    }

    return (RandSokuonPair) {
        .regular = regular,
        .pair = pair
    };
}

void hiragana_append_sokuon(RandStr* randstr) {
    float x = randf();
    float y = randf();

    const char* kanachar;
    const char* romanjichar;
    bool regular;

    if (x <= DIAGRAPH_FREQ) {
        if (y <= DIACRITIC_FREQ) {
            // Diagraph with diacritic.
            int i = rand() % DIM(hiragana_sokuon_diagraphs_diacritics);
            kanachar = hiragana_sokuon_diagraphs_diacritics[i][0];
            romanjichar = hiragana_sokuon_diagraphs_diacritics[i][1];
        } else {
            // Diagraph
            RandSokuonPair randpair = rand_sokuon_pair(hiragana_sokuon_diagraphs,
                                                       DIM(hiragana_sokuon_diagraphs),
                                                       hiragana_sokuon_diagraphs_irregular,
                                                       DIM(hiragana_sokuon_diagraphs_irregular));
            regular = randpair.regular;
            kanachar = randpair.pair[0];
            romanjichar = randpair.pair[1];
        }
    } else {
        if (y <= DIACRITIC_FREQ) {
            // Diacritic
            int i = rand() % DIM(hiragana_sokuon_diacritics);
            kanachar = hiragana_sokuon_diacritics[i][0];
            romanjichar = hiragana_sokuon_diacritics[i][1];
        } else {
            // Monograph
            RandSokuonPair randpair = rand_sokuon_pair(hiragana_sokuon_monographs,
                                                       DIM(hiragana_sokuon_monographs),
                                                       hiragana_sokuon_monographs_irregular,
                                                       DIM(hiragana_sokuon_monographs_irregular));
            regular = randpair.regular;
            kanachar = randpair.pair[0];
            romanjichar = randpair.pair[1];
        }
    }

    // Assert that strs are not empty.
    // Sakuon should not be the first mora.
    assert(randstr->kanastr[0] != 0);
    assert(randstr->romanjistr[0] != 0);
   
    if (regular) {
        // Apply regular gemination romanisation rules.
        strcat(randstr->kanastr, hiragana_sokuon);
        strncat(randstr->romanjistr, romanjichar, 1); // Append first romanji consonant to repeat it.
    }

    strcat(randstr->kanastr, kanachar);
    strcat(randstr->romanjistr, romanjichar);
}

RandStr rand_hiragana_str(MaxCharLens* lens, int len) {
    //                                          + 1 for null terminator.
    char* kanastr = calloc(lens->kana_len * len + 1, sizeof(char));
    char* romanjistr = calloc(lens->romanji_len * len + 1, sizeof(char));

    RandStr randstr = {
        .kanastr = kanastr,
        .romanjistr = romanjistr
    };

    int i = 0;
    while (i < len) {
        float x = randf();

        if (i == 0) {
            // First mora.
            //     Should not append sokuon.

            // Scale frequencies to compensate for no sokuon.
            float diagraph_x = DIAGRAPH_FREQ + DIAGRAPH_FREQ * SOKUON_FREQ;
            float diacritic_x = diagraph_x + DIACRITIC_FREQ + DIACRITIC_FREQ * SOKUON_FREQ;

            if (x <= diagraph_x) {
                // Diagraoh
                randstr_append_rand_diagraph(&randstr,
                                             hiragana_diagraphs, DIM(hiragana_diagraphs),
                                             hiragana_diagraphs_diacritics, DIM(hiragana_diagraphs_diacritics));
                i += 2;
            } else if (x <= diacritic_x) {
                // Diacritic
                randstr_append_rand_char(&randstr, hiragana_diacritics, DIM(hiragana_diacritics));
                i++;
            } else {
                // Monograph
                randstr_append_rand_char(&randstr, hiragana_monographs, DIM(hiragana_monographs));
                i++;
            }
        } else if (i == len - 1) {
            // Last mora.
            //     Should not append diagraph since they are two mora.
            //     Should not append sokuon.
          
            // Scale frequency to compensate for no sokuon and diagraphs.
            float diacritic_x = DIACRITIC_FREQ + DIACRITIC_FREQ * DIAGRAPH_FREQ + DIACRITIC_FREQ * SOKUON_FREQ;

            if (x < diacritic_x) {
                // Diacritic
                randstr_append_rand_char(&randstr, hiragana_diacritics, DIM(hiragana_diacritics));
                i++;
            } else {
                // Monograph
                randstr_append_rand_char(&randstr, hiragana_monographs, DIM(hiragana_monographs));
                i++;
            }
        } else {
            if (x <= SOKUON_FREQ) {
                // Sakuon
                hiragana_append_sokuon(&randstr);
                i += 2;
            } else if (x <= SOKUON_FREQ + DIAGRAPH_FREQ) {
                // Diagraoh
                randstr_append_rand_diagraph(&randstr,
                                             hiragana_diagraphs, DIM(hiragana_diagraphs),
                                             hiragana_diagraphs_diacritics, DIM(hiragana_diagraphs_diacritics));
                i += 2;
            } else if (x <= SOKUON_FREQ + DIAGRAPH_FREQ + DIACRITIC_FREQ) {
                // Diacritic
                randstr_append_rand_char(&randstr, hiragana_diacritics, DIM(hiragana_diacritics));
                i++;
            } else {
                // Monograph
                randstr_append_rand_char(&randstr, hiragana_monographs, DIM(hiragana_monographs));
                i++;
            }
        }
    }

    return randstr;
}

void randstr_free(RandStr* self) {
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

void hiragana_kana_problem(MaxCharLens* lens) {
    int char_count = 5;

    RandStr randstr = rand_hiragana_str(lens, char_count);

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
    wclear(sub_win);
    wrefresh(sub_win);
    delwin(sub_win);

    wclear(problem_win);
    wrefresh(problem_win);
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
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        wattron(result_win, COLOR_PAIR(1) | A_BOLD);
        mvwprintw(result_win, 2, 4, "CORRECT!");
        wstandend(result_win);
        mvwprintw(result_win, 4, 4, randstr.kanastr);
        mvwprintw(result_win, 6, 4, in);
    } else {
        init_pair(1, COLOR_RED, COLOR_BLACK);
        wattron(result_win, COLOR_PAIR(1) | A_BOLD);
        mvwprintw(result_win, 2, 4, "incorrect");
        wstandend(result_win);
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

    wclear(result_win);
    wrefresh(result_win);
    delwin(result_win);

    randstr_free(&randstr);
}

int main() {
    setlocale(LC_ALL, "");
    srand(time(0));

    MaxCharLens hiragana_lens = hiragana_max_char_lens();

    initscr();

    // Assert that terminal supports color.
    assert(has_colors() == TRUE);

    start_color();
    keypad(stdscr, TRUE);
    noecho();

    refresh();

    while (true) {
        hiragana_kana_problem(&hiragana_lens);
    }

    endwin();

    return 0;
}
