#include <stddef.h>
#include <string.h>

#include "ckana_def.h"

#include "MaxLens.h"

void max_lens_linear_search(MaxLens* lens, const Pair const* pairs, size_t pairs_len) {
    // Linear search for maximums.
    for (int i = 0; i < pairs_len; i++) {
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
