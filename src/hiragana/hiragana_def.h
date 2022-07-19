#pragma once

#include "../lib/ckana_lib.h"

static const Pair hiragana_monographs[] = {
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

static const Pair hiragana_diacritics[] = {
    { u8"かﾞ", "ga" }, { u8"きﾞ", "gi" }, { u8"くﾞ", "gu" }, { u8"けﾞ", "ge" }, { u8"こﾞ", "go" },
    { u8"さﾞ", "za" }, { u8"しﾞ", "ji" }, { u8"すﾞ", "zu" }, { u8"せﾞ", "ze" }, { u8"そﾞ", "zo" },
    { u8"たﾞ", "da" }, { u8"ちﾞ", "ji" }, { u8"つﾞ", "zu" }, { u8"てﾞ", "de" }, { u8"とﾞ", "do" },
    { u8"はﾞ", "ba" }, { u8"ひﾞ", "bi" }, { u8"ふﾞ", "bu" }, { u8"へﾞ", "be" }, { u8"ほﾞ", "bo" },
    { u8"はﾟ", "pa" }, { u8"ひﾟ", "pi" }, { u8"ふﾟ", "pu" }, { u8"へﾟ", "pe" }, { u8"ほﾟ", "po" }
};

static const Pair hiragana_diagraphs[] = {
    { u8"きゃ", "kya" }, { u8"きゅ", "kyu" }, { u8"きょ", "kyo" },
    { u8"しゃ", "sha" }, { u8"しゅ", "shu" }, { u8"しょ", "sho" },
    { u8"ちゃ", "cha" }, { u8"ちゅ", "chu" }, { u8"ちょ", "cho" },
    { u8"にゃ", "nya" }, { u8"にゅ", "nyu" }, { u8"にょ", "nyo" },
    { u8"ひゃ", "hya" }, { u8"ひゅ", "hyu" }, { u8"ひょ", "hyo" },
    { u8"みゃ", "mya" }, { u8"みゅ", "myu" }, { u8"みょ", "myo" },
    { u8"りゃ", "rya" }, { u8"りゅ", "ryu" }, { u8"りょ", "ryo" }
};

static const Pair hiragana_diagraphs_diacritics[] = {
    { u8"きﾞゃ", "gya" }, { u8"きﾞゅ", "gyu" }, { u8"きﾞょ", "gyo" },
    { u8"しﾞゃ", "ja" },  { u8"しﾞゅ", "ju" },  { u8"しﾞょ", "jo" },
    { u8"ちﾞゃ", "ja" },  { u8"ちﾞゅ", "ju" },  { u8"ちﾞょ", "jo" },
    { u8"ひﾞゃ", "bya" }, { u8"ひﾞゅ", "byu" }, { u8"ひﾞょ", "byo" },
    { u8"ひﾟゃ", "pya" }, { u8"ひﾟゅ", "pyu" }, { u8"ひﾟょ", "pyo" }
};

static const char const* hiragana_sokuon = u8"っ";

static const PairPtr hiragana_sokuon_monographs[] = {
    // K
    hiragana_monographs[5],  hiragana_monographs[6],  hiragana_monographs[7],  hiragana_monographs[8],  hiragana_monographs[9],
    // S
    hiragana_monographs[10], hiragana_monographs[11], hiragana_monographs[12], hiragana_monographs[13], hiragana_monographs[14],
    // T
    hiragana_monographs[15],                          hiragana_monographs[17], hiragana_monographs[18], hiragana_monographs[19]
};

static const Pair hiragana_sokuon_monographs_irregular[] = {
    // T
    { u8"っち", "tchi" }
};

static const PairPtr hiragana_sokuon_diacritics[] = {
    // P
    hiragana_diacritics[20], hiragana_diacritics[21], hiragana_diacritics[22], hiragana_diacritics[23], hiragana_diacritics[24]
};

static const PairPtr hiragana_sokuon_diagraphs[] = {
    // K
    hiragana_diagraphs[0], hiragana_diagraphs[1], hiragana_diagraphs[2], 
    // S
    hiragana_diagraphs[3], hiragana_diagraphs[4], hiragana_diagraphs[5]
};

static const Pair hiragana_sokuon_diagraphs_irregular[] = {
    // T
    { u8"っちゃ", "tcha" }, { u8"っちゅ", "tchu" }, { u8"っちょ", "tcho" }
};

static const PairPtr hiragana_sokuon_diagraphs_diacritics[] = {
    // P
    hiragana_diagraphs_diacritics[12],  hiragana_diagraphs_diacritics[13], hiragana_diagraphs_diacritics[14]
};
