/*---------------------------------------------------------*\
| SteelSeriesApexRegions.h                                  |
|                                                           |
|   Region settings for SteelSeries Apex 5/7/Pro and TKL    |
|                                                           |
|   Joseph East (dripsnek)                             2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <map>
#include <string>
#include <vector>
#include "RGBController_SteelSeriesApex.h"
#include "RGBControllerKeyNames.h"

#define NA  0xFFFFFFFF

/*----------------------------------------------------------------------*\
| As of firmware 4.1.0 there are in total 111 possible standard keys     |
| which are shared across the Apex Pro / 7 / TKL / 5 and their regional  |
| SKUs in addition to the 6 media keys. No SKU has all 111, however      |
| regardless of the physial layout, the one configuration can be used    |
| across all SKUs with missing keys simply having no effect.             |
| The complication comes in the visualisation as different key layouts   |
| change the LED positions, additionally some labels / scancodes are     |
| overloaded based on the language which clashes with the OpenRGB        |
| defaults. In order to account for this a base SKU (ANSI) is assumed    |
| which is transformed into a regional SKU when device detection returns |
| a known SKU number from the first 5 characters of the serial number.   |
\*----------------------------------------------------------------------*/

#define MATRIX_HEIGHT 6
#define MATRIX_WIDTH 22

/*-------------------------------------------------------*\
| Default keymap where values are indicies into led_names |
| or NA for no key.                                       |
\*-------------------------------------------------------*/
#define MATRIX_MAP_ANSI\
    { {   37,  NA,  53,  54,  55,  56,  NA,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  NA,  NA,  NA,  NA },\
      {   48,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  41,  42,  38,  NA,  68,  69,  70,  94,  95,  96,  97 },\
      {   39,  NA,  16,  22,  4 ,  17,  19,  24,  20,  8 ,  14,  15,  43,  44,  88,  71,  72,  73, 106, 107, 108,  98 },\
      {   52,  NA,  0 ,  18,  3 ,  5 ,  6 ,  7 ,  9 ,  10,  11,  46,  47,  36,  NA,  NA,  NA,  NA, 103, 104, 105,  NA },\
      {   80,  NA,  25,  23,  2 ,  21,  1 ,  13,  12,  49,  50,  51,  84,  NA,  NA,  NA,  77,  NA, 100, 101, 102,  99 },\
      {   79,  82,  81,  NA,  NA,  NA,  NA,  40,  NA,  NA,  NA,  85,  86,  87,  83,  75,  76,  74, 109,  NA, 110,  NA } };

static const int matrix_mapsize = MATRIX_HEIGHT * MATRIX_WIDTH;

static const char* led_names[] =
{
    KEY_EN_A,
    KEY_EN_B,
    KEY_EN_C,
    KEY_EN_D,
    KEY_EN_E,
    KEY_EN_F,
    KEY_EN_G,
    KEY_EN_H,
    KEY_EN_I,
    KEY_EN_J,
    KEY_EN_K,
    KEY_EN_L,
    KEY_EN_M,
    KEY_EN_N,
    KEY_EN_O,
    KEY_EN_P,
    KEY_EN_Q,
    KEY_EN_R,
    KEY_EN_S,
    KEY_EN_T,
    KEY_EN_U,
    KEY_EN_V,
    KEY_EN_W,
    KEY_EN_X,
    KEY_EN_Y,
    KEY_EN_Z,
    KEY_EN_1,
    KEY_EN_2,
    KEY_EN_3,
    KEY_EN_4,
    KEY_EN_5,
    KEY_EN_6,
    KEY_EN_7,
    KEY_EN_8,
    KEY_EN_9,
    KEY_EN_0,
    KEY_EN_ANSI_ENTER,
    KEY_EN_ESCAPE,
    KEY_EN_BACKSPACE,
    KEY_EN_TAB,
    KEY_EN_SPACE,
    KEY_EN_MINUS,
    KEY_EN_EQUALS,
    KEY_EN_LEFT_BRACKET,
    KEY_EN_RIGHT_BRACKET,
    KEY_EN_POUND,
    KEY_EN_SEMICOLON,
    KEY_EN_QUOTE,
    KEY_EN_BACK_TICK,
    KEY_EN_COMMA,
    KEY_EN_PERIOD,
    KEY_EN_FORWARD_SLASH,
    KEY_EN_CAPS_LOCK,
    KEY_EN_F1,
    KEY_EN_F2,
    KEY_EN_F3,
    KEY_EN_F4,
    KEY_EN_F5,
    KEY_EN_F6,
    KEY_EN_F7,
    KEY_EN_F8,
    KEY_EN_F9,
    KEY_EN_F10,
    KEY_EN_F11,
    KEY_EN_F12,
    KEY_EN_PRINT_SCREEN,
    KEY_EN_SCROLL_LOCK,
    KEY_EN_PAUSE_BREAK,
    KEY_EN_INSERT,
    KEY_EN_HOME,
    KEY_EN_PAGE_UP,
    KEY_EN_DELETE,
    KEY_EN_END,
    KEY_EN_PAGE_DOWN,
    KEY_EN_RIGHT_ARROW,
    KEY_EN_LEFT_ARROW,
    KEY_EN_DOWN_ARROW,
    KEY_EN_UP_ARROW,
    KEY_EN_ISO_BACK_SLASH,
    KEY_EN_LEFT_CONTROL,
    KEY_EN_LEFT_SHIFT,
    KEY_EN_LEFT_ALT,
    KEY_EN_LEFT_WINDOWS,
    KEY_EN_RIGHT_CONTROL,
    KEY_EN_RIGHT_SHIFT,
    KEY_EN_RIGHT_ALT,
    KEY_EN_RIGHT_WINDOWS,
    KEY_EN_RIGHT_FUNCTION,
    KEY_EN_ANSI_BACK_SLASH,
    KEY_JP_RO,
    KEY_JP_KANA,
    KEY_JP_YEN,
    KEY_JP_HENKAN,
    KEY_JP_MUHENKAN,
    KEY_EN_NUMPAD_LOCK,
    KEY_EN_NUMPAD_DIVIDE,
    KEY_EN_NUMPAD_TIMES,
    KEY_EN_NUMPAD_MINUS,
    KEY_EN_NUMPAD_PLUS,
    KEY_EN_NUMPAD_ENTER,
    KEY_EN_NUMPAD_1,
    KEY_EN_NUMPAD_2,
    KEY_EN_NUMPAD_3,
    KEY_EN_NUMPAD_4,
    KEY_EN_NUMPAD_5,
    KEY_EN_NUMPAD_6,
    KEY_EN_NUMPAD_7,
    KEY_EN_NUMPAD_8,
    KEY_EN_NUMPAD_9,
    KEY_EN_NUMPAD_0,
    KEY_EN_NUMPAD_PERIOD,
};

struct matrix_region_patch
{
    int row;
    int column;
    unsigned int value;
};

struct sku_patch
{
    std::vector<matrix_region_patch> base_patch;
    std::vector<matrix_region_patch> region_patch;
    std::map<int, std::string> key_patch;
};

/*-----------------------------------------------*\
| There are two types of patches, a SKU may       |
| use one, both or none. The first type are       |
| region patches which modify the location of     |
| a logical key (in terms of RGB) based on the    |
| scancode emitted by the physical key on the     |
| keyboard. This is the most important as it      |
| impacts what keys are lit in terms of RGB.      |
|                                                 |
| The second type are keyname lookups             |
| where the face of the key may be different      |
| between SKUs but the scancode is the same, this |
| is for convenience of the LED view in the GUI.  |
|                                                 |
| Each SKU has up to 3 operations performed on    |
| it. A generic TKL region patch (where req'd)    |
| the SKU region patch then a keyname lookup in   |
| that order.                                     |
\*-----------------------------------------------*/

/*-----------------------------------------------*\
| Region patches are structs consisting of        |
| {row, column, value} where row and column are   |
| 0 indexed into the logical key layout defined   |
| by MATRIX_MAP_ANSI. This array closely matches  |
| the physical layout of the keyboard. Value is   |
| an index into the led_names array and refers    |
| to the replacement character which should exist |
| at that location, or NA for no character. When  |
| the SetSkuRegion function is called, the        |
| contents of MATRIX_MAP_ANSI populate a local    |
| array, then based on the SKU argument the       |
| specific patches are applied to the local copy  |
| before being passed to the controller.          |
\*-----------------------------------------------*/

static const std::vector<matrix_region_patch> apex_jp_region_patch =
{
    {1, 13, 91},
    {1, 14, 38},
    {2, 14, 36},
    {3, 13, 45},
    {4, 12, 89},
    {4, 13, 84},
    {5,  3, 93},
    {5, 10, 92},
    {5, 11, 90},
    {5, 12, 85},
};

static const std::vector<matrix_region_patch> apex_iso_region_patch =
{
    {2, 14, 36},
    {3, 13, 45},
    {4,  1, 78},
};

/*-----------------------------------------------*\
| The TKL region patch is common for all TKL SKUs |
\*-----------------------------------------------*/

static const std::vector<matrix_region_patch> apex_tkl_us_region_patch =
{
    {0, 15, NA},
    {0, 16, NA},
    {0, 17, NA},
    {1, 18, NA},
    {1, 19, NA},
    {1, 20, NA},
    {1, 21, NA},
    {2, 18, NA},
    {2, 19, NA},
    {2, 20, NA},
    {2, 21, NA},
    {3, 18, NA},
    {3, 19, NA},
    {3, 20, NA},
    {4, 18, NA},
    {4, 19, NA},
    {4, 20, NA},
    {4, 21, NA},
    {5, 18, NA},
    {5, 20, NA},
};

/*-----------------------------------------------*\
| Keyname lookups change the character displayed  |
| on the LED view GUI by overriding the value     |
| associated with the index in led_names.         |
\*-----------------------------------------------*/

static const std::map<int, std::string> apex_jp_keyname_lookup =
{
    {42, KEY_JP_CHEVRON},
    {43, KEY_JP_AT},
    {44, KEY_EN_LEFT_BRACKET},
    {45, KEY_EN_RIGHT_BRACKET},
    {47, KEY_JP_COLON},
    {48, KEY_JP_EJ},
    {84, KEY_EN_RIGHT_SHIFT},
    {36, KEY_EN_ISO_ENTER},
};

static const std::map<int, std::string> apex_uk_keyname_lookup =
{
    {36, KEY_EN_ISO_ENTER},
};

static const std::map<int, std::string> apex_nor_keyname_lookup =
{
    {36, KEY_EN_ISO_ENTER},
    {41, KEY_NORD_PLUS_QUESTION},
    {42, KEY_NORD_ACUTE_GRAVE},
    {43, KEY_NORD_AAL},
    {44, KEY_NORD_DOTS_CARET},
    {45, KEY_NORD_QUOTE},
    {46, KEY_NORD_A_OE},
    {47, KEY_NORD_O_AE},
    {48, KEY_NORD_HALF},
    {51, KEY_NORD_HYPHEN},
    {78, KEY_NORD_ANGLE_BRACKET},
};

static const std::map<std::string, sku_patch> patch_lookup =
{
    /*----------------------------------------------------------*\
    | All TKL keyboards must use apex_tkl_us_region_patch as the |
    | base patch, then apply the regional patch on top (if any). |
    \*----------------------------------------------------------*/

    /*--------*\
    | APEX PRO |
    \*--------*/
    { "64739", { apex_tkl_us_region_patch, apex_iso_region_patch, apex_uk_keyname_lookup }},
    { "64738", { apex_tkl_us_region_patch, apex_iso_region_patch, apex_nor_keyname_lookup }},
    { "64737", { apex_tkl_us_region_patch, apex_jp_region_patch, apex_jp_keyname_lookup }},
    { "64856", { apex_tkl_us_region_patch, apex_iso_region_patch, apex_uk_keyname_lookup }},
    { "64734", { apex_tkl_us_region_patch, {}, {} }},

    { "64631", { {}, apex_iso_region_patch, apex_nor_keyname_lookup }},
    { "64634", { {}, apex_iso_region_patch, apex_uk_keyname_lookup }},
    { "64629", { {}, apex_jp_region_patch, apex_jp_keyname_lookup }},

    /*--------*\
    | APEX 7   |
    \*--------*/
    { "64646", { apex_tkl_us_region_patch, {}, {} }},
    { "64758", { apex_tkl_us_region_patch, {}, {} }},
    { "64747", { apex_tkl_us_region_patch, {}, {} }},
    { "64652", { apex_tkl_us_region_patch, apex_iso_region_patch, apex_uk_keyname_lookup }},
    { "64760", { apex_tkl_us_region_patch, apex_iso_region_patch, apex_uk_keyname_lookup }},
    { "64749", { apex_tkl_us_region_patch, apex_iso_region_patch, apex_uk_keyname_lookup }},
    { "64651", { apex_tkl_us_region_patch, apex_iso_region_patch, apex_nor_keyname_lookup }},
    { "64649", { apex_tkl_us_region_patch, apex_jp_region_patch, apex_jp_keyname_lookup }},
    { "64756", { apex_tkl_us_region_patch, apex_jp_region_patch, apex_jp_keyname_lookup }},

    { "64635", { {}, apex_iso_region_patch, apex_uk_keyname_lookup }},
    { "64778", { {}, apex_iso_region_patch, apex_uk_keyname_lookup }},
    { "64788", { {}, apex_iso_region_patch, apex_uk_keyname_lookup }},
    { "64641", { {}, apex_iso_region_patch, apex_nor_keyname_lookup }},
    { "64775", { {}, apex_iso_region_patch, apex_nor_keyname_lookup }},
    { "64787", { {}, apex_iso_region_patch, apex_nor_keyname_lookup }},
    { "64639", { {}, apex_jp_region_patch, apex_jp_keyname_lookup }},
    { "64772", { {}, apex_jp_region_patch, apex_jp_keyname_lookup }},

    /*--------*\
    | APEX 5   |
    \*--------*/
    { "64534", { {}, apex_iso_region_patch, apex_uk_keyname_lookup }},
    { "64537", { {}, apex_jp_region_patch, apex_jp_keyname_lookup }},
    { "64533", { {}, apex_iso_region_patch, apex_nor_keyname_lookup }},
};

static void SetSkuRegion (matrix_map_type& input, std::string& sku)
{
    std::map<std::string, sku_patch>::const_iterator it     = patch_lookup.find(sku);
    unsigned int local_matrix [MATRIX_HEIGHT][MATRIX_WIDTH] = MATRIX_MAP_ANSI;
    input.height                                            = MATRIX_HEIGHT;
    input.width                                             = MATRIX_WIDTH;

    if(it != patch_lookup.end())
    {
        for(std::size_t i = 0; i < it->second.base_patch.size(); i++)
        {
            local_matrix[it->second.base_patch[i].row][it->second.base_patch[i].column] = it->second.base_patch[i].value;
        }
        for(std::size_t i = 0; i < it->second.region_patch.size(); i++)
        {
            local_matrix[it->second.region_patch[i].row][it->second.region_patch[i].column] = it->second.region_patch[i].value;
        }
    }
    memcpy(input.map, (unsigned int *)local_matrix, sizeof(unsigned int)*MATRIX_HEIGHT*MATRIX_WIDTH);
}

static void SetSkuLedNames (std::vector<led>& input, std::string& sku, unsigned int led_count)
{
    std::map<std::string, sku_patch>::const_iterator it = patch_lookup.find(sku);

    for(unsigned int led_idx = 0; led_idx < led_count; led_idx++)
    {
        led new_led;

        if(it != patch_lookup.end())
        {
            std::map<int, std::string>::const_iterator jt = it->second.key_patch.find(led_idx);

            if(jt == it->second.key_patch.end())
            {
                new_led.name = led_names[led_idx];
            }
            else if(jt != it->second.key_patch.end())
            {
                new_led.name = jt->second;
            }
        }
        else
        {
            new_led.name = led_names[led_idx];
        }
        input.push_back(new_led);
    }
}

/*-----------------------------------------------------------*\
| SKU codes for all known Apex Pro / 7 / 5 & TKL variant      |
| keyboards as at Janauary 2022. Generated by cross-checking  |
| store listings aginst Steelseries website.                  |
|                                                             |
|  -- APEX PRO --                                             |
|                                                             |
|  "64626", // US                                             |
|  "64627", // German                                         |
|  "64628", // French                                         |
|  "64629", // Japanese                                       |
|  "64630", // Korean                                         |
|  "64631", // Nordic                                         |
|  "64632", // Taiwanese                                      |
|  "64633", // Thai                                           |
|  "64634"  // UK                                             |
|                                                             |
|  >> APEX PRO TKL                                            |
|                                                             |
|  "64734", // US TKL                                         |
|  "64735", // German TKL                                     |
|  "64736", // French TKL                                     |
|  "64737", // Japanese TKL                                   |
|  "64738", // Nordic TKL                                     |
|  "64739", // UK TKL                                         |
|                                                             |
|  -- APEX 7   --                                             |
|                                                             |
|  >> RED switches                                            |
|                                                             |
|  "64635", // UK Red                                         |
|  "64636", // US Red                                         |
|  "64637", // German Red                                     |
|  "64638", // French Red                                     |
|  "64639", // Japanese Red                                   |
|  "64640", // Korean Red                                     |
|  "64641", // Nordic Red                                     |
|  "64642", // Russian Red                                    |
|  "64643", // Thai Red                                       |
|  "64644", // Turkish Red                                    |
|  "64645", // Taiwanese Red                                  |
|                                                             |
|  >> RED TKL                                                 |
|                                                             |
|  "64646"  // US Red TKL                                     |
|  "64647", // German Red TKL                                 |
|  "64648", // French Red TKL                                 |
|  "64649", // Japanese Red TKL                               |
|  "64650", // Korean Red TKL                                 |
|  "64651", // Nordic Red TKL                                 |
|  "64652", // UK Red TKL                                     |
|                                                             |
|  >> BLUE switches                                           |
|                                                             |
|  "64770"  // German Blue                                    |
|  "64771"  // French Blue                                    |
|  "64772"  // Japanese Blue                                  |
|  "64773"  // Korean Blue                                    |
|  "64774"  // US Blue                                        |
|  "64775"  // Nordic Blue                                    |
|  "64776"  // Thai Blue                                      |
|  "64777"  // Taiwanese Blue                                 |
|  "64778"  // UK Blue                                        |
|                                                             |
|  >> BLUE TKL                                                |
|                                                             |
|  "64756"  // Japanese Blue TKL                              |
|  "64757"  // Korean Blue TKL                                |
|  "64758"  // US Blue TKL                                    |
|  "64760"  // UK Blue TKL                                    |
|                                                             |
|  >> BROWN switches                                          |
|                                                             |
|  "64784"  // German Brown                                   |
|  "64785"  // French Brown                                   |
|  "64786"  // US Brown                                       |
|  "64787"  // Nordic Brown                                   |
|  "64788"  // UK Brown                                       |
|                                                             |
|  >> BROWN TKL                                               |
|                                                             |
|  "64746"  // French Brown TKL                               |
|  "64747"  // US Brown TKL                                   |
|  "64749"  // UK Brown TKL                                   |
|                                                             |
|  -- APEX 5   --                                             |
|                                                             |
|  "64533", // Nordic                                         |
|  "64534", // UK                                             |
|  "64535", // German                                         |
|  "64536", // French                                         |
|  "64537", // Japanese                                       |
|  "64538", // Turkish                                        |
|  "64539", // US                                             |
|                                                             |
\*-----------------------------------------------------------*/
