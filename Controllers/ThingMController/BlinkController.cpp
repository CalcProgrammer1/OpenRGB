/*---------------------------------------------------------*\
| BlinkController.cpp                                       |
|                                                           |
|   Driver for ThingM Blink                                 |
|                                                           |
|   Eric S (edbgon)                             01 Oct 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "BlinkController.h"
#include "StringUtils.h"

BlinkController::BlinkController(hid_device* dev_handle, char *_path)
{
    dev             = dev_handle;
    location        = _path;

    /*---------------------------------------------------------*\
    | Get device name from HID manufacturer and product strings |
    \*---------------------------------------------------------*/
    wchar_t name_string[HID_MAX_STR];

    hid_get_manufacturer_string(dev, name_string, HID_MAX_STR);
    device_name = StringUtils::wstring_to_string(name_string);

    hid_get_product_string(dev, name_string, HID_MAX_STR);
    device_name.append(" ").append(StringUtils::wstring_to_string(name_string));
}

BlinkController::~BlinkController()
{
    if(dev)
    {
        hid_close(dev);
    }
}

std::string BlinkController::GetDeviceName()
{
    return device_name;
}

std::string BlinkController::GetSerial()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string BlinkController::GetLocation()
{
    return("HID: " + location);
}

void BlinkController::SendUpdate(unsigned char led, unsigned char red, unsigned char green, unsigned char blue, unsigned int speed)
{

    unsigned char buffer[BLINK_PACKET_SIZE] = { 0x00 };
    memset(buffer, 0x00, BLINK_PACKET_SIZE);

    buffer[0x00] = 0x01;
    buffer[0x01] = 0x63;
    buffer[0x02] = red;
    buffer[0x03] = green;
    buffer[0x04] = blue;

    if(speed > 0)
    {
        buffer[0x05] = (speed & 0xff00) >> 8;
        buffer[0x06] = speed & 0x00ff;
    }

    buffer[0x07] = led;

    hid_send_feature_report(dev, buffer, BLINK_PACKET_SIZE);
}
