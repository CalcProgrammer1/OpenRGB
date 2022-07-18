/*-----------------------------------------*\
|  ENESMBusInterface_i2c_smbus.cpp          |
|                                           |
|  Code for ENE I2C/SMBus interface         |
|                                           |
|  Adam Honse (CalcProgrammer1) 11/21/2021  |
\*-----------------------------------------*/

#include "ENESMBusInterface_i2c_smbus.h"

ENESMBusInterface_i2c_smbus::ENESMBusInterface_i2c_smbus(i2c_smbus_interface* bus)
{
    this->bus = bus;
}

ENESMBusInterface_i2c_smbus::~ENESMBusInterface_i2c_smbus()
{

}

std::string ENESMBusInterface_i2c_smbus::GetLocation()
{
    std::string return_string(bus->device_name);
    return("I2C: " + return_string);
}

int ENESMBusInterface_i2c_smbus::GetMaxBlock()
{
    return(3);
}

unsigned char ENESMBusInterface_i2c_smbus::ENERegisterRead(ene_dev_id dev, ene_register reg)
{
    //Write ENE register
    bus->i2c_smbus_write_word_data(dev, 0x00, ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF));

    //Read ENE value
    return(bus->i2c_smbus_read_byte_data(dev, 0x81));
}

void ENESMBusInterface_i2c_smbus::ENERegisterWrite(ene_dev_id dev, ene_register reg, unsigned char val)
{
    //Write ENE register
    bus->i2c_smbus_write_word_data(dev, 0x00, ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF));

    //Write ENE value
    bus->i2c_smbus_write_byte_data(dev, 0x01, val);
}

void ENESMBusInterface_i2c_smbus::ENERegisterWriteBlock(ene_dev_id dev, ene_register reg, unsigned char * data, unsigned char sz)
{
    //Write ENE register
    bus->i2c_smbus_write_word_data(dev, 0x00, ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF));

    //Write ENE block data
    bus->i2c_smbus_write_block_data(dev, 0x03, sz, data);
}
