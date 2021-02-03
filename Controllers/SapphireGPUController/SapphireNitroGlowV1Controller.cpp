/*-----------------------------------------*\
|  SapphireNitroGlowV1Controller.cpp        |
|                                           |
|  Driver for Sapphire Nitro Glow V1 GPU    |
|  RGB lighting controller                  |
|                                           |
|  Adam Honse (CalcProgrammer1) 7/15/2020   |
\*-----------------------------------------*/

#include "SapphireNitroGlowV1Controller.h"

SapphireNitroGlowV1Controller::SapphireNitroGlowV1Controller(i2c_smbus_interface* bus, sapphire_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;
}

SapphireNitroGlowV1Controller::~SapphireNitroGlowV1Controller()
{

}

std::string SapphireNitroGlowV1Controller::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

unsigned char SapphireNitroGlowV1Controller::GetRed()
{
    return(bus->i2c_smbus_read_byte_data(dev, SAPPHIRE_NITRO_GLOW_V1_REG_RED));
}

unsigned char SapphireNitroGlowV1Controller::GetGreen()
{
    return(bus->i2c_smbus_read_byte_data(dev, SAPPHIRE_NITRO_GLOW_V1_REG_GREEN));
}

unsigned char SapphireNitroGlowV1Controller::GetBlue()
{
    return(bus->i2c_smbus_read_byte_data(dev, SAPPHIRE_NITRO_GLOW_V1_REG_BLUE));
}

void SapphireNitroGlowV1Controller::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V1_REG_RED,   red);
    bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V1_REG_GREEN, green);
    bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V1_REG_BLUE,  blue);
}

unsigned char SapphireNitroGlowV1Controller::GetMode()
{
    return(bus->i2c_smbus_read_byte_data(dev, SAPPHIRE_NITRO_GLOW_V1_REG_MODE));
}

void SapphireNitroGlowV1Controller::SetMode(unsigned char mode)
{
    bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V1_REG_MODE, mode);
}
