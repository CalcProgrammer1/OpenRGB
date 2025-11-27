/*---------------------------------------------------------*\
| FnaticStreakController.cpp                                |
|                                                           |
|   Driver for Fnatic Streak and miniStreak keyboards       |
|                                                           |
|   Based on leddy project by Hanna Czenczek                |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include "FnaticStreakController.h"
#include "StringUtils.h"
#include "LogManager.h"

FnaticStreakController::FnaticStreakController(hid_device* dev_handle, hid_device_info* dev_info, std::string dev_name, FnaticStreakType kb_type)
{
    dev             = dev_handle;
    location        = dev_info->path;
    name            = dev_name;
    keyboard_type   = kb_type;
    profile         = 1;
    software_effect_mode = false;

    memset(color_buf, 0x00, sizeof(color_buf));

    /*-----------------------------------------------------*\
    | Get the firmware version from the device info         |
    \*-----------------------------------------------------*/
    char fw_version_buf[8];
    memset(fw_version_buf, '\0', sizeof(fw_version_buf));

    unsigned short version = dev_info->release_number;
    snprintf(fw_version_buf, 8, "%.2X.%.2X", (version & 0xFF00) >> 8, version & 0x00FF);

    firmware_version = fw_version_buf;
}

FnaticStreakController::~FnaticStreakController()
{
    hid_close(dev);
}

std::string FnaticStreakController::GetDeviceLocation()
{
    return("HID " + location);
}

std::string FnaticStreakController::GetNameString()
{
    return(name);
}

std::string FnaticStreakController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string FnaticStreakController::GetFirmwareVersion()
{
    return(firmware_version);
}

FnaticStreakType FnaticStreakController::GetKeyboardType()
{
    return(keyboard_type);
}

unsigned int FnaticStreakController::GetLEDCount()
{
    if(keyboard_type == FNATIC_STREAK_TYPE_MINI)
    {
        return 106;
    }
    else
    {
        return 124;
    }
}

void FnaticStreakController::SetProfile(unsigned char new_profile)
{
    if(new_profile >= 1 && new_profile <= 4)
    {
        profile = new_profile;
        SoftwareEffectEnd();
    }
}

void FnaticStreakController::SoftwareEffectStart()
{
    software_effect_mode = true;
}

void FnaticStreakController::SoftwareEffectEnd()
{
    software_effect_mode = false;
    RefreshProfile();
}

void FnaticStreakController::SendKeepalive()
{
    /*-----------------------------------------------------*\
    | Send keepalive packet (0x07 or 0xfe) to prevent       |
    | keyboard from reverting to profile effect during      |
    | direct/preview mode                                   |
    \*-----------------------------------------------------*/
    unsigned char prefix[] = { 0x07 };
    SendRequest(prefix, sizeof(prefix), nullptr, 0);
}

void FnaticStreakController::RefreshProfile()
{
    unsigned char data[] = { profile };
    unsigned char prefix[] = { 0x04 };
    SendRequest(prefix, sizeof(prefix), data, sizeof(data));
}

void FnaticStreakController::SendRequest(const unsigned char* prefix, size_t prefix_len, const unsigned char* raw_data, size_t data_len)
{
    size_t total_len = prefix_len + data_len;
    size_t offset = 0;
    unsigned char cmd = (prefix_len > 0) ? prefix[0] : raw_data[0];

    while(offset < total_len)
    {
        unsigned char packet[65];
        memset(packet, 0x00, sizeof(packet));

        /*-----------------------------------------------------*\
        | Packet format:                                        |
        | [0]     = Report ID (0x00)                            |
        | [1]     = Command                                     |
        | [2-4]   = Total length (24-bit little endian)         |
        | [5-7]   = Offset (24-bit little endian)               |
        | [8-64]  = Data (57 bytes max per packet)              |
        \*-----------------------------------------------------*/
        packet[0] = 0x00;
        packet[1] = cmd;
        packet[2] = (unsigned char)(total_len & 0xFF);
        packet[3] = (unsigned char)((total_len >> 8) & 0xFF);
        packet[4] = (unsigned char)((total_len >> 16) & 0xFF);
        packet[5] = (unsigned char)(offset & 0xFF);
        packet[6] = (unsigned char)((offset >> 8) & 0xFF);
        packet[7] = (unsigned char)((offset >> 16) & 0xFF);

        for(size_t i = offset; i < offset + 57 && i < total_len; i++)
        {
            if(i < prefix_len)
            {
                packet[i - offset + 8] = prefix[i];
            }
            else
            {
                packet[i - offset + 8] = raw_data[i - prefix_len];
            }
        }

        hid_write(dev, packet, 65);
        offset += 57;
    }

    /*-----------------------------------------------------*\
    | For command 0x05, save changes and refresh profile    |
    \*-----------------------------------------------------*/
    if(cmd == 0x05)
    {
        unsigned char save_prefix[] = { 0x13 };
        SendRequest(save_prefix, sizeof(save_prefix), nullptr, 0);

        unsigned char profile_data[] = { profile };
        unsigned char profile_prefix[] = { 0x04 };
        SendRequest(profile_prefix, sizeof(profile_prefix), profile_data, sizeof(profile_data));
    }
}

void FnaticStreakController::SetLEDsDirect(std::vector<led> leds, std::vector<RGBColor> colors, unsigned int brightness)
{
    unsigned int total_leds = GetLEDCount();

    /*-----------------------------------------------------*\
    | Clear the color buffer                                |
    \*-----------------------------------------------------*/
    memset(color_buf, 0x00, sizeof(color_buf));

    /*-----------------------------------------------------*\
    | Transfer colors to the buffer                         |
    | Format: sequential RGB triplets indexed by LED value  |
    | The LED value corresponds to the physical LED index   |
    | in the keyboard hardware (0-123 for full, 0-105 mini) |
    | Apply brightness scaling (0-100%)                     |
    \*-----------------------------------------------------*/
    unsigned int leds_to_set = (unsigned int)std::min(colors.size(), leds.size());

    for(unsigned int i = 0; i < leds_to_set; i++)
    {
        unsigned int led_idx = leds[i].value;
        if(led_idx < total_leds)
        {
            if(brightness >= 100)
            {
                /*-----------------------------------------*\
                | Full brightness - no scaling needed       |
                \*-----------------------------------------*/
                color_buf[led_idx * 3 + 0] = RGBGetRValue(colors[i]);
                color_buf[led_idx * 3 + 1] = RGBGetGValue(colors[i]);
                color_buf[led_idx * 3 + 2] = RGBGetBValue(colors[i]);
            }
            else
            {
                /*-----------------------------------------*\
                | Apply brightness scaling                  |
                \*-----------------------------------------*/
                color_buf[led_idx * 3 + 0] = (unsigned char)(RGBGetRValue(colors[i]) * brightness / 100);
                color_buf[led_idx * 3 + 1] = (unsigned char)(RGBGetGValue(colors[i]) * brightness / 100);
                color_buf[led_idx * 3 + 2] = (unsigned char)(RGBGetBValue(colors[i]) * brightness / 100);
            }
        }
    }
}

void FnaticStreakController::SendRGBToDevice()
{
    unsigned int total_leds = GetLEDCount();
    unsigned int data_size = total_leds * 3;

    /*-----------------------------------------------------*\
    | For direct/software control, use command 0x0f         |
    | This bypasses the profile and allows immediate update |
    | The 0x03 subcommand indicates per-key color data      |
    \*-----------------------------------------------------*/
    unsigned char prefix[] = { 0x0f, 0x03 };
    SendRequest(prefix, sizeof(prefix), color_buf, data_size);
}

void FnaticStreakController::SetPulse(unsigned char color_mode, unsigned char r, unsigned char g, unsigned char b, unsigned char speed)
{
    /*-----------------------------------------------------*\
    | Pulse effect - cmd 0x06                               |
    | Data: [mode, r, g, b, speed]                          |
    \*-----------------------------------------------------*/
    unsigned char prefix[] = { 0x05, profile, 0x02 };
    unsigned char data[] = { FNATIC_STREAK_CMD_PULSE, color_mode, r, g, b, speed };
    SendRequest(prefix, sizeof(prefix), data, sizeof(data));
}

void FnaticStreakController::SetWave(unsigned char color_mode, unsigned char r, unsigned char g, unsigned char b, unsigned char speed, unsigned char direction)
{
    /*-----------------------------------------------------*\
    | Wave effect - cmd 0x07                                |
    | Data: [mode, r, g, b, speed, direction]               |
    \*-----------------------------------------------------*/
    unsigned char prefix[] = { 0x05, profile, 0x02 };
    unsigned char data[] = { FNATIC_STREAK_CMD_WAVE, color_mode, r, g, b, speed, direction };
    SendRequest(prefix, sizeof(prefix), data, sizeof(data));
}

void FnaticStreakController::SetReactive(unsigned char color_mode, unsigned char r, unsigned char g, unsigned char b, unsigned char speed, bool keyup)
{
    /*-----------------------------------------------------*\
    | Reactive effect - cmd 0x09                            |
    | Data: [mode, r, g, b, speed, trigger]                 |
    | trigger: 0 = keydown, 1 = keyup                       |
    \*-----------------------------------------------------*/
    unsigned char prefix[] = { 0x05, profile, 0x02 };
    unsigned char data[] = { FNATIC_STREAK_CMD_REACTIVE, color_mode, r, g, b, speed, (unsigned char)(keyup ? 0 : 1) };
    SendRequest(prefix, sizeof(prefix), data, sizeof(data));
}

void FnaticStreakController::SetReactiveRipple(unsigned char color_mode, unsigned char r, unsigned char g, unsigned char b, unsigned char speed, bool keyup)
{
    /*-----------------------------------------------------*\
    | Reactive Ripple effect - cmd 0x0A                     |
    | Data: [mode, r, g, b, speed, trigger]                 |
    | trigger: 0 = keydown, 1 = keyup                       |
    \*-----------------------------------------------------*/
    unsigned char prefix[] = { 0x05, profile, 0x02 };
    unsigned char data[] = { FNATIC_STREAK_CMD_REACTIVE_RIPPLE, color_mode, r, g, b, speed, (unsigned char)(keyup ? 0 : 1) };
    SendRequest(prefix, sizeof(prefix), data, sizeof(data));
}

void FnaticStreakController::SetRain(unsigned char color_mode, unsigned char r, unsigned char g, unsigned char b, unsigned char speed, unsigned char direction)
{
    /*-----------------------------------------------------*\
    | Rain effect - cmd 0x0B                                |
    | Data: [mode, r, g, b, speed, direction]               |
    | Note: Does not support rainbow mode                   |
    \*-----------------------------------------------------*/
    unsigned char prefix[] = { 0x05, profile, 0x02 };
    unsigned char mode = (color_mode == FNATIC_STREAK_COLOR_MODE_RAINBOW) ? FNATIC_STREAK_COLOR_MODE_RANDOM : color_mode;
    unsigned char data[] = { FNATIC_STREAK_CMD_RAIN, mode, r, g, b, speed, direction };
    SendRequest(prefix, sizeof(prefix), data, sizeof(data));
}

void FnaticStreakController::SetGradient(unsigned char colors[][3], unsigned char positions[], unsigned int count)
{
    /*-----------------------------------------------------*\
    | Gradient effect - cmd 0x0C                            |
    | Data: [count, {r, g, b, pos} * 10]                    |
    | (always sends 10 color slots, unused are zeroed)      |
    \*-----------------------------------------------------*/
    unsigned char prefix[] = { 0x05, profile, 0x02 };
    unsigned char data[42];
    memset(data, 0x00, sizeof(data));

    data[0] = FNATIC_STREAK_CMD_GRADIENT;
    data[1] = (unsigned char)count;

    for(unsigned int i = 0; i < count && i < 10; i++)
    {
        data[2 + i * 4 + 0] = colors[i][0];
        data[2 + i * 4 + 1] = colors[i][1];
        data[2 + i * 4 + 2] = colors[i][2];
        data[2 + i * 4 + 3] = positions[i];
    }

    SendRequest(prefix, sizeof(prefix), data, sizeof(data));
}

void FnaticStreakController::SetFade(unsigned char color_mode, unsigned char colors[][3], unsigned char positions[], unsigned int count, unsigned char speed)
{
    /*-----------------------------------------------------*\
    | Fade effect - cmd 0x0D                                |
    | Data: [mode, count, {r, g, b, pos} * 10, speed]       |
    \*-----------------------------------------------------*/
    unsigned char prefix[] = { 0x05, profile, 0x02 };
    unsigned char data[44];
    memset(data, 0x00, sizeof(data));

    data[0] = FNATIC_STREAK_CMD_FADE;
    data[1] = color_mode;
    data[2] = (unsigned char)count;

    for(unsigned int i = 0; i < count && i < 10; i++)
    {
        data[3 + i * 4 + 0] = colors[i][0];
        data[3 + i * 4 + 1] = colors[i][1];
        data[3 + i * 4 + 2] = colors[i][2];
        data[3 + i * 4 + 3] = positions[i];
    }

    data[43] = speed;

    SendRequest(prefix, sizeof(prefix), data, sizeof(data));
}
