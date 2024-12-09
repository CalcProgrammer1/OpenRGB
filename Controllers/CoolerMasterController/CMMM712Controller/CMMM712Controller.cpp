/*---------------------------------------------------------*\
| CMMM712Controller.cpp                                     |
|                                                           |
|   Driver for Cooler Master MM712 mouse                    |
|   Derived from CMMM711Controller.cpp                      |
|                                                           |
|   Chris M (Dr_No)                             14 Feb 2021 |
|   Frans Meulenbroeks                          08 Dec 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "CMMM712Controller.h"
#include "StringUtils.h"

#define CM_MM712_PACKET_SIZE 65
#define CM_MM712_INTERRUPT_TIMEOUT 250
#define HID_MAX_STR 255

enum
{
    CM_MM712_MODE_BYTE              = 4,
    CM_MM712_BRIGHTNESS_BYTE        = 6,
    CM_MM712_SPEED_BYTE             = 7,
    CM_MM712_RED_BYTE               = 8,
    CM_MM712_GREEN_BYTE             = 9,
    CM_MM712_BLUE_BYTE              = 10,
};

CMMM712Controller::CMMM712Controller(hid_device* dev_handle, char *_path)
{
    dev                     = dev_handle;
    location                = _path;

    /*---------------------------------------------------------*\
    | Get device name from HID manufacturer and product strings |
    \*---------------------------------------------------------*/
    wchar_t name_string[HID_MAX_STR];

    hid_get_manufacturer_string(dev, name_string, HID_MAX_STR);
    device_name = StringUtils::wstring_to_string(name_string);

    hid_get_product_string(dev, name_string, HID_MAX_STR);
    device_name.append(" ").append(StringUtils::wstring_to_string(name_string));

    SendInitPacket();
    GetModeStatus();
    GetColorStatus(current_mode);
}

CMMM712Controller::~CMMM712Controller()
{
    hid_close(dev);
}

void CMMM712Controller::SendBuffer(uint8_t *buffer, uint8_t buffer_size)
{
    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_MM712_INTERRUPT_TIMEOUT);
}

void CMMM712Controller::GetColorStatus(uint8_t mode)
{
    uint8_t buffer[CM_MM712_PACKET_SIZE]    = { 0x00, 0x4C, 0x81, 0x03, mode };

    SendBuffer(buffer, CM_MM712_PACKET_SIZE);
    initial_color           = ToRGBColor(buffer[CM_MM712_RED_BYTE - 2], buffer[CM_MM712_GREEN_BYTE - 2], buffer[CM_MM712_BLUE_BYTE - 2]);
}

void CMMM712Controller::GetModeStatus()
{
    uint8_t buffer[CM_MM712_PACKET_SIZE]    = { 0x00, 0x4C, 0x81, 0x07 };

    SendBuffer(buffer, CM_MM712_PACKET_SIZE);
    current_mode            = buffer[CM_MM712_MODE_BYTE - 1];
    SetMode(current_mode);
}

std::string CMMM712Controller::GetDeviceName()
{
    return(device_name);
}

std::string CMMM712Controller::GetSerial()
{
    wchar_t serial_string[HID_MAX_STR];
    int ret = hid_get_serial_number_string(dev, serial_string, HID_MAX_STR);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string CMMM712Controller::GetLocation()
{
    return("HID: " + location);
}

unsigned char CMMM712Controller::GetMode()
{
    return(current_mode);
}

RGBColor CMMM712Controller::GetInitialLedColor()
{
    return initial_color;
}

void CMMM712Controller::SetLedsDirect(RGBColor color)
{
    unsigned char buffer[CM_MM712_PACKET_SIZE]  =
    {
        0x00, 0x5A, 0x81, 0x03,
        (unsigned char)RGBGetRValue(color),
        (unsigned char)RGBGetGValue(color),
        (unsigned char)RGBGetBValue(color)
    };

    if(current_mode!=CM_MM712_MODE_DIRECT)
    {
        SetDirectMode(true);
    }
    hid_write(dev, buffer, CM_MM712_PACKET_SIZE);
//    SendBuffer(buffer, CM_MM712_PACKET_SIZE);
}

void CMMM712Controller::SendUpdate(uint8_t mode, uint8_t speed, RGBColor color, uint8_t brightness)
{
    unsigned char buffer[CM_MM712_PACKET_SIZE]  =
    {
        0x00, 0x4C, 0x81, 0x04, mode, 0xFF, brightness, speed,
        (unsigned char)RGBGetRValue(color),
        (unsigned char)RGBGetGValue(color),
        (unsigned char)RGBGetBValue(color),
        0xFF
    };

    if(current_mode==CM_MM712_MODE_DIRECT)
    {
        SetDirectMode(false);
        SendInitPacket();
    }

    SendBuffer(buffer, CM_MM712_PACKET_SIZE);
    SetMode(mode);
}

void CMMM712Controller::SendInitPacket()
{
    unsigned char buffer[CM_MM712_PACKET_SIZE]  = { 0x00, 0x44, 0x81, 0x02 };

    SendBuffer(buffer, CM_MM712_PACKET_SIZE);
}

void CMMM712Controller::SetDirectMode(bool onoff)
{
    unsigned char buffer[CM_MM712_PACKET_SIZE]  = { 0x00, 0x5a, 0x81, (unsigned char)(0x01+onoff) };

    hid_write(dev, buffer, CM_MM712_PACKET_SIZE);
}

void CMMM712Controller::SetMode(uint8_t mode)
{
    unsigned char buffer[CM_MM712_PACKET_SIZE]  = { 0x00, 0x4C, 0x81, 0x08, mode};

    if(current_mode==CM_MM712_MODE_DIRECT)
    {
        SendInitPacket();
    }

    SendBuffer(buffer, CM_MM712_PACKET_SIZE);
    current_mode = mode;
}

void CMMM712Controller::SetProfile(uint8_t profile)
{
    unsigned char buffer[CM_MM712_PACKET_SIZE]  = { 0x00, 0x44, 0x81, 0x01, profile};

    SendBuffer(buffer, CM_MM712_PACKET_SIZE);
}

void CMMM712Controller::SaveStatus()
{
    unsigned char buffer[CM_MM712_PACKET_SIZE]  = { 0x54, 0x81, 1};

    SendBuffer(buffer, CM_MM712_PACKET_SIZE);
}
