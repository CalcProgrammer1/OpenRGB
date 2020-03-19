/*-----------------------------------------*\
|  RedragonController.h                     |
|                                           |
|  Definitions and types for Redragon RGB   |
|  keyboard, mouse, and mousemat lighting   |
|  controller                               |
|                                           |
|  Adam Honse (CalcProgrammer1) 3/15/2020   |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

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

enum
{
    REDRAGON_M117_MODE_WAVE              = 0x00,
    REDRAGON_M117_MODE_RANDOM_BREATHING  = 0x01,
    REDRAGON_M117_MODE_STATIC            = 0x02,
    REDRAGON_M117_MODE_BREATHING         = 0x04,
    REDRAGON_M117_MODE_RAINBOW           = 0x08,
    REDRAGON_M117_MODE_FLASHING          = 0x10,
};

class RedragonController
{
public:
    RedragonController(hid_device* dev_handle);
    ~RedragonController();

private:
    hid_device*             dev;

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

    void        SendMouseApply();

    void        SendMouseMode
                   (
                   unsigned char       mode,
                   unsigned char       speed,
                   unsigned char       red,
                   unsigned char       green,
                   unsigned char       blue
                   );
};
