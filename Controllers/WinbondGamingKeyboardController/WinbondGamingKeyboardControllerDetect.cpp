/*---------------------------------------------------------*\
| WinbondGamingKeyboardControllerDetect.cpp                 |
|                                                           |
|   Detector for Winbond Gaming Keyboard                    |
|                                                           |
|   Daniel Gibson                               03 Dec 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "RGBController_WinbondGamingKeyboard.h"
#include "LogManager.h"

/*-----------------------------------------------------*\
| Winbond vendor ID                                     |
\*-----------------------------------------------------*/
#define WINBOND_VID 0x0416

/*-----------------------------------------------------*\
| Winbond product ID                                    |
\*-----------------------------------------------------*/
#define WINBOND_GAMING_KEYBOARD_PID 0xB23C

void DetectWinbondGamingKeyboard(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    /*--------------------------------------------------------------------------------------------------*\
    | NOTE: according to https://4pda.to/forum/index.php?showtopic=1061923,                              |
    | the "KT108" keyboard, which has the same VID:PID, uses the product_string "KT108 keyboard"         |
    | that could be used for KT108-specific settings? OTOH, according to                                 |
    | https://usb-ids.gowdy.us/read/UD/0416/b23c there are also variants of KT108 with                   |
    | product string "Gaming Keyboard".                                                                  |
    | Apart from the KT108, there's also a KT87, but no idea about its product_string,                   |
    | or even its VID/PID (I *assume* it's also 0416:B23C)                                               |
    | KT87 and KT108 seem to be sold under the brands WIANXP, Nautilus and Capturer                      |
    |                                                                                                    |
    | Apart from those noname keyboards that one might only find on aliexpress and similar shops,        |
    | the Pulsar PCMK TKL keyboard (barebone) uses this VID+PID, and that is the one this is             |
    | tested with - the ISO variant, specifically.                                                       |
    | ANSI and JIS variants also exist, I'll try to support them as best as I can.                       |
    |                                                                                                    |
    | Pulsar also offers a 60% barebone in ISO, ANSI and JIS, but no idea about its VID or PID,          |
    | or product_string, much less about its protocol (even if it uses the same firmware,                |
    | I don't know which key corresponds to which bytes in the HID message that sets the per-key colors) |
    \*--------------------------------------------------------------------------------------------------*/

    if(dev)
    {
        LOG_INFO("Detected WinbondGamingKeyboard at %s, product_string is %ls name is %s", info->path, info->product_string, name.c_str());

        WinbondGamingKeyboardController* controller = new WinbondGamingKeyboardController(dev, *info, name);
        RGBController* rgb_controller               = new RGBController_WinbondGamingKeyboard(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
    else
    {
        LOG_WARNING("Couldn't open hid dev %s: %ls", info->path, hid_error(NULL));
    }
}

REGISTER_HID_DETECTOR_PU("Winbond Gaming Keyboard", DetectWinbondGamingKeyboard, WINBOND_VID, WINBOND_GAMING_KEYBOARD_PID, 0xFF1B, 0x91);
