#pragma once

#include <string>
#include "RGBController.h"
#include "RGBControllerKeyNames.h"

#define CORSAIR_ZONES_MAX   6

enum corsair_v2_device_mode
{
    CORSAIR_V2_MODE_HW              = 0x01,     /* Hardware RGB mode                */
    CORSAIR_V2_MODE_SW              = 0x02,     /* Software RGB mode                */
};

typedef struct
{
    std::string             name;
    zone_type               type;
    uint8_t                 rows;
    uint8_t                 cols;
}   corsair_v2_zone;

typedef struct
{
    uint8_t                 zone;
    uint8_t                 row;
    uint8_t                 col;
    uint8_t                 index;
    const char*             name;
}   corsair_v2_led;

typedef struct
{
    uint16_t                pid;
    bool                    wireless;
    device_type             type;
    uint8_t                 supports;
    uint8_t                 rows;
    uint8_t                 cols;
    const corsair_v2_zone*  zones[CORSAIR_ZONES_MAX];
    const corsair_v2_led*   layout;
    uint16_t                layout_size;
}   corsair_v2_device;

/*-----------------------------------------------------*\
| Corsair V2 Protocol Keyboards                         |
\*-----------------------------------------------------*/
#define CORSAIR_K55_RGB_PRO_PID                     0x1BA4
#define CORSAIR_K60_RGB_PRO_PID                     0x1BA0
#define CORSAIR_K60_RGB_PRO_LP_PID                  0x1BAD

/*-----------------------------------------------------*\
| Corsair V2 Protocol Mousemats                         |
\*-----------------------------------------------------*/
#define CORSAIR_MM700_PID                           0x1B9B

/*-----------------------------------------------------*\
| These constant values are defined in                  |
| CorsairPeripheralV2Devices.cpp                        |
\*-----------------------------------------------------*/
extern const unsigned int CORSAIR_V2_DEVICE_COUNT;
extern const corsair_v2_device** corsair_v2_device_list;
