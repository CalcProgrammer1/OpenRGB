/*---------------------------------------------------------*\
| CorsairK65MiniController.cpp                              |
|                                                           |
|   Driver for Corsair K65 Mini keyboard                    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "CorsairK65MiniController.h"
#include "LogManager.h"
#include "StringUtils.h"

CorsairK65MiniController::CorsairK65MiniController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev             = dev_handle;
    location        = path;
    name            = dev_name;

    LightingControl();
}

CorsairK65MiniController::~CorsairK65MiniController()
{
    hid_close(dev);
}

std::string CorsairK65MiniController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string CorsairK65MiniController::GetName()
{
    return(name);
}

std::string CorsairK65MiniController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void CorsairK65MiniController::LightingControl()
{
    /*-----------------------------------------*\
    |  A few init packets have to be sent to    |
    |  enabled software control                 |
    \*-----------------------------------------*/
    unsigned char usb_buf[PACKET_LENGTH];
    memset(usb_buf, 0x00, PACKET_LENGTH);

    usb_buf[0x01] = K65_WRITE_COMMAND;
    usb_buf[0x02] = 0x01;
    usb_buf[0x03] = 0x03;
    usb_buf[0x05] = 0x02;

    hid_write(dev, usb_buf, PACKET_LENGTH);

    memset(usb_buf, 0x00, PACKET_LENGTH);

    usb_buf[0x01] = K65_WRITE_COMMAND;
    usb_buf[0x02] = 0x02;
    usb_buf[0x03] = 0x6E;

    hid_write(dev, usb_buf, PACKET_LENGTH);

    memset(usb_buf, 0x00, PACKET_LENGTH);

    usb_buf[0x01] = K65_WRITE_COMMAND;
    usb_buf[0x02] = 0x0D;
    usb_buf[0x03] = 0x01;
    usb_buf[0x04] = 0x22;

    hid_write(dev, usb_buf, PACKET_LENGTH);
}

void CorsairK65MiniController::SetLEDs(std::vector<RGBColor>colors, std::vector<unsigned int> positions)
{
    unsigned char usb_buf[PACKET_LENGTH];
    memset(usb_buf, 0x00, PACKET_LENGTH);

    /*-----------------------------------------*\
    |  Direct mode pattern                      |
    |  08 06 01 73 01 00 00 12 00 RR GG BB ...  |
    \*-----------------------------------------*/
    usb_buf[0x01] = K65_WRITE_COMMAND;
    usb_buf[0x02] = 0x06;
    usb_buf[0x03] = 0x01;
    usb_buf[0x04] = 0x73;

    usb_buf[0x05] = 0x01;
    usb_buf[0x08] = 0x12;

    for(unsigned int i = 0 ; i < colors.size(); i++)
    {
        unsigned int position = positions[i];
        usb_buf[0x0A + position * 3]      = RGBGetRValue(colors[i]);
        usb_buf[0x0A + position * 3 + 1]  = RGBGetGValue(colors[i]);
        usb_buf[0x0A + position * 3 + 2]  = RGBGetBValue(colors[i]);
    }

    hid_write(dev, usb_buf, PACKET_LENGTH);
}
