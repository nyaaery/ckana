#pragma once

#include "../lib/ckana_def.h"
#include "../lib/RandStr.h"
#include "../lib/MaxLens.h"

MaxLens hiragana_max_lens();
void hiragana_append_rand_sokuon(RandStr* randstr);
RandStr hiragana_rand_str(MaxLens* lens, int len);
PairPtr hiragana_rand_pair();
