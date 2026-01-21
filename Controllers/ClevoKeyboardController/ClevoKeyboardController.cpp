/*---------------------------------------------------------*\
| ClevoKeyboardController.cpp                               |
|                                                           |
|   Driver for Clevo laptop per-key RGB keyboard (ITE 8291) |
|   Protocol based on tuxedo-drivers ite_8291 module        |
|                                                           |
|   Kyle Cascade (kyle@cascade.family)          16 Jan 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "ClevoKeyboardController.h"
#include <cstring>

ClevoKeyboardController::ClevoKeyboardController(hid_device* dev_handle, const hid_device_info& info)
{
    dev      = dev_handle;
    location = info.path;
    version  = info.release_number;
}

ClevoKeyboardController::~ClevoKeyboardController()
{
    hid_close(dev);
}

std::string ClevoKeyboardController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string ClevoKeyboardController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());

    return(return_string);
}

std::string ClevoKeyboardController::GetFirmwareVersion()
{
    char version_string[16];
    snprintf(version_string, sizeof(version_string), "%d.%02d", version >> 8, version & 0xFF);
    return(version_string);
}

void ClevoKeyboardController::WriteControl(unsigned char* data)
{
    hid_send_feature_report(dev, data, CLEVO_KEYBOARD_REPORT_SIZE);
}

void ClevoKeyboardController::WriteRowData(unsigned char* data)
{
    hid_write(dev, data, CLEVO_KEYBOARD_ROW_DATA_SIZE);
}

void ClevoKeyboardController::TurnOff()
{
    /*---------------------------------------------------------*\
    | Turn off: 08 01 00 00 00 00 00 00                         |
    \*---------------------------------------------------------*/
    unsigned char buf[CLEVO_KEYBOARD_REPORT_SIZE];

    memset(buf, 0x00, CLEVO_KEYBOARD_REPORT_SIZE);
    buf[0] = 0x08;
    buf[1] = 0x01;

    WriteControl(buf);
}

void ClevoKeyboardController::SetMode(unsigned char mode, unsigned char brightness, unsigned char speed, unsigned char behaviour)
{
    /*---------------------------------------------------------*\
    | Set params: 08 [power] [mode] [speed] [brightness] 08     |
    |             [behaviour] 00                                |
    | power: 01=off, 02=on                                      |
    \*---------------------------------------------------------*/
    unsigned char buf[CLEVO_KEYBOARD_REPORT_SIZE];

    memset(buf, 0x00, CLEVO_KEYBOARD_REPORT_SIZE);
    buf[0] = 0x08;
    buf[1] = 0x02;                                          // Power on
    buf[2] = mode;
    buf[3] = speed;
    buf[4] = brightness;
    buf[5] = 0x08;
    buf[6] = behaviour;

    WriteControl(buf);
}

void ClevoKeyboardController::SetModeColor(unsigned char color_idx, unsigned char red, unsigned char green, unsigned char blue)
{
    /*---------------------------------------------------------*\
    | Set color define: 14 00 [index] R G B 00 00               |
    | index: 1-7 for built-in effects                           |
    \*---------------------------------------------------------*/
    unsigned char buf[CLEVO_KEYBOARD_REPORT_SIZE];

    memset(buf, 0x00, CLEVO_KEYBOARD_REPORT_SIZE);
    buf[0] = 0x14;
    buf[1] = 0x00;
    buf[2] = color_idx;
    buf[3] = red;
    buf[4] = green;
    buf[5] = blue;

    WriteControl(buf);
}

void ClevoKeyboardController::SendColors(unsigned char* color_data, unsigned char brightness)
{
    /*---------------------------------------------------------*\
    | Per-key RGB mode (mode 0x33)                              |
    | 1. Set params with mode 0x33 and brightness               |
    | 2. For each row 0-5:                                      |
    |    - Announce row: 16 00 [row] 00 00 00 00 00             |
    |    - Send 65 bytes row data via output report             |
    |                                                           |
    | Row data format (65 bytes):                               |
    |   [0x00 padding][0x00 padding]                            |
    |   [B0..B20][G0..G20][R0..R20]                             |
    \*---------------------------------------------------------*/
    unsigned char ctrl_buf[CLEVO_KEYBOARD_REPORT_SIZE];
    unsigned char row_buf[CLEVO_KEYBOARD_ROW_DATA_SIZE];

    /*---------------------------------------------------------*\
    | Clamp brightness                                          |
    \*---------------------------------------------------------*/
    if(brightness > CLEVO_KEYBOARD_BRIGHTNESS_MAX)
    {
        brightness = CLEVO_KEYBOARD_BRIGHTNESS_MAX;
    }

    /*---------------------------------------------------------*\
    | Set params for per-key mode                               |
    \*---------------------------------------------------------*/
    memset(ctrl_buf, 0x00, CLEVO_KEYBOARD_REPORT_SIZE);
    ctrl_buf[0] = 0x08;
    ctrl_buf[1] = 0x02;                                     // Power on
    ctrl_buf[2] = CLEVO_KEYBOARD_MODE_DIRECT;               // Per-key mode
    ctrl_buf[3] = 0x00;                                     // Speed (unused)
    ctrl_buf[4] = brightness;
    ctrl_buf[5] = 0x00;
    ctrl_buf[6] = 0x00;

    WriteControl(ctrl_buf);

    /*---------------------------------------------------------*\
    | Send each row                                             |
    \*---------------------------------------------------------*/
    for(int row = 0; row < CLEVO_KEYBOARD_NUM_ROWS; row++)
    {
        /*-----------------------------------------------------*\
        | Announce row data                                     |
        \*-----------------------------------------------------*/
        memset(ctrl_buf, 0x00, CLEVO_KEYBOARD_REPORT_SIZE);
        ctrl_buf[0] = 0x16;
        ctrl_buf[1] = 0x00;
        ctrl_buf[2] = row;

        WriteControl(ctrl_buf);

        /*-----------------------------------------------------*\
        | Build row data buffer                                 |
        | Format: [pad][pad][B0..B20][G0..G20][R0..R20]         |
        \*-----------------------------------------------------*/
        memset(row_buf, 0x00, CLEVO_KEYBOARD_ROW_DATA_SIZE);

        for(int col = 0; col < CLEVO_KEYBOARD_NUM_COLS; col++)
        {
            int led_idx = (row * CLEVO_KEYBOARD_NUM_COLS) + col;
            int color_offset = led_idx * 3;

            unsigned char red   = color_data[color_offset + 0];
            unsigned char green = color_data[color_offset + 1];
            unsigned char blue  = color_data[color_offset + 2];

            /*-------------------------------------------------*\
            | Row data layout (after 2-byte padding):           |
            | Bytes  2-22: Blue values for columns 0-20         |
            | Bytes 23-43: Green values for columns 0-20        |
            | Bytes 44-64: Red values for columns 0-20          |
            \*-------------------------------------------------*/
            row_buf[2 + col]                             = blue;
            row_buf[2 + CLEVO_KEYBOARD_NUM_COLS + col]   = green;
            row_buf[2 + CLEVO_KEYBOARD_NUM_COLS*2 + col] = red;
        }

        WriteRowData(row_buf);
    }
}
