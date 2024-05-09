/*---------------------------------------------------------*\
| ASRockPolychromeV2SMBusController.cpp                     |
|                                                           |
|   Driver for SMBus ASRock Polychrome V2 motherboards      |
|                                                           |
|   Adam Honse (CalcProgrammer1)                14 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "ASRockPolychromeV2SMBusController.h"
#include "dmiinfo.h"
#include "LogManager.h"

#define ASROCK_ZONE_LED_COUNT_MESSAGE_EN    "[%s] Zone %i LED count: %02d"

using namespace std::chrono_literals;

ASRockPolychromeV2SMBusController::ASRockPolychromeV2SMBusController(i2c_smbus_interface* bus, polychrome_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;

    DMIInfo dmi;

    device_name =   "ASRock " + dmi.getMainboard();

    ReadLEDConfiguration();
}

ASRockPolychromeV2SMBusController::~ASRockPolychromeV2SMBusController()
{

}

std::string ASRockPolychromeV2SMBusController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

std::string ASRockPolychromeV2SMBusController::GetDeviceName()
{
    return(device_name);
}

std::string ASRockPolychromeV2SMBusController::GetFirmwareVersion()
{
    uint8_t  major_version = fw_version >> 8;
    uint8_t  minor_version = fw_version & 0xFF;

    return(std::to_string(major_version) + "." + std::to_string(minor_version));
}


void ASRockPolychromeV2SMBusController::ReadLEDConfiguration()
{
    /*---------------------------------------------------------------------------------*\
    | The LED configuration register holds 6 bytes, so the first read should return 6   |
    | If not, set all zone sizes to zero                                                |
    \*---------------------------------------------------------------------------------*/
    LOG_DEBUG("[%s] Reading LED config from controller", device_name.c_str());
    uint8_t asrock_zone_count[I2C_SMBUS_BLOCK_MAX] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
    if (bus->i2c_smbus_read_block_data(dev, POLYCHROME_V2_REG_LED_CONFIG, asrock_zone_count) == 0x06)
    {
        zone_led_count[POLYCHROME_V2_ZONE_1]           = asrock_zone_count[0];
        zone_led_count[POLYCHROME_V2_ZONE_2]           = asrock_zone_count[1];
        zone_led_count[POLYCHROME_V2_ZONE_3]           = asrock_zone_count[2];
        zone_led_count[POLYCHROME_V2_ZONE_4]           = asrock_zone_count[3];
        zone_led_count[POLYCHROME_V2_ZONE_5]           = asrock_zone_count[4];
        zone_led_count[POLYCHROME_V2_ZONE_ADDRESSABLE] = asrock_zone_count[5];
        LOG_DEBUG(ASROCK_ZONE_LED_COUNT_MESSAGE_EN, device_name.c_str(), POLYCHROME_V2_ZONE_1, zone_led_count[POLYCHROME_V2_ZONE_1]);
        LOG_DEBUG(ASROCK_ZONE_LED_COUNT_MESSAGE_EN, device_name.c_str(), POLYCHROME_V2_ZONE_2, zone_led_count[POLYCHROME_V2_ZONE_2]);
        LOG_DEBUG(ASROCK_ZONE_LED_COUNT_MESSAGE_EN, device_name.c_str(), POLYCHROME_V2_ZONE_3, zone_led_count[POLYCHROME_V2_ZONE_3]);
        LOG_DEBUG(ASROCK_ZONE_LED_COUNT_MESSAGE_EN, device_name.c_str(), POLYCHROME_V2_ZONE_4, zone_led_count[POLYCHROME_V2_ZONE_4]);
        LOG_DEBUG(ASROCK_ZONE_LED_COUNT_MESSAGE_EN, device_name.c_str(), POLYCHROME_V2_ZONE_5, zone_led_count[POLYCHROME_V2_ZONE_5]);
        LOG_DEBUG("[%s] Addressable Zone LED count: %02d", device_name.c_str(), zone_led_count[POLYCHROME_V2_ZONE_ADDRESSABLE]);
    }
    else
    {
        LOG_WARNING("[%s] LED config read failed", device_name.c_str());
        memset(zone_led_count, 0, sizeof(zone_led_count));
    }
}

uint8_t ASRockPolychromeV2SMBusController::GetMode()
{
    return(active_mode);
}

void ASRockPolychromeV2SMBusController::SetColorsAndSpeed(uint8_t led, uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t color_speed_pkt[4] = { red, green, blue, active_speed };
    uint8_t select_led_pkt[1]  = { led };

    /*-----------------------------------------------------*\
    | Select LED                                            |
    \*-----------------------------------------------------*/
    switch(active_mode)
    {
        case POLYCHROME_V2_MODE_OFF:
        case POLYCHROME_V2_MODE_RAINBOW:
        case POLYCHROME_V2_MODE_SPECTRUM_CYCLE:
            break;

        default:
            bus->i2c_smbus_write_block_data(dev, POLYCHROME_V2_REG_LED_SELECT, 1, select_led_pkt);
            std::this_thread::sleep_for(1ms);

            /*-----------------------------------------------------*\
            | Polychrome firmware always writes color to fixed reg  |
            \*-----------------------------------------------------*/
            bus->i2c_smbus_write_block_data(dev, POLYCHROME_V2_REG_COLOR, 3, color_speed_pkt);
            std::this_thread::sleep_for(1ms);
            break;
    }
}

void ASRockPolychromeV2SMBusController::SetMode(uint8_t zone,uint8_t mode, uint8_t speed)
{
    uint8_t       led_count_pkt[1]  = { 0x00 };
    active_zone                     = zone;
    active_mode                     = mode;
    active_speed                    = speed;

    bus->i2c_smbus_write_block_data(dev, POLYCHROME_V2_REG_MODE, 1, &active_mode);
    std::this_thread::sleep_for(1ms);

    /*-----------------------------------------------------*\
    | Select a single LED                                   |
    \*-----------------------------------------------------*/
    bus->i2c_smbus_write_block_data(dev, POLYCHROME_V2_REG_LED_COUNT, 0, led_count_pkt);
    std::this_thread::sleep_for(1ms);

    switch(active_mode)
    {
        /*-----------------------------------------------------*\
        | These modes don't take a speed                        |
        \*-----------------------------------------------------*/
        case POLYCHROME_V2_MODE_OFF:
        case POLYCHROME_V2_MODE_STATIC:
            break;

        /*-----------------------------------------------------*\
        | All other modes, write speed to active mode register  |
        \*-----------------------------------------------------*/
        default:
            bus->i2c_smbus_write_block_data(dev, active_mode, 1, &speed);
            std::this_thread::sleep_for(1ms);
            break;
    }
}
