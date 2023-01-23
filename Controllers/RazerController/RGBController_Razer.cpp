#include "RGBController_Razer.h"
#include "RazerDevices.h"

/**------------------------------------------------------------------*\
    @name Razer
    @category Keyboard,Microphone,Mouse,Mousemat,HeadsetStand,Case,GPU,Accessory
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectRazerControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_Razer::RGBController_Razer(RazerController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetName();
    vendor                  = "Razer";
    type                    = controller->GetDeviceType();
    description             = "Razer Device";
    location                = controller->GetDeviceLocation();
    version                 = controller->GetFirmwareString();
    serial                  = controller->GetSerialString();
    uint8_t max_brightness  = controller->GetMaxBrightness();

    if(type == DEVICE_TYPE_KEYBOARD)
    {
        std::string layout = controller->GetKeyboardLayoutName();

        description.append(", ");
        description.append(layout);
    }

    std::string variant = controller->GetVariantName();
    description.append(", ");
    description.append(variant);

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = RAZER_MODE_DIRECT;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    Direct.brightness_min   = 0;
    Direct.brightness_max   = max_brightness;
    Direct.brightness       = max_brightness;
    modes.push_back(Direct);

    mode Off;
    Off.name                = "Off";
    Off.value               = RAZER_MODE_OFF;
    Off.flags               = 0;
    Off.color_mode          = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name             = "Static";
    Static.value            = RAZER_MODE_STATIC;
    Static.flags            = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Static.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_min       = 1;
    Static.colors_max       = 1;
    Static.colors.resize(1);
    Static.brightness_min   = 0;
    Static.brightness_max   = max_brightness;
    Static.brightness       = max_brightness;
    modes.push_back(Static);

    if(controller->SupportsBreathing())
    {
        mode Breathing;
        Breathing.name           = "Breathing";
        Breathing.value          = RAZER_MODE_BREATHING;
        Breathing.flags          = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Breathing.color_mode     = MODE_COLORS_MODE_SPECIFIC;
        Breathing.colors_min     = 1;
        Breathing.colors_max     = 2;
        Breathing.colors.resize(1);
        Breathing.brightness_min = 0;
        Breathing.brightness_max = max_brightness;
        Breathing.brightness     = max_brightness;
        modes.push_back(Breathing);
    }

    mode SpectrumCycle;
    SpectrumCycle.name           = "Spectrum Cycle";
    SpectrumCycle.value          = RAZER_MODE_SPECTRUM_CYCLE;
    SpectrumCycle.flags          = MODE_FLAG_HAS_BRIGHTNESS;
    SpectrumCycle.color_mode     = MODE_COLORS_NONE;
    SpectrumCycle.brightness_min = 0;
    SpectrumCycle.brightness_max = max_brightness;
    SpectrumCycle.brightness     = max_brightness;
    modes.push_back(SpectrumCycle);

    if(controller->SupportsWave())
    {
        mode Wave;
        Wave.name               = "Wave";
        Wave.value              = RAZER_MODE_WAVE;
        Wave.flags              = MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_BRIGHTNESS;
        Wave.direction          = MODE_DIRECTION_RIGHT;
        Wave.color_mode         = MODE_COLORS_NONE;
        Wave.brightness_min     = 0;
        Wave.brightness_max     = max_brightness;
        Wave.brightness         = max_brightness;
        modes.push_back(Wave);
    }

    if(controller->SupportsReactive())
    {
        mode Reactive;
        Reactive.name           = "Reactive";
        Reactive.value          = RAZER_MODE_REACTIVE;
        Reactive.flags          = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Reactive.color_mode     = MODE_COLORS_MODE_SPECIFIC;
        Reactive.colors_min     = 1;
        Reactive.colors_max     = 1;
        Reactive.colors.resize(1);
        Reactive.brightness_min = 0;
        Reactive.brightness_max = max_brightness;
        Reactive.brightness     = max_brightness;
        modes.push_back(Reactive);
    }

    SetupZones();
}

RGBController_Razer::~RGBController_Razer()
{
    delete controller;
}

void RGBController_Razer::SetupZones()
{
    unsigned int device_index = controller->GetDeviceIndex();
    unsigned char layout_type = controller->GetKeyboardLayoutType();

    /*---------------------------------------------------------*\
    | Fill in zone information based on device table            |
    \*---------------------------------------------------------*/
    for(unsigned int zone_id = 0; zone_id < RAZER_MAX_ZONES; zone_id++)
    {
        if(device_list[device_index]->zones[zone_id] != NULL)
        {
            zone new_zone;

            new_zone.name       = device_list[device_index]->zones[zone_id]->name;
            new_zone.type       = device_list[device_index]->zones[zone_id]->type;

            new_zone.leds_count = device_list[device_index]->zones[zone_id]->rows * device_list[device_index]->zones[zone_id]->cols;
            new_zone.leds_min   = new_zone.leds_count;
            new_zone.leds_max   = new_zone.leds_count;

            if(new_zone.type == ZONE_TYPE_MATRIX)
            {
                matrix_map_type * new_map = new matrix_map_type;
                new_zone.matrix_map = new_map;

                new_map->height = device_list[device_index]->zones[zone_id]->rows;
                new_map->width  = device_list[device_index]->zones[zone_id]->cols;

                new_map->map = new unsigned int[new_map->height * new_map->width];

                for(unsigned int y = 0; y < new_map->height; y++)
                {
                    for(unsigned int x = 0; x < new_map->width; x++)
                    {
                        bool exists_in_keymap = false;

                        if(new_zone.name != ZONE_EN_KEYBOARD)
                        {
                            /*---------------------------------------------------------*\
                            | For zones other than the actual keyboard, we want all     |
                            | entries in the matrix visible in the LED view (such as    |
                            | underglow)                                                |
                            \*---------------------------------------------------------*/
                            exists_in_keymap = true;
                        }
                        else if(device_list[device_index]->keymap != NULL)
                        {
                            for(unsigned int i = 0; i < device_list[device_index]->keymap_size; i++)
                            {
                                razer_key key = device_list[device_index]->keymap[i];
                                if(zone_id == key.zone && y  == key.row  && x  == key.col && (key.layout & layout_type))
                                {
                                    exists_in_keymap = true;
                                    break;
                                }
                            }   
                        }
                        else
                        {
                            /*---------------------------------------------------------*\
                            | If the device has no keymap defined we want all entries in|
                            | the matrix to be visible in the LED view                  |
                            \*---------------------------------------------------------*/
                            exists_in_keymap = true;
                        }

                        if (exists_in_keymap)
                        {
                            new_map->map[(y * new_map->width) + x] = (y * new_map->width) + x;
                        }
                        else
                        {
                            new_map->map[(y * new_map->width) + x] = -1;
                        }
                    }
                }
            }
            else
            {
                new_zone.matrix_map = NULL;
            }

            zones.push_back(new_zone);
        }
    }

    for(unsigned int zone_id = 0; zone_id < zones.size(); zone_id++)
    {
        for (unsigned int row_id = 0; row_id < device_list[device_index]->zones[zone_id]->rows; row_id++)
        {
            for (unsigned int col_id = 0; col_id < device_list[device_index]->zones[zone_id]->cols; col_id++)
            {
                led* new_led = new led();

                new_led->name = device_list[device_index]->zones[zone_id]->name;

                if(zones[zone_id].leds_count > 1)
                {
                    new_led->name.append(" LED ");
                    new_led->name.append(std::to_string(col_id + 1));
                }

                if(device_list[device_index]->keymap != NULL)
                {
                    bool not_found = true;

                    for(unsigned int i = 0; i < device_list[device_index]->keymap_size; i++)
                    {
                        if(zone_id      == device_list[device_index]->keymap[i].zone &&
                           row_id       == device_list[device_index]->keymap[i].row  &&
                           col_id       == device_list[device_index]->keymap[i].col  &&
                           layout_type  &  device_list[device_index]->keymap[i].layout)
                        {
                            new_led->name = device_list[device_index]->keymap[i].name;
                            not_found = false;
                            break;
                        }
                    }

                    /*-----------------------------------------------------------------*\
                    | If this is the "Keyboard" zone and key was not found in the map   |
                    |   then change the value of the key to hide it from view           |
                    \*-----------------------------------------------------------------*/
                    if(not_found && zones[zone_id].name == ZONE_EN_KEYBOARD)
                    {
                        zones[zone_id].matrix_map->map[row_id * zones[zone_id].matrix_map->width + col_id] = NA;
                    }
                }

                leds.push_back(*new_led);
            }
        }
    }

    SetupColors();
}

void RGBController_Razer::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_Razer::DeviceUpdateLEDs()
{
    controller->SetLEDs(&colors[0]);
}

void RGBController_Razer::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_Razer::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_Razer::DeviceUpdateMode()
{
    switch(modes[active_mode].value)
    {
        case RAZER_MODE_OFF:
            controller->SetModeOff();
            break;

        case RAZER_MODE_STATIC:
            if(modes[active_mode].colors.size() == 1)
            {
                unsigned char red = RGBGetRValue(modes[active_mode].colors[0]);
                unsigned char grn = RGBGetGValue(modes[active_mode].colors[0]);
                unsigned char blu = RGBGetBValue(modes[active_mode].colors[0]);

                controller->SetModeStatic(red, grn, blu);
            }
            break;

        case RAZER_MODE_BREATHING:
            if(modes[active_mode].color_mode == MODE_COLORS_RANDOM)
            {
                controller->SetModeBreathingRandom();
            }
            else if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
            {
                if(modes[active_mode].colors.size() == 1)
                {
                    unsigned char red = RGBGetRValue(modes[active_mode].colors[0]);
                    unsigned char grn = RGBGetGValue(modes[active_mode].colors[0]);
                    unsigned char blu = RGBGetBValue(modes[active_mode].colors[0]);

                    controller->SetModeBreathingOneColor(red, grn, blu);
                }
                else if(modes[active_mode].colors.size() == 2)
                {
                    unsigned char red1 = RGBGetRValue(modes[active_mode].colors[0]);
                    unsigned char grn1 = RGBGetGValue(modes[active_mode].colors[0]);
                    unsigned char blu1 = RGBGetBValue(modes[active_mode].colors[0]);
                    unsigned char red2 = RGBGetRValue(modes[active_mode].colors[1]);
                    unsigned char grn2 = RGBGetGValue(modes[active_mode].colors[1]);
                    unsigned char blu2 = RGBGetBValue(modes[active_mode].colors[1]);

                    controller->SetModeBreathingTwoColors(red1, grn1, blu1, red2, grn2, blu2);
                }
            }
            break;

        case RAZER_MODE_SPECTRUM_CYCLE:
            controller->SetModeSpectrumCycle();
            break;

        case RAZER_MODE_WAVE:
            switch(modes[active_mode].direction)
            {
                case MODE_DIRECTION_LEFT:
                    controller->SetModeWave(2);
                    break;

                default:
                    controller->SetModeWave(1);
                    break;
            }
            break;
    }

    if(modes[active_mode].flags & MODE_FLAG_HAS_BRIGHTNESS)
    {
        controller->SetBrightness(modes[active_mode].brightness);
    }
    else
    {
        controller->SetBrightness(255);
    }
}
