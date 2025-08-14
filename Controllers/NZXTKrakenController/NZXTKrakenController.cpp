/*---------------------------------------------------------*\
| NZXTKrakenController.cpp                                  |
|                                                           |
|   Driver for NZXT Kraken                                  |
|                                                           |
|   Martin Hartl (inlart)                       04 Apr 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include <string>
#include <sstream>
#include "NZXTKrakenController.h"
#include "StringUtils.h"

static void SetColor(const std::vector<RGBColor>& colors, unsigned char* color_data)
{
    for(std::size_t idx = 0; idx < colors.size(); idx++)
    {
        int pixel_idx = (int)idx * 3;
        RGBColor color = colors[idx];
        color_data[pixel_idx + 0x00] = RGBGetRValue(color);
        color_data[pixel_idx + 0x01] = RGBGetGValue(color);
        color_data[pixel_idx + 0x02] = RGBGetBValue(color);
    }
}

static RGBColor ToLogoColor(RGBColor rgb)
{
    return ToRGBColor(RGBGetGValue(rgb), RGBGetRValue(rgb), RGBGetBValue(rgb));
}

NZXTKrakenController::NZXTKrakenController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;

    /*-----------------------------------------------------*\
    | Get the firmware version                              |
    \*-----------------------------------------------------*/
    UpdateStatus();
}

NZXTKrakenController::~NZXTKrakenController()
{
    hid_close(dev);
}

std::string NZXTKrakenController::GetFirmwareVersion()
{
    return firmware_version;
}

std::string NZXTKrakenController::GetLocation()
{
    return("HID: " + location);
}

std::string NZXTKrakenController::GetName()
{
    return(name);
}

std::string NZXTKrakenController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void NZXTKrakenController::UpdateStatus()
{
    unsigned char usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Read packet                                           |
    \*-----------------------------------------------------*/
    hid_read(dev, usb_buf, 64);

    /*-----------------------------------------------------*\
    | Extract cooler information                            |
    \*-----------------------------------------------------*/
    liquid_temperature = usb_buf[0x1] + (usb_buf[0x2] * 0.1);
    fan_speed = usb_buf[0x3] << 8 | usb_buf[0x4];
    pump_speed = usb_buf[0x5] << 8 | usb_buf[0x6];

    /*-----------------------------------------------------*\
    | Extract firmware version                              |
    \*-----------------------------------------------------*/
    int major = usb_buf[0xb];
    int minor = usb_buf[0xc] << 8 | usb_buf[0xd];
    int patch = usb_buf[0xe];
    std::stringstream ss;
    ss << major << '.' << minor << '.' << patch;
    firmware_version = ss.str();
}

void NZXTKrakenController::UpdateEffect
    (
    NZXTKrakenChannel_t channel,
    unsigned char mode,
    bool direction,
    unsigned char speed,
    int seq,
    std::vector<RGBColor> colors
    )
{
    unsigned char color_data[9 * 3];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(color_data, 0, sizeof(color_data));

    /*-----------------------------------------------------*\
    | Fill in color data                                    |
    \*-----------------------------------------------------*/
    if(!colors.empty() && channel != NZXT_KRAKEN_CHANNEL_RING)
    {
        colors[0] = ToLogoColor(colors[0]);
    }

    /*-----------------------------------------------------*\
    | Update color data                                     |
    \*-----------------------------------------------------*/
    SetColor(colors, color_data);

    /*-----------------------------------------------------*\
    | Send update packet                                    |
    \*-----------------------------------------------------*/
    SendEffect(channel, mode, direction, color_data, speed, false, seq);
}

void NZXTKrakenController::SendEffect
    (
    unsigned char   channel,
    unsigned char   mode,
    bool            direction,
    unsigned char*  color_data,
    unsigned char   speed /* = 0x02 */,
    bool            movement /* = false */,
    int             cis /* = 0 */,
    int             size /* = 0 */
    )
{
    unsigned char usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set effect mode                                       |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x02;
    usb_buf[0x01]   = 0x4c;

    /*-----------------------------------------------------*\
    | Set effect channel, movement and direction            |
    \*-----------------------------------------------------*/
    usb_buf[0x02]   = channel;
    usb_buf[0x02]   |= movement ? ( 1 << 3 ) : 0;
    usb_buf[0x02]   |= direction ? ( 1 << 4 ) : 0;

    /*-----------------------------------------------------*\
    | Set mode                                              |
    \*-----------------------------------------------------*/
    usb_buf[0x03]   = mode;

    /*-----------------------------------------------------*\
    | Set effect speed, size and color in set               |
    \*-----------------------------------------------------*/
    usb_buf[0x04]   = speed;
    usb_buf[0x04]   |= size << 3;
    usb_buf[0x04]   |= cis << 5;

    /*-----------------------------------------------------*\
    | Copy color data bytes                                 |
    \*-----------------------------------------------------*/
    if(color_data)
    {
        memcpy(usb_buf + 0x05, color_data, 9 * 3);
    }

    /*-----------------------------------------------------*\
    | Send effect                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 64);
}
