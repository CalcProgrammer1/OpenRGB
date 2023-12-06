#include "StringUtils.h"
#include <string>

const char* StringUtils::wchar_to_char(const wchar_t* pwchar)
{
    if (pwchar == nullptr)
    {
        return "";
    }
    // get the number of characters in the string.
    int currentCharIndex = 0;
    char currentChar = pwchar[currentCharIndex];

    while (currentChar != '\0')
    {
        currentCharIndex++;
        currentChar = pwchar[currentCharIndex];
    }

    const int charCount = currentCharIndex + 1;

    // allocate a new block of memory size char (1 byte) instead of wide char (2 bytes)
    char* filePathC = (char*)malloc(sizeof(char) * charCount);

    for (int i = 0; i < charCount; i++)
    {
        // convert to char (1 byte)
        char character = pwchar[i];

        *filePathC = character;

        filePathC += sizeof(char);

    }
    filePathC += '\0';

    filePathC -= (sizeof(char) * charCount);

    return filePathC;
}

const std::string StringUtils::remove_null_terminating_chars(std::string input)
{
    while (!input.empty() && input.back() == 0)
    {
        input.pop_back();
    }

    return input;
}

