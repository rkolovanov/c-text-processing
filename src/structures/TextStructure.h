#ifndef TEXT_STRUCTURE_H
#define TEXT_STRUCTURE_H

#include "SentenceStructure.h"

#define INITIAL_SENTENCES_NUMBER 5

struct Text {
    Sentence* sentences;
    size_t sentencesNumber;
};

typedef struct Text Text;

#endif
