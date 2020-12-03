/*-----------------------------------------*\
|  ATC800Controller.h                       |
|                                           |
|  Definitions and types for ATC800 CPU     |
|  Cooler                                   |
|                                           |
|  Felipe Cavalcanti     08/13/2020         |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    AORUS_ATC800_MODE_OFF               = 0x00,
    AORUS_ATC800_MODE_STATIC            = 0x01,
    AORUS_ATC800_MODE_PULSE             = 0x02,
    AORUS_ATC800_MODE_FLASHING          = 0x04,
    AORUS_ATC800_MODE_DOUBLE_FLASH      = 0x05,
};

enum
{
    AORUS_ATC800_SPEED_SLOWEST          = 0x00, /* Slowest speed                */
    AORUS_ATC800_SPEED_SLOW             = 0x01, /* Slow speed                   */
    AORUS_ATC800_SPEED_NORMAL           = 0x02, /* Normal speed                 */
    AORUS_ATC800_SPEED_FAST             = 0x03, /* Fast speed                   */
    AORUS_ATC800_SPEED_FASTEST          = 0x04, /* Fastest speed                */
};

enum
{
    AORUS_ATC800_FANS_ZONE              = 0,
    AORUS_ATC800_TOP_ZONE               = 1
};

class ATC800Controller
{
public:
    ATC800Controller(hid_device* dev_handle, const char* path);
    ~ATC800Controller();

    std::string GetDeviceLocation();
    std::string GetSerialString();

    void        SendCoolerMode
                    (
                    unsigned char       mode,
                    unsigned short      speed,
                    unsigned char       channel,
                    unsigned char       red,
                    unsigned char       green,
                    unsigned char       blue
                    );

private:
    hid_device*             dev;
    std::string             location;
};
