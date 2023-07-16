/*-----------------------------------------*\
|  SeagateController.h                      |
|                                           |
|  Definitions for Seagate Firecuda         |
|  External HDD RGB controller              |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/15/2023   |
\*-----------------------------------------*/

#pragma once

#include <string>
#include <windows.h>
#include <fileapi.h>
#include "ntddscsi.h"

class SeagateController
{
public:
    SeagateController(HANDLE fd, wchar_t* path);
    ~SeagateController();

    std::string GetLocation();

    void SetLED
        (
        unsigned char led_id,
        unsigned char r,
        unsigned char g,
        unsigned char b
        );

private:
    HANDLE       fd;
    std::wstring path;
};
