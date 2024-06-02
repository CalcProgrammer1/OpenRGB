/*---------------------------------------------------------*\
| MountainKeyboardController.h                              |
|                                                           |
|   Driver for Mountain keyboard                            |
|                                                           |
|   Wojciech Lazarski                              Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Mountain vendor ID                                    |
\*-----------------------------------------------------*/
#define MOUNTAIN_VID                        0x3282
/*-----------------------------------------------------*\
| Everest keyboard product IDs                          |
\*-----------------------------------------------------*/
#define MOUNTAIN_EVEREST_PID                0x0001


#define MOUNTAIN_KEYBOARD_MAX_TRANSFER_COLORS           126
#define MOUNTAIN_KEYBOARD_TRANSFER_BUFFER_SIZE          (3*MOUNTAIN_KEYBOARD_MAX_TRANSFER_COLORS)

#define MOUNTAIN_KEYBOARD_MAX_TRANSFER_EDGE_COLORS       46
#define MOUNTAIN_KEYBOARD_TRANSFER_EDGE_BUFFER_SIZE     (3*MOUNTAIN_KEYBOARD_MAX_TRANSFER_EDGE_COLORS)

#define MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE                65
#define MOUNTAIN_KEYBOARD_USB_BUFFER_HEADER_SIZE          8
#define MOUNTAIN_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE    \
        (MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE-MOUNTAIN_KEYBOARD_USB_BUFFER_HEADER_SIZE)

enum
{
    MOUNTAIN_KEYBOARD_SAVE_CMD              = 0x13,
    MOUNTAIN_KEYBOARD_SEND_CMD              = 0x14
};

enum
{
    MOUNTAIN_KEYBOARD_SEND_COLOR_DATA_CMD   = 0x2C,
    MOUNTAIN_KEYBOARD_SEND_COLOR_EDGE_CMD   = 0x2D,
    MOUNTAIN_KEYBOARD_SELECT_MODE_CMD       = 0x00,
    MOUNTAIN_KEYBOARD_CONFIRM_CMD           = 0xA0
};

enum
{
    MOUNTAIN_KEYBOARD_STATIC_MSG            = 0x00,
    MOUNTAIN_KEYBOARD_COLOR_WAVE_MSG        = 0x04,
    MOUNTAIN_KEYBOARD_TORNADO_MSG           = 0x07,
    MOUNTAIN_KEYBOARD_BREATHING_MSG         = 0x01,
    MOUNTAIN_KEYBOARD_REACTIVE_MSG          = 0x03,
    MOUNTAIN_KEYBOARD_MATRIX_MSG            = 0x09,
    MOUNTAIN_KEYBOARD_CUSTOM_MSG            = 0x0A,
    MOUNTAIN_KEYBOARD_OFF_MSG               = 0x0C
};

enum
{
    MOUNTAIN_KEYBOARD_IDX_STATIC            = 0x00,
    MOUNTAIN_KEYBOARD_IDX_COLOR_WAVE        = 0x01,
    MOUNTAIN_KEYBOARD_IDX_TORNADO           = 0x02,
    MOUNTAIN_KEYBOARD_IDX_BREATHING         = 0x03,
    MOUNTAIN_KEYBOARD_IDX_REACTIVE          = 0x04,
    MOUNTAIN_KEYBOARD_IDX_MATRIX            = 0x05,
    MOUNTAIN_KEYBOARD_IDX_CUSTOM            = 0x06,
    MOUNTAIN_KEYBOARD_IDX_OFF               = 0x08,
    MOUNTAIN_KEYBOARD_IDX_INVALID           = 0xFF,
};

enum
{
    MOUNTAIN_KEYBOARD_COLOR_MODE_RAINBOW    = 0x00,
    MOUNTAIN_KEYBOARD_COLOR_MODE_SINGLE     = 0x01,
    MOUNTAIN_KEYBOARD_COLOR_MODE_DUAL       = 0x02
};

enum
{
    MOUNTAIN_KEYBOARD_DIRECTION_UP          = 0x06,
    MOUNTAIN_KEYBOARD_DIRECTION_DOWN        = 0x02,
    MOUNTAIN_KEYBOARD_DIRECTION_LEFT        = 0x04,
    MOUNTAIN_KEYBOARD_DIRECTION_RIGHT       = 0x00,
    MOUNTAIN_KEYBOARD_DIRECTION_ANTICLK     = 0x0A,
    MOUNTAIN_KEYBOARD_DIRECTION_CLK         = 0x09,
};

#define  MOUNTAIN_KEYBOARD_SAVE_MAGIC1  0x55

typedef struct
{
    unsigned char color_mode;
    unsigned char brightness;
    unsigned char speed;
    unsigned char direction;

    union
    {
        struct
        {
            unsigned char r;
            unsigned char g;
            unsigned char b;
        } one_color;
        struct
        {
            unsigned char r1;
            unsigned char g1;
            unsigned char b1;
            unsigned char r2;
            unsigned char g2;
            unsigned char b2;
        } two_colors;
    } mode;
} color_setup;

class MountainKeyboardController
{
public:
    MountainKeyboardController(hid_device* dev_handle, const char* path);
    ~MountainKeyboardController();

    std::string     GetDeviceLocation();
    std::string     GetSerialString();

    void SendOffCmd();
    void SendColorStaticCmd(color_setup setup);
    void SendColorWaveCmd(color_setup setup);
    void SendColorTornadoCmd(color_setup setup);
    void SendColorBreathingCmd(color_setup setup);
    void SendColorMatrixCmd(color_setup setup);
    void SendColorReactiveCmd(color_setup setup);

    void SendDirectColorCmd(bool quick_mode, unsigned char brightness, unsigned char *color_data, unsigned int color_count);
    void SendDirectColorEdgeCmd(bool quick_mode, unsigned char brightness, unsigned char *color_data, unsigned int data_size);

    void SaveData(unsigned char mode_idx);
    void SelectMode(unsigned char mode_idx);

private:
    void SendColorStartPacketCmd(unsigned char brightness);
    void SendColorPacketCmd(unsigned char pkt_no,unsigned char brightness, unsigned char *data, unsigned int data_size);
    void SendColorEdgePacketCmd(unsigned char pkt_no, unsigned char *data, unsigned int data_size);
    void SendColorPacketFinishCmd();

    hid_device*             dev;
    std::string             location;
    unsigned short          usb_pid;
};
