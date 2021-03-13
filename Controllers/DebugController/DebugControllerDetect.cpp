#include "Detector.h"
#include "RGBController.h"
#include "RGBController_Debug.h"
#include "RGBController_Dummy.h"
#include "SettingsManager.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <string>


//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int debug_keyboard_matrix_map[6][23] =
    { {   0,  NA,  16,  30,  44,  54,  NA,  65,  75,  84,  95,  NA,   8,  23 ,  38,   6 ,  22,  36,  49,  NA,  NA,  NA,  NA },
      {   1,  17,  31,  45,  55,  66,  76,  85,  96,   9,  24,  NA,  39,   7 ,  37,  NA ,  60,  70,  80,  52,  63,  73,  82 },
      {   2,  NA,  18,  32,  46,  56,  NA,  67,  77,  86,  97,  10,  25,  40 ,  90,  101,  50,  61,  71,  51,  62,  72,  93 },
      {   3,  NA,  19,  33,  47,  57,  NA,  68,  78,  87,  98,  11,  26,  41 ,  28,  14 ,  NA,  NA,  NA,  92, 103,  53,  NA },
      {   4,  20,  34,  48,  58,  69,  NA,  79,  NA,  88,  99,  12,  27,  42 ,  81,  NA ,  NA, 102,  NA,  64,  74,  83, 104 },
      {   5,  21,  35,  NA,  NA,  NA,  NA,  59,  NA,  NA,  NA,  NA,  89,  100,  13,  91 ,  15,  29,  43,  94,  NA, 105,  NA } };

static unsigned int dummy_keyboard_underglow_map[3][10] =
    { {   0 , 1 ,  2 ,  3 ,  4 ,  5 ,  6 ,  7 ,  8 ,  9  },
      {   10, 11,  12,  13,  14,  15,  16,  17,  18,  19 },
      {   20, 21,  22,  23,  24,  25,  26,  27,  28,  29 } };

static const char *led_names[] =
{
    "Key: Escape",
    "Key: `",
    "Key: Tab",
    "Key: Caps Lock",
    "Key: Left Shift",
    "Key: Left Control",
    "Key: F12",
    "Key: =",
    "Key: F9",
    "Key: 9",
    "Key: O",
    "Key: L",
    "Key: ,",
    "Key: Menu",
    "Key: Enter (ISO)",
    "Key: Left Arrow",
    "Key: F1",
    "Key: 1",
    "Key: Q",
    "Key: A",
    "Key: \\ (ISO)",
    "Key: Left Windows",
    "Key: Print Screen",
    "Key: F10",
    "Key: 0",
    "Key: P",
    "Key: ;",
    "Key: .",
    "Key: Enter",
    "Key: Down Arrow",
    "Key: F2",
    "Key: 2",
    "Key: W",
    "Key: S",
    "Key: Z",
    "Key: Left Alt",
    "Key: Scroll Lock",
    "Key: Backspace",
    "Key: F11",
    "Key: -",
    "Key: [",
    "Key: '",
    "Key: /",
    "Key: Right Arrow",
    "Key: F3",
    "Key: 3",
    "Key: E",
    "Key: D",
    "Key: X",
    "Key: Pause/Break",
    "Key: Delete",
    "Key: Number Pad 7",
    "Key: Num Lock",
    "Key: Number Pad 6",
    "Key: F4",
    "Key: 4",
    "Key: R",
    "Key: F",
    "Key: C",
    "Key: Space",
    "Key: Insert",
    "Key: End",
    "Key: Number Pad 8",
    "Key: Number Pad /",
    "Key: Number Pad 1",
    "Key: F5",
    "Key: 5",
    "Key: T",
    "Key: G",
    "Key: V",
    "Key: Home",
    "Key: Page Down",
    "Key: Number Pad 9",
    "Key: Number Pad *",
    "Key: Number Pad 2",
    "Key: F6",
    "Key: 6",
    "Key: Y",
    "Key: H",
    "Key: B",
    "Key: Page Up",
    "Key: Right Shift",
    "Key: Number Pad -",
    "Key: Number Pad 3",
    "Key: F7",
    "Key: 7",
    "Key: U",
    "Key: J",
    "Key: N",
    "Key: Right Alt",
    "Key: ]",
    "Key: Right Control",
    "Key: Number Pad 4",
    "Key: Number Pad +",
    "Key: Number Pad 0",
    "Key: F8",
    "Key: 8",
    "Key: I",
    "Key: K",
    "Key: M",
    "Key: Right Windows",
    "Key: \\ (ANSI)",
    "Key: Up Arrow",
    "Key: Number Pad 5",
    "Key: Number Pad Enter",
    "Key: Number Pad .",
    "RGB Strip 1",
    "RGB Strip 2",
    "RGB Strip 3",
    "RGB Strip 4",
    "RGB Strip 5",
    "RGB Strip 6",
    "RGB Strip 7",
    "RGB Strip 8",
    "RGB Strip 9",
    "RGB Strip 10",
    "RGB Strip 11",
    "RGB Strip 12",
    "RGB Strip 13",
    "RGB Strip 14",
    "RGB Strip 15",
    "RGB Strip 16",
    "RGB Strip 17",
    "RGB Strip 18",
    "Key: Media Previous",
    "Key: Media Play/Pause",
    "Key: Media Next",
    "Key: Media Mute",
    "Underglow LED 1",
    "Underglow LED 2",
    "Underglow LED 3",
    "Underglow LED 4",
    "Underglow LED 5",
    "Underglow LED 6",
    "Underglow LED 7",
    "Underglow LED 8",
    "Underglow LED 9",
    "Underglow LED 10",
    "Underglow LED 11",
    "Underglow LED 12",
    "Underglow LED 13",
    "Underglow LED 14",
    "Underglow LED 15",
    "Underglow LED 16",
    "Underglow LED 17",
    "Underglow LED 18",
    "Underglow LED 19",
    "Underglow LED 20",
    "Underglow LED 21",
    "Underglow LED 22",
    "Underglow LED 23",
    "Underglow LED 24",
    "Underglow LED 25",
    "Underglow LED 26",
    "Underglow LED 27",
    "Underglow LED 28",
    "Underglow LED 29",
    "Underglow LED 30"
};

/******************************************************************************************\
*                                                                                          *
*   DetectDebugControllers                                                                 *
*                                                                                          *
*       Add dummy controllers based on the DebugDevices key in the settings json           *
*                                                                                          *
\******************************************************************************************/

void DetectDebugControllers(std::vector<RGBController*> &rgb_controllers)
{
    json            debug_settings;

    /*-------------------------------------------------*\
    | Get Debug Device settings from settings manager   |
    \*-------------------------------------------------*/
    debug_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("DebugDevices");

    /*-------------------------------------------------*\
    | If the Debug settings contains devices, process   |
    \*-------------------------------------------------*/
    if(debug_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < debug_settings["devices"].size(); device_idx++)
        {
            std::string type = "";

            if(debug_settings["devices"][device_idx].contains("type"))
            {
                type = debug_settings["devices"][device_idx]["type"];
            }

            if(type == "motherboard")
            {
                /*---------------------------------------------------------*\
                | Create a dummy motherboard                                |
                \*---------------------------------------------------------*/
                RGBController_Dummy* dummy_motherboard = new RGBController_Dummy();

                dummy_motherboard->name                     = "Debug Motherboard";
                dummy_motherboard->type                     = DEVICE_TYPE_MOTHERBOARD;
                dummy_motherboard->description              = "Debug Motherboard Device";
                dummy_motherboard->location                 = "Debug Motherboard Location";
                dummy_motherboard->version                  = "Debug Motherboard Version";
                dummy_motherboard->serial                   = "Debug Motherboard Serial";

                /*---------------------------------------------------------*\
                | Create a direct mode for the dummy motherboard            |
                \*---------------------------------------------------------*/
                mode dummy_motherboard_direct_mode;

                dummy_motherboard_direct_mode.name          = "Direct";
                dummy_motherboard_direct_mode.value         = 0;
                dummy_motherboard_direct_mode.flags         = MODE_FLAG_HAS_PER_LED_COLOR;
                dummy_motherboard_direct_mode.color_mode    = MODE_COLORS_PER_LED;

                dummy_motherboard->modes.push_back(dummy_motherboard_direct_mode);

                /*---------------------------------------------------------*\
                | Create a single zone/LED for the dummy motherboard        |
                \*---------------------------------------------------------*/
                zone dummy_motherboard_single_zone;

                dummy_motherboard_single_zone.name          = "Single Zone";
                dummy_motherboard_single_zone.type          = ZONE_TYPE_SINGLE;
                dummy_motherboard_single_zone.leds_min      = 1;
                dummy_motherboard_single_zone.leds_max      = 1;
                dummy_motherboard_single_zone.leds_count    = 1;
                dummy_motherboard_single_zone.matrix_map    = NULL;

                dummy_motherboard->zones.push_back(dummy_motherboard_single_zone);

                led dummy_motherboard_single_led;

                dummy_motherboard_single_led.name           = "Single LED";

                dummy_motherboard->leds.push_back(dummy_motherboard_single_led);

                /*---------------------------------------------------------*\
                | Create a linear zone for the dummy motherboard            |
                \*---------------------------------------------------------*/
                zone dummy_motherboard_linear_zone;

                dummy_motherboard_linear_zone.name          = "Linear Zone";
                dummy_motherboard_linear_zone.type          = ZONE_TYPE_LINEAR;
                dummy_motherboard_linear_zone.leds_min      = 10;
                dummy_motherboard_linear_zone.leds_max      = 10;
                dummy_motherboard_linear_zone.leds_count    = 10;
                dummy_motherboard_linear_zone.matrix_map    = NULL;

                dummy_motherboard->zones.push_back(dummy_motherboard_linear_zone);

                for(std::size_t led_idx = 0; led_idx < 10; led_idx++)
                {
                    led dummy_motherboard_linear_led;

                    dummy_motherboard_linear_led.name       = "Linear LED " + std::to_string(led_idx);

                    dummy_motherboard->leds.push_back(dummy_motherboard_linear_led);
                }

                dummy_motherboard->SetupColors();

                /*---------------------------------------------------------*\
                | Push the dummy motherboard onto the controller list       |
                \*---------------------------------------------------------*/
                rgb_controllers.push_back(dummy_motherboard);
            }
            else if(type == "dram")
            {
                /*---------------------------------------------------------*\
                | Create a dummy DRAM                                       |
                \*---------------------------------------------------------*/
                RGBController_Dummy* dummy_dram = new RGBController_Dummy();

                dummy_dram->name                            = "Debug DRAM";
                dummy_dram->type                            = DEVICE_TYPE_DRAM;
                dummy_dram->description                     = "Debug DRAM Device";
                dummy_dram->location                        = "Debug DRAM Location";
                dummy_dram->version                         = "Debug DRAM Version";
                dummy_dram->serial                          = "Debug DRAM Serial";

                /*---------------------------------------------------------*\
                | Create a direct mode for the dummy DRAM                   |
                \*---------------------------------------------------------*/
                mode dummy_dram_direct_mode;

                dummy_dram_direct_mode.name                 = "Direct";
                dummy_dram_direct_mode.value                = 0;
                dummy_dram_direct_mode.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
                dummy_dram_direct_mode.color_mode           = MODE_COLORS_PER_LED;

                dummy_dram->modes.push_back(dummy_dram_direct_mode);

                /*---------------------------------------------------------*\
                | Create a single zone/LED for the dummy DRAM               |
                \*---------------------------------------------------------*/
                zone dummy_dram_single_zone;

                dummy_dram_single_zone.name                 = "Single Zone";
                dummy_dram_single_zone.type                 = ZONE_TYPE_SINGLE;
                dummy_dram_single_zone.leds_min             = 1;
                dummy_dram_single_zone.leds_max             = 1;
                dummy_dram_single_zone.leds_count           = 1;
                dummy_dram_single_zone.matrix_map           = NULL;

                dummy_dram->zones.push_back(dummy_dram_single_zone);

                led dummy_dram_single_led;

                dummy_dram_single_led.name                  = "Single LED";

                dummy_dram->leds.push_back(dummy_dram_single_led);

                /*---------------------------------------------------------*\
                | Create a linear zone for the dummy DRAM                   |
                \*---------------------------------------------------------*/
                zone dummy_dram_linear_zone;

                dummy_dram_linear_zone.name                 = "Linear Zone";
                dummy_dram_linear_zone.type                 = ZONE_TYPE_LINEAR;
                dummy_dram_linear_zone.leds_min             = 5;
                dummy_dram_linear_zone.leds_max             = 5;
                dummy_dram_linear_zone.leds_count           = 5;
                dummy_dram_linear_zone.matrix_map           = NULL;

                dummy_dram->zones.push_back(dummy_dram_linear_zone);

                for(std::size_t led_idx = 0; led_idx < 5; led_idx++)
                {
                    led dummy_dram_linear_led;

                    dummy_dram_linear_led.name              = "Linear LED " + std::to_string(led_idx);

                    dummy_dram->leds.push_back(dummy_dram_linear_led);
                }

                dummy_dram->SetupColors();

                /*---------------------------------------------------------*\
                | Push the dummy DRAM onto the controller list              |
                \*---------------------------------------------------------*/
                rgb_controllers.push_back(dummy_dram);
            }
            else if(type == "gpu")
            {
                /*---------------------------------------------------------*\
                | Create a dummy GPU                                        |
                \*---------------------------------------------------------*/
                RGBController_Dummy* dummy_gpu = new RGBController_Dummy();

                dummy_gpu->name                             = "Debug GPU";
                dummy_gpu->type                             = DEVICE_TYPE_GPU;
                dummy_gpu->description                      = "Debug GPU Device";
                dummy_gpu->location                         = "Debug GPU Location";
                dummy_gpu->version                          = "Debug GPU Version";
                dummy_gpu->serial                           = "Debug GPU Serial";

                /*---------------------------------------------------------*\
                | Create a direct mode for the dummy GPU                    |
                \*---------------------------------------------------------*/
                mode dummy_gpu_direct_mode;

                dummy_gpu_direct_mode.name                  = "Direct";
                dummy_gpu_direct_mode.value                 = 0;
                dummy_gpu_direct_mode.flags                 = MODE_FLAG_HAS_PER_LED_COLOR;
                dummy_gpu_direct_mode.color_mode            = MODE_COLORS_PER_LED;

                dummy_gpu->modes.push_back(dummy_gpu_direct_mode);

                /*---------------------------------------------------------*\
                | Create a single zone/LED for the dummy GPU                |
                \*---------------------------------------------------------*/
                zone dummy_gpu_single_zone;

                dummy_gpu_single_zone.name                  = "Single Zone";
                dummy_gpu_single_zone.type                  = ZONE_TYPE_SINGLE;
                dummy_gpu_single_zone.leds_min              = 1;
                dummy_gpu_single_zone.leds_max              = 1;
                dummy_gpu_single_zone.leds_count            = 1;
                dummy_gpu_single_zone.matrix_map            = NULL;

                dummy_gpu->zones.push_back(dummy_gpu_single_zone);

                led dummy_gpu_single_led;

                dummy_gpu_single_led.name                   = "Single LED";

                dummy_gpu->leds.push_back(dummy_gpu_single_led);

                /*---------------------------------------------------------*\
                | Create a linear zone for the dummy GPU                    |
                \*---------------------------------------------------------*/
                zone dummy_gpu_linear_zone;

                dummy_gpu_linear_zone.name                  = "Linear Zone";
                dummy_gpu_linear_zone.type                  = ZONE_TYPE_LINEAR;
                dummy_gpu_linear_zone.leds_min              = 15;
                dummy_gpu_linear_zone.leds_max              = 15;
                dummy_gpu_linear_zone.leds_count            = 15;
                dummy_gpu_linear_zone.matrix_map            = NULL;

                dummy_gpu->zones.push_back(dummy_gpu_linear_zone);

                for(std::size_t led_idx = 0; led_idx < 15; led_idx++)
                {
                    led dummy_gpu_linear_led;

                    dummy_gpu_linear_led.name               = "Linear LED " + std::to_string(led_idx);

                    dummy_gpu->leds.push_back(dummy_gpu_linear_led);
                }

                dummy_gpu->SetupColors();

                /*---------------------------------------------------------*\
                | Push the dummy GPU onto the controller list               |
                \*---------------------------------------------------------*/
                rgb_controllers.push_back(dummy_gpu);
            }
            else if(type == "keyboard")
            {
                /*---------------------------------------------------------*\
                | Create a dummy Keyboard                                   |
                \*---------------------------------------------------------*/
                RGBController_Dummy* dummy_keyboard = new RGBController_Dummy();

                dummy_keyboard->name                             = "Debug Keyboard";
                dummy_keyboard->type                             = DEVICE_TYPE_KEYBOARD;
                dummy_keyboard->description                      = "Debug Keyboard Device";
                dummy_keyboard->location                         = "Debug Keyboard Location";
                dummy_keyboard->version                          = "Debug Keyboard Version";
                dummy_keyboard->serial                           = "Debug Keyboard Serial";

                /*---------------------------------------------------------*\
                | Create a direct mode for the dummy keyboard               |
                \*---------------------------------------------------------*/
                mode dummy_keyboard_direct_mode;

                dummy_keyboard_direct_mode.name                  = "Direct";
                dummy_keyboard_direct_mode.value                 = 0;
                dummy_keyboard_direct_mode.flags                 = MODE_FLAG_HAS_PER_LED_COLOR;
                dummy_keyboard_direct_mode.color_mode            = MODE_COLORS_PER_LED;

                dummy_keyboard->modes.push_back(dummy_keyboard_direct_mode);

                /*---------------------------------------------------------*\
                | Create a matrix zone for the debug Keyboard               |
                \*---------------------------------------------------------*/
                zone dummy_keyboard_matrix_zone;

                dummy_keyboard_matrix_zone.name                  = "Keyboard Matrix Zone";
                dummy_keyboard_matrix_zone.type                  = ZONE_TYPE_MATRIX;
                dummy_keyboard_matrix_zone.leds_min              = 106;
                dummy_keyboard_matrix_zone.leds_max              = 106;
                dummy_keyboard_matrix_zone.leds_count            = 106;
                dummy_keyboard_matrix_zone.matrix_map            = new matrix_map_type;
                dummy_keyboard_matrix_zone.matrix_map->height    = 6;
                dummy_keyboard_matrix_zone.matrix_map->width     = 23;
                dummy_keyboard_matrix_zone.matrix_map->map       = (unsigned int*)&debug_keyboard_matrix_map;

                dummy_keyboard->zones.push_back(dummy_keyboard_matrix_zone);

                /*-----------------------------------------------------------------*\
                | Add another matrix zone so that is can resemble a huntsman elite  |
                \*-----------------------------------------------------------------*/
                zone dummy_keyboard_underglow_matrix_zone;

                dummy_keyboard_underglow_matrix_zone.name               = "Underglow";
                dummy_keyboard_underglow_matrix_zone.type               = ZONE_TYPE_MATRIX;
                dummy_keyboard_underglow_matrix_zone.leds_min           = 30;
                dummy_keyboard_underglow_matrix_zone.leds_max           = 30;
                dummy_keyboard_underglow_matrix_zone.leds_count         = 30;
                dummy_keyboard_underglow_matrix_zone.matrix_map         = new matrix_map_type;
                dummy_keyboard_underglow_matrix_zone.matrix_map->height = 3;
                dummy_keyboard_underglow_matrix_zone.matrix_map->width  = 10;
                dummy_keyboard_underglow_matrix_zone.matrix_map->map    = (unsigned int*)&dummy_keyboard_underglow_map;

                dummy_keyboard->zones.push_back(dummy_keyboard_underglow_matrix_zone);

                /*---------------------------------------------------------*\
                | Create a linear zone for the dummy GPU                    |
                \*---------------------------------------------------------*/
                zone dummy_keyboard_linear_zone;

                dummy_keyboard_linear_zone.name                  = "Linear Zone";
                dummy_keyboard_linear_zone.type                  = ZONE_TYPE_LINEAR;
                dummy_keyboard_linear_zone.leds_min              = 18;
                dummy_keyboard_linear_zone.leds_max              = 18;
                dummy_keyboard_linear_zone.leds_count            = 18;
                dummy_keyboard_linear_zone.matrix_map            = NULL;

                dummy_keyboard->zones.push_back(dummy_keyboard_linear_zone);

                for(std::size_t led_idx = 0; led_idx < 154; led_idx++)
                {
                    led dummy_keyboard_led;
                    dummy_keyboard_led.name = led_names[led_idx];
                    dummy_keyboard->leds.push_back(dummy_keyboard_led);
                }

                dummy_keyboard->SetupColors();

                /*---------------------------------------------------------*\
                | Push the dummy Keyboard onto the controller list          |
                \*---------------------------------------------------------*/
                rgb_controllers.push_back(dummy_keyboard);
            }
            else if(type == "argb")
            {
                /*---------------------------------------------------------*\
                | Create a dummy ARGB                                       |
                \*---------------------------------------------------------*/
                RGBController_Debug* dummy_argb = new RGBController_Debug();

                dummy_argb->name                            = "Debug ARGB";
                dummy_argb->type                            = DEVICE_TYPE_LEDSTRIP;
                dummy_argb->description                     = "Debug ARGB Device";
                dummy_argb->location                        = "Debug ARGB Location";
                dummy_argb->version                         = "Debug ARGB Version";
                dummy_argb->serial                          = "Debug ARGB Serial";

                /*---------------------------------------------------------*\
                | Create a direct mode for the dummy ARGB                   |
                \*---------------------------------------------------------*/
                mode dummy_argb_direct_mode;

                dummy_argb_direct_mode.name                 = "Direct";
                dummy_argb_direct_mode.value                = 0;
                dummy_argb_direct_mode.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
                dummy_argb_direct_mode.color_mode           = MODE_COLORS_PER_LED;

                dummy_argb->modes.push_back(dummy_argb_direct_mode);

                /*---------------------------------------------------------*\
                | Create a linear zone for the dummy ARGB                   |
                \*---------------------------------------------------------*/
                zone dummy_argb_linear_zone;

                dummy_argb_linear_zone.name                 = "Resizable zone";
                dummy_argb_linear_zone.type                 = ZONE_TYPE_LINEAR;
                dummy_argb_linear_zone.leds_min             = 1;
                dummy_argb_linear_zone.leds_max             = 100;
                dummy_argb_linear_zone.leds_count           = 0;
                dummy_argb_linear_zone.matrix_map           = NULL;

                dummy_argb->zones.push_back(dummy_argb_linear_zone);

                dummy_argb->SetupColors();
                dummy_argb->ResizeZone(0, 60);

                /*---------------------------------------------------------*\
                | Push the dummy ARGB onto the controller list              |
                \*---------------------------------------------------------*/
                rgb_controllers.push_back(dummy_argb);
            }
        }
    }

    if (debug_settings.contains("CustomDevices"))
    {
        for (int CustomDevice = 0; CustomDevice < (int)debug_settings["CustomDevices"].size(); CustomDevice++)
        {
            json CustomDev = debug_settings["CustomDevices"][CustomDevice];
            /*---------------------------*\
            | Create a custom controller  |
            \*---------------------------*/
            RGBController_Debug* dummy_custom = new RGBController_Debug();

            /*----------------------------------------------------------------------*\
            | if ANY of the attributes are missing then go ahead and skip the entry  |
            \*----------------------------------------------------------------------*/
            if (
                    !CustomDev.contains("DeviceName")        ||
                    !CustomDev.contains("DeviceType")        ||
                    !CustomDev.contains("DeviceDescription") ||
                    !CustomDev.contains("DeviceLocation")    ||
                    !CustomDev.contains("DeviceVersion")     ||
                    !CustomDev.contains("DeviceSerial")      ||
                    !CustomDev.contains("DeviceZones")
                    )
            {
                continue;
            }


            /*-------------*\
            | Set the name  |
            \*-------------*/
            dummy_custom->name = CustomDev["DeviceName"];

            /*---------------------*\
            | Find the device type  |
            \*---------------------*/
            if      (CustomDev["DeviceType"] == "motherboard")   dummy_custom->type = DEVICE_TYPE_MOTHERBOARD;
            else if (CustomDev["DeviceType"] == "dram")          dummy_custom->type = DEVICE_TYPE_DRAM;
            else if (CustomDev["DeviceType"] == "gpu")           dummy_custom->type = DEVICE_TYPE_GPU;
            else if (CustomDev["DeviceType"] == "cooler")        dummy_custom->type = DEVICE_TYPE_COOLER;
            else if (CustomDev["DeviceType"] == "led_strip")     dummy_custom->type = DEVICE_TYPE_LEDSTRIP;
            else if (CustomDev["DeviceType"] == "keyboard")      dummy_custom->type = DEVICE_TYPE_KEYBOARD;
            else if (CustomDev["DeviceType"] == "mouse")         dummy_custom->type = DEVICE_TYPE_MOUSE;
            else if (CustomDev["DeviceType"] == "mousemat")      dummy_custom->type = DEVICE_TYPE_MOUSEMAT;
            else if (CustomDev["DeviceType"] == "headset")       dummy_custom->type = DEVICE_TYPE_HEADSET;
            else if (CustomDev["DeviceType"] == "headset_stand") dummy_custom->type = DEVICE_TYPE_HEADSET_STAND;
            else if (CustomDev["DeviceType"] == "gamepad")       dummy_custom->type = DEVICE_TYPE_GAMEPAD;
            else if (CustomDev["DeviceType"] == "light")         dummy_custom->type = DEVICE_TYPE_LIGHT;
            else if (CustomDev["DeviceType"] == "speaker")       dummy_custom->type = DEVICE_TYPE_SPEAKER;
            else if (CustomDev["DeviceType"] == "unknown")       dummy_custom->type = DEVICE_TYPE_UNKNOWN;

            /*-----------------------------------------------*\
            | Set description, location, version, and serial  |
            \*-----------------------------------------------*/
            dummy_custom->description = CustomDev["DeviceDescription"];
            dummy_custom->location    = CustomDev["DeviceLocation"];
            dummy_custom->version     = CustomDev["DeviceVersion"];
            dummy_custom->serial      = CustomDev["DeviceSerial"];

            /*----------------*\
            | Create the mode  |
            \*----------------*/
            mode dummy_custom_direct;
            dummy_custom_direct.name                 = "Direct";
            dummy_custom_direct.value                = 0;
            dummy_custom_direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
            dummy_custom_direct.color_mode           = MODE_COLORS_PER_LED;
            dummy_custom->modes.push_back(dummy_custom_direct);

            /*--------------*\
            | Fill in zones  |
            \*--------------*/
            for (int ZoneID = 0; ZoneID < (int)CustomDev["DeviceZones"].size(); ZoneID++)
            {
                json ZoneJson = CustomDev["DeviceZones"][ZoneID];
                if
                (
                    !ZoneJson.contains("name")       ||
                    !ZoneJson.contains("type")       ||
                    !ZoneJson.contains("leds_min")   ||
                    !ZoneJson.contains("leds_max")   ||
                    !ZoneJson.contains("leds_count")
                )
                {
                    continue;
                }
                zone custom_zone;

                custom_zone.name = ZoneJson["name"];

                if      (ZoneJson["type"] == "linear") custom_zone.type = ZONE_TYPE_LINEAR;
                else if (ZoneJson["type"] == "matrix") custom_zone.type = ZONE_TYPE_MATRIX;
                else if (ZoneJson["type"] == "single") custom_zone.type = ZONE_TYPE_SINGLE;
                else
                {
                    continue;
                }

                custom_zone.leds_min   = ZoneJson["leds_min"];
                custom_zone.leds_max   = ZoneJson["leds_max"];
                custom_zone.leds_count = ZoneJson["leds_count"];

                /*-----------------------*\
                | Fill in the matrix map  |
                \*-----------------------*/
                bool BadVal = false;
                if (custom_zone.type == ZONE_TYPE_MATRIX)
                {
                    if
                    (
                        !ZoneJson.contains("matrix_height") ||
                        !ZoneJson.contains("matrix_width")  ||
                        !ZoneJson.contains("matrix_map")
                    )
                    {
                        /*--------------------------------------------------------------*\
                        | If there is no map then the zone can't be valid. Don't add it  |
                        \*--------------------------------------------------------------*/
                        continue;
                    }

                    custom_zone.matrix_map = new matrix_map_type;

                    custom_zone.matrix_map->width  = ZoneJson["matrix_width"];
                    custom_zone.matrix_map->height = ZoneJson["matrix_height"];

                    int H = custom_zone.matrix_map->height;
                    int W = custom_zone.matrix_map->width;

                    BadVal = (ZoneJson["matrix_map"].size() != custom_zone.matrix_map->height);

                    unsigned int* MatrixARR = new unsigned int[H * W];
                    for (int MatrixMapRow = 0; MatrixMapRow < H; MatrixMapRow++)
                    {
                        /*-----------------------------------------------------------------------------------------------------*\
                        | If something went wrong then make no attempt to recover and just move on in a way that doesn't crash  |
                        | Even 1 bad row can corrupt the map so skip the zone entirely                                          |
                        \*-----------------------------------------------------------------------------------------------------*/
                        if ((custom_zone.matrix_map->width != ZoneJson["matrix_map"][MatrixMapRow].size()) || BadVal)
                        {
                            BadVal = true;
                            break;
                        }

                        for (int MatrixMapCol = 0; MatrixMapCol < W; MatrixMapCol++)
                        {
                            int Val = ZoneJson["matrix_map"][MatrixMapRow][MatrixMapCol];

                            if ((signed)Val == -1)
                            {
                                MatrixARR[MatrixMapRow * W + MatrixMapCol] = NA;
                            }
                            else
                            {
                                MatrixARR[MatrixMapRow * W + MatrixMapCol] = (unsigned)Val;
                            }
                        }
                    }

                    custom_zone.matrix_map->map = MatrixARR;
                }

                /*------------------------------------*\
                | Don't add the zone if it is invalid  |
                \*------------------------------------*/
                if (BadVal)
                {
                    continue;
                }

                bool UseCustomLabels = false;
                if (ZoneJson.contains("custom_labels"))
                {
                    /*-------------------------------------------------------*\
                    | If the count is correct and the zone is non-resizeable  |
                    \*-------------------------------------------------------*/
                    if ((ZoneJson["custom_labels"].size() == custom_zone.leds_count) && (custom_zone.leds_min == custom_zone.leds_max))
                    {
                        UseCustomLabels = true;
                    }
                }

                /*------------------*\
                | Set the LED names  |
                \*------------------*/
                for (int LED_ID = 0; LED_ID < (int)custom_zone.leds_count; LED_ID++)
                {
                    led custom_led;
                    if (UseCustomLabels)
                    {
                        /*----------------------------------------*\
                        | Set the label to the user defined label  |
                        \*----------------------------------------*/
                        custom_led.name = ZoneJson["custom_labels"][LED_ID];
                    }
                    else
                    {
                        /*------------------------------------------------*\
                        | Set default labels because something went wrong  |
                        \*------------------------------------------------*/
                        custom_led.name = ("Custom LED. Zone " + std::to_string(ZoneID) + ", LED " + std::to_string(LED_ID));
                    }
                    dummy_custom->leds.push_back(custom_led);
                }

                dummy_custom->zones.push_back(custom_zone);
            }

            dummy_custom->SetupColors();

            rgb_controllers.push_back(dummy_custom);
        }
    }

}   /* DetectDebugControllers() */

REGISTER_DETECTOR("Debug Controllers", DetectDebugControllers);
