/*---------------------------------------------------------*\
| MountainKeyboardController.h                              |
|                                                           |
|   Driver for Mountain keyboard                            |
|                                                           |
|   O'D.Sæzl                                       Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define MOUNTAIN60_KEYBOARD_MAX_TRANSFER_COLORS             191
#define MOUNTAIN60_KEYBOARD_TRANSFER_BUFFER_SIZE            (4*MOUNTAIN60_KEYBOARD_MAX_TRANSFER_COLORS)

#define MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE                 65
#define MOUNTAIN60_KEYBOARD_USB_BUFFER_HEADER_SIZE          9

#define MOUNTAIN60_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE     \
(MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE-MOUNTAIN60_KEYBOARD_USB_BUFFER_HEADER_SIZE)

    enum
{
    MOUNTAIN60_KEYBOARD_CHECK_NUMPAD           = 0x08,
    MOUNTAIN60_KEYBOARD_SELECT_MODE_CMD        = 0x16,
    MOUNTAIN60_KEYBOARD_SEND_CMD               = 0x17,
    MOUNTAIN60_KEYBOARD_START_DIRECT_CMD       = 0x34,
    MOUNTAIN60_KEYBOARD_MAP_DIRECT_CMD         = 0x35,
    MOUNTAIN60_KEYBOARD_END_DIRECT_CMD         = 0x36,
    MOUNTAIN60_KEYBOARD_SAVE_CMD               = 0x1A,
};

enum
{
    MOUNTAIN60_KEYBOARD_MODE_STATIC            = 0x01,
    MOUNTAIN60_KEYBOARD_MODE_COLOR_WAVE        = 0x02,
    MOUNTAIN60_KEYBOARD_MODE_TORNADO           = 0x03,
    MOUNTAIN60_KEYBOARD_MODE_BREATHING         = 0x04,
    MOUNTAIN60_KEYBOARD_MODE_REACTIVE          = 0x05,
    MOUNTAIN60_KEYBOARD_MODE_MATRIX            = 0x06,
    MOUNTAIN60_KEYBOARD_MODE_CUSTOM            = 0x07,
    MOUNTAIN60_KEYBOARD_MODE_YETI              = 0x08,
    MOUNTAIN60_KEYBOARD_MODE_OFF               = 0x09,
    MOUNTAIN60_KEYBOARD_MODE_INVALID           = 0xFF,
};

enum
{
    MOUNTAIN60_KEYBOARD_COLOR_MODE_RAINBOW     = 0x02,
    MOUNTAIN60_KEYBOARD_COLOR_MODE_SINGLE      = 0x00,
    MOUNTAIN60_KEYBOARD_COLOR_MODE_DUAL        = 0x10
};

enum
{
    MOUNTAIN60_KEYBOARD_DIRECTION_UP           = 0x06,
    MOUNTAIN60_KEYBOARD_DIRECTION_DOWN         = 0x02,
    MOUNTAIN60_KEYBOARD_DIRECTION_LEFT         = 0x04,
    MOUNTAIN60_KEYBOARD_DIRECTION_RIGHT        = 0x00,
    MOUNTAIN60_KEYBOARD_DIRECTION_ANTICLK      = 0x0A,
    MOUNTAIN60_KEYBOARD_DIRECTION_CLK          = 0x09,
};

class Mountain60KeyboardController
{
public:
    Mountain60KeyboardController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~Mountain60KeyboardController();

    std::string     GetDeviceLocation();
    std::string     GetNameString();
    std::string     GetSerialString();

    void            UpdateData();
    void            SaveData(unsigned char mode_idx);
    void            SelectMode(unsigned char mode_idx);
    void            SendModeDetails(const mode* current_mode);
    void            SendDirect(unsigned int brightness,unsigned char* color_data, unsigned int color_count);

private:
    hid_device*     dev;
    std::string     location;
    std::string     name;

    unsigned char   ConvertDirection(unsigned int direction, bool rotation);

    void            SendDirectStartPacketCmd(unsigned int brightness);
    void            SendDirectPacketCmd(unsigned char stream_control, unsigned char *data, unsigned int data_size);
    void            SendDirectPacketFinishCmd();
};
