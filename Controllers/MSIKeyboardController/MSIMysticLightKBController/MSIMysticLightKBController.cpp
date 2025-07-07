/*---------------------------------------------------------*\
| MSIMysticLightKBController.cpp                            |
|                                                           |
|   Driver for MSI Mystic Light MS-1565 keyboard leds       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "MSIMysticLightKBController.h"

#include "StringUtils.h"
#include "hidapi.h"
#include <map>

std::map<MS_1565_ZONE, unsigned char> zone_map =
{
    { MS_1565_ZONE_1,      1 },
    { MS_1565_ZONE_2,      2 },
    { MS_1565_ZONE_3,      4 },
    { MS_1565_ZONE_4,      8 },
    { MS_1565_ZONE_DEVICE, 15}
};

MSIKeyboardController::MSIKeyboardController
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

MSIKeyboardController::~MSIKeyboardController()
{
    hid_close(dev);
}

void MSIKeyboardController::SetMode
(
    MS_1565_MODE mode,
    MS_1565_SPEED speed1,
    MS_1565_SPEED speed2,
    MS_1565_WAVE_DIRECTION wave_dir,
    MS_1565_ZONE zone,
    ColorKeyFrame color_keyframes[]
)
{
    unsigned char buf[64] = {};
    buf[0] = 0x02;
    buf[1] = 0x01;
    buf[2] = zone_map[zone];
    hid_send_feature_report(dev, buf, sizeof(buf));

    FeaturePacket_MS1565 data;
    data.mode = (unsigned char)(mode);
    for(int i = 0; i < MAX_MS_1565_KEYFRAMES; i++)
    {
        data.color_keyframes[i] = color_keyframes[i];
    }
    data.speed2      = speed2;
    data.speed1      = speed1;

    data.wave_dir = (unsigned char)(wave_dir);
    /*-----------------------------------------------------*\
    | Send packet to hardware, return true if successful    |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, (unsigned char *)&data, sizeof(data));
    return;
}

std::string MSIKeyboardController::GetDeviceName()
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

std::string MSIKeyboardController::GetFWVersion()
{
    /*-----------------------------------------------------*\
    | This device doesn't support firmware version          |
    \*-----------------------------------------------------*/
    std::string firmware_version = "";
    return firmware_version;
}

std::string MSIKeyboardController::GetDeviceLocation()
{
    return ("HID: " + location);
}

std::string MSIKeyboardController::GetSerial()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}
