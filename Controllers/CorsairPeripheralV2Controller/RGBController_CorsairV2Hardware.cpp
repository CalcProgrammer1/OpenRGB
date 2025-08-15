/*---------------------------------------------------------*\
| RGBController_CorsairV2HardwareController.cpp             |
|                                                           |
|   RGBController for Corsair V2 peripherals - hardware     |
|   modes                                                   |
|                                                           |
|   Chris M (Dr_No)                             10 Dec 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "LogManager.h"
#include "RGBController_CorsairV2Hardware.h"

using namespace std::chrono_literals;

/**------------------------------------------------------------------*\
    @name Corsair Peripherals V2 Hardware
    @category Keyboard
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectCorsairV2HardwareControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_CorsairV2HW::RGBController_CorsairV2HW(CorsairPeripheralV2Controller *controller_ptr)
{
    controller                          = controller_ptr;
    const corsair_v2_device* corsair    = controller->GetDeviceData();

    name                                = controller->GetName();
    vendor                              = "Corsair";
    description                         = "Corsair Peripheral V2 HW Device";
    type                                = corsair->type;
    version                             = controller->GetFirmwareString();
    location                            = controller->GetDeviceLocation();
    serial                              = controller->GetSerialString();

    mode Direct;
    Direct.name                         = "Direct";
    Direct.value                        = CORSAIR_V2_MODE_DIRECT;
    Direct.flags                        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode                   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                         = "Static";
    Static.value                        = CORSAIR_V2_MODE_STATIC;
    Static.flags                        = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Static.colors_min                   = 1;
    Static.colors_max                   = 1;
    Static.colors.resize(Static.colors_max);
    Static.brightness_min               = CORSAIR_V2_BRIGHTNESS_MIN;
    Static.brightness_max               = CORSAIR_V2_BRIGHTNESS_MAX;
    Static.brightness                   = CORSAIR_V2_BRIGHTNESS_MAX;
    Static.color_mode                   = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Static);

    SetupZones();
    /*-----------------------------------------------------*\
    | The Corsair K55 RGB PRO requires a packet within      |
    | 1 minutes of sending the lighting change in order     |
    | to not revert back into rainbow mode.  Start a thread |
    | to continuously send a keepalive packet every 50 sec  |
    \*-----------------------------------------------------*/
    keepalive_thread_run                = true;
    keepalive_thread                    = new std::thread(&RGBController_CorsairV2HW::KeepaliveThread, this);
}

RGBController_CorsairV2HW::~RGBController_CorsairV2HW()
{
    /*-----------------------------------------------------*\
    | Close keepalive thread                                |
    \*-----------------------------------------------------*/
    keepalive_thread_run = false;
    keepalive_thread->join();
    delete keepalive_thread;

    /*---------------------------------------------------------*\
    | Delete the matrix map                                     |
    \*---------------------------------------------------------*/
    for(unsigned int zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        if(zones[zone_index].type == ZONE_TYPE_MATRIX)
        {
            delete zones[zone_index].matrix_map;
        }
    }

    delete controller;
}

void RGBController_CorsairV2HW::SetupZones()
{
    std::string physical_size;
    KEYBOARD_LAYOUT new_layout;
    unsigned int max_led_value              = 0;
    const corsair_v2_device* corsair        = controller->GetDeviceData();
    unsigned int layout                     = controller->GetKeyboardLayout();

    switch(layout)
    {
        case CORSAIR_V2_KB_LAYOUT_ISO:
            new_layout = KEYBOARD_LAYOUT_ISO_QWERTY;
            break;

        case CORSAIR_V2_KB_LAYOUT_JIS:
            new_layout = KEYBOARD_LAYOUT_JIS;
            break;

        case CORSAIR_V2_KB_LAYOUT_ANSI:
        case CORSAIR_V2_KB_LAYOUT_ABNT:
        default:
            new_layout = KEYBOARD_LAYOUT_ANSI_QWERTY;
            break;
    }

    /*---------------------------------------------------------*\
    | Fill in zones from the device data                        |
    \*---------------------------------------------------------*/
    for(size_t i = 0; i < CORSAIR_ZONES_MAX; i++)
    {
        if(corsair->zones[i] == NULL)
        {
            break;
        }
        else
        {
            zone new_zone;

            new_zone.name                   = corsair->zones[i]->name;
            new_zone.type                   = corsair->zones[i]->type;

            if(new_zone.type == ZONE_TYPE_MATRIX)
            {
                KeyboardLayoutManager new_kb(new_layout, corsair->layout_new->base_size, corsair->layout_new->key_values);

                matrix_map_type * new_map   = new matrix_map_type;
                new_zone.matrix_map         = new_map;

                new_map->height             = corsair->zones[i]->rows;
                new_map->width              = corsair->zones[i]->cols;
                new_map->map                = new unsigned int[new_map->height * new_map->width];

                if(corsair->layout_new->base_size != KEYBOARD_SIZE_EMPTY)
                {
                    /*---------------------------------------------------------*\
                    | Minor adjustments to keyboard layout                      |
                    \*---------------------------------------------------------*/
                    keyboard_keymap_overlay_values* temp    = corsair->layout_new;
                    new_kb.ChangeKeys(*temp);

                    /*---------------------------------------------------------*\
                    | Matrix map still uses declared zone rows and columns      |
                    |   as the packet structure depends on the matrix map       |
                    \*---------------------------------------------------------*/
                    new_kb.GetKeyMap(new_map->map, KEYBOARD_MAP_FILL_TYPE_COUNT, new_map->height, new_map->width);

                    /*---------------------------------------------------------*\
                    | Create LEDs for the Matrix zone                           |
                    |   Place keys in the layout to populate the matrix         |
                    \*---------------------------------------------------------*/
                    new_zone.leds_count                     = new_kb.GetKeyCount();
                    LOG_DEBUG("[%s] Created KB matrix with %d rows and %d columns containing %d keys",
                              controller->GetName().c_str(), new_kb.GetRowCount(), new_kb.GetColumnCount(), new_zone.leds_count);

                    for(unsigned int led_idx = 0; led_idx < new_zone.leds_count; led_idx++)
                    {
                        led new_led;

                        new_led.name                = new_kb.GetKeyNameAt(led_idx);
                        new_led.value               = new_kb.GetKeyValueAt(led_idx);
                        max_led_value               = std::max(max_led_value, new_led.value);
                        leds.push_back(new_led);
                    }
                }

                /*---------------------------------------------------------*\
                | Add 1 the max_led_value to account for the 0th index      |
                \*---------------------------------------------------------*/
                max_led_value++;
            }
            else
            {
                new_zone.leds_count             = corsair->zones[i]->rows * corsair->zones[i]->cols;
                new_zone.matrix_map             = NULL;

                /*---------------------------------------------------------*\
                | Create LEDs for the Linear / Single zone                  |
                \*---------------------------------------------------------*/
                for(size_t led_idx = 0; led_idx < new_zone.leds_count; led_idx++)
                {
                    led new_led;

                    new_led.name                = new_zone.name + " ";
                    new_led.name.append(std::to_string( led_idx ));
                    new_led.value               = (unsigned int)leds.size();

                    leds.push_back(new_led);
                }

                max_led_value                   = std::max(max_led_value, (unsigned int)leds.size());
            }

            /*---------------------------------------------------------*\
            | name is not set yet so description is used instead        |
            \*---------------------------------------------------------*/
            LOG_DEBUG("[%s] Creating a %s zone: %s with %d LEDs", description.c_str(),
                      ((new_zone.type == ZONE_TYPE_MATRIX) ? "matrix": "linear"),
                      new_zone.name.c_str(), new_zone.leds_count);
            new_zone.leds_min                   = new_zone.leds_count;
            new_zone.leds_max                   = new_zone.leds_count;
            zones.push_back(new_zone);
        }
    }

    SetupColors();

    /*---------------------------------------------------------*\
    | Create a buffer map of pointers which contains the        |
    |   layout order of colors the device expects.              |
    \*---------------------------------------------------------*/
    for(size_t led_idx = 0; led_idx < max_led_value; led_idx++)
    {
        buffer_map.push_back(&null_color);
    }

    for(size_t led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        buffer_map[leds[led_idx].value] = &colors[led_idx];
    }
}

void RGBController_CorsairV2HW::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_CorsairV2HW::DeviceUpdateLEDs()
{
    last_update_time = std::chrono::steady_clock::now();

    controller->SetLedsDirect(buffer_map);
}

void RGBController_CorsairV2HW::UpdateZoneLEDs(int /*zone*/)
{
    controller->SetLedsDirect(buffer_map);
}

void RGBController_CorsairV2HW::UpdateSingleLED(int /*led*/)
{
    controller->SetLedsDirect(buffer_map);
}

void RGBController_CorsairV2HW::DeviceUpdateMode()
{

}

void RGBController_CorsairV2HW::KeepaliveThread()
{
    while(keepalive_thread_run.load())
    {
        if(active_mode == 0)
        {
            if((std::chrono::steady_clock::now() - last_update_time) >
                std::chrono::milliseconds(CORSAIR_V2_UPDATE_PERIOD))
            {
                DeviceUpdateLEDs();
            }
        }
        std::this_thread::sleep_for(CORSAIR_V2_SLEEP_PERIOD);
    }
}
