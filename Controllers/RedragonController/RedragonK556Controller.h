/*-----------------------------------------*\
|  RedragonK556Controller.h                 |
|                                           |
|  Definitions and types for Redragon K556  |
|  Devarajas keyboard lighting controller   |
|                                           |
|  Adam Honse (CalcProgrammer1) 3/15/2020   |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

#define REDRAGON_K556_MAX_PACKET_SIZE   ( 0x36 )        /* max packet size for color*/
                                                        /* update packets           */
enum
{
    REDRAGON_K556_COMMAND_BEGIN                     = 0x01, /* Begin packet command     */
    REDRAGON_K556_COMMAND_END                       = 0x02, /* End packet command       */
    REDRAGON_K556_COMMAND_SET_PARAMETER             = 0x06, /* Set parameter command    */
    REDRAGON_K556_COMMAND_READ_CUSTOM_COLOR_DATA    = 0x10, /* Read custom color data   */
    REDRAGON_K556_COMMAND_WRITE_CUSTOM_COLOR_DATA   = 0x11, /* Write custom color data  */
};

enum
{
    REDRAGON_K556_PARAMETER_MODE                    = 0x00, /* Mode parameter           */
    REDRAGON_K556_PARAMETER_BRIGHTNESS              = 0x01, /* Brightness parameter     */
    REDRAGON_K556_PARAMETER_SPEED                   = 0x02, /* Speed parameter          */
    REDRAGON_K556_PARAMETER_DIRECTION               = 0x03, /* Direction parameter      */
    REDRAGON_K556_PARAMETER_RANDOM_COLOR_FLAG       = 0x04, /* Random color parameter   */
    REDRAGON_K556_PARAMETER_MODE_COLOR              = 0x05, /* Mode color (RGB)         */
    REDRAGON_K556_PARAMETER_POLLING_RATE            = 0x0F, /* Polling rate             */
    REDRAGON_K556_PARAMETER_SURMOUNT_MODE_COLOR     = 0x11, /* Surmount mode color      */
};

enum
{
    REDRAGON_K556_MODE_COLOR_WAVE_SHORT             = 0x01, /* "Go with the stream"     */
    REDRAGON_K556_MODE_COLOR_WAVE_LONG              = 0x02, /* "Clouds fly"             */
    REDRAGON_K556_MODE_COLOR_WHEEL                  = 0x03, /* "Winding paths"          */
    REDRAGON_K556_MODE_SPECTRUM_CYCLE               = 0x04, /* "The trial of light"     */
    REDRAGON_K556_MODE_BREATHING                    = 0x05, /* "Breathing"              */
    REDRAGON_K556_MODE_STATIC                       = 0x06, /* "Normally on"            */
    REDRAGON_K556_MODE_REACTIVE                     = 0x07, /* "Pass without trace"     */
    REDRAGON_K556_MODE_REACTIVE_RIPPLE              = 0x08, /* "Ripple graff"           */
    REDRAGON_K556_MODE_REACTIVE_LINE                = 0x09, /* "Fast run without trace" */
    REDRAGON_K556_MODE_STARLIGHT_FAST               = 0x0A, /* "Swift action"           */
    REDRAGON_K556_MODE_BLOOMING                     = 0x0B, /* "Flowers blooming"       */
    REDRAGON_K556_MODE_RAINBOW_WAVE_VERTICAL        = 0x0C, /* "Snow winter jasmine"    */
    REDRAGON_K556_MODE_HURRICANE                    = 0x0D, /* "Hurricane"              */
    REDRAGON_K556_MODE_ACCUMULATE                   = 0x0E, /* "Accumulate"             */
    REDRAGON_K556_MODE_STARLIGHT_SLOW               = 0x0F, /* "Digital times"          */
    REDRAGON_K556_MODE_VISOR                        = 0x10, /* "Both ways"              */
    REDRAGON_K556_MODE_SURMOUNT                     = 0x11, /* "Surmount"               */
    REDRAGON_K556_MODE_RAINBOW_WAVE_CIRCLE          = 0x12, /* "Fast and the Furious"   */
    REDRAGON_K556_MODE_CUSTOM                       = 0x14, /* "Coastal"                */
};

enum
{
    REDRAGON_K556_BRIGHTNESS_LOWEST                 = 0x00, /* Lowest brightness (off)  */
    REDRAGON_K556_BRIGHTNESS_HIGHEST                = 0x04, /* Highest brightness       */
};

enum
{
    REDRAGON_K556_SPEED_SLOWEST                     = 0x05, /* Slowest speed setting    */
    REDRAGON_K556_SPEED_NORMAL                      = 0x03, /* Normal speed setting     */
    REDRAGON_K556_SPEED_FASTEST                     = 0x00, /* Fastest speed setting    */
};

enum
{
    REDRAGON_K556_SURMOUNT_MODE_COLOR_RED           = 0x01, /* Red surmount color       */
    REDRAGON_K556_SURMOUNT_MODE_COLOR_YELLOW        = 0x02, /* Yellow surmount color    */
    REDRAGON_K556_SURMOUNT_MODE_COLOR_GREEN         = 0x03, /* Green surmount color     */
    REDRAGON_K556_SURMOUNT_MODE_COLOR_BLUE          = 0x04, /* Blue surmount color      */
};

enum
{
    REDRAGON_K556_POLLING_RATE_125HZ                = 0x00, /* 125Hz polling rate       */
    REDRAGON_K556_POLLING_RATE_250HZ                = 0x01, /* 250Hz polling rate       */
    REDRAGON_K556_POLLING_RATE_500HZ                = 0x02, /* 500Hz polling rate       */
    REDRAGON_K556_POLLING_RATE_1000HZ               = 0x03, /* 1000Hz polling rate      */
};

class RedragonK556Controller
{
public:
    RedragonK556Controller(hid_device* dev_handle);
    ~RedragonK556Controller();

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
