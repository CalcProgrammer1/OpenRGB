/*---------------------------------------------------------*\
| SinowealthKeyboard10cController.cpp                       |
|                                                           |
|   Driver for Sinowealth Keyboards with PID 010C           |
|                                                           |
|   Rodrigo Tavares                             27 Nov 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include "SinowealthKeyboard10cController.h"
#include "RGBController.h"
#include "StringUtils.h"

using namespace kbd10c;

SinowealthKeyboard10cController::SinowealthKeyboard10cController(hid_device* dev_handle, char* path,
                                                                 std::string dev_name)
{
    dev  = dev_handle;
    name = dev_name;

    current_mode = MODE_DIRECT;

    location = path;
}

SinowealthKeyboard10cController::~SinowealthKeyboard10cController()
{
    hid_close(dev);
}

std::string SinowealthKeyboard10cController::GetLocation()
{
    return ("HID: " + location);
}

std::string SinowealthKeyboard10cController::GetName()
{
    return (name);
}

unsigned char SinowealthKeyboard10cController::GetCurrentMode()
{
    return current_mode;
}

std::string SinowealthKeyboard10cController::GetSerialString()
{
    wchar_t serial_string[128];
    int     ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return ("");
    }

    return (StringUtils::wstring_to_string(serial_string));
}

void SinowealthKeyboard10cController::SetLEDsDirect(std::vector<RGBColor> colors)
{
    const int     buffer_size = 520;
    unsigned char buf[buffer_size];
    memset(buf, 0x00, buffer_size);

    buf[0x00] = 0x06;
    buf[0x01] = 0x08;
    buf[0x04] = 0x01;
    buf[0x06] = 0x7A;
    buf[0x07] = 0x01;

    for(size_t i = 0; i < colors.size(); ++i)
    {
        buf[0x08 + i * 3]     = RGBGetRValue(colors[i]);
        buf[0x08 + i * 3 + 1] = RGBGetGValue(colors[i]);
        buf[0x08 + i * 3 + 2] = RGBGetBValue(colors[i]);
    }

    hid_send_feature_report(dev, buf, buffer_size);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}
