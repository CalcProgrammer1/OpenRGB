/*---------------------------------------------------------*\
| MSIMysticLight64Controller.cpp                            |
|                                                           |
|   Driver for MSI Mystic Light 64-byte motherboard         |
|                                                           |
|   T-bond                                      03 Apr 2020 |
|   Adam Honse                                  06 Mar 2021 |
|   Elchanan Haas                               23 Aug 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <algorithm>
#include <array>
#include <bitset>
#include "MSIMysticLight64Controller.h"
#include "StringUtils.h"

MSIMysticLight64Controller::MSIMysticLight64Controller
(
    hid_device *handle,
    const char *path
)
{
    dev = handle;
    if(dev)
    {
        location = path;
    }
}

MSIMysticLight64Controller::~MSIMysticLight64Controller()
{
    hid_close(dev);
}

void MSIMysticLight64Controller::SetMode
(
    MSI_64_MODE mode,
    MSI_SPEED speed,
    MSI_BRIGHTNESS brightness,
    unsigned int num_colors,
    Color colors[]
)
{
    FeaturePacket_64 data;
    for(int i = 0; i < MSI_64_MAX_COLORS; i++)
    {
        data.colors[i] = colors[i];
    }
    data.speed      = speed;
    data.brightness = brightness;
    data.num_colors = num_colors;
    data.mode       = mode;
    /*-----------------------------------------------------*\
    | Send packet to hardware, return true if successful    |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, (unsigned char *)&data, sizeof(data));
    return;
}

std::string MSIMysticLight64Controller::GetDeviceName()
{
    wchar_t tname[256];

    /*-----------------------------------------------------*\
    | Get the manufacturer string from HID                  |
    \*-----------------------------------------------------*/
    hid_get_manufacturer_string(dev, tname, 256);

    /*-----------------------------------------------------*\
    | Convert to std::string                                |
    \*-----------------------------------------------------*/
    std::string name = StringUtils::wstring_to_string(tname);

    /*-----------------------------------------------------*\
    | Get the product string from HID                       |
    \*-----------------------------------------------------*/
    hid_get_product_string(dev, tname, 256);

    /*-----------------------------------------------------*\
    | Append the product string to the manufacturer string  |
    \*-----------------------------------------------------*/
    name.append(" ").append(StringUtils::wstring_to_string(tname));

    return(name);
}

std::string MSIMysticLight64Controller::GetFWVersion()
{
    /*-----------------------------------------------------*\
    | This device doesn't support firmware version          |
    \*-----------------------------------------------------*/
    std::string firmware_version = "";
    return firmware_version;
}

std::string MSIMysticLight64Controller::GetDeviceLocation()
{
    return ("HID: " + location);
}

std::string MSIMysticLight64Controller::GetSerial()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}
