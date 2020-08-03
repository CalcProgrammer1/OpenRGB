/*-----------------------------------------*\
|  HyperXPulsefireSurgeController.h         |
|                                           |
|  Definitions and types for HyperX         |
|  Pulsefire Surge lighting controller      |
|                                           |
|  Adam Honse (CalcProgrammer1) 7/25/2020   |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    HYPERX_PULSEFIRE_SURGE_MODE_SOLID           = 0x00,     /* Solid color mode     */
    HYPERX_PULSEFIRE_SURGE_MODE_CYCLE           = 0x01,     /* Spectrum cycle mode  */
    HYPERX_PULSEFIRE_SURGE_MODE_BREATHING       = 0x02,     /* Breathing mode       */
    HYPERX_PULSEFIRE_SURGE_MODE_WAVE            = 0x03,     /* Wave mode            */
    HYPERX_PULSEFIRE_SURGE_MODE_TRIGGER         = 0x04,     /* Trigger mode         */
};

class HyperXPulsefireSurgeController
{
public:
    HyperXPulsefireSurgeController(hid_device* dev_handle);
    ~HyperXPulsefireSurgeController();

    void SendData
            (
            unsigned char   mode,
            RGBColor*       color_data
            );

private:
    hid_device*             dev;

    void SendWakeup();
};
