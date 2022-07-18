/*-----------------------------------------*\
|  ENESMBusInterface_SpectrixS40G_Windows.h |
|                                           |
|  Definitions and types for ENE XPG        |
|  Spectrix S40G NVMe interface             |
|  Windows implementation                   |
|                                           |
|  Adam Honse (CalcProgrammer1) 11/21/2021  |
\*-----------------------------------------*/

#include "ENESMBusInterface.h"
#include <windows.h>

#pragma once

class ENESMBusInterface_SpectrixS40G : public ENESMBusInterface
{
public:
    ENESMBusInterface_SpectrixS40G(HANDLE fd, wchar_t* path);
    ~ENESMBusInterface_SpectrixS40G();

    std::string   GetLocation();
    int           GetMaxBlock();
    unsigned char ENERegisterRead(ene_dev_id dev, ene_register reg);
    void          ENERegisterWrite(ene_dev_id dev, ene_register reg, unsigned char val);
    void          ENERegisterWriteBlock(ene_dev_id dev, ene_register reg, unsigned char * data, unsigned char sz);

private:
    HANDLE       nvme_fd;
    std::wstring path;
};
