/*---------------------------------------------------------*\
| RoccatBurstProAirController.cpp                           |
|                                                           |
|   Driver for Roccat Burst Pro Air                         |
|                                                           |
|   Morgan Guimard (morg)                       16 Jun 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "RoccatBurstProAirController.h"
#include "StringUtils.h"

RoccatBurstProAirController::RoccatBurstProAirController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev                 = dev_handle;
    location            = info.path;
    name                = dev_name;
}

RoccatBurstProAirController::~RoccatBurstProAirController()
{
    hid_close(dev);
}

std::string RoccatBurstProAirController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string RoccatBurstProAirController::GetNameString()
{
    return(name);
}

std::string RoccatBurstProAirController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void RoccatBurstProAirController::SetColors(std::vector<RGBColor> colors)
{
    unsigned char usb_buf[ROCCAT_BURST_PRO_AIR_REPORT_SIZE];
    memset(usb_buf, 0x00, ROCCAT_BURST_PRO_AIR_REPORT_SIZE);

    usb_buf[0] = ROCCAT_BURST_PRO_AIR_REPORT_ID;

    usb_buf[1] = 0x01;
    usb_buf[2] = 0x4C;
    usb_buf[3] = 0x06;
    usb_buf[4] = 0x14;

    for(unsigned char i = 0; i < colors.size(); i++)
    {
        usb_buf[5 + 5 * i] = i + 1;
        usb_buf[6 + 5 * i] = 0xFF;
        usb_buf[7 + 5 * i] = RGBGetRValue(colors[i]);
        usb_buf[8 + 5 * i] = RGBGetGValue(colors[i]);
        usb_buf[9 + 5 * i] = RGBGetBValue(colors[i]);
    }

    hid_send_feature_report(dev, usb_buf, ROCCAT_BURST_PRO_AIR_REPORT_SIZE);
}

void RoccatBurstProAirController::SetModeValues(unsigned char mode_value, unsigned char speed, unsigned char brightness)
{
    unsigned char usb_buf[ROCCAT_BURST_PRO_AIR_REPORT_SIZE];
    memset(usb_buf, 0x00, ROCCAT_BURST_PRO_AIR_REPORT_SIZE);

    usb_buf[0] = ROCCAT_BURST_PRO_AIR_REPORT_ID;

    usb_buf[1] = 0x01;
    usb_buf[2] = 0x4C;
    usb_buf[3] = 0x06;
    usb_buf[4] = 0x06;

    usb_buf[5] = mode_value;
    usb_buf[6] = speed;
    usb_buf[7] = brightness;

    usb_buf[8] = 0x0F;

    hid_send_feature_report(dev, usb_buf, ROCCAT_BURST_PRO_AIR_REPORT_SIZE);
}
