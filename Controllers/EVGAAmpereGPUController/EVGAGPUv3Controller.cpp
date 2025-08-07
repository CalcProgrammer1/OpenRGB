/*---------------------------------------------------------*\
| EVGAGPUv3Controller.cpp                                   |
|                                                           |
|   Driver for EVGA V3 (Ampere) GPU                         |
|                                                           |
|   TheRogueZeta                                15 Jul 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "EVGAGPUv3Controller.h"
#include "LogManager.h"

EVGAGPUv3Controller::EVGAGPUv3Controller(i2c_smbus_interface* bus, evga_dev_id dev, std::string dev_name)
{
    this->bus   = bus;
    this->dev   = dev;
    this->name  = dev_name;
}

EVGAGPUv3Controller::~EVGAGPUv3Controller()
{

}

std::string EVGAGPUv3Controller::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char        addr[5];

    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

std::string EVGAGPUv3Controller::GetDeviceName()
{
    return(name);
}

void EVGAGPUv3Controller::GetDeviceModes()
{
    LOG_DEBUG("[%s] Getting Zone and LED count from HW", name.c_str());
    uint8_t data_pkt[I2C_SMBUS_BLOCK_MAX] = {};

    uint8_t result = bus->i2c_smbus_read_i2c_block_data(dev, EVGA_GPU_V3_REG_MODE, 10, data_pkt);
    if (result == 10)
    {
        for(uint8_t zone = 0 ; zone < 4; zone ++)
        {
            zone_modes[zone] = data_pkt[zone + 1];
            zone_led_count[zone] = data_pkt[zone + 5];
            LOG_DEBUG("[%s] Zone %1d LED count: %02d, mode: %02d", name.c_str(), zone + 1, zone_led_count[zone], zone_modes[zone]);
        }
        zone_sync = data_pkt[9];
        LOG_DEBUG("[%s] Zone Sync is %1d", name.c_str(), zone_sync);
    }
    else
    {
            LOG_DEBUG("[%s] Invalid block read result: %02d", name.c_str(), result);
            memset(zone_led_count, 0, sizeof(zone_led_count));
            memset(zone_modes, 0, sizeof(zone_modes));
    }
    initCard();
}

std::string EVGAGPUv3Controller::GetFWVersion()
{
    return(fwVersion);
}

std::string EVGAGPUv3Controller::ReadFWVersion()
{
    LOG_TRACE("[%s] Getting FW from HW", name.c_str());
    uint8_t         data_pkt[I2C_SMBUS_BLOCK_MAX] = {};
    std::string     return_string = "";
    char            version[10];

    uint8_t         result = bus->i2c_smbus_read_i2c_block_data(dev, EVGA_GPU_V3_REG_FIRMWARE, 6, data_pkt);
    if (result == 6)
    {
        uint8_t     major = data_pkt[4];
        uint8_t     minor = data_pkt[5];

        snprintf(version, 10, "1.%02d.%02d", major, minor);
        return_string.append(version);
        LOG_TRACE("[%s] Firmware %s", name.c_str(), version);
        fwVersion = return_string;
        return(return_string);
    }
    else
    {
        return "";
    }
}

uint8_t EVGAGPUv3Controller::GetZoneMode(uint8_t zone)
{
    return zone_modes[zone];
}

EVGAv3_config EVGAGPUv3Controller::GetZoneConfig(uint8_t zone, uint8_t mode)
{
    EVGAv3_config               zone_config;
    u16_to_u8                   speed16;
    bool                        readFail = false;

    zone_config.brightness      = EVGA_GPU_V3_BRIGHTNESS_DEFAULT;
    zone_config.direction       = 0;
    zone_config.numberOfColors  = 0;
    zone_config.speed           = EVGA_GPU_V3_SPEED_GENERIC_NORMAL;

    LOG_DEBUG("[%s] Retriving Zone %1d config for mode %1d from HW", name.c_str(), zone, mode);
    uint8_t data_pkt[I2C_SMBUS_BLOCK_MAX] = {};

    switch (mode)
    {
        case EVGA_GPU_V3_MODE_STATIC:
            {
                uint8_t result = bus->i2c_smbus_read_i2c_block_data(dev, EVGA_GPU_V3_REG_STATIC + zone, 5, data_pkt);
                if (result == 5)
                {
                    //Load data
                    zone_config.brightness      = data_pkt[1];
                    uint8_t red                 = data_pkt[2];
                    uint8_t green               = data_pkt[3];
                    uint8_t blue                = data_pkt[4];
                    zone_config.colors[0]       = ToRGBColor(red, green, blue);
                    zone_config.numberOfColors  = 1;
                    zone_config.speed           = 0;
                    zone_config.direction       = 0;
                }
                else
                {
                    readFail = true;
                }
            }
            break;

        case EVGA_GPU_V3_MODE_BREATHING:
            {
                uint8_t result = bus->i2c_smbus_read_i2c_block_data(dev, EVGA_GPU_V3_REG_BREATHING + zone, 10, data_pkt);
                if (result == 10)
                {
                    //Load data
                    zone_config.brightness      = data_pkt[1];
                    uint8_t red1                = data_pkt[2];
                    uint8_t green1              = data_pkt[3];
                    uint8_t blue1               = data_pkt[4];
                    zone_config.colors[0]       = ToRGBColor(red1, green1, blue1);
                    uint8_t red2                = data_pkt[5];
                    uint8_t green2              = data_pkt[6];
                    uint8_t blue2               = data_pkt[7];
                    zone_config.colors[1]       = ToRGBColor(red2, green2, blue2);
                    zone_config.numberOfColors= (zone_config.colors[1] != 0 ) ? 2 : 1 ;
                    speed16.lsb                 = data_pkt[8];
                    speed16.msb                 = data_pkt[9];
                    zone_config.speed           = speed16.u16;
                    zone_config.direction       = 0;
                }
                else
                {
                    readFail = true;
                }
            }
            break;

        case EVGA_GPU_V3_MODE_RAINBOW:
        case EVGA_GPU_V3_MODE_RAINBOW_WAVE:
        case EVGA_GPU_V3_MODE_STAR:
            {
                uint8_t result = bus->i2c_smbus_read_i2c_block_data(dev, EVGA_GPU_V3_REG_STATIC + ((mode -1) * 4) + zone, 4, data_pkt);
                if (result == 4)
                {
                    //Load data
                    zone_config.brightness      = data_pkt[1];
                    zone_config.numberOfColors  = 0;
                    speed16.lsb                 = data_pkt[2];
                    speed16.msb                 = data_pkt[3];
                    zone_config.speed           = speed16.u16;
                }
                else
                {
                    readFail = true;
                }
            }
            break;

        case EVGA_GPU_V3_MODE_WAVE:
            {
                uint8_t result = bus->i2c_smbus_read_i2c_block_data(dev, EVGA_GPU_V3_REG_STATIC + ((mode -1) * 4) + zone, 7, data_pkt);
                if (result == 7)
                {
                    //Load data
                    zone_config.brightness      = data_pkt[1];
                    uint8_t red                 = data_pkt[2];
                    uint8_t green               = data_pkt[3];
                    uint8_t blue                = data_pkt[4];
                    zone_config.colors[0]       = ToRGBColor(red, green, blue);
                    zone_config.numberOfColors  = 1;
                    speed16.lsb                 = data_pkt[5];
                    speed16.msb                 = data_pkt[6];
                    zone_config.speed           = speed16.u16;
                }
                else
                {
                    readFail = true;
                }
            }
            break;

        case EVGA_GPU_V3_MODE_COLOR_CYCLE:
        case EVGA_GPU_V3_MODE_COLOR_STACK:
            {
                uint8_t cd_pkt[5];

                if(mode == EVGA_GPU_V3_MODE_COLOR_STACK)
                {
                    uint8_t color_count = bus->i2c_smbus_read_i2c_block_data(dev, EVGA_GPU_V3_REG_COLOR_STACK_COLOR_COUNT, 5, cd_pkt);

                    if(color_count == 5)
                    {
                        zone_config.numberOfColors = cd_pkt[zone + 1];
                    }
                    else
                    {
                        readFail = true;
                    }

                    uint8_t direction = bus->i2c_smbus_read_i2c_block_data(dev, EVGA_GPU_V3_REG_COLOR_STACK_DIRECTION, 5, cd_pkt);
                    if(direction == 5)
                    {
                        zone_config.direction = cd_pkt[zone + 1];
                    }
                    else
                    {
                        readFail = true;
                    }

                }
                else
                {
                    uint8_t color_count = bus->i2c_smbus_read_i2c_block_data(dev, EVGA_GPU_V3_REG_COLOR_CYCLE_COUNT, 5, cd_pkt);

                    if(color_count == 5)
                    {
                        zone_config.numberOfColors = cd_pkt[zone + 1];
                    }
                    else
                    {
                        readFail = true;
                    }
                }

                uint8_t result = bus->i2c_smbus_read_i2c_block_data(dev, EVGA_GPU_V3_REG_STATIC + ((mode -1) * 4) + zone, 31, data_pkt);
                if (result == 31)
                {
                    zone_config.brightness      = data_pkt[1];
                    for(uint8_t color_index = 0; color_index < zone_config.numberOfColors; color_index++)
                    {
                        uint8_t red             = data_pkt[(color_index * 4) + 2];
                        uint8_t green           = data_pkt[(color_index * 4) + 3];
                        uint8_t blue            = data_pkt[(color_index * 4) + 4];
                        zone_config.colors[color_index] = ToRGBColor(red, green, blue);
                    }
                    speed16.lsb                 = data_pkt[29];
                    speed16.msb                 = data_pkt[30];
                    zone_config.speed           = speed16.u16;
                }
                else
                {
                    readFail = true;
                }
            }
            break;

        default:
            break;
    }

    if(readFail == false)
    {
        LOG_TRACE("[%s] Zone %1d Brightness: 0x%02X, Colors: %1d, Speed: 0x%04X, Direction %1d.", name.c_str(), zone, zone_config.brightness, zone_config.numberOfColors, zone_config.speed, zone_config.direction);
        for(uint8_t color_index = 0; color_index < zone_config.numberOfColors; color_index++)
        {
            LOG_TRACE("[%s] Color Index [%2d]: 0x%06X", name.c_str(), color_index, zone_config.colors[color_index]);
        }
        LOG_DEBUG("[%s] Done loading Zone %1d configuration from HW", name.c_str(), zone);
    }
    else
    {
        zone_config.direction       = 0;
        zone_config.numberOfColors  = 0;
        for(uint8_t i = 0; i < 7; i++)
        {
            zone_config.colors[i] = 0;
        }
        LOG_DEBUG("[%s] Failed while loading Zone %1d configuration from HW", name.c_str(), zone);
    }
    return zone_config;
}

void EVGAGPUv3Controller::initCard()
{
    // This command needs to be sent before the card will respond to OpenRGB commands
    // NvAPI_I2CWriteEx: Dev: 0x2D RegSize: 0x01 Reg: 0xB2 Size: 0x05 Data: 0x04 0xC6 0xEB 0xEA 0x15
    uint8_t data_pkt[5] = {0x04, 0xC6, 0xEB, 0xEA, 0x15};
    bus->i2c_smbus_write_i2c_block_data(dev, EVGA_GPU_V3_REG_ENABLE, sizeof(data_pkt), data_pkt);
    LOG_TRACE("[%s] Sending SW int packet", name.c_str());
    return;
}

void EVGAGPUv3Controller::SaveConfig()
{
    LOG_DEBUG("[%s] Sending save packet", name.c_str());

    //NvAPI_I2CWriteEx: Dev: 0x2D RegSize: 0x01 Reg: 0x90 Size: 0x05 Data: 0x04 0x9E 0xEB 0x00 0x90 //Sent on close of PX1
    uint8_t data_pkt[5] = {0x04, 0x9E, 0xEB, 0x00, 0x90};

    bus->i2c_smbus_write_i2c_block_data(dev, EVGA_GPU_V3_REG_SAVE, sizeof(data_pkt), data_pkt);
    return;
}

void EVGAGPUv3Controller::ResizeARGB(uint8_t newSize)
{
    if(newSize < EVGAGPUV3_LEDS_MIN)
    {
        newSize = EVGAGPUV3_LEDS_MIN;
    }
    else if(newSize > EVGAGPUV3_LEDS_MAX)
    {
        newSize = EVGAGPUV3_LEDS_MAX;
    }

    LOG_DEBUG("[%s] Resizing ARGB header with %02d size", name.c_str(), newSize);
    uint8_t data_pkt[EVGAGPUV3_MODE_PACKET_SIZE] = { 0x09, EVGAGPUV3_INIT, EVGAGPUV3_INIT, EVGAGPUV3_INIT, EVGAGPUV3_INIT,
                                                           EVGAGPUV3_INIT, EVGAGPUV3_INIT, EVGAGPUV3_INIT, newSize, EVGAGPUV3_INIT};

    bus->i2c_smbus_write_i2c_block_data(dev, EVGA_GPU_V3_REG_MODE, sizeof(data_pkt), data_pkt);
    return;
}

void EVGAGPUv3Controller::SetAllModes(uint8_t zone_0_mode, uint8_t zone_1_mode,uint8_t zone_2_mode,uint8_t zone_3_mode, bool sync)
{
    uint8_t     mode_pkt[EVGAGPUV3_MODE_PACKET_SIZE]    = { EVGAGPUV3_MODE_PACKET_SIZE - 1,
                                                        EVGAGPUV3_INIT, EVGAGPUV3_INIT, EVGAGPUV3_INIT, EVGAGPUV3_INIT,
                                                        EVGAGPUV3_INIT, EVGAGPUV3_INIT, EVGAGPUV3_INIT, EVGAGPUV3_INIT, 0x0 };

    // Hack to keep card in sync after power loss (standby) without requireing a rescan.
    // To be replaced with code that calls this once upon system resume once OpenRGB
    // knows about system power states.
    initCard();

    // Keep zone_modes in sync
    zone_modes[0] = zone_0_mode;
    zone_modes[1] = zone_1_mode;
    zone_modes[2] = zone_2_mode;
    zone_modes[3] = zone_3_mode;

    // Prep packet
    mode_pkt[1] = zone_0_mode;
    mode_pkt[2] = zone_1_mode;
    mode_pkt[3] = zone_2_mode;
    mode_pkt[4] = zone_3_mode;
    mode_pkt[9] = sync;

    bus->i2c_smbus_write_i2c_block_data(dev, EVGA_GPU_V3_REG_MODE, EVGAGPUV3_MODE_PACKET_SIZE, mode_pkt);
    //LOG_TRACE("[%s] Setting all zones to mode: %02d, %2d, %2d, %2d, zone sync %1d.", name.c_str(), zone_0_mode, zone_1_mode, zone_2_mode, zone_3_mode, sync);
}

void EVGAGPUv3Controller::SetZoneMode(uint8_t zone, uint8_t mode)
{
    uint8_t     mode_pkt[EVGAGPUV3_MODE_PACKET_SIZE]    = { EVGAGPUV3_MODE_PACKET_SIZE - 1,
                                                        EVGAGPUV3_INIT, EVGAGPUV3_INIT, EVGAGPUV3_INIT, EVGAGPUV3_INIT,
                                                        EVGAGPUV3_INIT, EVGAGPUV3_INIT, EVGAGPUV3_INIT, EVGAGPUV3_INIT, 0x0 };

    // Hack to keep card in sync after power loss (standby) without requireing a rescan.
    // To be replaced with code that calls this once upon system resume once OpenRGB
    // knows about system power states.
    initCard();

    // Keep zone_modes in sync
    zone_modes[zone] = mode;
    // Prep packet
    mode_pkt[zone + 1] = mode;

    bus->i2c_smbus_write_i2c_block_data(dev, EVGA_GPU_V3_REG_MODE, EVGAGPUV3_MODE_PACKET_SIZE, mode_pkt);
    //LOG_TRACE("[%s] Setting individual zone %1d to mode %02d", name.c_str(), zone, mode);
}

void EVGAGPUv3Controller::SetZone(uint8_t zone, uint8_t mode, EVGAv3_config zone_config)
{
    std::string mode_name;
    u16_to_u8   speed16                                 = { (uint16_t) zone_config.speed };

    switch (mode)
    {
    case EVGA_GPU_V3_MODE_OFF:
        break;

    case EVGA_GPU_V3_MODE_STATIC:
        {
            uint8_t zone_pkt[5]     = {EVGAGPUV3_INIT, EVGAGPUV3_INIT, EVGAGPUV3_INIT, EVGAGPUV3_INIT, EVGAGPUV3_INIT};

            zone_pkt[0]             = sizeof(zone_pkt) - 1;
            zone_pkt[1]             = zone_config.brightness;
            zone_pkt[2]             = RGBGetRValue(zone_config.colors[0]);
            zone_pkt[3]             = RGBGetGValue(zone_config.colors[0]);
            zone_pkt[4]             = RGBGetBValue(zone_config.colors[0]);

            bus->i2c_smbus_write_i2c_block_data(dev, EVGA_GPU_V3_REG_STATIC + zone, sizeof(zone_pkt), zone_pkt);
        }
        break;

    case EVGA_GPU_V3_MODE_BREATHING:
        {
            uint8_t zone_pkt[10];
            memset(zone_pkt, EVGAGPUV3_INIT, sizeof(zone_pkt));

            zone_pkt[0]             = sizeof(zone_pkt) - 1;
            zone_pkt[1]             = zone_config.brightness;
            zone_pkt[2]             = RGBGetRValue(zone_config.colors[0]);
            zone_pkt[3]             = RGBGetGValue(zone_config.colors[0]);
            zone_pkt[4]             = RGBGetBValue(zone_config.colors[0]);
            if(zone_config.numberOfColors == 1)
            {
                zone_config.colors[1] = 0;
            }
            zone_pkt[5]             = RGBGetRValue(zone_config.colors[1]);
            zone_pkt[6]             = RGBGetGValue(zone_config.colors[1]);
            zone_pkt[7]             = RGBGetBValue(zone_config.colors[1]);
            zone_pkt[8]             = speed16.lsb;
            zone_pkt[9]             = speed16.msb;

            bus->i2c_smbus_write_i2c_block_data(dev, EVGA_GPU_V3_REG_STATIC + ((mode -1) * 4) + zone, sizeof(zone_pkt), zone_pkt);
        }
        break;

    case EVGA_GPU_V3_MODE_RAINBOW:
    case EVGA_GPU_V3_MODE_RAINBOW_WAVE:
    case EVGA_GPU_V3_MODE_STAR:
        {
            uint8_t zone_pkt[4];
            memset(zone_pkt, EVGAGPUV3_INIT, sizeof(zone_pkt));

            zone_pkt[0]             = sizeof(zone_pkt) - 1;
            zone_pkt[1]             = zone_config.brightness;
            zone_pkt[2]             = speed16.lsb;
            zone_pkt[3]             = speed16.msb;

            bus->i2c_smbus_write_i2c_block_data(dev, EVGA_GPU_V3_REG_STATIC + ((mode -1) * 4) + zone, sizeof(zone_pkt), zone_pkt);
        }
        break;

    case EVGA_GPU_V3_MODE_WAVE:
        {
            uint8_t zone_pkt[7];
            memset(zone_pkt, EVGAGPUV3_INIT, sizeof(zone_pkt));

            zone_pkt[0]             = sizeof(zone_pkt) - 1;
            zone_pkt[1]             = zone_config.brightness;
            zone_pkt[2]             = RGBGetRValue(zone_config.colors[0]);
            zone_pkt[3]             = RGBGetGValue(zone_config.colors[0]);
            zone_pkt[4]             = RGBGetBValue(zone_config.colors[0]);
            zone_pkt[5]             = speed16.lsb;
            zone_pkt[6]             = speed16.msb;

            bus->i2c_smbus_write_i2c_block_data(dev, EVGA_GPU_V3_REG_STATIC + ((mode -1) * 4) + zone, sizeof(zone_pkt), zone_pkt);
        }
        break;

    case EVGA_GPU_V3_MODE_COLOR_CYCLE:
    case EVGA_GPU_V3_MODE_COLOR_STACK:
        {
            uint8_t zone_pkt[31];;
            memset(zone_pkt, EVGAGPUV3_INIT, sizeof(zone_pkt));
            uint8_t color_cnt_pkt[5];
            memset(color_cnt_pkt, EVGAGPUV3_INIT, sizeof(color_cnt_pkt));

            // Zone packet construction
            zone_pkt[0] = sizeof(zone_pkt) - 1;
            for(uint8_t color_index = 0; color_index < zone_config.numberOfColors; color_index++)
            {
                zone_pkt[1 + color_index * 4] = zone_config.brightness;
                zone_pkt[2 + color_index * 4] = RGBGetRValue(zone_config.colors[color_index]);
                zone_pkt[3 + color_index * 4] = RGBGetGValue(zone_config.colors[color_index]);
                zone_pkt[4 + color_index * 4] = RGBGetBValue(zone_config.colors[color_index]);
            }
            zone_pkt[29] = speed16.lsb;
            zone_pkt[30] = speed16.msb;

            // Color Count packet construction
            color_cnt_pkt[0]                = sizeof(color_cnt_pkt) - 1;
            color_cnt_pkt[zone+1]           = zone_config.numberOfColors;

            if(mode == EVGA_GPU_V3_MODE_COLOR_STACK)
            {
                uint8_t direction_pkt[5];
                memset(direction_pkt, EVGAGPUV3_INIT, sizeof(direction_pkt));
                // Direction packet construction
                direction_pkt[0]            = sizeof(direction_pkt) - 1;
                direction_pkt[zone+1]       = zone_config.direction;

                bus->i2c_smbus_write_i2c_block_data(dev, EVGA_GPU_V3_REG_STATIC + ((mode -1) * 4) + zone, sizeof(zone_pkt), zone_pkt);
                bus->i2c_smbus_write_i2c_block_data(dev, EVGA_GPU_V3_REG_COLOR_STACK_COLOR_COUNT, sizeof(color_cnt_pkt), color_cnt_pkt);
                bus->i2c_smbus_write_i2c_block_data(dev, EVGA_GPU_V3_REG_COLOR_STACK_DIRECTION, sizeof(direction_pkt), direction_pkt);
            }
            else
            {
                bus->i2c_smbus_write_i2c_block_data(dev, EVGA_GPU_V3_REG_STATIC + ((mode -1) * 4) + zone, sizeof(zone_pkt), zone_pkt);
                bus->i2c_smbus_write_i2c_block_data(dev, EVGA_GPU_V3_REG_COLOR_CYCLE_COUNT, sizeof(color_cnt_pkt), color_cnt_pkt);
            }
        }
        break;
    default:
        {
            LOG_TRACE("[%s] Mode %02d not found", name.c_str(), mode);
        }
        break;
    }
}
