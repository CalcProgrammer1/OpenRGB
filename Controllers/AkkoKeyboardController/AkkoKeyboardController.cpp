/*---------------------------------------------------------*\
| AkkoKeyboardController.cpp                                |
|                                                           |
|   Driver for Akko Multi-modes keyboards                   |
|                                                           |
|   Protocol based on the RongYuan/ROYUAN HID protocol      |
|   documented by the akko-light project                    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "AkkoKeyboardController.h"
#include "LogManager.h"
#include "StringUtils.h"

#include <algorithm>
#include <chrono>
#include <cstring>
#include <thread>

AkkoKeyboardController::AkkoKeyboardController(hid_device* dev_handle, char* path)
{
    dev                 = dev_handle;
    location            = path;
    valid               = false;

    current_mode        = AKKO_EFFECT_STATIC;
    current_speed       = AKKO_SPEED_DEFAULT;
    current_brightness  = AKKO_BRIGHTNESS_DEFAULT;
    current_option      = AKKO_OPTION_FIXED_COLOR;
    current_red         = 0x00;
    current_green       = 0x75;
    current_blue        = 0xFF;

    wchar_t name_string[AKKO_HID_MAX_STR] = { 0 };

    if(hid_get_manufacturer_string(dev, name_string, AKKO_HID_MAX_STR) == 0)
    {
        device_name = StringUtils::wstring_to_string(name_string);
    }

    std::memset(name_string, 0, sizeof(name_string));

    if(hid_get_product_string(dev, name_string, AKKO_HID_MAX_STR) == 0)
    {
        std::string product_name = StringUtils::wstring_to_string(name_string);

        if(!product_name.empty())
        {
            if(!device_name.empty())
            {
                device_name.append(" ");
            }

            device_name.append(product_name);
        }
    }

    if(device_name.empty())
    {
        device_name = "Akko Multi-modes Keyboard-B";
    }

    valid = ReadState();
}

AkkoKeyboardController::~AkkoKeyboardController()
{
    hid_close(dev);
}

bool AkkoKeyboardController::IsValid() const
{
    return(valid);
}

std::string AkkoKeyboardController::GetDeviceName()
{
    return(device_name);
}

std::string AkkoKeyboardController::GetSerial()
{
    wchar_t serial_string[AKKO_HID_MAX_STR] = { 0 };

    if(hid_get_serial_number_string(dev, serial_string, AKKO_HID_MAX_STR) != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string AkkoKeyboardController::GetLocation()
{
    return("HID: " + location);
}

unsigned char AkkoKeyboardController::GetMode() const
{
    return(current_mode);
}

unsigned char AkkoKeyboardController::GetSpeed() const
{
    return(current_speed);
}

unsigned char AkkoKeyboardController::GetBrightness() const
{
    return(current_brightness);
}

unsigned char AkkoKeyboardController::GetRed() const
{
    return(current_red);
}

unsigned char AkkoKeyboardController::GetGreen() const
{
    return(current_green);
}

unsigned char AkkoKeyboardController::GetBlue() const
{
    return(current_blue);
}

unsigned char AkkoKeyboardController::CalculateChecksum(const unsigned char* data, std::size_t length)
{
    unsigned int sum = 0;

    for(std::size_t index = 0; index < length; index++)
    {
        sum += data[index];
    }

    return(static_cast<unsigned char>(0xFF - (sum & 0xFF)));
}

bool AkkoKeyboardController::ReadState()
{
    unsigned char request[AKKO_REPORT_LENGTH + 1] = { 0x00 };

    request[1] = AKKO_COMMAND_GET_LED_PARAMS;
    request[8] = CalculateChecksum(&request[1], 7);

    int result = hid_send_feature_report(dev, request, sizeof(request));

    if(result < 0)
    {
        LOG_DEBUG("[AKKO]: GET_LEDPARAM request failed: %ls", hid_error(dev));
        return(false);
    }

    for(unsigned int attempt = 0; attempt < 5; attempt++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(80));

        unsigned char response[AKKO_REPORT_LENGTH + 1] = { 0x00 };
        result = hid_get_feature_report(dev, response, sizeof(response));

        if(result > 8 && response[1] == AKKO_COMMAND_GET_LED_PARAMS)
        {
            current_mode        = response[2];
            current_speed       = response[3] <= AKKO_SPEED_MAX ? AKKO_SPEED_MAX - response[3] : AKKO_SPEED_DEFAULT;
            current_brightness  = std::min<unsigned char>(response[4], AKKO_BRIGHTNESS_MAX);
            current_option      = response[5];
            current_red         = response[6];
            current_green       = response[7];
            current_blue        = response[8];
            return(true);
        }
    }

    LOG_DEBUG("[AKKO]: Device did not answer GET_LEDPARAM");
    return(false);
}

bool AkkoKeyboardController::SendUpdate()
{
    unsigned char buffer[AKKO_REPORT_LENGTH + 1] = { 0x00 };

    buffer[1] = AKKO_COMMAND_SET_LED_PARAMS;
    buffer[2] = current_mode;
    buffer[3] = AKKO_SPEED_MAX - std::min<unsigned char>(current_speed, AKKO_SPEED_MAX);
    buffer[4] = std::min<unsigned char>(current_brightness, AKKO_BRIGHTNESS_MAX);
    buffer[5] = current_option;
    buffer[6] = current_red;
    buffer[7] = current_green;
    buffer[8] = current_blue;
    buffer[9] = CalculateChecksum(&buffer[1], 8);

    int result = hid_send_feature_report(dev, buffer, sizeof(buffer));

    if(result < 0)
    {
        LOG_ERROR("[AKKO]: SET_LEDPARAM failed: %ls", hid_error(dev));
        return(false);
    }

    return(true);
}

void AkkoKeyboardController::SetMode(unsigned char mode, unsigned char speed, unsigned char brightness)
{
    current_mode        = mode;
    current_speed       = std::min<unsigned char>(speed, AKKO_SPEED_MAX);
    current_brightness  = std::min<unsigned char>(brightness, AKKO_BRIGHTNESS_MAX);
    current_option      = AKKO_OPTION_FIXED_COLOR;

    SendUpdate();
}

void AkkoKeyboardController::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    current_red     = red;
    current_green   = green;
    current_blue    = blue;
    current_option  = AKKO_OPTION_FIXED_COLOR;

    SendUpdate();
}
