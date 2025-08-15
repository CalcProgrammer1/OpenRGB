/*---------------------------------------------------------*\
| VS_XG270QG_Controller.cpp                                 |
|                                                           |
|   Driver for ViewSonic XG270QG                            |
|                                                           |
|   Lanzaa                                      23 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "LogManager.h"
#include "StringUtils.h"
#include "VS_XG270QG_Controller.h"

VS_XG270QG_Controller::VS_XG270QG_Controller(hid_device* device, const char* path, std::string dev_name)
{
    dev         = device;
    location    = path;
    name        = dev_name;
}

VS_XG270QG_Controller::~VS_XG270QG_Controller()
{
    hid_close(dev);
}

std::string VS_XG270QG_Controller::GetLocation()
{
    return(location);
}

std::string VS_XG270QG_Controller::GetName()
{
    return(name);
}

std::string VS_XG270QG_Controller::GetSerial()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void VS_XG270QG_Controller::SetMode(uint8_t mode1, uint8_t r1, uint8_t g1, uint8_t b1, uint8_t mode2, uint8_t r2, uint8_t g2, uint8_t b2)
{
    SendModeComplete(mode1, r1, g1, b1, mode2, r2, g2, b2);
}

void VS_XG270QG_Controller::SendModeComplete
    (
    uint8_t mode1, uint8_t r1, uint8_t g1, uint8_t b1,
    uint8_t mode2, uint8_t r2, uint8_t g2, uint8_t b2
    )
{
    uint8_t data[] =
    {
        0x02,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x00,
        0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    data[0x01] = mode1; // Downward facing LEDs
    data[0x02] = r1;
    data[0x03] = g1;
    data[0x04] = b1;
    data[0x05] = 0x00;
    data[0x06] = 0x0A;
    data[0x07] = 0x00;

    data[0x08] = mode2; // Back facing LEDs
    data[0x09] = r2;
    data[0x0A] = g2;
    data[0x0B] = b2;
    data[0x0C] = 0x00;
    data[0x0D] = 0x0A; // Might be speed related

    // original data packets are 0x40=64 long
    SendCommand(data, 0x20);
}

void VS_XG270QG_Controller::SendCommand(uint8_t  *data, size_t length)
{
    hid_send_feature_report(dev, data, length);
}

