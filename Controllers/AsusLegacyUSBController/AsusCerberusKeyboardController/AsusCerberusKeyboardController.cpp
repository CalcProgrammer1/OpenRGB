/*---------------------------------------------------------*\
| AsusCerberusKeyboardController.cpp                        |
|                                                           |
|   Driver for ASUS Cerberus keyboard                       |
|                                                           |
|   Mola19                                      28 May 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cmath>
#include <cstring>
#include <math.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include "AsusCerberusKeyboardController.h"
#include "StringUtils.h"

#define ASUS_CERBERUS_KB_PACKET_SIZE 8

AsusCerberusKeyboardController::AsusCerberusKeyboardController(hid_device* dev_handle, const char* path, unsigned short rev_version, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
    version     = rev_version;
}

AsusCerberusKeyboardController::~AsusCerberusKeyboardController()
{
    hid_close(dev);
}

std::string AsusCerberusKeyboardController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string AsusCerberusKeyboardController::GetDeviceName()
{
    return(name);
}

std::string AsusCerberusKeyboardController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string AsusCerberusKeyboardController::GetVersion()
{
    char versionstr[5];
    snprintf(versionstr, 5, "%X", version);
    return(std::string(versionstr));
}

void AsusCerberusKeyboardController::SetProfile
    (
    uint8_t           profile
    )
{
    uint8_t usb_buf[ASUS_CERBERUS_KB_PACKET_SIZE];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00] = 0x07;
    usb_buf[0x01] = 0x03;
    usb_buf[0x02] = profile;

    hid_send_feature_report(dev, usb_buf, ASUS_CERBERUS_KB_PACKET_SIZE);
}

void AsusCerberusKeyboardController::SetPerLEDColor
    (
    uint8_t           key,
    uint8_t           red,
    uint8_t           green,
    uint8_t           blue
    )
{
    uint8_t profile = 1;

    uint8_t usb_buf[ASUS_CERBERUS_KB_PACKET_SIZE];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00] = 0x07;
    usb_buf[0x01] = 0x0D;
    usb_buf[0x02] = profile;
    usb_buf[0x03] = key;
    usb_buf[0x04] = red;
    usb_buf[0x05] = green;
    usb_buf[0x06] = blue;

    hid_send_feature_report(dev, usb_buf, ASUS_CERBERUS_KB_PACKET_SIZE);
}

void AsusCerberusKeyboardController::SendPerLEDColorEnd()
{
    SetPerLEDColor(255, 0, 0, 0);
}

void AsusCerberusKeyboardController::SetPerLEDMode
    (
    uint8_t           mode
    )
{
    /*------------------------------------------------------------------------------------------------------*\
    | this device has 6 different profiles, but there is no way to fetch them from the device,               |
    | hence this device always sends to the first profile until a better solution is implemented in OpenRGB  |
    \*------------------------------------------------------------------------------------------------------*/
    uint8_t profile = 1;

    /*--------------------------------------------------------------------------------*\
    | 8 booleans per byte, each boolean controls one key                               |
    | 0 = static, 1 = breathing                                                        |
    | since openrgb doesn't support per led modes, either all static or all breathing  |
    \*--------------------------------------------------------------------------------*/

    uint8_t modebyte = (mode == 1) ? 0xFF : 0x00;

    for(int i = 0; i < 4; i++)
    {
        uint8_t usb_buf[ASUS_CERBERUS_KB_PACKET_SIZE];

        memset(usb_buf, 0x00, sizeof(usb_buf));

        usb_buf[0x00] = 0x07;
        usb_buf[0x01] = 0x0E;
        usb_buf[0x02] = profile;
        usb_buf[0x03] = i;
        usb_buf[0x04] = modebyte;
        usb_buf[0x05] = modebyte;
        usb_buf[0x06] = modebyte;
        usb_buf[0x07] = modebyte;

        hid_send_feature_report(dev, usb_buf, ASUS_CERBERUS_KB_PACKET_SIZE);
    }
}

void AsusCerberusKeyboardController::SetMode
    (
    uint8_t           mode,
    uint8_t           red,
    uint8_t           green,
    uint8_t           blue,
    uint8_t           direction,
    uint8_t           brightness
    )
{
    /*------------------------------------------------------------------------------------------------------*\
    | this device has 6 different profiles, but there is no way to fetch them from the device,               |
    | hence this device always sends to the first profile until a better solution is implemented in OpenRGB  |
    \*------------------------------------------------------------------------------------------------------*/
    uint8_t profile = 1;

    uint8_t usb_buf_1[ASUS_CERBERUS_KB_PACKET_SIZE];

    memset(usb_buf_1, 0x00, sizeof(usb_buf_1));

    usb_buf_1[0x00] = 0x07;
    usb_buf_1[0x01] = 0x0A;
    usb_buf_1[0x02] = profile;
    usb_buf_1[0x03] = mode;
    usb_buf_1[0x04] = direction;

    hid_send_feature_report(dev, usb_buf_1, ASUS_CERBERUS_KB_PACKET_SIZE);

    uint8_t usb_buf_2[ASUS_CERBERUS_KB_PACKET_SIZE];

    memset(usb_buf_2, 0x00, sizeof(usb_buf_2));

    usb_buf_2[0x00] = 0x07;
    usb_buf_2[0x01] = 0x0B;
    usb_buf_2[0x02] = profile;
    usb_buf_2[0x03] = mode;
    usb_buf_2[0x04] = red;
    usb_buf_2[0x05] = green;
    usb_buf_2[0x06] = blue;

    hid_send_feature_report(dev, usb_buf_2, ASUS_CERBERUS_KB_PACKET_SIZE);

    uint8_t usb_buf_3[ASUS_CERBERUS_KB_PACKET_SIZE];

    memset(usb_buf_3, 0x00, sizeof(usb_buf_3));

    usb_buf_3[0x00] = 0x07;
    usb_buf_3[0x01] = 0x0C;
    usb_buf_3[0x02] = profile;
    usb_buf_3[0x03] = brightness;

    hid_send_feature_report(dev, usb_buf_3, ASUS_CERBERUS_KB_PACKET_SIZE);
}
