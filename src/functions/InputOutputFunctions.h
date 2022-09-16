#ifndef INPUT_OUTPUT_FUNCTIONS_H
#define INPUT_OUTPUT_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "../structures/SentenceStructure.h"
#include "../structures/TextStructure.h"
#include "Functions.h"

Sentence readSentence(FILE*);
Text* readText(FILE*);

void printText(Text*);

#endif
