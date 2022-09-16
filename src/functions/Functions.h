#ifndef TEXT_FUNCTIONS_H
#define TEXT_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include "../structures/SentenceStructure.h"
#include "../structures/TextStructure.h"
#include "../structures/WordStructure.h"
#include "../structures/SplitSentenceStructure.h"

wchar_t* getPunctuationMarks(wchar_t*, const wchar_t*, size_t);
SplitSentence getSplitSentenceFromSentence(Sentence);
Sentence getSentenceFromSplitSentence(SplitSentence);
size_t getSentenceDuplicateWordsNumber(Sentence);
size_t getNumberOfVowels(Word);
int compareByVowelsNumber(const void*, const void*);
char isEndSentence(Sentence);

void sortTextByVowels(Text*);
void printSampleStrings(Text*);
void printSentencesDuplicateWordsNumber(Text*);
void deleteSentencesWithoutCapitalLetters(Text*);
void deleteDuplicateSentences(Text*);
void deleteSentence(Text*, size_t);

void freeWordMemory(Word*);
void freeSplitSentenceMemory(SplitSentence*);
void freeSentenceMemory(Sentence*);
void freeTextMemory(Text*);

#endif
