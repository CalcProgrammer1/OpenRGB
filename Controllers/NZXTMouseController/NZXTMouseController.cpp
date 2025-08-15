/*---------------------------------------------------------*\
| NZXTMouseController.cpp                                   |
|                                                           |
|   Driver for NZXT Mouse                                   |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      13 Dec 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "NZXTMouseController.h"
#include "StringUtils.h"

NZXTMouseController::NZXTMouseController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;

    /*-----------------------------------------------------*\
    | Request firmware version                              |
    \*-----------------------------------------------------*/
    SendFirmwareRequest();
}

NZXTMouseController::~NZXTMouseController()
{
    hid_close(dev);
}

std::string NZXTMouseController::GetFirmwareVersion()
{
    return(firmware_version);
}

std::string NZXTMouseController::GetLocation()
{
    return("HID: " + location);
}

std::string NZXTMouseController::GetName()
{
    return(name);
}

std::string NZXTMouseController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void NZXTMouseController::SetLEDs
    (
    RGBColor *      colors
    )
{
    unsigned char   usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Direct packet                                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x43;
    usb_buf[0x01]   = 0xAE;
    usb_buf[0x03]   = 0x10;
    usb_buf[0x04]   = 0x02;
    usb_buf[0x05]   = 0x3F;

    usb_buf[24]     = 0x06;

    usb_buf[25]     = RGBGetRValue(colors[2]);
    usb_buf[26]     = RGBGetGValue(colors[2]);
    usb_buf[27]     = RGBGetBValue(colors[2]);

    usb_buf[29]     = RGBGetRValue(colors[1]);
    usb_buf[30]     = RGBGetGValue(colors[1]);
    usb_buf[31]     = RGBGetBValue(colors[1]);

    usb_buf[33]     = RGBGetRValue(colors[0]);
    usb_buf[34]     = RGBGetGValue(colors[0]);
    usb_buf[35]     = RGBGetBValue(colors[0]);

    usb_buf[37]     = RGBGetRValue(colors[3]);
    usb_buf[38]     = RGBGetGValue(colors[3]);
    usb_buf[39]     = RGBGetBValue(colors[3]);

    usb_buf[41]     = RGBGetRValue(colors[4]);
    usb_buf[42]     = RGBGetGValue(colors[4]);
    usb_buf[43]     = RGBGetBValue(colors[4]);

    usb_buf[45]     = RGBGetRValue(colors[5]);
    usb_buf[46]     = RGBGetGValue(colors[5]);
    usb_buf[47]     = RGBGetBValue(colors[5]);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, sizeof(usb_buf));
}

void NZXTMouseController::SendFirmwareRequest()
{
    unsigned char   usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Firmware Request packet                        |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x43;
    usb_buf[0x01]   = 0x81;
    usb_buf[0x03]   = 0x01;

    hid_write(dev, usb_buf, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Receive packets until 0x43 0x86 is received           |
    \*-----------------------------------------------------*/
    do
    {
        hid_read(dev, usb_buf, sizeof(usb_buf));
    } while( (usb_buf[0] != 0x43) || (usb_buf[1] != 0x86) );

    /*-----------------------------------------------------*\
    | Format firmware version string                        |
    \*-----------------------------------------------------*/
    snprintf(firmware_version, 16, "%u.%u.%u", usb_buf[0x03], usb_buf[0x04], usb_buf[0x05]);
}
