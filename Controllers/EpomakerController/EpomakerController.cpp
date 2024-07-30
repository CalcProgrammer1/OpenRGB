/*---------------------------------------------------------*\
| EpomakerController.cpp                                    |
|                                                           |
|   Driver for Epomaker keyboard                            |
|                                                           |
|   Alvaro Munoz (alvaromunoz)                  05 Jun 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <math.h>
#include "EpomakerController.h"
#include "LogManager.h"
#include "StringUtils.h"

EpomakerController::EpomakerController(hid_device* dev_handle, char *_path)
{
    dev         = dev_handle;
    location    = _path;

    /*---------------------------------------------------------*\
    | Get device name from HID manufacturer and product strings |
    \*---------------------------------------------------------*/
    wchar_t name_string[HID_MAX_STR];

    hid_get_manufacturer_string(dev, name_string, HID_MAX_STR);
    device_name = StringUtils::wstring_to_string(name_string);

    hid_get_product_string(dev, name_string, HID_MAX_STR);
    device_name.append(" ").append(StringUtils::wstring_to_string(name_string));

    current_mode        = EPOMAKER_MODE_ALWAYS_ON;
    current_speed       = EPOMAKER_SPEED_DEFAULT;
    current_brightness  = EPOMAKER_BRIGHTNESS_DEFAULT;
    current_dazzle      = EPOMAKER_OPTION_DAZZLE_OFF;
    current_option      = EPOMAKER_OPTION_DEFAULT;
}

EpomakerController::~EpomakerController()
{
    hid_close(dev);
}

std::string EpomakerController::GetDeviceName()
{
    return(device_name);
}

std::string EpomakerController::GetSerial()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string EpomakerController::GetLocation()
{
    return("HID: " + location);
}

void EpomakerController::SetMode(unsigned char mode, unsigned char speed, unsigned char brightness)
{
    current_mode  = mode;
    current_speed = speed;
    current_brightness = brightness;

    SendUpdate();
}

void EpomakerController::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    current_red   = red;
    current_green = green;
    current_blue  = blue;

    SendUpdate();
}

void EpomakerController::SetDazzle(bool is_dazzle)
{
    if(is_dazzle)
    {
        current_dazzle = EPOMAKER_OPTION_DAZZLE_ON;
    }
    else
    {
        current_dazzle = EPOMAKER_OPTION_DAZZLE_OFF;
    }
}

void EpomakerController::SetOption(unsigned char option)
{
    current_option = option;
}

void EpomakerController::SendUpdate()
{
    unsigned char buffer[EPOMAKER_PACKET_LENGTH + 1] = { 0x00 };

    buffer[EPOMAKER_BYTE_COMMAND]    = EPOMAKER_COMMAND_RGB;
    buffer[EPOMAKER_BYTE_MODE]       = current_mode;
    buffer[EPOMAKER_BYTE_SPEED]      = current_speed;
    buffer[EPOMAKER_BYTE_BRIGHTNESS] = current_brightness;
    buffer[EPOMAKER_BYTE_FLAGS]      = current_option | current_dazzle;
    buffer[EPOMAKER_BYTE_RED]        = current_red;
    buffer[EPOMAKER_BYTE_GREEN]      = current_green;
    buffer[EPOMAKER_BYTE_BLUE]       = current_blue;

    int sum_bits = 0;
    for(int i = EPOMAKER_BYTE_COMMAND; i <= EPOMAKER_BYTE_BLUE; i++)
    {
        sum_bits += buffer[i];
    }

    int next_pow2 = (int)(pow(2, ceil(log2((double)(sum_bits)))));
    int filler = next_pow2 - sum_bits - 1;

    buffer[EPOMAKER_BYTE_FILLER]     = filler;

    int send_buffer_result = hid_send_feature_report(dev, buffer, (sizeof(buffer) / sizeof(buffer[0])));
    if(send_buffer_result<0)
    {
        LOG_ERROR("[EPOMAKER]: Send Buffer Error. HIDAPI Error: %ls", hid_error(dev));
    }

}
