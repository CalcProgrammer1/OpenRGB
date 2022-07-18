/*-----------------------------------------*\
|  ENESMBusInterface_i2c_smbus.h            |
|                                           |
|  Definitions and types for ENE I2C/SMBus  |
|  interface                                |
|                                           |
|  Adam Honse (CalcProgrammer1) 11/21/2021  |
\*-----------------------------------------*/

#include "ENESMBusInterface.h"
#include "i2c_smbus.h"

#pragma once

class ENESMBusInterface_i2c_smbus : public ENESMBusInterface
{
public:
    ENESMBusInterface_i2c_smbus(i2c_smbus_interface* bus);
    ~ENESMBusInterface_i2c_smbus();

    std::string   GetLocation();
    int           GetMaxBlock();
    unsigned char ENERegisterRead(ene_dev_id dev, ene_register reg);
    void          ENERegisterWrite(ene_dev_id dev, ene_register reg, unsigned char val);
    void          ENERegisterWriteBlock(ene_dev_id dev, ene_register reg, unsigned char * data, unsigned char sz);

private:
    i2c_smbus_interface *   bus;
};
