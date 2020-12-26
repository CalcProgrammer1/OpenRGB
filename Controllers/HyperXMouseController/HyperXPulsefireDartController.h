/*-------------------------------------------*\
|  HyperXPulsefireDartController.h            |
|                                             |
|  Definitions and types for HyperX           |
|  Pulsefire Dart lighting controller         |
|                                             |
|  Santeri Pikarinen (santeri3700) 12/26/2020 |
\*-------------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    HYPERX_PULSEFIRE_DART_PACKET_ID_DIRECT = 0xd2, /* Direct control packet */

    HYPERX_PULSEFIRE_DART_MODE_STATIC      = 0x00, /* Static color mode */
    HYPERX_PULSEFIRE_DART_MODE_CYCLE       = 0x12, /* Spectrum cycle mode */
    HYPERX_PULSEFIRE_DART_MODE_BREATHING   = 0x20, /* Single color breathing mode */
    HYPERX_PULSEFIRE_DART_MODE_TRIGGER     = 0x30, /* Trigger fade mode */

    HYPERX_PULSEFIRE_DART_SPEED_MIN        = 0x64,
    HYPERX_PULSEFIRE_DART_SPEED_MAX        = 0x00,
    HYPERX_PULSEFIRE_DART_SPEED_MED        = 0x32,
    HYPERX_PULSEFIRE_DART_SPEED_NONE       = 0x00, /* For static color mode */

    HYPERX_PULSEFIRE_DART_LED_SCROLL       = 0x00,
    HYPERX_PULSEFIRE_DART_LED_LOGO         = 0x10,
    HYPERX_PULSEFIRE_DART_LED_ALL          = 0x20
};

class HyperXPulsefireDartController
{
public:
    HyperXPulsefireDartController(hid_device* dev_handle, const char* path);
    ~HyperXPulsefireDartController();

    std::string GetDeviceLocation();
    std::string GetSerialString();

    void SendDirect
            (
            RGBColor        color_data,
            int             led,
            int             mode,
            int             brightness,
            int             speed
            );

private:
    hid_device*             dev;
    std::string             location;
};
