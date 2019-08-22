/*-----------------------------------------*\
|  HyperXController.cpp                     |
|                                           |
|  Definitions and types for HyperX Predator|
|  RGB RAM lighting controller              |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/29/2019   |
\*-----------------------------------------*/

#include "HyperXController.h"
#include <cstring>

HyperXController::HyperXController(i2c_smbus_interface* bus, hyperx_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;

    strcpy(device_name, "HyperX Predator RGB");
    led_count = 1;
}

HyperXController::~HyperXController()
{

}

char* HyperXController::GetDeviceName()
{
    return(device_name);
}

unsigned int HyperXController::GetLEDCount()
{
    return(led_count);
}

void HyperXController::SetAllColors(unsigned char red, unsigned char green, unsigned char blue)
{
    bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_APPLY, 0x01);

    bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_RED, red);
    bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_GREEN, green);
    bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_BLUE, blue);

    bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_APPLY, 0x02);
    bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_APPLY, 0x03);
}

void HyperXController::SetLEDColor(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_APPLY, 0x01);

    bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_RED, red);
    bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_GREEN, green);
    bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_BLUE, blue);

    bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_APPLY, 0x02);
    bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_APPLY, 0x03);
}

void HyperXController::SetMode(unsigned char mode)
{
    bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_APPLY, 0x01);

    switch (mode)
    {
    case HYPERX_MODE_STATIC:
        bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_MODE2, HYPERX_MODE2_STATIC);
        break;

    case HYPERX_MODE_RAINBOW:
        bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_MODE1, HYPERX_MODE1_RAINBOW);
        break;

    case HYPERX_MODE_COMET:
        bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_MODE2, HYPERX_MODE2_COMET);
        break;

    case HYPERX_MODE_HEARTBEAT:
        bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_MODE2, HYPERX_MODE2_HEARTBEAT);
        break;

    case HYPERX_MODE_CYCLE:
        bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_MODE1, HYPERX_MODE1_CYCLE);
        break;

    case HYPERX_MODE_BREATHING:
        bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_MODE2, HYPERX_MODE2_BREATHING);
        break;

    case HYPERX_MODE_BOUNCE:
        bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_MODE2, HYPERX_MODE2_BOUNCE);
        break;

    case HYPERX_MODE_BLINK:
        bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_MODE2, HYPERX_MODE2_BLINK);
        break;
    }

    bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_APPLY, 0x02);
    bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_APPLY, 0x03);
}