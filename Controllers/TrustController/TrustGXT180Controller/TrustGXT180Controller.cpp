/*---------------------------------------------------------*\
| TrustGXT180Controller.cpp                                 |
|                                                           |
|   Driver for Trust GXT 180                                |
|                                                           |
|   Morgan Guimard (morg)                       24 Mar 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string.h>
#include "StringUtils.h"
#include "TrustGXT180Controller.h"

TrustGXT180Controller::TrustGXT180Controller(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev                 = dev_handle;
    location            = info.path;
    name                = dev_name;
}

TrustGXT180Controller::~TrustGXT180Controller()
{
    hid_close(dev);
}

std::string TrustGXT180Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string TrustGXT180Controller::GetNameString()
{
    return(name);
}

std::string TrustGXT180Controller::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void TrustGXT180Controller::SetMode(RGBColor color, unsigned char brightness, unsigned char speed, unsigned char mode_value)
{
    /*-----------------------------------------*\
    | Create and zero out the buffer            |
    \*-----------------------------------------*/
    unsigned char usb_buf[TRUST_GXT_180_REPORT_SIZE];
    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0]          = TRUST_GXT_180_REPORT_ID;
    usb_buf[1]          = 0x06;
    usb_buf[2]          = 0xBB;
    usb_buf[3]          = 0xAA;
    usb_buf[4]          = 0x2A;
    usb_buf[6]          = 0x0A;

    if(mode_value != TRUST_GXT_180_OFF_MODE_VALUE)
    {
        usb_buf[8]      = RGBGetRValue(color);
        usb_buf[9]      = RGBGetGValue(color);
        usb_buf[10]     = RGBGetBValue(color);
    }

    usb_buf[11]         = mode_value;
    usb_buf[13]         = brightness;
    usb_buf[14]         = speed;

    /*-----------------------------------------*\
    | Send the feature report                   |
    \*-----------------------------------------*/
    hid_send_feature_report(dev, usb_buf, TRUST_GXT_180_REPORT_SIZE);
}
