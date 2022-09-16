#ifndef WORD_STRUCTURE_H
#define WORD_STRUCTURE_H

#include <stddef.h>

struct Word {
    wchar_t* characters;
    size_t charactersNumber;
};

typedef struct Word Word;

#endif
