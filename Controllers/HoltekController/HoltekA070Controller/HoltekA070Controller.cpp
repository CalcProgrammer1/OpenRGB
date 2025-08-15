/*---------------------------------------------------------*\
| HoltekA070Controller.cpp                                  |
|                                                           |
|   Driver for Holtek mouse                                 |
|                                                           |
|   Santeri Pikarinen (santeri3700)             01 Aug 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "HoltekA070Controller.h"
#include "StringUtils.h"

HoltekA070Controller::HoltekA070Controller(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

HoltekA070Controller::~HoltekA070Controller()
{
    hid_close(dev);
}

std::string HoltekA070Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string HoltekA070Controller::GetNameString()
{
    return(name);
}

std::string HoltekA070Controller::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void HoltekA070Controller::SendCustomColor
    (
    unsigned char       red,
    unsigned char       green,
    unsigned char       blue
    )
{
    char usb_buf[8];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up RGB Control packet                             |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x07;  // PACKET SIZE?
    usb_buf[0x01]           = 0x0a;  // SET RGB
    usb_buf[0x02]           = 0x00;  // SAVE (does not work with SET RGB)
    usb_buf[0x03]           = red;   // RED
    usb_buf[0x04]           = green; // GREEN
    usb_buf[0x05]           = blue;  // BLUE
    usb_buf[0x06]           = 0x00;  // PADDING?
    usb_buf[0x07]           = 0x00;  // PADDING?

    // So far no saving function has been discovered for the "SET RGB" command.
    // Such functionality might not even exist for the A070 series.
    // The chosen RGB color will therefore reset after a power cycle.

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, (unsigned char *)usb_buf, sizeof(usb_buf));
}

void HoltekA070Controller::SendMode
    (
    unsigned char       mode
    )
{

    char usb_buf[8];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up lighting mode control packet                   |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x07; // PACKET SIZE?
    usb_buf[0x01]           = 0x0b; // SET LIGHTING MODE
    usb_buf[0x02]           = 0x01; // SAVE
    usb_buf[0x03]           = mode; // MODE 01-04
    usb_buf[0x04]           = 0x00; // PADDING?
    usb_buf[0x05]           = 0x00; // PADDING?
    usb_buf[0x06]           = 0x00; // PADDING?
    usb_buf[0x07]           = 0x00; // PADDING?

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, (unsigned char *)usb_buf, sizeof(usb_buf));
}
