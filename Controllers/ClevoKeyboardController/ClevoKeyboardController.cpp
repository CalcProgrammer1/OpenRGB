/*---------------------------------------------------------*\
| ClevoKeyboardController.cpp                               |
|                                                           |
|   Driver for Clevo per-key RGB keyboard                   |
|   Supports ITE 8291 (PID 0x600B) and ITE 829x (0x8910)    |
|                                                           |
|   Kyle Cascade (kyle@cascade.family)          16 Jan 2026 |
|   Valentin Lobstein (balgogan@protonmail.com) 27 Mar 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <algorithm>
#include <cstring>
#include "ClevoKeyboardController.h"
#include "StringUtils.h"

ClevoKeyboardController::ClevoKeyboardController(hid_device* dev_handle, const hid_device_info& info, clevo_keyboard_type kb_type)
{
    dev      = dev_handle;
    type     = kb_type;
    location = info.path;
    version  = info.release_number;
}

ClevoKeyboardController::~ClevoKeyboardController()
{
    hid_close(dev);
}

clevo_keyboard_type ClevoKeyboardController::GetType()
{
    return type;
}

int ClevoKeyboardController::GetNumRows()
{
    return(type == CLEVO_KB_ITE829X) ? CLEVO_829X_NUM_ROWS : CLEVO_8291_NUM_ROWS;
}

int ClevoKeyboardController::GetNumCols()
{
    return(type == CLEVO_KB_ITE829X) ? CLEVO_829X_NUM_COLS : CLEVO_8291_NUM_COLS;
}

int ClevoKeyboardController::GetNumLEDs()
{
    return GetNumRows() * GetNumCols();
}

unsigned char ClevoKeyboardController::GetBrightnessMax()
{
    return(type == CLEVO_KB_ITE829X) ? CLEVO_829X_BRIGHTNESS_MAX : CLEVO_8291_BRIGHTNESS_MAX;
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

    return(StringUtils::wstring_to_string(serial_string));
}

std::string ClevoKeyboardController::GetFirmwareVersion()
{
    char version_string[16];
    snprintf(version_string, sizeof(version_string), "%d.%02d", version >> 8, version & 0xFF);
    return(version_string);
}

/*---------------------------------------------------------*\
| ITE 8291: 8-byte feature report, no report ID             |
\*---------------------------------------------------------*/
void ClevoKeyboardController::WriteControl8291(unsigned char* data)
{
    hid_send_feature_report(dev, data, CLEVO_8291_REPORT_SIZE);
}

void ClevoKeyboardController::WriteRowData(unsigned char* data)
{
    hid_write(dev, data, CLEVO_8291_ROW_DATA_SIZE);
}

/*---------------------------------------------------------*\
| ITE 829x: 6-byte feature report with report ID 0xCC       |
\*---------------------------------------------------------*/
void ClevoKeyboardController::SendCommand829x(unsigned char cmd, unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3)
{
    unsigned char buf[CLEVO_829X_REPORT_SIZE];

    buf[0] = CLEVO_829X_REPORT_ID;
    buf[1] = cmd;
    buf[2] = d0;
    buf[3] = d1;
    buf[4] = d2;
    buf[5] = d3;

    hid_send_feature_report(dev, buf, CLEVO_829X_REPORT_SIZE);
}

void ClevoKeyboardController::TurnOff()
{
    if(type == CLEVO_KB_ITE829X)
    {
        SendCommand829x(CLEVO_829X_CMD_BRIGHTNESS);
        return;
    }

    unsigned char buf[CLEVO_8291_REPORT_SIZE];
    memset(buf, 0x00, CLEVO_8291_REPORT_SIZE);
    buf[0] = CLEVO_8291_CMD_MODE;
    buf[1] = CLEVO_8291_POWER_OFF;
    WriteControl8291(buf);
}

void ClevoKeyboardController::SetMode(unsigned char mode, unsigned char brightness, unsigned char speed, unsigned char behaviour)
{
    if(type == CLEVO_KB_ITE829X)
    {
        SendCommand829x(CLEVO_829X_CMD_BRIGHTNESS, brightness, speed);
        return;
    }

    unsigned char buf[CLEVO_8291_REPORT_SIZE];
    memset(buf, 0x00, CLEVO_8291_REPORT_SIZE);
    buf[0] = CLEVO_8291_CMD_MODE;
    buf[1] = CLEVO_8291_POWER_ON;
    buf[2] = mode;
    buf[3] = speed;
    buf[4] = brightness;
    buf[5] = CLEVO_8291_MODE_FLAG;
    buf[6] = behaviour;
    WriteControl8291(buf);
}

void ClevoKeyboardController::SetModeColor(unsigned char color_idx, RGBColor color)
{
    if(type == CLEVO_KB_ITE829X)
    {
        return;
    }

    unsigned char buf[CLEVO_8291_REPORT_SIZE];
    memset(buf, 0x00, CLEVO_8291_REPORT_SIZE);
    buf[0] = CLEVO_8291_CMD_COLOR;
    buf[2] = color_idx;
    buf[3] = RGBGetRValue(color);
    buf[4] = RGBGetGValue(color);
    buf[5] = RGBGetBValue(color);
    WriteControl8291(buf);
}

void ClevoKeyboardController::SetBrightnessSpeed829x(unsigned char brightness, unsigned char speed)
{
    if(brightness > CLEVO_829X_BRIGHTNESS_MAX)
    {
        brightness = CLEVO_829X_BRIGHTNESS_MAX;
    }

    if(brightness != current_brightness_829x || speed != current_speed_829x)
    {
        current_brightness_829x = brightness;
        current_speed_829x      = speed;
        SendCommand829x(CLEVO_829X_CMD_BRIGHTNESS, brightness, speed);
    }
}

void ClevoKeyboardController::SendColorSlot829x(unsigned char cmd, unsigned char slot, RGBColor color)
{
    SendCommand829x(cmd, slot, RGBGetRValue(color), RGBGetGValue(color), RGBGetBValue(color));
}

/*---------------------------------------------------------*\
| ITE 829x: Save/Clear BIOS keyboard state                  |
\*---------------------------------------------------------*/
void ClevoKeyboardController::SaveToBios829x()
{
    SendCommand829x(CLEVO_829X_CMD_SAVE_BIOS, 0x01);
}

void ClevoKeyboardController::ClearBiosSave829x()
{
    SendCommand829x(CLEVO_829X_CMD_SAVE_BIOS, 0x00);
}

void ClevoKeyboardController::StopAnimation829x()
{
    SendCommand829x(CLEVO_829X_CMD_ANIM, CLEVO_829X_CMD_STOP_ANIM);
}

void ClevoKeyboardController::SetEffect829x(unsigned char mode_value, const std::vector<RGBColor>& colors, unsigned int direction, unsigned int color_mode)
{
    bool mode_changed = (mode_value != current_mode_829x);
    current_mode_829x = mode_value;
    bool use_custom_color = (color_mode != MODE_COLORS_RANDOM) && !colors.empty();

    /*---------------------------------------------------------*\
    | Animation modes: send [CC, 00, mode_id] on mode change    |
    \*---------------------------------------------------------*/
    struct anim_entry { unsigned char mode; unsigned char anim_id; };
    static const anim_entry anim_modes[] =
    {
        { CLEVO_829X_MODE_WAVE,          0x04 },
        { CLEVO_829X_MODE_RANDOM,        0x09 },
        { CLEVO_829X_MODE_SCAN,          0x0A },
        { CLEVO_829X_MODE_SNAKE,         0x0B },
        { CLEVO_829X_MODE_RAINBOW_CYCLE, 0x02 },
        { CLEVO_829X_MODE_RAINBOW_GRAD,  0x04 },
    };

    const anim_entry* anim_it = std::find_if(std::begin(anim_modes), std::end(anim_modes),
        [mode_value](const anim_entry& e) { return e.mode == mode_value; });

    if(anim_it != std::end(anim_modes) && mode_changed)
    {
        SendCommand829x(CLEVO_829X_CMD_ANIM, anim_it->anim_id);
    }

    /*---------------------------------------------------------*\
    | Direction-based effects (Wave/Snake)                      |
    | Slot = base + direction_index                             |
    | Custom color: base = 0xA1, Preset: base = 0x71            |
    \*---------------------------------------------------------*/
    /*---------------------------------------------------------*\
    | Map OpenRGB direction enum to firmware slot index         |
    \*---------------------------------------------------------*/
    static const unsigned char dir_to_wave_idx[] =
    {
        6,  /* MODE_DIRECTION_LEFT       */
        7,  /* MODE_DIRECTION_RIGHT      */
        4,  /* MODE_DIRECTION_UP         */
        5,  /* MODE_DIRECTION_DOWN       */
        6,  /* MODE_DIRECTION_HORIZONTAL */
        4,  /* MODE_DIRECTION_VERTICAL   */
        0,  /* MODE_DIRECTION_UP_LEFT    */
        1,  /* MODE_DIRECTION_UP_RIGHT   */
        2,  /* MODE_DIRECTION_DOWN_LEFT  */
        3,  /* MODE_DIRECTION_DOWN_RIGHT */
    };

    static const unsigned char dir_to_snake_idx[] =
    {
        0,  /* MODE_DIRECTION_LEFT       */
        0,  /* MODE_DIRECTION_RIGHT      */
        0,  /* MODE_DIRECTION_UP         */
        0,  /* MODE_DIRECTION_DOWN       */
        0,  /* MODE_DIRECTION_HORIZONTAL */
        0,  /* MODE_DIRECTION_VERTICAL   */
        0,  /* MODE_DIRECTION_UP_LEFT    */
        1,  /* MODE_DIRECTION_UP_RIGHT   */
        2,  /* MODE_DIRECTION_DOWN_LEFT  */
        3,  /* MODE_DIRECTION_DOWN_RIGHT */
    };

    if((mode_value == CLEVO_829X_MODE_WAVE || mode_value == CLEVO_829X_MODE_RAINBOW_GRAD) && direction < sizeof(dir_to_wave_idx))
    {
        unsigned char base = use_custom_color ? CLEVO_829X_COLOR_SLOT_BASE : CLEVO_829X_PRESET_SLOT_BASE;
        unsigned char idx  = dir_to_wave_idx[direction];
        unsigned char slot = base + idx;

        if(use_custom_color)
        {
            SendColorSlot829x(CLEVO_829X_CMD_WAVE_COLOR, slot, colors[0]);
        }

        if(!use_custom_color)
        {
            SendCommand829x(CLEVO_829X_CMD_WAVE_COLOR, slot);
        }
    }

    if(mode_value == CLEVO_829X_MODE_SNAKE && direction < sizeof(dir_to_snake_idx))
    {
        unsigned char base = use_custom_color ? CLEVO_829X_COLOR_SLOT_BASE : CLEVO_829X_PRESET_SLOT_BASE;
        unsigned char idx  = dir_to_snake_idx[direction];
        unsigned char slot = base + idx;

        if(use_custom_color)
        {
            SendColorSlot829x(CLEVO_829X_CMD_SNAKE_COLOR, slot, colors[0]);
        }

        if(!use_custom_color)
        {
            SendCommand829x(CLEVO_829X_CMD_SNAKE_COLOR, slot);
        }
    }

    /*---------------------------------------------------------*\
    | Breathing/Flashing: random or custom color                |
    \*---------------------------------------------------------*/
    if(mode_value == CLEVO_829X_MODE_BREATH && use_custom_color)
    {
        SendColorSlot829x(0x0A, CLEVO_829X_COLOR_CUSTOM, colors[0]);
    }

    if(mode_value == CLEVO_829X_MODE_BREATH && !use_custom_color)
    {
        SendCommand829x(0x0A);
    }

    if(mode_value == CLEVO_829X_MODE_BLINK && use_custom_color)
    {
        SendColorSlot829x(0x0B, CLEVO_829X_COLOR_CUSTOM, colors[0]);
    }

    if(mode_value == CLEVO_829X_MODE_BLINK && !use_custom_color)
    {
        SendCommand829x(0x0B);
    }

    /*---------------------------------------------------------*\
    | Random: custom color only (multicolor is the default)     |
    \*---------------------------------------------------------*/
    if(mode_value == CLEVO_829X_MODE_RANDOM && use_custom_color)
    {
        SendColorSlot829x(CLEVO_829X_CMD_RANDOM_COLOR, CLEVO_829X_COLOR_SLOT_BASE, colors[0]);
    }

    /*---------------------------------------------------------*\
    | Scan: 2 custom color bands                                |
    \*---------------------------------------------------------*/
    if(mode_value == CLEVO_829X_MODE_SCAN)
    {
        for(size_t i = 0; i < colors.size() && i < 2; i++)
        {
            SendColorSlot829x(CLEVO_829X_CMD_SCAN_COLOR, CLEVO_829X_COLOR_SLOT_BASE + i, colors[i]);
        }
    }

    /*---------------------------------------------------------*\
    | Fn Highlight: per-key color on function keys              |
    \*---------------------------------------------------------*/
    if(mode_value == CLEVO_829X_MODE_FN_COLOR && !colors.empty())
    {
        static const unsigned char fn_keys[] =
        {
            0x01, 0x02, 0x03, 0x05, 0x06, 0x07, 0x08, 0x09,
            0x0A, 0x0B, 0x0C, 0x21, 0x23, 0x31, 0x32, 0x33,
            0x53, 0x73, 0xA2
        };

        SendCommand829x(CLEVO_829X_CMD_ANIM, CLEVO_829X_CMD_CLEAR);

        for(unsigned char key : fn_keys)
        {
            SendCommand829x(CLEVO_829X_CMD_SET_LED, key,
                RGBGetRValue(colors[0]),
                RGBGetGValue(colors[0]),
                RGBGetBValue(colors[0]));
        }
    }
}

void ClevoKeyboardController::SendColors(const unsigned char* color_data, unsigned char brightness, unsigned char speed)
{
    int num_rows = GetNumRows();
    int num_cols = GetNumCols();

    if(type == CLEVO_KB_ITE829X)
    {
        if(brightness > CLEVO_829X_BRIGHTNESS_MAX)
        {
            brightness = CLEVO_829X_BRIGHTNESS_MAX;
        }

        SendCommand829x(CLEVO_829X_CMD_BRIGHTNESS, brightness, speed);

        for(int row = 0; row < num_rows; row++)
        {
            for(int col = 0; col < num_cols; col++)
            {
                int idx = (row * num_cols + col) * 3;

                SendCommand829x(CLEVO_829X_CMD_SET_LED,
                    CLEVO_829X_LED_ID(row, col),
                    color_data[idx + 0],
                    color_data[idx + 1],
                    color_data[idx + 2]);
            }
        }

        return;
    }

    /*---------------------------------------------------------*\
    | ITE 8291: row-based color transfer                        |
    \*---------------------------------------------------------*/
    unsigned char ctrl_buf[CLEVO_8291_REPORT_SIZE];
    unsigned char row_buf[CLEVO_8291_ROW_DATA_SIZE];

    if(brightness > CLEVO_8291_BRIGHTNESS_MAX)
    {
        brightness = CLEVO_8291_BRIGHTNESS_MAX;
    }

    memset(ctrl_buf, 0x00, CLEVO_8291_REPORT_SIZE);
    ctrl_buf[0] = CLEVO_8291_CMD_MODE;
    ctrl_buf[1] = CLEVO_8291_POWER_ON;
    ctrl_buf[2] = CLEVO_KEYBOARD_MODE_DIRECT;
    ctrl_buf[4] = brightness;
    WriteControl8291(ctrl_buf);

    for(int row = 0; row < num_rows; row++)
    {
        memset(ctrl_buf, 0x00, CLEVO_8291_REPORT_SIZE);
        ctrl_buf[0] = CLEVO_8291_CMD_ROW;
        ctrl_buf[2] = row;
        WriteControl8291(ctrl_buf);

        memset(row_buf, 0x00, CLEVO_8291_ROW_DATA_SIZE);

        for(int col = 0; col < num_cols; col++)
        {
            int idx = (row * num_cols + col) * 3;

            row_buf[2 + col]                = color_data[idx + 2];
            row_buf[2 + num_cols + col]     = color_data[idx + 1];
            row_buf[2 + num_cols * 2 + col] = color_data[idx + 0];
        }

        WriteRowData(row_buf);
    }
}
