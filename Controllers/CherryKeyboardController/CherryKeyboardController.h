/*---------------------------------------------------------*\
| CherryKeyboardController.h                                |
|                                                           |
|   Driver for Cherry keyboard                              |
|                                                           |
|   Sebastian Kraus                             25 Dec 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define CHERRY_KB_PACKET_SIZE                        64
#define CHERRY_KB_MAX_PACKET_SIZE                  ( 0x36 )/* max packet size for color*/
                                                            /* update packets           */

/*-----------------------------------------------------*\
| Cherry keyboard product IDs                           |
\*-----------------------------------------------------*/
#define MX_BOARD_3_0S_FL_NBL_PID        0x0077
#define MX_BOARD_3_0S_FL_RGB_PID        0x0079
#define MX_BOARD_3_0S_FL_RGB_KOR_PID    0x0083
#define MX_1_0_FL_BL_PID                0x00AB
#define MX_BOARD_1_0_TKL_RGB_PID        0x00AC
#define MX_BOARD_8_0_TKL_RGB_PID        0x00B7
#define MX_BOARD_10_0_FL_RGB_PID        0x00BB
#define G80_3000_TKL_NBL_PID            0x00C3
#define MX_BOARD_2_0S_FL_RGB_EU_PID     0x01A6
#define MX_BOARD_2_0S_FL_RGB_US_PID     0x00C4
#define MX_BOARD_2_0S_FL_NBL_PID        0x00CE
#define G80_3000_TKL_RGB_PID            0x00C5
#define MV_BOARD_3_0FL_RGB_PID          0x00C7
#define CCF_MX_8_0_TKL_BL_PID           0x00C9
#define CCF_MX_1_0_TKL_BL_PID           0x00CA
#define CCF_MX_1_0_TKL_NBL_PID          0x00CB
#define G80_3000_TKL_NBL_KOR_PID        0x00CD
#define MX_1_0_FL_NBL_PID               0x00D2
#define MX_1_0_FL_RGB_PID               0x00D3
#define G80_3000N_TKL_RGB_EU_PID        0x00DD
#define G80_3000N_TKL_RGB_US_PID        0x00E0
#define G80_3000N_FL_RGB_EU_PID         0x00DE
#define G80_3000N_FL_RGB_US_PID         0x00E1
#define MX_BOARD_10_0N_FL_RGB_EU_PID    0x00DF
#define MX_BOARD_10_0N_FL_RGB_US_PID    0x00E2


enum
{
    CHERRY_KB_COMMAND_BEGIN                        = 0x01, /* Begin packet command     */
    CHERRY_KB_COMMAND_END                          = 0x02, /* End packet command       */
    CHERRY_KB_COMMAND_SET_PARAMETER                = 0x06, /* Set parameter command    */
    CHERRY_KB_COMMAND_READ_CUSTOM_COLOR_DATA       = 0x1B, /* Read custom color data   */
    CHERRY_KB_COMMAND_WRITE_CUSTOM_COLOR_DATA      = 0x0B, /* Write custom color data  */
};

enum
{
    CHERRY_KB_PARAMETER_MODE                       = 0x00, /* Mode parameter           */
    CHERRY_KB_PARAMETER_BRIGHTNESS                 = 0x01, /* Brightness parameter     */
    CHERRY_KB_PARAMETER_SPEED                      = 0x02, /* Speed parameter          */
    CHERRY_KB_PARAMETER_DIRECTION                  = 0x03, /* Direction parameter      */
    CHERRY_KB_PARAMETER_RANDOM_COLOR_FLAG          = 0x04, /* Random color parameter   */
    CHERRY_KB_PARAMETER_MODE_COLOR                 = 0x05, /* Mode color (RGB)         */
    CHERRY_KB_PARAMETER_POLLING_RATE               = 0x0F, /* Polling rate             */
    CHERRY_KB_PARAMETER_SURMOUNT_MODE_COLOR        = 0x11, /* Surmount mode color      */
};

enum
{
    CHERRY_KB_MODE_WAVE                            = 0x00,
    CHERRY_KB_MODE_SPECTRUM                        = 0x01,
    CHERRY_KB_MODE_BREATHING                       = 0x02,
    CHERRY_KB_MODE_STATIC                          = 0x03,
    CHERRY_KB_MODE_RADAR                           = 0x04,
    CHERRY_KB_MODE_VORTEX                          = 0x05,
    CHERRY_KB_MODE_FIRE                            = 0x06,
    CHERRY_KB_MODE_STARS                           = 0x07,
    CHERRY_KB_MODE_CUSTOM                          = 0x08,
    CHERRY_KB_MODE_ROLLING                         = 0x0A,
    CHERRY_KB_MODE_RAIN                            = 0x0B,
    CHERRY_KB_MODE_CURVE                           = 0x0C,
    CHERRY_KB_MODE_WAVE_MID                        = 0x0E,
    CHERRY_KB_MODE_SCAN                            = 0x0F,
    CHERRY_KB_MODE_RADIATION                       = 0x12,
    CHERRY_KB_MODE_RIPPLES                         = 0x13,
    CHERRY_KB_MODE_SINGLE_KEY                      = 0x15,

};

enum
{
    CHERRY_KB_BRIGHTNESS_LOWEST                    = 0x00, /* Lowest brightness (off)  */
    CHERRY_KB_BRIGHTNESS_HIGHEST                   = 0x04, /* Highest brightness       */
};

enum
{
    CHERRY_KB_SPEED_SLOWEST                        = 0x04, /* Slowest speed setting    */
    CHERRY_KB_SPEED_NORMAL                         = 0x02, /* Normal speed setting     */
    CHERRY_KB_SPEED_FASTEST                        = 0x00, /* Fastest speed setting    */
};

enum
{
    CHERRY_KB_SURMOUNT_MODE_COLOR_RED              = 0x01, /* Red surmount color       */
    CHERRY_KB_SURMOUNT_MODE_COLOR_YELLOW           = 0x02, /* Yellow surmount color    */
    CHERRY_KB_SURMOUNT_MODE_COLOR_GREEN            = 0x03, /* Green surmount color     */
    CHERRY_KB_SURMOUNT_MODE_COLOR_BLUE             = 0x04, /* Blue surmount color      */
};

enum
{
    CHERRY_KB_POLLING_RATE_125HZ                   = 0x00, /* 125Hz polling rate       */
    CHERRY_KB_POLLING_RATE_250HZ                   = 0x01, /* 250Hz polling rate       */
    CHERRY_KB_POLLING_RATE_500HZ                   = 0x02, /* 500Hz polling rate       */
    CHERRY_KB_POLLING_RATE_1000HZ                  = 0x03, /* 1000Hz polling rate      */
};

class CherryKeyboardController
{
public:
    CherryKeyboardController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~CherryKeyboardController();

    std::string GetDeviceLocation();
    std::string GetDeviceName();
    std::string GetSerialString();

    void        SetKeyboardColors
                    (
                    unsigned char *     color_data,
                    unsigned int        size
                    );

    void        SendKeyboardBegin();

    void        SendKeyboardMode
                    (
                    unsigned char       mode
                    );

    void        SendKeyboardModeEx
                    (
                    unsigned char       mode,
                    unsigned char       brightness,
                    unsigned char       speed,
                    unsigned char       direction,
                    unsigned char       random_flag,
                    unsigned char       red,
                    unsigned char       green,
                    unsigned char       blue
                    );

    void        SendKeyboardData
                    (
                    unsigned char *     data,
                    unsigned char       data_size,
                    unsigned short      data_offset
                    );

    void        SendKeyboardEnd();

private:
    hid_device*             dev;
    std::string             location;
    std::string             name;

    void        ComputeChecksum
                    (
                    char                usb_buf[CHERRY_KB_PACKET_SIZE]
                    );

    void        SendKeyboardParameter
                    (
                    unsigned char       parameter,
                    unsigned char       parameter_size,
                    unsigned char*      parameter_data
                    );
};
