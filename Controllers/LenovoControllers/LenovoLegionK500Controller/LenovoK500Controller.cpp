/*---------------------------------------------------------*\
| LenovoK500Controller.cpp                                  |
|                                                           |
|   Driver for Lenovo Legion K500 keyboard                  |
|                                                           |
|   MX1D                                        10 Aug 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include <vector>
#include "LenovoK500Controller.h"
#include "LogManager.h"

using namespace std;

typedef struct
{
    unsigned char byte_idx;
    unsigned char bit_idx;
} k500_key;

static const k500_key k500_matrix[104] =
{
    {0, 0}, // ESC
    {0, 1}, // F1
    {0, 2}, // F2
    {0, 3}, // F3
    {0, 4}, // F4
    {0, 5}, // F5
    {0, 6}, // F6
    {0, 7}, // F7
    {1, 0}, // F8
    {1, 1}, // F9
    {1, 2}, // F10
    {1, 3}, // F11
    {1, 4}, // F12
    {1, 5}, // PrtSc
    {1, 6}, // ScrollLock
    {1, 7}, // Pause
    {2, 0}, // Grave
    {2, 1}, // 1
    {2, 2}, // 2
    {2, 3}, // 3
    {2, 4}, // 4
    {2, 5}, // 5
    {2, 6}, // 6
    {2, 7}, // 7
    {3, 0}, // 8
    {3, 1}, // 9
    {3, 2}, // 0
    {3, 3}, // Minus
    {3, 4}, // Equal
    {3, 5}, // Backspace
    {3, 6}, // Insert
    {3, 7}, // Home
    {7, 6}, // PgUp
    {12, 0}, // NumLock
    {12, 5}, // NumSlash
    {13, 1}, // NumAsterisk
    {14, 0}, // NumMinus
    {4, 0}, // Tab
    {4, 1}, // Q
    {4, 2}, // W
    {4, 3}, // E
    {4, 4}, // R
    {4, 5}, // T
    {4, 6}, // Y
    {4, 7}, // U
    {5, 0}, // I
    {5, 1}, // O
    {5, 2}, // P
    {5, 3}, // LBracket
    {5, 4}, // RBracket
    {5, 5}, // Backslash
    {5, 6}, // Delete
    {5, 7}, // End
    {7, 7}, // PgDn
    {12, 1}, // Num7
    {12, 6}, // Num8
    {13, 2}, // Num9
    {14, 1}, // NumPlus
    {6, 0}, // CapsLock
    {6, 1}, // A
    {6, 2}, // S
    {6, 3}, // D
    {6, 4}, // F
    {6, 5}, // G
    {6, 6}, // H
    {6, 7}, // J
    {7, 0}, // K
    {7, 1}, // L
    {7, 2}, // Semicolon
    {7, 3}, // Quote
    {7, 4}, // Enter
    {12, 2}, // Num4
    {12, 7}, // Num5
    {13, 3}, // Num6
    {8, 0}, // LShift
    {8, 1}, // Z
    {8, 2}, // X
    {8, 3}, // C
    {8, 4}, // V
    {8, 5}, // B
    {8, 6}, // N
    {8, 7}, // M
    {9, 0}, // Comma
    {9, 1}, // Period
    {9, 2}, // Slash
    {9, 3}, // RShift
    {9, 6}, // Up
    {12, 3}, // Num1
    {13, 0}, // Num2
    {13, 4}, // Num3
    {14, 3}, // NumEnter
    {10, 0}, // LCtrl
    {10, 1}, // LWin
    {10, 2}, // LAlt
    {10, 5}, // Space
    {11, 0}, // RAlt
    {11, 1}, // RWin
    {11, 2}, // Menu
    {11, 3}, // RCtrl
    {11, 6}, // Left
    {11, 7}, // Down
    {9, 7}, // Right
    {12, 4}, // Num0
    {13, 5}, // NumDecimal
};

static const unsigned char default_palette[] =
{
    0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00,
    0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00,
    0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0xff,
    0x00, 0x00, 0x00, 0xff, 0xff, 0x00
};

LenovoK500Controller::LenovoK500Controller(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    device      = dev_handle;
    location    = info.path;
    name        = dev_name;
}

LenovoK500Controller::~LenovoK500Controller()
{
    hid_close(device);
}

std::string LenovoK500Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string LenovoK500Controller::GetDeviceName()
{
    return(name);
}

void LenovoK500Controller::SetMode(unsigned char mode_value, std::vector<RGBColor> colors, unsigned int brightness, unsigned int speed)
{
    LOG_DEBUG("[%s] Setting mode %02X, brightness %d, speed %d", name.c_str(), mode_value, brightness, speed);

    unsigned char feature_buf[9];
    memset(feature_buf, 0x00, sizeof(feature_buf));
    feature_buf[0] = 0x00; // Report ID
    feature_buf[1] = 0x14;
    feature_buf[2] = 0x00;
    feature_buf[3] = 0x5A;
    hid_send_feature_report(device, feature_buf, sizeof(feature_buf));

    unsigned char usb_buf[K500_DATA_SIZE + 1];
    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00] = 0x00; // HID Report ID (0 = no report ID)
    usb_buf[0x01] = 0x01; // Magic byte
    usb_buf[0x02] = mode_value;
    usb_buf[0x03] = static_cast<unsigned char>(brightness);
    usb_buf[0x04] = static_cast<unsigned char>(speed);

    memcpy(&usb_buf[0x11], default_palette, sizeof(default_palette));

    if(mode_value == K500_MODE_CUSTOMIZE)
    {
        memset(&usb_buf[0x31], 0x00, 16);
        unsigned int custom_color = 0;
        
        for(size_t i = 0; i < colors.size() && i < 104; i++)
        {
            if(colors[i] != 0)
            {
                custom_color = colors[i];
                break;
            }
        }
        
        for(size_t i = 0; i < colors.size() && i < 104; i++)
        {
            if(colors[i] != 0)
            {
                unsigned char byte_idx = k500_matrix[i].byte_idx;
                unsigned char bit_idx  = k500_matrix[i].bit_idx;
                usb_buf[0x31 + byte_idx] |= (1 << bit_idx);
            }
        }
        
        usb_buf[0x11] = static_cast<unsigned char>(RGBGetRValue(custom_color));
        usb_buf[0x12] = static_cast<unsigned char>(RGBGetGValue(custom_color));
        usb_buf[0x13] = static_cast<unsigned char>(RGBGetBValue(custom_color));
    }
    else
    {
        if(colors.size() > 0)
        {
            usb_buf[0x11] = static_cast<unsigned char>(RGBGetRValue(colors[0]));
            usb_buf[0x12] = static_cast<unsigned char>(RGBGetGValue(colors[0]));
            usb_buf[0x13] = static_cast<unsigned char>(RGBGetBValue(colors[0]));
        }
        
        if(colors.size() > 1)
        {
            usb_buf[0x15] = static_cast<unsigned char>(RGBGetRValue(colors[1]));
            usb_buf[0x16] = static_cast<unsigned char>(RGBGetGValue(colors[1]));
            usb_buf[0x17] = static_cast<unsigned char>(RGBGetBValue(colors[1]));
        }
        
        if(colors.size() > 2)
        {
            usb_buf[0x19] = static_cast<unsigned char>(RGBGetRValue(colors[2]));
            usb_buf[0x1A] = static_cast<unsigned char>(RGBGetGValue(colors[2]));
            usb_buf[0x1B] = static_cast<unsigned char>(RGBGetBValue(colors[2]));
        }
    }

    hid_write(device, usb_buf, sizeof(usb_buf));
}
