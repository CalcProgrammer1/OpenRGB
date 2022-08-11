#pragma once

#include <string>
#include "RGBController.h"
#include "RGBControllerKeyNames.h"

#define CORSAIR_ZONES_MAX   5

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
}   corsair_zone;

typedef struct
{
    uint8_t                 zone;
    uint8_t                 row;
    uint8_t                 col;
    const char*             name;
}   corsair_led;

typedef struct
{
    uint16_t                pid;
    bool                    wireless;
    device_type             type;
    uint8_t                 supports;
    uint8_t                 rows;
    uint8_t                 cols;
    const corsair_zone*     zones[CORSAIR_ZONES_MAX];
    const corsair_led*      layout;
    uint16_t                layout_size;
}   corsair_device;

/*-----------------------------------------------------*\
| Corsair V2 Protocol Keyboards                         |
\*-----------------------------------------------------*/
#define CORSAIR_K55_RGB_PRO_PID                     0x1BA4

/*-----------------------------------------------------*\
| Corsair V2 Protocol Mousemats                         |
\*-----------------------------------------------------*/
#define CORSAIR_MM700_PID                           0x1B9B



/*-------------------------------------------------------------*\
|  Corsair K55 RGB Pro 1B1C:1BA4                                |
|                                                               |
|  Zone "Keyboard"                                              |
|       Linear                                                  |
|       1 Row, 6 Columns                                        |
\*-------------------------------------------------------------*/
static const corsair_zone k55_rgb_pro_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_LINEAR,
    1,
    6
};

static const corsair_device k55_rgb_pro_device =
{
    CORSAIR_K55_RGB_PRO_PID,
    false,
    DEVICE_TYPE_KEYBOARD,
    CORSAIR_V2_MODE_SW,
    1,
    6,
    {
        &k55_rgb_pro_zone,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    },
    nullptr,
    0
};

/*-------------------------------------------------------------*\
|  Corsair MM700 1B1C:1B9B                                      |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|                                                               |
|  Zone "Edge"                                                  |
|       Linear                                                  |
|       1 Row, 2 Columns                                        |
\*-------------------------------------------------------------*/
static const corsair_zone mm700_right_zone =
{
    "Right",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const corsair_zone mm700_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const corsair_zone mm700_left_zone =
{
    "Left",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const corsair_device mm700_device =
{
    CORSAIR_MM700_PID,
    false,
    DEVICE_TYPE_MOUSEMAT,
    CORSAIR_V2_MODE_SW,
    1,
    3,
    {
        &mm700_left_zone,
        &mm700_right_zone,
        &mm700_logo_zone,
        nullptr,
        nullptr
    },
    nullptr,
    0
};

/*-------------------------------------------------------------------------*\
|  DEVICE MASTER LIST                                                       |
\*-------------------------------------------------------------------------*/
#define CORSAIR_DEVICE_COUNT    (sizeof(device_list) / sizeof(device_list[ 0 ]))

static const corsair_device* device_list[] =
{
/*-----------------------------------------------------------------*\
|  KEYBOARDS                                                        |
\*-----------------------------------------------------------------*/
    &k55_rgb_pro_device,

/*-----------------------------------------------------------------*\
|  MOUSEMATS                                                        |
\*-----------------------------------------------------------------*/
    &mm700_device,
};
