/*---------------------------------------------------------*\
| CMKeyboardDevices.h                                       |
|                                                           |
|   Device list for Cooler Master keyboards                 |
|                                                           |
|   Tam D (too.manyhobbies)                     30 Nov 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "KeyboardLayoutManager.h"

/*-----------------------------------------------------*\
| List of all supported effects by this controller.     |
| All of these effects are firmware controlled, and     |
| they types of effects supported will depend on the    |
| Keyboard.                                             |
|                                                       |
| To enable a command, the SetEffect(effectId) needs    |
| to be called. The specific effectId->Effect mapping   |
| depends on the keyboard.                              |
\*-----------------------------------------------------*/
enum cm_keyboard_effect_type
{
    NONE = 0,
    DIRECT,
    SINGLE,
    FULLY_LIT,
    STATIC,
    BREATHE,
    CYCLE,
    WAVE,
    RIPPLE,
    CROSS,
    RAINDROPS,
    STARS,
    SNAKE,
    CUSTOMIZED,
    INDICATOR,
    MULTILAYER,
    REACTIVE_FADE,
    REACTIVE_PUNCH,
    REACTIVE_TORNADO,
    HEARTBEAT,
    FIREBALL,
    SNOW,
    CIRCLE_SPECTRUM,
    WATER_RIPPLE,
    OFF
};

#define CM_KB_ZONES_MAX 1

typedef struct
{
    std::string                     name;
    zone_type                       type;
} cm_kb_zone;

typedef struct
{
    uint16_t                        product_id;
    const cm_kb_zone *              zones[CM_KB_ZONES_MAX];
    keyboard_keymap_overlay_values* layout_new;
} cm_kb_device;

#define COOLERMASTER_VID    0x2516

#define CMKB_MAXKEYS        256

/*-----------------------------------------------------------------*\
| keyboard support status is indicated to the right of              |
| the PID definition. Attribution to products is also               |
| indicated.                                                        |
|                                                                   |
| libcmmk                                                           |
| signal    https://gitlab.com/signalrgb/signal-plugins             |
| openrgb                                                           |
| ck550-macos https://github.com/vookimedlo/ck550-macos/tree/master |
| reversed                                                          |
|                                                                   |
| issue tickets, open merge requests etc are provided               |
| for developer references.                                         |
| # denotes issue ticket                                            |
| ! denotes merge/pull request                                      |
\*-----------------------------------------------------------------*/
#define COOLERMASTER_KEYBOARD_CK351_PID           0x014F // unsupported
#define COOLERMASTER_KEYBOARD_CK530_PID           0x009F // [ck550-macos]
#define COOLERMASTER_KEYBOARD_CK530_V2_PID        0x0147 // [signal]
#define COOLERMASTER_KEYBOARD_CK550_V2_PID        0x0145 // [openrgb #800, #2863, signal]
#define COOLERMASTER_KEYBOARD_CK552_V2_PID        0x007F // [ck550-macos, signal]
#define COOLERMASTER_KEYBOARD_CK570_V2_PID        0x01E8 // unsupported
#define COOLERMASTER_KEYBOARD_CK720_PID           0x016B // unsupported
#define COOLERMASTER_KEYBOARD_CK721_PID           0x016D // unsupported
#define COOLERMASTER_KEYBOARD_CK721LINE_PID       0x01EE // unsupported
#define COOLERMASTER_KEYBOARD_PRO_L_PID           0x003B // [libcmmk !16]
#define COOLERMASTER_KEYBOARD_PRO_L_WHITE_PID     0x0047 // [libcmmk]
#define COOLERMASTER_KEYBOARD_PRO_S_PID           0x003C // [libcmmk #30 !31 !36, !37, !7, #5(closed), #3(closed)]
// MASTERKEYS PRO M [libcmmk #17]
#define COOLERMASTER_KEYBOARD_MK721_PID           0x016F // unsupported
#define COOLERMASTER_KEYBOARD_MK730_PID           0x008F // [openrgb #1630, libcmmk]
#define COOLERMASTER_KEYBOARD_MK750_PID           0x0067 // fw1.2 [libcmmk #25 !9, !14, signal]
#define COOLERMASTER_KEYBOARD_MK770_PID           0x01D5 // unsupported
#define COOLERMASTER_KEYBOARD_MK850_PID           0x0069 // [signal]
#define COOLERMASTER_KEYBOARD_SK620B_PID          0x0157 // [openrgb #4292]
#define COOLERMASTER_KEYBOARD_SK620W_PID          0x0159 // [openrgb #4292, signal]
#define COOLERMASTER_KEYBOARD_SK622B_PID          0x0149 // [openrgb #3110, signal #217(closed)]
#define COOLERMASTER_KEYBOARD_SK622W_PID          0x014B // [signal]
#define COOLERMASTER_KEYBOARD_SK630_PID           0x0089 // [openrgb #967, libcmmk !21]
#define COOLERMASTER_KEYBOARD_SK631B_PID          0x008B // unsupported
#define COOLERMASTER_KEYBOARD_SK631W_PID          0x0125 // [libcmmk]
#define COOLERMASTER_KEYBOARD_SK650_PID           0x008D // [openrgb #613, libcmmk #23 !37 !27 !28, signal]
#define COOLERMASTER_KEYBOARD_SK651B_PID          0x0091 // unsupported
#define COOLERMASTER_KEYBOARD_SK651W_PID          0x0127 // [signal]
#define COOLERMASTER_KEYBOARD_SK652_PID           0x015D // [signal]
#define COOLERMASTER_KEYBOARD_SK653_PID           0x01AB // [openrgb #3571, signal]

extern const unsigned int COOLERMASTER_KEYBOARD_DEVICE_COUNT;
extern const cm_kb_device** cm_kb_device_list;
