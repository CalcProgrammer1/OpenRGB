/*---------------------------------------------------------*\
| AsusSagarisKeyboardController.cpp                         |
|                                                           |
|   Driver for ASUS Sagaris keyboard                        |
|                                                           |
|   Mola19                                      20 Aug 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include <math.h>
#include <string>
#include <string.h>
#include <vector>
#include "AsusSagarisKeyboardController.h"
#include "LogManager.h"
#include "StringUtils.h"

#define ASUS_SAGARIS_KB_PACKET_SIZE 65

AsusSagarisKeyboardController::AsusSagarisKeyboardController(hid_device* dev_handle, const char* path, unsigned short rev_version, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
    version     = rev_version;
}

AsusSagarisKeyboardController::~AsusSagarisKeyboardController()
{
    hid_close(dev);
}

std::string AsusSagarisKeyboardController::GetVersion()
{
    return std::to_string((int) floor(version / 0x100)) + "." + std::to_string(version % 0x100);
}

std::string AsusSagarisKeyboardController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string AsusSagarisKeyboardController::GetDeviceName()
{
    return(name);
}

std::string AsusSagarisKeyboardController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

sagaris_mode AsusSagarisKeyboardController::GetMode()
{
    ClearResponses();

    uint8_t usb_buf_out[ASUS_SAGARIS_KB_PACKET_SIZE];
    memset(usb_buf_out, 0x00, sizeof(usb_buf_out));

    usb_buf_out[0x00] = 0x06;
    usb_buf_out[0x01] = 0x0B;
    usb_buf_out[0x02] = 0x03;

    hid_write(dev, usb_buf_out, ASUS_SAGARIS_KB_PACKET_SIZE);

    unsigned char usb_buf_in[ASUS_SAGARIS_KB_PACKET_SIZE];
    memset(usb_buf_in, 0x00, sizeof(usb_buf_in));
    int return_length = hid_read_timeout(dev, usb_buf_in, ASUS_SAGARIS_KB_PACKET_SIZE, 100);

    if(return_length == -1)
    {
        LOG_DEBUG("[Asus Sagaris GK1100]: Could not fetch mode");
        sagaris_mode default_mode;
        default_mode.mode       = SAGARIS_KEYBOARD_MODE_STATIC;
        default_mode.brightness = SAGARIS_KEYBOARD_BRIGHTNESS_DEFAULT;
        default_mode.speed      = 0;
        default_mode.colorIndex = 0;

        return default_mode;
    }

    sagaris_mode current_mode;
    current_mode.mode       = usb_buf_in[3];
    current_mode.brightness = usb_buf_in[4];
    current_mode.speed      = usb_buf_in[5];
    current_mode.colorIndex = usb_buf_in[6];

    return current_mode;
}

std::vector<RGBColor> AsusSagarisKeyboardController::GetColors()
{
    ClearResponses();

    uint8_t usb_buf_out[ASUS_SAGARIS_KB_PACKET_SIZE];
    memset(usb_buf_out, 0x00, sizeof(usb_buf_out));

    usb_buf_out[0x00] = 0x06;
    usb_buf_out[0x01] = 0x0B;
    usb_buf_out[0x02] = 0x05;

    hid_write(dev, usb_buf_out, ASUS_SAGARIS_KB_PACKET_SIZE);

    std::vector<RGBColor> colors;
    colors.resize(7);

    for(int i = 0; i < 7; i++)
    {
        unsigned char usb_buf_in[ASUS_SAGARIS_KB_PACKET_SIZE];
        memset(usb_buf_in, 0x00, sizeof(usb_buf_in));
        int return_length = hid_read_timeout(dev, usb_buf_in, ASUS_SAGARIS_KB_PACKET_SIZE, 100);

        if(return_length == -1)
        {
           LOG_DEBUG("[Asus Sagaris GK1100]: Could not fetch color %i", i);
           colors[i] = ToRGBColor(0, 0, 0);
           continue;
        }

        colors[i] = ToRGBColor(usb_buf_in[4] * 16, usb_buf_in[5] * 16, usb_buf_in[6] * 16);
    }

    return colors;
}

void AsusSagarisKeyboardController::SetColor
    (
    uint8_t           index,
    uint8_t           red,
    uint8_t           green,
    uint8_t           blue
    )
{
    uint8_t usb_buf[ASUS_SAGARIS_KB_PACKET_SIZE];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00] = 0x06;
    usb_buf[0x01] = 0x0B;
    usb_buf[0x02] = 0x04;
    usb_buf[0x03] = 0x00;
    usb_buf[0x04] = index;
    usb_buf[0x05] = red;
    usb_buf[0x06] = green;
    usb_buf[0x07] = blue;

    hid_write(dev, usb_buf, ASUS_SAGARIS_KB_PACKET_SIZE);
}

void AsusSagarisKeyboardController::SetMode
    (
    uint8_t           mode,
    uint8_t           brightness,
    uint8_t           speed,
    uint8_t           colorIndex
    )
{
    ClearResponses();

    uint8_t usb_buf[ASUS_SAGARIS_KB_PACKET_SIZE];
    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00] = 0x06;
    usb_buf[0x01] = 0x0B;
    usb_buf[0x02] = 0x01;
    usb_buf[0x03] = mode;
    usb_buf[0x04] = brightness;
    usb_buf[0x05] = speed;
    usb_buf[0x06] = colorIndex;

    hid_write(dev, usb_buf, ASUS_SAGARIS_KB_PACKET_SIZE);

    AwaitResponse(20);
}

void AsusSagarisKeyboardController::AwaitResponse(int ms)
{
    unsigned char usb_buf[ASUS_SAGARIS_KB_PACKET_SIZE];
    hid_read_timeout(dev, usb_buf, ASUS_SAGARIS_KB_PACKET_SIZE, ms);
}

void AsusSagarisKeyboardController::ClearResponses()
{
    int result = 1;
    unsigned char usb_buf_flush[65];
    while(result > 0)
    {
        result = hid_read_timeout(dev, usb_buf_flush, 65, 0);
    }
}
