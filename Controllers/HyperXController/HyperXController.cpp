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
    led_count = 5;

    mode = HYPERX_MODE_DIRECT;
}

HyperXController::~HyperXController()
{

}

std::string HyperXController::GetDeviceName()
{
    return(device_name);
}

std::string HyperXController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return(return_string);
}

unsigned int HyperXController::GetLEDCount()
{
    return(led_count);
}

unsigned int HyperXController::GetMode()
{
    return(mode);
}

void HyperXController::SetEffectColor(unsigned char red, unsigned char green, unsigned char blue)
{
    bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_APPLY, 0x01);

    bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_EFFECT_RED, red);
    bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_EFFECT_GREEN, green);
    bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_EFFECT_BLUE, blue);

    bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_APPLY, 0x02);
    bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_APPLY, 0x03);
}

void HyperXController::SetAllColors(unsigned char red, unsigned char green, unsigned char blue)
{
    bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_APPLY, 0x01);

    for(int slot = 0; slot < 4; slot++)
    {
        unsigned char red_base    = HYPERX_REG_SLOT0_LED0_RED        + (0x20 * slot);
        unsigned char green_base  = HYPERX_REG_SLOT0_LED0_GREEN      + (0x20 * slot);
        unsigned char blue_base   = HYPERX_REG_SLOT0_LED0_BLUE       + (0x20 * slot);
        unsigned char bright_base = HYPERX_REG_SLOT0_LED0_BRIGHTNESS + (0x20 * slot);

        if(mode == HYPERX_MODE_DIRECT)
        {
            bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_MODE3, HYPERX_MODE3_DIRECT);
        }

        for(int led = 0; led < 5; led++)
        {
            bus->i2c_smbus_write_byte_data(dev, red_base    + (3 * led), red  );
            bus->i2c_smbus_write_byte_data(dev, green_base  + (3 * led), green);
            bus->i2c_smbus_write_byte_data(dev, blue_base   + (3 * led), blue );
            bus->i2c_smbus_write_byte_data(dev, bright_base + (3 * led), 0x64 );
        }
    }

    bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_APPLY, 0x02);
    bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_APPLY, 0x03);
}

void HyperXController::SetLEDColor(unsigned int slot, unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char red_base    = HYPERX_REG_SLOT0_LED0_RED        + (0x20 * slot);
    unsigned char green_base  = HYPERX_REG_SLOT0_LED0_GREEN      + (0x20 * slot);
    unsigned char blue_base   = HYPERX_REG_SLOT0_LED0_BLUE       + (0x20 * slot);
    unsigned char bright_base = HYPERX_REG_SLOT0_LED0_BRIGHTNESS + (0x20 * slot);

    bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_APPLY, 0x01);

    bus->i2c_smbus_write_byte_data(dev, red_base    + (3 * led), red  );
    bus->i2c_smbus_write_byte_data(dev, green_base  + (3 * led), green);
    bus->i2c_smbus_write_byte_data(dev, blue_base   + (3 * led), blue );
    bus->i2c_smbus_write_byte_data(dev, bright_base + (3 * led), 0x64 );

    bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_APPLY, 0x02);
    bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_APPLY, 0x03);
}

void HyperXController::SetMode(unsigned char new_mode)
{
    mode = new_mode;
    
    bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_APPLY, 0x01);

    switch (mode)
    {
    case HYPERX_MODE_DIRECT:
        bus->i2c_smbus_write_byte_data(dev, HYPERX_REG_MODE3, HYPERX_MODE3_DIRECT);
        break;

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
