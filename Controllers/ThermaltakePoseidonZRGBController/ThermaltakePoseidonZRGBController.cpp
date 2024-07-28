/*---------------------------------------------------------*\
| ThermaltakePoseidonZRGBController.cpp                     |
|                                                           |
|   Driver for Thermaltake Poseidon Z RGB                   |
|                                                           |
|   Adam Honse (CalcProgrammer1)                25 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "StringUtils.h"
#include "ThermaltakePoseidonZRGBController.h"

using namespace std::chrono_literals;

static unsigned int keys[] = {0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x15,
                              0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23,
                              0x24, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31,
                              0x32, 0x33, 0x34, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3E, 0x3F, 0x40,
                              0x41, 0x42, 0x43, 0x44, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4E, 0x4F,
                              0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C,
                              0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x66, 0x67, 0x68, 0x69, 0x6A,
                              0x6C, 0x6D, 0x6F, 0x70, 0x72, 0x73, 0x75, 0x76, 0x77, 0x78, 0x7C, 0x80, 0x81 };

PoseidonZRGBController::PoseidonZRGBController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;
}

PoseidonZRGBController::~PoseidonZRGBController()
{
    hid_close(dev);
}

std::string PoseidonZRGBController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string PoseidonZRGBController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void PoseidonZRGBController::SetMode(unsigned char mode, unsigned char direction, unsigned char speed)
{
    active_mode = mode;
    active_direction = direction;
    active_speed = speed;

    SendControl
        (
        POSEIDONZ_PROFILE_P1,
        POSEIDONZ_PROFILE_P1,
        active_direction,
        active_mode,
        POSEIDONZ_BRIGHTNESS_MAX,
        active_speed
        );

    std::this_thread::sleep_for(200ms);
}

void PoseidonZRGBController::SetLEDsDirect(std::vector<RGBColor> colors)
{
    unsigned char red_grn_buf[264];
    unsigned char blu_buf[264];

    /*-----------------------------------------------------*\
    | Zero out buffers                                      |
    \*-----------------------------------------------------*/
    memset(red_grn_buf, 0x00, sizeof(red_grn_buf));
    memset(blu_buf, 0x00, sizeof(blu_buf));

    /*-----------------------------------------------------*\
    | Set up Direct packets                                 |
    \*-----------------------------------------------------*/
    red_grn_buf[0] = 0x07;
    red_grn_buf[1] = POSEIDONZ_PACKET_ID_SET_DIRECT;
    red_grn_buf[2] = POSEIDONZ_PROFILE_P1;
    red_grn_buf[3] = POSEIDONZ_DIRECT_RED_GREEN;
    red_grn_buf[4] = 0x00;
    red_grn_buf[5] = 0x00;
    red_grn_buf[6] = 0x00;
    red_grn_buf[7] = 0x00;

    blu_buf[0] = 0x07;
    blu_buf[1] = POSEIDONZ_PACKET_ID_SET_DIRECT;
    blu_buf[2] = POSEIDONZ_PROFILE_P1;
    blu_buf[3] = POSEIDONZ_DIRECT_BLUE;
    blu_buf[4] = 0x00;
    blu_buf[5] = 0x00;
    blu_buf[6] = 0x00;
    blu_buf[7] = 0x00;

    /*-----------------------------------------------------*\
    | Fill in color data                                    |
    \*-----------------------------------------------------*/
    for(std::size_t i = 0; i < 104; i++)
    {
        red_grn_buf[keys[i]      ] = RGBGetRValue(colors[i]);
        red_grn_buf[keys[i] + 128] = RGBGetGValue(colors[i]);
        blu_buf[    keys[i]      ] = RGBGetBValue(colors[i]);
    }

    /*-----------------------------------------------------*\
    | Send packets                                          |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, red_grn_buf, 264);

    std::this_thread::sleep_for(5ms);

    hid_send_feature_report(dev, blu_buf, 264);
}

void PoseidonZRGBController::SetLEDs(std::vector<RGBColor> colors)
{
    unsigned char red_color_data[104];
    unsigned char grn_color_data[104];
    unsigned char blu_color_data[104];

    for(std::size_t i = 0; i < 104; i++)
    {
        red_color_data[i] = RGBGetRValue(colors[i]);
        grn_color_data[i] = RGBGetGValue(colors[i]);
        blu_color_data[i] = RGBGetBValue(colors[i]);
    }

    SendColor
        (
        POSEIDONZ_PROFILE_P1,
        POSEIDONZ_COLOR_RED,
        red_color_data
        );

    std::this_thread::sleep_for(10ms);

    SendColor
        (
        POSEIDONZ_PROFILE_P1,
        POSEIDONZ_COLOR_GREEN,
        grn_color_data
        );

    std::this_thread::sleep_for(10ms);

    SendColor
        (
        POSEIDONZ_PROFILE_P1,
        POSEIDONZ_COLOR_BLUE,
        blu_color_data
        );

    std::this_thread::sleep_for(10ms);

    SendControl
        (
        POSEIDONZ_PROFILE_P1,
        POSEIDONZ_PROFILE_P1,
        active_direction,
        active_mode,
        POSEIDONZ_BRIGHTNESS_MAX,
        active_speed
        );
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void PoseidonZRGBController::SendColor
    (
    unsigned char   profile_to_edit,
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
    | Set up Color packet                                   |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x07;
    buf[0x01]   = POSEIDONZ_PACKET_ID_SET_COLOR;
    buf[0x02]   = profile_to_edit;
    buf[0x03]   = color_channel;

    /*-----------------------------------------------------*\
    | Fill in color data                                    |
    \*-----------------------------------------------------*/
    for(int i = 0; i < 104; i++)
    {
        buf[keys[i]] = color_data[i];
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 264);
}

void PoseidonZRGBController::SendControl
    (
    unsigned char   profile_to_activate,
    unsigned char   profile_to_edit,
    unsigned char   direction,
    unsigned char   mode,
    unsigned char   brightness,
    unsigned char   speed
    )
{
    unsigned char buf[264];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Effect packet                                  |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x07;
    buf[0x01]   = POSEIDONZ_PACKET_ID_SET_EFFECT;
    buf[0x02]   = profile_to_activate;
    buf[0x08]   = profile_to_edit;
    buf[0x0A]   = direction;
    buf[0x0C]   = mode;
    buf[0x0D]   = brightness;
    buf[0x10]   = 0x08;
    buf[0x12]   = speed;
    buf[0x13]   = 0x50;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 264);
}
