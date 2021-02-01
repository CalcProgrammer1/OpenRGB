/*-----------------------------------------*\
|  SapphireNitroGlowV3Controller.cpp        |
|                                           |
|  Driver for Sapphire Nitro Glow V3 GPU    |
|  RGB lighting controller                  |
|                                           |
|  K900 2/3/2021                            |
\*-----------------------------------------*/

#include "SapphireNitroGlowV3Controller.h"

SapphireNitroGlowV3Controller::SapphireNitroGlowV3Controller(i2c_smbus_interface* bus, sapphire_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;
}

SapphireNitroGlowV3Controller::~SapphireNitroGlowV3Controller()
{

}

std::string SapphireNitroGlowV3Controller::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

unsigned char SapphireNitroGlowV3Controller::GetRed()
{
    return(bus->i2c_smbus_read_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_RED));
}

unsigned char SapphireNitroGlowV3Controller::GetGreen()
{
    return(bus->i2c_smbus_read_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_GREEN));
}

unsigned char SapphireNitroGlowV3Controller::GetBlue()
{
    return(bus->i2c_smbus_read_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_BLUE));
}

void SapphireNitroGlowV3Controller::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_RED,   red);
    bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_GREEN, green);
    bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_BLUE,  blue);
}

void SapphireNitroGlowV3Controller::SetMode(unsigned char mode, unsigned char speed)
{
    switch(mode)
    {
        case SAPPHIRE_NITRO_GLOW_V3_MODE_RAINBOW:
            bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_EXTERNAL_CONTROL, 0);
            bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_RAINBOW_ANIMATION_SPEED, speed);
            bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_MODE, mode);
            break;

        case SAPPHIRE_NITRO_GLOW_V3_MODE_RUNWAY:
            bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_EXTERNAL_CONTROL, 0);
            bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_RUNWAY_ANIMATION_SPEED, speed);
            bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_RUNWAY_ANIMATION_REPEAT_COUNT, 5);
            bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_MODE, mode);
            break;

        case SAPPHIRE_NITRO_GLOW_V3_MODE_COLOR_CYCLE:
            bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_EXTERNAL_CONTROL, 0);
            bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_COLOR_CYCLE_ANIMATION_SPEED, speed);
            bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_MODE, mode);
            break;

        case SAPPHIRE_NITRO_GLOW_V3_MODE_SERIAL:
            bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_EXTERNAL_CONTROL, 0);
            bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_SERIAL_ANIMATION_SPEED, speed);
            bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_MODE, mode);
            break;

        case SAPPHIRE_NITRO_GLOW_V3_MODE_SAPPHIRE_BLUE:
            break;

        case SAPPHIRE_NITRO_GLOW_V3_MODE_AUDIO_VISUALIZATION:
            break;

        case SAPPHIRE_NITRO_GLOW_V3_MODE_CUSTOM:
            bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_EXTERNAL_CONTROL, 0);
            bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_MODE, mode);
            break;

        case SAPPHIRE_NITRO_GLOW_V3_MODE_OFF:
            break;

        case SAPPHIRE_NITRO_GLOW_V3_MODE_EXTERNAL_CONTROL:
            bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_EXTERNAL_CONTROL, 1);
            break;
    }
}