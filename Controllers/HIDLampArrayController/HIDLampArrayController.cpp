/*---------------------------------------------------------*\
| HIDLampArrayController.cpp                                |
|                                                           |
|   Driver for HID LampArray Devices                        |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      26 Mar 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "HIDLampArrayController.h"

HIDLampArrayController::HIDLampArrayController(lamparray_hid_devs *devs_handle)
{
    devs = devs_handle;

    GetLampArrayAttributesReport();
}

HIDLampArrayController::~HIDLampArrayController()
{

}

std::string HIDLampArrayController::GetDeviceLocation()
{
    return("");
}

std::string HIDLampArrayController::GetSerialString()
{
    return("");
}

unsigned int HIDLampArrayController::GetLampCount()
{
    return(lamp_count);
}

void HIDLampArrayController::GetLampArrayAttributesReport()
{
    unsigned char   usb_buf[65];
    int             report_size;

    memset(usb_buf, 0, sizeof(usb_buf));

    usb_buf[0] = 0x01;

    report_size = hid_get_feature_report(devs->hid_dev_LampArrayAttributesReport, usb_buf, sizeof(usb_buf));

    lamp_count      = (unsigned short)((usb_buf[2] << 8) | usb_buf[1]);
    bounding_width  = (unsigned int)((usb_buf[6] << 24) | (usb_buf[5] << 16) | (usb_buf[4] << 8) | usb_buf[3]);
    bounding_height = (unsigned int)((usb_buf[10] << 24) | (usb_buf[9] << 16) | (usb_buf[8] << 8) | usb_buf[7]);
    bounding_depth  = (unsigned int)((usb_buf[14] << 24) | (usb_buf[13] << 16) | (usb_buf[12] << 8) | usb_buf[11]);
    kind            = (unsigned int)((usb_buf[18] << 24) | (usb_buf[17] << 16) | (usb_buf[16] << 8) | usb_buf[15]);
    min_interval    = (unsigned int)((usb_buf[22] << 24) | (usb_buf[21] << 16) | (usb_buf[20] << 8) | usb_buf[19]);
}
