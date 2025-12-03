/*---------------------------------------------------------*\
| LogitechG522Controller.cpp                                |
|                                                           |
|   Driver for Logitech G522 headset                        |
|                                                           |
|   fawmdev                                     03 Dec 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include "LogitechG522Controller.h"

LogitechG522Controller::LogitechG522Controller(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

LogitechG522Controller::~LogitechG522Controller()
{
    hid_close(dev);
}

std::string LogitechG522Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string LogitechG522Controller::GetDeviceName()
{
    return(name);
}

void LogitechG522Controller::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    SendColorPacket(red, green, blue);
}

void LogitechG522Controller::SetOff()
{
    SendColorPacket(0x00, 0x00, 0x00);
}

void LogitechG522Controller::SendColorPacket(unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char usb_buf[LOGITECH_G522_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Feature Report                                 |
    | Byte 0: Report ID (0x50)                              |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = LOGITECH_G522_REPORT_ID;

    /*-----------------------------------------------------*\
    | Data header (13 bytes)                                |
    | Based on reverse engineering of G522 protocol         |
    \*-----------------------------------------------------*/
    usb_buf[0x01] = 0x23;
    usb_buf[0x02] = 0x12;
    usb_buf[0x03] = 0x00;
    usb_buf[0x04] = 0x03;
    usb_buf[0x05] = 0x1A;
    usb_buf[0x06] = 0x00;
    usb_buf[0x07] = 0x0D;
    usb_buf[0x08] = 0x00;
    usb_buf[0x09] = 0x12;
    usb_buf[0x0A] = 0x3A;
    usb_buf[0x0B] = 0x00;
    usb_buf[0x0C] = 0x00;
    usb_buf[0x0D] = 0x00;

    /*-----------------------------------------------------*\
    | Color data (RGB order)                                |
    \*-----------------------------------------------------*/
    usb_buf[0x0E] = red;
    usb_buf[0x0F] = green;
    usb_buf[0x10] = blue;

    /*-----------------------------------------------------*\
    | Send Feature Report                                   |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, usb_buf, LOGITECH_G522_PACKET_SIZE);
}
