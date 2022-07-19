#include <stdlib.h>
#include <string.h>

#include "ckana_def.h"
#include "ckana_lib.h"

#include "RandStr.h"

void randstr_append_rand_char(RandStr* self, const Pair const* pairs, size_t pairs_len) {
    int i = rand() % pairs_len;

    const char* kanachar = pairs[i][0];
    const char* romanjichar = pairs[i][1];

    if (self->kanastr[0] == 0) {
        // kanastr is empty.
        strcpy(self->kanastr, kanachar);
    } else {
        strcat(self->kanastr, kanachar);
    }

    if (self->romanjistr[0] == 0) {
        // romanjistr is empty.
        strcpy(self->romanjistr, romanjichar);
    } else {
        strcat(self->romanjistr, romanjichar);
    }
}

void randstr_append_rand_diagraph(RandStr* self,
                                  const Pair const* pairs, size_t pairs_len,
                                  const Pair const* diacritic_pairs, size_t diacritic_pairs_len) {
    float x = randf();

    // Scale frequency to compensate for no sokuon and diagraphs.
    float diacritic_x = DIACRITIC_FREQ + DIACRITIC_FREQ * DIAGRAPH_FREQ + DIACRITIC_FREQ * SOKUON_FREQ;

    if (x <= diacritic_x) {
        // Diagraph with diacritic.
        randstr_append_rand_char(self, diacritic_pairs, diacritic_pairs_len);
    } else {
        // Diagraph
        randstr_append_rand_char(self, pairs, pairs_len);
    }
}

void randstr_free(RandStr* self) {
    free(self->kanastr);
    free(self->romanjistr);
}
