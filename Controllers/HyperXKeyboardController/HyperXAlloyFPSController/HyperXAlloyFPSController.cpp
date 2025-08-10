/*---------------------------------------------------------*\
| HyperXAlloyFPSController.cpp                              |
|                                                           |
|   Driver for HyperX Alloy FPS keyboard                    |
|                                                           |
|   Adam Honse (CalcProgrammer1)                30 Jan 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "HyperXAlloyFPSController.h"
#include "StringUtils.h"

using namespace std::chrono_literals;

static unsigned int keys[] = {0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14,
                              0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x20, 0x21, 0x22,
                              0x23, 0x24, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30,
                              0x31, 0x32, 0x33, 0x34, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3E, 0x3F, 0x41,
                              0x44, 0x45, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x51, 0x54, 0x55,
                              0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5E, 0x5F, 0x61, 0x64, 0x65, 0x68, 0x69, 0x6A,
                              0x6B, 0x6C, 0x6E, 0x6F, 0x74, 0x75, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E,
                              0x7F, 0x81, 0x84, 0x85, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x91,
                              0x94, 0x95 };

HyperXAlloyFPSController::HyperXAlloyFPSController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

HyperXAlloyFPSController::~HyperXAlloyFPSController()
{
    hid_close(dev);
}

std::string HyperXAlloyFPSController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string HyperXAlloyFPSController::GetNameString()
{
    return(name);
}

std::string HyperXAlloyFPSController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void HyperXAlloyFPSController::SetLEDsDirect(std::vector<RGBColor> colors)
{
    unsigned char red_color_data[106];
    unsigned char grn_color_data[106];
    unsigned char blu_color_data[106];

    for(std::size_t i = 0; i < 106; i++)
    {
        red_color_data[i] = RGBGetRValue(colors[i]);
        grn_color_data[i] = RGBGetGValue(colors[i]);
        blu_color_data[i] = RGBGetBValue(colors[i]);
    }

    SendDirect
        (
        HYPERX_ALLOY_FPS_COLOR_CHANNEL_RED,
        red_color_data
        );

    std::this_thread::sleep_for(10ms);

    SendDirect
        (
        HYPERX_ALLOY_FPS_COLOR_CHANNEL_GREEN,
        grn_color_data
        );

    std::this_thread::sleep_for(10ms);

    SendDirect
        (
        HYPERX_ALLOY_FPS_COLOR_CHANNEL_BLUE,
        blu_color_data
        );
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void HyperXAlloyFPSController::SendDirect
    (
    unsigned char   color_channel,
    unsigned char*  color_data
    )
{
    unsigned char buf[264];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Direct packet                                  |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x07;
    buf[0x01]   = HYPERX_ALLOY_FPS_PACKET_ID_DIRECT;
    buf[0x02]   = color_channel;
    buf[0x03]   = 0xA0;

    /*-----------------------------------------------------*\
    | Fill in color data                                    |
    \*-----------------------------------------------------*/
    for(int i = 0; i < 106; i++)
    {
        buf[keys[i]] = color_data[i];
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 264);
}
