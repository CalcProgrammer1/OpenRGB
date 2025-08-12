/*---------------------------------------------------------*\
| LegoDimensionsToypadBaseController.cpp                    |
|                                                           |
|   Driver for Lego Dimensions Toypad Base                  |
|                                                           |
|   Morgan Guimard (morg)                       02 Jun 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string.h>
#include "LegoDimensionsToypadBaseController.h"
#include "StringUtils.h"

LegoDimensionsToypadBaseController::LegoDimensionsToypadBaseController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev                 = dev_handle;
    location            = info.path;
    name                = dev_name;

    Activate();
}

LegoDimensionsToypadBaseController::~LegoDimensionsToypadBaseController()
{
    hid_close(dev);
}

std::string LegoDimensionsToypadBaseController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string LegoDimensionsToypadBaseController::GetNameString()
{
    return(name);
}

std::string LegoDimensionsToypadBaseController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void LegoDimensionsToypadBaseController::Activate()
{
    unsigned char usb_buf[LEGO_DIMENSIONS_TOYPAD_BASE_PACKET_LENGTH];

    memset(usb_buf, 0x00, LEGO_DIMENSIONS_TOYPAD_BASE_PACKET_LENGTH);

    usb_buf[1]  = LEGO_DIMENSIONS_TOYPAD_BASE_REPORT_ID;
    usb_buf[2]  = 0x0F; // command length
    usb_buf[3]  = LEGO_DIMENSIONS_TOYPAD_BASE_ACTIVATE_VALUE;
    usb_buf[4]  = 0x01;
    usb_buf[5]  = 0x28; //'('
    usb_buf[6]  = 0x63; //'c'
    usb_buf[7]  = 0x29; //')'
    usb_buf[8]  = 0x20; //' '
    usb_buf[9]  = 0x4C; //'L'
    usb_buf[10] = 0x45; //'E'
    usb_buf[11] = 0x47; //'G'
    usb_buf[12] = 0x4F; //'O'
    usb_buf[13] = 0x20; //' '
    usb_buf[14] = 0x32; //'2'
    usb_buf[15] = 0x30; //'0'
    usb_buf[16] = 0x31; //'1'
    usb_buf[17] = 0x34; //'4'
    usb_buf[18] = 0xF7; // checksum

    hid_write(dev, usb_buf, LEGO_DIMENSIONS_TOYPAD_BASE_PACKET_LENGTH);
}

void LegoDimensionsToypadBaseController::SetDirect(unsigned char zone, RGBColor color)
{
    unsigned char usb_buf[LEGO_DIMENSIONS_TOYPAD_BASE_PACKET_LENGTH];

    memset(usb_buf, 0x00, LEGO_DIMENSIONS_TOYPAD_BASE_PACKET_LENGTH);

    usb_buf[1] = LEGO_DIMENSIONS_TOYPAD_BASE_REPORT_ID;
    usb_buf[2] = 0x06; // command length
    usb_buf[3] = LEGO_DIMENSIONS_TOYPAD_BASE_DIRECT_MODE_VALUE;
    usb_buf[4] = 0x02; // constant value
    usb_buf[5] = zone;
    usb_buf[6] = RGBGetRValue(color);
    usb_buf[7] = RGBGetGValue(color);
    usb_buf[8] = RGBGetBValue(color);

    for(unsigned int i = 1; i < 9; i ++)
    {
        usb_buf[9] += usb_buf[i];  // checksum
    }

    hid_write(dev, usb_buf, LEGO_DIMENSIONS_TOYPAD_BASE_PACKET_LENGTH);
}

void LegoDimensionsToypadBaseController::SetMode(unsigned char zone, unsigned char mode_value, uint8_t speed, RGBColor color)
{
    unsigned char usb_buf[LEGO_DIMENSIONS_TOYPAD_BASE_PACKET_LENGTH];

    memset(usb_buf, 0x00, LEGO_DIMENSIONS_TOYPAD_BASE_PACKET_LENGTH);

    usb_buf[1]  = LEGO_DIMENSIONS_TOYPAD_BASE_REPORT_ID;
    usb_buf[3]  = mode_value;
    usb_buf[5]  = zone;

    if(mode_value == LEGO_DIMENSIONS_TOYPAD_BASE_FLASH_MODE_VALUE)
    {
        usb_buf[2]  = 0x09;     // command length
        usb_buf[4]  = 0x1F;     // constant value
        usb_buf[6]  = speed;    // light on length
        usb_buf[7]  = speed;    // light off length
        usb_buf[8]  = 10;       // number of pulses
        usb_buf[9]  = RGBGetRValue(color);
        usb_buf[10] = RGBGetGValue(color);
        usb_buf[11] = RGBGetBValue(color);

        for(unsigned int i = 1; i < 12; i ++)
        {
            usb_buf[12] += usb_buf[i];  // checksum
        }
    }
    else if (mode_value == LEGO_DIMENSIONS_TOYPAD_BASE_FADE_MODE_VALUE)
    {
        usb_buf[2]  = 0x08;     // command length
        usb_buf[4]  = 0x0F;     // constant value
        usb_buf[6]  = speed;    // light on length
        usb_buf[7]  = 10;       // number of pulses
        usb_buf[8]  = RGBGetRValue(color);
        usb_buf[9]  = RGBGetGValue(color);
        usb_buf[10] = RGBGetBValue(color);

        for(unsigned int i = 1; i < 11; i ++)
        {
            usb_buf[11] += usb_buf[i];  // checksum
        }
    }

    hid_write(dev, usb_buf, LEGO_DIMENSIONS_TOYPAD_BASE_PACKET_LENGTH);
}
