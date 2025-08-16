/*---------------------------------------------------------*\
| LogitechX56Controller.cpp                                 |
|                                                           |
|   Driver for Logitech X56                                 |
|                                                           |
|   Edbgon                                      11 Jun 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "LogitechX56Controller.h"
#include "StringUtils.h"

LogitechX56Controller::LogitechX56Controller(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

LogitechX56Controller::~LogitechX56Controller()
{
    hid_close(dev);
}

std::string LogitechX56Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string LogitechX56Controller::GetDeviceName()
{
    return(name);
}

std::string LogitechX56Controller::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void LogitechX56Controller::SetColor(RGBColor color, uint8_t brightness)
{
    unsigned char buf[X56_CONTROLLER_PACKET_SIZE];
    unsigned char cbuf[X56_CONTROLLER_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, X56_CONTROLLER_PACKET_SIZE);
    memset(cbuf, 0x00, X56_CONTROLLER_PACKET_SIZE);

    /*-----------------------------------------------------*\
    | Set up init packet                                    |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x09;
    buf[0x02]   = 0x02;
    buf[0x03]   = brightness;

    /*-----------------------------------------------------*\
    | Set up color packet                                   |
    \*-----------------------------------------------------*/
    cbuf[0x00]  = 0x09;
    cbuf[0x02]  = 0x03;
    cbuf[0x03]  = RGBGetRValue(color);
    cbuf[0x04]  = RGBGetGValue(color);
    cbuf[0x05]  = RGBGetBValue(color);

    /*-----------------------------------------------------*\
    | Send packets                                          |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf,  X56_CONTROLLER_PACKET_SIZE);
    hid_send_feature_report(dev, cbuf, X56_CONTROLLER_PACKET_SIZE);

}

void LogitechX56Controller::Save()
{
    uint8_t buffer[X56_CONTROLLER_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buffer, 0x00, X56_CONTROLLER_PACKET_SIZE);

    /*-----------------------------------------------------*\
    | Set up init packet                                    |
    \*-----------------------------------------------------*/
    buffer[0x00]    = 0x01;
    buffer[0x01]    = 0x01;

    hid_send_feature_report(dev, buffer, X56_CONTROLLER_PACKET_SIZE);
}
