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
int RGBController_OpenRazer::GetMode()
{
    return(0);
}

void RGBController_OpenRazer::SetMode(int mode)
{
    char update_value = 1;

    switch(matrix_type)
    {
        case RAZER_TYPE_MATRIX_FRAME:
        case RAZER_TYPE_MATRIX_NOFRAME:
            {
                switch(mode)
                {
                    case RAZER_MODE_CUSTOM:
                        matrix_effect_custom.write(&update_value, 1);
                        matrix_effect_custom.flush();
                        break;
                    
                    case RAZER_MODE_OFF:
                        matrix_effect_none.write(&update_value, 1);
                        matrix_effect_none.flush();
                        break;

                    case RAZER_MODE_STATIC:
                        break;

                    case RAZER_MODE_BREATHING:
                        break;

                    case RAZER_MODE_SPECTRUM_CYCLE:
                        matrix_effect_spectrum.write(&update_value, 1);
                        matrix_effect_spectrum.flush();
                        break;
                    
                    case RAZER_MODE_WAVE:
                        matrix_effect_wave.write(&update_value, 1);
                        matrix_effect_wave.flush();
                        break;

                    case RAZER_MODE_REACTIVE:
                        matrix_effect_reactive.write(&update_value, 1);
                        matrix_effect_reactive.flush();
                        break;
                }
            }
            break;

        case RAZER_TYPE_NOMATRIX:
            {
                switch(mode)
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
    }
}

void RGBController_OpenRazer::SetCustomMode()
{
    SetMode(RAZER_MODE_CUSTOM);
}

void RGBController_OpenRazer::UpdateLEDs()
{
    switch(matrix_type)
    {
        case RAZER_TYPE_MATRIX_FRAME:
        case RAZER_TYPE_MATRIX_NOFRAME:
        case RAZER_TYPE_MATRIX_STATIC:
            {
                char update_value = 1;
                
                for (int row = 0; row < matrix_rows; row++)
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

                    for(int col = 0; col < matrix_cols; col++)
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
    }
}

void RGBController_OpenRazer::SetAllLEDs(RGBColor color)
{
    for(int i = 0; i < colors.size(); i++)
    {
        colors[i] = color;
    }

    UpdateLEDs();
}

void RGBController_OpenRazer::SetAllZoneLEDs(int zone, RGBColor color)
{
    for (int x = 0; x < zones[zone].map.size(); x++)
    {
        for (int y = 0; y < zones[zone].map[x].size(); y++)
        {
            colors[zones[zone].map[x][y]] = color;
        }
    }

    UpdateLEDs();
}

void RGBController_OpenRazer::SetLED(int led, RGBColor color)
{
    colors[led] = color;

    UpdateLEDs();
}

static std::string GetDeviceTypeString(std::string dev_path)
{
    // Read device_type for device name string
    std::string dev_type_path = dev_path + "/device_type";
    std::ifstream dev_type_file;
    std::string ret_str;

    dev_type_file.open(dev_type_path);
    std::getline(dev_type_file, ret_str);
    dev_type_file.close();

    return(ret_str);
}

static std::string GetFirmwareVersionString(std::string dev_path)
{
    // Read firmware_version for firmware version string
    std::string firm_ver_path = dev_path + "/firmware_version";
    std::ifstream firm_ver_file;
    std::string ret_str;

    firm_ver_file.open(firm_ver_path);
    std::getline(firm_ver_file, ret_str);
    firm_ver_file.close();

    return(ret_str);   
}

static std::string GetSerialNumberString(std::string dev_path)
{
    // Read device_serial for serial number string
    std::string ser_num_path = dev_path + "/device_serial";
    std::ifstream ser_num_file;
    std::string ret_str;

    ser_num_file.open(ser_num_path);
    std::getline(ser_num_file, ret_str);
    ser_num_file.close();

    return(ret_str);    
}

void RGBController_OpenRazer::SetupMatrixDevice(std::string dev_path, unsigned int rows, unsigned int cols)
{
    matrix_custom_frame.open(dev_path + "/matrix_custom_frame");
    matrix_effect_custom.open(dev_path + "/matrix_effect_custom");
    matrix_effect_breath.open(dev_path + "/matrix_effect_breath");
    matrix_effect_none.open(dev_path + "/matrix_effect_none");
    matrix_effect_reactive.open(dev_path + "/matrix_effect_reactive");
    matrix_effect_spectrum.open(dev_path + "/matrix_effect_spectrum");
    matrix_effect_static.open(dev_path + "/matrix_effect_static");
    matrix_effect_wave.open(dev_path + "/matrix_effect_wave");    

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
    logo_led_effect.open(dev_path + "/logo_led_effect");
    logo_led_rgb.open(dev_path + "/logo_led_rgb");
    scroll_led_effect.open(dev_path + "/scroll_led_effect");
    scroll_led_rgb.open(dev_path + "/scroll_led_rgb");

    matrix_type = RAZER_TYPE_NOMATRIX;
}

RGBController_OpenRazer::RGBController_OpenRazer(std::string dev_path)
{
    unsigned int led_count = 0;

    /*-----------------------------------------------------------------*\
    | Start device at -1.  This indicates the device was not detected   |
    \*-----------------------------------------------------------------*/
    device = -1;

    /*-----------------------------------------------------------------*\
    | Get the device name from the OpenRazer driver                     |
    \*-----------------------------------------------------------------*/
    name = GetDeviceTypeString(dev_path);

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
    serial = GetSerialNumberString(dev_path);

    /*-----------------------------------------------------------------*\
    | Get the firmware version from the dev path                        |
    \*-----------------------------------------------------------------*/
    version = GetFirmwareVersionString(dev_path);

    /*-----------------------------------------------------------------*\
    | Loop through all known devices to look for a name match           |
    \*-----------------------------------------------------------------*/
    for (int i = 0; i < RAZER_NUM_DEVICES; i++)
    {
        if (device_list[i]->name == name)
        {
            /*---------------------------------------------------------*\
            | Set device ID                                             |
            \*---------------------------------------------------------*/
            device = i;

            /*---------------------------------------------------------*\
            | Set device type                                           |
            \*---------------------------------------------------------*/
            type = device_list[i]->type;

            /*---------------------------------------------------------*\
            | Initialize modes                                          |
            \*---------------------------------------------------------*/
            mode razer_modes[RAZER_NUM_MODES];

            razer_modes[0].name = "Custom";
            razer_modes[1].name = "Off";
            razer_modes[2].name = "Static";
            razer_modes[3].name = "Breathing";
            razer_modes[4].name = "Spectrum Cycle";
            razer_modes[5].name = "Wave";
            razer_modes[6].name = "Reactive";

            for (int i = 0; i < RAZER_NUM_MODES; i++)
            {
                modes.push_back(razer_modes[i]);
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

                    for (int row_id = 0; row_id < device_list[i]->zones[zone_id]->rows; row_id++)
                    {
                        std::vector<int> new_zone_map;

                        for (int col_id = 0; col_id < device_list[i]->zones[zone_id]->cols; col_id++)
                        {
                            RGBColor new_color = 0x00000000;
                            colors.push_back(new_color);

                            led* new_led = new led();
                            new_led->name = device_list[i]->zones[zone_id]->name;
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