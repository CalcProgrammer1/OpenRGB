/*---------------------------------------------------------*\
| SteelSeriesApexTZoneController.cpp                        |
|                                                           |
|   Driver for SteelSeries Apex T Zone                      |
|                                                           |
|   Edbgon                                      06 Oct 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include "SteelSeriesApexTZoneController.h"

SteelSeriesApexTZoneController::SteelSeriesApexTZoneController(hid_device* dev_handle, const char* path, std::string dev_name) : SteelSeriesApex3Controller(dev_handle, path, dev_name)
{

}

SteelSeriesApexTZoneController::~SteelSeriesApexTZoneController()
{

}

uint8_t SteelSeriesApexTZoneController::GetLedCount()
{
    return STEELSERIES_TZ_LED_COUNT;
}

uint8_t SteelSeriesApexTZoneController::GetMaxBrightness()
{
    return STEELSERIES_TZ_BRIGHTNESS_MAX;
}

bool SteelSeriesApexTZoneController::SupportsRainbowWave()
{
    return false;
}

bool SteelSeriesApexTZoneController::SupportsSave()
{
    return true;
}

void SteelSeriesApexTZoneController::Save()
{
    unsigned char buf[STEELSERIES_TZ_WRITE_PACKET_SIZE] = { 0x00, 0x06, 0x00, 0x08 };

    hid_write(dev, buf, STEELSERIES_TZ_WRITE_PACKET_SIZE);

    buf[0x01]           = 0x09;
    buf[0x03]           = 0x00;
    hid_write(dev, buf, STEELSERIES_TZ_WRITE_PACKET_SIZE);
}

void SteelSeriesApexTZoneController::SetColor(std::vector<RGBColor> colors, uint8_t /*mode*/, uint8_t brightness)
{
    unsigned char buf[STEELSERIES_TZ_WRITE_PACKET_SIZE] = { 0x00 };

    /*-----------------------------------------------------*\
    | Zero out buffer, set up packet and send               |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, STEELSERIES_TZ_WRITE_PACKET_SIZE);

    buf[0x01]           = 0x0A;
    buf[0x03]           = brightness;
    hid_write(dev, buf, STEELSERIES_TZ_WRITE_PACKET_SIZE);

    buf[0x01]           = 0x0B;
    for(size_t i = 0; i < colors.size(); i++)
    {
        uint8_t index   = (uint8_t)(i * 3);

        buf[index + 3]  = RGBGetRValue(colors[i]);;
        buf[index + 4]  = RGBGetGValue(colors[i]);;
        buf[index + 5]  = RGBGetBValue(colors[i]);;
    }

    hid_write(dev, buf, STEELSERIES_TZ_WRITE_PACKET_SIZE);
}
