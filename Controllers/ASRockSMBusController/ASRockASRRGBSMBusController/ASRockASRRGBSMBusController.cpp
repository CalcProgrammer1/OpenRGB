/*---------------------------------------------------------*\
| ASRockASRRGBSMBusController.cpp                           |
|                                                           |
|   Driver for SMBus ASRock ASR RGB motherboards            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                14 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "ASRockASRRGBSMBusController.h"
#include <cstring>
#include "dmiinfo.h"
#include "LogManager.h"

#define ASROCK_ZONE_LED_COUNT_MESSAGE_EN    "[%s] Zone %i LED count: %02d"

using namespace std::chrono_literals;

ASRockASRRGBSMBusController::ASRockASRRGBSMBusController(i2c_smbus_interface* bus, polychrome_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;

    DMIInfo dmi;

    device_name =   "ASRock " + dmi.getMainboard();

}

ASRockASRRGBSMBusController::~ASRockASRRGBSMBusController()
{

}

std::string ASRockASRRGBSMBusController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

std::string ASRockASRRGBSMBusController::GetDeviceName()
{
    return(device_name);
}

std::string ASRockASRRGBSMBusController::GetFirmwareVersion()
{
    uint8_t  major_version = fw_version >> 8;
    uint8_t  minor_version = fw_version & 0xFF;

    return(std::to_string(major_version) + "." + std::to_string(minor_version));
}

uint8_t ASRockASRRGBSMBusController::GetMode()
{
    return(active_mode);
}

void ASRockASRRGBSMBusController::SetColorsAndSpeed(uint8_t led, uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t color_speed_pkt[4] = { red, green, blue, active_speed };
    uint8_t select_led_pkt[1]  = { led };

    /*-----------------------------------------------------*\
    | Select LED                                            |
    \*-----------------------------------------------------*/
    if(active_mode != ASRLED_MODE_OFF)
    {
        bus->i2c_smbus_write_block_data(dev, ASROCK_ASR_REG_LED_SELECT, 1, select_led_pkt);
        std::this_thread::sleep_for(1ms);
    }

    switch(active_mode)
    {
        /*-----------------------------------------------------*\
        | These modes take 4 bytes in R/G/B/S order             |
        \*-----------------------------------------------------*/
        case ASRLED_MODE_BREATHING:
        case ASRLED_MODE_STROBE:
        case ASRLED_MODE_SPECTRUM_CYCLE:
            bus->i2c_smbus_write_block_data(dev, active_mode, 4, color_speed_pkt);
            break;

        /*-----------------------------------------------------*\
        | These modes take 3 bytes in R/G/B order               |
        \*-----------------------------------------------------*/
        default:
        case ASRLED_MODE_STATIC:
        case ASRLED_MODE_MUSIC:
            bus->i2c_smbus_write_block_data(dev, active_mode, 3, color_speed_pkt);
            break;

        /*-----------------------------------------------------*\
        | These modes take 1 byte - speed                       |
        \*-----------------------------------------------------*/
        case ASRLED_MODE_RANDOM:
        case ASRLED_MODE_WAVE:
            bus->i2c_smbus_write_block_data(dev, active_mode, 1, &active_speed);
            break;

        /*-----------------------------------------------------*\
        | These modes take no bytes                             |
        \*-----------------------------------------------------*/
        case ASRLED_MODE_OFF:
            break;
    }
    std::this_thread::sleep_for(1ms);
}

void ASRockASRRGBSMBusController::SetMode(uint8_t zone,uint8_t mode, uint8_t speed)
{
    active_zone                     = zone;
    active_mode                     = mode;
    active_speed                    = speed;

    bus->i2c_smbus_write_block_data(dev, ASROCK_ASR_REG_MODE, 1, &active_mode);
    std::this_thread::sleep_for(1ms);
}
