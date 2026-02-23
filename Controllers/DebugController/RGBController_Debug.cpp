/*---------------------------------------------------------*\
| RGBController_Debug.cpp                                   |
|                                                           |
|   Debug RGBController that can mimic various devices for  |
|   development and test purposes                           |
|                                                           |
|   Adam Honse (CalcProgrammer1)                31 Jul 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <algorithm>
#include <cstring>
#include "KeyboardLayoutManager.h"
#include "RGBController_Debug.h"

/**------------------------------------------------------------------*\
    @name Debug
    @category Unknown
    @type I2C
    @save :x:
    @direct :x:
    @effects :x:
    @detectors DetectDebugControllers
    @comment
\*-------------------------------------------------------------------*/

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

#define NUM_LAYOUTS 6

static const std::string layout_names[] =
{
    "Default",
    "ANSI QWERTY",
    "ISO QWERTY",
    "ISO QWERTZ",
    "ISO AZERTY",
    "JIS"
};

static unsigned int debug_keyboard_underglow_map[3][10] =
    { {    0,   1,   2,   3,   4,   5,   6,   7,   8,   9 },
      {   10,  11,  12,  13,  14,  15,  16,  17,  18,  19 },
      {   20,  21,  22,  23,  24,  25,  26,  27,  28,  29 } };

RGBController_Debug::RGBController_Debug(bool custom, json settings)
{
    custom_controller   = custom;
    debug_settings      = settings;

    if(custom_controller)
    {
        /*-------------------------------------------------*\
        | Set the name                                      |
        \*-------------------------------------------------*/
        name = debug_settings["DeviceName"];

        /*-------------------------------------------------*\
        | Find the device type                              |
        \*-------------------------------------------------*/
        if      (debug_settings["DeviceType"] == "motherboard")   type = DEVICE_TYPE_MOTHERBOARD;
        else if (debug_settings["DeviceType"] == "dram")          type = DEVICE_TYPE_DRAM;
        else if (debug_settings["DeviceType"] == "gpu")           type = DEVICE_TYPE_GPU;
        else if (debug_settings["DeviceType"] == "cooler")        type = DEVICE_TYPE_COOLER;
        else if (debug_settings["DeviceType"] == "led_strip")     type = DEVICE_TYPE_LEDSTRIP;
        else if (debug_settings["DeviceType"] == "keyboard")      type = DEVICE_TYPE_KEYBOARD;
        else if (debug_settings["DeviceType"] == "mouse")         type = DEVICE_TYPE_MOUSE;
        else if (debug_settings["DeviceType"] == "mousemat")      type = DEVICE_TYPE_MOUSEMAT;
        else if (debug_settings["DeviceType"] == "headset")       type = DEVICE_TYPE_HEADSET;
        else if (debug_settings["DeviceType"] == "headset_stand") type = DEVICE_TYPE_HEADSET_STAND;
        else if (debug_settings["DeviceType"] == "gamepad")       type = DEVICE_TYPE_GAMEPAD;
        else if (debug_settings["DeviceType"] == "light")         type = DEVICE_TYPE_LIGHT;
        else if (debug_settings["DeviceType"] == "speaker")       type = DEVICE_TYPE_SPEAKER;
        else if (debug_settings["DeviceType"] == "unknown")       type = DEVICE_TYPE_UNKNOWN;

        /*-------------------------------------------------*\
        | Set description, location, version, and serial    |
        \*-------------------------------------------------*/
        description                             = debug_settings["DeviceDescription"];
        location                                = debug_settings["DeviceLocation"];
        version                                 = debug_settings["DeviceVersion"];
        serial                                  = debug_settings["DeviceSerial"];
    }
    else
    {
        std::string name_setting    = "";
        std::string type_setting    = "keyboard";

        if(debug_settings.contains("name"))
        {
            name_setting = debug_settings["name"];
        }

        if(debug_settings.contains("type"))
        {
            type_setting = debug_settings["type"];
        }

        if(type_setting == "motherboard")
        {
            name                = "Debug Motherboard";
            type                = DEVICE_TYPE_MOTHERBOARD;
        }
        else if(type_setting == "dram")
        {
            name                = "Debug DRAM";
            type                = DEVICE_TYPE_DRAM;
        }
        else if(type_setting == "gpu")
        {
            name                = "Debug GPU";
            type                = DEVICE_TYPE_GPU;
        }
        else if(type_setting == "keyboard")
        {
            name                = "Debug Keyboard";
            type                = DEVICE_TYPE_KEYBOARD;
        }
      else if(type_setting == "mouse")
        {
            name                = "Debug Mouse";
            type                = DEVICE_TYPE_MOUSE;
        }
        else if(type_setting == "argb")
        {
            name                = "Debug ARGB Controller";
            type                = DEVICE_TYPE_LEDSTRIP;
        }

        /*---------------------------------------------------------*\
        | Fill in debug controller information                      |
        \*---------------------------------------------------------*/
        description                     = name + " Device";
        vendor                          = name + " Vendor String";
        location                        = name + " Location String";
        version                         = name + " Version String";
        serial                          = name + " Serial String";

        if(name_setting != "")
        {
            name                        = name_setting;
        }
    }

    /*-----------------------------------------------------*\
    | Create the mode                                       |
    \*-----------------------------------------------------*/
    mode Direct;

    Direct.name                         = "Direct";
    Direct.value                        = 0;
    Direct.flags                        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode                   = MODE_COLORS_PER_LED;

    modes.push_back(Direct);

    SetupZones();
}

RGBController_Debug::~RGBController_Debug()
{
    Shutdown();
}

void RGBController_Debug::SetupZones()
{
    /*-------------------------------------------------*\
    | Only set LED count on the first run               |
    \*-------------------------------------------------*/
    bool        first_run   = false;
    std::size_t zone_idx    = 0;

    if(zones.size() == 0)
    {
        first_run = true;
    }

    /*-------------------------------------------------*\
    | Clear any existing color/LED configuration        |
    \*-------------------------------------------------*/
    leds.clear();
    colors.clear();

    if(custom_controller)
    {
        /*-------------------------------------------------*\
        | Fill in zones                                     |
        \*-------------------------------------------------*/
        for(int ZoneID = 0; ZoneID < (int)debug_settings["DeviceZones"].size(); ZoneID++)
        {
            json ZoneJson = debug_settings["DeviceZones"][ZoneID];

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

            /*---------------------------------------------*\
            | Fill in the matrix map                        |
            \*---------------------------------------------*/
            bool BadVal = false;

            if(custom_zone.type == ZONE_TYPE_MATRIX)
            {
                if
                (
                    !ZoneJson.contains("matrix_height") ||
                    !ZoneJson.contains("matrix_width")  ||
                    !ZoneJson.contains("matrix_map")
                )
                {
                    /*-------------------------------------*\
                    | If there is no map then the zone      |
                    | can't be valid. Don't add it          |
                    \*-------------------------------------*/
                    continue;
                }

                unsigned int H                  = ZoneJson["matrix_width"];
                unsigned int W                  = ZoneJson["matrix_height"];

                BadVal                          = ((unsigned int)ZoneJson["matrix_map"].size() != H);

                unsigned int* MatrixARR         = new unsigned int[H * W];

                for(unsigned int MatrixMapRow = 0; MatrixMapRow < H; MatrixMapRow++)
                {
                    /*-------------------------------------*\
                    | If something went wrong then make no  |
                    | attempt to recover and just move on   |
                    | in a way that doesn't crash.  Even 1  |
                    | bad row can corrupt the map so skip   |
                    | the zone entirely                     |
                    \*-------------------------------------*/
                    if((W != (unsigned int)ZoneJson["matrix_map"][MatrixMapRow].size()) || BadVal)
                    {
                        BadVal = true;
                        break;
                    }

                    for(unsigned int MatrixMapCol = 0; MatrixMapCol < W; MatrixMapCol++)
                    {
                        unsigned int Val = ZoneJson["matrix_map"][MatrixMapRow][MatrixMapCol];

                        if((signed)Val == -1)
                        {
                            MatrixARR[MatrixMapRow * W + MatrixMapCol] = NA;
                        }
                        else
                        {
                            MatrixARR[MatrixMapRow * W + MatrixMapCol] = Val;
                        }
                    }
                }

                custom_zone.matrix_map.Set(H, W, MatrixARR);
            }

            /*---------------------------------------------*\
            | Don't add the zone if it is invalid           |
            \*---------------------------------------------*/
            if(BadVal)
            {
                continue;
            }

            bool UseCustomLabels = false;
            if(ZoneJson.contains("custom_labels"))
            {
                /*-----------------------------------------*\
                | If the count is correct and the zone is   |
                | non-resizeable                            |
                \*-----------------------------------------*/
                if((ZoneJson["custom_labels"].size() == custom_zone.leds_count) && (custom_zone.leds_min == custom_zone.leds_max))
                {
                    UseCustomLabels = true;
                }
            }

            /*---------------------------------------------*\
            | Set the LED names                             |
            \*---------------------------------------------*/
            for(int LED_ID = 0; LED_ID < (int)custom_zone.leds_count; LED_ID++)
            {
                led custom_led;
                if(UseCustomLabels)
                {
                    /*-------------------------------------*\
                    | Set the label to the user defined     |
                    | label                                 |
                    \*-------------------------------------*/
                    custom_led.name = ZoneJson["custom_labels"][LED_ID];
                }
                else
                {
                    /*-------------------------------------*\
                    | Set default labels because something  |
                    | went wrong                            |
                    \*-------------------------------------*/
                    custom_led.name = ("Custom LED. Zone " + std::to_string(ZoneID) + ", LED " + std::to_string(LED_ID));
                }

                leds.push_back(custom_led);
            }

            zones.push_back(custom_zone);
        }
    }
    else
    {
        bool    zone_single             = true;
        bool    zone_linear             = true;
        bool    zone_resizable          = false;
        bool    zone_keyboard           = false;
        bool    zone_underglow          = false;

        if(debug_settings.contains("single"))
        {
            zone_single = debug_settings["single"];
        }

        if(debug_settings.contains("linear"))
        {
            zone_linear = debug_settings["linear"];
        }

        if(debug_settings.contains("resizable"))
        {
            zone_resizable = debug_settings["resizable"];
        }

        if(debug_settings.contains("keyboard"))
        {
            zone_keyboard = debug_settings["keyboard"];
        }

        if(debug_settings.contains("underglow"))
        {
            zone_underglow = debug_settings["underglow"];
        }

        /*-------------------------------------------------*\
        | Create a single zone/LED                          |
        \*-------------------------------------------------*/
        if(zone_single)
        {
            zone single_zone;

            single_zone.name            = "Single Zone";
            single_zone.type            = ZONE_TYPE_SINGLE;
            single_zone.leds_min        = 1;
            single_zone.leds_max        = 1;
            single_zone.leds_count      = 1;

            if(first_run)
            {
                zones.push_back(single_zone);
            }
            else
            {
                zones[zone_idx] = single_zone;
            }

            led single_led;

            single_led.name             = "Single LED";

            leds.push_back(single_led);

            led_alt_names.push_back("");

            zone_idx++;
        }

        /*-------------------------------------------------*\
        | Create a linear zone                              |
        \*-------------------------------------------------*/
        if(zone_linear)
        {
            zone linear_zone;

            linear_zone.name            = "Linear Zone";
            linear_zone.type            = ZONE_TYPE_LINEAR;
            linear_zone.leds_min        = 10;
            linear_zone.leds_max        = 10;
            linear_zone.leds_count      = 10;

            if(first_run)
            {
                zones.push_back(linear_zone);
            }
            else
            {
                zones[zone_idx] = linear_zone;
            }

            for(std::size_t led_idx = 0; led_idx < 10; led_idx++)
            {
                led linear_led;

                linear_led.name         = "Linear LED " + std::to_string(led_idx);

                leds.push_back(linear_led);

                led_alt_names.push_back("");
            }

            zone_idx++;
        }

        /*-------------------------------------------------*\
        | Create a keyboard matrix zone                     |
        \*-------------------------------------------------*/
        if(zone_keyboard)
        {
            KEYBOARD_LAYOUT layout              = KEYBOARD_LAYOUT::KEYBOARD_LAYOUT_ANSI_QWERTY;
            KEYBOARD_SIZE   size                = KEYBOARD_SIZE::KEYBOARD_SIZE_FULL;

            if(debug_settings.contains("layout"))
            {
                KEYBOARD_LAYOUT temp_layout = debug_settings["layout"];

                if(temp_layout < NUM_LAYOUTS)
                {
                    layout = temp_layout;
                }
            }

            if(debug_settings.contains("size"))
            {
                size = debug_settings["size"];
            }

            KeyboardLayoutManager new_kb(layout, size);

            description                        += ", Layout: " + layout_names[layout] + ", Size: " + new_kb.GetName();

            /*---------------------------------------------*\
            | Check for custom key inserts and swaps        |
            \*---------------------------------------------*/
            const char* change_keys             = "change_keys";

            if(debug_settings.contains(change_keys))
            {
                std::vector<keyboard_led> change;

                const char* ins_row             = "ins_row";
                const char* rmv_key             = "rmv_key";
                const char* rmv_row             = "rmv_row";
                const char* swp_key             = "swp_key";

                const char* dbg_zone            = "Zone";
                const char* dbg_row             = "Row";
                const char* dbg_col             = "Col";
                const char* dbg_val             = "Val";
                const char* dbg_name            = "Name";
                const char* dbg_opcode          = "Opcode";

                for(size_t i = 0; i < debug_settings[change_keys].size(); i++)
                {
                    keyboard_led* key           = new keyboard_led;

                    key->zone                   = debug_settings[change_keys][i][dbg_zone];
                    key->row                    = debug_settings[change_keys][i][dbg_row];
                    key->col                    = debug_settings[change_keys][i][dbg_col];
                    key->value                  = debug_settings[change_keys][i][dbg_val];
                    key->name                   = debug_settings[change_keys][i][dbg_name].get_ref<const std::string&>().c_str();

                    if(debug_settings[change_keys][i][dbg_opcode] == ins_row)
                    {
                        key->opcode             = KEYBOARD_OPCODE_INSERT_ROW;
                    }
                    else if(debug_settings[change_keys][i][dbg_opcode] == rmv_key)
                    {
                        key->opcode             = KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT;
                    }
                    else if(debug_settings[change_keys][i][dbg_opcode] == rmv_row)
                    {
                        key->opcode             = KEYBOARD_OPCODE_REMOVE_ROW;
                    }
                    else if(debug_settings[change_keys][i][dbg_opcode] == swp_key)
                    {
                        key->opcode             = KEYBOARD_OPCODE_SWAP_ONLY;
                    }
                    else
                    {
                        key->opcode             = KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT;
                    }

                    change.push_back(*key);
                }

                new_kb.ChangeKeys(change);
            }

            zone keyboard_zone;

            keyboard_zone.name                  = "Keyboard Zone";
            keyboard_zone.type                  = ZONE_TYPE_MATRIX;
            keyboard_zone.leds_min              = new_kb.GetKeyCount();
            keyboard_zone.leds_max              = new_kb.GetKeyCount();
            keyboard_zone.leds_count            = new_kb.GetKeyCount();
            keyboard_zone.matrix_map = new_kb.GetKeyMap(KEYBOARD_MAP_FILL_TYPE_COUNT);

            if(first_run)
            {
                zones.push_back(keyboard_zone);
            }
            else
            {
                zones[zone_idx] = keyboard_zone;
            }

            for(unsigned int led_idx = 0; led_idx < keyboard_zone.leds_count; led_idx++)
            {
                led keyboard_led;

                keyboard_led.name = new_kb.GetKeyNameAt(led_idx);

                leds.push_back(keyboard_led);

                led_alt_names.push_back(new_kb.GetKeyAltNameAt(led_idx));
            }

            zone_idx++;
        }

        /*-------------------------------------------------*\
        | Create an underglow matrix zone                   |
        \*-------------------------------------------------*/
        if(zone_underglow)
        {
            zone underglow_zone;

            underglow_zone.name                 = "Underglow Zone";
            underglow_zone.type                 = ZONE_TYPE_MATRIX;
            underglow_zone.leds_min             = 30;
            underglow_zone.leds_max             = 30;
            underglow_zone.leds_count           = 30;
            underglow_zone.matrix_map.Set(3, 10, (unsigned int*)&debug_keyboard_underglow_map);

            if(first_run)
            {
                zones.push_back(underglow_zone);
            }
            else
            {
                zones[zone_idx] = underglow_zone;
            }

            for(std::size_t led_idx = 0; led_idx < underglow_zone.leds_count; led_idx++)
            {
                led underglow_led;

                underglow_led.name              = "Underglow LED "  + std::to_string(led_idx);;

                leds.push_back(underglow_led);

                led_alt_names.push_back("");
            }

            zone_idx++;
        }

        /*-------------------------------------------------*\
        | Create a resizable linear zone                    |
        \*-------------------------------------------------*/
        if(zone_resizable)
        {
            zone resizable_zone;

            resizable_zone.leds_min     = 0;
            resizable_zone.leds_max     = 100;

            if(first_run)
            {
                resizable_zone.flags    = ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE
                                        | ZONE_FLAG_MANUALLY_CONFIGURABLE_NAME
                                        | ZONE_FLAG_MANUALLY_CONFIGURABLE_TYPE
                                        | ZONE_FLAG_MANUALLY_CONFIGURABLE_MATRIX_MAP;
                zones.push_back(resizable_zone);
            }

            if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_NAME))
            {
                zones[zone_idx].name        = "Resizable Zone";
            }

            if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_SIZE))
            {
                zones[zone_idx].leds_count  = 0;
            }

            if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_TYPE))
            {
                zones[zone_idx].type        = ZONE_TYPE_LINEAR;
            }

            if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_MATRIX_MAP))
            {
                zones[zone_idx].matrix_map.width    = 0;
                zones[zone_idx].matrix_map.height   = 0;
                zones[zone_idx].matrix_map.map.resize(0);
            }

            for(std::size_t led_idx = 0; led_idx < zones[zone_idx].leds_count; led_idx++)
            {
                led resizable_led;

                resizable_led.name          = zones[zone_idx].name + ", LED " + std::to_string(led_idx);

                leds.push_back(resizable_led);

                led_alt_names.push_back("");
            }

            zone_idx++;
        }
    }

    SetupColors();
}

void RGBController_Debug::DeviceConfigureZone(int zone_idx)
{
    if((size_t)zone_idx < zones.size())
    {
        SetupZones();
    }
}

void RGBController_Debug::DeviceUpdateLEDs()
{

}

void RGBController_Debug::DeviceUpdateZoneLEDs(int /*zone*/)
{

}

void RGBController_Debug::DeviceUpdateSingleLED(int /*led*/)
{

}

void RGBController_Debug::DeviceUpdateMode()
{

}
