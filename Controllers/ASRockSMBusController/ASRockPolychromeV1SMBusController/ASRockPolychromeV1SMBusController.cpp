/*---------------------------------------------------------*\
| ASRockPolychromeV1SMBusController.cpp                     |
|                                                           |
|   Driver for SMBus ASRock Polychrome V1 motherboards      |
|                                                           |
|   Adam Honse (CalcProgrammer1)                14 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "ASRockPolychromeV1SMBusController.h"
#include "dmiinfo.h"
#include "LogManager.h"

#define ASROCK_ZONE_LED_COUNT_MESSAGE_EN    "[%s] Zone %i LED count: %02d"

using namespace std::chrono_literals;

ASRockPolychromeV1SMBusController::ASRockPolychromeV1SMBusController(i2c_smbus_interface* bus, polychrome_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;

    DMIInfo dmi;
    device_name =   "ASRock " + dmi.getMainboard();

    ReadLEDConfiguration();
}

ASRockPolychromeV1SMBusController::~ASRockPolychromeV1SMBusController()
{

}

std::string ASRockPolychromeV1SMBusController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

std::string ASRockPolychromeV1SMBusController::GetDeviceName()
{
    return(device_name);
}

std::string ASRockPolychromeV1SMBusController::GetFirmwareVersion()
{
    uint8_t  major_version = fw_version >> 8;
    uint8_t  minor_version = fw_version & 0xFF;

    return(std::to_string(major_version) + "." + std::to_string(minor_version));
}

void ASRockPolychromeV1SMBusController::ReadLEDConfiguration()
{
    /*---------------------------------------------------------------------------------*\
    | The LED configuration register holds 6 bytes, so the first read should return 6   |
    | If not, set all zone sizes to zero                                                |
    \*---------------------------------------------------------------------------------*/
    LOG_DEBUG("[%s] Reading Zone sizes from controller", device_name.c_str());
    uint8_t asrock_zone_count[I2C_SMBUS_BLOCK_MAX] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
    if (bus->i2c_smbus_read_block_data(dev, POLYCHROME_V1_REG_ZONE_SIZE, asrock_zone_count) == 0x06)
    {
        zone_led_count[POLYCHROME_V1_ZONE_1]           = asrock_zone_count[0];
        zone_led_count[POLYCHROME_V1_ZONE_2]           = asrock_zone_count[1];
        zone_led_count[POLYCHROME_V1_ZONE_3]           = asrock_zone_count[2];
        zone_led_count[POLYCHROME_V1_ZONE_4]           = asrock_zone_count[3];
        zone_led_count[POLYCHROME_V1_ZONE_5]           = asrock_zone_count[4];
        zone_led_count[POLYCHROME_V1_ZONE_ADDRESSABLE] = asrock_zone_count[5];
        LOG_DEBUG(ASROCK_ZONE_LED_COUNT_MESSAGE_EN, device_name.c_str(), POLYCHROME_V1_ZONE_1, zone_led_count[POLYCHROME_V1_ZONE_1]);
        LOG_DEBUG(ASROCK_ZONE_LED_COUNT_MESSAGE_EN, device_name.c_str(), POLYCHROME_V1_ZONE_2, zone_led_count[POLYCHROME_V1_ZONE_2]);
        LOG_DEBUG(ASROCK_ZONE_LED_COUNT_MESSAGE_EN, device_name.c_str(), POLYCHROME_V1_ZONE_3, zone_led_count[POLYCHROME_V1_ZONE_3]);
        LOG_DEBUG(ASROCK_ZONE_LED_COUNT_MESSAGE_EN, device_name.c_str(), POLYCHROME_V1_ZONE_4, zone_led_count[POLYCHROME_V1_ZONE_4]);
        LOG_DEBUG(ASROCK_ZONE_LED_COUNT_MESSAGE_EN, device_name.c_str(), POLYCHROME_V1_ZONE_5, zone_led_count[POLYCHROME_V1_ZONE_5]);
        LOG_DEBUG("[%s] Addressable Zone LED count: %02d", device_name.c_str(), zone_led_count[POLYCHROME_V1_ZONE_ADDRESSABLE]);
    }
    else
    {
        LOG_WARNING("[%s] LED config read failed", device_name.c_str());
        memset(zone_led_count, 0, sizeof(zone_led_count));
    }
}

uint8_t ASRockPolychromeV1SMBusController::GetARGBColorOrder()
{
    uint8_t temp[1] = { 0x00 };
    LOG_TRACE("[%s] Reading ARGB color order config from the controller", device_name.c_str());

    //Read the data
    if(bus->i2c_smbus_read_block_data(dev, POLYCHROME_V1_REG_ARGB_GRB, temp) == 0x01)
    {
        if(temp[0] == 1)
        {
            LOG_DEBUG("[%s] Color order is GRB for the ARGB header", device_name.c_str());
        }
        else
        {
            LOG_DEBUG("[%s] Color order is RGB for the ARGB header", device_name.c_str());
        }
        return temp[0];
    }
    else
    {
        return 0;
    }
}

RGBColor ASRockPolychromeV1SMBusController::GetZoneColor(uint8_t zone)
{
    LOG_TRACE("[%s] Reading color from zone %02d", device_name.c_str(), zone);
    return zone_config[zone].color;
}

uint8_t ASRockPolychromeV1SMBusController::GetZoneMode(uint8_t zone)
{
    LOG_TRACE("[%s] Retreving mode %02X from zone_modes for zone %02d", device_name.c_str(), zone_config[zone].mode, zone);
    return(zone_config[zone].mode);
}

void ASRockPolychromeV1SMBusController::LoadZoneConfig()
{
    uint8_t zone[1] = { 0x00 };
    uint8_t mode[1] = { 0xFF };
    uint8_t color_speed_pkt[4] = { 0, 0, 0, 0 };

    LOG_TRACE("[%s] Reading modes from all zones", device_name.c_str());
    //Polychrome v1 supports per zone modes so we need to set the zone before we can read the mode.
    //Write the zone index.

    for(uint8_t zone_idx = 0 ; zone_idx < POLYCHROME_V1_ZONE_COUNT; zone_idx ++)
    {
        if(zone_led_count[zone_idx] > 0)
        {
            zone[0] = zone_idx;
            bus->i2c_smbus_write_block_data(dev, POLYCHROME_V1_REG_ZONE_SELECT, 1, zone);

            //Read the data back.
            if(bus->i2c_smbus_read_block_data(dev, POLYCHROME_V1_REG_ZONE_SELECT, zone) == 0x01)
            {
                //Validate that we changed correctly.
                if(zone[0] == zone_idx)
                {
                    //Read the mode for the zone.
                    if(bus->i2c_smbus_read_block_data(dev, POLYCHROME_V1_REG_MODE, mode) == 0x01)
                    {
                        LOG_DEBUG("[%s] Mode 0x%02x for zone %02d", device_name.c_str(), mode[0], zone_idx);
                        zone_config[zone_idx].mode = mode[0];

                        bus->i2c_smbus_read_block_data(dev, zone_config[zone_idx].mode, color_speed_pkt);

                        zone_config[zone_idx].color = color_speed_pkt[0] << 16 | color_speed_pkt[1] << 8 | color_speed_pkt[2];
                        zone_config[zone_idx].speed = color_speed_pkt [3];

                        LOG_TRACE("[%s] Mode config: %06X, %02X", device_name.c_str(), zone_config[zone_idx].color, zone_config[zone_idx].speed );
                    }
                }
                else
                {
                    LOG_WARNING("[%s] Zone mode register failed to change!", device_name.c_str() );
                }
            }
        }
        else
        {
            zone_config[zone_idx].mode = 0;
            zone_config[zone_idx].color = 0;
            zone_config[zone_idx].speed = 0;
        }
    }
}

void ASRockPolychromeV1SMBusController::SetARGBColorOrder(bool value)
{
    uint8_t temp[1] = { 0x00 };

    LOG_TRACE("[%s] Setting ARGB color order config to the controller", device_name.c_str());

    temp[0] = (value) ? 0x01 : 0x00;
    bus->i2c_smbus_write_block_data(dev, POLYCHROME_V1_REG_ARGB_GRB, 1, temp);
    GetARGBColorOrder();
}

bool ASRockPolychromeV1SMBusController::SetARGBSize(uint8_t new_size)
{
    LOG_DEBUG("[%s] Setting ARGB header to %02d.", device_name.c_str(), new_size);

    uint8_t asrock_zone_count[6] = { 0x0 };
    uint8_t new_asrock_zone_count[6] = { 0x0 };

    //memcpy(new_asrock_zone_count, zone_led_count, sizeof(new_asrock_zone_count - 1));
    new_asrock_zone_count[POLYCHROME_V1_ZONE_1] = zone_led_count[POLYCHROME_V1_ZONE_1];
    new_asrock_zone_count[POLYCHROME_V1_ZONE_2] = zone_led_count[POLYCHROME_V1_ZONE_2];
    new_asrock_zone_count[POLYCHROME_V1_ZONE_3] = zone_led_count[POLYCHROME_V1_ZONE_3];
    new_asrock_zone_count[POLYCHROME_V1_ZONE_4] = zone_led_count[POLYCHROME_V1_ZONE_4];
    new_asrock_zone_count[POLYCHROME_V1_ZONE_5] = zone_led_count[POLYCHROME_V1_ZONE_5];
    new_asrock_zone_count[POLYCHROME_V1_ZONE_ADDRESSABLE] = new_size;

    //Write the new config to the register
    bus->i2c_smbus_write_block_data(dev, POLYCHROME_V1_REG_ZONE_SIZE, 6, new_asrock_zone_count);

    //Validate the write
    if (bus->i2c_smbus_read_block_data(dev, POLYCHROME_V1_REG_ZONE_SIZE, asrock_zone_count) == 0x06)
    {
        for (uint8_t i = 0; i < 6; i++)
        {
            if (new_asrock_zone_count[i] != asrock_zone_count[i])
            {
                LOG_WARNING("[%s] Failed to validate zone %02d size!", device_name.c_str(), i);
                return false;
            }
        }
        LOG_DEBUG("[%s] Zone configuration validation completed.", device_name.c_str());
        return true;
    }
    return false;
}

void ASRockPolychromeV1SMBusController::SetColorsAndSpeed(uint8_t zone, uint8_t red, uint8_t green, uint8_t blue)
{
    LOG_TRACE("[%s] Updating color and speed for zone %02d: 0x%06X", device_name.c_str(), zone, (red << 16 | green << 8 | blue));
    uint8_t color_speed_pkt[4] = { red, green, blue, zone_config[zone].speed };
    uint8_t select_zone_pkt[1]  = { zone };

    /*-----------------------------------------------------*\
    | Select Zone                                           |
    \*-----------------------------------------------------*/
    if(zone_config[zone].mode != POLYCHROME_V1_MODE_OFF)
    {
        bus->i2c_smbus_write_block_data(dev, POLYCHROME_V1_REG_ZONE_SELECT, 1, select_zone_pkt);
        std::this_thread::sleep_for(1ms);
    }

    switch(zone_config[zone].mode)
    {
        /*-----------------------------------------------------*\
        | These modes take 4 bytes in R/G/B/S order             |
        \*-----------------------------------------------------*/
        case POLYCHROME_V1_MODE_BREATHING:
        case POLYCHROME_V1_MODE_STROBE:
        case POLYCHROME_V1_MODE_SPECTRUM_CYCLE:
        case POLYCHROME_V1_MODE_SPRING:
        case POLYCHROME_V1_MODE_METEOR:
        case POLYCHROME_V1_MODE_STACK:
        case POLYCHROME_V1_MODE_CRAM:
        case POLYCHROME_V1_MODE_SCAN:
        case POLYCHROME_V1_MODE_NEON:
        case POLYCHROME_V1_MODE_WATER:

            bus->i2c_smbus_write_block_data(dev, zone_config[zone].mode, 4, color_speed_pkt);
            break;

        /*-----------------------------------------------------*\
        | These modes take 3 bytes in R/G/B order               |
        \*-----------------------------------------------------*/
        default:
        case POLYCHROME_V1_MODE_STATIC:
        case POLYCHROME_V1_MODE_MUSIC:
            bus->i2c_smbus_write_block_data(dev, zone_config[zone].mode, 3, color_speed_pkt);
            break;

        /*-----------------------------------------------------*\
        | These modes take 1 byte - speed                       |
        \*-----------------------------------------------------*/
        case POLYCHROME_V1_MODE_RANDOM:
        case POLYCHROME_V1_MODE_WAVE:
        case POLYCHROME_V1_MODE_RAINBOW:
            bus->i2c_smbus_write_block_data(dev, zone_config[zone].mode, 1, &zone_config[zone].speed);
            break;

        /*-----------------------------------------------------*\
        | These modes take no bytes                             |
        \*-----------------------------------------------------*/
        case POLYCHROME_V1_MODE_OFF:
            break;
    }
    std::this_thread::sleep_for(1ms);

}

void ASRockPolychromeV1SMBusController::SetMode(uint8_t zone, uint8_t mode, uint8_t speed)
{
    LOG_TRACE("[%s] Updating mode for zone %02d, Mode 0x%02X, Speed 0x%02X", device_name.c_str(), zone, mode, speed);
    uint8_t led_count_pkt[1]    = { 0x00 };
    zone_config[zone].mode      = mode;
    zone_config[zone].speed     = speed;

    /*-----------------------------------------------------*\
    | Make sure set all register is set to 0                |
    \*-----------------------------------------------------*/
    bus->i2c_smbus_write_block_data(dev, POLYCHROME_V1_REG_SET_ALL, 1, led_count_pkt);
    std::this_thread::sleep_for(1ms);

    /*-----------------------------------------------------*\
    | Set the zone we are working on                        |
    \*-----------------------------------------------------*/
    bus->i2c_smbus_write_block_data(dev, POLYCHROME_V1_REG_ZONE_SELECT, 1, &zone);
    std::this_thread::sleep_for(1ms);

    /*-----------------------------------------------------*\
    | Write the mode                                        |
    \*-----------------------------------------------------*/
    bus->i2c_smbus_write_block_data(dev, POLYCHROME_V1_REG_MODE, 1, &zone_config[zone].mode);
    std::this_thread::sleep_for(1ms);
}
