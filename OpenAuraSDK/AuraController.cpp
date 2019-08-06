/*-----------------------------------------*\
|  AuraController.h                         |
|                                           |
|  Driver for ASUS Aura RGB lighting        |
|  controller                               |
|                                           |
|  Adam Honse (CalcProgrammer1) 8/19/2018   |
\*-----------------------------------------*/

#include "AuraController.h"
#include <cstring>

AuraController::AuraController(i2c_smbus_interface* bus, aura_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;

    AuraUpdateDeviceName();

    // LED-0116 - First generation motherboard controller
    if (strcmp(device_name, "LED-0116") == 0)
    {
        direct_reg = AURA_REG_COLORS_DIRECT;
        effect_reg = AURA_REG_COLORS_EFFECT;
        led_count  = 5;
    }
    // DIMM_LED-0102 - First generation DRAM controller (Trident Z RGB)
    else if (strcmp(device_name, "DIMM_LED-0102") == 0)
    {
        direct_reg = AURA_REG_COLORS_DIRECT;
        effect_reg = AURA_REG_COLORS_EFFECT;
        led_count  = 5;
    }
    // AUDA0-E6K5-0101 - Second generation DRAM controller (Geil Super Luce)
    else if (strcmp(device_name, "AUDA0-E6K5-0101") == 0)
    {
        direct_reg = AURA_REG_COLORS_DIRECT_V2;
        effect_reg = AURA_REG_COLORS_EFFECT_V2;
        led_count  = 5;
    }
    // AUMA0-E6K5-0106 - Second generation motherboard controller
    else if (strcmp(device_name, "AUMA0-E6K5-0106") == 0)
    {
        direct_reg = AURA_REG_COLORS_DIRECT_V2;
        effect_reg = AURA_REG_COLORS_EFFECT_V2;
        led_count = 5;
    }
    // AUMA0-E6K5-0105 - Second generation motherboard controller
    else if (strcmp(device_name, "AUMA0-E6K5-0105") == 0)
    {
        direct_reg = AURA_REG_COLORS_DIRECT_V2;
        effect_reg = AURA_REG_COLORS_EFFECT_V2;
        led_count = 5;
    }
    // AUMA0-E6K5-0104 - Second generation motherboard controller
    else if (strcmp(device_name, "AUMA0-E6K5-0104") == 0)
    {
        direct_reg = AURA_REG_COLORS_DIRECT_V2;
        effect_reg = AURA_REG_COLORS_EFFECT_V2;
        led_count = 5;
    }
    // Assume first generation controller if string does not match
    else
    {
        direct_reg = AURA_REG_COLORS_DIRECT;
        effect_reg = AURA_REG_COLORS_EFFECT;
        led_count = 5;
    }
    
}

AuraController::~AuraController()
{

}

char * AuraController::GetDeviceName()
{
    return(device_name);
}

unsigned int AuraController::GetLEDCount()
{
    return(led_count);
}

void AuraController::SetAllColorsDirect(unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char* colors = new unsigned char[led_count * 3];

    for (int i = 0; i < (led_count * 3); i += 3)
    {
        colors[i + 0] = red;
        colors[i + 1] = blue;
        colors[i + 2] = green;
    }

    AuraRegisterWriteBlock(direct_reg, colors, led_count * 3);

    delete colors;
}

void AuraController::SetAllColorsEffect(unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char* colors = new unsigned char[led_count * 3];

    for (int i = 0; i < (led_count * 3); i += 3)
    {
        colors[i + 0] = red;
        colors[i + 1] = blue;
        colors[i + 2] = green;
    }

    AuraRegisterWriteBlock(effect_reg, colors, led_count * 3);
    
    AuraRegisterWrite(AURA_REG_APPLY, AURA_APPLY_VAL);

    delete colors;
}

void AuraController::SetDirect(unsigned char direct)
{
    AuraRegisterWrite(AURA_REG_DIRECT, direct);
    AuraRegisterWrite(AURA_REG_APPLY, AURA_APPLY_VAL);
}

void AuraController::SetLEDColorDirect(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char colors[3] = { red, blue, green };

    AuraRegisterWriteBlock(direct_reg + ( 3 * led ), colors, 3);
}

void AuraController::SetLEDColorEffect(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char colors[3] = { red, blue, green };

    AuraRegisterWriteBlock(effect_reg + (3 * led), colors, 3);

    AuraRegisterWrite(AURA_REG_APPLY, AURA_APPLY_VAL);
}

void AuraController::SetMode(unsigned char mode)
{
    AuraRegisterWrite(AURA_REG_MODE, mode);
    AuraRegisterWrite(AURA_REG_APPLY, AURA_APPLY_VAL);
}

void AuraController::AuraUpdateDeviceName()
{
    for (int i = 0; i < 16; i++)
    {
        device_name[i] = AuraRegisterRead(AURA_REG_DEVICE_NAME + i);
    }
}

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
    //Write Aura register
    bus->i2c_smbus_write_word_data(dev, 0x00, ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF));

    //Write Aura block data
    bus->i2c_smbus_write_block_data(dev, 0x03, sz, data);

}