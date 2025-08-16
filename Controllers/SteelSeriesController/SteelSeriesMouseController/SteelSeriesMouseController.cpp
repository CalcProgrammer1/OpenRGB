/*---------------------------------------------------------*\
| SteelSeriesMouseController.cpp                            |
|                                                           |
|   Driver for SteelSeries Mouse                            |
|                                                           |
|   Chris M (Dr_No)                             09 Jun 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "SteelSeriesMouseController.h"
#include "StringUtils.h"

SteelSeriesMouseController::SteelSeriesMouseController(hid_device*  dev_handle, steelseries_type proto_type, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
    proto       = proto_type;
}

SteelSeriesMouseController::~SteelSeriesMouseController()
{

}

std::string SteelSeriesMouseController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string SteelSeriesMouseController::GetNameString()
{
    return(name);
}

std::string SteelSeriesMouseController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

steelseries_type SteelSeriesMouseController::GetMouseType()
{
    return proto;
}

void SteelSeriesMouseController::Save()
{
    const uint8_t   SAVE_BUFFER_SIZE            = 10;
    uint8_t         usb_buf[SAVE_BUFFER_SIZE]   = { 0x00, 0x09 };

    hid_write(dev, usb_buf, SAVE_BUFFER_SIZE);
}
