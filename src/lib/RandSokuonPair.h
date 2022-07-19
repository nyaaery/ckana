#pragma once

#include <stddef.h>
#include <stdbool.h>

#include "ckana_def.h"

typedef struct {
    bool regular;
    PairPtr pair;
} RandSokuonPair;

RandSokuonPair rand_sokuon_pair(PairPtr const* regular_pairs, size_t regular_pairs_len,
                                Pair const* irregular_pairs, size_t irregular_pairs_len);
