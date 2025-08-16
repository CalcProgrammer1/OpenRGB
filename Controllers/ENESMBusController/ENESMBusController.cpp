/*---------------------------------------------------------*\
| ENESMBusController.cpp                                    |
|                                                           |
|   Driver for ENE SMBus devices                            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                19 Aug 2018 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "ENESMBusController.h"
#include "LogManager.h"

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
    "SSD",
    "Unknown",
};

ENESMBusController::ENESMBusController(ENESMBusInterface* interface, ene_dev_id dev, std::string dev_name, device_type dev_type)
{
    this->interface     = interface;
    this->dev           = dev;
    this->name          = dev_name;
    this->type          = dev_type;
    supports_mode_14    = false;

    if(interface->GetInterfaceType() != ENE_INTERFACE_TYPE_ROG_ARION)
    {
        UpdateDeviceName();

        /*-------------------------------------------------*\
        | Read the device configuration table               |
        \*-------------------------------------------------*/
        for(int i = 0; i < 64; i++)
        {
            config_table[i] = ENERegisterRead(ENE_REG_CONFIG_TABLE + i);
        }

        /*-------------------------------------------------*\
        | If this is running with TRACE or higher loglevel  |
        | then dump the entire Feature list to log          |
        \*-------------------------------------------------*/
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
    }
    else
    {
        LOG_TRACE("[ENE SMBus] ROG STRIX ARION detected, filling in hard coded config table entries.", dev);
        memset(config_table, 0, sizeof(config_table));
        config_table[ENE_CONFIG_LED_COUNT] = 4;
        config_table[0x03] = 4;
        strcpy(device_version, "ROG STRIX ARION");
    }

    /*-----------------------------------------------------*\
    | Read LED count from configuration table               |
    \*-----------------------------------------------------*/
    led_count = config_table[ENE_CONFIG_LED_COUNT];

    /*-----------------------------------------------------*\
    | LED-0116 - First generation motherboard controller    |
    \*-----------------------------------------------------*/
    if(strcmp(device_version, "LED-0116") == 0)
    {
        direct_reg  = ENE_REG_COLORS_DIRECT;
        effect_reg  = ENE_REG_COLORS_EFFECT;
        channel_cfg = ENE_CONFIG_CHANNEL_V1;
    }
    /*-----------------------------------------------------*\
    | DIMM_LED-0102 - First generation DRAM controller      |
    | (Trident Z RGB)                                       |
    \*-----------------------------------------------------*/
    else if(strcmp(device_version, "DIMM_LED-0102") == 0)
    {
        direct_reg  = ENE_REG_COLORS_DIRECT;
        effect_reg  = ENE_REG_COLORS_EFFECT;
        channel_cfg = ENE_CONFIG_CHANNEL_V1;
    }
    /*-----------------------------------------------------*\
    | AUDA0-E6K5-0101 - Second generation DRAM controller   |
    | (Geil Super Luce)                                     |
    \*-----------------------------------------------------*/
    else if(strcmp(device_version, "AUDA0-E6K5-0101") == 0)
    {
        direct_reg  = ENE_REG_COLORS_DIRECT_V2;
        effect_reg  = ENE_REG_COLORS_EFFECT_V2;
        channel_cfg = ENE_CONFIG_CHANNEL_V1;

        /*-------------------------------------------------*\
        | Check for Mode 14 support, only known to exist on |
        | modules where the DRAM 3 zone ID exists           |
        \*-------------------------------------------------*/
        for(std::size_t cfg_zone_idx = 0; cfg_zone_idx < ENE_NUM_ZONES; cfg_zone_idx++)
        {
            if(config_table[channel_cfg + cfg_zone_idx] == (unsigned char)ENE_LED_CHANNEL_DRAM_3)
            {
                supports_mode_14 = true;
                break;
            }
        }
    }
    /*-----------------------------------------------------*\
    | AUMA0-E6K5-0106 - Second generation motherboard       |
    | controller                                            |
    \*-----------------------------------------------------*/
    else if(strcmp(device_version, "AUMA0-E6K5-0106") == 0)
    {
        direct_reg  = ENE_REG_COLORS_DIRECT_V2;
        effect_reg  = ENE_REG_COLORS_EFFECT_V2;
        channel_cfg = ENE_CONFIG_CHANNEL_V2;
    }
    /*-----------------------------------------------------*\
    | AUMA0-E6K5-0105 - Second generation motherboard       |
    | controller                                            |
    \*-----------------------------------------------------*/
    else if(strcmp(device_version, "AUMA0-E6K5-0105") == 0)
    {
        direct_reg  = ENE_REG_COLORS_DIRECT_V2;
        effect_reg  = ENE_REG_COLORS_EFFECT_V2;
        channel_cfg = ENE_CONFIG_CHANNEL_V2;
    }
    /*-----------------------------------------------------*\
    | AUMA0-E6K5-0104 - Second generation motherboard       |
    | controller                                            |
    \*-----------------------------------------------------*/
    else if(strcmp(device_version, "AUMA0-E6K5-0104") == 0)
    {
        direct_reg  = ENE_REG_COLORS_DIRECT_V2;
        effect_reg  = ENE_REG_COLORS_EFFECT_V2;
        channel_cfg = ENE_CONFIG_CHANNEL_V2;
    }
    /*-----------------------------------------------------*\
    | AUMA0-E8K4-0101 - First generation motherboard        |
    | controller                                            |
    \*-----------------------------------------------------*/
    else if(strcmp(device_version, "AUMA0-E8K4-0101") == 0)
    {
        direct_reg  = ENE_REG_COLORS_DIRECT;
        effect_reg  = ENE_REG_COLORS_EFFECT;
        channel_cfg = ENE_CONFIG_CHANNEL_V1;
    }
    /*-----------------------------------------------------*\
    | AUMA0-E6K5-0107 - Second generation GPU controller    |
    \*-----------------------------------------------------*/
    else if(strcmp(device_version, "AUMA0-E6K5-0107") == 0)
    {
        direct_reg  = ENE_REG_COLORS_DIRECT_V2;
        effect_reg  = ENE_REG_COLORS_EFFECT_V2;
        channel_cfg = ENE_CONFIG_CHANNEL_V2;

        /*-------------------------------------------------*\
        | Read LED count from configuration table           |
        \*-------------------------------------------------*/
        led_count = config_table[ENE_CONFIG_LED_COUNT_0107];
    }
    /*-----------------------------------------------------*\
    | AUMA0-E6K5-1110 - Third generation GPU controller?    |
    | found an ASUS ROG Strix 4080 OC, seems to be equal to |
    | AUMA0-E6K5-0107                                       |
    \*-----------------------------------------------------*/
    else if(strcmp(device_version, "AUMA0-E6K5-1110") == 0)
    {
        direct_reg  = ENE_REG_COLORS_DIRECT_V2;
        effect_reg  = ENE_REG_COLORS_EFFECT_V2;
        channel_cfg = ENE_CONFIG_CHANNEL_V2;

        /*-------------------------------------------------*\
        | Read LED count from configuration table           |
        \*-------------------------------------------------*/
        led_count = config_table[ENE_CONFIG_LED_COUNT_1110];
    }
    /*-----------------------------------------------------*\
    | AUMA0-E6K5-1111 - Fourth generation GPU controller?   |
    | found on ASUS ROG Strix 4090 OC EVA-02 Edition, seems |
    | to be equal to AUMA0-E6K5-0107                        |
    \*-----------------------------------------------------*/
    else if(strcmp(device_version, "AUMA0-E6K5-1111") == 0)
    {
        direct_reg  = ENE_REG_COLORS_DIRECT_V2;
        effect_reg  = ENE_REG_COLORS_EFFECT_V2;
        channel_cfg = ENE_CONFIG_CHANNEL_V2;

        /*-------------------------------------------------*\
        | Read LED count from configuration table           |
        \*-------------------------------------------------*/
        led_count = config_table[ENE_CONFIG_LED_COUNT_0107];
    }
    /*-----------------------------------------------------*\
    | AUMA0-E6K5-1107 - Second generation GPU controller    |
    | Found on ASUS TUF 4070 TI OC, seems to be equal to    |
    | AUMA0-E6K5-0107                                       |
    \*-----------------------------------------------------*/
    else if(strcmp(device_version, "AUMA0-E6K5-1107") == 0)
    {
        direct_reg  = ENE_REG_COLORS_DIRECT_V2;
        effect_reg  = ENE_REG_COLORS_EFFECT_V2;
        channel_cfg = ENE_CONFIG_CHANNEL_V2;

        /*-------------------------------------------------*\
        | Read LED count from configuration table           |
        \*-------------------------------------------------*/
        led_count = config_table[ENE_CONFIG_LED_COUNT_0107];
    }
    /*-----------------------------------------------------*\
    | AUMA0-E6K5-0008                                       |
    | Found on ASUS STRIX 4070 Super OC                     |
    \*-----------------------------------------------------*/
    else if(strcmp(device_version, "AUMA0-E6K5-0008") == 0)
    {
        direct_reg  = ENE_REG_COLORS_DIRECT_V2;
        effect_reg  = ENE_REG_COLORS_EFFECT_V2;
        channel_cfg = ENE_CONFIG_CHANNEL_V1;

        /*-------------------------------------------------*\
        | Read LED count from configuration table           |
        \*-------------------------------------------------*/
        led_count = config_table[ENE_CONFIG_LED_COUNT_0107];
    }
    /*-----------------------------------------------------*\
    | AUMA0-E6K5-1113                                       |
    | Found on ASUS ASTRAL 5080 OC                          |
    \*-----------------------------------------------------*/
    else if(strcmp(device_version, "AUMA0-E6K5-1113") == 0)
    {
        direct_reg  = ENE_REG_COLORS_DIRECT_V2;
        effect_reg  = ENE_REG_COLORS_EFFECT_V2;
        channel_cfg = ENE_CONFIG_CHANNEL_V2;

        /*-------------------------------------------------*\
        | Read LED count from configuration table           |
        \*-------------------------------------------------*/
        led_count = config_table[ENE_CONFIG_LED_COUNT_0107];
    }
    /*-----------------------------------------------------*\
    | ROG ARION - ASUS ROG Arion external SSD enclosure     |
    | This device does not support ENE read, so we fake the |
    | device name string if the interface is ROG Arion type.|
    | It uses second generation registers.                  |
    \*-----------------------------------------------------*/
    else if(strcmp(device_version, "ROG STRIX ARION") == 0)
    {
        direct_reg  = ENE_REG_COLORS_DIRECT_V2;
        effect_reg  = ENE_REG_COLORS_EFFECT_V2;
        channel_cfg = ENE_CONFIG_CHANNEL_V2;
    }
    /*-----------------------------------------------------*\
    | Assume first generation controller if string does not |
    | match                                                 |
    \*-----------------------------------------------------*/
    else
    {
        direct_reg  = ENE_REG_COLORS_DIRECT;
        effect_reg  = ENE_REG_COLORS_EFFECT;
        channel_cfg = ENE_CONFIG_CHANNEL_V1;
    }
}

ENESMBusController::~ENESMBusController()
{
    delete interface;
}

std::string ENESMBusController::GetLocation()
{
    std::string return_string = interface->GetLocation();

    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);

    return(return_string);
}

std::string ENESMBusController::GetName()
{
    return(name);
}

std::string ENESMBusController::GetVersion()
{
    return(device_version);
}

device_type ENESMBusController::GetType()
{
    return(type);
}

const char * ENESMBusController::GetChannelName(unsigned int cfg_zone)
{
    LOG_TRACE("[%s] Config table for zone %02d: %02d", device_version, cfg_zone, config_table[channel_cfg + cfg_zone]);

    if(interface->GetInterfaceType() == ENE_INTERFACE_TYPE_ROG_ARION)
    {
        return(ene_channels[10]);
    }
    else
    {
        switch(config_table[channel_cfg + cfg_zone])
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
            case (unsigned char)ENE_LED_CHANNEL_DRAM_2:
            case (unsigned char)ENE_LED_CHANNEL_DRAM_3:
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
                return(ene_channels[11]);
                break;
        }
    }
}

unsigned int ENESMBusController::GetLEDCount(unsigned int cfg_zone)
{
    LOG_TRACE("[%s] LED Count for zone %02d: %02d", device_version, cfg_zone, config_table[0x03 + cfg_zone]);
    return(config_table[0x03 + cfg_zone]);
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

void ENESMBusController::SetAllColorsDirect(RGBColor* colors)
{
    unsigned char* color_buf   = new unsigned char[led_count * 3];
    unsigned int   bytes_sent  = 0;

    for(unsigned int i = 0; i < (led_count * 3); i += 3)
    {
        color_buf[i + 0] = RGBGetRValue(colors[i / 3]);
        color_buf[i + 1] = RGBGetBValue(colors[i / 3]);
        color_buf[i + 2] = RGBGetGValue(colors[i / 3]);
    }

    while(bytes_sent < (led_count * 3))
    {
        int bytes_to_send = (led_count * 3) - bytes_sent;

        if(bytes_to_send > interface->GetMaxBlock())
        {
            bytes_to_send = interface->GetMaxBlock();
        }

        ENERegisterWriteBlock(direct_reg + bytes_sent, &color_buf[bytes_sent], bytes_to_send);

        bytes_sent += bytes_to_send;
    }

    delete[] color_buf;
}

void ENESMBusController::SetAllColorsEffect(RGBColor* colors)
{
    unsigned char* color_buf   = new unsigned char[led_count * 3];
    unsigned int   bytes_sent  = 0;

    for(unsigned int i = 0; i < (led_count * 3); i += 3)
    {
        color_buf[i + 0] = RGBGetRValue(colors[i / 3]);
        color_buf[i + 1] = RGBGetBValue(colors[i / 3]);
        color_buf[i + 2] = RGBGetGValue(colors[i / 3]);
    }

    while(bytes_sent < (led_count * 3))
    {
        int bytes_to_send = (led_count * 3) - bytes_sent;

        if(bytes_to_send > interface->GetMaxBlock())
        {
            bytes_to_send = interface->GetMaxBlock();
        }

        ENERegisterWriteBlock(effect_reg + bytes_sent, &color_buf[bytes_sent], bytes_to_send);

        bytes_sent += bytes_to_send;
    }

    ENERegisterWrite(ENE_REG_APPLY, ENE_APPLY_VAL);

    delete[] color_buf;
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

bool ENESMBusController::SupportsMode14()
{
    return(supports_mode_14);
}

void ENESMBusController::UpdateDeviceName()
{
    for (int i = 0; i < 16; i++)
    {
        device_version[i] = ENERegisterRead(ENE_REG_DEVICE_NAME + i);
    }
}

unsigned char ENESMBusController::ENERegisterRead(ene_register reg)
{
    return(interface->ENERegisterRead(dev, reg));
}

void ENESMBusController::ENERegisterWrite(ene_register reg, unsigned char val)
{
    interface->ENERegisterWrite(dev, reg, val);
}

void ENESMBusController::ENERegisterWriteBlock(ene_register reg, unsigned char * data, unsigned char sz)
{
    interface->ENERegisterWriteBlock(dev, reg, data, sz);
}
