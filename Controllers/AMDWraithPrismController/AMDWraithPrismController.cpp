/*---------------------------------------------------------*\
| AMDWraithPrismController.cpp                              |
|                                                           |
|   Driver for AMD Wraith Prism                             |
|                                                           |
|   Adam Honse (CalcProgrammer1)                06 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include "AMDWraithPrismController.h"
#include "StringUtils.h"

AMDWraithPrismController::AMDWraithPrismController(hid_device* dev_handle, const char* path)
{
    dev                         = dev_handle;
    location                    = path;

    current_fan_mode            = AMD_WRAITH_PRISM_FAN_LOGO_MODE_STATIC;
    current_fan_speed           = 0xFF;
    current_fan_random_color    = false;
    current_fan_brightness      = 0xFF;

    current_logo_mode           = AMD_WRAITH_PRISM_FAN_LOGO_MODE_STATIC;
    current_logo_speed          = 0xFF;
    current_logo_random_color   = false;
    current_logo_brightness     = 0xFF;

    current_ring_mode           = AMD_WRAITH_PRISM_EFFECT_CHANNEL_STATIC;
    current_ring_speed          = 0xFF;
    current_ring_direction      = false;
    current_ring_brightness     = 0xFF;
}

AMDWraithPrismController::~AMDWraithPrismController()
{
    hid_close(dev);
}

std::string AMDWraithPrismController::GetLocationString()
{
    return("HID: " + location);
}

std::string AMDWraithPrismController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string AMDWraithPrismController::GetFirmwareVersionString()
{
    std::string ret_string = "";

    unsigned char usb_buf[] =
    {
        0x00,
        0x12, 0x20, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
    };

    unsigned char   fw_buf[16] = {0x00};

    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 64);

    for(int char_idx = 0; char_idx < 16; char_idx+=2)
    {
        if(usb_buf[char_idx + 0x08] != 0)
        {
            fw_buf[char_idx / 2] = usb_buf[char_idx + 0x08];
        }
        else
        {
            break;
        }
    }

    ret_string.append((char *)fw_buf);

    return(ret_string);
}

void AMDWraithPrismController::SetFanMode(unsigned char mode, unsigned char speed, unsigned char brightness, bool random_color)
{
    current_fan_mode            = mode;
    current_fan_speed           = speed_values_fan_logo[mode][speed];
    current_fan_brightness      = brightness;
    current_fan_random_color    = random_color;
}

void AMDWraithPrismController::SetFanColor(unsigned char red, unsigned char green, unsigned char blue)
{
    SendEffectChannelUpdate
    (
        AMD_WRAITH_PRISM_EFFECT_CHANNEL_FAN_LED,
        current_fan_speed,
        false,
        current_fan_random_color,
        current_fan_mode,
        current_fan_brightness,
        red,
        green,
        blue
    );
}

void AMDWraithPrismController::SetLogoMode(unsigned char mode, unsigned char speed, unsigned char brightness, bool random_color)
{
    current_logo_mode           = mode;
    current_logo_speed          = speed_values_fan_logo[mode][speed];
    current_logo_brightness     = brightness;
    current_logo_random_color   = random_color;
}

void AMDWraithPrismController::SetLogoColor(unsigned char red, unsigned char green, unsigned char blue)
{
    SendEffectChannelUpdate
    (
        AMD_WRAITH_PRISM_EFFECT_CHANNEL_LOGO_LED,
        current_logo_speed,
        false,
        current_logo_random_color,
        current_logo_mode,
        current_logo_brightness,
        red,
        green,
        blue
    );
}

void AMDWraithPrismController::SetRingMode(unsigned char mode, unsigned char speed, unsigned char brightness, bool direction, bool random_color)
{
    current_ring_mode           = mode;
    current_ring_speed          = speed_values_ring[mode][speed];
    current_ring_direction      = direction;
    current_ring_brightness     = brightness;
    current_ring_random_color   = random_color;
}

void AMDWraithPrismController::SetRingColor(unsigned char red, unsigned char green, unsigned char blue)
{
    SetRingEffectChannel(current_ring_mode);

    SendEffectChannelUpdate
    (
        current_ring_mode,
        current_ring_speed,
        current_ring_direction,
        current_ring_random_color,
        mode_value_ring[current_ring_mode],
        current_ring_brightness,
        red,
        green,
        blue
    );

    SendApplyCommand();
}

void AMDWraithPrismController::SetRingEffectChannel(unsigned char channel)
{
    SendChannelRemap(channel, AMD_WRAITH_PRISM_EFFECT_CHANNEL_LOGO_LED, AMD_WRAITH_PRISM_EFFECT_CHANNEL_FAN_LED);
}

void AMDWraithPrismController::SendEnableCommand(bool direct)
{
    unsigned char usb_buf[] =
    {
        0x00,
        0x41, 0x80, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
    };

    if(direct)
    {
        usb_buf[0x02]   = 0x03;
    }

    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 64);
}

void AMDWraithPrismController::SendApplyCommand()
{
    unsigned char usb_buf[] =
    {
        0x00,
        0x51, 0x28, 0x00, 0x00,
        0xE0, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
    };

    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 64);
}

void AMDWraithPrismController::SendDirectPacket
    (
    unsigned char   size,
    unsigned char * led_ids,
    RGBColor *      colors
    )
{
    unsigned char usb_buf[] =
    {
        0x00,
        0xC0, 0x01, size, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
    };

    for(unsigned int led_idx = 0; led_idx < size; led_idx++)
    {
        unsigned int index  = led_idx * 4;

        usb_buf[index + 5]  = led_ids[led_idx];
        usb_buf[index + 6]  = RGBGetRValue(colors[led_idx]);
        usb_buf[index + 7]  = RGBGetGValue(colors[led_idx]);
        usb_buf[index + 8]  = RGBGetBValue(colors[led_idx]);
    }

    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 64);
}

void AMDWraithPrismController::SendEffectChannelUpdate
    (
    unsigned char effect_channel,
    unsigned char speed,
    bool          direction,
    bool          random_color,
    unsigned char mode,
    unsigned char brightness,
    unsigned char red,
    unsigned char green,
    unsigned char blue
    )
{
    unsigned char usb_buf[] =
    {
        0x00,
        0x51, 0x2C, 0x01, 0x00,
        0x05, 0xFF, 0x00, 0x01,
        0xFF, 0xFF, 0x00, 0xFF,
        0x00, 0x00, 0x00, 0x00,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF
    };

    usb_buf[0x05]   = effect_channel;
    usb_buf[0x06]   = speed;
    usb_buf[0x07]   = (direction ? 0x01 : 0x00) | (random_color ? 0x80 : 0x00);
    usb_buf[0x08]   = mode;

    usb_buf[0x0A]   = brightness;

    usb_buf[0x0B]   = red;
    usb_buf[0x0C]   = green;
    usb_buf[0x0D]   = blue;

    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 64);
}

void AMDWraithPrismController::SendChannelRemap(unsigned char ring_channel, unsigned char logo_channel, unsigned char fan_channel)
{
    unsigned char usb_buf[] =
    {
        0x00,
        0x51, 0xA0, 0x01, 0x00,
        0x00, 0x03, 0x00, 0x00,
        0x05, 0x06, 0x07, 0x07,
        0x07, 0x07, 0x07, 0x07,
        0x07, 0x07, 0x07, 0x07,
        0x07, 0x07, 0x07, 0x07,
        0x07, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
    };

    usb_buf[0x09] = logo_channel;
    usb_buf[0x0A] = fan_channel;

    for(int led = 0x0B; led <= 0x19; led++)
    {
        usb_buf[led] = ring_channel;
    }

    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 64);
}
