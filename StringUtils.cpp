/*---------------------------------------------------------*\
| StringUtils.cpp                                           |
|                                                           |
|   String utility functions                                |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <codecvt>
#include <locale>
#include <string>
#include "StringUtils.h"

const char* StringUtils::wchar_to_char(const wchar_t* pwchar)
{
    if(pwchar == nullptr)
    {
        return "";
    }

    /*-----------------------------------------------------*\
    | Get the number of characters in the string.           |
    \*-----------------------------------------------------*/
    int currentCharIndex = 0;
    char currentChar = (char)pwchar[currentCharIndex];

    while(currentChar != '\0')
    {
        currentCharIndex++;
        currentChar = (char)pwchar[currentCharIndex];
    }

    const int charCount = currentCharIndex + 1;

    /*-----------------------------------------------------*\
    | Allocate a new block of memory size char (1 byte)     |
    | instead of wide char (2 bytes)                        |
    \*-----------------------------------------------------*/
    char* filePathC = (char*)malloc(sizeof(char) * charCount);

    for(int i = 0; i < charCount; i++)
    {
        /*-------------------------------------------------*\
        | Convert to char (1 byte)                          |
        \*-------------------------------------------------*/
        char character = (char)pwchar[i];

        *filePathC = character;

        filePathC += sizeof(char);

    }

    filePathC += '\0';

    filePathC -= (sizeof(char) * charCount);

    return(filePathC);
}

std::string StringUtils::wstring_to_string(const std::wstring wstring)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

    return(converter.to_bytes(wstring));
}

std::string StringUtils::u16string_to_string(const std::u16string wstring)
{
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> converter;

    return(converter.to_bytes(wstring));
}

const std::string StringUtils::remove_null_terminating_chars(std::string input)
{
    while(!input.empty() && input.back() == 0)
    {
        input.pop_back();
    }

    return(input);
}
