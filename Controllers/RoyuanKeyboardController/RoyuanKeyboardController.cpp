/*---------------------------------------------------------*\
| RoyuanKeyboardController.cpp                              |
|                                                           |
|   Driver for ROYUAN-based keyboard lighting controllers   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RoyuanKeyboardController.h"
#include "LogManager.h"
#include "StringUtils.h"

#include <algorithm>
#include <chrono>
#include <cstring>
#include <thread>

RoyuanKeyboardProfile RoyuanKeyboardProfile::EpomakerLegacy()
{
    RoyuanKeyboardProfile profile;

    profile.protocol             = RoyuanKeyboardProtocol::Legacy;
    profile.checksum_type        = RoyuanChecksumType::NextPowerOfTwo;
    profile.speed_max            = 0x05;
    profile.speed_default        = 0x04;
    profile.brightness_max       = 0x04;
    profile.brightness_default   = 0x04;
    profile.option_default       = 0x00;
    profile.dazzle_default       = 0x07;
    profile.reverse_speed        = false;
    profile.supports_readback    = false;
    profile.fallback_name        = "ROYUAN Keyboard";

    return(profile);
}

RoyuanKeyboardProfile RoyuanKeyboardProfile::AkkoBSeries()
{
    RoyuanKeyboardProfile profile;

    profile.protocol             = RoyuanKeyboardProtocol::AkkoBSeries;
    profile.checksum_type        = RoyuanChecksumType::OnesComplement;
    profile.speed_max            = ROYUAN_AKKO_SPEED_MAX;
    profile.speed_default        = ROYUAN_AKKO_SPEED_DEFAULT;
    profile.brightness_max       = ROYUAN_AKKO_BRIGHTNESS_MAX;
    profile.brightness_default   = ROYUAN_AKKO_BRIGHTNESS_DEFAULT;
    profile.option_default       = 0x08;
    profile.dazzle_default       = 0x00;
    profile.reverse_speed        = true;
    profile.supports_readback    = true;
    profile.fallback_name        = "Akko 3068B Plus";

    return(profile);
}

RoyuanKeyboardController::RoyuanKeyboardController(hid_device* dev_handle, char* path, const RoyuanKeyboardProfile& profile)
{
    dev                 = dev_handle;
    location            = path;
    this->profile       = profile;
    valid               = false;

    current_mode        = 0x01;
    current_speed       = profile.speed_default;
    current_brightness  = profile.brightness_default;
    current_dazzle      = profile.dazzle_default;
    current_option      = profile.option_default;
    current_red         = 0x00;
    current_green       = 0x00;
    current_blue        = 0x00;

    wchar_t name_string[ROYUAN_HID_MAX_STR] = { 0 };

    if(hid_get_manufacturer_string(dev, name_string, ROYUAN_HID_MAX_STR) == 0)
    {
        device_name = StringUtils::wstring_to_string(name_string);
    }

    std::memset(name_string, 0, sizeof(name_string));

    if(hid_get_product_string(dev, name_string, ROYUAN_HID_MAX_STR) == 0)
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
        device_name = profile.fallback_name;
    }

    valid = !profile.supports_readback || ReadState();
}

RoyuanKeyboardController::~RoyuanKeyboardController()
{
    hid_close(dev);
}

bool RoyuanKeyboardController::IsValid() const
{
    return(valid);
}

std::string RoyuanKeyboardController::GetDeviceName()
{
    return(device_name);
}

std::string RoyuanKeyboardController::GetSerial()
{
    wchar_t serial_string[ROYUAN_HID_MAX_STR] = { 0 };

    if(hid_get_serial_number_string(dev, serial_string, ROYUAN_HID_MAX_STR) != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string RoyuanKeyboardController::GetLocation()
{
    return("HID: " + location);
}

RoyuanKeyboardProtocol RoyuanKeyboardController::GetProtocol() const
{
    return(profile.protocol);
}

unsigned char RoyuanKeyboardController::GetMode() const
{
    return(current_mode);
}

unsigned char RoyuanKeyboardController::GetSpeed() const
{
    return(current_speed);
}

unsigned char RoyuanKeyboardController::GetBrightness() const
{
    return(current_brightness);
}

unsigned char RoyuanKeyboardController::GetRed() const
{
    return(current_red);
}

unsigned char RoyuanKeyboardController::GetGreen() const
{
    return(current_green);
}

unsigned char RoyuanKeyboardController::GetBlue() const
{
    return(current_blue);
}

unsigned char RoyuanKeyboardController::CalculateChecksum(const unsigned char* data, std::size_t length) const
{
    unsigned int sum = 0;

    for(std::size_t index = 0; index < length; index++)
    {
        sum += data[index];
    }

    if(profile.checksum_type == RoyuanChecksumType::OnesComplement)
    {
        return(static_cast<unsigned char>(0xFF - (sum & 0xFF)));
    }

    unsigned int next_power_of_two = 1;

    while(next_power_of_two < sum)
    {
        next_power_of_two <<= 1;
    }

    return(static_cast<unsigned char>((next_power_of_two - sum - 1) & 0xFF));
}

bool RoyuanKeyboardController::ReadState()
{
    unsigned char request[ROYUAN_REPORT_LENGTH + 1] = { 0x00 };

    request[1] = ROYUAN_COMMAND_GET_LED_PARAMS;
    request[8] = CalculateChecksum(&request[1], 7);

    int result = hid_send_feature_report(dev, request, sizeof(request));

    if(result < 0)
    {
        LOG_DEBUG("[ROYUAN]: GET_LEDPARAM request failed: %ls", hid_error(dev));
        return(false);
    }

    for(unsigned int attempt = 0; attempt < 5; attempt++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(80));

        unsigned char response[ROYUAN_REPORT_LENGTH + 1] = { 0x00 };
        result = hid_get_feature_report(dev, response, sizeof(response));

        if(result > 8 && response[1] == ROYUAN_COMMAND_GET_LED_PARAMS)
        {
            current_mode        = response[2];
            current_speed       = response[3] <= profile.speed_max ? profile.speed_max - response[3] : profile.speed_default;
            current_brightness  = std::min<unsigned char>(response[4], profile.brightness_max);
            current_option      = response[5];
            current_red         = response[6];
            current_green       = response[7];
            current_blue        = response[8];
            return(true);
        }
    }

    LOG_DEBUG("[ROYUAN]: Device did not answer GET_LEDPARAM");
    return(false);
}

bool RoyuanKeyboardController::SendUpdate()
{
    unsigned char buffer[ROYUAN_REPORT_LENGTH + 1] = { 0x00 };

    buffer[1] = ROYUAN_COMMAND_SET_LED_PARAMS;
    buffer[2] = current_mode;

    if(profile.reverse_speed)
    {
        buffer[3] = profile.speed_max - std::min<unsigned char>(current_speed, profile.speed_max);
    }
    else
    {
        buffer[3] = std::min<unsigned char>(current_speed, profile.speed_max);
    }

    buffer[4] = std::min<unsigned char>(current_brightness, profile.brightness_max);
    buffer[5] = current_option | current_dazzle;
    buffer[6] = current_red;
    buffer[7] = current_green;
    buffer[8] = current_blue;
    buffer[9] = CalculateChecksum(&buffer[1], 8);

    int result = hid_send_feature_report(dev, buffer, sizeof(buffer));

    if(result < 0)
    {
        LOG_ERROR("[ROYUAN]: SET_LEDPARAM failed: %ls", hid_error(dev));
        return(false);
    }

    return(true);
}

void RoyuanKeyboardController::SetDazzle(bool is_dazzle)
{
    if(profile.protocol == RoyuanKeyboardProtocol::Legacy)
    {
        current_dazzle = is_dazzle ? 0x08 : 0x07;
    }
}

void RoyuanKeyboardController::SetOption(unsigned char option)
{
    current_option = option;
}

void RoyuanKeyboardController::SetMode(unsigned char mode, unsigned char speed, unsigned char brightness)
{
    current_mode        = mode;
    current_speed       = std::min<unsigned char>(speed, profile.speed_max);
    current_brightness  = std::min<unsigned char>(brightness, profile.brightness_max);

    if(profile.protocol == RoyuanKeyboardProtocol::AkkoBSeries)
    {
        current_option = profile.option_default;
    }

    SendUpdate();
}

void RoyuanKeyboardController::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    current_red     = red;
    current_green   = green;
    current_blue    = blue;

    if(profile.protocol == RoyuanKeyboardProtocol::AkkoBSeries)
    {
        current_option = profile.option_default;
    }

    SendUpdate();
}
