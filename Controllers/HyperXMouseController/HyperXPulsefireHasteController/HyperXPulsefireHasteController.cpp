/*---------------------------------------------------------*\
| HyperXPulsefireHasteController.cpp                        |
|                                                           |
|   Driver for HyperX Pulsefire Haste                       |
|                                                           |
|   Adam Honse (CalcProgrammer1)                19 Aug 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "HyperXPulsefireHasteController.h"
#include "StringUtils.h"

HyperXPulsefireHasteController::HyperXPulsefireHasteController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

HyperXPulsefireHasteController::~HyperXPulsefireHasteController()
{
    hid_close(dev);
}

std::string HyperXPulsefireHasteController::GetDeviceLocation()
{
    return("HID " + location);
}

std::string HyperXPulsefireHasteController::GetNameString()
{
    return(name);
}

std::string HyperXPulsefireHasteController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void HyperXPulsefireHasteController::SendDirect
    (
    RGBColor*       color_data
    )
{
    SendDirectSetup();
    SendDirectColor(color_data);
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void HyperXPulsefireHasteController::SendDirectSetup()
{
    unsigned char buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Direct Mode Setup packet                       |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x00;
    buf[0x01]   = HYPERX_PULSEFIRE_HASTE_PACKET_ID_SETUP;
    buf[0x02]   = 0xF2;

    buf[0x08]   = 0x02;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 65);
}

void HyperXPulsefireHasteController::SendDirectColor
    (
    RGBColor*       color_data
    )
{
    unsigned char buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Direct Mode packet                             |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x00;
    buf[0x01]   = HYPERX_PULSEFIRE_HASTE_PACKET_ID_COLOR;

    buf[0x02]   = RGBGetRValue(color_data[0]);
    buf[0x03]   = RGBGetGValue(color_data[0]);
    buf[0x04]   = RGBGetBValue(color_data[0]);

    buf[0x08]   = 0x02;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 65);
}
