/*-----------------------------------------*\
|  PatriotViperController.cpp               |
|                                           |
|  Definitions and types for Patriot Viper  |
|  RGB RAM lighting controller              |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/1/2020    |
\*-----------------------------------------*/

#include "PatriotViperController.h"
#include <cstring>

PatriotViperController::PatriotViperController(i2c_smbus_interface* bus, viper_dev_id dev, unsigned char slots)
{
    this->bus   = bus;
    this->dev   = dev;
    slots_valid = slots;

    strcpy(device_name, "Patriot Viper RGB");

    led_count = 0;

    for(int i = 0; i < 8; i++)
    {
        if((slots_valid & (1 << i)) != 0)
        {
            led_count += 5;
        }
    }
}

PatriotViperController::~PatriotViperController()
{

}

std::string PatriotViperController::GetDeviceName()
{
    return(device_name);
}

std::string PatriotViperController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return(return_string);
}

unsigned int PatriotViperController::GetLEDCount()
{
    return(led_count);
}

unsigned int PatriotViperController::GetSlotCount()
{
    unsigned int slot_count = 0;

    for(int slot = 0; slot < 4; slot++)
    {
        if((slots_valid & (1 << slot)) != 0)
        {
            slot_count++;
        }
    }

    return(slot_count);
}

unsigned int PatriotViperController::GetMode()
{
    return(0);
}

void PatriotViperController::SetEffectColor(unsigned char red, unsigned char green, unsigned char blue)
{

}

void PatriotViperController::SetAllColors(unsigned char red, unsigned char green, unsigned char blue)
{
    bus->i2c_smbus_write_byte_data(dev, 0xFF, 0xFF);
    bus->i2c_smbus_write_byte_data(dev, 0xFF, 0xFF);

    bus->i2c_smbus_write_byte_data(dev, 0x01, 0x04);
    bus->i2c_smbus_write_byte_data(dev, 0x00, 0x00);
    
    bus->i2c_smbus_write_byte_data(dev, 0x30, red);
    bus->i2c_smbus_write_byte_data(dev, green, blue);

    bus->i2c_smbus_write_byte_data(dev, 0x31, red);
    bus->i2c_smbus_write_byte_data(dev, green, blue);

    bus->i2c_smbus_write_byte_data(dev, 0x32, red);
    bus->i2c_smbus_write_byte_data(dev, green, blue);

    bus->i2c_smbus_write_byte_data(dev, 0x33, red);
    bus->i2c_smbus_write_byte_data(dev, green, blue);

    bus->i2c_smbus_write_byte_data(dev, 0x34, red);
    bus->i2c_smbus_write_byte_data(dev, green, blue);

    bus->i2c_smbus_write_byte_data(dev, 0x35, 0x01);
    bus->i2c_smbus_write_byte_data(dev, 0x00, 0x00);
}

void PatriotViperController::SetLEDColor(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    bus->i2c_smbus_write_byte_data(dev, 0xFF, 0xFF);
    bus->i2c_smbus_write_byte_data(dev, 0xFF, 0xFF);

    bus->i2c_smbus_write_byte_data(dev, 0x01, 0x04);
    bus->i2c_smbus_write_byte_data(dev, 0x00, 0x00);

    bus->i2c_smbus_write_byte_data(dev, 0x30 + led, red);
    bus->i2c_smbus_write_byte_data(dev, green, blue);

    bus->i2c_smbus_write_byte_data(dev, 0x35, 0x01);
    bus->i2c_smbus_write_byte_data(dev, 0x00, 0x00);
}


void PatriotViperController::SetLEDColor(unsigned int slot, unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    bus->i2c_smbus_write_byte_data(dev, 0xFF, 0xFF);
    bus->i2c_smbus_write_byte_data(dev, 0xFF, 0xFF);

    bus->i2c_smbus_write_byte_data(dev, 0x01, 0x04);
    bus->i2c_smbus_write_byte_data(dev, 0x00, 0x00);

    bus->i2c_smbus_write_byte_data(dev, 0x30 + led, red);
    bus->i2c_smbus_write_byte_data(dev, green, blue);

    bus->i2c_smbus_write_byte_data(dev, 0x35, 0x01);
    bus->i2c_smbus_write_byte_data(dev, 0x00, 0x00);
}

void PatriotViperController::SetMode(unsigned char new_mode)
{

}
