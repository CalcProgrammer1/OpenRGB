/*---------------------------------------------------------*\
| ClevoLightbarController.cpp                               |
|                                                           |
|   Driver for Clevo laptop lightbar (ITE 8291 rev 0.03)    |
|   Protocol based on tuxedo-drivers ite_8291_lb module     |
|                                                           |
|   Kyle Cascade (kyle@cascade.family)          16 Jan 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "ClevoLightbarController.h"
#include <cstring>

ClevoLightbarController::ClevoLightbarController(hid_device* dev_handle, const hid_device_info& info)
{
    dev      = dev_handle;
    location = info.path;
    version  = info.release_number;
}

ClevoLightbarController::~ClevoLightbarController()
{
    hid_close(dev);
}

std::string ClevoLightbarController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string ClevoLightbarController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());

    return(return_string);
}

std::string ClevoLightbarController::GetFirmwareVersion()
{
    char version_string[16];
    snprintf(version_string, sizeof(version_string), "%d.%02d", version >> 8, version & 0xFF);
    return(version_string);
}

void ClevoLightbarController::WriteControl(unsigned char* data)
{
    hid_send_feature_report(dev, data, CLEVO_LIGHTBAR_REPORT_SIZE);
}

void ClevoLightbarController::TurnOn()
{
    /*---------------------------------------------------------*\
    | Not required for 0x7001 - device turns on when setting    |
    | color/brightness                                          |
    \*---------------------------------------------------------*/
}

void ClevoLightbarController::TurnOff()
{
    /*---------------------------------------------------------*\
    | Turn off sequence for device 0x7001                       |
    \*---------------------------------------------------------*/
    unsigned char buf[CLEVO_LIGHTBAR_REPORT_SIZE];

    memset(buf, 0x00, CLEVO_LIGHTBAR_REPORT_SIZE);
    buf[0] = 0x12;
    buf[2] = 0x03;
    WriteControl(buf);

    memset(buf, 0x00, CLEVO_LIGHTBAR_REPORT_SIZE);
    buf[0] = 0x08;
    buf[1] = 0x05;
    WriteControl(buf);

    memset(buf, 0x00, CLEVO_LIGHTBAR_REPORT_SIZE);
    buf[0] = 0x08;
    buf[1] = 0x01;
    WriteControl(buf);

    memset(buf, 0x00, CLEVO_LIGHTBAR_REPORT_SIZE);
    buf[0] = 0x1A;
    buf[7] = 0x01;
    WriteControl(buf);
}

void ClevoLightbarController::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    /*---------------------------------------------------------*\
    | Set color: 0x14 0x00 0x01 R G B 0x00 0x00                 |
    \*---------------------------------------------------------*/
    unsigned char buf[CLEVO_LIGHTBAR_REPORT_SIZE];

    memset(buf, 0x00, CLEVO_LIGHTBAR_REPORT_SIZE);
    buf[0] = 0x14;
    buf[1] = 0x00;
    buf[2] = 0x01;
    buf[3] = red;
    buf[4] = green;
    buf[5] = blue;

    WriteControl(buf);
}

void ClevoLightbarController::SetBrightness(unsigned char brightness)
{
    /*---------------------------------------------------------*\
    | Set brightness (mono mode):                               |
    | 0x08 0x22 0x01 0x01 brightness 0x01 0x00 0x00             |
    \*---------------------------------------------------------*/
    unsigned char buf[CLEVO_LIGHTBAR_REPORT_SIZE];

    memset(buf, 0x00, CLEVO_LIGHTBAR_REPORT_SIZE);
    buf[0] = 0x08;
    buf[1] = 0x22;
    buf[2] = 0x01;
    buf[3] = 0x01;
    buf[4] = brightness;
    buf[5] = 0x01;

    WriteControl(buf);
}

void ClevoLightbarController::SetMode(unsigned char mode, unsigned char brightness, unsigned char speed)
{
    /*---------------------------------------------------------*\
    | Set mode for device 0x7001:                               |
    | 0x08 0x22 MODE SPEED BRIGHTNESS 0x01 0x00 0x00            |
    |                                                           |
    | SPEED: 0x01 (fastest) to 0x0a (slowest) - we invert       |
    \*---------------------------------------------------------*/
    unsigned char buf[CLEVO_LIGHTBAR_REPORT_SIZE];

    /*---------------------------------------------------------*\
    | Invert speed: UI uses 1=slow, 10=fast                     |
    | Protocol uses 1=fast, 10=slow                             |
    \*---------------------------------------------------------*/
    unsigned char inverted_speed = (CLEVO_LIGHTBAR_SPEED_MAX + CLEVO_LIGHTBAR_SPEED_MIN) - speed;

    memset(buf, 0x00, CLEVO_LIGHTBAR_REPORT_SIZE);
    buf[0] = 0x08;
    buf[1] = 0x22;
    buf[2] = mode;
    buf[3] = inverted_speed;
    buf[4] = brightness;
    buf[5] = 0x01;

    WriteControl(buf);
}
