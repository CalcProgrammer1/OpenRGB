/*---------------------------------------------------------*\
| LaviewTechnologyController.cpp                            |
|                                                           |
|   Driver for Laview Tech. mice, including Glorious        |
|                                                           |
|   Kosta A (kostaarvanitis)                    27 Oct 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include <cmath>
#include "LogManager.h"
#include "LaviewTechnologyController.h"
#include "StringUtils.h"

LaviewTechnologyController::LaviewTechnologyController(hid_device* dev_handle, hid_device_info* dev_info, std::string dev_name)
{
    device      = dev_handle;
    location    = dev_info->path;
    name        = dev_name;
    version     = dev_info->release_number;
    vendor      = StringUtils::wstring_to_string(dev_info->manufacturer_string);
    serial      = StringUtils::wstring_to_string(dev_info->serial_number);
}

LaviewTechnologyController::~LaviewTechnologyController()
{
    hid_close(device);
}

std::string LaviewTechnologyController::GetLocation()
{
    return("HID: " + location);
}

std::string LaviewTechnologyController::GetName()
{
    return(name);
}

std::string LaviewTechnologyController::GetVendor()
{
    return(vendor);
}

std::string LaviewTechnologyController::GetSerialNumber()
{
    return(serial);
}

std::string LaviewTechnologyController::GetFirmwareVersion()
{
    return std::to_string(version);
}

void LaviewTechnologyController::SetMode(unsigned int mode, unsigned int brightness, unsigned int speed, RGBColor* color)
{
    uint8_t buf[LAVIEW_TECHNOLOGY_REPORT_SIZE];
    memset(buf, 0x00, sizeof(buf));

    buf[0] = 0xA1;
    buf[1] = 0x0C;
    buf[5] = 0x01; // Profile

    buf[16] = mode;
    buf[56] = std::clamp(0u, brightness, 64u);
    buf[58] = std::clamp(0u, speed, 64u);
    buf[60] = 0; // Set for some modes???

    switch (mode)
    {
    case LAVIEW_TECHNOLOGY_MODE_STATIC:
    case LAVIEW_TECHNOLOGY_MODE_BREATHING:
        buf[17] = RGBGetRValue(color[0]);
        buf[18] = RGBGetGValue(color[0]);
        buf[19] = RGBGetBValue(color[0]);
        break;
    default:
        /*----------------------------------------------------------------*\
        | For these reports we inject the default color scheme into the   |
        | availble rgb values which in the official control application   |
        | are not present in some modes causing them to not function.     | 
        | There is no mechanism in the control software for manipulating  |
        | these values, as such this functionality is also omitted here.  |
        \*----------------------------------------------------------------*/
        const unsigned char bytes[] = {
            0xFF, 0x00, 0x00, 0xFF, 0xA5, 0x00, 0xFF, 0xFF, 0x00, 
            0x00, 0xFF, 0x00, 0x00, 0x7F, 0xFF, 0x00, 0x00, 0xFF, 
            0x8B, 0x00, 0xFF 
        };
        memcpy(&buf[17], bytes, sizeof(bytes));
        break;
    };

    int ret = hid_send_feature_report(device, buf, sizeof(buf));
    if(ret < 0)
    {
        LOG_ERROR("[%s] Failure to send report (%ls)!", name.c_str(), hid_error(device));
    }
}