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

private:
    HANDLE       nvme_fd;
    std::wstring path;
    int          a;
};
