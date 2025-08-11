/*---------------------------------------------------------*\
| HyperXPulsefireRaidController.cpp                         |
|                                                           |
|   Driver for HyperX Pulsefire Raid                        |
|                                                           |
|   Morgan Guimard (morg)                       06 Apr 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "HyperXPulsefireRaidController.h"
#include "StringUtils.h"

using namespace std::chrono_literals;

HyperXPulsefireRaidController::HyperXPulsefireRaidController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev                 = dev_handle;
    location            = info.path;
    name                = dev_name;
}

HyperXPulsefireRaidController::~HyperXPulsefireRaidController()
{
    hid_close(dev);
}

std::string HyperXPulsefireRaidController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string HyperXPulsefireRaidController::GetNameString()
{
    return(name);
}

std::string HyperXPulsefireRaidController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void HyperXPulsefireRaidController::SendColors(std::vector<RGBColor> colors)
{
    unsigned char usb_buf[HYPERX_PULSFIRE_RAID_PACKET_DATA_LENGTH];
    memset(usb_buf, 0x00, HYPERX_PULSFIRE_RAID_PACKET_DATA_LENGTH);

    usb_buf[0] = HYPERX_PULSFIRE_RAID_REPORT_ID;
    usb_buf[1] = HYPERX_PULSFIRE_RAID_DIRECT_MODE_START_PACKET;

    for(unsigned int i = 0; i < 2; i++)
    {
        usb_buf[3 * i + 2] = RGBGetRValue(colors[i]);
        usb_buf[3 * i + 3] = RGBGetGValue(colors[i]);
        usb_buf[3 * i + 4] = RGBGetBValue(colors[i]);
    }

    usb_buf[8] = HYPERX_PULSFIRE_RAID_DIRECT_MODE_END_PACKET;

    Send(usb_buf);
}


void HyperXPulsefireRaidController::Send(unsigned char* packet)
{
    hid_send_feature_report(dev, packet, HYPERX_PULSFIRE_RAID_PACKET_DATA_LENGTH);
    std::this_thread::sleep_for(10ms);
}
