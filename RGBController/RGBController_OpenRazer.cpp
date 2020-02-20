/*-----------------------------------------*\
|  RGBController_OpenRazer.cpp              |
|                                           |
|  Generic RGB Interface for OpenRazer      |
|  kernel drivers for Chroma peripherals    |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/15/2019   |
\*-----------------------------------------*/

#include "RGBController_OpenRazer.h"

#include <fstream>
#include <unistd.h>

void RGBController_OpenRazer::UpdateLEDs()
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
                    
                    char output_array[output_array_size];

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
                }
                
                if(matrix_type == RAZER_TYPE_MATRIX_FRAME)
                {
                    matrix_effect_custom.write(&update_value, 1);
                    matrix_effect_custom.flush();
                }
            }
            break;
#if 0
        case RAZER_TYPE_NOMATRIX:
            {
                unsigned int output_array_size = 3;
                char output_array[output_array_size];
                char update_value = 0;

                output_array[0] = (char)RGBGetRValue(colors[0]);
                output_array[1] = (char)RGBGetGValue(colors[0]);
                output_array[2] = (char)RGBGetBValue(colors[0]);
                logo_led_rgb.write(output_array, output_array_size);

                output_array[0] = (char)RGBGetRValue(colors[1]);
                output_array[1] = (char)RGBGetGValue(colors[1]);
                output_array[2] = (char)RGBGetBValue(colors[1]);
                scroll_led_rgb.write(output_array, output_array_size);

                logo_led_rgb.flush();
                scroll_led_rgb.flush();

                logo_led_effect.write(&update_value, 1);
                scroll_led_effect.write(&update_value, 1);
                logo_led_effect.flush();
                scroll_led_effect.flush();
            }
            break;
#endif
    }
}

void RGBController_OpenRazer::UpdateZoneLEDs(int zone)
{
    UpdateLEDs();
}

void RGBController_OpenRazer::UpdateSingleLED(int led)
{
    UpdateLEDs();
}

void RGBController_OpenRazer::SetupMatrixDevice(std::string dev_path, unsigned int rows, unsigned int cols)
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

void RGBController_OpenRazer::SetupNonMatrixDevice(std::string dev_path)
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
    logo_matrix_effect_spectrum.open(  dev_path + "/logo_matrix_effect_spectrum");
    logo_matrix_effect_reactive.open(  dev_path + "/logo_matrix_effect_reactive");

    scroll_led_brightness.open(        dev_path + "/scroll_led_brightness");
    scroll_matrix_effect_none.open(    dev_path + "/scroll_matrix_effect_none");
    scroll_matrix_effect_static.open(  dev_path + "/scroll_matrix_effect_static");
    scroll_matrix_effect_spectrum.open(dev_path + "/scroll_matrix_effect_spectrum");
    scroll_matrix_effect_reactive.open(dev_path + "/scroll_matrix_effect_reactive");

    scroll_led_effect.open(            dev_path + "/scroll_led_effect");
    scroll_led_rgb.open(               dev_path + "/scroll_led_rgb");
}

RGBController_OpenRazer::RGBController_OpenRazer(std::string dev_path)
{
    unsigned int led_count = 0;

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
            | Initialize modes                                          |
            \*---------------------------------------------------------*/
            if(matrix_effect_custom)
            {
                mode Custom;
                Custom.name       = "Custom";
                Custom.value      = RAZER_MODE_CUSTOM;
                Custom.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
                Custom.color_mode = MODE_COLORS_PER_LED;
                modes.push_back(Custom);
            }

            if(matrix_effect_none)
            {
                mode Off;
                Off.name       = "Off";
                Off.value      = RAZER_MODE_OFF;
                Off.flags      = 0;
                Off.color_mode = MODE_COLORS_NONE;
                modes.push_back(Off);
            }

            if(matrix_effect_static)
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

            if(matrix_effect_breath)
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

            if(matrix_effect_spectrum)
            {
                mode SpectrumCycle;
                SpectrumCycle.name       = "Spectrum Cycle";
                SpectrumCycle.value      = RAZER_MODE_SPECTRUM_CYCLE;
                SpectrumCycle.flags      = 0;
                SpectrumCycle.color_mode = MODE_COLORS_NONE;
                modes.push_back(SpectrumCycle);
            }

            if(matrix_effect_wave)
            {
                mode Wave;
                Wave.name       = "Wave";
                Wave.value      = RAZER_MODE_WAVE;
                Wave.flags      = MODE_FLAG_HAS_DIRECTION_LR;
                Wave.direction  = MODE_DIRECTION_RIGHT;
                Wave.color_mode = MODE_COLORS_NONE;
                modes.push_back(Wave);
            }

            if(matrix_effect_reactive)
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
                SetupMatrixDevice(dev_path, device_list[i]->rows, device_list[i]->cols);    
            }
            else
            {
                SetupNonMatrixDevice(dev_path);
            }
            
            /*---------------------------------------------------------*\
            | Fill in zone information based on device table            |
            \*---------------------------------------------------------*/
            for (int zone_id = 0; zone_id < RAZER_MAX_ZONES; zone_id++)
            {
                if (device_list[i]->zones[zone_id] != NULL)
                {
                    zone new_zone;
                    new_zone.name = device_list[i]->zones[zone_id]->name;
                    new_zone.type = device_list[i]->zones[zone_id]->type;

                    for (unsigned int row_id = 0; row_id < device_list[i]->zones[zone_id]->rows; row_id++)
                    {
                        std::vector<int> new_zone_map;

                        for (unsigned int col_id = 0; col_id < device_list[i]->zones[zone_id]->cols; col_id++)
                        {
                            char id_buf[8];
                            snprintf(id_buf, 8, "%d", col_id + 1);

                            RGBColor new_color = 0x00000000;
                            colors.push_back(new_color);

                            led* new_led = new led();
                            new_led->name = device_list[i]->zones[zone_id]->name + " LED ";
                            new_led->name.append(id_buf);
                            leds.push_back(*new_led);

                            new_zone_map.push_back(led_count);

                            led_count++;
                        }

                        new_zone.map.push_back(new_zone_map);
                    }
                    zones.push_back(new_zone);
                }
            }
        }
    }
}

void RGBController_OpenRazer::SetCustomMode()
{
    active_mode = RAZER_MODE_CUSTOM;
}

void RGBController_OpenRazer::UpdateMode()
{
    char update_value[6];

    update_value[0] = 1;

    switch(matrix_type)
    {
        case RAZER_TYPE_MATRIX_FRAME:
        case RAZER_TYPE_MATRIX_NOFRAME:
            {
                switch(modes[active_mode].value)
                {
                    case RAZER_MODE_CUSTOM:
                        matrix_effect_custom.write(update_value, 1);
                        matrix_effect_custom.flush();
                        break;

                    case RAZER_MODE_OFF:
                        matrix_effect_none.write(update_value, 1);
                        matrix_effect_none.flush();
                        break;

                    case RAZER_MODE_STATIC:
                        update_value[0] = RGBGetRValue(modes[RAZER_MODE_STATIC].colors[0]);
                        update_value[1] = RGBGetGValue(modes[RAZER_MODE_STATIC].colors[0]);
                        update_value[2] = RGBGetBValue(modes[RAZER_MODE_STATIC].colors[0]);
                        matrix_effect_static.write(update_value, 3);
                        matrix_effect_static.flush();
                        break;

                    case RAZER_MODE_BREATHING:
                        switch(modes[RAZER_MODE_BREATHING].color_mode)
                        {
                            case MODE_COLORS_MODE_SPECIFIC:
                                update_value[0] = RGBGetRValue(modes[RAZER_MODE_BREATHING].colors[0]);
                                update_value[1] = RGBGetGValue(modes[RAZER_MODE_BREATHING].colors[0]);
                                update_value[2] = RGBGetBValue(modes[RAZER_MODE_BREATHING].colors[0]);

                                if(modes[RAZER_MODE_BREATHING].colors.size() == 2)
                                {
                                    update_value[3] = RGBGetRValue(modes[RAZER_MODE_BREATHING].colors[1]);
                                    update_value[4] = RGBGetGValue(modes[RAZER_MODE_BREATHING].colors[1]);
                                    update_value[5] = RGBGetBValue(modes[RAZER_MODE_BREATHING].colors[1]);

                                    matrix_effect_breath.write(update_value, 6);
                                    matrix_effect_breath.flush();
                                }
                                else
                                {
                                    matrix_effect_breath.write(update_value, 3);
                                    matrix_effect_breath.flush();
                                }
                                break;

                            case MODE_COLORS_RANDOM:
                                matrix_effect_breath.write(update_value, 1);
                                matrix_effect_breath.flush();
                                break;
                        }
                        break;

                    case RAZER_MODE_SPECTRUM_CYCLE:
                        matrix_effect_spectrum.write(update_value, 1);
                        matrix_effect_spectrum.flush();
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
                        matrix_effect_wave.write(update_value, 1);
                        matrix_effect_wave.flush();
                        break;

                    case RAZER_MODE_REACTIVE:
                        matrix_effect_reactive.write(update_value, 1);
                        matrix_effect_reactive.flush();
                        break;
                }
            }
            break;
#if 0
        case RAZER_TYPE_NOMATRIX:
            {
                switch(active_mode)
                {
                    case RAZER_MODE_CUSTOM:
                        update_value = 0;
                        logo_led_effect.write(&update_value, 1);
                        scroll_led_effect.write(&update_value, 1);
                        logo_led_effect.flush();
                        scroll_led_effect.flush();
                        break;

                    case RAZER_MODE_SPECTRUM_CYCLE:
                        update_value = '4';
                        logo_led_effect.write(&update_value, 1);
                        scroll_led_effect.write(&update_value, 1);
                        logo_led_effect.flush();
                        scroll_led_effect.flush();
                        break;
                }
            }
#endif
    }
}
