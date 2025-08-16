/*---------------------------------------------------------*\
| RoccatHordeAimoController.cpp                             |
|                                                           |
|   Driver for Roccat Horde Aimo                            |
|                                                           |
|   Morgan Guimard (morg)                       24 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "RoccatHordeAimoController.h"
#include "StringUtils.h"

RoccatHordeAimoController::RoccatHordeAimoController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev                 = dev_handle;
    location            = info.path;
    name                = dev_name;

    InitialPacket();
}

RoccatHordeAimoController::~RoccatHordeAimoController()
{
    hid_close(dev);
}

void RoccatHordeAimoController::InitialPacket()
{
    unsigned char usb_buf[8];

    memset(usb_buf, 0x00,8);

    usb_buf[0x00] = 0x13;
    usb_buf[0x01] = 0x08;
    usb_buf[0x02] = 0x01;

    hid_send_feature_report(dev, usb_buf, 8);
}

std::string RoccatHordeAimoController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string RoccatHordeAimoController::GetNameString()
{
    return(name);
}

std::string RoccatHordeAimoController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void RoccatHordeAimoController::SetColors(std::vector<RGBColor> colors)
{
    unsigned char usb_buf[WRITE_PACKET_LENGTH];

    usb_buf[0x00] = REPORT_ID;
    usb_buf[0x01] = WRITE_PACKET_LENGTH;
    usb_buf[0x02] = 0xFF;
    usb_buf[0x03] = 0xFF;

    for(unsigned int i = 0; i < 6; i++)
    {
        usb_buf[0x04 + (i * 3)] = RGBGetRValue(colors[i]);
        usb_buf[0x05 + (i * 3)] = RGBGetGValue(colors[i]);
        usb_buf[0x06 + (i * 3)] = RGBGetBValue(colors[i]);
    }

    int crc = 0;

    for(unsigned int i = 0; i < WRITE_PACKET_LENGTH - 2; i++)
    {
        crc += usb_buf[i];
    }

    usb_buf[22] = crc;
    usb_buf[23] = crc >> 8;

    hid_send_feature_report(dev, usb_buf, WRITE_PACKET_LENGTH);

    unsigned char usb_read_buf[READ_PACKET_LENGTH];
    hid_get_feature_report(dev, usb_read_buf, READ_PACKET_LENGTH);
}
