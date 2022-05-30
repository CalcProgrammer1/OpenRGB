/*---------------------------------------------*\
|  MSIMysticLight185Controller.cpp              |
|                                               |
|  Driver for MSI Mystic Light (185-byte)       |
|  USB lighting controller                      |
|                                               |
|  T-bond 3/4/2020                              |
|  Adam Honse 3/6/2021                          |
|                                               |
| The direct mode part has been implemented     |
| based on the mystic-why project provided by   |
| Aleksandr Garashchenko                        |
| (https://github.com/garashchenko/mystic-why)  |
\*---------------------------------------------*/

#include "MSIMysticLight185Controller.h"
#include <algorithm>
#include <array>
#include <bitset>


#define BITSET(val, bit, pos)       ((unsigned char)std::bitset<8>(val).set((pos), (bit)).to_ulong())


struct Config
{
    unsigned short                           pid;                   // PID of the board
    int                                      numof_onboard_leds;    // number of onboard leds
    int                                      numof_pipe1_leds;      // number of pipe 1 leds (used in per LED mode only)
    int                                      numof_pipe2_leds;      // number of pipe 2 leds (used in per LED mode only)
    int                                      numof_JRGBs;           // number of supported JRGB headers (used in per LED mode only)
    const std::vector<MSI_ZONE>*             supported_zones;       // pointer to vector of supported zones
    MSIMysticLight185Controller::DIRECT_MODE per_led_mode;          // type of direct mode support
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
    MSI_ZONE_J_CORSAIR,
    MSI_ZONE_J_PIPE_1,
    MSI_ZONE_ON_BOARD_LED_0
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



/*---------------------------------------------------------------------------------------------------------------------------------*\
| Definition of the board sepcific configurations (number of onboard LEDs and supported zones).                                     |
|                                                                                                                                   |
| Only tested boards are listed here (refer to MSIMysticLightControllerDetect.cpp). If more boards                                  |
| are tested the list must be extended here. Otherwise the default settings will be used (6 onboard LEDs, 2 JRGB, no direct mode).  |
| Boards with yet unknown supported zones are configured to support all zones.                                                      |
\*---------------------------------------------------------------------------------------------------------------------------------*/

#define NUMOF_CONFIGS       (sizeof(board_configs) / sizeof(Config))

static const Config board_configs[] =
{
    { 0x7B93, 6,  1, 0, 1, &zones_set4,  MSIMysticLight185Controller::DIRECT_MODE_ZONE_BASED },     // MPG X570 GAMING PRO CARBON WIFI
    { 0x7C34, 0,  1, 1, 1, &zones_set8,  MSIMysticLight185Controller::DIRECT_MODE_ZONE_BASED },     // MEG X570 GODLIKE                 verified
    { 0x7C35, 0,  1, 0, 1, &zones_set9,  MSIMysticLight185Controller::DIRECT_MODE_ZONE_BASED },     // MEG X570 ACE                     verified
    { 0x7C36, 6,  1, 0, 1, &zones_set4,  MSIMysticLight185Controller::DIRECT_MODE_ZONE_BASED },     // PRESTIGE X570 CREATION
    { 0x7C37, 1,  0, 0, 2, &zones_set0,  MSIMysticLight185Controller::DIRECT_MODE_DISABLED },       // MPG X570 GAMING PLUS             verified
    { 0x7C56, 6,  0, 0, 1, &zones_set2,  MSIMysticLight185Controller::DIRECT_MODE_PER_LED },        // MPG B550 GAMING PLUS             verified
    { 0x7C59, 0,  8, 0, 1, &zones_set9,  MSIMysticLight185Controller::DIRECT_MODE_DISABLED },       // CREATOR TRX40
    { 0x7C67, 6,  0, 0, 1, &zones_set5,  MSIMysticLight185Controller::DIRECT_MODE_DISABLED },       // MAG B365M MORTAR                 verified
    { 0x7C71, 6,  6, 0, 1, &zones_set3,  MSIMysticLight185Controller::DIRECT_MODE_PER_LED },        // MEG Z490 ACE                     verified
    { 0x7C73, 6,  4, 0, 1, &zones_set3,  MSIMysticLight185Controller::DIRECT_MODE_PER_LED },        // MPG Z490 GAMING CARBON WIFI      verified
    { 0x7C75, 6,  0, 0, 1, &zones_set2,  MSIMysticLight185Controller::DIRECT_MODE_PER_LED },        // MPG Z490 GAMING PLUS             verified
    { 0x7C76, 6,  0, 0, 2, &zones_set0,  MSIMysticLight185Controller::DIRECT_MODE_DISABLED },       // MPG Z490M GAMING EDGE
    { 0x7C79, 6,  0, 0, 2, &zones_set0,  MSIMysticLight185Controller::DIRECT_MODE_PER_LED },        // MPG Z490 GAMING EDGE WIFI        verified
    { 0x7C80, 6,  0, 0, 2, &zones_set0,  MSIMysticLight185Controller::DIRECT_MODE_PER_LED },        // MAG Z490 TOMAHAWK                verified
    { 0x7C81, 6,  0, 0, 2, &zones_set0,  MSIMysticLight185Controller::DIRECT_MODE_DISABLED },       // MAG B460 TOMAHAWK
    { 0x7C83, 6,  0, 0, 1, &zones_set5,  MSIMysticLight185Controller::DIRECT_MODE_DISABLED },       // B460M PRO-VDH WIFI
    { 0x7C84, 6,  0, 0, 2, &zones_set0,  MSIMysticLight185Controller::DIRECT_MODE_ZONE_BASED },     // MAG X570 TOMAHAWK WIFI           verified
    { 0x7C86, 6,  0, 0, 1, &zones_set5,  MSIMysticLight185Controller::DIRECT_MODE_DISABLED },       // MPG B460I GAMING EDGE
    { 0x7C90, 6,  4, 0, 1, &zones_set3,  MSIMysticLight185Controller::DIRECT_MODE_PER_LED },        // MPG B550 GAMING CARBON WIFI      verified
    { 0x7C91, 6,  0, 0, 2, &zones_set0,  MSIMysticLight185Controller::DIRECT_MODE_PER_LED },        // MAG B550 TOMAHAWK                verified
    { 0x7C92, 6,  0, 0, 0, &zones_set6,  MSIMysticLight185Controller::DIRECT_MODE_DISABLED },       // MPG B550I GAMING EDGE WIFI
    { 0x7C94, 6,  0, 0, 1, &zones_set11, MSIMysticLight185Controller::DIRECT_MODE_PER_LED },        // MAG B550M MORTAR                 verified
    { 0x7C95, 6,  0, 0, 2, &zones_set0,  MSIMysticLight185Controller::DIRECT_MODE_PER_LED },        // B550M PRO-VDH WIFI               verified
    { 0x7C98, 6,  0, 0, 1, &zones_set5,  MSIMysticLight185Controller::DIRECT_MODE_PER_LED },        // Z490 PLUS                        verified
    { 0x7D06, 4,  4, 0, 1, &zones_set3,  MSIMysticLight185Controller::DIRECT_MODE_DISABLED },       // MPG Z590 GAMING FORCE
    { 0x7D07, 4,  5, 0, 2, &zones_set7,  MSIMysticLight185Controller::DIRECT_MODE_PER_LED },        // MPG Z590 GAMING EDGE WIFI        verified
    { 0x7D09, 6,  0, 0, 1, &zones_set2,  MSIMysticLight185Controller::DIRECT_MODE_DISABLED },       // Z590-A PRO WIFI
    { 0x7D13, 6,  0, 0, 1, &zones_set1,  MSIMysticLight185Controller::DIRECT_MODE_DISABLED },       // MEG B550 UNIFY
    { 0x7D15, 6,  0, 0, 2, &zones_set0,  MSIMysticLight185Controller::DIRECT_MODE_DISABLED },       // MAG B560 TOMAHAWK WIFI
    { 0x7D17, 7,  0, 0, 1, &zones_set11, MSIMysticLight185Controller::DIRECT_MODE_PER_LED },        // MAG B560M MORTAR                 verified
    { 0x7D20, 7,  0, 0, 1, &zones_set5,  MSIMysticLight185Controller::DIRECT_MODE_PER_LED },        // MAG B560M PRO                    verified
    { 0x7D25, 7,  0, 0, 1, &zones_set11, MSIMysticLight185Controller::DIRECT_MODE_PER_LED },        // PRO Z690-A WIFI DDR4             verified
    { 0x7D27, 7,  0, 0, 1, &zones_set1,  MSIMysticLight185Controller::DIRECT_MODE_PER_LED },        // MEG Z690 ACE
    { 0x7D28, 6,  0, 0, 1, &zones_set1,  MSIMysticLight185Controller::DIRECT_MODE_PER_LED },        // MEG Z690 UNIFY-X                 verified
    { 0x7D29, 6,  0, 0, 0, &zones_set6,  MSIMysticLight185Controller::DIRECT_MODE_DISABLED },       // MEG Z690I UNIFY
    { 0x7D31, 4,  4, 0, 1, &zones_set12, MSIMysticLight185Controller::DIRECT_MODE_DISABLED },       // MPG EDGE WIFI DDR4
    { 0x7D32, 1,  0, 0, 1, &zones_set10, MSIMysticLight185Controller::DIRECT_MODE_PER_LED },        // MAG Z690 TOMAHAWK WIFI DDR4      verified
    { 0x7D42, 7,  0, 0, 1, &zones_set11, MSIMysticLight185Controller::DIRECT_MODE_PER_LED },        // MAG B660 MORTAR WIFI DDR4        verified
    { 0x7D50, 6, 12, 0, 1, &zones_set4,  MSIMysticLight185Controller::DIRECT_MODE_PER_LED },        // MEG X570S ACE MAX                verified
    { 0x7D52, 6, 14, 0, 1, &zones_set4,  MSIMysticLight185Controller::DIRECT_MODE_PER_LED },        // MPG X570S CARBON EK X
    { 0x7D53, 6,  0, 0, 2, &zones_set0,  MSIMysticLight185Controller::DIRECT_MODE_PER_LED },        // MPG X570S EDGE MAX WIFI          verified
    { 0x7D54, 6,  0, 0, 2, &zones_set0,  MSIMysticLight185Controller::DIRECT_MODE_PER_LED },        // MAG X570S TOMAHAWK MAX WIFI      verified
};


FeaturePacket_185 enable_per_led_msg;

Color* per_led_onboard_leds;
Color* per_led_jpipe1;
Color* per_led_jpipe2;
Color* per_led_jrgb;
Color* per_led_jrainbow1;
Color* per_led_jrainbow2;
Color* per_led_jcorsair;


MSIMysticLight185Controller::MSIMysticLight185Controller
    (
    hid_device*     handle,
    const char      *path,
    unsigned short  pid
    )
{
    dev = handle;

    if(dev)
    {
        location = path;

        ReadName();
        ReadSerial();
        ReadFwVersion();
        ReadSettings();
    }

    /*---------------------------------------------*\
    | Initialize save flag and some static settings |
    \*---------------------------------------------*/
    data.save_data = 0;
    data.on_board_led.colorFlags = 0x80 | SYNC_SETTING_ONBOARD; // always enable onboard sync flag to have expected zone control

    const Config* board_config = nullptr;

    for(std::size_t i = 0; i < NUMOF_CONFIGS; ++i)
    {
        if (board_configs[i].pid == pid)
        {
            board_config = &board_configs[i];
            break;
        }
    }

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

    /*-----------------------------------------*\
    | Initialize Per LED data                   |
    \*-----------------------------------------*/
    std::memset(per_led_data.leds, 0, sizeof(Color) * NUMOF_PER_LED_MODE_LEDS);
    per_led_onboard_leds = per_led_data.leds;
    per_led_jpipe1       = per_led_onboard_leds + numof_onboard_leds;
    per_led_jpipe2       = per_led_jpipe1 + numof_pipe1_leds;
    per_led_jrgb         = per_led_jpipe2 + numof_pipe2_leds;
    per_led_jrainbow1    = per_led_jrgb + numof_JRGBs;
    per_led_jrainbow2    = per_led_jrainbow1 + PER_LED_MODE_JRAINBOW_LED_COUNT;
    per_led_jcorsair     = per_led_jrainbow2 + PER_LED_MODE_JRAINBOW_LED_COUNT;

    unsigned char sync_mode = 0x81;

    no_onboards = true;

    for(std::size_t i = 0; i < supported_zones->size(); ++i)
    {
        switch((*supported_zones)[i])
        {
            case MSI_ZONE_J_RAINBOW_1:
                sync_mode |= SYNC_SETTING_JRAINBOW1;
                break;

            case MSI_ZONE_J_RAINBOW_2:
                sync_mode |= SYNC_SETTING_JRAINBOW2;
                break;

            case MSI_ZONE_J_RAINBOW_3:
            case MSI_ZONE_J_CORSAIR:
                sync_mode |= SYNC_SETTING_JCORSAIR;
                break;

            case MSI_ZONE_J_PIPE_1:
                sync_mode |= SYNC_SETTING_JPIPE1;
                break;

            case MSI_ZONE_J_PIPE_2:
                sync_mode |= SYNC_SETTING_JPIPE2;
                break;

            case MSI_ZONE_ON_BOARD_LED_0:
                no_onboards = false;

            default:
                break;
        }
    }

    /*---------------------------------------------------------------------------------------------------------*\
    | Set up per LED switching message.                                                                         |
    | Static initialization made problems with some compilers. Therefore this is done programmatically here.    |
    \*---------------------------------------------------------------------------------------------------------*/
    enable_per_led_msg.j_rgb_1.speedAndBrightnessFlags = 0x08;
    enable_per_led_msg.j_rgb_1.colorFlags = 0x80;
    enable_per_led_msg.j_pipe_1.speedAndBrightnessFlags = 0x2A;
    enable_per_led_msg.j_pipe_1.colorFlags = 0x80;
    enable_per_led_msg.j_pipe_2.speedAndBrightnessFlags = 0x2A;
    enable_per_led_msg.j_pipe_2.colorFlags = 0x80;
    enable_per_led_msg.j_rainbow_1.speedAndBrightnessFlags = 0x08;
    enable_per_led_msg.j_rainbow_1.colorFlags = 0x80;
    enable_per_led_msg.j_rainbow_2.speedAndBrightnessFlags = 0x08;
    enable_per_led_msg.j_rainbow_2.colorFlags = 0x80;
    enable_per_led_msg.j_corsair.effect = MSI_MODE_DISABLE;
    enable_per_led_msg.j_corsair.corsair_quantity = 0x00;
    enable_per_led_msg.j_corsair.padding[2] = 0x82;
    enable_per_led_msg.j_corsair.is_individual = 0x78;
    enable_per_led_msg.j_corsair_outerll120.speedAndBrightnessFlags = 0x28;
    enable_per_led_msg.j_corsair_outerll120.colorFlags = 0x80;
    enable_per_led_msg.on_board_led.effect = 0x25;
    enable_per_led_msg.on_board_led.speedAndBrightnessFlags = 0x29 | SYNC_SETTING_JRGB;
    enable_per_led_msg.on_board_led.colorFlags = sync_mode;
    enable_per_led_msg.on_board_led_1.speedAndBrightnessFlags = 0x28;
    enable_per_led_msg.on_board_led_1.colorFlags = 0x80;
    enable_per_led_msg.on_board_led_2.speedAndBrightnessFlags = 0x28;
    enable_per_led_msg.on_board_led_2.colorFlags = 0x80;
    enable_per_led_msg.on_board_led_3.speedAndBrightnessFlags = 0x28;
    enable_per_led_msg.on_board_led_3.colorFlags = 0x80;
    enable_per_led_msg.on_board_led_4.speedAndBrightnessFlags = 0x28;
    enable_per_led_msg.on_board_led_4.colorFlags = 0x80;
    enable_per_led_msg.on_board_led_5.speedAndBrightnessFlags = 0x28;
    enable_per_led_msg.on_board_led_5.colorFlags = 0x80;
    enable_per_led_msg.on_board_led_6.speedAndBrightnessFlags = 0x28;
    enable_per_led_msg.on_board_led_6.colorFlags = 0x80;
    enable_per_led_msg.on_board_led_7.speedAndBrightnessFlags = 0x28;
    enable_per_led_msg.on_board_led_7.colorFlags = 0x80;
    enable_per_led_msg.on_board_led_8.speedAndBrightnessFlags = 0x28;
    enable_per_led_msg.on_board_led_8.colorFlags = 0x80;
    enable_per_led_msg.on_board_led_9.speedAndBrightnessFlags = 0x28;
    enable_per_led_msg.on_board_led_9.colorFlags = 0x80;
    enable_per_led_msg.j_rgb_2.speedAndBrightnessFlags = 0x2A;
    enable_per_led_msg.j_rgb_2.colorFlags = 0x80;

    /*-----------------------------------------*\
    | Initialize zone based per LED data        |
    \*-----------------------------------------*/
    zone_based_per_led_data.j_rgb_1.speedAndBrightnessFlags = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.j_rgb_1.colorFlags = BITSET(zone_based_per_led_data.j_rgb_1.colorFlags, true, 7u);
    zone_based_per_led_data.j_pipe_1.speedAndBrightnessFlags = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.j_pipe_1.colorFlags = BITSET(zone_based_per_led_data.j_pipe_1.colorFlags, true, 7u);
    zone_based_per_led_data.j_pipe_2.speedAndBrightnessFlags = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.j_pipe_2.colorFlags = BITSET(zone_based_per_led_data.j_pipe_2.colorFlags, true, 7u);
    zone_based_per_led_data.j_rainbow_1.speedAndBrightnessFlags = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.j_rainbow_1.colorFlags = BITSET(zone_based_per_led_data.j_rainbow_1.colorFlags, true, 7u);
    zone_based_per_led_data.j_rainbow_2.speedAndBrightnessFlags = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.j_rainbow_2.colorFlags = BITSET(zone_based_per_led_data.j_rainbow_2.colorFlags, true, 7u);
    zone_based_per_led_data.on_board_led.speedAndBrightnessFlags = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.on_board_led.colorFlags = BITSET(zone_based_per_led_data.on_board_led.colorFlags, true, 7u);
    zone_based_per_led_data.on_board_led_1.speedAndBrightnessFlags = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.on_board_led_1.colorFlags = BITSET(zone_based_per_led_data.on_board_led_1.colorFlags, true, 7u);
    zone_based_per_led_data.on_board_led_2.speedAndBrightnessFlags = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.on_board_led_2.colorFlags = BITSET(zone_based_per_led_data.on_board_led_2.colorFlags, true, 7u);
    zone_based_per_led_data.on_board_led_3.speedAndBrightnessFlags = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.on_board_led_3.colorFlags = BITSET(zone_based_per_led_data.on_board_led_3.colorFlags, true, 7u);
    zone_based_per_led_data.on_board_led_4.speedAndBrightnessFlags = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.on_board_led_4.colorFlags = BITSET(zone_based_per_led_data.on_board_led_4.colorFlags, true, 7u);
    zone_based_per_led_data.on_board_led_5.speedAndBrightnessFlags = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.on_board_led_5.colorFlags = BITSET(zone_based_per_led_data.on_board_led_5.colorFlags, true, 7u);
    zone_based_per_led_data.on_board_led_6.speedAndBrightnessFlags = MSI_BRIGHTNESS_LEVEL_100 << 2 << 2;
    zone_based_per_led_data.on_board_led_6.colorFlags = BITSET(zone_based_per_led_data.on_board_led_6.colorFlags, true, 7u);
    zone_based_per_led_data.on_board_led_7.speedAndBrightnessFlags = MSI_BRIGHTNESS_LEVEL_100;
    zone_based_per_led_data.on_board_led_7.colorFlags = BITSET(zone_based_per_led_data.on_board_led_7.colorFlags, true, 7u);
    zone_based_per_led_data.on_board_led_8.speedAndBrightnessFlags = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.on_board_led_8.colorFlags = BITSET(zone_based_per_led_data.on_board_led_8.colorFlags, true, 7u);
    zone_based_per_led_data.on_board_led_9.speedAndBrightnessFlags = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.on_board_led_9.colorFlags = BITSET(zone_based_per_led_data.on_board_led_9.colorFlags, true, 7u);
    zone_based_per_led_data.j_rgb_2.speedAndBrightnessFlags = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.j_rgb_2.colorFlags = BITSET(zone_based_per_led_data.j_rgb_2.colorFlags, true, 7u);
    zone_based_per_led_data.save_data = 0;

    direct_mode = false;
}

MSIMysticLight185Controller::~MSIMysticLight185Controller()
{
    hid_close(dev);
}

void MSIMysticLight185Controller::SetMode
    (
    MSI_ZONE        zone,
    MSI_MODE        mode,
    MSI_SPEED       speed,
    MSI_BRIGHTNESS  brightness,
    bool            rainbow_color
    )
{
    if((per_led_mode == DIRECT_MODE_ZONE_BASED) && (zone > MSI_ZONE_ON_BOARD_LED_0))
    {
        return;
    }

    ZoneData* zone_data = GetZoneData(data, zone);

    if(zone_data == nullptr)
    {
        return;
    }

    zone_data->effect                    = mode;
    zone_data->speedAndBrightnessFlags   = (brightness << 2) | (speed & 0x03);
    zone_data->colorFlags                = BITSET(zone_data->colorFlags, !rainbow_color, 7u);
    zone == MSI_ZONE_J_RAINBOW_3 ? zone_data->padding = 4 : zone_data->padding = 0;

    ZoneData* on_board_zone = GetZoneData(data, MSI_ZONE_ON_BOARD_LED_0);

    if(no_onboards && ((zone == MSI_ZONE_J_RGB_1) || (zone == MSI_ZONE_J_RGB_1) || (zone == MSI_ZONE_J_PIPE_1) || (zone == MSI_ZONE_J_PIPE_2)))
    {
        on_board_zone->effect                  =  zone_data->effect;
        on_board_zone->speedAndBrightnessFlags =  zone_data->speedAndBrightnessFlags;
        on_board_zone->colorFlags              =  zone_data->colorFlags;
        on_board_zone->colorFlags              |= SYNC_SETTING_ONBOARD;
        on_board_zone->padding                 =  0x00;
    }

    if(mode > MSI_MODE_LIGHTNING)
    {
        on_board_zone->speedAndBrightnessFlags |= SYNC_SETTING_JRGB;
        on_board_zone->colorFlags              |= (SYNC_SETTING_JPIPE1 | SYNC_SETTING_JPIPE2);
    }
    else
    {
        on_board_zone->speedAndBrightnessFlags &= ~SYNC_SETTING_JRGB;
        on_board_zone->colorFlags              &= ~(SYNC_SETTING_JPIPE1 | SYNC_SETTING_JPIPE2);
    }
}

std::string MSIMysticLight185Controller::GetDeviceName()
{
    return name;
}

std::string MSIMysticLight185Controller::GetFWVersion()
{
    return std::string("AP/LD ").append(version_APROM).append(" / ").append(version_LDROM);
}

std::string MSIMysticLight185Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string MSIMysticLight185Controller::GetSerial()
{
    return chip_id;
}

bool MSIMysticLight185Controller::ReadSettings()
{
    /*-----------------------------------------------------*\
    | Read packet from hardware, return true if successful  |
    \*-----------------------------------------------------*/
    return (hid_get_feature_report(dev, (unsigned char*)&data, sizeof(data)) == sizeof data);
}

bool MSIMysticLight185Controller::Update
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
            return (hid_send_feature_report(dev, (unsigned char*)&per_led_data, sizeof(per_led_data)) == sizeof(per_led_data));
        }
        else
        {
            return (hid_send_feature_report(dev, (unsigned char*)&zone_based_per_led_data, sizeof(zone_based_per_led_data)) == sizeof(zone_based_per_led_data));
        }
    }
    else
    {
        data.save_data = save;
        return (hid_send_feature_report(dev, (unsigned char*)&data, sizeof(data)) == sizeof(data));
    }
}

void MSIMysticLight185Controller::SetZoneColor
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
    if((per_led_mode == DIRECT_MODE_ZONE_BASED) && (zone > MSI_ZONE_ON_BOARD_LED_0))
    {
        return;
    }

    ZoneData* zone_data = GetZoneData(data, zone);

    if(zone_data == nullptr)
    {
        return;
    }

    zone_data->color.R  = red1;
    zone_data->color.G  = grn1;
    zone_data->color.B  = blu1;
    zone_data->color2.R = red2;
    zone_data->color2.G = grn2;
    zone_data->color2.B = blu2;

    if(no_onboards && ((zone == MSI_ZONE_J_RGB_1) || (zone == MSI_ZONE_J_RGB_1) || (zone == MSI_ZONE_J_PIPE_1) || (zone == MSI_ZONE_J_PIPE_2)))
    {
        ZoneData* on_board_zone = GetZoneData(data, MSI_ZONE_ON_BOARD_LED_0);

        on_board_zone->color.R  = red1;
        on_board_zone->color.G  = grn1;
        on_board_zone->color.B  = blu1;
        on_board_zone->color2.R = red2;
        on_board_zone->color2.G = grn2;
        on_board_zone->color2.B = blu2;
    }
}

void MSIMysticLight185Controller::SetLedColor
    (
    MSI_ZONE        zone,
    int             index,
    unsigned char   red,
    unsigned char   grn,
    unsigned char   blu
    )
{
    if(per_led_mode == DIRECT_MODE_PER_LED)
    {
        Color* zone_data = GetPerLedZoneData(zone);

        if(zone_data != nullptr)
        {
            int maxSize = 0;

            switch(zone)
            {
                case MSI_ZONE_J_RGB_1:
                case MSI_ZONE_J_RGB_2:
                    maxSize = 1;
                    break;

                case MSI_ZONE_J_RAINBOW_1:
                case MSI_ZONE_J_RAINBOW_2:
                    maxSize = PER_LED_MODE_JRAINBOW_LED_COUNT;
                    break;

                case MSI_ZONE_ON_BOARD_LED_0:
                    maxSize = numof_onboard_leds;
                    break;

                case MSI_ZONE_J_RAINBOW_3:
                case MSI_ZONE_J_CORSAIR:
                    maxSize = PER_LED_MODE_CORSAIR_LED_COUNT;
                    break;

                case MSI_ZONE_J_PIPE_1:
                    maxSize = numof_pipe1_leds;
                    break;

                case MSI_ZONE_J_PIPE_2:
                    maxSize = numof_pipe2_leds;
                    break;

                default:
                    break;
            }

            if(index < maxSize)
            {
                zone_data[index].R = red;
                zone_data[index].G = grn;
                zone_data[index].B = blu;
            }
        }
    }
    else
    {
        if(((zone == MSI_ZONE_J_RAINBOW_1) || (zone == MSI_ZONE_J_RAINBOW_2) || (zone == MSI_ZONE_J_PIPE_1) || (zone == MSI_ZONE_J_PIPE_2)) && (index != 0))
        {
            return;
        }

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

ZoneData *MSIMysticLight185Controller::GetZoneData
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
            return &data_packet.j_rgb_2;
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
            return &data_packet.on_board_led_6;
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

Color *MSIMysticLight185Controller::GetPerLedZoneData
    (
    MSI_ZONE  zone
    )
{
    switch(zone)
    {
        case MSI_ZONE_J_RAINBOW_1:
            return per_led_jrainbow1;
        case MSI_ZONE_J_RAINBOW_2:
            return per_led_jrainbow2;
        case MSI_ZONE_ON_BOARD_LED_0:
            return per_led_onboard_leds;
        case MSI_ZONE_J_RAINBOW_3:
        case MSI_ZONE_J_CORSAIR:
            return per_led_jcorsair;
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

RainbowZoneData *MSIMysticLight185Controller::GetRainbowZoneData
    (
    MSI_ZONE zone
    )
{
    switch(zone)
    {
        case MSI_ZONE_J_RAINBOW_1:
            return &data.j_rainbow_1;
        case MSI_ZONE_J_RAINBOW_2:
            return &data.j_rainbow_2;
        case MSI_ZONE_J_RAINBOW_3:
            return (RainbowZoneData*)&data.j_corsair;
        case MSI_ZONE_J_CORSAIR:
        default:
            return nullptr;
    }
}

bool MSIMysticLight185Controller::ReadFwVersion()
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

void MSIMysticLight185Controller::ReadSerial()
{
    wchar_t serial[256];

    /*-----------------------------------------------------*\
    | Get the serial number string from HID                 |
    \*-----------------------------------------------------*/
    hid_get_serial_number_string(dev, serial, 256);

    /*-----------------------------------------------------*\
    | Convert wchar_t into std::wstring into std::string    |
    \*-----------------------------------------------------*/
    std::wstring wserial = std::wstring(serial);
    chip_id = std::string(wserial.begin(), wserial.end());
}

void MSIMysticLight185Controller::ReadName()
{
    wchar_t tname[256];

    /*-----------------------------------------------------*\
    | Get the manufacturer string from HID                  |
    \*-----------------------------------------------------*/
    hid_get_manufacturer_string(dev, tname, 256);

    /*-----------------------------------------------------*\
    | Convert wchar_t into std::wstring into std::string    |
    \*-----------------------------------------------------*/
    std::wstring wname = std::wstring(tname);
    name = std::string(wname.begin(), wname.end());

    /*-----------------------------------------------------*\
    | Get the product string from HID                       |
    \*-----------------------------------------------------*/
    hid_get_product_string(dev, tname, 256);

    /*-----------------------------------------------------*\
    | Append the product string to the manufacturer string  |
    \*-----------------------------------------------------*/
    wname = std::wstring(tname);
    name.append(" ").append(std::string(wname.begin(), wname.end()));
}

void MSIMysticLight185Controller::GetMode(MSI_ZONE zone,
                                          MSI_MODE &mode,
                                          MSI_SPEED &speed,
                                          MSI_BRIGHTNESS &brightness,
                                          bool &rainbow_color)
{
    /*-----------------------------------------------------*\
    | Get data for given zone                               |
    \*-----------------------------------------------------*/
    ZoneData *zoneData = GetZoneData(data, zone);

    /*-----------------------------------------------------*\
    | Return if zone is invalid                             |
    \*-----------------------------------------------------*/
    if(!zoneData)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Update pointers with data                             |
    \*-----------------------------------------------------*/
    mode            = (MSI_MODE)zoneData->effect;
    speed           = (MSI_SPEED)(zoneData->speedAndBrightnessFlags & 0x03);
    brightness      = (MSI_BRIGHTNESS)((zoneData->speedAndBrightnessFlags >> 2) & 0x1F);
    rainbow_color   = (zoneData->colorFlags & 0x80) >> 7;
}

void MSIMysticLight185Controller::SetCycleCount
    (
    MSI_ZONE        zone,
    unsigned char   cycle_num
    )
{
    RainbowZoneData *requested_zone = GetRainbowZoneData(zone);

    if(!requested_zone)
    {
        return;
    }

    requested_zone->cycle_or_led_num = cycle_num;
}

void MSIMysticLight185Controller::SetDirectMode
    (
    bool mode
    )
{
    direct_mode = mode;

    if(direct_mode)
    {
        if(per_led_mode == DIRECT_MODE_PER_LED)
        {
            hid_send_feature_report(dev, (unsigned char*)&enable_per_led_msg, sizeof(enable_per_led_msg));
        }
    }
}

size_t MSIMysticLight185Controller::GetMaxDirectLeds
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
        case MSI_ZONE_J_RAINBOW_2:
        case MSI_ZONE_J_RAINBOW_3:
            return 200;

        case MSI_ZONE_J_CORSAIR:
            return PER_LED_MODE_CORSAIR_LED_COUNT;

        case MSI_ZONE_ON_BOARD_LED_0:
            return numof_onboard_leds;

        default:
            return 1;
    }
}
