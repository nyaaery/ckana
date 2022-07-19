#pragma once

#include <stddef.h>

#include "ckana_def.h"

typedef struct {
    char* kanastr;
    char* romanjistr;
} RandStr;

void randstr_append_rand_char(RandStr* self, const Pair const* pairs, size_t pairs_len);
void randstr_append_rand_diagraph(RandStr* self,
                                  const Pair const* pairs, size_t pairs_len,
                                  const Pair const* diacritic_pairs, size_t diacritic_pairs_len);
void randstr_free(RandStr* self);
