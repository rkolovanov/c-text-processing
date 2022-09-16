#include <locale.h>
#include "functions/Functions.h"
#include "functions/InputOutputFunctions.h"

int main() {
    setlocale(LC_ALL, "ru_RU.utf8");

    int userAction;
    char loopEnabled = 1;

    Text* text = NULL;

    do {
        wprintf(L"\n[ Выберите метод считывания текста ]\n  1: Считать текст с консоли.\n  2: Считать текст с "
                "файла (text.txt).\n  0: Выйти из прораммы.\n\n>>> ");

        wscanf(L"%d", &userAction);
        fgetwc(stdin);

        FILE* stream = NULL;

        switch (userAction) {
            case 0:
                return 0;

            case 1:
                wprintf(L"\nВведите текст:\n");

                text = readText(stdin);
                deleteDuplicateSentences(text);

                break;

            case 2:
                stream = fopen("text.txt", "r");

                if (stream == NULL) {
                    wprintf(L"[ Ошибка ] Файл text.txt не существует или не доступен для программы.");
                    return 0;
                }

                text = readText(stream);
                deleteDuplicateSentences(text);

                wprintf(L"\n[ Считанный текст ]\n\n");
                printText(text);
                wprintf(L"\n");

                break;

            default:
                wprintf(L"[ Вы ввели некорректное значение ]\n");
        }

    } while (userAction < 0 || userAction > 2);

    while (loopEnabled) {
        wprintf(L"\n[ Выберите действие ]\n  1: Для каждого предложения вывести строку образец, удовлетво"
                "ряющую каждому слову в предложении.\n  2: Удалить все предложения, в которых нет заглавных букв в "
                "начале слова.\n  3: Отсортировать слова в предложении по количеству гласных букв в слове.\n  4: "
                "Для каждого предложения вывести количество одинаковых слов в строке.\n  5: Вывести текст на экран."
                "\n  0: Выйти из программы.\n\n>>> ");
        wscanf(L"%d", &userAction);
        wprintf(L"\n");

        switch (userAction) {
            case 0:
                loopEnabled = 0;
                break;

            case 1:
                printSampleStrings(text);
                break;

            case 2:
                deleteSentencesWithoutCapitalLetters(text);
                break;

            case 3:
                sortTextByVowels(text);
                break;

            case 4:
                printSentencesDuplicateWordsNumber(text);
                break;

            case 5:
                printText(text);
                wprintf(L"\n");
                break;

            default:
                wprintf(L"\n[ Вы ввели некорректное значение ]\n\n");
        }
    }

    freeTextMemory(text);

    return 0;
}