#include "Functions.h"

wchar_t* getPunctuationMarks(wchar_t* wcs1, const wchar_t* wcs2, size_t marksNumber) {
    wchar_t* punctuationMarks = calloc(marksNumber, sizeof(wchar_t));

    wcs1 = wcspbrk(wcs1, wcs2);

    for (size_t i = 0; i < marksNumber && wcs1 != NULL; i++) {
        punctuationMarks[i] = *wcs1;

        while (wcschr(wcs2, *wcs1) != NULL) {
            wcs1 += 1;
        }

        wcs1 = wcspbrk(wcs1, wcs2);
    }

    return punctuationMarks;
}

SplitSentence getSplitSentenceFromSentence(Sentence sentence) {
    SplitSentence splitSentence;
    splitSentence.words = calloc(INITIAL_WORDS_NUMBER, sizeof(Word));
    splitSentence.punctuationMarks = NULL;
    splitSentence.wordsNumber = 0;
    splitSentence.endCharacter = sentence.endCharacter;

    size_t splitSentenceMaxSize = INITIAL_WORDS_NUMBER;

    wchar_t* sentenceCopy = calloc(sentence.charactersNumber + 1, sizeof(wchar_t));
    wchar_t* sentenceCopySave = sentenceCopy;
    wchar_t* p;

    wcscpy(sentenceCopy, sentence.characters);

    sentenceCopy = wcstok(sentenceCopy, L"!?., ", &p);

    while (sentenceCopy != NULL) {
        size_t wordLength = wcslen(sentenceCopy);

        if (splitSentence.wordsNumber >= splitSentenceMaxSize) {
            splitSentenceMaxSize *= 2;
            splitSentence.words = realloc(splitSentence.words, splitSentenceMaxSize * sizeof(Word));
        }

        if (wordLength != 0) {
            Word *word = &(splitSentence.words[splitSentence.wordsNumber++]);
            word->characters = calloc(wordLength + 1, sizeof(wchar_t));
            word->charactersNumber = wordLength;

            wcscpy(word->characters, sentenceCopy);
            word->characters[wordLength] = L'\0';
        }

        sentenceCopy = wcstok(NULL, L"!?., ", &p);
    }

    if (splitSentence.wordsNumber < splitSentenceMaxSize) {
        splitSentence.words = realloc(splitSentence.words, splitSentence.wordsNumber * sizeof(Word));
    }

    splitSentence.punctuationMarks = getPunctuationMarks(sentence.characters, L"!?., ", splitSentence.wordsNumber);

    free(sentenceCopySave);

    return splitSentence;
}

Sentence getSentenceFromSplitSentence(SplitSentence splitSentence) {
    Sentence sentence;
    sentence.characters = NULL;
    sentence.charactersNumber = 0;
    sentence.endCharacter = splitSentence.endCharacter;

    for (size_t i = 0; i < splitSentence.wordsNumber; i++) {
        sentence.charactersNumber += (splitSentence.words[i].charactersNumber + 1);

        if (splitSentence.punctuationMarks[i] != L' ') {
            sentence.charactersNumber++;
        }
    }

    sentence.characters = calloc(sentence.charactersNumber + 1, sizeof(wchar_t));
    sentence.characters[0] = L'\0';

    size_t index = 0;

    for (size_t i = 0; i < splitSentence.wordsNumber; i++) {
        wcscat(sentence.characters, splitSentence.words[i].characters);
        index += splitSentence.words[i].charactersNumber;

        sentence.characters[index++] = splitSentence.punctuationMarks[i];

        if (splitSentence.punctuationMarks[i] == L',') {
            sentence.characters[index++] = L' ';
        }
    }

    sentence.characters[index] = L'\0';

    return sentence;
}

void printSampleStrings(Text* text) {
    for (size_t i = 0; i < text->sentencesNumber; i++) {
        SplitSentence splitSentence = getSplitSentenceFromSentence(text->sentences[i]);

        if (splitSentence.wordsNumber == 1) {
            wprintf(L"Строка-образец для предложения №1: %ls\n", splitSentence.words[0].characters);

        } else {
            Word longWord = splitSentence.words[0];
            Word shortWord = splitSentence.words[0];

            for (int j = 0; j < splitSentence.wordsNumber; j++) {
                if (splitSentence.words[j].charactersNumber < shortWord.charactersNumber) {
                    shortWord = splitSentence.words[j];
                }

                if (splitSentence.words[j].charactersNumber > longWord.charactersNumber) {
                    longWord = splitSentence.words[j];
                }
            }

            wchar_t *string = calloc(shortWord.charactersNumber + 2, sizeof(wchar_t));
            wcscpy(string, shortWord.characters);

            string[shortWord.charactersNumber] = L'*';
            string[shortWord.charactersNumber + 1] = L'\0';

            for (size_t j = 0; j < splitSentence.wordsNumber; j++) {
                if (splitSentence.words[j].characters != shortWord.characters) {
                    for (size_t k = 0; k < shortWord.charactersNumber; k++) {
                        if (splitSentence.words[j].characters[k] != string[k]) {
                            string[k] = L'?';
                        }
                    }
                }
            }

            size_t endIndex = shortWord.charactersNumber;

            if (string[0] == L'?') {
                string[0] = L'*';

                while (wcschr(L"?*", string[1]) != NULL && string[1] != L'\0') {
                    for (size_t j = 1; j <= endIndex; j++) {
                        string[j] = string[j + 1];
                    }
                    endIndex--;
                }
            }

            wprintf(L"Строка-образец для предложения №%d: %ls\n", i + 1, string);
            free(string);
        }

        freeSplitSentenceMemory(&splitSentence);
    }
}

char isEndSentence(Sentence sentence) {
    if (wcscmp(sentence.characters, L"\n") == 0) {
        return 1;
    } else {
        return 0;
    }
}

void deleteSentencesWithoutCapitalLetters(Text* text) {
    size_t i = 0;

    while (i < text->sentencesNumber) {
        SplitSentence splitSentence = getSplitSentenceFromSentence(text->sentences[i]);

        char delete = 1;

        for (size_t j = 0; j < splitSentence.wordsNumber && delete; j++) {
            if (iswupper(splitSentence.words[j].characters[0])) {
                delete = 0;
            }
        }

        if (delete) {
            deleteSentence(text, i);
        } else {
            i++;
        }

        freeSplitSentenceMemory(&splitSentence);
    }

}

size_t getSentenceDuplicateWordsNumber(Sentence sentence) {
    SplitSentence splitSentence = getSplitSentenceFromSentence(sentence);

    size_t maxNumber = 1;

    for (size_t i = 0; i < splitSentence.wordsNumber; i++) {
        size_t number = 1;

        for (size_t j = i + 1; j < splitSentence.wordsNumber; j++) {
            if (wcscmp(splitSentence.words[i].characters, splitSentence.words[j].characters) == 0) {
                number++;
            }
        }

        if (number > maxNumber) {
            maxNumber = number;
        }
    }

    freeSplitSentenceMemory(&splitSentence);

    return maxNumber;
}

size_t getNumberOfVowels(Word word) {
    size_t number = 0;

    for (size_t i = 0; i < word.charactersNumber; i++) {
        if (wcschr(L"aeiouyаеёиоуыэюя", towlower(word.characters[i])) != NULL) {
            number++;
        }
    }

    return number;
}

int compareByVowelsNumber(const void* word1, const void* word2) {
    size_t number1 = getNumberOfVowels(*((const Word*)word1));
    size_t number2 = getNumberOfVowels(*((const Word*)word2));

    if (number1 > number2) return 1;
    if (number1 < number2) return -1;

    return 0;
}

void sortTextByVowels(Text* text) {
    for (size_t i = 0; i < text->sentencesNumber; i++) {
        SplitSentence splitSentence = getSplitSentenceFromSentence(text->sentences[i]);

        qsort(splitSentence.words, splitSentence.wordsNumber, sizeof(Word), compareByVowelsNumber);

        freeSentenceMemory(text->sentences + i);
        text->sentences[i] = getSentenceFromSplitSentence(splitSentence);

        freeSplitSentenceMemory(&splitSentence);
    }
}

void printSentencesDuplicateWordsNumber(Text* text) {
    for (size_t i = 0; i < text->sentencesNumber; i++) {
        size_t number = getSentenceDuplicateWordsNumber(text->sentences[i]);

        if (number == 1) {
            wprintf(L"Одинаковых слов не найдено в предложении №%d: %ls\n", i + 1, text->sentences[i].characters);
        } else {
            wprintf(L"Найдено %d одинаковых слов(а) в предложении №%d: %ls\n", number, i + 1, text->sentences[i].characters);
        }
    }
}

void deleteSentence(Text* text, size_t index) {
    freeSentenceMemory(text->sentences + index);

    for (size_t i = index; i < text->sentencesNumber - 1; i++) {
        text->sentences[i] = text->sentences[i + 1];
    }

    text->sentencesNumber--;
    text->sentences = realloc(text->sentences, text->sentencesNumber * sizeof(Sentence));
}

void deleteDuplicateSentences(Text* text) {
    for (size_t i = 0; i < text->sentencesNumber - 1; i++) {
        size_t j = i + 1;

        while (j < text->sentencesNumber) {
            if (wcscasecmp(text->sentences[i].characters, text->sentences[j].characters) == 0) {
                deleteSentence(text, j);
            } else {
                j++;
            }
        }
    }
}

void freeWordMemory(Word* word) {
    free(word->characters);
    word->characters = NULL;
    word->charactersNumber = 0;
}

void freeSplitSentenceMemory(SplitSentence* splitSentence) {
    for (size_t i = 0; i < splitSentence->wordsNumber; i++) {
        freeWordMemory(splitSentence->words + i);
    }

    free(splitSentence->punctuationMarks);
    splitSentence->words = NULL;
    splitSentence->punctuationMarks = NULL;
    splitSentence->wordsNumber = 0;
}

void freeSentenceMemory(Sentence* sentence) {
    free(sentence->characters);
    sentence->characters = NULL;
    sentence->charactersNumber = 0;
}

void freeTextMemory(Text* text) {
    for (size_t i = 0; i < text->sentencesNumber; i++) {
        freeSentenceMemory(text->sentences + i);
    }

    free(text->sentences);
    text->sentences = NULL;
    text->sentencesNumber = 0;
}
