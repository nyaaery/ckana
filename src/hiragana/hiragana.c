#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "../lib/ckana_def.h"
#include "../lib/ckana_lib.h"
#include "../lib/MaxLens.h"
#include "../lib/RandStr.h"
#include "../lib/RandSokuonPair.h"
#include "hiragana_def.h"

#include "hiragana.h"

MaxLens hiragana_max_lens() {
    MaxLens lens = {
        .kana_len = 0,
        .romanji_len = 0
    };
    
    max_lens_linear_search(&lens, hiragana_monographs, DIM(hiragana_monographs));
    max_lens_linear_search(&lens, hiragana_diacritics, DIM(hiragana_diacritics));
    max_lens_linear_search(&lens, hiragana_diagraphs, DIM(hiragana_diagraphs));
    max_lens_linear_search(&lens, hiragana_diagraphs_diacritics, DIM(hiragana_diagraphs_diacritics));

    int sokuon_len = strlen(hiragana_sokuon);
    if (sokuon_len > lens.kana_len) {
        lens.kana_len = sokuon_len;
    }

    return lens;
}

void hiragana_append_rand_sokuon(RandStr* randstr) {
    float x = randf();
    float y = randf();

    // Scale frequency to compensate for no sokuon and diagraphs.
    float diacritic_y = DIACRITIC_FREQ + DIACRITIC_FREQ * DIAGRAPH_FREQ + DIACRITIC_FREQ * SOKUON_FREQ;

    const char* kanachar;
    const char* romanjichar;
    bool regular;

    if (x <= DIAGRAPH_FREQ) {
        if (y <= diacritic_y) {
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
        if (y <= diacritic_y) {
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

RandStr hiragana_rand_str(MaxLens* lens, int len) {
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
                hiragana_append_rand_sokuon(&randstr);
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

PairPtr hiragana_rand_pair() {
    PairPtr pair;

    float x = randf();

    // Scale frequencies to compensate for no sokuon.
    float diagraph_x = DIAGRAPH_FREQ + DIAGRAPH_FREQ * SOKUON_FREQ;
    float diacritic_x = diagraph_x + DIACRITIC_FREQ + DIACRITIC_FREQ * SOKUON_FREQ;

    if (x <= diagraph_x) {
        // Diagraph
        float y = randf();

        // Scale frequency to compensate for no sokuon and diagraphs.
        float diacritic_y = DIACRITIC_FREQ + DIACRITIC_FREQ * DIAGRAPH_FREQ + DIACRITIC_FREQ * SOKUON_FREQ;

        if (y <= diacritic_y) {
            // Diagraph with diacritic.
            int i = rand() % DIM(hiragana_diagraphs_diacritics);
            pair = hiragana_diagraphs_diacritics[i];
        } else {
            // Diagraph
            int i = rand() % DIM(hiragana_diagraphs);
            pair = hiragana_diagraphs[i];
        }
    } else if (x <= diacritic_x) {
        // Diacritic
        int i = rand() % DIM(hiragana_diacritics);
        pair = hiragana_diacritics[i];
    } else {
        // Monograph
        int i = rand() % DIM(hiragana_monographs);
        pair = hiragana_monographs[i];
    }

    return pair;
}

