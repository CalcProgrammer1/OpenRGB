/*---------------------------------------------------------*\
| EVisionKeyboardController.h                               |
|                                                           |
|   Driver for EVision keyboard (Redragon, Glorious, Ajazz, |
|   Tecware, and many other brands)                         |
|                                                           |
|   Adam Honse (CalcProgrammer1)                15 Mar 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define EVISION_KB_MAX_PACKET_SIZE                  ( 0x36 )/* max packet size for color*/
                                                            /* update packets           */
enum
{
    EVISION_KB_COMMAND_BEGIN                        = 0x01, /* Begin packet command     */
    EVISION_KB_COMMAND_END                          = 0x02, /* End packet command       */
    EVISION_KB_COMMAND_SET_PARAMETER                = 0x06, /* Set parameter command    */
    EVISION_KB_COMMAND_READ_CUSTOM_COLOR_DATA       = 0x10, /* Read custom color data   */
    EVISION_KB_COMMAND_WRITE_CUSTOM_COLOR_DATA      = 0x11, /* Write custom color data  */
};

enum
{
    EVISION_KB_PARAMETER_MODE                       = 0x00, /* Mode parameter           */
    EVISION_KB_PARAMETER_BRIGHTNESS                 = 0x01, /* Brightness parameter     */
    EVISION_KB_PARAMETER_SPEED                      = 0x02, /* Speed parameter          */
    EVISION_KB_PARAMETER_DIRECTION                  = 0x03, /* Direction parameter      */
    EVISION_KB_PARAMETER_RANDOM_COLOR_FLAG          = 0x04, /* Random color parameter   */
    EVISION_KB_PARAMETER_MODE_COLOR                 = 0x05, /* Mode color (RGB)         */
    EVISION_KB_PARAMETER_POLLING_RATE               = 0x0F, /* Polling rate             */
    EVISION_KB_PARAMETER_SURMOUNT_MODE_COLOR        = 0x11, /* Surmount mode color      */
};

enum
{
    EVISION_KB_MODE_COLOR_WAVE_SHORT                = 0x01, /* "Go with the stream"     */
    EVISION_KB_MODE_COLOR_WAVE_LONG                 = 0x02, /* "Clouds fly"             */
    EVISION_KB_MODE_COLOR_WHEEL                     = 0x03, /* "Winding paths"          */
    EVISION_KB_MODE_SPECTRUM_CYCLE                  = 0x04, /* "The trial of light"     */
    EVISION_KB_MODE_BREATHING                       = 0x05, /* "Breathing"              */
    EVISION_KB_MODE_STATIC                          = 0x06, /* "Normally on"            */
    EVISION_KB_MODE_REACTIVE                        = 0x07, /* "Pass without trace"     */
    EVISION_KB_MODE_REACTIVE_RIPPLE                 = 0x08, /* "Ripple graff"           */
    EVISION_KB_MODE_REACTIVE_LINE                   = 0x09, /* "Fast run without trace" */
    EVISION_KB_MODE_STARLIGHT_FAST                  = 0x0A, /* "Swift action"           */
    EVISION_KB_MODE_BLOOMING                        = 0x0B, /* "Flowers blooming"       */
    EVISION_KB_MODE_RAINBOW_WAVE_VERTICAL           = 0x0C, /* "Snow winter jasmine"    */
    EVISION_KB_MODE_HURRICANE                       = 0x0D, /* "Hurricane"              */
    EVISION_KB_MODE_ACCUMULATE                      = 0x0E, /* "Accumulate"             */
    EVISION_KB_MODE_STARLIGHT_SLOW                  = 0x0F, /* "Digital times"          */
    EVISION_KB_MODE_VISOR                           = 0x10, /* "Both ways"              */
    EVISION_KB_MODE_SURMOUNT                        = 0x11, /* "Surmount"               */
    EVISION_KB_MODE_RAINBOW_WAVE_CIRCLE             = 0x12, /* "Fast and the Furious"   */
    EVISION_KB_MODE_CUSTOM                          = 0x14, /* "Coastal"                */
};

enum
{
    EVISION_KB_BRIGHTNESS_LOWEST                    = 0x00, /* Lowest brightness (off)  */
    EVISION_KB_BRIGHTNESS_HIGHEST                   = 0x04, /* Highest brightness       */
};

enum
{
    EVISION_KB_SPEED_SLOWEST                        = 0x05, /* Slowest speed setting    */
    EVISION_KB_SPEED_NORMAL                         = 0x03, /* Normal speed setting     */
    EVISION_KB_SPEED_FASTEST                        = 0x00, /* Fastest speed setting    */
};

enum
{
    EVISION_KB_SURMOUNT_MODE_COLOR_RED              = 0x01, /* Red surmount color       */
    EVISION_KB_SURMOUNT_MODE_COLOR_YELLOW           = 0x02, /* Yellow surmount color    */
    EVISION_KB_SURMOUNT_MODE_COLOR_GREEN            = 0x03, /* Green surmount color     */
    EVISION_KB_SURMOUNT_MODE_COLOR_BLUE             = 0x04, /* Blue surmount color      */
};

enum
{
    EVISION_KB_POLLING_RATE_125HZ                   = 0x00, /* 125Hz polling rate       */
    EVISION_KB_POLLING_RATE_250HZ                   = 0x01, /* 250Hz polling rate       */
    EVISION_KB_POLLING_RATE_500HZ                   = 0x02, /* 500Hz polling rate       */
    EVISION_KB_POLLING_RATE_1000HZ                  = 0x03, /* 1000Hz polling rate      */
};

class EVisionKeyboardController
{
public:
    EVisionKeyboardController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~EVisionKeyboardController();

    std::string GetDeviceLocation();
    std::string GetNameString();
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
                    char                usb_buf[64]
                    );

    void        SendKeyboardParameter
                    (
                    unsigned char       parameter,
                    unsigned char       parameter_size,
                    unsigned char*      parameter_data
                    );
};
