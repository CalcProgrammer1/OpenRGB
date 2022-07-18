/*-----------------------------------------*\
|  ENESMBusInterface.h                      |
|                                           |
|  Definitions and types for ENE interface  |
|                                           |
|  Adam Honse (CalcProgrammer1) 11/21/2021  |
\*-----------------------------------------*/

#include <string>

#pragma once

typedef unsigned short	ene_register;
typedef unsigned char	ene_dev_id;

class ENESMBusInterface
{
public:
    virtual               ~ENESMBusInterface() = default;

    virtual std::string   GetLocation() = 0;
    virtual int           GetMaxBlock() = 0;
    virtual unsigned char ENERegisterRead(ene_dev_id dev, ene_register reg) = 0;
    virtual void          ENERegisterWrite(ene_dev_id dev, ene_register reg, unsigned char val) = 0;
    virtual void          ENERegisterWriteBlock(ene_dev_id dev, ene_register reg, unsigned char * data, unsigned char sz) = 0;
};
