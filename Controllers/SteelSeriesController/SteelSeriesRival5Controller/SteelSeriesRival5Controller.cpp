/*---------------------------------------------------------*\
| SteelSeriesRival5Controller.cpp                           |
|                                                           |
|   Controller for SteelSeries Rival 5                      |
|                                                           |
|   A Ingram (Woodsy900)                        20 Jul 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "SteelSeriesRival5Controller.h"
#include "LogManager.h"
#include <cstring>

SteelSeriesRival5Controller::SteelSeriesRival5Controller(
    hid_device* dev_handle,
    steelseries_type proto_type,
    const char* path,
    std::string dev_name
)
    : SteelSeriesMouseController(
        dev_handle,
        proto_type,
        path,
        dev_name
    )

{
    memset(zone_colors, 0, sizeof(zone_colors));
}

SteelSeriesRival5Controller::~SteelSeriesRival5Controller()
{
    hid_close(dev);
}


steelseries_mouse SteelSeriesRival5Controller::GetMouse()
{
    return rival_5;
}

std::string SteelSeriesRival5Controller::GetFirmwareVersion()
{
    return "";
 /*--------------------------------------------------------*\
 | Note: Rival 5 does not have a "Get firmware" command     |
 \*--------------------------------------------------------*/
}

void SteelSeriesRival5Controller::SetLightEffectAll(uint8_t)
{
 /*--------------------------------------------------------*\
 | Note: Not Used                                           |
 \*--------------------------------------------------------*/
}

void SteelSeriesRival5Controller::SetRainbowEffect(bool enable)
{
    uint8_t report[64] = {0};

    if(enable)
    {
        report[1] = 0x22;
        report[2] = 0xFF;
        report[3] = 0x03;
    }

    hid_send_feature_report(
        dev,
        report,
        sizeof(report)
    );
}

void SteelSeriesRival5Controller::Save()
{
 /*--------------------------------------------------------*\
 | Note: Rival 5 does not have a "save" command             |
 |       The Rival 5 saves the current state automatically  |
 \*--------------------------------------------------------*/
}

void SteelSeriesRival5Controller::SetColor(
    unsigned char zone_id,
    unsigned char red,
    unsigned char green,
    unsigned char blue,
    unsigned char /*brightness*/
)
{
    if(zone_id >= 10)
    {
        return;
    }

    zone_colors[zone_id][0] = red;
    zone_colors[zone_id][1] = green;
    zone_colors[zone_id][2] = blue;
}

void SteelSeriesRival5Controller::UpdateLEDs(const std::vector<RGBColor>& colors)
{
    uint8_t report[64] = {0};

    report[0] = 0x00;
    report[1] = 0x21;
    report[2] = 0xFF;
    report[3] = 0x03;

    for(size_t i = 0; i < 10 && i < colors.size(); i++)
    {
        report[4 + (i * 3) + 0] = RGBGetRValue(colors[i]);
        report[4 + (i * 3) + 1] = RGBGetGValue(colors[i]);
        report[4 + (i * 3) + 2] = RGBGetBValue(colors[i]);
    }

    int result =
        hid_send_feature_report(
            dev,
            report,
            sizeof(report)
        );
}

void SteelSeriesRival5Controller::SetBrightness(uint8_t brightness)
{
    if(brightness == current_brightness) return;   // only on change
    current_brightness = brightness;

    uint8_t report[64] = {0};
    report[1] = 0x23;
    //report[2] = brightness;
    if(brightness >= 88)
    {
        report[2] = 0x64;
    }
    else if(brightness >= 63)
    {
        report[2] = 0x32;
    }
    else if(brightness >= 38)
    {
        report[2] = 0x19;
    }
    else if(brightness >= 13)
    {
        report[2] = 0x0C;
    }
    else
    {
        report[2] = 0x00;
    }                          // 0x00–0x64
    hid_send_feature_report(dev, report, sizeof(report));
}

void SteelSeriesRival5Controller::SetReactiveColor(bool on, uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t report[64] = {0};

    report[1] = 0x26;

    if(on) 
    { 
        report[2] = 0x01; 
        report[3] = 0x00; 
        report[4] = r; 
        report[5] = g; 
        report[6] = b; 
    }
    // off → bytes stay 0x00 → [00 26 00 00 00 00 00]
    hid_send_feature_report(dev, report, sizeof(report));
}