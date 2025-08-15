/*---------------------------------------------------------*\
| TrustGXT114Controller.cpp                                 |
|                                                           |
|   Driver for Trust GXT 114                                |
|                                                           |
|   Morgan Guimard (morg)                       24 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string.h>
#include "StringUtils.h"
#include "TrustGXT114Controller.h"

TrustGXT114Controller::TrustGXT114Controller(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev             = dev_handle;
    location        = info.path;
    name            = dev_name;
}

TrustGXT114Controller::~TrustGXT114Controller()
{
    hid_close(dev);
}

std::string TrustGXT114Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string TrustGXT114Controller::GetNameString()
{
    return(name);
}

std::string TrustGXT114Controller::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

bool TrustGXT114Controller::Test()
{
    /*-----------------------------------------*\
    | Send a get feature report, filtering out  |
    | hid devices that do not anwser.           |
    \*-----------------------------------------*/
    uint8_t usb_buf[TRUST_GXT_114_REPORT_SIZE] = { TRUST_GXT_114_REPORT_ID };
    return hid_get_feature_report(dev, usb_buf, TRUST_GXT_114_REPORT_SIZE) > 0;
}

void TrustGXT114Controller::SetMode(RGBColor color, unsigned char brightness, unsigned char speed, unsigned char mode_value)
{
    unsigned char speed_bright = mode_value == STATIC_MODE_VALUE ? brightness : speed;

    /*-----------------------------------------*\
    | Create and zero out the buffer            |
    \*-----------------------------------------*/
    unsigned char usb_buf[TRUST_GXT_114_REPORT_SIZE];
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------*\
    | Fill dynamic data                         |
    \*-----------------------------------------*/
    usb_buf[0]      = TRUST_GXT_114_REPORT_ID;

    usb_buf[93]     = mode_value;
    usb_buf[94]     = 0x00;                         // freq (extra param, let's default it)
    usb_buf[95]     = 0x00;                         // times (extra param, let's default it)
    usb_buf[96]     = speed_bright;                 // speed or brightness depending on the mode

    usb_buf[103]    = RGBGetRValue(color);          // r
    usb_buf[104]    = RGBGetGValue(color);          // g
    usb_buf[105]    = RGBGetBValue(color);          // b

    /*-----------------------------------------*\
    | Fill the constant data bytes              |
    \*-----------------------------------------*/
    for(unsigned int i = 79 ; i <= 89; i++)
    {
         usb_buf[i] = 0x80;
    }

    std::vector<unsigned char> values_FF =
    {
        101, 108, 109, 113, 114, 115, 117, 118, 119, 124, 127, 128,
        131, 134, 135, 138, 139, 141, 142, 143, 144
    };

    for(unsigned i = 0; i < values_FF.size(); i ++)
    {
        usb_buf[values_FF[i]] = 0xff;
    }

    usb_buf[8]      = 0x4c;                         // constant data

    usb_buf[71]     = 0x11;                         // constant data
    usb_buf[72]     = 0xb0;                         // constant data
    usb_buf[74]     = 0x89;                         // constant data
    usb_buf[75]     = 0x0e;                         // constant data
    usb_buf[76]     = 0x9d;                         // constant data
    usb_buf[77]     = 0xa7;                         // constant data
    usb_buf[78]     = 0xb7;                         // constant data

    usb_buf[148]    = 0x58;                         // constant data
    usb_buf[149]    = 0x30;                         // constant data
    usb_buf[150]    = 0x31;                         // constant data
    usb_buf[151]    = 0x30;                         // constant data
    usb_buf[152]    = 0x31;                         // constant data
    usb_buf[153]    = 0x30;                         // constant data

    /*-----------------------------------------*\
    | Send the feature report                   |
    \*-----------------------------------------*/
    hid_send_feature_report(dev, usb_buf, TRUST_GXT_114_REPORT_SIZE);
}
