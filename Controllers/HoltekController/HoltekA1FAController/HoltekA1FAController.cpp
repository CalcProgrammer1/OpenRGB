/*---------------------------------------------------------*\
| HoltekA1FAController.cpp                                  |
|                                                           |
|   Driver for Holtek mousemat                              |
|                                                           |
|   Edoardo Ridolfi (edo2313)                   26 Dec 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "HoltekA1FAController.h"
#include "StringUtils.h"

HoltekA1FAController::HoltekA1FAController(hid_device *dev_handle, const char *path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

HoltekA1FAController::~HoltekA1FAController()
{
    hid_close(dev);
}

std::string HoltekA1FAController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string HoltekA1FAController::GetNameString()
{
    return(name);
}

std::string HoltekA1FAController::GetSerialString()
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

void HoltekA1FAController::SendData(unsigned char mode, unsigned char brightness, unsigned char speed, unsigned char preset, unsigned char red, unsigned char green, unsigned char blue)
{
    char usb_buf[9] = {0x00};

    /*-----------------------------------------------------*\
    | Set up RGB Control packet                             |
    \*-----------------------------------------------------*/
    usb_buf[HOLTEK_A1FA_BYTE_COMMAND]       = 0x08;
    usb_buf[HOLTEK_A1FA_BYTE_MODE]          = mode;
    usb_buf[HOLTEK_A1FA_BYTE_BRIGHTNESS]    = brightness;
    usb_buf[HOLTEK_A1FA_BYTE_SPEED]         = speed;
    usb_buf[HOLTEK_A1FA_BYTE_PRESET]        = preset;
    usb_buf[HOLTEK_A1FA_BYTE_RED]           = red;
    usb_buf[HOLTEK_A1FA_BYTE_GREEN]         = green;
    usb_buf[HOLTEK_A1FA_BYTE_BLUE]          = blue;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, (unsigned char *)usb_buf, sizeof(usb_buf));
}
