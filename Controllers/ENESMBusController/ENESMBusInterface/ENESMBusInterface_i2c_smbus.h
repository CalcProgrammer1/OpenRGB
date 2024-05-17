/*---------------------------------------------------------*\
| ENESMBusInterface_i2c_smbus.h                             |
|                                                           |
|   ENE SMBus interface for I2C/SMBus                       |
|                                                           |
|   Adam Honse (CalcProgrammer1)                21 Nov 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "ENESMBusInterface.h"
#include "i2c_smbus.h"

class ENESMBusInterface_i2c_smbus : public ENESMBusInterface
{
public:
    ENESMBusInterface_i2c_smbus(i2c_smbus_interface* bus);
    ~ENESMBusInterface_i2c_smbus();

    ene_interface_type  GetInterfaceType();
    std::string         GetLocation();
    int                 GetMaxBlock();
    unsigned char       ENERegisterRead(ene_dev_id dev, ene_register reg);
    void                ENERegisterWrite(ene_dev_id dev, ene_register reg, unsigned char val);
    void                ENERegisterWriteBlock(ene_dev_id dev, ene_register reg, unsigned char * data, unsigned char sz);

private:
    i2c_smbus_interface *   bus;
};
