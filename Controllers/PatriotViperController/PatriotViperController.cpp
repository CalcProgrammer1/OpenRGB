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
    return("I2C: " + return_string);
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
    return(mode);
}

void PatriotViperController::SetEffectColor(unsigned char red, unsigned char green, unsigned char blue)
{
    ViperRegisterWrite(VIPER_REG_START, 0xFF, 0xFF, 0xFF);
    ViperRegisterWrite(VIPER_REG_STATIC, 0x04, 0x00, 0x00);

    ViperRegisterWrite(VIPER_REG_LED0_EFFECT_COLOR, red, blue, green);
    ViperRegisterWrite(VIPER_REG_LED1_EFFECT_COLOR, red, blue, green);
    ViperRegisterWrite(VIPER_REG_LED2_EFFECT_COLOR, red, blue, green);
    ViperRegisterWrite(VIPER_REG_LED3_EFFECT_COLOR, red, blue, green);
    ViperRegisterWrite(VIPER_REG_LED4_EFFECT_COLOR, red, blue, green);

    ViperRegisterWrite(VIPER_REG_MODE, mode, 0x00, speed);
    ViperRegisterWrite(VIPER_REG_MODE, 0xAA, 0x00, 0x00);
    ViperRegisterWrite(VIPER_REG_MODE, 0xFA, 0x00, 0x00);
}

void PatriotViperController::SetAllColors(unsigned char red, unsigned char green, unsigned char blue)
{
    ViperRegisterWrite(VIPER_REG_START, 0xFF, 0xFF, 0xFF);
    ViperRegisterWrite(VIPER_REG_STATIC, 0x04, 0x00, 0x00);

    ViperRegisterWrite(VIPER_REG_LED0_DIRECT_COLOR, red, blue, green);
    ViperRegisterWrite(VIPER_REG_LED1_DIRECT_COLOR, red, blue, green);
    ViperRegisterWrite(VIPER_REG_LED2_DIRECT_COLOR, red, blue, green);
    ViperRegisterWrite(VIPER_REG_LED3_DIRECT_COLOR, red, blue, green);
    ViperRegisterWrite(VIPER_REG_LED4_DIRECT_COLOR, red, blue, green);

    ViperRegisterWrite(VIPER_REG_APPLY, 0x01, 0x00, 0x00);
}

void PatriotViperController::SetLEDColor(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    ViperRegisterWrite(VIPER_REG_START, 0xFF, 0xFF, 0xFF);
    ViperRegisterWrite(VIPER_REG_STATIC, 0x04, 0x00, 0x00);

    ViperRegisterWrite(VIPER_REG_LED0_DIRECT_COLOR + led, red, blue, green);

    ViperRegisterWrite(VIPER_REG_APPLY, 0x01, 0x00, 0x00);
}

void PatriotViperController::SetLEDEffectColor(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    ViperRegisterWrite(VIPER_REG_START, 0xFF, 0xFF, 0xFF);
    ViperRegisterWrite(VIPER_REG_STATIC, 0x04, 0x00, 0x00);

    ViperRegisterWrite(VIPER_REG_LED0_EFFECT_COLOR + led, red, blue, green);

    ViperRegisterWrite(VIPER_REG_MODE, mode, 0x00, speed);
    ViperRegisterWrite(VIPER_REG_MODE, 0xAA, 0x00, 0x00);
    ViperRegisterWrite(VIPER_REG_MODE, 0xFA, 0x00, 0x00);
}

void PatriotViperController::SetLEDColor(unsigned int /*slot*/, unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    ViperRegisterWrite(VIPER_REG_START, 0xFF, 0xFF, 0xFF);
    ViperRegisterWrite(VIPER_REG_STATIC, 0x04, 0x00, 0x00);

    ViperRegisterWrite(VIPER_REG_LED0_DIRECT_COLOR + led, red, blue, green);

    ViperRegisterWrite(VIPER_REG_APPLY, 0x01, 0x00, 0x00);
}

void PatriotViperController::SetLEDEffectColor(unsigned int /*slot*/, unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    ViperRegisterWrite(VIPER_REG_START, 0xFF, 0xFF, 0xFF);
    ViperRegisterWrite(VIPER_REG_STATIC, 0x04, 0x00, 0x00);

    ViperRegisterWrite(VIPER_REG_LED0_EFFECT_COLOR + led, red, blue, green);

    ViperRegisterWrite(VIPER_REG_MODE, mode, 0x00, speed);
    ViperRegisterWrite(VIPER_REG_MODE, 0xAA, 0x00, 0x00);
    ViperRegisterWrite(VIPER_REG_MODE, 0xFA, 0x00, 0x00);
}

void PatriotViperController::SetMode(unsigned char new_mode, unsigned char new_speed)
{
    direct = false;
    mode   = new_mode;
    speed  = new_speed;

    ViperRegisterWrite(VIPER_REG_START, 0xFF, 0xFF, 0xFF);
    ViperRegisterWrite(VIPER_REG_STATIC, 0x04, 0x00, 0x00);
    ViperRegisterWrite(VIPER_REG_MODE, mode, 0x00, speed);
    ViperRegisterWrite(VIPER_REG_MODE, 0xAA, 0x00, 0x00);
    ViperRegisterWrite(VIPER_REG_MODE, 0xFA, 0x00, 0x00);
}

void PatriotViperController::SetDirect()
{
    direct = true;
    ViperRegisterWrite(VIPER_REG_START, 0xFF, 0xFF, 0xFF);
    ViperRegisterWrite(VIPER_REG_STATIC, 0x04, 0x00, 0x00);
    ViperRegisterWrite(VIPER_REG_APPLY, 0x01, 0x00, 0x00);
}

void PatriotViperController::ViperRegisterWrite(viper_register reg, unsigned char val0, unsigned char val1, unsigned char val2)
{
    bus->i2c_smbus_write_byte_data(dev, reg, val0);
    bus->i2c_smbus_write_byte_data(dev, val2, val1);
}
