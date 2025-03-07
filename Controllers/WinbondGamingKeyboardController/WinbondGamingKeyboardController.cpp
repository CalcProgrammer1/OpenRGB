/*---------------------------------------------------------*\
| WinbondGamingKeyboardController.cpp                       |
|                                                           |
|   Driver for Winbond Gaming Keyboard                      |
|                                                           |
|   Daniel Gibson                               03 Dec 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <algorithm>
#include <string.h>
#include "LogManager.h"
#include "StringUtils.h"
#include "WinbondGamingKeyboardController.h"
#include "RGBController_WinbondGamingKeyboard.h"

#define WINBOND_HID_DATA_LEN 64

WinbondGamingKeyboardController::WinbondGamingKeyboardController(hid_device *dev_handle, const hid_device_info &info, const std::string& name)
    : dev(dev_handle)
{
    location = "HID: ";
    location += info.path;

    SetNameVendorDescription(info, name);
    SetVersionLayout();
}


void WinbondGamingKeyboardController::SetNameVendorDescription(const hid_device_info &info, const std::string& devname)
{
    bool using_product_string = false;
    if(info.product_string != nullptr && info.product_string[0] != 0)
    {
        using_product_string    = true;
        /*----------------------------------------------------------------------------------------------------*\
        | info->product_string can have at most 126 wchars + terminating 0                                     |
        | (according to                                                                                        |
        |  https://stackoverflow.com/questions/7193645/how-long-is-the-string-of-manufacturer-of-a-usb-device) |
        | in UTF-8 that's at most 126*4 chars + terminating 0                                                  |
        \*----------------------------------------------------------------------------------------------------*/
        char product_name[126*4 + 1];
        snprintf(product_name, sizeof(product_name), "%ls", info.product_string);
        name                    = product_name;
    }
    else
    {
        name                    = devname;
    }

    /*----------------------------------------------------------------------------*\
    | the Pulsar PCMK TKL keyboard (barebone) uses "PCMK TKL" as product string,   |
    | at least with the latest firmware. I assume that other Pulsar keyboards also |
    | contain "PCMK", so use that to set the vendor to Pulsar (unfortunately,      |
    |  Pulsar doesn't seem to set iManufacturer in the USB device descriptor)      |
    \*----------------------------------------------------------------------------*/
    if(name.find("PCMK") != std::string::npos)
    {
        vendor      = "Pulsar";
        if(name.find("TKL") != std::string::npos)
        {
            kb_size = KEYBOARD_SIZE_TKL;
            hasLogoLight = true;
        }
        else if(name.find("60") != std::string::npos)
        {
            /*---------------------------------------------------*\
            | I *guess* that the PCMK 60% has 60 in its name here |
            | (if it even uses the same USB PID:VID ...)          |
            \*---------------------------------------------------*/
            kb_size = KEYBOARD_SIZE_SIXTY;
        }

    }
    else if((name.find("Rockfall") != std::string::npos) || (name.find("Skyfall") != std::string::npos))
    {
        vendor      = "Hator";
        layout      = KEYBOARD_LAYOUT_ANSI_QWERTY;
        if(name.find("TKL") != std::string::npos)
        {
            kb_size = KEYBOARD_SIZE_TKL;
        }
    }
    else
    {
        vendor      = "Winbond";
    }

    if(using_product_string)
    {
        description = vendor + " " + name;
    }
    else
    {
        description = name;
    }
}

void WinbondGamingKeyboardController::ParseVersionString(const char *str)
{
    /*------------------------------------------------------------------------------*\
    | str should be something like "2NUC,01,KB,FL,K221UKCVRGB,V1.05.03" (for ISO/UK) |
    | we're interested in the chars before "CVRGB" for the layout,                   |
    | and the last "V" and following for the version string                          |
    \*------------------------------------------------------------------------------*/
    const char* ver = strrchr(str, 'V');
    if(ver != nullptr)
    {
        version = ver;
    }
    else
    {
        version = "???";
    }

    LOG_DEBUG("[%s] Version response was: '%s'", name.c_str(), str);

    const char* cvrgb = strstr(str, "CVRGB");
    if(cvrgb != nullptr && cvrgb > str + 2)
    {
        const char* lang = cvrgb - 2;

        if(lang[0] == 'U' && lang[1] == 'K')
        {
            LOG_DEBUG("[%s] Detected ISO layout from that version string", name.c_str());
            layout = KEYBOARD_LAYOUT_ISO_QWERTY;
        }
        else if(lang[0] == 'J' && lang[1] == 'P')
        {
            LOG_DEBUG("[%s] Detected JIS layout from that version string", name.c_str());
            layout = KEYBOARD_LAYOUT_JIS;
        }
        else
        {
            LOG_DEBUG("[%s] Detected ANSI layout from that version string", name.c_str());
            layout = KEYBOARD_LAYOUT_ANSI_QWERTY;
        }
    }
    else
    {
        LOG_DEBUG("[%s] Couldn't detect any layout from that string (didn't contain \"CVRGB\"), defaulting to ISO", name.c_str());
    }

    /*------------------------------------------------------------------------------------------*\
    | NOTE: I don't know exactly how the string would look like on ANSI or JIS keyboards,        |
    | but the firmware updaters for Pulsar PCMK TKL are called                                   |
    |    K221CVRGB_V10507.exe for ANSI                                                           |
    |  K221JPCVRGB_V10503.exe for JIS                                                            |
    |  K221UKCVRGB_V10503.exe for ISO                                                            |
    | so I assume that ANSI indeed has no extra string before CVRGB and JIS has "JP" there       |
    |                                                                                            |
    | For future reference, PCMK 60% JIS firmware name: GD147CKB_M252KBFL_K225JPCVRGB_V10408.exe |
    | no idea if that even uses the same chip, and how the LEDs are handled, though              |
    |                                                                                            |
    | Furthermore, no idea how to detect the layout of other Winbond Gaming Keyboards,           |
    | like KT108 or KT87 (if that one really uses the same chip).                                |
    | Maybe they use a similar version reply, maybe not...                                       |
    \*------------------------------------------------------------------------------------------*/
}

void WinbondGamingKeyboardController::SetVersionLayout()
{
    {
        /*-------------------------------------------------------------*\
        | this requests a string with information about the version etc |
        \*-------------------------------------------------------------*/
        unsigned char msg[WINBOND_HID_DATA_LEN] = { 0x01, 0x0D, 0 };
        hid_write(dev, msg, WINBOND_HID_DATA_LEN);
    }

    /*--------------------------------------------------------*\
    | the reply looks like                                     |
    | 0x1, 0x0D, 0, 0, 0, "2NUC,01,KB,FL,K221UKCVRGB,V1.05.03" |
    \*--------------------------------------------------------*/
    for(int i=0; i<10; ++i) // 10 retries
    {
        /*-----------------------------------------------------------------------------*\
        | +1 to make sure there's always a terminating \0 byte at the end of the string |
        \*-----------------------------------------------------------------------------*/
        unsigned char reply[WINBOND_HID_DATA_LEN + 1] = {};
        int len = hid_read_timeout(dev, reply, WINBOND_HID_DATA_LEN, 150);
        if(len < 0)
        {
            continue;
        }
        if(reply[0] != 1 || reply[1] != 0x0D || reply[4] != 0) // not the message we want
        {
            continue;
        }

        const char* str = (const char*)reply + 2; // skip 0x01 0x0D bytes
        /*-----------------------------------------------------------------*\
        | skip any leading whitespace, \0 bytes and other unprintable chars |
        \*-----------------------------------------------------------------*/
        for(int j=0; j < WINBOND_HID_DATA_LEN - 2; ++j, ++str)
        {
            if(*str > ' ' )
                break;
        }
        if(*str != '\0')
        {
            ParseVersionString(str);
            return;
        }
    }

    /*--------------*\
    | fallback       |
    \*--------------*/
    version = "???";
}

std::string WinbondGamingKeyboardController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

static void setModeImpl(hid_device* dev, bool is_logo, unsigned char effect_mode, unsigned char colors[2][3],
                        bool full_color, unsigned char direction, unsigned char speed, unsigned char brightness)
{
    unsigned char buf[WINBOND_HID_DATA_LEN] =
    {
        1,             // byte 0: Report ID (always 1)
        7,             // byte 1: the "command", in this case 7 (set key LED mode) or 8 (set logo LED mode)
        0, 0, 0, 0x0E, // bytes 2-5: not sure about the meaning, they were like this..
        effect_mode,   // byte 6 (effect mode, like WINBOND_GK_MODE_STATIC)
        brightness,    // byte 7
        speed          // byte 8
        // the remaining bytes are set below or remain 0
    };

    if(is_logo)
    {
        buf[1] = 8;
        buf[5] = 0x0D;
    }

    /*------------------------------*\
    | bytes 9-14 are fg and bg color |
    \*------------------------------*/
    memcpy(buf+9, colors, 2*3);

    buf[15] = direction;
    buf[16] = full_color;
    // the rest of the buffer remains 0

    hid_write(dev, buf, WINBOND_HID_DATA_LEN);
}

void WinbondGamingKeyboardController::SetLEDsData(const std::vector<RGBColor>& colors, const std::vector<led>& leds, int brightness)
{
    /*---------------------------------------------------------------------------*\
    | There are 8 HID messages to set the LEDs.                                   |
    | Each message starts with the bytes shown below, followed by 18 RGB triplets |
    | (except for the last message that has 12 RGB triplets). These triplets only |
    | use values up to 0xC1 (193), instead of the usual 0xFF (255).               |
    | Byte 0 is the Report ID (1), 1 is the command (9), 2 and 3 are always 0 (?),|
    |  4 is the message index, 5 is the length of the following RGB data in bytes |
    \*---------------------------------------------------------------------------*/
    unsigned char msgs[8][WINBOND_HID_DATA_LEN] =
    {
        { 1, 9, 0, 0, 0, 0x36, 0 },
        { 1, 9, 0, 0, 1, 0x36, 0 },
        { 1, 9, 0, 0, 2, 0x36, 0 },
        { 1, 9, 0, 0, 3, 0x36, 0 },
        { 1, 9, 0, 0, 4, 0x36, 0 },
        { 1, 9, 0, 0, 5, 0x36, 0 },
        { 1, 9, 0, 0, 6, 0x36, 0 },
        { 1, 9, 0, 0, 7, 0x12, 0 },
    };

    RGBColor logo_color = ToRGBColor(128, 128, 128);

    for(size_t i = 0, n = colors.size(); i < n; ++i)
    {
        unsigned val = leds[i].value;
        if(val == 0) // no value set
        {
            continue;
        }

        /*--------------------------------------------------------------------------------------------------*\
        | the following two lines are the inverse of the KV() macro in RGBController_WinbondGamingKeyboard.h |
        \*--------------------------------------------------------------------------------------------------*/
        int msg_num = (val >> 8) & 255;
        int r_offset = (val & 255);
        r_offset = r_offset*3 + 6; // 6 is position of first color byte in message
        /*----------------------------*\
        | special case: logo LED color |
        \*----------------------------*/
        if(msg_num == 255)
        {
            logo_color = colors[i];
            // logo light is set separately, just remember its color
            continue;
        }

        msg_num &= 7; // 0..7

        /*----------------------------*\
        | transform 0..0xFF to 0..0xC1 |
        \*----------------------------*/
        int r = (RGBGetRValue(colors[i]) * 0xC1) / 0xFF;
        int g = (RGBGetGValue(colors[i]) * 0xC1) / 0xFF;
        int b = (RGBGetBValue(colors[i]) * 0xC1) / 0xFF;
        msgs[msg_num][r_offset]   = r & 255;
        msgs[msg_num][r_offset+1] = g & 255;
        msgs[msg_num][r_offset+2] = b & 255;
    }

    for(int i=0; i<8; ++i)
    {
        hid_write(dev, msgs[i], WINBOND_HID_DATA_LEN);
    }

    if(hasLogoLight)
    {
        unsigned char colors[2][3] = {};
        colors[0][0] = RGBGetRValue(logo_color);
        colors[0][1] = RGBGetGValue(logo_color);
        colors[0][2] = RGBGetBValue(logo_color);
        setModeImpl(dev, true, WINBOND_GK_MODE_STATIC, colors, false, 0, 3, brightness);
    }
}

static unsigned char getDirection(unsigned int dir, int effect)
{
    /*---------------------------------------------------------------*\
    | Winbond effect directions: 0: right, 1: left, 2: up, 3: down,   |
    | 4: to outside, 5: to inside, 6: clockwise, 7: counter-clockwise |
    \*---------------------------------------------------------------*/

    switch(effect)
    {
        case WINBOND_GK_MODE_WAVE:
            /*-----------------------------------*\
            | LR, UD; HV for to inside/to outside |
            \*-----------------------------------*/
            switch(dir)
            {
                case MODE_DIRECTION_LEFT:
                    return 1;
                case MODE_DIRECTION_RIGHT:
                    return 0;
                case MODE_DIRECTION_UP:
                    return 2;
                case MODE_DIRECTION_DOWN:
                    return 3;
                case MODE_DIRECTION_HORIZONTAL:
                    return 4;
                case MODE_DIRECTION_VERTICAL:
                    return 5;
            }

            break;
        case WINBOND_GK_MODE_SNAKE: // DOWN/UP for CW/CCW
            return dir == MODE_DIRECTION_DOWN ? 6 : 7;
        case WINBOND_GK_MODE_AURORA: // HORIZONTAL/VERTICAL for to outside/to inside
            return dir == MODE_DIRECTION_HORIZONTAL ? 4 : 5;
    }
    return 0; // effects without a direction
}

void WinbondGamingKeyboardController::SetMode(const mode& m)
{
    unsigned char colors[2][3] = { {0, 0, 255}, {0, 0, 0} };

    for(size_t i=0; i < std::min((size_t)2, m.colors.size()); ++i)
    {
        RGBColor c = m.colors[i];
        colors[i][0] = RGBGetRValue(c);
        colors[i][1] = RGBGetGValue(c);
        colors[i][2] = RGBGetBValue(c);
    }

    unsigned char direction = getDirection(m.direction, m.value);
    bool full_color = (m.color_mode == MODE_COLORS_RANDOM || m.value == WINBOND_GK_MODE_CUSTOM);

    setModeImpl(dev, false, m.value, colors, full_color, direction, m.speed, m.brightness);

    if(hasLogoLight)
    {
        /*---------------------------------------------------------------------------------*\
        | logo light supports static, neon (WINBOND_GK_MODE_LOGO_NEON), breathe, wave       |
        | all logo modes have brightness, all but static have speed and 2 colors + random   |
        | select a mode supported by the logo that matches the key mode                     |
        |                                                                                   |
        | TODO: the keyboard allows selecting completely different modes for keys and the   |
        |       logo light, but OpenRGB currently doesn't support different modes per zone. |
        |       if OpenRGB ever supports that, change the code accordingly                  |
        \*---------------------------------------------------------------------------------*/
        int mode = m.value;
        int speed = m.speed;
        if(mode == WINBOND_GK_MODE_NEON)
        {
            mode = WINBOND_GK_MODE_LOGO_NEON;
        }
        else if(mode == WINBOND_GK_MODE_SNAKE)
        {
            mode = WINBOND_GK_MODE_WAVE;
            direction = (direction == 6) ? 1 : 0;
        }
        else if(mode > WINBOND_GK_MODE_SNAKE)
        {
            /*------------------------------------------------------*\
            | these remaining modes are reactive, no real equivalent |
            | => use static                                          |
            \*------------------------------------------------------*/
            mode = WINBOND_GK_MODE_STATIC;
            direction = 0;
            speed = 0;
        }
        // else: the other modes < WINBOND_GK_MODE_SNAKE are supported as is

        setModeImpl(dev, true, mode, colors, full_color, direction, speed, m.brightness);
    }
}
