#pragma once

#include <stddef.h>

#include "ckana_def.h"

typedef struct {
    int kana_len;
    int romanji_len;
} MaxLens;

void max_lens_linear_search(MaxLens* self, const Pair const* pairs, size_t pairs_len);
