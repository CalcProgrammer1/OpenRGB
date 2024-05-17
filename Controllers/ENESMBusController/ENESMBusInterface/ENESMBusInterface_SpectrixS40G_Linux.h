/*---------------------------------------------------------*\
| ENESMBusInterface_SpectrixS40G_Linux.h                    |
|                                                           |
|   ENE SMBus interface for XPG Spectrix S40G (Linux)       |
|                                                           |
|   Adam Honse (CalcProgrammer1)                21 Nov 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "ENESMBusInterface.h"

class ENESMBusInterface_SpectrixS40G : public ENESMBusInterface
{
public:
    ENESMBusInterface_SpectrixS40G(int fd, char* path);
    ~ENESMBusInterface_SpectrixS40G();

    ene_interface_type  GetInterfaceType();
    std::string         GetLocation();
    int                 GetMaxBlock();
    unsigned char       ENERegisterRead(ene_dev_id dev, ene_register reg);
    void                ENERegisterWrite(ene_dev_id dev, ene_register reg, unsigned char val);
    void                ENERegisterWriteBlock(ene_dev_id dev, ene_register reg, unsigned char * data, unsigned char sz);

private:
    int         nvme_fd;
    std::string path;
};
