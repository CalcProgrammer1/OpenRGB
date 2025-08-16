/*---------------------------------------------------------*\
| SteelSeriesApex8ZoneController.cpp                        |
|                                                           |
|   Driver for SteelSeries Apex 8 Zone                      |
|                                                           |
|   Chris M (Dr_No)                             23 Feb 2022 |
|   Paul K. Gerke                               27 Oct 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string>
#include "SteelSeriesApex8ZoneController.h"
#include "LogManager.h"

SteelSeriesApex8ZoneController::SteelSeriesApex8ZoneController(hid_device* dev_handle, const char* path, std::string dev_name) : SteelSeriesApex3Controller(dev_handle, path, dev_name)
{

}

SteelSeriesApex8ZoneController::~SteelSeriesApex8ZoneController()
{

}

uint8_t SteelSeriesApex8ZoneController::GetLedCount()
{
    return STEELSERIES_8Z_LED_COUNT;
}

uint8_t SteelSeriesApex8ZoneController::GetMaxBrightness()
{
    return STEELSERIES_8Z_BRIGHTNESS_MAX;
}

bool SteelSeriesApex8ZoneController::SupportsRainbowWave()
{
    return true;
}

bool SteelSeriesApex8ZoneController::SupportsSave()
{
    return false;
}

void SteelSeriesApex8ZoneController::Save()
{
    /*---------------------------------------------------------*\
    | This device does not yet support saving                   |
    \*---------------------------------------------------------*/
}

void SteelSeriesApex8ZoneController::SetBrightness(uint8_t brightness)
{
    uint8_t buffer[STEELSERIES_8Z_WRITE_PACKET_SIZE]    = { 0x00, 0x23, brightness };

    hid_write(dev, buffer, STEELSERIES_8Z_WRITE_PACKET_SIZE);

    current_brightness = brightness;
}

uint8_t SteelSeriesApex8ZoneController::GetBrightness()
{
    uint8_t buffer[STEELSERIES_8Z_WRITE_PACKET_SIZE]    = { 0x00, 0xA3 };

    hid_write(dev, buffer, STEELSERIES_8Z_WRITE_PACKET_SIZE);

    int result = hid_read_timeout(dev, buffer, STEELSERIES_8Z_WRITE_PACKET_SIZE, STEELSERIES_APEX3_HID_TIMEOUT);
    if (result > 1 && buffer[0x00] == 0xA3)
    {
        return(buffer[0x01]);
    }

    return(STEELSERIES_8Z_BRIGHTNESS_MAX);
}

void SteelSeriesApex8ZoneController::SetColor(std::vector<RGBColor> colors, uint8_t mode, uint8_t brightness)
{
    uint8_t buffer[STEELSERIES_8Z_WRITE_PACKET_SIZE]    = { 0x00, 0x21, 0xFF };

    buffer[1]              += mode;

    for(unsigned int i = 0; i < colors.size(); i++)
    {
        uint8_t index       = i * 3;

        buffer[index + 3]   = RGBGetRValue(colors[i]);;
        buffer[index + 4]   = RGBGetGValue(colors[i]);;
        buffer[index + 5]   = RGBGetBValue(colors[i]);;
    }

    hid_write(dev, buffer, STEELSERIES_8Z_WRITE_PACKET_SIZE);

    if(current_brightness != brightness)
    {
        SetBrightness(brightness);
    }
}
