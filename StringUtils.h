#ifndef STRING_UTILS_H
#define STRING_UTILS_H

class StringUtils
{
public:
    static const char* wchar_to_char(const wchar_t* pwchar);
    static std::string make_filename(std::string input);
};

#endif // STRING_UTILS_H
