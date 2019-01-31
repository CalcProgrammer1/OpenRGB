/*-----------------------------------------*\
|  AuraController.h                         |
|                                           |
|  Driver for ASUS Aura RGB lighting        |
|  controller                               |
|                                           |
|  Adam Honse (CalcProgrammer1) 8/19/2018   |
\*-----------------------------------------*/

#include "AuraController.h"

unsigned char AuraController::AuraRegisterRead(aura_register reg)
{
    //Write Aura register
    bus->i2c_smbus_write_word_data(dev, 0x00, ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF));

    //Read Aura value
    return(bus->i2c_smbus_read_byte_data(dev, 0x81));

}

void AuraController::AuraRegisterWrite(aura_register reg, unsigned char val)
{
    //Write Aura register
    bus->i2c_smbus_write_word_data(dev, 0x00, ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF));

    //Write Aura value
    bus->i2c_smbus_write_byte_data(dev, 0x01, val);

}

void AuraController::AuraRegisterWriteBlock(aura_register reg, unsigned char * data, unsigned char sz)
{
    //Write Aura register (0x8000 for colors)
    bus->i2c_smbus_write_word_data(dev, 0x00, ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF));

    //Write Aura block data
    bus->i2c_smbus_write_block_data(dev, 0x03, 15, data);

}