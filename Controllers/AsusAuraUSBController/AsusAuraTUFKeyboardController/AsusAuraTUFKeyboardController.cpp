/*---------------------------------------------------------*\
| AsusAuraTUFKeyboardController.cpp                         |
|                                                           |
|   Driver for ASUS Aura TUF keyboard                       |
|                                                           |
|   Mola19                                      03 Mar 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cmath>
#include <cstring>
#include <math.h>
#include <string>
#include <stdio.h>
#include <string.h>
#include <vector>
#include "AsusAuraTUFKeyboardController.h"
#include "StringUtils.h"

#define HID_MAX_STR 128

AuraTUFKeyboardController::AuraTUFKeyboardController(hid_device* dev_handle, const char* path, uint16_t pid, unsigned short version, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
    device_pid  = pid;
    rev_version = version;

    is_per_led_keyboard = (pid != AURA_TUF_K1_GAMING_PID && pid != AURA_TUF_K5_GAMING_PID);
}

AuraTUFKeyboardController::~AuraTUFKeyboardController()
{
    hid_close(dev);
}

std::string AuraTUFKeyboardController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string AuraTUFKeyboardController::GetName()
{
    return(name);
}

std::string AuraTUFKeyboardController::GetSerialString()
{
    wchar_t serial_string[HID_MAX_STR];
    memset(serial_string, 0, sizeof(serial_string));

    int ret = hid_get_serial_number_string(dev, serial_string, HID_MAX_STR);
    if(ret != 0)
    {
        return("");
    }

    /*-------------------------------------------------------------------------*\
    | Skip non-ASCII, trailing garbage in serial numbers. Required by the       |
    | Scope II 96, whose original firmware outputs garbage, which even differs  |
    | after computer reboots and therefore breaks OpenRGB profile matching.     |
    \*-------------------------------------------------------------------------*/
    switch(device_pid)
    {
        case AURA_ROG_STRIX_SCOPE_II_96_WIRELESS_USB_PID:
            serial_string[12] = L'\0';
            break;
        default:
            break;
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string AuraTUFKeyboardController::GetVersion()
{
    if(device_pid != AURA_ROG_CLAYMORE_PID)
    {
        unsigned char usb_buf[65];
        memset(usb_buf, 0x00, sizeof(usb_buf));

        usb_buf[0x00]   = 0x00;
        usb_buf[0x01]   = 0x12;
        usb_buf[0x02]   = 0x00;

        ClearResponses();

        hid_write(dev, usb_buf, 65);

        unsigned char usb_buf_out[65];
        hid_read(dev, usb_buf_out, 65);

        char version[9];

        switch(device_pid)
        {
            case AURA_ROG_AZOTH_USB_PID:
            case AURA_ROG_AZOTH_2_4_PID:
            case AURA_TUF_K3_GAMING_PID:
            case AURA_ROG_STRIX_FLARE_II_ANIMATE_PID:
            case AURA_ROG_STRIX_SCOPE_NX_WIRELESS_DELUXE_USB_PID:
            case AURA_ROG_STRIX_SCOPE_II_PID:
            case AURA_ROG_STRIX_SCOPE_II_RX_PID:
            case AURA_ROG_STRIX_SCOPE_II_96_WIRELESS_USB_PID:
                snprintf(version, 9, "%02X.%02X.%02X", usb_buf_out[6], usb_buf_out[5], usb_buf_out[4]);
                break;
            case AURA_ROG_STRIX_SCOPE_NX_WIRELESS_DELUXE_2_4_PID:
                snprintf(version, 9, "%02X.%02X.%02X", usb_buf_out[13], usb_buf_out[12], usb_buf_out[11]);
                break;
            default:
                snprintf(version, 9, "%02X.%02X.%02X", usb_buf_out[5], usb_buf_out[6], usb_buf_out[7]);
        }

        return std::string(version);
    }
    else
    {
        unsigned char usb_buf[65];
        memset(usb_buf, 0x00, sizeof(usb_buf));
        usb_buf[0x00]   = 0x00;
        usb_buf[0x01]   = 0x10;
        usb_buf[0x02]   = 0x01;
        usb_buf[0x03]   = 0x00;
        usb_buf[0x04]   = 0x00;
        usb_buf[0x05]   = 0x00; // set to 1 to get firmware version of numpad

        ClearResponses();
        hid_write(dev, usb_buf, 65);
        AwaitResponse(20);

        unsigned char usb_buf1[65];
        memset(usb_buf1, 0x00, sizeof(usb_buf1));
        usb_buf1[0x00]   = 0x00;
        usb_buf1[0x01]   = 0x12;
        usb_buf1[0x02]   = 0x22;

        ClearResponses();
        hid_write(dev, usb_buf1, 65);

        unsigned char usb_buf_out[65];
        hid_read(dev, usb_buf_out, 65);

        char version[9];
        snprintf(version, 9, "%02X.%02X.%02X", usb_buf_out[8], usb_buf_out[9], usb_buf_out[10]);

        return std::string(version);
    }
}

int AuraTUFKeyboardController::GetLayout()
{
    if(device_pid != AURA_ROG_CLAYMORE_PID)
    {
        unsigned char usb_buf[65];
        memset(usb_buf, 0x00, sizeof(usb_buf));

        usb_buf[0x00]   = 0x00;
        usb_buf[0x01]   = 0x12;
        usb_buf[0x02]   = 0x12;

        ClearResponses();
        hid_write(dev, usb_buf, 65);

        unsigned char usb_buf_out[65];
        hid_read(dev, usb_buf_out, 65);

        return(usb_buf_out[4] * 100 + usb_buf_out[5]);
    }
    else
    {
        switch(rev_version >> 0b1100)
        {
            case 1:
                return 117;
            case 2:
                return 204;
            case 3:
                return 221;
            case 4:
                return 117;
            default:
                return 117;
        }
    }
}

/*---------------------------------------------------------*\
| only needed for Claymore                                  |
\*---------------------------------------------------------*/
int AuraTUFKeyboardController::GetNumpadLocation()
{
    unsigned char usb_buf[65];
    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x40;
    usb_buf[0x02]   = 0x60;

    ClearResponses();
    hid_write(dev, usb_buf, 65);

    unsigned char usb_buf_out[65];
    hid_read(dev, usb_buf_out, 65);

    return(usb_buf_out[5] * 2 + usb_buf_out[4]);
}

void AuraTUFKeyboardController::SaveMode()
{
    unsigned char usb_save_buf[65];
    memset(usb_save_buf, 0x00, sizeof(usb_save_buf));

    usb_save_buf[0x00]   = 0x00;
    usb_save_buf[0x01]   = 0x50;
    usb_save_buf[0x02]   = 0x55;

    ClearResponses();
    hid_write(dev, usb_save_buf, 65);
    AwaitResponse(60);
}

/*---------------------------------------------------------*\
| only needed for Claymore                                  |
\*---------------------------------------------------------*/
void AuraTUFKeyboardController::AllowRemoteControl(unsigned char type)
{
    unsigned char usb_buf[65];
    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x41;
    usb_buf[0x02]   = type;

    ClearResponses();
    hid_write(dev, usb_buf, 65);
    AwaitResponse(20);
}

void AuraTUFKeyboardController::UpdateSingleLed
    (
    int             led,
    unsigned char   red,
    unsigned char   green,
    unsigned char   blue
    )
{
    /*-----------------------------------------------------*\
    | Set up message packet for single LED                  |
    \*-----------------------------------------------------*/
    unsigned char usb_buf[65];
    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0] = 0x00;
    usb_buf[1] = 0xC0;
    usb_buf[2] = 0x81;
    usb_buf[3] = 0x01;
    usb_buf[4] = 0x00;

    /*-----------------------------------------------------*\
    | Convert LED index                                     |
    \*-----------------------------------------------------*/
    usb_buf[5] = led;
    usb_buf[6] = red;
    usb_buf[7] = green;
    usb_buf[8] = blue;

    ClearResponses();
    hid_write(dev, usb_buf, 65);
    AwaitResponse(20);
}

void AuraTUFKeyboardController::UpdateLeds
    (
    std::vector<led_color>    colors
    )
{
    int packets = (int)ceil((float)colors.size() / 15.0f);

    for(int i = 0; i < packets; i++)
    {
        unsigned char usb_buf[65];
        memset(usb_buf, 0x00, sizeof(usb_buf));

        int remaining = (int)colors.size() - i * 15;

        int leds = (remaining > 0x0F) ? 0x0F : remaining;

        usb_buf[0] = 0x00;
        usb_buf[1] = 0xC0;
        usb_buf[2] = 0x81;
        usb_buf[3] = leds;
        usb_buf[4] = 0x00;

        for(int j = 0; j < leds; j++)
        {
            usb_buf[j * 4 + 5] = (is_per_led_keyboard) ? colors[i * 15 + j].value : 0x00;
            usb_buf[j * 4 + 6] = RGBGetRValue(colors[i * 15 + j].color);
            usb_buf[j * 4 + 7] = RGBGetGValue(colors[i * 15 + j].color);
            usb_buf[j * 4 + 8] = RGBGetBValue(colors[i * 15 + j].color);
        }

        ClearResponses();
        hid_write(dev, usb_buf, 65);
        AwaitResponse(20);
    }
}

void AuraTUFKeyboardController::UpdateK1Wave
    (
    std::vector<RGBColor>   colors,
    unsigned char           direction,
    unsigned char           speed,
    unsigned char           brightness
    )
{
    unsigned char usb_buf[65];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x51;
    usb_buf[0x02]   = 0x2C;
    usb_buf[0x03]   = 0x03;
    usb_buf[0x04]   = 0x00;
    usb_buf[0x05]   = speed;
    usb_buf[0x06]   = brightness;
    usb_buf[0x07]   = 0x00;
    usb_buf[0x08]   = direction;
    usb_buf[0x09]   = 0x00;

    usb_buf[10] = 5;
    usb_buf[11] = RGBGetRValue(colors[4]);
    usb_buf[12] = RGBGetGValue(colors[4]);
    usb_buf[13] = RGBGetBValue(colors[4]);

    ClearResponses();
    hid_write(dev, usb_buf, 65);
    AwaitResponse(20);

    for(unsigned int i = 0; i < 4; i ++)
    {
        usb_buf[10 + i * 4] = i + 1;
        usb_buf[11 + i * 4] = RGBGetRValue(colors[i]);
        usb_buf[12 + i * 4] = RGBGetGValue(colors[i]);
        usb_buf[13 + i * 4] = RGBGetBValue(colors[i]);
    }

    ClearResponses();
    hid_write(dev, usb_buf, 65);
    AwaitResponse(20);
}

void AuraTUFKeyboardController::UpdateScopeIIRainbowRipple
    (
    unsigned char           mode,
    std::vector<RGBColor>   colors,
    unsigned char           direction,
    unsigned char           color_mode,
    unsigned char           speed,
    unsigned char           brightness
    )
{
    unsigned char usb_buf[65];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x51;
    usb_buf[0x02]   = 0x2C;
    usb_buf[0x03]   = mode;
    usb_buf[0x04]   = 0x02;
    usb_buf[0x05]   = speed;
    usb_buf[0x06]   = brightness;
    usb_buf[0x07]   = color_mode;
    usb_buf[0x08]   = direction;
    usb_buf[0x09]   = 0x02;
    usb_buf[0x0A]   = (unsigned char)colors.size();

    for(unsigned int i = 0; i < 2; i++)
    {
        if(i >= colors.size())
        {
            continue;
        }

        usb_buf[11 + i * 4] = (unsigned char)(100.0f / (float)colors.size() * (i + 1));
        usb_buf[12 + i * 4] = RGBGetRValue(colors[i]);
        usb_buf[13 + i * 4] = RGBGetGValue(colors[i]);
        usb_buf[14 + i * 4] = RGBGetBValue(colors[i]);
    }

    ClearResponses();
    hid_write(dev, usb_buf, 65);
    AwaitResponse(20);

    memset(usb_buf + 4, 0x00, sizeof(usb_buf) - 4);

    usb_buf[0x04] = 0x01;

    for(unsigned int i = 0; i < 4; i++)
    {
        if((i + 2) >= colors.size())
        {
            continue;
        }
        usb_buf[5 + i * 4] = (unsigned char)(100.0f / (float)colors.size() * (i + 1 + 2));
        usb_buf[6 + i * 4] = RGBGetRValue(colors[i + 2]);
        usb_buf[7 + i * 4] = RGBGetGValue(colors[i + 2]);
        usb_buf[8 + i * 4] = RGBGetBValue(colors[i + 2]);
    }

    ClearResponses();
    hid_write(dev, usb_buf, 65);
    AwaitResponse(20);

    memset(usb_buf + 4, 0x00, sizeof(usb_buf) - 4);

    usb_buf[0x04] = 0x00;

    for(unsigned int i = 0; i < 1; i ++)
    {
        if((i + 6) >= colors.size())
        {
            continue;
        }

        usb_buf[5 + i * 4] = (unsigned char)(100.0f / (float)colors.size() * (i + 1 + 6));
        usb_buf[6 + i * 4] = RGBGetRValue(colors[i + 6]);
        usb_buf[7 + i * 4] = RGBGetGValue(colors[i + 6]);
        usb_buf[8 + i * 4] = RGBGetBValue(colors[i + 6]);
    }

    ClearResponses();
    hid_write(dev, usb_buf, 65);
    AwaitResponse(20);
}

void AuraTUFKeyboardController::UpdateScopeIIQuicksand
    (
    std::vector<RGBColor>   colors,
    unsigned char           direction,
    unsigned char           color_mode,
    unsigned char           speed,
    unsigned char           brightness
    )
{
    unsigned char usb_buf[65];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x51;
    usb_buf[0x02]   = 0x2C;
    usb_buf[0x03]   = 0x07;
    usb_buf[0x04]   = 0x01;
    usb_buf[0x05]   = speed;
    usb_buf[0x06]   = brightness;
    usb_buf[0x07]   = color_mode;
    usb_buf[0x08]   = direction;
    usb_buf[0x09]   = 0x02;

    for(unsigned int i = 0; i < 3; i ++)
    {
        usb_buf[10 + i * 3] = RGBGetRValue(colors[i]);
        usb_buf[11 + i * 3] = RGBGetGValue(colors[i]);
        usb_buf[12 + i * 3] = RGBGetBValue(colors[i]);
    }

    ClearResponses();
    hid_write(dev, usb_buf, 65);
    AwaitResponse(20);

    memset(usb_buf + 4, 0x00, sizeof(usb_buf) - 4);

    usb_buf[0x04] = 0x00;

    for(unsigned int i = 0; i < 3; i ++)
    {
        usb_buf[5 + i * 3] = RGBGetRValue(colors[i+3]);
        usb_buf[6 + i * 3] = RGBGetGValue(colors[i+3]);
        usb_buf[7 + i * 3] = RGBGetBValue(colors[i+3]);
    }

    ClearResponses();
    hid_write(dev, usb_buf, 65);
    AwaitResponse(20);
}

void AuraTUFKeyboardController::UpdateDevice
    (
    unsigned char           mode,
    std::vector<RGBColor>   colors,
    unsigned char           direction,
    unsigned char           color_mode,
    unsigned char           speed,
    unsigned char           brightness
    )
{
    if(device_pid == AURA_TUF_K1_GAMING_PID && mode == AURA_KEYBOARD_MODE_WAVE)
    {
        return UpdateK1Wave(colors, direction, speed, brightness);
    }

    if(device_pid == AURA_ROG_AZOTH_USB_PID
    || device_pid == AURA_ROG_AZOTH_2_4_PID
    || device_pid == AURA_ROG_STRIX_SCOPE_NX_WIRELESS_DELUXE_USB_PID
    || device_pid == AURA_ROG_STRIX_SCOPE_NX_WIRELESS_DELUXE_2_4_PID
    || device_pid == AURA_ROG_STRIX_SCOPE_II_PID
    || device_pid == AURA_ROG_STRIX_SCOPE_II_RX_PID
    || device_pid == AURA_ROG_STRIX_SCOPE_II_96_WIRELESS_USB_PID)
    {
        if(mode == AURA_KEYBOARD_MODE_WAVE || mode == AURA_KEYBOARD_MODE_RIPPLE)
        {
            return UpdateScopeIIRainbowRipple(mode, colors, direction, color_mode, speed, brightness);
        }
        else if (mode == AURA_KEYBOARD_MODE_QUICKSAND)
        {
            return UpdateScopeIIQuicksand(colors, direction, color_mode, speed, brightness);
        }
    }

    unsigned char usb_buf[65];
    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x51;
    usb_buf[0x02]   = 0x2C;

    if(device_pid != AURA_ROG_CLAYMORE_PID)
    {
        usb_buf[0x03]   = mode;
        usb_buf[0x04]   = 0x00;
        usb_buf[0x05]   = speed;
        usb_buf[0x06]   = brightness;
        usb_buf[0x07]   = color_mode;
        usb_buf[0x08]   = direction;

        if(is_per_led_keyboard)
        {
            usb_buf[0x09]   = 0x02;

            if(mode == AURA_KEYBOARD_MODE_WAVE || mode == AURA_KEYBOARD_MODE_RIPPLE)
            {
                usb_buf[0x0A]   = (unsigned char)colors.size();

                /*-----------------------------------------------------*\
                | Loop over every color given                           |
                \*-----------------------------------------------------*/
                for(unsigned int i = 0; i < colors.size(); i ++)
                {
                    if(colors[i])
                    {
                        usb_buf[11 + i * 4] = (unsigned char)(100.0f / (float)colors.size() * (i + 1));
                        usb_buf[12 + i * 4] = RGBGetRValue(colors[i]);
                        usb_buf[13 + i * 4] = RGBGetGValue(colors[i]);
                        usb_buf[14 + i * 4] = RGBGetBValue(colors[i]);
                    }
                }
            }
            else
            {
                /*-----------------------------------------------------*\
                | Loop over Color1, Color2 and Background if there      |
                \*-----------------------------------------------------*/
                for(unsigned int i = 0; i != colors.size(); i++)
                {
                    if(colors[i])
                    {
                        usb_buf[10 + i * 3] = RGBGetRValue(colors[i]);
                        usb_buf[11 + i * 3] = RGBGetGValue(colors[i]);
                        usb_buf[12 + i * 3] = RGBGetBValue(colors[i]);
                    }
                }

            }
        }
        else
        {
            /*-----------------------------------------------------------------*\
            | Only handles K5 Rainbow.                                          |
            | K1 is filtered out earlier and is executed in `UpdateK1Wave()`    |
            \*-----------------------------------------------------------------*/
            if(mode == AURA_KEYBOARD_MODE_WAVE)
            {
                usb_buf[0x09]   = 0x05;

                for(unsigned int i = 0; i < 5; i ++)
                {
                    usb_buf[10 + i * 4] = i + 1;
                    usb_buf[11 + i * 4] = RGBGetRValue(colors[i]);
                    usb_buf[12 + i * 4] = RGBGetGValue(colors[i]);
                    usb_buf[13 + i * 4] = RGBGetBValue(colors[i]);
                }
            }
            else
            {
                /*-----------------------------------------------------*\
                | Loop over Color1, Color2 and Background if there      |
                \*-----------------------------------------------------*/
                for(unsigned int i = 0; i != colors.size(); i++)
                {
                    if(colors[i])
                    {
                        usb_buf[ 9 + i * 3] = RGBGetRValue(colors[i]);
                        usb_buf[10 + i * 3] = RGBGetGValue(colors[i]);
                        usb_buf[11 + i * 3] = RGBGetBValue(colors[i]);
                    }
                }
            }
        }
    }
    else
    {
        usb_buf[0x03]   = 0x00;
        usb_buf[0x04]   = 0x00;
        usb_buf[0x05]   = mode;
        usb_buf[0x06]   = speed;

        bool random     = (color_mode == 1);
        bool pattern    = (color_mode == 2);

        usb_buf[0x07]   = random * 128 + pattern * 32 + direction;
        usb_buf[0x08]   = 0xFF; // "byteExt1" unknown usage
        usb_buf[0x09]   = 0xFF; // "byteExt2" unknown usage
        usb_buf[0x0A]   = (mode == 2) ? 0x80 : 0xFF; // "Lightness" (not Brightness)

        if(mode == 7)
        {
            UpdateQuicksandColors(colors);
        }
        else
        {
            if(mode == 4 && color_mode == 0)
            {
                usb_buf[11] = 0xFF;
                usb_buf[12] = 0xFF;
                usb_buf[13] = 0xFF;
            }

            for(unsigned int i = 0; i < colors.size(); i ++)
            {
                if(colors[i])
                {
                    usb_buf[11 + i * 3] = RGBGetRValue(colors[i]);
                    usb_buf[12 + i * 3] = RGBGetGValue(colors[i]);
                    usb_buf[13 + i * 3] = RGBGetBValue(colors[i]);
                }
            }
        }

        for(int i = 1; i < 5; i++)
        {
            usb_buf[5 + i * 12] = 0xFF;
        }
    }

    ClearResponses();
    hid_write(dev, usb_buf, 65);
    AwaitResponse(20);
}

/*---------------------------------------------------------*\
| only needed for Claymore                                  |
\*---------------------------------------------------------*/
void AuraTUFKeyboardController::UpdateQuicksandColors
    (
    std::vector<RGBColor>   colors
    )
{
    unsigned char usb_buf[65];
    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x51;
    usb_buf[0x02]   = 0x91;

    for(unsigned int i = 0; i < 6; i++)
    {
        if(colors[i])
        {
            usb_buf[5 + i * 3] = RGBGetRValue(colors[i]);
            usb_buf[6 + i * 3] = RGBGetGValue(colors[i]);
            usb_buf[7 + i * 3] = RGBGetBValue(colors[i]);
        }
    }

    ClearResponses();
    hid_write(dev, usb_buf, 65);
    AwaitResponse(20);
}

/*---------------------------------------------------------*\
| only needed for Claymore                                  |
\*---------------------------------------------------------*/
void AuraTUFKeyboardController::UpdateMode(unsigned char mode)
{
    unsigned char usb_buf[65];
    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x51;
    usb_buf[0x02]   = 0x28;
    usb_buf[0x03]   = 0x00;
    usb_buf[0x04]   = 0x00;
    usb_buf[0x05]   = mode;

    ClearResponses();
    hid_write(dev, usb_buf, 65);
    AwaitResponse(20);
}

void AuraTUFKeyboardController::AwaitResponse(int ms)
{
    unsigned char usb_buf_out[65];
    hid_read_timeout(dev, usb_buf_out, 65, ms);
}

void AuraTUFKeyboardController::ClearResponses()
{
    int result = 1;
    unsigned char usb_buf_flush[65];
    while(result > 0)
    {
        result = hid_read_timeout(dev, usb_buf_flush, 65, 0);
    }
}

