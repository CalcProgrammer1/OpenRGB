/*-----------------------------------------*\
|  ENESMBusInterface_SpectrixS40G.h         |
|                                           |
|  Definitions and types for ENE XPG        |
|  Spectrix S40G NVMe interface             |
|                                           |
|  Adam Honse (CalcProgrammer1) 11/21/2021  |
\*-----------------------------------------*/

#include "ENESMBusInterface.h"

#pragma once

class ENESMBusInterface_SpectrixS40G : public ENESMBusInterface
{
public:
    ENESMBusInterface_SpectrixS40G(int fd, char* path);
    ~ENESMBusInterface_SpectrixS40G();

    std::string   GetLocation();
    int           GetMaxBlock();
    unsigned char ENERegisterRead(ene_dev_id dev, ene_register reg);
    void          ENERegisterWrite(ene_dev_id dev, ene_register reg, unsigned char val);
    void          ENERegisterWriteBlock(ene_dev_id dev, ene_register reg, unsigned char * data, unsigned char sz);

private:
    int         nvme_fd;
    std::string path;
};
