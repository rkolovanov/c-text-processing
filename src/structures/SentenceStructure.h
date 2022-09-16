#ifndef SENTENCE_STRUCTURE_H
#define SENTENCE_STRUCTURE_H

#include <stddef.h>

#define INITIAL_CHARACTERS_NUMBER 10

struct Sentence {
    wchar_t* characters;
    size_t charactersNumber;
    wchar_t endCharacter;
};

typedef struct Sentence Sentence;

#endif
