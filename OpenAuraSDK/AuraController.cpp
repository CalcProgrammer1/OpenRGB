/*-----------------------------------------*\
|  AuraController.h                         |
|                                           |
|  Driver for ASUS Aura RGB lighting        |
|  controller                               |
|                                           |
|  Adam Honse (CalcProgrammer1) 8/19/2018   |
\*-----------------------------------------*/

#include "AuraController.h"

AuraController::AuraController(i2c_smbus_interface* bus, aura_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;

    AuraUpdateDeviceName();

    led_count = 5;  //temporary, some controllers have more channels
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

    AuraRegisterWriteBlock(AURA_REG_COLORS_DIRECT, colors, led_count * 3);

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

    AuraRegisterWriteBlock(AURA_REG_COLORS_EFFECT, colors, led_count * 3);
    
    AuraRegisterWrite(AURA_REG_APPLY, AURA_APPLY_VAL);

    delete colors;
}

void AuraController::SetLEDColorDirect(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char colors[3] = { red, blue, green };

    AuraRegisterWriteBlock(AURA_REG_COLORS_DIRECT + ( 3 * led ), colors, 3);
}

void AuraController::SetLEDColorEffect(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char colors[3] = { red, blue, green };

    AuraRegisterWriteBlock(AURA_REG_COLORS_EFFECT + (3 * led), colors, 3);

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