#ifndef WORD_LIST_STRUCTURE_H
#define WORD_LIST_STRUCTURE_H

#include <stddef.h>
#include "WordStructure.h"

#define INITIAL_WORDS_NUMBER 4

struct SplitSentence {
    Word* words;
    size_t wordsNumber;
    wchar_t* punctuationMarks;
    wchar_t endCharacter;
};

typedef struct SplitSentence SplitSentence;

#endif
