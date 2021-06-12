/*-----------------------------------------*\
|  AsusAuraSMBusController.cpp              |
|                                           |
|  Driver for ASUS Aura RGB lighting        |
|  controller                               |
|                                           |
|  Adam Honse (CalcProgrammer1) 8/19/2018   |
\*-----------------------------------------*/

#include "AsusAuraSMBusController.h"
#include <cstring>

static const char* aura_channels[] =                /* Aura channel strings                 */
{
    "Audio",
    "Backplate",
    "Back I/O",
    "Center",
    "Center",
    "DRAM",
    "PCIe",
    "RGB Header",
    "RGB Header 2",
    "RGB Header",
    "Unknown",
};

AuraSMBusController::AuraSMBusController(i2c_smbus_interface* bus, aura_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;

    AuraUpdateDeviceName();

    // Read the device configuration table
    for (int i = 0; i < 64; i++)
    {
        config_table[i] = AuraRegisterRead(AURA_REG_CONFIG_TABLE + i);
    }

    // Read LED count from configuration table
    led_count = config_table[AURA_CONFIG_LED_COUNT];

    // LED-0116 - First generation motherboard controller
    if (strcmp(device_name, "LED-0116") == 0)
    {
        direct_reg  = AURA_REG_COLORS_DIRECT;
        effect_reg  = AURA_REG_COLORS_EFFECT;
        channel_cfg = AURA_CONFIG_CHANNEL_V1;
    }
    // DIMM_LED-0102 - First generation DRAM controller (Trident Z RGB)
    else if (strcmp(device_name, "DIMM_LED-0102") == 0)
    {
        direct_reg  = AURA_REG_COLORS_DIRECT;
        effect_reg  = AURA_REG_COLORS_EFFECT;
        channel_cfg = AURA_CONFIG_CHANNEL_V1;
    }
    // AUDA0-E6K5-0101 - Second generation DRAM controller (Geil Super Luce)
    else if (strcmp(device_name, "AUDA0-E6K5-0101") == 0)
    {
        direct_reg  = AURA_REG_COLORS_DIRECT_V2;
        effect_reg  = AURA_REG_COLORS_EFFECT_V2;
        channel_cfg = AURA_CONFIG_CHANNEL_V1;
    }
    // AUMA0-E6K5-0106 - Second generation motherboard controller
    else if (strcmp(device_name, "AUMA0-E6K5-0106") == 0)
    {
        direct_reg  = AURA_REG_COLORS_DIRECT_V2;
        effect_reg  = AURA_REG_COLORS_EFFECT_V2;
        channel_cfg = AURA_CONFIG_CHANNEL_V2;
    }
    // AUMA0-E6K5-0105 - Second generation motherboard controller
    else if (strcmp(device_name, "AUMA0-E6K5-0105") == 0)
    {
        direct_reg  = AURA_REG_COLORS_DIRECT_V2;
        effect_reg  = AURA_REG_COLORS_EFFECT_V2;
        channel_cfg = AURA_CONFIG_CHANNEL_V2;
    }
    // AUMA0-E6K5-0104 - Second generation motherboard controller
    else if (strcmp(device_name, "AUMA0-E6K5-0104") == 0)
    {
        direct_reg = AURA_REG_COLORS_DIRECT_V2;
        effect_reg = AURA_REG_COLORS_EFFECT_V2;
        channel_cfg = AURA_CONFIG_CHANNEL_V2;
    }
    // AUMA0-E8K4-0101 - Second generation motherboard controller
    else if (strcmp(device_name, "AUMA0-E8K4-0101") == 0)
    {
        direct_reg = AURA_REG_COLORS_DIRECT_V2;
        effect_reg = AURA_REG_COLORS_EFFECT_V2;
        channel_cfg = AURA_CONFIG_CHANNEL_V2;
    }
    // Assume first generation controller if string does not match
    else
    {
        direct_reg  = AURA_REG_COLORS_DIRECT;
        effect_reg  = AURA_REG_COLORS_EFFECT;
        channel_cfg = AURA_CONFIG_CHANNEL_V1;
    }
}

AuraSMBusController::~AuraSMBusController()
{

}

std::string AuraSMBusController::GetDeviceName()
{
    return(device_name);
}

std::string AuraSMBusController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

unsigned char AuraSMBusController::GetChannel(unsigned int led)
{
    return(config_table[channel_cfg + led]);
}

const char * AuraSMBusController::GetChannelName(unsigned int led)
{
    switch (config_table[channel_cfg + led])
    {
    case (unsigned char)AURA_LED_CHANNEL_AUDIO:
        return(aura_channels[0]);
        break;

    case (unsigned char)AURA_LED_CHANNEL_BACKPLATE:
        return(aura_channels[1]);
        break;

    case (unsigned char)AURA_LED_CHANNEL_BACK_IO:
        return(aura_channels[2]);
        break;

    case (unsigned char)AURA_LED_CHANNEL_CENTER:
        return(aura_channels[3]);
        break;

    case (unsigned char)AURA_LED_CHANNEL_CENTER_START:
        return(aura_channels[4]);
        break;

    case (unsigned char)AURA_LED_CHANNEL_DRAM:
        return(aura_channels[5]);
        break;
    
    case (unsigned char)AURA_LED_CHANNEL_PCIE:
        return(aura_channels[6]);
        break;

    case (unsigned char)AURA_LED_CHANNEL_RGB_HEADER:
        return(aura_channels[7]);
        break;

    case (unsigned char)AURA_LED_CHANNEL_RGB_HEADER_2:
        return(aura_channels[8]);
        break;

    case (unsigned char)AURA_LED_CHANNEL_RGB_HEADER_3:
        return(aura_channels[9]);
        break;

    default:
        return(aura_channels[10]);
        break;
    }
}

unsigned int AuraSMBusController::GetLEDCount()
{
    return(led_count);
}

unsigned char AuraSMBusController::GetLEDRed(unsigned int led)
{
    return(AuraRegisterRead(direct_reg + ( 3 * led )));
}

unsigned char AuraSMBusController::GetLEDGreen(unsigned int led)
{
    return(AuraRegisterRead(direct_reg + ( 3 * led ) + 2));
}

unsigned char AuraSMBusController::GetLEDBlue(unsigned int led)
{
    return(AuraRegisterRead(direct_reg + ( 3 * led ) + 1));
}

void AuraSMBusController::SetAllColorsDirect(RGBColor* colors)
{
    unsigned char* color_buf = new unsigned char[led_count * 3];

    for (unsigned int i = 0; i < (led_count * 3); i += 3)
    {
        color_buf[i + 0] = RGBGetRValue(colors[i / 3]);
        color_buf[i + 1] = RGBGetBValue(colors[i / 3]);
        color_buf[i + 2] = RGBGetGValue(colors[i / 3]);
    }

    AuraRegisterWriteBlock(direct_reg, color_buf, led_count * 3);

    delete[] color_buf;
}

void AuraSMBusController::SetAllColorsEffect(RGBColor* colors)
{
    unsigned char* color_buf = new unsigned char[led_count * 3];

    for (unsigned int i = 0; i < (led_count * 3); i += 3)
    {
        color_buf[i + 0] = RGBGetRValue(colors[i / 3]);
        color_buf[i + 1] = RGBGetBValue(colors[i / 3]);
        color_buf[i + 2] = RGBGetGValue(colors[i / 3]);
    }

    AuraRegisterWriteBlock(effect_reg, color_buf, led_count * 3);

    AuraRegisterWrite(AURA_REG_APPLY, AURA_APPLY_VAL);

    delete[] color_buf;
}

void AuraSMBusController::SetDirect(unsigned char direct)
{
    AuraRegisterWrite(AURA_REG_DIRECT, direct);
    AuraRegisterWrite(AURA_REG_APPLY, AURA_APPLY_VAL);
}

void AuraSMBusController::SetLEDColorDirect(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char colors[3] = { red, blue, green };

    AuraRegisterWriteBlock(direct_reg + ( 3 * led ), colors, 3);
}

void AuraSMBusController::SetLEDColorEffect(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char colors[3] = { red, blue, green };

    AuraRegisterWriteBlock(effect_reg + (3 * led), colors, 3);

    AuraRegisterWrite(AURA_REG_APPLY, AURA_APPLY_VAL);
}

void AuraSMBusController::SetMode(unsigned char mode)
{
    AuraRegisterWrite(AURA_REG_MODE, mode);
    AuraRegisterWrite(AURA_REG_APPLY, AURA_APPLY_VAL);
}

void AuraSMBusController::AuraUpdateDeviceName()
{
    for (int i = 0; i < 16; i++)
    {
        device_name[i] = AuraRegisterRead(AURA_REG_DEVICE_NAME + i);
    }
}

unsigned char AuraSMBusController::AuraRegisterRead(aura_register reg)
{
    //Write Aura register
    bus->i2c_smbus_write_word_data(dev, 0x00, ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF));

    //Read Aura value
    return(bus->i2c_smbus_read_byte_data(dev, 0x81));

}

void AuraSMBusController::AuraRegisterWrite(aura_register reg, unsigned char val)
{
    //Write Aura register
    bus->i2c_smbus_write_word_data(dev, 0x00, ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF));

    //Write Aura value
    bus->i2c_smbus_write_byte_data(dev, 0x01, val);

}

void AuraSMBusController::AuraRegisterWriteBlock(aura_register reg, unsigned char * data, unsigned char sz)
{
    //Write Aura register
    bus->i2c_smbus_write_word_data(dev, 0x00, ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF));

    //Write Aura block data
    bus->i2c_smbus_write_block_data(dev, 0x03, sz, data);

}
