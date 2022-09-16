#include "InputOutputFunctions.h"

Sentence readSentence(FILE* stream) {
    Sentence sentence;
    sentence.characters = calloc(INITIAL_CHARACTERS_NUMBER, sizeof(wchar_t));
    sentence.charactersNumber = 0;

    size_t characterMaxNumber = INITIAL_CHARACTERS_NUMBER;

    do {
        if (sentence.charactersNumber + 1 >= characterMaxNumber) {
            characterMaxNumber *= 2;
            sentence.characters = realloc(sentence.characters, characterMaxNumber * sizeof(wchar_t));
        }

        sentence.characters[sentence.charactersNumber++] = fgetwc(stream);

    } while (wcschr(L".!?\n", sentence.characters[sentence.charactersNumber - 1]) == NULL);

    sentence.characters[sentence.charactersNumber] = L'\0';

    if (sentence.charactersNumber + 1 < characterMaxNumber) {
        sentence.characters = realloc(sentence.characters, (sentence.charactersNumber + 1) * sizeof(wchar_t));
    }

    return sentence;
}

Text* readText(FILE* stream) {
    Text* text = malloc(sizeof(Text));
    text->sentences = calloc(INITIAL_SENTENCES_NUMBER, sizeof(Sentence));
    text->sentencesNumber = 0;

    wchar_t endCharacter = L'\n';
    size_t sentencesMaxNumber = INITIAL_SENTENCES_NUMBER;
    char loopEnabled = 1;

    do {
        if (text->sentencesNumber >= sentencesMaxNumber) {
            sentencesMaxNumber *= 2;
            text->sentences = realloc(text->sentences, sentencesMaxNumber * sizeof(Sentence));
        }

        text->sentences[text->sentencesNumber++] = readSentence(stream);

        if (endCharacter == L'\n' && isEndSentence(text->sentences[text->sentencesNumber - 1])) {
            freeSentenceMemory(text->sentences + (text->sentencesNumber - 1));
            text->sentencesNumber--;
            loopEnabled = 0;
        } else {
            endCharacter = fgetwc(stream);
            text->sentences[text->sentencesNumber - 1].endCharacter = endCharacter;
        }

        if (endCharacter == WEOF) {
            loopEnabled = 0;
        }

    } while (loopEnabled);

    if (text->sentencesNumber < sentencesMaxNumber) {
        text->sentences = realloc(text->sentences, text->sentencesNumber * sizeof(Sentence));
    }

    return text;
}

void printText(Text* text) {
    for (int i = 0; i < text->sentencesNumber; i++) {
        wprintf(L"%ls%lc", text->sentences[i].characters, text->sentences[i].endCharacter);
    }
}
