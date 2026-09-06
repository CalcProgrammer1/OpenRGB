/*---------------------------------------------------------*\
| MSIMotherboard185Controller.cpp                           |
|                                                           |
|   Driver for MSI Mystic Light 185-byte motherboard        |
|                                                           |
|   Direct mode functionality has been implemented based on |
|   the mystic-why project provided by Aleksandr            |
|   Garashchenko                                            |
|   (https://github.com/garashchenko/mystic-why)            |
|                                                           |
|   T-bond                                      03 Apr 2020 |
|   Adam Honse                                  06 Mar 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <algorithm>
#include <array>
#include <bitset>
#include <cstdio>
#include "MSIMotherboard185Controller.h"
#include "LogManager.h"
#include "StringUtils.h"

using namespace std::chrono_literals;


#define BITSET(val, bit, pos)       ((unsigned char)std::bitset<8>(val).set((pos), (bit)).to_ulong())

#define SYNC_PER_LED_MODE_JRAINBOW_LED_COUNT     40
#define SYNC_PER_LED_MODE_CORSAIR_LED_COUNT     120
#define JRAINBOW1_MAX_LED_COUNT                 200
#define JRAINBOW2_MAX_LED_COUNT                 240
#define JCORSAIR_MAX_LED_COUNT                  240

#define MSI_DIRECT_MODE                         0x25
#define PER_LED_BASIC_SYNC_MODE                 (MSI_SPEED_BRIGHTNESS_FLAG_SYNC_SETTING_JRGB | MSI_COLOR_FLAG_SYNC_SETTING_ONBOARD | MSI_COLOR_FLAG_SYNC_SETTING_JPIPE1 | MSI_COLOR_FLAG_SYNC_SETTING_JPIPE2)
#define PER_LED_FULL_SYNC_MODE                  (PER_LED_BASIC_SYNC_MODE | MSI_COLOR_FLAG_SYNC_SETTING_JRAINBOW1 | MSI_COLOR_FLAG_SYNC_SETTING_JRAINBOW2 | MSI_COLOR_FLAG_SYNC_SETTING_JCORSAIR)

struct mystic_light_185_config
{
    unsigned short                           pid;                   // PID of the board
    const char*                              name;                  // DMI board name for filtering (nullptr = match any name)
    int                                      numof_onboard_leds;    // number of onboard leds
    int                                      numof_pipe1_leds;      // number of pipe 1 leds (used in per LED mode only)
    int                                      numof_pipe2_leds;      // number of pipe 2 leds (used in per LED mode only)
    int                                      numof_JRGBs;           // number of supported JRGB headers (used in per LED mode only)
    const std::vector<MSI_ZONE>*             supported_zones;       // pointer to vector of supported zones
    MSIMotherboard185Controller::DIRECT_MODE per_led_mode;          // type of direct mode support
};

const std::vector<MSI_ZONE> all_zones =
{
    MSI_ZONE_J_RGB_1,
    MSI_ZONE_J_RGB_2,
    MSI_ZONE_J_RAINBOW_1,
    MSI_ZONE_J_RAINBOW_2,
    MSI_ZONE_J_CORSAIR,
    MSI_ZONE_J_PIPE_1,
    MSI_ZONE_J_PIPE_2,
    MSI_ZONE_ON_BOARD_LED_0
};

const std::vector<MSI_ZONE> zones_set0 =
{
    MSI_ZONE_J_RGB_1,
    MSI_ZONE_J_RGB_2,
    MSI_ZONE_J_RAINBOW_1,
    MSI_ZONE_J_RAINBOW_2,
    MSI_ZONE_ON_BOARD_LED_0
};

const std::vector<MSI_ZONE> zones_set1 =
{
    MSI_ZONE_J_RGB_1,
    MSI_ZONE_J_RAINBOW_1,
    MSI_ZONE_J_RAINBOW_2,
    MSI_ZONE_J_CORSAIR
};

const std::vector<MSI_ZONE> zones_set2 =
{
    MSI_ZONE_J_RGB_1,
    MSI_ZONE_J_RAINBOW_1,
    MSI_ZONE_J_RAINBOW_2,
    MSI_ZONE_ON_BOARD_LED_0
};

const std::vector<MSI_ZONE> zones_set3 =
{
    MSI_ZONE_J_RGB_1,
    MSI_ZONE_J_RAINBOW_1,
    MSI_ZONE_J_RAINBOW_2,
    MSI_ZONE_J_CORSAIR,
    MSI_ZONE_J_PIPE_1,
    MSI_ZONE_ON_BOARD_LED_0
};

const std::vector<MSI_ZONE> zones_set4 =
{
    MSI_ZONE_J_RGB_1,
    MSI_ZONE_J_RAINBOW_1,
    MSI_ZONE_J_RAINBOW_2,
    MSI_ZONE_J_PIPE_1
};

const std::vector<MSI_ZONE> zones_set5 =
{
    MSI_ZONE_J_RGB_1,
    MSI_ZONE_J_RAINBOW_1
};

const std::vector<MSI_ZONE> zones_set6 =
{
    MSI_ZONE_J_RAINBOW_1
};

const std::vector<MSI_ZONE> zones_set7 =
{
    MSI_ZONE_J_RGB_1,
    MSI_ZONE_J_RGB_2,
    MSI_ZONE_J_RAINBOW_1,
    MSI_ZONE_J_RAINBOW_2,
    MSI_ZONE_J_PIPE_1,
    MSI_ZONE_ON_BOARD_LED_0
};

const std::vector<MSI_ZONE> zones_set8 =
{
    MSI_ZONE_J_RGB_1,
    MSI_ZONE_J_RAINBOW_1,
    MSI_ZONE_J_RAINBOW_2,
    MSI_ZONE_J_CORSAIR,
    MSI_ZONE_J_PIPE_1,
    MSI_ZONE_J_PIPE_2
};

const std::vector<MSI_ZONE> zones_set9 =
{
    MSI_ZONE_J_RGB_1,
    MSI_ZONE_J_RAINBOW_1,
    MSI_ZONE_J_RAINBOW_2,
    MSI_ZONE_J_CORSAIR,
    MSI_ZONE_J_PIPE_1
};

const std::vector<MSI_ZONE> zones_set10 =
{
    MSI_ZONE_J_RGB_1,
    MSI_ZONE_J_RAINBOW_1,
    MSI_ZONE_J_RAINBOW_2,
    MSI_ZONE_J_RAINBOW_3
};

const std::vector<MSI_ZONE> zones_set11 =
{
    MSI_ZONE_J_RGB_1,
    MSI_ZONE_J_RAINBOW_1,
    MSI_ZONE_J_RAINBOW_2
};

const std::vector<MSI_ZONE> zones_set12 =
{
    MSI_ZONE_J_RGB_1,
    MSI_ZONE_J_RAINBOW_1,
    MSI_ZONE_J_RAINBOW_2,
    MSI_ZONE_J_RAINBOW_3,
    MSI_ZONE_J_PIPE_1,
    MSI_ZONE_ON_BOARD_LED_0
};

const std::vector<MSI_ZONE> zones_set13 =
{
    MSI_ZONE_J_RGB_1,
    MSI_ZONE_J_RGB_2,
    MSI_ZONE_J_RAINBOW_1,
    MSI_ZONE_J_RAINBOW_2
};

const std::vector<MSI_ZONE> zones_set14 =
{
    MSI_ZONE_J_RAINBOW_1,
    MSI_ZONE_J_RAINBOW_2
};

const std::vector<MSI_ZONE> zones_set15 =
{
    MSI_ZONE_J_RGB_1,
    MSI_ZONE_J_RAINBOW_1,
    MSI_ZONE_J_RAINBOW_2,
    MSI_ZONE_J_RAINBOW_3,
    MSI_ZONE_J_PIPE_1,
    MSI_ZONE_J_PIPE_2,
    MSI_ZONE_ON_BOARD_LED_0
};

const std::vector<MSI_ZONE> zones_set16 =
{
    MSI_ZONE_J_RGB_1,
    MSI_ZONE_J_RAINBOW_1,
    MSI_ZONE_J_RAINBOW_2,
    MSI_ZONE_J_RAINBOW_3,
    MSI_ZONE_J_PIPE_1,
    MSI_ZONE_J_PIPE_2
};

const std::vector<MSI_ZONE> zones_set17 =
{
    MSI_ZONE_J_RGB_1,
    MSI_ZONE_J_RGB_2,
    MSI_ZONE_J_RAINBOW_1,
    MSI_ZONE_J_RAINBOW_2,
    MSI_ZONE_J_PIPE_1
};

const std::vector<MSI_ZONE> zones_set18 =
{
    MSI_ZONE_J_RGB_1,
    MSI_ZONE_J_RAINBOW_1,
    MSI_ZONE_J_RAINBOW_2,
    MSI_ZONE_J_RAINBOW_3,
    MSI_ZONE_J_PIPE_1
};

const std::vector<MSI_ZONE> zones_set19 =
{
    MSI_ZONE_J_RAINBOW_1,
    MSI_ZONE_J_RAINBOW_2,
    MSI_ZONE_J_RAINBOW_3
};

const std::vector<MSI_ZONE> zones_set20 =
{
    MSI_ZONE_J_RGB_1,
};

const std::vector<MSI_ZONE> zones_set21 =
{
    MSI_ZONE_J_RGB_1,
    MSI_ZONE_J_RGB_2,
    MSI_ZONE_J_RAINBOW_1,
    MSI_ZONE_J_RAINBOW_2,
    MSI_ZONE_J_CORSAIR,
    MSI_ZONE_J_PIPE_1,
    MSI_ZONE_J_PIPE_2,
    MSI_ZONE_ON_BOARD_LED_0
};


/*---------------------------------------------------------------------------------------------------------------------------------*\
| Definition of the board sepcific configurations (number of onboard LEDs and supported zones).                                     |
|                                                                                                                                   |
| Only tested boards are listed here (refer to MSIMotherboardControllerDetect.cpp). If more boards                                  |
| are tested the list must be extended here. Otherwise the default settings will be used (6 onboard LEDs, 2 JRGB, no direct mode).  |
| Boards with yet unknown supported zones are configured to support all zones.                                                      |
\*---------------------------------------------------------------------------------------------------------------------------------*/

#define NUMOF_CONFIGS       (sizeof(board_configs) / sizeof(mystic_light_185_config))

static const mystic_light_185_config board_configs[] =
{
    { 0x7B93, nullptr,                              6,  1,  0, 1, &zones_set3,  MSIMotherboard185Controller::DIRECT_MODE_ZONE_BASED },    // MPG X570 GAMING PRO CARBON WIFI
    { 0x7C34, nullptr,                              0,  1,  1, 1, &zones_set8,  MSIMotherboard185Controller::DIRECT_MODE_ZONE_BASED },    // MEG X570 GODLIKE
    { 0x7C35, nullptr,                              0,  1,  0, 1, &zones_set9,  MSIMotherboard185Controller::DIRECT_MODE_ZONE_BASED },    // MEG X570 ACE
    { 0x7C36, nullptr,                              6,  1,  0, 1, &zones_set3,  MSIMotherboard185Controller::DIRECT_MODE_ZONE_BASED },    // PRESTIGE X570 CREATION
    { 0x7C37, nullptr,                              6,  0,  0, 2, &zones_set0,  MSIMotherboard185Controller::DIRECT_MODE_ZONE_BASED },    // MPG X570 GAMING PLUS
    { 0x7C56, "MSI B550-A PRO (MS-7C56)",           6,  0,  0, 1, &zones_set11, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MPG B550-A PRO
    { 0x7C56, "MSI MPG B550 GAMING PLUS (MS-7C56)", 6,  0,  0, 1, &zones_set2,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MPG B550 GAMING PLUS
    { 0x7C59, nullptr,                              0,  8,  0, 1, &zones_set9,  MSIMotherboard185Controller::DIRECT_MODE_DISABLED },      // CREATOR TRX40
    { 0x7C60, nullptr,                              6,  0,  0, 1, &zones_set1,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // TRX40-A PRO
    { 0x7C67, nullptr,                              6,  0,  0, 1, &zones_set5,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MAG B365M MORTAR
    { 0x7C71, nullptr,                              6,  6,  0, 1, &zones_set3,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MEG Z490 ACE
    { 0x7C73, nullptr,                              6,  4,  0, 1, &zones_set3,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MPG Z490 GAMING CARBON WIFI
    { 0x7C75, nullptr,                              6,  0,  0, 1, &zones_set2,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MPG Z490 GAMING PLUS
    { 0x7C76, nullptr,                              6,  0,  0, 2, &zones_set0,  MSIMotherboard185Controller::DIRECT_MODE_DISABLED },      // MPG Z490M GAMING EDGE
    { 0x7C77, nullptr,                              0,  0,  0, 0, &zones_set14, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MEG Z490I UNIFY
    { 0x7C79, nullptr,                              6,  0,  0, 2, &zones_set0,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MPG Z490 GAMING EDGE WIFI
    { 0x7C80, nullptr,                              6,  0,  0, 2, &zones_set0,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MAG Z490 TOMAHAWK
    { 0x7C02, nullptr,                              6,  0,  0, 2, &zones_set0,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MAG B450 TOMAHAWK MAX
    { 0x7C81, nullptr,                              6,  0,  0, 2, &zones_set0,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MAG B460 TOMAHAWK
    { 0x7C82, nullptr,                              6,  0,  0, 1, &zones_set5,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MAG B460M MORTAR WIFI
    { 0x7C83, nullptr,                              6,  0,  0, 1, &zones_set5,  MSIMotherboard185Controller::DIRECT_MODE_DISABLED },      // B460M PRO-VDH WIFI
    { 0x7C84, nullptr,                              6,  0,  0, 2, &zones_set0,  MSIMotherboard185Controller::DIRECT_MODE_ZONE_BASED },    // MAG X570 TOMAHAWK WIFI
    { 0x7C86, nullptr,                              6,  0,  0, 1, &zones_set5,  MSIMotherboard185Controller::DIRECT_MODE_DISABLED },      // MPG B460I GAMING EDGE
    { 0x7C87, nullptr,                              6,  0,  0, 2, &zones_set13, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // B450M BAZOOKA MAX WIFI
    { 0x7C90, nullptr,                              6,  4,  0, 1, &zones_set3,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MPG B550 GAMING CARBON WIFI
    { 0x7C91, nullptr,                              6,  0,  0, 2, &zones_set0,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MAG B550 TOMAHAWK
    { 0x7C92, nullptr,                              6,  0,  0, 0, &zones_set6,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MPG B550I GAMING EDGE WIFI
    { 0x7C94, nullptr,                              6,  0,  0, 1, &zones_set11, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MAG B550M MORTAR
    { 0x7C95, nullptr,                              6,  0,  0, 2, &zones_set0,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // B550M PRO-VDH WIFI
    { 0x7C98, nullptr,                              6,  0,  0, 1, &zones_set5,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // Z490 PLUS
    { 0x7D03, nullptr,                              0, 15, 18, 1, &zones_set8,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MPG Z590 GODLIKE
    { 0x7D06, nullptr,                              4,  4,  0, 1, &zones_set3,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MPG Z590 GAMING FORCE
    { 0x7D07, nullptr,                              4,  5,  0, 2, &zones_set7,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MPG Z590 GAMING EDGE WIFI
    { 0x7D08, nullptr,                              6,  0,  0, 2, &zones_set0,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MAG Z590 TOMAHAWK
    { 0x7D09, nullptr,                              6,  0,  0, 2, &zones_set11, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // Z590-A PRO WIFI
    { 0x7D13, nullptr,                              6,  0,  0, 1, &zones_set1,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MEG B550 UNIFY
    { 0x7D14, nullptr,                              6,  0,  0, 1, &zones_set5,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // A520M PRO
    { 0x7D15, nullptr,                              6,  0,  0, 2, &zones_set0,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MAG B560 TOMAHAWK WIFI
    { 0x7D17, nullptr,                              6,  0,  0, 2, &zones_set11, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MAG B560M MORTAR
    { 0x7D18, nullptr,                              6,  0,  0, 2, &zones_set5,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MAG B560M PRO-VDH
    { 0x7D19, nullptr,                              6,  0,  0, 2, &zones_set5,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MPG B560I GAMIING EDGE WIFI
    { 0x7D20, nullptr,                              6,  0,  0, 2, &zones_set5,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MAG B560M PRO
    { 0x7D25, nullptr,                              6,  0,  0, 2, &zones_set11, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // PRO Z690-A WIFI DDR4
    { 0x7D27, nullptr,                              6,  0,  0, 2, &zones_set1,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MEG Z690 ACE
    { 0x7D28, nullptr,                              6,  0,  0, 1, &zones_set1,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MEG Z690 UNIFY-X
    { 0x7D29, nullptr,                              6,  0,  0, 0, &zones_set6,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MEG Z690I UNIFY
    { 0x7D30, nullptr,                              6,  6,  0, 2, &zones_set3,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MPG Z690 CARBON WIFI
    { 0x7D31, nullptr,                              4,  8,  0, 2, &zones_set12, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MPG EDGE WIFI DDR4
    { 0x7D32, nullptr,                              1,  0,  0, 1, &zones_set10, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MAG Z690 TOMAHAWK WIFI DDR4
    { 0x7D33, nullptr,                              6,  0,  0, 2, &zones_set5,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // PRO Z790-VC WIFI
    { 0x7D36, nullptr,                              6,  0,  0, 2, &zones_set5,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // PRO Z690-P DDR4
    { 0x7D37, nullptr,                              6,  0,  0, 2, &zones_set11, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // PRO B760M-VC WIFI (MS-7D37) - fans on JRAINBOW2
    { 0x7D38, nullptr,                              0,  0,  0, 1, &zones_set1,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MEG Z590 UNIFY-X
    { 0x7D40, nullptr,                              0,  0,  0, 1, &zones_set5,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MPG B760i EDGE WIFI DDR4
    { 0x7D41, nullptr,                              6,  0,  0, 2, &zones_set13, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MAG B660M TOMAHAWK WIFI DDR4
    { 0x7D42, nullptr,                              6,  0,  0, 2, &zones_set11, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MAG B660 MORTAR WIFI DDR4
    { 0x7D43, nullptr,                              0,  0,  0, 2, &zones_set11, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // PRO B660M-A WIFI DDR4
    { 0x7D46, nullptr,                              0,  1,  1, 0, &zones_set11, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // PRO H610M-G DDR4
    { 0x7D50, nullptr,                              6, 12,  0, 1, &zones_set3,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MEG X570S ACE MAX
    { 0x7D51, nullptr,                              6,  0,  0, 2, &zones_set1,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MEG X570S UNIFY-X MAX
    { 0x7D52, nullptr,                              6, 14,  0, 1, &zones_set3,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MPG X570S CARBON EK X
    { 0x7D53, nullptr,                              6,  0,  0, 2, &zones_set0,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MPG X570S EDGE MAX WIFI
    { 0x7D54, nullptr,                              6,  0,  0, 2, &zones_set0,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MAG X570S TOMAHAWK MAX WIFI
    { 0x7D59, nullptr,                              0,  0,  0, 2, &zones_set11, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // PRO B660-A DDR4
    { 0x7D67, nullptr,                              0,  0,  0, 1, &zones_set11, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // PRO X670-P WIFI
    { 0x7D69, nullptr,                              9,  2,  4, 1, &zones_set15, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MEG X670E ACE
    { 0x7D70, nullptr,                              0,  6,  0, 1, &zones_set9,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MPG X670E Carbon WIFI
    { 0x7D73, nullptr,                              1,  0,  0, 0, &zones_set6,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MPG B650I EDGE WIFI
    { 0x7D74, nullptr,                              0,  6,  0, 1, &zones_set18, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MPG B650 CARBON WIFI
    { 0x7D75, nullptr,                              0,  0,  0, 2, &zones_set13, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MAG B650 TOMAHAWK WIFI
    { 0x7D76, nullptr,                              0,  0,  0, 2, &zones_set13, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MAG B650M MORTAR WIFI
    { 0x7D77, nullptr,                              0,  0,  0, 2, &zones_set13, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // PRO B650M-A WIFI
    { 0x7D78, nullptr,                              0,  0,  0, 2, &zones_set13, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // PRO B650-P WIFI
    { 0x7D86, nullptr,                              0, 18,  4, 1, &zones_set16, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MEG Z790 ACE
    { 0x7D88, nullptr,                              0,  0,  0, 1, &zones_set20, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MPG Z790-S WIFI
    { 0x7D89, nullptr,                              0,  6,  0, 1, &zones_set18, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MPG Z790 CARBON WIFI
    { 0x7D90, nullptr,                              0,  0,  0, 1, &zones_set11, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // B760M BOMBER DDR4
    { 0x7D91, nullptr,                              1,  0,  0, 1, &zones_set10, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MAG Z790 TOMAHAWK WIFI
    { 0x7D93, nullptr,                              6,  0,  0, 1, &zones_set2,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // Z790 GAMING PRO WIFI
    { 0x7D96, nullptr,                              0,  0,  0, 2, &zones_set13, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MAG B760 TOMAHAWK WIFI DDR5
    { 0x7D97, nullptr,                              6,  0,  0, 2, &zones_set11, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MAG B660 MORTAR MAX WIFI DDR4
    { 0x7D98, nullptr,                              0,  0,  0, 1, &zones_set11, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // PRO B760-P WIFI DDR4
    { 0x7D99, nullptr,                              6,  0,  0, 1, &zones_set11, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // PRO B760M-A WIFI DDR4
    { 0x7E01, nullptr,                              0,  0,  0, 1, &zones_set11, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MAG B760M MORTAR MAX
    { 0x7E03, nullptr,                              6,  0,  0, 0, &zones_set6,  MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MPG Z790I EDGE WIFI
    { 0x7E06, nullptr,                              0,  0,  0, 2, &zones_set11, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // PRO Z790-P WIFI DDR4
    { 0x7E07, nullptr,                              0,  0,  0, 2, &zones_set10, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // PRO Z790-A WIFI DDR4
    { 0x7E09, nullptr,                              0,  0,  0, 0, &zones_set19, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // B650M PROJECT ZERO
    { 0x7E10, nullptr,                              0,  6,  0, 2, &zones_set17, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MPG B650 EDGE WIFI
    { 0x7E12, nullptr,                              0,  0,  0, 2, &zones_set13, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MAG X670E TOMAHAWK WIFI
    { 0x0076, nullptr,                              0,  0,  0, 2, &zones_set13, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // MAG X670E TOMAHAWK WIFI (Common PID)
    { 0x7E16, nullptr,                              0,  0,  0, 2, &zones_set13, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // X670E GAMING PLUS WIFI
    { 0x7E24, nullptr,                              0,  0,  0, 2, &zones_set13, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // B650M GAMING PLUS WIFI
    { 0x7E26, nullptr,                              0,  0,  0, 2, &zones_set13, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // B650 GAMING PLUS WIFI
    { 0x7E27, nullptr,                              0,  0,  0, 2, &zones_set13, MSIMotherboard185Controller::DIRECT_MODE_PER_LED },       // PRO B650M-P
    { 0x7E28, nullptr,                              6,  0,  0, 2, &zones_set21, MSIMotherboard185Controller::DIRECT_MODE_ZONE_BASED },    // MSI PRO A620M-B (MS-7E28)
};


FeaturePacket_185 enable_per_led_msg;

Color* per_led_onboard_leds;
Color* per_led_jpipe1;
Color* per_led_jpipe2;
Color* per_led_jrgb;
Color* per_led_jrainbow1_sync;
Color* per_led_jrainbow2_sync;
Color* per_led_jcorsair_sync;

/*-----------------------------------------------------*\
| Static helper to check if a zone is supported by the  |
| given supported_zones vector                          |
\*-----------------------------------------------------*/
bool MSIMotherboard185Controller::IsZoneSupported(MSI_ZONE zone, const std::vector<MSI_ZONE>* supported_zones)
{
    if(supported_zones == nullptr)
    {
        return false;
    }
    return std::find(supported_zones->begin(), supported_zones->end(), zone) != supported_zones->end();
}

MSIMotherboard185Controller::MSIMotherboard185Controller(hid_device* handle, const char* path, unsigned short pid, std::string dev_name)
{
    dev         = handle;
    location    = path;
    name        = dev_name;

    /*-----------------------------------------------------*\
    | Perform initial reads - get the controller's firmware |
    | version and then read the controller's active         |
    | configuration data                                    |
    \*-----------------------------------------------------*/
    ReadFwVersion();
    ReadConfigurationData();

    /*-----------------------------------------------------*\
    | Common PID boards embed their virtual PID in the      |
    | serial number, extract it                             |
    \*-----------------------------------------------------*/
    if(pid == MSI_USB_PID_COMMON)
    {
        std::string pidStr(GetSerial().substr(0, 4));
        pid = std::stoi(pidStr, nullptr, 16);
    }

    /*-----------------------------------------------------*\
    | Determine if this board has mixed-mode support        |
    \*-----------------------------------------------------*/
    mixed_mode_support = false;

    if(pid >= 0x7D03)
    {
        mixed_mode_support = true;
    }

    /*-----------------------------------------------------*\
    | The last byte of the packet is the save flag.  This   |
    | byte is stored as a zero in the data flash, but is    |
    | set to 1 at the protocol level to tell the firmware   |
    | to save the active configuration to flash.            |
    \*-----------------------------------------------------*/
    configuration_data.save_data = 0;

    configuration_data.on_board_led.colorFlags = MSI_COLOR_FLAG_RAINBOW | MSI_COLOR_FLAG_SYNC_SETTING_ONBOARD; // always enable onboard sync flag to have expected zone control

    /*-----------------------------------------------------*\
    | Search for the board configuration matching this      |
    | board.                                                |
    \*-----------------------------------------------------*/
    const mystic_light_185_config* board_config = nullptr;

    /*-----------------------------------------------------*\
    | First pass: look for a config that matches both PID   |
    | and board name (when name is specified in the config) |
    \*-----------------------------------------------------*/
    for(std::size_t i = 0; i < NUMOF_CONFIGS; ++i)
    {
        if(board_configs[i].pid == pid && board_configs[i].name != nullptr)
        {
            if(name.find(board_configs[i].name) != std::string::npos)
            {
                board_config = &board_configs[i];
                break;
            }
        }
    }

    /*-----------------------------------------------------*\
    | Second pass: fall back to PID-only match for entries  |
    | without a name filter (backward compatible)           |
    \*-----------------------------------------------------*/
    if(board_config == nullptr)
    {
        for(std::size_t i = 0; i < NUMOF_CONFIGS; ++i)
        {
            if(board_configs[i].pid == pid && board_configs[i].name == nullptr)
            {
                board_config = &board_configs[i];
                break;
            }
        }
    }

    /*-----------------------------------------------------*\
    | Fill in the number of each zone type based on the     |
    | board configuration                                   |
    \*-----------------------------------------------------*/
    if(board_config != nullptr)
    {
        numof_onboard_leds = board_config->numof_onboard_leds;
        numof_pipe1_leds   = board_config->numof_pipe1_leds;
        numof_pipe2_leds   = board_config->numof_pipe2_leds;
        numof_JRGBs        = board_config->numof_JRGBs;
        supported_zones    = board_config->supported_zones;
        per_led_mode       = board_config->per_led_mode;
    }
    else
    {
        numof_onboard_leds = 6;
        numof_pipe1_leds   = 1;
        numof_pipe2_leds   = 1;
        numof_JRGBs        = 2;
        supported_zones    = &all_zones;
        per_led_mode       = DIRECT_MODE_DISABLED;
    }

    /*-----------------------------------------------------*\
    | Initialize per-LED data                               |
    \*-----------------------------------------------------*/
    std::memset(per_led_data_onboard_and_sync.leds, 0, sizeof(Color) * NUMOF_PER_LED_MODE_LEDS);
    std::memset(per_led_data_jrainbow1.leds, 0, sizeof(Color) * NUMOF_PER_LED_MODE_LEDS);
    per_led_data_jrainbow1.hdr1 = 4;
    std::memset(per_led_data_jrainbow2.leds, 0, sizeof(Color) * NUMOF_PER_LED_MODE_LEDS);
    per_led_data_jrainbow2.hdr1 = 4;
    per_led_data_jrainbow2.hdr2 = 1;
    std::memset(per_led_data_jcorsair.leds, 0, sizeof(Color) * NUMOF_PER_LED_MODE_LEDS);
    per_led_data_jcorsair.hdr1  = 5;

    per_led_onboard_leds   = per_led_data_onboard_and_sync.leds;
    per_led_jpipe1         = per_led_onboard_leds + numof_onboard_leds;
    per_led_jpipe2         = per_led_jpipe1 + numof_pipe1_leds;
    per_led_jrgb           = per_led_jpipe2 + numof_pipe2_leds;
    per_led_jrainbow1_sync = per_led_jrgb + numof_JRGBs;
    per_led_jrainbow2_sync = per_led_jrainbow1_sync + SYNC_PER_LED_MODE_JRAINBOW_LED_COUNT;
    per_led_jcorsair_sync  = per_led_jrainbow2_sync + SYNC_PER_LED_MODE_JRAINBOW_LED_COUNT;

    no_onboards  = true;
    no_jrainbow1 = true;
    no_jrainbow2 = true;
    no_jcorsair  = true;

    for(std::size_t i = 0; i < supported_zones->size(); ++i)
    {
        switch((*supported_zones)[i])
        {
            case MSI_ZONE_ON_BOARD_LED_0:
                no_onboards = false;
                break;

            case MSI_ZONE_J_RAINBOW_1:
                no_jrainbow1 = false;
                break;

            case MSI_ZONE_J_RAINBOW_2:
                no_jrainbow2 = false;
                break;

            case MSI_ZONE_J_CORSAIR:
            case MSI_ZONE_J_RAINBOW_3:
                no_jcorsair = false;
                break;

            default:
                break;
        }
    }

    /*-----------------------------------------------------*\
    | Set up per-LED switching message for synchronized     |
    | mode.                                                 |
    \*-----------------------------------------------------*/
    enable_per_led_msg.j_rgb_1.speedAndBrightnessFlags              = 0x08;
    enable_per_led_msg.j_rgb_1.colorFlags                           = 0x80;
    enable_per_led_msg.j_pipe_1.speedAndBrightnessFlags             = 0x2A;
    enable_per_led_msg.j_pipe_1.colorFlags                          = 0x80;
    enable_per_led_msg.j_pipe_2.speedAndBrightnessFlags             = 0x2A;
    enable_per_led_msg.j_pipe_2.colorFlags                          = 0x80;
    enable_per_led_msg.j_rainbow_1.speedAndBrightnessFlags          = 0x29;
    enable_per_led_msg.j_rainbow_1.colorFlags                       = 0x80;
    enable_per_led_msg.j_rainbow_1.cycle_or_led_num                 = 0x28;
    enable_per_led_msg.j_rainbow_2.speedAndBrightnessFlags          = 0x29;
    enable_per_led_msg.j_rainbow_2.colorFlags                       = 0x80;
    enable_per_led_msg.j_rainbow_2.cycle_or_led_num                 = 0x28;
    enable_per_led_msg.j_corsair.fan_flags                          = 0x29;
    enable_per_led_msg.j_corsair.corsair_quantity                   = 0x00;
    enable_per_led_msg.j_corsair.padding[2]                         = 0x82;
    enable_per_led_msg.j_corsair.is_individual                      = 0x78;
    enable_per_led_msg.j_corsair_outerll120.speedAndBrightnessFlags = 0x28;
    enable_per_led_msg.j_corsair_outerll120.colorFlags              = 0x80;
    enable_per_led_msg.on_board_led.effect                          = MSI_DIRECT_MODE;
    enable_per_led_msg.on_board_led.speedAndBrightnessFlags         = 0x29 | MSI_SPEED_BRIGHTNESS_FLAG_SYNC_SETTING_JRGB;
    enable_per_led_msg.on_board_led.colorFlags                      = PER_LED_FULL_SYNC_MODE;
    enable_per_led_msg.on_board_led_1.speedAndBrightnessFlags       = 0x28;
    enable_per_led_msg.on_board_led_1.colorFlags                    = 0x80;
    enable_per_led_msg.on_board_led_2.speedAndBrightnessFlags       = 0x28;
    enable_per_led_msg.on_board_led_2.colorFlags                    = 0x80;
    enable_per_led_msg.on_board_led_3.speedAndBrightnessFlags       = 0x28;
    enable_per_led_msg.on_board_led_3.colorFlags                    = 0x80;
    enable_per_led_msg.on_board_led_4.speedAndBrightnessFlags       = 0x28;
    enable_per_led_msg.on_board_led_4.colorFlags                    = 0x80;
    enable_per_led_msg.on_board_led_5.speedAndBrightnessFlags       = 0x28;
    enable_per_led_msg.on_board_led_5.colorFlags                    = 0x80;
    enable_per_led_msg.on_board_led_6.speedAndBrightnessFlags       = 0x28;
    enable_per_led_msg.on_board_led_6.colorFlags                    = 0x80;
    enable_per_led_msg.on_board_led_7.speedAndBrightnessFlags       = 0x28;
    enable_per_led_msg.on_board_led_7.colorFlags                    = 0x80;
    enable_per_led_msg.on_board_led_8.speedAndBrightnessFlags       = 0x28;
    enable_per_led_msg.on_board_led_8.colorFlags                    = 0x80;
    enable_per_led_msg.on_board_led_9.speedAndBrightnessFlags       = 0x28;
    enable_per_led_msg.on_board_led_9.colorFlags                    = 0x80;
    enable_per_led_msg.j_rgb_2.speedAndBrightnessFlags              = 0x2A;
    enable_per_led_msg.j_rgb_2.colorFlags                           = 0x80;

    /*-----------------------------------------------------*\
    | Initialize zone based per LED data                    |
    \*-----------------------------------------------------*/
    zone_based_per_led_data.j_rgb_1.speedAndBrightnessFlags         = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.j_rgb_1.colorFlags                      = BITSET(zone_based_per_led_data.j_rgb_1.colorFlags, true, 7u);
    zone_based_per_led_data.j_pipe_1.speedAndBrightnessFlags        = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.j_pipe_1.colorFlags                     = BITSET(zone_based_per_led_data.j_pipe_1.colorFlags, true, 7u);
    zone_based_per_led_data.j_pipe_2.speedAndBrightnessFlags        = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.j_pipe_2.colorFlags                     = BITSET(zone_based_per_led_data.j_pipe_2.colorFlags, true, 7u);
    zone_based_per_led_data.j_rainbow_1.speedAndBrightnessFlags     = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.j_rainbow_1.colorFlags                  = BITSET(zone_based_per_led_data.j_rainbow_1.colorFlags, true, 7u);
    zone_based_per_led_data.j_rainbow_2.speedAndBrightnessFlags     = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.j_rainbow_2.colorFlags                  = BITSET(zone_based_per_led_data.j_rainbow_2.colorFlags, true, 7u);
    zone_based_per_led_data.on_board_led.speedAndBrightnessFlags    = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.on_board_led.colorFlags                 = BITSET(zone_based_per_led_data.on_board_led.colorFlags, true, 7u);
    zone_based_per_led_data.on_board_led_1.speedAndBrightnessFlags  = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.on_board_led_1.colorFlags               = BITSET(zone_based_per_led_data.on_board_led_1.colorFlags, true, 7u);
    zone_based_per_led_data.on_board_led_2.speedAndBrightnessFlags  = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.on_board_led_2.colorFlags               = BITSET(zone_based_per_led_data.on_board_led_2.colorFlags, true, 7u);
    zone_based_per_led_data.on_board_led_3.speedAndBrightnessFlags  = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.on_board_led_3.colorFlags               = BITSET(zone_based_per_led_data.on_board_led_3.colorFlags, true, 7u);
    zone_based_per_led_data.on_board_led_4.speedAndBrightnessFlags  = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.on_board_led_4.colorFlags               = BITSET(zone_based_per_led_data.on_board_led_4.colorFlags, true, 7u);
    zone_based_per_led_data.on_board_led_5.speedAndBrightnessFlags  = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.on_board_led_5.colorFlags               = BITSET(zone_based_per_led_data.on_board_led_5.colorFlags, true, 7u);
    zone_based_per_led_data.on_board_led_6.speedAndBrightnessFlags  = MSI_BRIGHTNESS_LEVEL_100 << 2 << 2;
    zone_based_per_led_data.on_board_led_6.colorFlags               = BITSET(zone_based_per_led_data.on_board_led_6.colorFlags, true, 7u);
    zone_based_per_led_data.on_board_led_7.speedAndBrightnessFlags  = MSI_BRIGHTNESS_LEVEL_100;
    zone_based_per_led_data.on_board_led_7.colorFlags               = BITSET(zone_based_per_led_data.on_board_led_7.colorFlags, true, 7u);
    zone_based_per_led_data.on_board_led_8.speedAndBrightnessFlags  = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.on_board_led_8.colorFlags               = BITSET(zone_based_per_led_data.on_board_led_8.colorFlags, true, 7u);
    zone_based_per_led_data.on_board_led_9.speedAndBrightnessFlags  = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.on_board_led_9.colorFlags               = BITSET(zone_based_per_led_data.on_board_led_9.colorFlags, true, 7u);
    zone_based_per_led_data.j_rgb_2.speedAndBrightnessFlags         = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.j_rgb_2.colorFlags                      = BITSET(zone_based_per_led_data.j_rgb_2.colorFlags, true, 7u);

    direct_mode             = false;
    direct_mode_configured  = false;
    sync_direct_mode        = true;
}

MSIMotherboard185Controller::~MSIMotherboard185Controller()
{
    hid_close(dev);
}

void MSIMotherboard185Controller::SetMode
    (
    MSI_ZONE        zone,
    MSI_MODE        mode,
    MSI_SPEED       speed,
    MSI_BRIGHTNESS  brightness,
    bool            rainbow_color
    )
{
    /*-----------------------------------------------------*\
    | If the target zone is not supported by this board,    |
    | return without modifying the zone's data              |
    \*-----------------------------------------------------*/
    if(!IsZoneSupported(zone, supported_zones))
    {
        return;
    }

    if((per_led_mode == DIRECT_MODE_ZONE_BASED) && (zone > MSI_ZONE_ON_BOARD_LED_0))
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Get the zone data pointer within the configuration    |
    | data buffer                                           |
    \*-----------------------------------------------------*/
    ZoneData* zone_data = GetZoneData(configuration_data, zone);

    /*-----------------------------------------------------*\
    | Return if zone is invalid                             |
    \*-----------------------------------------------------*/
    if(zone_data == nullptr)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Update the zone's mode and mode parameters            |
    \*-----------------------------------------------------*/
    zone_data->effect                    = mode;
    zone_data->speedAndBrightnessFlags   = ((brightness << MSI_SPEED_BRIGHTNESS_FLAG_BRIGHTNESS_SHIFT) & MSI_SPEED_BRIGHTNESS_FLAG_BRIGHTNESS_MASK)
                                         | ((speed << MSI_SPEED_BRIGHTNESS_FLAG_SPEED_SHIFT)           & MSI_SPEED_BRIGHTNESS_FLAG_SPEED_MASK);
    zone_data->colorFlags                = (rainbow_color ? MSI_COLOR_FLAG_RAINBOW : 0);

    /*-----------------------------------------------------*\
    | J_RAINBOW_3 zone needs padding of 4                   |
    \*-----------------------------------------------------*/
    zone == MSI_ZONE_J_RAINBOW_3 ? zone_data->padding = 4 : zone_data->padding = 0;

    /*-----------------------------------------------------*\
    | Get the zone data pointer to the on-board LED 0 zone  |
    \*-----------------------------------------------------*/
    ZoneData* on_board_zone = GetZoneData(configuration_data, MSI_ZONE_ON_BOARD_LED_0);

    /*-----------------------------------------------------*\
    | If the zone is JRGB or JPIPE and the board has no     |
    | onboard LEDs, set the onboard zone to this zone's     |
    | settings                                              |
    \*-----------------------------------------------------*/
    if(no_onboards && ((zone == MSI_ZONE_J_RGB_1) || (zone == MSI_ZONE_J_RGB_2) || (zone == MSI_ZONE_J_PIPE_1) || (zone == MSI_ZONE_J_PIPE_2)))
    {
        on_board_zone->effect                  = zone_data->effect;
        on_board_zone->speedAndBrightnessFlags = zone_data->speedAndBrightnessFlags;
        on_board_zone->colorFlags              = zone_data->colorFlags;
        on_board_zone->colorFlags             |= MSI_COLOR_FLAG_SYNC_SETTING_ONBOARD;
        on_board_zone->padding                 = 0x00;
    }

    /*-----------------------------------------------------*\
    | For modes above MSI_MODE_LIGHTNING, set the onboard   |
    | zone to sync with JRGB, JPIPE1, and JPIPE2            |
    \*-----------------------------------------------------*/
    if(mode > MSI_MODE_LIGHTNING)
    {
        on_board_zone->speedAndBrightnessFlags |= MSI_SPEED_BRIGHTNESS_FLAG_SYNC_SETTING_JRGB;
        on_board_zone->colorFlags              |= (MSI_COLOR_FLAG_SYNC_SETTING_JPIPE1 | MSI_COLOR_FLAG_SYNC_SETTING_JPIPE2);
    }
    else
    {
        on_board_zone->speedAndBrightnessFlags &= ~MSI_SPEED_BRIGHTNESS_FLAG_SYNC_SETTING_JRGB;
        on_board_zone->colorFlags              &= ~(MSI_COLOR_FLAG_SYNC_SETTING_JPIPE1 | MSI_COLOR_FLAG_SYNC_SETTING_JPIPE2);
    }

    /*-----------------------------------------------------*\
    | For modes at or below MSI_MODE_LIGHTNING, set all     |
    | onboard LED zones to match onboard LED zone 0         |
    \*-----------------------------------------------------*/
    if((zone == MSI_ZONE_ON_BOARD_LED_0) && (mode <= MSI_MODE_LIGHTNING))
    {
        for(int i = 0; i < numof_onboard_leds; ++i)
        {
            zone_data = GetZoneData(configuration_data, (MSI_ZONE)((int)MSI_ZONE_ON_BOARD_LED_1 + i));

            if(zone_data != nullptr)
            {
                zone_data->effect                    = mode;
                zone_data->speedAndBrightnessFlags   = ((brightness << MSI_SPEED_BRIGHTNESS_FLAG_BRIGHTNESS_SHIFT) & MSI_SPEED_BRIGHTNESS_FLAG_BRIGHTNESS_MASK)
                                                     | ((speed << MSI_SPEED_BRIGHTNESS_FLAG_SPEED_SHIFT)           & MSI_SPEED_BRIGHTNESS_FLAG_SPEED_MASK);
                zone_data->colorFlags                = (rainbow_color ? MSI_COLOR_FLAG_RAINBOW : 0);
                zone_data->padding                   = 0x00;
            }
        }
    }
}

std::string MSIMotherboard185Controller::GetDeviceName()
{
    return name;
}

std::string MSIMotherboard185Controller::GetFWVersion()
{
    return std::string("AP/LD ").append(version_APROM).append(" / ").append(version_LDROM);
}

std::string MSIMotherboard185Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string MSIMotherboard185Controller::GetSerial()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

bool MSIMotherboard185Controller::ReadConfigurationData()
{
    /*-----------------------------------------------------*\
    | Read packet from hardware, return true if successful  |
    \*-----------------------------------------------------*/
    int ret = hid_get_feature_report(dev, (unsigned char*)&configuration_data, sizeof(configuration_data));

    /*-----------------------------------------------------*\
    | Log the configuration data in hex at TRACE level      |
    \*-----------------------------------------------------*/
    if(LogManager::get()->GetLogLevel() >= LL_TRACE)
    {
        LOG_TRACE("[MSIMotherboard185Controller] Read configuration data packet (%zu bytes):", sizeof(configuration_data));
        for(size_t i = 0; i < sizeof(configuration_data); i += 16)
        {
            std::string hex_line = "";
            for(size_t j = i; j < i + 16 && j < sizeof(configuration_data); j++)
            {
                char buf[4];
                snprintf(buf, sizeof(buf), "%02X ", ((unsigned char*)&configuration_data)[j]);
                hex_line += buf;
            }
            LOG_TRACE("    %04zX: %s", i, hex_line.c_str());
        }
    }

    return(ret == sizeof configuration_data);
}

bool MSIMotherboard185Controller::Update
    (
    bool save
    )
{
    /*-----------------------------------------------------*\
    | Send packet to hardware, return true if successful    |
    \*-----------------------------------------------------*/
    if(direct_mode)
    {
        if(per_led_mode == DIRECT_MODE_PER_LED)
        {
            /*---------------------------------------------*\
            | Skip sending update if there are no LEDs      |
            | configured                                    |
            \*---------------------------------------------*/
            bool has_active_per_led_data = false;

            if(!no_onboards)
            {
                has_active_per_led_data = true;
            }
            if(!no_jrainbow1)
            {
                has_active_per_led_data = true;
            }
            if(!no_jrainbow2)
            {
                has_active_per_led_data = true;
            }
            if(!no_jcorsair)
            {
                has_active_per_led_data = true;
            }

            if(!has_active_per_led_data)
            {
                return false;
            }

            /*---------------------------------------------*\
            | Ensure the enable per-LED mode message has    |
            | been sent before sending the per-LED data     |
            \*---------------------------------------------*/
            if(!direct_mode_configured)
            {
                if(!SelectPerLedProtocol())
                {
                    return false;
                }
            }

            if(sync_direct_mode)
            {
                return(hid_send_feature_report(dev, (unsigned char*)&per_led_data_onboard_and_sync, sizeof(per_led_data_onboard_and_sync)) == sizeof(per_led_data_onboard_and_sync));
            }
            else
            {
                if(!no_jrainbow1)
                {
                    hid_send_feature_report(dev, (unsigned char*)&per_led_data_jrainbow1, sizeof(per_led_data_jrainbow1));
                    std::this_thread::sleep_for(13ms);
                }
                if(!no_jrainbow2)
                {
                    hid_send_feature_report(dev, (unsigned char*)&per_led_data_jrainbow2, sizeof(per_led_data_jrainbow2));
                    std::this_thread::sleep_for(13ms);
                }
                if(!no_jcorsair)
                {
                    hid_send_feature_report(dev, (unsigned char*)&per_led_data_jcorsair, sizeof(per_led_data_jcorsair));
                    std::this_thread::sleep_for(13ms);
                }
                return(hid_send_feature_report(dev, (unsigned char*)&per_led_data_onboard_and_sync, sizeof(per_led_data_onboard_and_sync)) == sizeof(per_led_data_onboard_and_sync));
            }
        }
        else
        {
            return(hid_send_feature_report(dev, (unsigned char*)&zone_based_per_led_data, sizeof(zone_based_per_led_data)) == sizeof(zone_based_per_led_data));
        }
    }
    else
    {
        /*-------------------------------------------------*\
        | Save a copy of the new configuration data         |
        \*-------------------------------------------------*/
        FeaturePacket_185 new_configuration = configuration_data;
        memcpy((unsigned char*)&new_configuration, (unsigned char*)&configuration_data, sizeof(configuration_data));

        /*-------------------------------------------------*\
        | Read the existing configuration data              |
        \*-------------------------------------------------*/
        ReadConfigurationData();

        /*-------------------------------------------------*\
        | Set the save flag on the old data                 |
        \*-------------------------------------------------*/
        configuration_data.save_data = save;

        /*-------------------------------------------------*\
        | First, send a copy of the existing configuration  |
        | data                                              |
        \*-------------------------------------------------*/
        hid_send_feature_report(dev, (unsigned char*)&configuration_data, sizeof(configuration_data));

        /*-------------------------------------------------*\
        | Second, restore the new configuration data into   |
        | the buffer and then send it.  MSI software on     |
        | Windows sends the old data followed by new data   |
        | so this mimics that flow.                         |
        \*-------------------------------------------------*/
        memcpy((unsigned char*)&new_configuration, (unsigned char*)&new_configuration, sizeof(new_configuration));

        /*-------------------------------------------------*\
        | Set the save flag on the new data                 |
        \*-------------------------------------------------*/
        configuration_data.save_data = save;

        return(hid_send_feature_report(dev, (unsigned char*)&configuration_data, sizeof(configuration_data)) == sizeof(configuration_data));
    }
}

void MSIMotherboard185Controller::SetZoneColor
    (
    MSI_ZONE        zone,
    unsigned char   red1,
    unsigned char   grn1,
    unsigned char   blu1,
    unsigned char   red2,
    unsigned char   grn2,
    unsigned char   blu2
    )
{
    /*-----------------------------------------------------*\
    | If the target zone is not supported by this board,    |
    | return without modifying the zone's data              |
    \*-----------------------------------------------------*/
    if(!IsZoneSupported(zone, supported_zones))
    {
        return;
    }

    if((per_led_mode == DIRECT_MODE_ZONE_BASED) && (zone > MSI_ZONE_ON_BOARD_LED_0))
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Get the zone data pointer within the configuration    |
    | data buffer                                           |
    \*-----------------------------------------------------*/
    ZoneData* zone_data = GetZoneData(configuration_data, zone);

    /*-----------------------------------------------------*\
    | Return if zone is invalid                             |
    \*-----------------------------------------------------*/
    if(zone_data == nullptr)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Set the colors in the zone data                       |
    \*-----------------------------------------------------*/
    zone_data->color.R  = red1;
    zone_data->color.G  = grn1;
    zone_data->color.B  = blu1;
    zone_data->color2.R = red2;
    zone_data->color2.G = grn2;
    zone_data->color2.B = blu2;

    /*-----------------------------------------------------*\
    | If the zone is JRGB or JPIPE and the board has no     |
    | onboard LEDs, set the onboard zone's colors to this   |
    | zone's colors                                         |
    \*-----------------------------------------------------*/
    if(no_onboards && ((zone == MSI_ZONE_J_RGB_1) || (zone == MSI_ZONE_J_RGB_2) || (zone == MSI_ZONE_J_PIPE_1) || (zone == MSI_ZONE_J_PIPE_2)))
    {
        ZoneData* on_board_zone = GetZoneData(configuration_data, MSI_ZONE_ON_BOARD_LED_0);

        on_board_zone->color.R  = red1;
        on_board_zone->color.G  = grn1;
        on_board_zone->color.B  = blu1;
        on_board_zone->color2.R = red2;
        on_board_zone->color2.G = grn2;
        on_board_zone->color2.B = blu2;
    }

    /*-----------------------------------------------------*\
    | Set all onboard LED zones' colors to match onboard    |
    | LED 0                                                 |
    \*-----------------------------------------------------*/
    if(zone == MSI_ZONE_ON_BOARD_LED_0)
    {
        for(int i = 0; i < numof_onboard_leds; ++i)
        {
            zone_data = GetZoneData(configuration_data, (MSI_ZONE)((int)MSI_ZONE_ON_BOARD_LED_1 + i));

            if(zone_data != nullptr)
            {
                zone_data->color.R  = red1;
                zone_data->color.G  = grn1;
                zone_data->color.B  = blu1;
                zone_data->color2.R = red2;
                zone_data->color2.G = grn2;
                zone_data->color2.B = blu2;
            }
        }
    }
}

void MSIMotherboard185Controller::SetLedColor
    (
    MSI_ZONE        zone,
    int             index,
    unsigned char   red,
    unsigned char   grn,
    unsigned char   blu
    )
{
    /*-----------------------------------------------------*\
    | If the target zone is not supported by this board,    |
    | return without modifying the zone's data              |
    \*-----------------------------------------------------*/
    if(!IsZoneSupported(zone, supported_zones))
    {
        return;
    }

    /*-----------------------------------------------------*\
    | If this board supports Per-LED direct mode, update    |
    | the per-LED packet buffers                            |
    \*-----------------------------------------------------*/
    if(per_led_mode == DIRECT_MODE_PER_LED)
    {
        Color* zone_data = GetPerLedZoneData(zone);

        if(zone_data != nullptr)
        {
            int maxSize = (int)GetMaxDirectLeds(zone);

            if(sync_direct_mode)
            {
                switch(zone)
                {
                    case MSI_ZONE_J_RAINBOW_1:
                    case MSI_ZONE_J_RAINBOW_2:
                        maxSize = SYNC_PER_LED_MODE_JRAINBOW_LED_COUNT;
                        break;

                    case MSI_ZONE_J_RAINBOW_3:
                    case MSI_ZONE_J_CORSAIR:
                        maxSize = SYNC_PER_LED_MODE_CORSAIR_LED_COUNT;
                        break;

                    default:
                        break;
                }
            }

            if(index < maxSize)
            {
                zone_data[index].R = red;
                zone_data[index].G = grn;
                zone_data[index].B = blu;
            }
        }
    }
    /*-----------------------------------------------------*\
    | Otherwise, colors are set per zone.                   |
    \*-----------------------------------------------------*/
    else
    {
        /*-------------------------------------------------*\
        | The JRAINBOW and JPIPE zones have a single LED    |
        | each at the RGBController layer, so index must be |
        | 0.                                                |
        \*-------------------------------------------------*/
        if(((zone == MSI_ZONE_J_RAINBOW_1) || (zone == MSI_ZONE_J_RAINBOW_2) || (zone == MSI_ZONE_J_PIPE_1) || (zone == MSI_ZONE_J_PIPE_2)) && (index != 0))
        {
            return;
        }
        /*-------------------------------------------------*\
        | The onboard LEDs are treated as a single zone by  |
        | the RGBController layer, so LED index is used to  |
        | select which onboard LED zone in the              |
        | configuration data to update with the given color.|
        \*-------------------------------------------------*/
        if(zone >= MSI_ZONE_ON_BOARD_LED_0)
        {
            zone = (MSI_ZONE)((int)zone + index + 1);
        }

        ZoneData *zone_data = GetZoneData(zone_based_per_led_data, zone);

        if(zone_data == nullptr)
        {
            return;
        }

        zone_data->color.R  = red;
        zone_data->color.G  = grn;
        zone_data->color.B  = blu;
        zone_data->color2.R = red;
        zone_data->color2.G = grn;
        zone_data->color2.B = blu;
    }
}

ZoneData *MSIMotherboard185Controller::GetZoneData
    (
    FeaturePacket_185&  data_packet,
    MSI_ZONE            zone
    )
{
    switch(zone)
    {
        case MSI_ZONE_J_RGB_1:
            return &data_packet.j_rgb_1;
        case MSI_ZONE_J_RGB_2:
            if(mixed_mode_support)
            {
                return &data_packet.on_board_led_6;
            }
            else
            {
                return &data_packet.j_rgb_2;
            }
        case MSI_ZONE_J_RAINBOW_1:
            return &data_packet.j_rainbow_1;
        case MSI_ZONE_J_RAINBOW_2:
            return &data_packet.j_rainbow_2;
        case MSI_ZONE_J_RAINBOW_3:
            return (ZoneData*)&data_packet.j_corsair;
        case MSI_ZONE_J_PIPE_1:
            return &data_packet.j_pipe_1;
        case MSI_ZONE_J_PIPE_2:
            return &data_packet.j_pipe_2;
        case MSI_ZONE_ON_BOARD_LED_0:
            return &data_packet.on_board_led;
        case MSI_ZONE_ON_BOARD_LED_1:
            return &data_packet.on_board_led_1;
        case MSI_ZONE_ON_BOARD_LED_2:
            return &data_packet.on_board_led_2;
        case MSI_ZONE_ON_BOARD_LED_3:
            return &data_packet.on_board_led_3;
        case MSI_ZONE_ON_BOARD_LED_4:
            return &data_packet.on_board_led_4;
        case MSI_ZONE_ON_BOARD_LED_5:
            return &data_packet.on_board_led_5;
        case MSI_ZONE_ON_BOARD_LED_6:
            if(mixed_mode_support)
            {
                return &data_packet.j_corsair_outerll120;
            }
            else
            {
                return &data_packet.on_board_led_6;
            }
        case MSI_ZONE_ON_BOARD_LED_7:
            return &data_packet.on_board_led_7;
        case MSI_ZONE_ON_BOARD_LED_8:
            return &data_packet.on_board_led_8;
        case MSI_ZONE_ON_BOARD_LED_9:
            return &data_packet.on_board_led_9;
        case MSI_ZONE_J_CORSAIR_OUTERLL120:
            return &data_packet.j_corsair_outerll120;
        case MSI_ZONE_J_CORSAIR:
            return (ZoneData*)&data_packet.j_corsair;
        default:
            break;
    }

    return nullptr;
}

Color *MSIMotherboard185Controller::GetPerLedZoneData
    (
    MSI_ZONE  zone
    )
{
    switch(zone)
    {
        case MSI_ZONE_J_RAINBOW_1:
            if(sync_direct_mode)
            {
                return per_led_jrainbow1_sync;
            }
            else
            {
                return per_led_data_jrainbow1.leds;
            }
        case MSI_ZONE_J_RAINBOW_2:
            if(sync_direct_mode)
            {
                return per_led_jrainbow2_sync;
            }
            else
            {
                return per_led_data_jrainbow2.leds;
            }
        case MSI_ZONE_ON_BOARD_LED_0:
            return per_led_onboard_leds;
        case MSI_ZONE_J_RAINBOW_3:
        case MSI_ZONE_J_CORSAIR:
            if(sync_direct_mode)
            {
                return per_led_jcorsair_sync;
            }
            else
            {
                return per_led_data_jcorsair.leds;
            }
        case MSI_ZONE_J_RGB_1:
            return per_led_jrgb;
        case MSI_ZONE_J_RGB_2:
            return per_led_jrgb + 1;
        case MSI_ZONE_J_PIPE_1:
            return per_led_jpipe1;
        case MSI_ZONE_J_PIPE_2:
            return per_led_jpipe2;
        default:
            break;
    }

    return nullptr;
}

RainbowZoneData *MSIMotherboard185Controller::GetRainbowZoneData
    (
    MSI_ZONE zone
    )
{
    switch(zone)
    {
        case MSI_ZONE_J_RAINBOW_1:
            return &configuration_data.j_rainbow_1;
        case MSI_ZONE_J_RAINBOW_2:
            return &configuration_data.j_rainbow_2;
        case MSI_ZONE_J_RAINBOW_3:
            return (RainbowZoneData*)&configuration_data.j_corsair;
        case MSI_ZONE_J_CORSAIR:
        default:
            return nullptr;
    }
}

bool MSIMotherboard185Controller::ReadFwVersion()
{
    unsigned char request[64];
    unsigned char response[64];
    int ret_val = 64;

    /*-----------------------------------------------------*\
    | First read the APROM                                  |
    | Checksum also available at report ID 180, with MSB    |
    | stored at index 0x08 and LSB at 0x09                  |
    \*-----------------------------------------------------*/

    /*-----------------------------------------------------*\
    | Zero out buffers                                      |
    \*-----------------------------------------------------*/
    memset(request, 0x00, sizeof(request));
    memset(response, 0x00, sizeof(response));

    /*-----------------------------------------------------*\
    | Set up APROM Firmware Version Request packet          |
    \*-----------------------------------------------------*/
    request[0x00]   = 0x01;
    request[0x01]   = 0xB0;

    /*-----------------------------------------------------*\
    | Fill request from 0x02 to 0x61 with 0xCC              |
    \*-----------------------------------------------------*/
    memset(&request[0x02], 0xCC, sizeof(request) - 2);

    /*-----------------------------------------------------*\
    | Send request and receive response packets             |
    \*-----------------------------------------------------*/
    ret_val &= hid_write(dev, request, 64);
    ret_val &= hid_read(dev, response, 64);

    /*-----------------------------------------------------*\
    | Extract high and low values from response             |
    \*-----------------------------------------------------*/
    unsigned char highValue = response[2] >> 4;
    unsigned char lowValue  = response[2] & 0x0F;

    /*-----------------------------------------------------*\
    | Build firmware string <high>.<low>                    |
    \*-----------------------------------------------------*/
    version_APROM = std::to_string((int)highValue).append(".").append(std::to_string((int)lowValue));

    /*-----------------------------------------------------*\
    | First read the LDROM                                  |
    | Checksum also available at report ID 184, with MSB    |
    | stored at index 0x08 and LSB at 0x09                  |
    \*-----------------------------------------------------*/

    /*-----------------------------------------------------*\
    | Set up LDROM Firmware Version Request packet          |
    \*-----------------------------------------------------*/
    request[0x00]   = 0x01;
    request[0x01]   = 0xB6;

    /*-----------------------------------------------------*\
    | Send request and receive response packets             |
    \*-----------------------------------------------------*/
    ret_val &= hid_write(dev, request, 64);
    ret_val &= hid_read(dev, response, 64);

    /*-----------------------------------------------------*\
    | Extract high and low values from response             |
    \*-----------------------------------------------------*/
    highValue = response[2] >> 4;
    lowValue  = response[2] & 0x0F;

    /*-----------------------------------------------------*\
    | Build firmware string <high>.<low>                    |
    \*-----------------------------------------------------*/
    version_LDROM = std::to_string((int)highValue).append(".").append(std::to_string((int)lowValue));

    /*-----------------------------------------------------*\
    | If return value is zero it means an HID transfer      |
    | failed                                                |
    \*-----------------------------------------------------*/
    return (ret_val > 0);
}

MSI_MODE MSIMotherboard185Controller::GetMode()
{
    if(configuration_data.on_board_led.effect == MSI_DIRECT_MODE)
    {
        direct_mode = true;
        return MSI_MODE_DIRECT_DUMMY;
    }
    else
    {
        return (MSI_MODE)configuration_data.j_rainbow_1.effect;
    }
}

void MSIMotherboard185Controller::GetMode
    (
    MSI_ZONE zone,
    MSI_MODE &mode,
    MSI_SPEED &speed,
    MSI_BRIGHTNESS &brightness,
    bool &rainbow_color,
    unsigned int &color
    )
{
    /*-----------------------------------------------------*\
    | Get the zone data pointer within the configuration    |
    | data buffer                                           |
    \*-----------------------------------------------------*/
    ZoneData *zone_data = GetZoneData(configuration_data, zone);

    /*-----------------------------------------------------*\
    | Return if zone is invalid                             |
    \*-----------------------------------------------------*/
    if(!zone_data)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Update pointers with data                             |
    \*-----------------------------------------------------*/
    mode            = (MSI_MODE)        zone_data->effect;
    speed           = (MSI_SPEED)     ((zone_data->speedAndBrightnessFlags >> MSI_SPEED_BRIGHTNESS_FLAG_SPEED_SHIFT)      & MSI_SPEED_BRIGHTNESS_FLAG_SPEED_MASK);
    brightness      = (MSI_BRIGHTNESS)((zone_data->speedAndBrightnessFlags >> MSI_SPEED_BRIGHTNESS_FLAG_BRIGHTNESS_SHIFT) & MSI_SPEED_BRIGHTNESS_FLAG_BRIGHTNESS_MASK);
    rainbow_color   = (zone_data->colorFlags & MSI_COLOR_FLAG_RAINBOW) == 0 ? true : false;
    color           = ToRGBColor(zone_data->color.R, zone_data->color.G, zone_data->color.B);
}

void MSIMotherboard185Controller::SetZoneLEDCount
    (
    MSI_ZONE        zone,
    unsigned char   led_num
    )
{
    RainbowZoneData *requested_zone = GetRainbowZoneData(zone);

    if(!requested_zone)
    {
        return;
    }

    requested_zone->cycle_or_led_num = led_num;
    SelectPerLedProtocol();
}

void MSIMotherboard185Controller::SetDirectMode
    (
    bool mode
    )
{
    direct_mode             = mode;
    direct_mode_configured  = false;
    SelectPerLedProtocol();
}

size_t MSIMotherboard185Controller::GetMaxDirectLeds
    (
    MSI_ZONE zone
    )
{
    switch(zone)
    {
        case MSI_ZONE_J_RGB_1:
        case MSI_ZONE_J_RGB_2:
            return 1;

        case MSI_ZONE_J_PIPE_1:
            return numof_pipe1_leds;

        case MSI_ZONE_J_PIPE_2:
            return numof_pipe2_leds;

        case MSI_ZONE_J_RAINBOW_1:
            if(per_led_mode == DIRECT_MODE_PER_LED)
            {
                return JRAINBOW1_MAX_LED_COUNT;
            }
            else
            {
                return 1;
            }
        case MSI_ZONE_J_RAINBOW_2:
        case MSI_ZONE_J_RAINBOW_3:
            if(per_led_mode == DIRECT_MODE_PER_LED)
            {
                return JRAINBOW2_MAX_LED_COUNT;
            }
            else
            {
                return 1;
            }

        case MSI_ZONE_J_CORSAIR:
            if(per_led_mode == DIRECT_MODE_PER_LED)
            {
                return JCORSAIR_MAX_LED_COUNT;
            }
            else
            {
                return 1;
            }

        case MSI_ZONE_ON_BOARD_LED_0:
            return numof_onboard_leds;

        default:
            return 1;
    }
}

bool MSIMotherboard185Controller::SelectPerLedProtocol()
{
    unsigned char jrainbow1_size = 0;
    unsigned char jrainbow2_size = 0;
    unsigned char jrainbow3_size = 0;

    RainbowZoneData* zone_data = GetRainbowZoneData(MSI_ZONE_J_RAINBOW_1);

    if(zone_data != nullptr)
    {
        jrainbow1_size = zone_data->cycle_or_led_num;
    }

    zone_data = GetRainbowZoneData(MSI_ZONE_J_RAINBOW_2);

    if(zone_data != nullptr)
    {
        jrainbow2_size = zone_data->cycle_or_led_num;
    }

    zone_data = GetRainbowZoneData(MSI_ZONE_J_RAINBOW_3);

    if(zone_data != nullptr)
    {
        jrainbow3_size = zone_data->cycle_or_led_num;
    }

    sync_direct_mode = true;

    if((jrainbow1_size > SYNC_PER_LED_MODE_JRAINBOW_LED_COUNT) ||
       (jrainbow2_size > SYNC_PER_LED_MODE_JRAINBOW_LED_COUNT) ||
       (jrainbow3_size > SYNC_PER_LED_MODE_CORSAIR_LED_COUNT))
    {
        sync_direct_mode = false;
    }

    if(sync_direct_mode)
    {
        enable_per_led_msg.j_rainbow_1.effect = MSI_MODE_STATIC;
        enable_per_led_msg.j_rainbow_1.cycle_or_led_num = SYNC_PER_LED_MODE_JRAINBOW_LED_COUNT;
        enable_per_led_msg.j_rainbow_2.effect = MSI_MODE_STATIC;
        enable_per_led_msg.j_rainbow_2.cycle_or_led_num = SYNC_PER_LED_MODE_JRAINBOW_LED_COUNT;
        enable_per_led_msg.j_corsair.effect = MSI_MODE_STATIC;
        enable_per_led_msg.j_corsair.is_individual = SYNC_PER_LED_MODE_CORSAIR_LED_COUNT;
        enable_per_led_msg.on_board_led.colorFlags = PER_LED_FULL_SYNC_MODE;
    }
    else
    {
        if(!no_jrainbow1)
        {
            enable_per_led_msg.j_rainbow_1.effect = MSI_DIRECT_MODE;
            enable_per_led_msg.j_rainbow_1.cycle_or_led_num = JRAINBOW1_MAX_LED_COUNT;
        }
        if(!no_jrainbow2)
        {
            enable_per_led_msg.j_rainbow_2.effect = MSI_DIRECT_MODE;
            enable_per_led_msg.j_rainbow_2.cycle_or_led_num = JRAINBOW2_MAX_LED_COUNT;
        }
        if(!no_jcorsair)
        {
            enable_per_led_msg.j_corsair.effect = MSI_DIRECT_MODE;
            enable_per_led_msg.j_corsair.is_individual = JCORSAIR_MAX_LED_COUNT;
        }
        enable_per_led_msg.on_board_led.colorFlags = PER_LED_BASIC_SYNC_MODE;
    }

    if(direct_mode)
    {
        if(per_led_mode == DIRECT_MODE_PER_LED)
        {
            if(hid_send_feature_report(dev, (unsigned char*)&enable_per_led_msg, sizeof(enable_per_led_msg)) != sizeof(enable_per_led_msg))
            {
                return false;
            }
            direct_mode_configured = true;
        }

        return true;
    }
    else
    {
        return false;
    }
}
