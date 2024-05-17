/*---------------------------------------------------------*\
| ENESMBusInterface_SpectrixS40G_Windows.h                  |
|                                                           |
|   ENE SMBus interface for XPG Spectrix S40G (Windows)     |
|                                                           |
|   Adam Honse (CalcProgrammer1)                21 Nov 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <windows.h>
#include "ENESMBusInterface.h"

class ENESMBusInterface_SpectrixS40G : public ENESMBusInterface
{
public:
    ENESMBusInterface_SpectrixS40G(HANDLE fd, wchar_t* path);
    ~ENESMBusInterface_SpectrixS40G();

    ene_interface_type  GetInterfaceType();
    std::string         GetLocation();
    int                 GetMaxBlock();
    unsigned char       ENERegisterRead(ene_dev_id dev, ene_register reg);
    void                ENERegisterWrite(ene_dev_id dev, ene_register reg, unsigned char val);
    void                ENERegisterWriteBlock(ene_dev_id dev, ene_register reg, unsigned char * data, unsigned char sz);

private:
    HANDLE       nvme_fd;
    std::wstring path;
};
