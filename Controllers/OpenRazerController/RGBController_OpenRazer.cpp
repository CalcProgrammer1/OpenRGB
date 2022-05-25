/*-----------------------------------------*\
|  RGBController_OpenRazer.cpp              |
|                                           |
|  Generic RGB Interface for OpenRazer      |
|  kernel drivers for Chroma peripherals    |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/15/2019   |
\*-----------------------------------------*/

#include "RGBController_OpenRazer.h"
#include "OpenRazerDevices.h"

#include <fstream>
#include <unistd.h>

using namespace std::chrono_literals;

void RGBController_OpenRazer::DeviceUpdateLEDs()
{

    switch(matrix_type)
    {
        case RAZER_TYPE_MATRIX_FRAME:
        case RAZER_TYPE_MATRIX_NOFRAME:
        case RAZER_TYPE_MATRIX_STATIC:
            {
                char update_value = 1;

                for (unsigned int row = 0; row < matrix_rows; row++)
                {
                    unsigned int output_array_size;
                    unsigned int output_offset;
                    unsigned int row_offset = (row * matrix_cols);

                    if(matrix_type == RAZER_TYPE_MATRIX_FRAME)
                    {
                        output_array_size = 3 + (matrix_cols* 3);
                        output_offset = 3;
                    }
                    else
                    {
                        output_array_size = 3;
                        output_offset = 0;
                    }

                    char* output_array = new char[output_array_size];

                    if(matrix_type == RAZER_TYPE_MATRIX_FRAME)
                    {
                        output_array[0] = row;
                        output_array[1] = 0;
                        output_array[2] = matrix_cols - 1;
                    }

                    for(unsigned int col = 0; col < matrix_cols; col++)
                    {
                        unsigned int color_idx = col + row_offset;
                        output_array[(col * 3) + 0 + output_offset] = (char)RGBGetRValue(colors[color_idx]);
                        output_array[(col * 3) + 1 + output_offset] = (char)RGBGetGValue(colors[color_idx]);
                        output_array[(col * 3) + 2 + output_offset] = (char)RGBGetBValue(colors[color_idx]);
                    }

                    if(matrix_type == RAZER_TYPE_MATRIX_FRAME)
                    {
                        matrix_custom_frame.write(output_array, output_array_size);
                        matrix_custom_frame.flush();
                    }
                    else if(matrix_type == RAZER_TYPE_MATRIX_NOFRAME)
                    {
                        matrix_effect_custom.write(output_array, output_array_size);
                        matrix_effect_custom.flush();
                    }
                    else
                    {
                        matrix_effect_static.write(output_array, output_array_size);
                        matrix_effect_static.flush();
                    }

                    delete[] output_array;

                    std::this_thread::sleep_for(1ms);
                }

                if(matrix_type == RAZER_TYPE_MATRIX_FRAME)
                {
                    matrix_effect_custom.write(&update_value, 1);
                    matrix_effect_custom.flush();
                }
            }
            break;

        case RAZER_TYPE_NOMATRIX:
            {
                DeviceUpdateMode();
            }
            break;
    }
}

void RGBController_OpenRazer::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_OpenRazer::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_OpenRazer::SetupMatrixDevice(unsigned int rows, unsigned int cols)
{
    if(!matrix_custom_frame)
    {
        if(!matrix_effect_custom)
        {
            matrix_type = RAZER_TYPE_MATRIX_STATIC;
        }
        else
        {
            matrix_type = RAZER_TYPE_MATRIX_NOFRAME;
        }

        matrix_rows = 1;
        matrix_cols = 1;
    }
    else
    {
        matrix_type = RAZER_TYPE_MATRIX_FRAME;

        matrix_rows = rows;
        matrix_cols = cols;
    }
}

void RGBController_OpenRazer::SetupNonMatrixDevice()
{
    matrix_type = RAZER_TYPE_NOMATRIX;
}

void RGBController_OpenRazer::OpenFunctions(std::string dev_path)
{
    device_type.open(                  dev_path + "/device_type");
    device_serial.open(                dev_path + "/device_serial");
    firmware_version.open(             dev_path + "/firmware_version");

    matrix_custom_frame.open(          dev_path + "/matrix_custom_frame");
    matrix_brightness.open(            dev_path + "/matrix_brightness");

    matrix_effect_custom.open(         dev_path + "/matrix_effect_custom");
    matrix_effect_none.open(           dev_path + "/matrix_effect_none");
    matrix_effect_static.open(         dev_path + "/matrix_effect_static");
    matrix_effect_breath.open(         dev_path + "/matrix_effect_breath");
    matrix_effect_spectrum.open(       dev_path + "/matrix_effect_spectrum");
    matrix_effect_reactive.open(       dev_path + "/matrix_effect_reactive");
    matrix_effect_wave.open(           dev_path + "/matrix_effect_wave");

    logo_led_brightness.open(          dev_path + "/logo_led_brightness");
    logo_matrix_effect_none.open(      dev_path + "/logo_matrix_effect_none");
    logo_matrix_effect_static.open(    dev_path + "/logo_matrix_effect_static");
    logo_matrix_effect_breath.open(    dev_path + "/logo_matrix_effect_breath");
    logo_matrix_effect_spectrum.open(  dev_path + "/logo_matrix_effect_spectrum");
    logo_matrix_effect_reactive.open(  dev_path + "/logo_matrix_effect_reactive");

    scroll_led_brightness.open(        dev_path + "/scroll_led_brightness");
    scroll_matrix_effect_none.open(    dev_path + "/scroll_matrix_effect_none");
    scroll_matrix_effect_static.open(  dev_path + "/scroll_matrix_effect_static");
    scroll_matrix_effect_breath.open(  dev_path + "/scroll_matrix_effect_breath");
    scroll_matrix_effect_spectrum.open(dev_path + "/scroll_matrix_effect_spectrum");
    scroll_matrix_effect_reactive.open(dev_path + "/scroll_matrix_effect_reactive");

    left_led_brightness.open(          dev_path + "/left_led_brightness");
    left_matrix_effect_none.open(      dev_path + "/left_matrix_effect_none");
    left_matrix_effect_static.open(    dev_path + "/left_matrix_effect_static");
    left_matrix_effect_breath.open(    dev_path + "/left_matrix_effect_breath");
    left_matrix_effect_spectrum.open(  dev_path + "/left_matrix_effect_spectrum");
    left_matrix_effect_reactive.open(  dev_path + "/left_matrix_effect_reactive");
    left_matrix_effect_wave.open(      dev_path + "/left_matrix_effect_wave");

    right_led_brightness.open(         dev_path + "/right_led_brightness");
    right_matrix_effect_none.open(     dev_path + "/right_matrix_effect_none");
    right_matrix_effect_static.open(   dev_path + "/right_matrix_effect_static");
    right_matrix_effect_breath.open(   dev_path + "/right_matrix_effect_breath");
    right_matrix_effect_spectrum.open( dev_path + "/right_matrix_effect_spectrum");
    right_matrix_effect_reactive.open( dev_path + "/right_matrix_effect_reactive");
    right_matrix_effect_wave.open(     dev_path + "/right_matrix_effect_wave");

    backlight_led_effect.open(         dev_path + "/backlight_led_effect");
    backlight_led_rgb.open(            dev_path + "/backlight_led_rgb");
    backlight_led_state.open(          dev_path + "/backlight_led_state");

    logo_led_effect.open(              dev_path + "/logo_led_effect");
    logo_led_rgb.open(                 dev_path + "/logo_led_rgb");
    logo_led_state.open(               dev_path + "/logo_led_state");

    scroll_led_effect.open(            dev_path + "/scroll_led_effect");
    scroll_led_rgb.open(               dev_path + "/scroll_led_rgb");
    scroll_led_state.open(             dev_path + "/scroll_led_state");

    /*-----------------------------------------------------------------*\
    | The Naga Chroma (and possibly others) expose a useless            |
    | matrix_effect_custom interface because they use the matrix_ name  |
    | for the keypad LED.  Close this useless interface in this case.   |
    | We can detect this when there is a logo matrix or scroll matrix   |
    | at the same time as matrix_effect_custom.                         |
    \*-----------------------------------------------------------------*/
    if((logo_matrix_effect_none || scroll_matrix_effect_none) && matrix_effect_custom)
    {
        matrix_effect_custom.close();
        matrix_effect_custom.setstate(std::ios::failbit);
    }
}

/**------------------------------------------------------------------*\
    @name Openrazer
    @category Keyboard,Mouse,Mousemat,HeadsetStand
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectOpenRazerControllers
    @comment The Openrazer controller has been deprecated in favour of
        the in built Razer controller.
\*-------------------------------------------------------------------*/

RGBController_OpenRazer::RGBController_OpenRazer(std::string dev_path)
{
    /*-----------------------------------------------------------------*\
    | Open the OpenRazer device functions                               |
    \*-----------------------------------------------------------------*/
    OpenFunctions(dev_path);

    /*-----------------------------------------------------------------*\
    | Start device at -1.  This indicates the device was not detected   |
    \*-----------------------------------------------------------------*/
    device_index = -1;

    /*-----------------------------------------------------------------*\
    | Get the device name from the OpenRazer driver                     |
    \*-----------------------------------------------------------------*/
    std::getline(device_type, name);

    /*-----------------------------------------------------------------*\
    | Set the description to indicate this is an OpenRazer device       |
    \*-----------------------------------------------------------------*/
    description = "OpenRazer Device";

    /*-----------------------------------------------------------------*\
    | Set the device path as the location                               |
    \*-----------------------------------------------------------------*/
    location = dev_path;

    /*-----------------------------------------------------------------*\
    | Get the serial number from the dev path                           |
    \*-----------------------------------------------------------------*/
    std::getline(device_serial, serial);

    /*-----------------------------------------------------------------*\
    | Get the firmware version from the dev path                        |
    \*-----------------------------------------------------------------*/
    std::getline(firmware_version, version);

    /*-----------------------------------------------------------------*\
    | Vendor is always Razer                                            |
    \*-----------------------------------------------------------------*/
    vendor = "Razer";

    /*-----------------------------------------------------------------*\
    | Loop through all known devices to look for a name match           |
    \*-----------------------------------------------------------------*/
    for (std::size_t i = 0; i < RAZER_NUM_DEVICES; i++)
    {
        if (device_list[i]->name == name)
        {
            /*---------------------------------------------------------*\
            | Set device ID                                             |
            \*---------------------------------------------------------*/
            device_index = i;

            /*---------------------------------------------------------*\
            | Set device type                                           |
            \*---------------------------------------------------------*/
            type = device_list[i]->type;

            /*---------------------------------------------------------*\
            | Device has Direct mode if matrix_custom_frame exists      |
            \*---------------------------------------------------------*/
            if(matrix_effect_custom)
            {
                mode Direct;
                Direct.name       = "Direct";
                Direct.value      = RAZER_MODE_CUSTOM;
                Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
                Direct.color_mode = MODE_COLORS_PER_LED;
                modes.push_back(Direct);
            }

            /*---------------------------------------------------------*\
            | Device has Off mode if any _effect_none or any _state     |
            | exists                                                    |
            \*---------------------------------------------------------*/
            if( matrix_effect_none
             || logo_matrix_effect_none
             || scroll_matrix_effect_none
             || left_matrix_effect_none
             || right_matrix_effect_none
             || backlight_led_state
             || logo_led_state
             || scroll_led_state)
            {
                mode Off;
                Off.name       = "Off";
                Off.value      = RAZER_MODE_OFF;
                Off.flags      = 0;
                Off.color_mode = MODE_COLORS_NONE;
                modes.push_back(Off);
            }

            /*---------------------------------------------------------*\
            | Device has Static mode if any _effect_static or any       |
            | _effect exists                                            |
            \*---------------------------------------------------------*/
            if( matrix_effect_static
             || logo_matrix_effect_static
             || scroll_matrix_effect_static
             || left_matrix_effect_static
             || right_matrix_effect_static
             || backlight_led_effect
             || logo_led_effect
             || scroll_led_effect)
            {
                mode Static;
                Static.name       = "Static";
                Static.value      = RAZER_MODE_STATIC;
                Static.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
                Static.colors_min = 1;
                Static.colors_max = 1;
                Static.color_mode = MODE_COLORS_MODE_SPECIFIC;
                Static.colors.resize(1);
                modes.push_back(Static);
            }

            /*---------------------------------------------------------*\
            | Device has Breathing mode if any _effect_breath exists    |
            \*---------------------------------------------------------*/
            if( matrix_effect_breath
             || logo_matrix_effect_breath
             || scroll_matrix_effect_breath
             || left_matrix_effect_breath
             || right_matrix_effect_breath)
            {
                mode Breathing;
                Breathing.name       = "Breathing";
                Breathing.value      = RAZER_MODE_BREATHING;
                Breathing.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
                Breathing.colors_min = 1;
                Breathing.colors_max = 2;
                Breathing.color_mode = MODE_COLORS_MODE_SPECIFIC;
                Breathing.colors.resize(1);
                modes.push_back(Breathing);
            }

            /*---------------------------------------------------------*\
            | Device has limited Breathing mode if any _effect exists   |
            \*---------------------------------------------------------*/
            if( backlight_led_effect
             || logo_led_effect
             || scroll_led_effect)
            {
                mode Breathing;
                Breathing.name       = "Breathing";
                Breathing.value      = RAZER_MODE_BREATHING;
                Breathing.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
                Breathing.colors_min = 1;
                Breathing.colors_max = 1;
                Breathing.color_mode = MODE_COLORS_MODE_SPECIFIC;
                Breathing.colors.resize(1);
                modes.push_back(Breathing);
            }

            /*---------------------------------------------------------*\
            | Device has Flashing mode if any _effect exists            |
            \*---------------------------------------------------------*/
            if( backlight_led_effect
             || logo_led_effect
             || scroll_led_effect)
            {
                mode Flashing;
                Flashing.name       = "Flashing";
                Flashing.value      = RAZER_MODE_FLASHING;
                Flashing.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
                Flashing.colors_min = 1;
                Flashing.colors_max = 1;
                Flashing.color_mode = MODE_COLORS_MODE_SPECIFIC;
                Flashing.colors.resize(1);
                modes.push_back(Flashing);
            }

            /*---------------------------------------------------------*\
            | Device has Spectrum Cycle mode if any _effect_spectrum or |
            | _effect exists                                            |
            \*---------------------------------------------------------*/
            if( matrix_effect_spectrum
             || logo_matrix_effect_spectrum
             || scroll_matrix_effect_spectrum
             || left_matrix_effect_spectrum
             || right_matrix_effect_spectrum
             || backlight_led_effect
             || logo_led_effect
             || scroll_led_effect)
            {
                mode SpectrumCycle;
                SpectrumCycle.name       = "Spectrum Cycle";
                SpectrumCycle.value      = RAZER_MODE_SPECTRUM_CYCLE;
                SpectrumCycle.flags      = 0;
                SpectrumCycle.color_mode = MODE_COLORS_NONE;
                modes.push_back(SpectrumCycle);
            }

            /*---------------------------------------------------------*\
            | Device has Wave mode if any _effect_wave exists           |
            \*---------------------------------------------------------*/
            if( matrix_effect_wave
             || left_matrix_effect_wave
             || right_matrix_effect_wave)
            {
                mode Wave;
                Wave.name       = "Wave";
                Wave.value      = RAZER_MODE_WAVE;
                Wave.flags      = MODE_FLAG_HAS_DIRECTION_LR;
                Wave.direction  = MODE_DIRECTION_RIGHT;
                Wave.color_mode = MODE_COLORS_NONE;
                modes.push_back(Wave);
            }

            /*---------------------------------------------------------*\
            | Device has Reactive mode if any _effect_reactive exists   |
            \*---------------------------------------------------------*/
            if( matrix_effect_reactive
             || logo_matrix_effect_reactive
             || scroll_matrix_effect_reactive
             || left_matrix_effect_reactive
             || right_matrix_effect_reactive)
            {
                mode Reactive;
                Reactive.name       = "Reactive";
                Reactive.value      = RAZER_MODE_REACTIVE;
                Reactive.flags      = 0;
                Reactive.color_mode = MODE_COLORS_NONE;
                modes.push_back(Reactive);
            }

            /*---------------------------------------------------------*\
            | Initialize file descriptors                               |
            \*---------------------------------------------------------*/
            if(device_list[i]->matrix_type == true)
            {
                SetupMatrixDevice(device_list[i]->rows, device_list[i]->cols);
            }
            else
            {
                SetupNonMatrixDevice();
            }

            SetupZones();
        }
    }
}

RGBController_OpenRazer::~RGBController_OpenRazer()
{
    /*---------------------------------------------------------*\
    | Delete the matrix map                                     |
    \*---------------------------------------------------------*/
    for(unsigned int zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        if(zones[zone_index].matrix_map != NULL)
        {
            if(zones[zone_index].matrix_map->map != NULL)
            {
                delete zones[zone_index].matrix_map->map;
            }

            delete zones[zone_index].matrix_map;
        }
    }
}

void RGBController_OpenRazer::SetupZones()
{
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
                        new_map->map[(y * new_map->width) + x] = (y * new_map->width) + x;
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
                    for(unsigned int i = 0; i < device_list[device_index]->keymap_size; i++)
                    {
                        if(zone_id == device_list[device_index]->keymap[i].zone &&
                           row_id  == device_list[device_index]->keymap[i].row  &&
                           col_id  == device_list[device_index]->keymap[i].col)
                        {
                            new_led->name = device_list[device_index]->keymap[i].name;
                        }
                    }
                }

                leds.push_back(*new_led);
            }
        }
    }

    SetupColors();
}

void RGBController_OpenRazer::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_OpenRazer::SetCustomMode()
{
    /*---------------------------------------------------------*\
    | If device supports custom mode, it will be mode index 0   |
    \*---------------------------------------------------------*/
    if(modes[0].value == RAZER_MODE_CUSTOM)
    {
        active_mode = 0;
    }
    /*---------------------------------------------------------*\
    | If not, use static mode.                                  |
    \*---------------------------------------------------------*/
    else
    {
        for(unsigned int i = 0; i < modes.size(); i++)
        {
            if(modes[i].value == RAZER_MODE_STATIC)
            {
                active_mode = i;
                break;
            }
        }
    }
}

void RGBController_OpenRazer::DeviceUpdateMode()
{
    char update_value[6];
    char effect_value[1];

    update_value[0] = 1;

    switch(modes[active_mode].value)
    {
        case RAZER_MODE_CUSTOM:
            if(matrix_effect_custom)
            {
                matrix_effect_custom.write(update_value, 1);
                matrix_effect_custom.flush();
            }
            break;

        case RAZER_MODE_OFF:
            if(matrix_effect_none)
            {
                matrix_effect_none.write(update_value, 1);
                matrix_effect_none.flush();
            }

            if(logo_matrix_effect_none)
            {
                logo_matrix_effect_none.write(update_value, 1);
                logo_matrix_effect_none.flush();
            }

            if(scroll_matrix_effect_none)
            {
                scroll_matrix_effect_none.write(update_value, 1);
                scroll_matrix_effect_none.flush();
            }

            if(left_matrix_effect_none)
            {
                left_matrix_effect_none.write(update_value, 1);
                left_matrix_effect_none.flush();
            }

            if(right_matrix_effect_none)
            {
                right_matrix_effect_none.write(update_value, 1);
                right_matrix_effect_none.flush();
            }

            if(backlight_led_state)
            {
                update_value[0] = '0';
                backlight_led_state.write(update_value, 1);
                backlight_led_state.flush();
            }

            if(logo_led_state)
            {
                update_value[0] = '0';
                logo_led_state.write(update_value, 1);
                logo_led_state.flush();
            }

            if(scroll_led_state)
            {
                update_value[0] = '0';
                scroll_led_state.write(update_value, 1);
                scroll_led_state.flush();
            }
            break;

        case RAZER_MODE_STATIC:
            effect_value[0] = '0';

            if(backlight_led_state)
            {
                update_value[0] = '1';
                backlight_led_state.write(update_value, 1);
                backlight_led_state.flush();
            }

            if(logo_led_state)
            {
                update_value[0] = '1';
                logo_led_state.write(update_value, 1);
                logo_led_state.flush();
            }

            if(scroll_led_state)
            {
                update_value[0] = '1';
                scroll_led_state.write(update_value, 1);
                scroll_led_state.flush();
            }

            update_value[0] = RGBGetRValue(modes[active_mode].colors[0]);
            update_value[1] = RGBGetGValue(modes[active_mode].colors[0]);
            update_value[2] = RGBGetBValue(modes[active_mode].colors[0]);

            if(matrix_effect_static)
            {
                matrix_effect_static.write(update_value, 3);
                matrix_effect_static.flush();
            }

            if(logo_matrix_effect_static)
            {
                logo_matrix_effect_static.write(update_value, 3);
                logo_matrix_effect_static.flush();
            }

            if(scroll_matrix_effect_static)
            {
                scroll_matrix_effect_static.write(update_value, 3);
                scroll_matrix_effect_static.flush();
            }

            if(left_matrix_effect_static)
            {
                left_matrix_effect_static.write(update_value, 3);
                left_matrix_effect_static.flush();
            }

            if(right_matrix_effect_static)
            {
                right_matrix_effect_static.write(update_value, 3);
                right_matrix_effect_static.flush();
            }

            if(backlight_led_effect && backlight_led_rgb)
            {
                backlight_led_rgb.write(update_value, 3);
                backlight_led_rgb.flush();
                backlight_led_effect.write(effect_value, 1);
                backlight_led_effect.flush();
            }

            if(logo_led_effect && logo_led_rgb)
            {
                logo_led_rgb.write(update_value, 3);
                logo_led_rgb.flush();
                logo_led_effect.write(effect_value, 1);
                logo_led_effect.flush();
            }

            if(scroll_led_effect && scroll_led_rgb)
            {
                scroll_led_rgb.write(update_value, 3);
                scroll_led_rgb.flush();
                scroll_led_effect.write(effect_value, 1);
                scroll_led_effect.flush();
            }
            break;

        case RAZER_MODE_FLASHING:
            effect_value[0] = '1';

            if(backlight_led_state)
            {
                update_value[0] = '1';
                backlight_led_state.write(update_value, 1);
                backlight_led_state.flush();
            }

            if(logo_led_state)
            {
                update_value[0] = '1';
                logo_led_state.write(update_value, 1);
                logo_led_state.flush();
            }

            if(scroll_led_state)
            {
                update_value[0] = '1';
                scroll_led_state.write(update_value, 1);
                scroll_led_state.flush();
            }

            update_value[0] = RGBGetRValue(modes[active_mode].colors[0]);
            update_value[1] = RGBGetGValue(modes[active_mode].colors[0]);
            update_value[2] = RGBGetBValue(modes[active_mode].colors[0]);

            if(backlight_led_effect && backlight_led_rgb)
            {
                backlight_led_rgb.write(update_value, 3);
                backlight_led_rgb.flush();
                backlight_led_effect.write(effect_value, 1);
                backlight_led_effect.flush();
            }

            if(logo_led_effect && logo_led_rgb)
            {
                logo_led_rgb.write(update_value, 3);
                logo_led_rgb.flush();
                logo_led_effect.write(effect_value, 1);
                logo_led_effect.flush();
            }

            if(scroll_led_effect && scroll_led_rgb)
            {
                scroll_led_rgb.write(update_value, 3);
                scroll_led_rgb.flush();
                scroll_led_effect.write(effect_value, 1);
                scroll_led_effect.flush();
            }
            break;

        case RAZER_MODE_BREATHING:
            effect_value[0] = '2';

            switch(modes[active_mode].color_mode)
            {
                case MODE_COLORS_MODE_SPECIFIC:
                    if(backlight_led_state)
                    {
                        update_value[0] = '1';
                        backlight_led_state.write(update_value, 1);
                        backlight_led_state.flush();
                    }

                    if(logo_led_state)
                    {
                        update_value[0] = '1';
                        logo_led_state.write(update_value, 1);
                        logo_led_state.flush();
                    }

                    if(scroll_led_state)
                    {
                        update_value[0] = '1';
                        scroll_led_state.write(update_value, 1);
                        scroll_led_state.flush();
                    }

                    update_value[0] = RGBGetRValue(modes[active_mode].colors[0]);
                    update_value[1] = RGBGetGValue(modes[active_mode].colors[0]);
                    update_value[2] = RGBGetBValue(modes[active_mode].colors[0]);

                    if(modes[active_mode].colors.size() == 2)
                    {
                        update_value[3] = RGBGetRValue(modes[active_mode].colors[1]);
                        update_value[4] = RGBGetGValue(modes[active_mode].colors[1]);
                        update_value[5] = RGBGetBValue(modes[active_mode].colors[1]);

                        if(matrix_effect_breath)
                        {
                            matrix_effect_breath.write(update_value, 6);
                            matrix_effect_breath.flush();
                        }

                        if(logo_matrix_effect_breath)
                        {
                            logo_matrix_effect_breath.write(update_value, 6);
                            logo_matrix_effect_breath.flush();
                        }

                        if(scroll_matrix_effect_breath)
                        {
                            scroll_matrix_effect_breath.write(update_value, 6);
                            scroll_matrix_effect_breath.flush();
                        }

                        if(left_matrix_effect_breath)
                        {
                            left_matrix_effect_breath.write(update_value, 6);
                            left_matrix_effect_breath.flush();
                        }

                        if(right_matrix_effect_breath)
                        {
                            right_matrix_effect_breath.write(update_value, 6);
                            right_matrix_effect_breath.flush();
                        }
                    }
                    else
                    {
                        if(matrix_effect_breath)
                        {
                            matrix_effect_breath.write(update_value, 3);
                            matrix_effect_breath.flush();
                        }

                        if(logo_matrix_effect_breath)
                        {
                            logo_matrix_effect_breath.write(update_value, 3);
                            logo_matrix_effect_breath.flush();
                        }

                        if(scroll_matrix_effect_breath)
                        {
                            scroll_matrix_effect_breath.write(update_value, 3);
                            scroll_matrix_effect_breath.flush();
                        }

                        if(left_matrix_effect_breath)
                        {
                            left_matrix_effect_breath.write(update_value, 3);
                            left_matrix_effect_breath.flush();
                        }

                        if(right_matrix_effect_breath)
                        {
                            right_matrix_effect_breath.write(update_value, 3);
                            right_matrix_effect_breath.flush();
                        }

                        if(backlight_led_effect && backlight_led_rgb)
                        {
                            backlight_led_rgb.write(update_value, 3);
                            backlight_led_rgb.flush();
                            backlight_led_effect.write(effect_value, 1);
                            backlight_led_effect.flush();
                        }

                        if(logo_led_effect && logo_led_rgb)
                        {
                            logo_led_rgb.write(update_value, 3);
                            logo_led_rgb.flush();
                            logo_led_effect.write(effect_value, 1);
                            logo_led_effect.flush();
                        }

                        if(scroll_led_effect && scroll_led_rgb)
                        {
                            scroll_led_rgb.write(update_value, 3);
                            scroll_led_rgb.flush();
                            scroll_led_effect.write(effect_value, 1);
                            scroll_led_effect.flush();
                        }
                    }
                    break;

                case MODE_COLORS_RANDOM:
                    if(matrix_effect_breath)
                    {
                        matrix_effect_breath.write(update_value, 1);
                        matrix_effect_breath.flush();
                    }

                    if(logo_matrix_effect_breath)
                    {
                        logo_matrix_effect_breath.write(update_value, 1);
                        logo_matrix_effect_breath.flush();
                    }

                    if(scroll_matrix_effect_breath)
                    {
                        scroll_matrix_effect_breath.write(update_value, 1);
                        scroll_matrix_effect_breath.flush();
                    }

                    if(left_matrix_effect_breath)
                    {
                        left_matrix_effect_breath.write(update_value, 1);
                        left_matrix_effect_breath.flush();
                    }

                    if(right_matrix_effect_breath)
                    {
                        right_matrix_effect_breath.write(update_value, 1);
                        right_matrix_effect_breath.flush();
                    }

                    break;
            }
            break;

        case RAZER_MODE_SPECTRUM_CYCLE:
            effect_value[0] = '4';

            if(backlight_led_state)
            {
                update_value[0] = '1';
                backlight_led_state.write(update_value, 1);
                backlight_led_state.flush();
            }

            if(logo_led_state)
            {
                update_value[0] = '1';
                logo_led_state.write(update_value, 1);
                logo_led_state.flush();
            }

            if(scroll_led_state)
            {
                update_value[0] = '1';
                scroll_led_state.write(update_value, 1);
                scroll_led_state.flush();
            }

            if(matrix_effect_spectrum)
            {
                matrix_effect_spectrum.write(update_value, 1);
                matrix_effect_spectrum.flush();
            }

            if(logo_matrix_effect_spectrum)
            {
                logo_matrix_effect_spectrum.write(update_value, 1);
                logo_matrix_effect_spectrum.flush();
            }

            if(scroll_matrix_effect_spectrum)
            {
                scroll_matrix_effect_spectrum.write(update_value, 1);
                scroll_matrix_effect_spectrum.flush();
            }

            if(left_matrix_effect_spectrum)
            {
                left_matrix_effect_spectrum.write(update_value, 1);
                left_matrix_effect_spectrum.flush();
            }

            if(right_matrix_effect_spectrum)
            {
                right_matrix_effect_spectrum.write(update_value, 1);
                right_matrix_effect_spectrum.flush();
            }

            if(backlight_led_effect)
            {
                backlight_led_effect.write(effect_value, 1);
                backlight_led_effect.flush();
            }

            if(logo_led_effect)
            {
                logo_led_effect.write(effect_value, 1);
                logo_led_effect.flush();
            }

            if(scroll_led_effect)
            {
                scroll_led_effect.write(effect_value, 1);
                scroll_led_effect.flush();
            }
            break;

        case RAZER_MODE_WAVE:
            switch(modes[active_mode].direction)
            {
                case MODE_DIRECTION_LEFT:
                    update_value[0] = '2';
                    break;

                default:
                    update_value[0] = '1';
                    break;
            }

            if(matrix_effect_wave)
            {
                matrix_effect_wave.write(update_value, 1);
                matrix_effect_wave.flush();
            }

            if(left_matrix_effect_wave)
            {
                left_matrix_effect_wave.write(update_value, 1);
                left_matrix_effect_wave.flush();
            }

            if(right_matrix_effect_wave)
            {
                right_matrix_effect_wave.write(update_value, 1);
                right_matrix_effect_wave.flush();
            }
            break;

        case RAZER_MODE_REACTIVE:
            if(matrix_effect_reactive)
            {
                matrix_effect_reactive.write(update_value, 1);
                matrix_effect_reactive.flush();
            }

            if(logo_matrix_effect_reactive)
            {
                logo_matrix_effect_reactive.write(update_value, 1);
                logo_matrix_effect_reactive.flush();
            }

            if(scroll_matrix_effect_reactive)
            {
                scroll_matrix_effect_reactive.write(update_value, 1);
                scroll_matrix_effect_reactive.flush();
            }

            if(left_matrix_effect_reactive)
            {
                left_matrix_effect_reactive.write(update_value, 1);
                left_matrix_effect_reactive.flush();
            }

            if(right_matrix_effect_reactive)
            {
                right_matrix_effect_reactive.write(update_value, 1);
                right_matrix_effect_reactive.flush();
            }
            break;
    }

    std::this_thread::sleep_for(20ms);
}
