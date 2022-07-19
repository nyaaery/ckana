#include <stdlib.h>
#include <stdbool.h>

#include "RandSokuonPair.h"

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
