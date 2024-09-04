#pragma once

#include <string>
#include <vector>
#include "RGBController.h"
#include "RGBControllerKeyNames.h"
#include "KeyboardLayoutManager.h"

#define AURA_CORE_LAPTOP_ZONES_MAX      6

enum aura_core_kb_layout
{
    AURA_CORE_LAPTOP_KB_LAYOUT_ANSI     = 0x01,     /* US ANSI Layout                   */
    AURA_CORE_LAPTOP_KB_LAYOUT_ISO      = 0x02,     /* EURO ISO Layout                  */
    AURA_CORE_LAPTOP_KB_LAYOUT_ABNT     = 0x03,     /* Brazilian Layout                 */
    AURA_CORE_LAPTOP_KB_LAYOUT_JIS      = 0x04,     /* Japanese Layout                  */
};

typedef struct
{
    std::string                         name;
    keyboard_keymap_overlay_values*     layout_new;
}   aura_core_laptop_zone;

typedef struct
{
    uint8_t                             zone;
    uint8_t                             row;
    uint8_t                             col;
    uint8_t                             index;
    const char*                         name;
}   aura_core_laptop_led;

typedef struct
{
    std::string                         dmi_name;
    const aura_core_laptop_zone*        zones[AURA_CORE_LAPTOP_ZONES_MAX];
}   aura_core_laptop_device;

/*-----------------------------------------------------*\
| Aura Core Laptop Protocol Keyboards                   |
\*-----------------------------------------------------*/
#define AURA_STRIX_SCAR_15_PID                      0x19B6

/*-----------------------------------------------------*\
| These constant values are defined in                  |
| AsusAuraCoreLaptopDevices.cpp                         |
\*-----------------------------------------------------*/
extern const unsigned int AURA_CORE_LAPTOP_DEVICE_COUNT;
extern const aura_core_laptop_device** aura_core_laptop_device_list;
