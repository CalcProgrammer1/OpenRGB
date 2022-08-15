/*---------------------------------------------------------------------*\
|  RGBController_CorsairV2SoftwareController.cpp                        |
|                                                                       |
|  Common driver for the newer Corsair peripherals that use             |
|    the `08` based USB protocol                                        |
|                                                                       |
|  Chris M (Dr_No)          11 Aug 2022                                 |
\*---------------------------------------------------------------------*/

#include "LogManager.h"
#include "RGBController_CorsairV2Software.h"

using namespace std::chrono_literals;

/**------------------------------------------------------------------*\
    @name Corsair K55 RGB Pro
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectCorsairK55RGBPROControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_CorsairV2SW::RGBController_CorsairV2SW(CorsairPeripheralV2Controller *controller_ptr)
{
    controller                      = controller_ptr;
    const corsair_device* corsair   = controller->GetDeviceData();

    vendor                          = "Corsair";
    description                     = controller->GetName();
    type                            = corsair->type;
    version                         = controller->GetFirmwareString();
    location                        = controller->GetDeviceLocation();
    serial                          = controller->GetSerialString();

    if(corsair->supports & CORSAIR_V2_MODE_SW)
    {
        mode Direct;
        Direct.name                 = "Direct";
        Direct.value                = CORSAIR_V2_MODE_DIRECT;
        Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
        Direct.color_mode           = MODE_COLORS_PER_LED;
        modes.push_back(Direct);
    }

    if(corsair->supports & CORSAIR_V2_MODE_HW)
    {
        mode Static;
        Static.name                 = "Static";
        Static.value                = CORSAIR_V2_MODE_STATIC;
        Static.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Static.colors_min           = 1;
        Static.colors_max           = 1;
        Static.colors.resize(Static.colors_max);
        Static.brightness_min       = CORSAIR_V2_BRIGHTNESS_MIN;
        Static.brightness_max       = CORSAIR_V2_BRIGHTNESS_MAX;
        Static.brightness           = CORSAIR_V2_BRIGHTNESS_MAX;
        Static.color_mode           = MODE_COLORS_MODE_SPECIFIC;
        modes.push_back(Static);
    }

    SetupZones();
    /*-----------------------------------------------------*\
    | The Corsair K55 RGB PRO requires a packet within      |
    | 1 minutes of sending the lighting change in order     |
    | to not revert back into rainbow mode.  Start a thread |
    | to continuously send a keepalive packet every 50 sec  |
    \*-----------------------------------------------------*/
    keepalive_thread_run            = true;
    keepalive_thread                = new std::thread(&RGBController_CorsairV2SW::KeepaliveThread, this);
}

RGBController_CorsairV2SW::~RGBController_CorsairV2SW()
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

void RGBController_CorsairV2SW::SetupZones()
{
    const corsair_device* corsair   = controller->GetDeviceData();

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
            new_zone.leds_count             = corsair->zones[i]->rows * corsair->zones[i]->cols;

            if(new_zone.type == ZONE_TYPE_MATRIX)
            {
                matrix_map_type * new_map   = new matrix_map_type;
                new_zone.matrix_map         = new_map;

                new_map->height             = corsair->zones[i]->rows;
                new_map->width              = corsair->zones[i]->cols;

                new_map->map = new unsigned int[new_map->height * new_map->width];

                for(unsigned int y = 0; y < new_map->height; y++)
                {
                    for(unsigned int x = 0; x < new_map->width; x++)
                    {
                        new_map->map[(y * new_map->width) + x] = (y * new_map->width) + x;
                    }
                }

                /*---------------------------------------------------------*\
                | Create LEDs for the Matrix zone                           |
                \*---------------------------------------------------------*/
                uint8_t blanks              = 0;
                for(size_t led_idx = 0; led_idx < new_zone.leds_count; led_idx++)
                {
                    led  new_led;
                    bool not_found = true;

                    for(size_t layout_index = 0; layout_index < corsair->layout_size; layout_index++)
                    {
                        uint8_t row         = layout_index / new_map->width;
                        uint8_t col         = layout_index % new_map->width;

                        if(  i == corsair->layout[layout_index].zone    &&
                           row == corsair->layout[layout_index].row     &&
                           col == corsair->layout[layout_index].col     )
                        {
                            new_led.name    = corsair->layout[layout_index].name;
                            not_found       = false;
                            break;
                        }
                    }

                    /*-----------------------------------------------------------------*\
                    | If this is the "Keyboard" zone and key was not found in the map   |
                    |   then change the value of the key to hide it from view           |
                    \*-----------------------------------------------------------------*/
                    if(not_found && new_zone.name == ZONE_EN_KEYBOARD)
                    {
                        new_zone.matrix_map->map[led_idx] = NA;
                        blanks++;
                    }
                    else
                    {
                        leds.push_back(new_led);
                    }
                }

                new_zone.leds_count        -= blanks;
            }
            else
            {
                new_zone.matrix_map         = NULL;

                /*---------------------------------------------------------*\
                | Create LEDs for the Linear / Single zone                  |
                \*---------------------------------------------------------*/
                for(size_t led_idx = 0; led_idx < new_zone.leds_count; led_idx++)
                {
                    led new_led;

                    new_led.name            = new_zone.name + " ";
                    new_led.name.append(std::to_string( led_idx ));
                    new_led.value           = leds.size();

                    leds.push_back(new_led);
                }
            }

            LOG_DEBUG("[%s] Creating a %s zone: %s with %d LEDs", name.c_str(),
                      ((new_zone.type == ZONE_TYPE_MATRIX) ? "matrix": "linear"),
                      new_zone.name.c_str(), new_zone.leds_count);
            new_zone.leds_min               = new_zone.leds_count;
            new_zone.leds_max               = new_zone.leds_count;
            zones.push_back(new_zone);
        }
    }

    SetupColors();
}

void RGBController_CorsairV2SW::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_CorsairV2SW::DeviceUpdateLEDs()
{
    last_update_time = std::chrono::steady_clock::now();

    controller->SetLedsDirect(colors);
}

void RGBController_CorsairV2SW::UpdateZoneLEDs(int /*zone*/)
{
    controller->SetLedsDirect(colors);
}

void RGBController_CorsairV2SW::UpdateSingleLED(int /*led*/)
{
    controller->SetLedsDirect(colors);
}

void RGBController_CorsairV2SW::DeviceUpdateMode()
{

}

void RGBController_CorsairV2SW::KeepaliveThread()
{
    while(keepalive_thread_run.load())
    {
        if(active_mode == 0)
        {
            if((std::chrono::steady_clock::now() - last_update_time) > std::chrono::milliseconds(50000))
            {
                DeviceUpdateLEDs();
            }
        }
        std::this_thread::sleep_for(3000ms);
    }
}
