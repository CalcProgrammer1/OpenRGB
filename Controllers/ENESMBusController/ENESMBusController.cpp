/*-----------------------------------------*\
|  ENESMBusController.cpp                   |
|                                           |
|  Driver for ENE SMBus RGB lighting        |
|  controller                               |
|                                           |
|  Formerly known as ASUS Aura SMBus. ASUS  |
|  AURA chips are rebranded ENE controllers |
|                                           |
|  Adam Honse (CalcProgrammer1) 8/19/2018   |
\*-----------------------------------------*/

#include "ENESMBusController.h"
#include "LogManager.h"
#include <cstring>

static const char* ene_channels[] =                 /* ENE channel strings                  */
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

ENESMBusController::ENESMBusController(i2c_smbus_interface* bus, ene_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;

    UpdateDeviceName();

    // Read the device configuration table
    for (int i = 0; i < 64; i++)
    {
        config_table[i] = ENERegisterRead(ENE_REG_CONFIG_TABLE + i);
    }

    /*-----------------------------------------------------------------*\
    | If this is running with TRACE or higher loglevel then             |
    |   dump the entire Feature list to log                             |
    \*-----------------------------------------------------------------*/
    if(LogManager::get()->getLoglevel() >= LL_TRACE)
    {
        LOG_TRACE("[ENE SMBus] ENE config table for 0x%02X:", dev);
        LOG_TRACE("    %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X", config_table[0],  config_table[1],  config_table[2],  config_table[3],
                                                                                                         config_table[4],  config_table[5],  config_table[6],  config_table[7],
                                                                                                         config_table[8],  config_table[9],  config_table[10], config_table[11],
                                                                                                         config_table[12], config_table[13], config_table[14], config_table[15]);

        LOG_TRACE("    %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X", config_table[16], config_table[17], config_table[18], config_table[19],
                                                                                                         config_table[20], config_table[21], config_table[22], config_table[23],
                                                                                                         config_table[24], config_table[25], config_table[26], config_table[27],
                                                                                                         config_table[28], config_table[29], config_table[30], config_table[31]);

        LOG_TRACE("    %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X", config_table[32], config_table[33], config_table[34], config_table[35],
                                                                                                         config_table[36], config_table[37], config_table[38], config_table[39],
                                                                                                         config_table[40], config_table[41], config_table[42], config_table[43],
                                                                                                         config_table[44], config_table[45], config_table[46], config_table[47]);

        LOG_TRACE("    %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X", config_table[48], config_table[49], config_table[50], config_table[51],
                                                                                                         config_table[52], config_table[53], config_table[54], config_table[55],
                                                                                                         config_table[56], config_table[57], config_table[58], config_table[59],
                                                                                                         config_table[60], config_table[61], config_table[62], config_table[63]);
    }

    // Read LED count from configuration table
    led_count = config_table[ENE_CONFIG_LED_COUNT];

    // LED-0116 - First generation motherboard controller
    if (strcmp(device_name, "LED-0116") == 0)
    {
        direct_reg  = ENE_REG_COLORS_DIRECT;
        effect_reg  = ENE_REG_COLORS_EFFECT;
        channel_cfg = ENE_CONFIG_CHANNEL_V1;
    }
    // DIMM_LED-0102 - First generation DRAM controller (Trident Z RGB)
    else if (strcmp(device_name, "DIMM_LED-0102") == 0)
    {
        direct_reg  = ENE_REG_COLORS_DIRECT;
        effect_reg  = ENE_REG_COLORS_EFFECT;
        channel_cfg = ENE_CONFIG_CHANNEL_V1;
    }
    // AUDA0-E6K5-0101 - Second generation DRAM controller (Geil Super Luce)
    else if (strcmp(device_name, "AUDA0-E6K5-0101") == 0)
    {
        direct_reg  = ENE_REG_COLORS_DIRECT_V2;
        effect_reg  = ENE_REG_COLORS_EFFECT_V2;
        channel_cfg = ENE_CONFIG_CHANNEL_V1;
    }
    // AUMA0-E6K5-0106 - Second generation motherboard controller
    else if (strcmp(device_name, "AUMA0-E6K5-0106") == 0)
    {
        direct_reg  = ENE_REG_COLORS_DIRECT_V2;
        effect_reg  = ENE_REG_COLORS_EFFECT_V2;
        channel_cfg = ENE_CONFIG_CHANNEL_V2;
    }
    // AUMA0-E6K5-0105 - Second generation motherboard controller
    else if (strcmp(device_name, "AUMA0-E6K5-0105") == 0)
    {
        direct_reg  = ENE_REG_COLORS_DIRECT_V2;
        effect_reg  = ENE_REG_COLORS_EFFECT_V2;
        channel_cfg = ENE_CONFIG_CHANNEL_V2;
    }
    // AUMA0-E6K5-0104 - Second generation motherboard controller
    else if (strcmp(device_name, "AUMA0-E6K5-0104") == 0)
    {
        direct_reg  = ENE_REG_COLORS_DIRECT_V2;
        effect_reg  = ENE_REG_COLORS_EFFECT_V2;
        channel_cfg = ENE_CONFIG_CHANNEL_V2;
    }
    // AUMA0-E8K4-0101 - First generation motherboard controller
    else if (strcmp(device_name, "AUMA0-E8K4-0101") == 0)
    {
        direct_reg  = ENE_REG_COLORS_DIRECT;
        effect_reg  = ENE_REG_COLORS_EFFECT;
        channel_cfg = ENE_CONFIG_CHANNEL_V1;
    }
    // AUMA0-E6K5-0107 - Second generation GPU controller
    else if (strcmp(device_name, "AUMA0-E6K5-0107") == 0)
    {
        direct_reg  = ENE_REG_COLORS_DIRECT_V2;
        effect_reg  = ENE_REG_COLORS_EFFECT_V2;
        channel_cfg = ENE_CONFIG_CHANNEL_V2;

        // Read LED count from configuration table
        led_count = config_table[ENE_CONFIG_LED_COUNT_0107];
    }
    // Assume first generation controller if string does not match
    else
    {
        direct_reg  = ENE_REG_COLORS_DIRECT;
        effect_reg  = ENE_REG_COLORS_EFFECT;
        channel_cfg = ENE_CONFIG_CHANNEL_V1;
    }
}

ENESMBusController::~ENESMBusController()
{

}

std::string ENESMBusController::GetDeviceName()
{
    return(device_name);
}

std::string ENESMBusController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

unsigned char ENESMBusController::GetChannel(unsigned int led)
{
    return(config_table[channel_cfg + led]);
}

const char * ENESMBusController::GetChannelName(unsigned int led)
{
    switch (config_table[channel_cfg + led])
    {
    case (unsigned char)ENE_LED_CHANNEL_AUDIO:
        return(ene_channels[0]);
        break;

    case (unsigned char)ENE_LED_CHANNEL_BACKPLATE:
        return(ene_channels[1]);
        break;

    case (unsigned char)ENE_LED_CHANNEL_BACK_IO:
        return(ene_channels[2]);
        break;

    case (unsigned char)ENE_LED_CHANNEL_CENTER:
        return(ene_channels[3]);
        break;

    case (unsigned char)ENE_LED_CHANNEL_CENTER_START:
        return(ene_channels[4]);
        break;

    case (unsigned char)ENE_LED_CHANNEL_DRAM:
        return(ene_channels[5]);
        break;
    
    case (unsigned char)ENE_LED_CHANNEL_PCIE:
        return(ene_channels[6]);
        break;

    case (unsigned char)ENE_LED_CHANNEL_RGB_HEADER:
        return(ene_channels[7]);
        break;

    case (unsigned char)ENE_LED_CHANNEL_RGB_HEADER_2:
        return(ene_channels[8]);
        break;

    case (unsigned char)ENE_LED_CHANNEL_RGB_HEADER_3:
        return(ene_channels[9]);
        break;

    default:
        return(ene_channels[10]);
        break;
    }
}

unsigned int ENESMBusController::GetLEDCount()
{
    return(led_count);
}

unsigned char ENESMBusController::GetLEDRed(unsigned int led)
{
    return(ENERegisterRead(direct_reg + ( 3 * led )));
}

unsigned char ENESMBusController::GetLEDGreen(unsigned int led)
{
    return(ENERegisterRead(direct_reg + ( 3 * led ) + 2));
}

unsigned char ENESMBusController::GetLEDBlue(unsigned int led)
{
    return(ENERegisterRead(direct_reg + ( 3 * led ) + 1));
}

unsigned char ENESMBusController::GetLEDRedEffect(unsigned int led)
{
    return(ENERegisterRead(effect_reg + ( 3 * led )));
}

unsigned char ENESMBusController::GetLEDGreenEffect(unsigned int led)
{
    return(ENERegisterRead(effect_reg + ( 3 * led ) + 2));
}

unsigned char ENESMBusController::GetLEDBlueEffect(unsigned int led)
{
    return(ENERegisterRead(effect_reg + ( 3 * led ) + 1));
}

void ENESMBusController::SaveMode()
{
    ENERegisterWrite(ENE_REG_APPLY, ENE_SAVE_VAL);
}

void ENESMBusController::SetAllColorsDirect(unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char* colors = new unsigned char[led_count * 3];

    for (unsigned int i = 0; i < (led_count * 3); i += 3)
    {
        colors[i + 0] = red;
        colors[i + 1] = blue;
        colors[i + 2] = green;
    }

    ENERegisterWriteBlock(direct_reg, colors, led_count * 3);

    delete[] colors;
}

void ENESMBusController::SetAllColorsEffect(unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char* colors = new unsigned char[led_count * 3];

    for (unsigned int i = 0; i < (led_count * 3); i += 3)
    {
        colors[i + 0] = red;
        colors[i + 1] = blue;
        colors[i + 2] = green;
    }

    ENERegisterWriteBlock(effect_reg, colors, led_count * 3);
    
    ENERegisterWrite(ENE_REG_APPLY, ENE_APPLY_VAL);

    delete[] colors;
}

void ENESMBusController::SetDirect(unsigned char direct)
{
    ENERegisterWrite(ENE_REG_DIRECT, direct);
    ENERegisterWrite(ENE_REG_APPLY, ENE_APPLY_VAL);
}

void ENESMBusController::SetLEDColorDirect(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char colors[3] = { red, blue, green };

    ENERegisterWriteBlock(direct_reg + ( 3 * led ), colors, 3);
}

void ENESMBusController::SetLEDColorEffect(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char colors[3] = { red, blue, green };

    ENERegisterWriteBlock(effect_reg + (3 * led), colors, 3);

    ENERegisterWrite(ENE_REG_APPLY, ENE_APPLY_VAL);
}

void ENESMBusController::SetMode(unsigned char mode, unsigned char speed, unsigned char direction)
{
    ENERegisterWrite(ENE_REG_MODE,      mode);
    ENERegisterWrite(ENE_REG_SPEED,     speed);
    ENERegisterWrite(ENE_REG_DIRECTION, direction);
    ENERegisterWrite(ENE_REG_APPLY,     ENE_APPLY_VAL);
}

void ENESMBusController::UpdateDeviceName()
{
    for (int i = 0; i < 16; i++)
    {
        device_name[i] = ENERegisterRead(ENE_REG_DEVICE_NAME + i);
    }
}

unsigned char ENESMBusController::ENERegisterRead(ene_register reg)
{
    //Write ENE register
    bus->i2c_smbus_write_word_data(dev, 0x00, ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF));

    //Read ENE value
    return(bus->i2c_smbus_read_byte_data(dev, 0x81));

}

void ENESMBusController::ENERegisterWrite(ene_register reg, unsigned char val)
{
    //Write ENE register
    bus->i2c_smbus_write_word_data(dev, 0x00, ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF));

    //Write ENE value
    bus->i2c_smbus_write_byte_data(dev, 0x01, val);

}

void ENESMBusController::ENERegisterWriteBlock(ene_register reg, unsigned char * data, unsigned char sz)
{
    //Write ENE register
    bus->i2c_smbus_write_word_data(dev, 0x00, ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF));

    //Write ENE block data
    bus->i2c_smbus_write_block_data(dev, 0x03, sz, data);

}
