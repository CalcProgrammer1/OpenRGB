#pragma once

#include <string>
#include "RGBController.h"
#include "RGBControllerKeyNames.h"
#include "KeyboardLayoutManager.h"

#define CORSAIR_ZONES_MAX               6

enum corsair_v2_device_mode
{
    CORSAIR_V2_MODE_HW                  = 0x01,     /* Hardware RGB mode                */
    CORSAIR_V2_MODE_SW                  = 0x02,     /* Software RGB mode                */
};

enum corsair_v2_supports
{
    CORSAIR_V2_TYPE_SW_COLOUR_BLOCK     = 1,
    CORSAIR_V2_TYPE_HW_COLOUR_BLOCK     = 2,
    CORSAIR_V2_TYPE_SW_TRIPLETS         = 3,
    CORSAIR_V2_TYPE_HW_TRIPLETS         = 4,
};

enum corsair_v2_kb_layout
{
    CORSAIR_V2_KB_LAYOUT_ANSI           = 0x01,     /* US ANSI Layout                   */
    CORSAIR_V2_KB_LAYOUT_ISO            = 0x02,     /* EURO ISO Layout                  */
    CORSAIR_V2_KB_LAYOUT_ABNT           = 0x03,     /* Brazilian Layout                 */
    CORSAIR_V2_KB_LAYOUT_JIS            = 0x04,     /* Japanese Layout                  */
};

typedef struct
{
    std::string                         name;
    zone_type                           type;
    uint8_t                             rows;
    uint8_t                             cols;
}   corsair_v2_zone;

typedef struct
{
    uint8_t                             zone;
    uint8_t                             row;
    uint8_t                             col;
    uint8_t                             index;
    const char*                         name;
}   corsair_v2_led;

typedef struct
{
    uint16_t                            pid;
    bool                                wireless;
    device_type                         type;
    corsair_v2_supports                 protocol;
    uint8_t                             rows;
    uint8_t                             cols;
    const corsair_v2_zone*              zones[CORSAIR_ZONES_MAX];
    keyboard_keymap_overlay_values*     layout_new;
}   corsair_v2_device;

/*-----------------------------------------------------*\
| Corsair V2 Protocol Keyboards                         |
\*-----------------------------------------------------*/
#define CORSAIR_K55_RGB_PRO_PID                     0x1BA4
#define CORSAIR_K60_RGB_PRO_PID                     0x1BA0
#define CORSAIR_K60_RGB_PRO_LP_PID                  0x1BAD

/*-----------------------------------------------------*\
| Corsair V2 Protocol Mice                              |
\*-----------------------------------------------------*/
#define CORSAIR_IRONCLAW_WIRELESS_PID               0x1BA6
#define CORSAIR_IRONCLAW_WIRELESS_WIRED_PID         0x1B4C
#define CORSAIR_M55_RGB_PRO_PID                     0x1B70

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
