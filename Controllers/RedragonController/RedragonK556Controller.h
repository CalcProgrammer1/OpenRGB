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
    REDRAGON_K556_MODE_RAINBOW_WAVE_SHORT       = 0x01, /* "Go with the stream"     */
    REDRAGON_K556_MODE_RAINBOW_WAVE_LONG        = 0x02, /* "Clouds fly"             */
    REDRAGON_K556_MODE_COLOR_WHEEL              = 0x03, /* "Winding paths"          */
    REDRAGON_K556_MODE_SPECTRUM_CYCLE           = 0x04, /* "The trial of light"     */
    REDRAGON_K556_MODE_BREATHING                = 0x05, /* "Breathing"              */
    REDRAGON_K556_MODE_STATIC                   = 0x06, /* "Normally on"            */
    REDRAGON_K556_MODE_REACTIVE                 = 0x07, /* "Pass without trace"     */
    REDRAGON_K556_MODE_REACTIVE_RIPPLE          = 0x08, /* "Ripple graff"           */
    REDRAGON_K556_MODE_REACTIVE_LINE            = 0x09, /* "Fast run without trace" */
    REDRAGON_K556_MODE_STARLIGHT_FAST           = 0x0A, /* "Swift action"           */
    REDRAGON_K556_MODE_BLOOMING                 = 0x0B, /* "Flowers blooming"       */
    REDRAGON_K556_MODE_RAINBOW_WAVE_VERTICAL    = 0x0C, /* "Snow winter jasmine"    */
    REDRAGON_K556_MODE_HURRICANE                = 0x0D, /* "Hurricane"              */
    REDRAGON_K556_MODE_ACCUMULATE               = 0x0E, /* "Accumulate"             */
    REDRAGON_K556_MODE_STARLIGHT_SLOW           = 0x0F, /* "Digital times"          */
    REDRAGON_K556_MODE_VISOR                    = 0x10, /* "Both ways"              */
    REDRAGON_K556_MODE_RAINBOW_WAVE_CIRCLE      = 0x12, /* "Fast and the Furious"   */
    REDRAGON_K556_MODE_CUSTOM                   = 0x14, /* "Coastal"                */
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

    void        SendKeyboardData
                    (
                    unsigned char *     data,
                    unsigned char       data_size,
                    unsigned short      data_offset
                    );

    void        SendKeyboardEnd();

private:
    hid_device*             dev;
};
