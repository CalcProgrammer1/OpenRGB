/*---------------------------------------------------------*\
| CorsairWirelessController.cpp                             |
|                                                           |
|   Driver for Corsair wireless keyboard                    |
|                                                           |
|   Adam Honse (CalcProgrammer1)                08 May 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "CorsairWirelessController.h"
#include "StringUtils.h"

using namespace std::chrono_literals;

CorsairWirelessController::CorsairWirelessController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;

    type        = DEVICE_TYPE_KEYBOARD;

    EnterDirectMode();
}

CorsairWirelessController::~CorsairWirelessController()
{
    hid_close(dev);
}

device_type CorsairWirelessController::GetDeviceType()
{
    return type;
}

std::string CorsairWirelessController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string CorsairWirelessController::GetFirmwareString()
{
    return firmware_version;
}

std::string CorsairWirelessController::GetName()
{
    return name;
}

std::string CorsairWirelessController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void CorsairWirelessController::SetLEDs(std::vector<RGBColor>colors)
{
    unsigned char buf[3 * 137];

    for(int color = 0; color < 137; color++)
    {
        buf[0 + color]   = RGBGetRValue(colors[color]);
        buf[137 + color] = RGBGetGValue(colors[color]);
        buf[274+color]   = RGBGetBValue(colors[color]);
    }

    StartDirectMode();

    SendDirectFrame(true,  &buf[0]);
    SendDirectFrame(false, &buf[57]);
    SendDirectFrame(false, &buf[118]);
    SendDirectFrame(false, &buf[179]);
    SendDirectFrame(false, &buf[240]);
    SendDirectFrame(false, &buf[301]);
    SendDirectFrame(false, &buf[362]);
}

void CorsairWirelessController::SetName(std::string device_name)
{
    name = device_name;
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void CorsairWirelessController::EnterDirectMode()
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Submit Mouse Colors packet                     |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x80;
    usb_buf[0x02]   = 0x01;
    usb_buf[0x03]   = 0x03;
    usb_buf[0x04]   = 0x00;
    usb_buf[0x05]   = 0x02;

    /*-----------------------------------------------------*\
    | Send packet using feature reports, as headset stand   |
    | seems to not update completely using HID writes       |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);

}

void CorsairWirelessController::StartDirectMode()
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Submit Mouse Colors packet                     |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x80;
    usb_buf[0x02]   = 0x0D;
    usb_buf[0x03]   = 0x00;
    usb_buf[0x04]   = 0x01;

    /*-----------------------------------------------------*\
    | Send packet using feature reports, as headset stand   |
    | seems to not update completely using HID writes       |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);

}

void CorsairWirelessController::ExitDirectMode()
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Submit Mouse Colors packet                     |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x80;
    usb_buf[0x02]   = 0x01;
    usb_buf[0x03]   = 0x03;
    usb_buf[0x04]   = 0x00;
    usb_buf[0x05]   = 0x01;

    /*-----------------------------------------------------*\
    | Send packet using feature reports, as headset stand   |
    | seems to not update completely using HID writes       |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);

}

void CorsairWirelessController::SendDirectFrame(bool first_frame, unsigned char* data)
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Submit Mouse Colors packet                     |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x80;
    usb_buf[0x02]   = first_frame ? 0x06 : 0x07;
    usb_buf[0x03]   = 0x00;

    if(first_frame)
    {
        usb_buf[0x04]   = 0x9B;
        usb_buf[0x05]   = 0x01;
    }

    /*-----------------------------------------------------*\
    | Copy in colors in <RED> <GREEN> <BLUE> order          |
    \*-----------------------------------------------------*/
    if(first_frame)
    {
        memcpy(&usb_buf[0x08], data, 57);
    }
    else
    {
        memcpy(&usb_buf[0x04], data, 61);
    }

    /*-----------------------------------------------------*\
    | Send packet using feature reports, as headset stand   |
    | seems to not update completely using HID writes       |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
}
