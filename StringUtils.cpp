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

std::string StringUtils::make_filename(std::string input)
{
    /*-----------------------------------------------------*\
    | Replace : characters with - characters                |
    \*-----------------------------------------------------*/
    input = std::regex_replace(input, std::regex(":"), "-");

    /*-----------------------------------------------------*\
    | Remove all other characters                           |
    \*-----------------------------------------------------*/
    input = std::regex_replace(input, std::regex("[#%&\\{\\}\\\\<>\\*\\?/!`';@+|=]"), "");

    /*-----------------------------------------------------*\
    | Remove leading . characters                           |
    \*-----------------------------------------------------*/
    input = std::regex_replace(input, std::regex("^\\.+"), "");

    /*-----------------------------------------------------*\
    | Remove control characters                             |
    \*-----------------------------------------------------*/
    input = std::regex_replace(input, std::regex("[\\x00-\\x1F\\x7F]"), "");

    /*-----------------------------------------------------*\
    | Return complete string                                |
    \*-----------------------------------------------------*/
    return(input);
}
