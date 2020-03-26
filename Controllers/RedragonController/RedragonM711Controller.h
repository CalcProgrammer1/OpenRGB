/*-----------------------------------------*\
|  RedragonM711Controller.h                 |
|                                           |
|  Definitions and types for Redragon M711  |
|  Cobra mouse lighting controller          |
|                                           |
|  Adam Honse (CalcProgrammer1) 3/15/2020   |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    REDRAGON_M711_MODE_WAVE              = 0x00,
    REDRAGON_M711_MODE_RANDOM_BREATHING  = 0x01,
    REDRAGON_M711_MODE_STATIC            = 0x02,
    REDRAGON_M711_MODE_BREATHING         = 0x04,
    REDRAGON_M711_MODE_RAINBOW           = 0x08,
    REDRAGON_M711_MODE_FLASHING          = 0x10,
};

class RedragonM711Controller
{
public:
    RedragonM711Controller(hid_device* dev_handle);
    ~RedragonM711Controller();

    void        SendMouseApply();

    void        SendMouseMode
                   (
                   unsigned char       mode,
                   unsigned char       speed,
                   unsigned char       red,
                   unsigned char       green,
                   unsigned char       blue
                   );

private:
    hid_device*             dev;
};
