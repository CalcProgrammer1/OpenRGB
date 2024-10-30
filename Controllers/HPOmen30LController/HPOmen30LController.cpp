/*---------------------------------------------------------*\
| HPOmen30LController.cpp                                   |
|                                                           |
|   Driver for HP Omen 30L                                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include "HPOmen30LController.h"

#define HP_OMEN_30L_BUFFER_SIZE     58
#define HP_OMEN_30L_VERSION_ID      0x12
#define HP_OMEN_30L_MAX_BRIGHTNESS  0x64

HPOmen30LController::HPOmen30LController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;

    strcpy(device_name, "HP Omen 30L");

    hp_zone logo;
    logo.value      = HP_OMEN_30L_LOGO_ZONE;
    logo.mode       = HP_OMEN_30L_DIRECT;
    logo.speed      = HP_OMEN_30L_SPEED_MED;
    logo.brightness = HP_OMEN_30L_MAX_BRIGHTNESS;
    hp_zones.push_back(logo);

    hp_zone bar;
    bar.value      = HP_OMEN_30L_BAR_ZONE;
    bar.mode       = HP_OMEN_30L_DIRECT;
    bar.speed      = HP_OMEN_30L_SPEED_MED;
    bar.brightness = HP_OMEN_30L_MAX_BRIGHTNESS;
    hp_zones.push_back(bar);

    hp_zone fan;
    fan.value      = HP_OMEN_30L_FAN_ZONE;
    fan.mode       = HP_OMEN_30L_DIRECT;
    fan.speed      = HP_OMEN_30L_SPEED_MED;
    fan.brightness = HP_OMEN_30L_MAX_BRIGHTNESS;
    hp_zones.push_back(fan);

    hp_zone cpu;
    cpu.value      = HP_OMEN_30L_CPU_ZONE;
    cpu.mode       = HP_OMEN_30L_DIRECT;
    cpu.speed      = HP_OMEN_30L_SPEED_MED;
    cpu.brightness = HP_OMEN_30L_MAX_BRIGHTNESS;
    hp_zones.push_back(cpu);

    hp_zone botFan;
    botFan.value      = HP_OMEN_30L_BOT_FAN_ZONE;
    botFan.mode       = HP_OMEN_30L_DIRECT;
    botFan.speed      = HP_OMEN_30L_SPEED_MED;
    botFan.brightness = HP_OMEN_30L_MAX_BRIGHTNESS;
    hp_zones.push_back(botFan);

    hp_zone midFan;
    midFan.value      = HP_OMEN_30L_MID_FAN_ZONE;
    midFan.mode       = HP_OMEN_30L_DIRECT;
    midFan.speed      = HP_OMEN_30L_SPEED_MED;
    midFan.brightness = HP_OMEN_30L_MAX_BRIGHTNESS;
    hp_zones.push_back(midFan);

    hp_zone topFan;
    topFan.value      = HP_OMEN_30L_TOP_FAN_ZONE;
    topFan.mode       = HP_OMEN_30L_DIRECT;
    topFan.speed      = HP_OMEN_30L_SPEED_MED;
    topFan.brightness = HP_OMEN_30L_MAX_BRIGHTNESS;
    hp_zones.push_back(topFan);
}

HPOmen30LController::~HPOmen30LController()
{
    hid_close(dev);
}

std::string HPOmen30LController::GetLocationString()
{
    return("HID: " + location);
}

char* HPOmen30LController::GetDeviceName()
{
    return device_name;
}

std::string HPOmen30LController::GetSerialString()
{
    std::string ret_string = "";
    return(ret_string);
}

std::string HPOmen30LController::GetEffectChannelString(unsigned char /*channel*/)
{
    std::string ret_string = "";
    return(ret_string);
}

std::string HPOmen30LController::GetFirmwareVersionString()
{
    std::string ret_string = "";
    return(ret_string);
}

void HPOmen30LController::SetZoneMode(int zone,unsigned char mode, unsigned char speed,unsigned char brightness)
{
    hp_zones[zone].mode       = mode;
    hp_zones[zone].speed      = speed;
    hp_zones[zone].brightness = brightness;

}

void HPOmen30LController::SetZoneColor(int zone, std::vector<RGBColor> colors)
{
    SendZoneUpdate(zone, colors);
}

void HPOmen30LController::SendZoneUpdate(int zone, std::vector<RGBColor> colors)
{
    unsigned char usb_buf[HP_OMEN_30L_BUFFER_SIZE] = {};  // zero-initialize array
    // 0x00 - 0x01: Unknown
    // 0x02: Version ID (HP_OMEN_30L_VERSION_ID)
    // 0x03: Lighting mode (static, direct, off, breathing, cycle, blinking, ...)
    // 0x04: Total color count (only different from 1 in modes with changing colors)
    // 0x05: Current color number (see above, used to set the different colors, one at a time, starting at 1)
    // 0x06 - 0x07: Unknown
    // 0x08 - 0x23: Used to control the RGB (static) or RGBA (direct) of the first seven LED zones
    // 0x24 - 0x2f: Unknown, probably also used for RGB/RGBA control if there's support up to 10 zones
    // 0x30: Brightness
    // 0x31: Type (static=0x02, direct=0x04, changing colors=0x0A)
    // 0x32 - 0x35: Unknown
    // 0x36: Zone to update (can only update one at a time)
    // 0x37: Power mode to update (on, suspend)
    // 0x38: Theme (either 0 to use the colors set in 0x04/0x05, or the ID of a predefined theme)
    // 0x39: Color change speed (only relevant for modes with changing colors)

    usb_buf[0x02]   = HP_OMEN_30L_VERSION_ID;
    usb_buf[0x36]   = hp_zones[zone].value;

    // The Omen controller allows setting different modes for when the computer is powered on
    // vs when the computer is suspended.
    // Because the OpenRGB UI does not allow such a choice, we're only changing the powered on mode.
    // If this ever changes, we need to take the user choice into consideration (HP_OMEN_30L_POWER_SUSPEND)
    usb_buf[0x37]   = HP_OMEN_30L_POWER_ON;
    usb_buf[0x03]   = hp_zones[zone].mode;

    if (hp_zones[zone].mode == HP_OMEN_30L_OFF)
    {
        hid_write(dev, usb_buf, HP_OMEN_30L_BUFFER_SIZE);
        return;
    }

    usb_buf[0x30]   = hp_zones[zone].brightness;
    int index = hp_zones[zone].value - 1;
    if(hp_zones[zone].mode == HP_OMEN_30L_DIRECT)
    {
        usb_buf[0x31] = HP_OMEN_30L_DIRECT;
        usb_buf[0x04] = 0x01;
        usb_buf[0x05] = 0x01;
        usb_buf[0x08 + index * 4] = HP_OMEN_30L_MAX_BRIGHTNESS;
        usb_buf[0x09 + index * 4] = RGBGetRValue(colors[zone]);
        usb_buf[0x0A + index * 4] = RGBGetGValue(colors[zone]);
        usb_buf[0x0B + index * 4] = RGBGetBValue(colors[zone]);

        hid_write(dev, usb_buf, HP_OMEN_30L_BUFFER_SIZE);
    }
    else if(hp_zones[zone].mode == HP_OMEN_30L_STATIC)
    {
        usb_buf[0x31] = 0x02;
        usb_buf[0x04] = 0x01;
        usb_buf[0x05] = 0x01;
        usb_buf[0x08 + index * 3] = RGBGetRValue(colors[zone]);
        usb_buf[0x09 + index * 3] = RGBGetGValue(colors[zone]);
        usb_buf[0x0A + index * 3] = RGBGetBValue(colors[zone]);

        hid_write(dev, usb_buf, HP_OMEN_30L_BUFFER_SIZE);
    }
    else
    {
        usb_buf[0x31] = 0x0A;
        usb_buf[0x39] = hp_zones[zone].speed;

        // Theme is custom by default, but if we could select a theme through the UI,
        // we would set it in usb_buf[0x38] here and ignore the custom colors vector
        unsigned char theme = HP_OMEN_30L_THEME_CUSTOM;
        usb_buf[0x38] = theme;
        if (theme == HP_OMEN_30L_THEME_CUSTOM)
        {
            usb_buf[0x04] = (unsigned char)colors.size();
            for(unsigned int i = 0; i < (unsigned int)colors.size(); i++)
            {
                usb_buf[0x05] = i + 1;
                usb_buf[0x08 + index * 3] = RGBGetRValue(colors[i]);
                usb_buf[0x09 + index * 3] = RGBGetGValue(colors[i]);
                usb_buf[0x0A + index * 3] = RGBGetBValue(colors[i]);
                hid_write(dev, usb_buf, HP_OMEN_30L_BUFFER_SIZE);
            }
        }
        else
        {
            hid_write(dev, usb_buf, HP_OMEN_30L_BUFFER_SIZE);
        }
    }
}
