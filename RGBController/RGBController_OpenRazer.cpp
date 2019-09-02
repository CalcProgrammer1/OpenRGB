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

int RGBController_OpenRazer::GetMode()
{
    return(0);
}

void RGBController_OpenRazer::SetMode(int mode)
{
    char update_value = 1;

    switch(type)
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
    switch(type)
    {
        case RAZER_TYPE_MATRIX_FRAME:
        case RAZER_TYPE_MATRIX_NOFRAME:
            {
                for (int row = 0; row < matrix_rows; row++)
                {
                    unsigned int output_array_size;
                    unsigned int output_offset;
                    unsigned int row_offset = (row * matrix_cols);

                    if(type == RAZER_TYPE_MATRIX_NOFRAME)
                    {
                        output_array_size = 3;
                        output_offset = 0;
                    }
                    else
                    {
                        output_array_size = 3 + (matrix_cols* 3);
                        output_offset = 3;
                    }
                    
                    char output_array[output_array_size];
                    char update_value = 1;

                    if(type != RAZER_TYPE_MATRIX_NOFRAME)
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
                    
                    if(type == RAZER_TYPE_MATRIX_NOFRAME)
                    {
                        matrix_effect_custom.write(output_array, output_array_size);
                        matrix_effect_custom.flush();
                    }
                    else
                    {
                        matrix_custom_frame.write(output_array, output_array_size);
                        matrix_custom_frame.flush();
                        matrix_effect_custom.write(&update_value, 1);
                        matrix_effect_custom.flush();
                    }
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

unsigned int RGBController_OpenRazer::GetTypeFromDeviceName(std::string dev_name)
{
    if(dev_name == "Razer Mamba Tournament Edition")
    {
        return(RAZER_MAMBA_TOURNAMENT_EDITION_CHROMA);
    }

    else if(dev_name == "Razer DeathAdder Chroma")
    {
        return(RAZER_DEATHADDER_CHROMA);
    }

    else if(dev_name == "Razer Diamondback Chroma")
    {
        return(RAZER_DIAMONDBACK_CHROMA);
    }

    else if(dev_name == "Razer DeathStalker Chroma")
    {
        return(RAZER_DEATHSTALKER_CHROMA);
    }

    else if(dev_name == "Razer Tartarus Chroma")
    {
        return(RAZER_TARTARUS_CHROMA);
    }

    else if(dev_name == "Razer Core")
    {
        return(RAZER_CORE);
    }

    else if(dev_name == "Razer Firefly")
    {
        return(RAZER_FIREFLY_CHROMA);
    }

    else if(dev_name == "Razer Goliathus Extended")
    {
        return(RAZER_GOLIATHUS_EXTENDED_CHROMA);
    }

    else if(dev_name == "Razer Chroma Mug Holder")
    {
        return(RAZER_MUG_HOLDER);
    }

    else if(dev_name == "Razer Kraken 7.1 Chroma")
    {
        return(RAZER_KRAKEN_V1);
    }

    else if(dev_name == "Razer Kraken 7.1 V2")
    {
        return(RAZER_KRAKEN_V2);
    }

    else if(dev_name == "Razer Chroma HDK")
    {
        return(RAZER_CHROMA_HDK);
    }

    else
    {
        return(RAZER_NO_DEVICE);
    }
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
        type = RAZER_TYPE_MATRIX_NOFRAME;
    }
    else
    {
        type = RAZER_TYPE_MATRIX_FRAME;

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

    type = RAZER_TYPE_NOMATRIX;
}

RGBController_OpenRazer::RGBController_OpenRazer(std::string dev_path)
{
    name = GetDeviceTypeString(dev_path);

    device = GetTypeFromDeviceName(name);

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

    switch(device)
    {
        case RAZER_MAMBA_TOURNAMENT_EDITION_CHROMA:
            {
                SetupMatrixDevice(dev_path, 1, 16);
                
                for (int i = 0; i < 16; i++)
                {
                    RGBColor new_color = 0x00000000;
                    colors.push_back(new_color);
                }

                for (int i = 0; i < 7; i++)
                {
                    led* new_led = new led();
                    new_led->name = "Left Strip";
                    leds.push_back(*new_led);
                }
                for (int i = 7; i < 14; i++)
                {
                    led* new_led = new led();
                    new_led->name = "Right Strip";
                    leds.push_back(*new_led);
                }

                {
                    led* new_led = new led();
                    new_led->name = "Logo";
                    leds.push_back(*new_led);
                }

                {
                    led* new_led = new led();
                    new_led->name = "Mouse Wheel";
                    leds.push_back(*new_led);
                }

                zone left_zone;
                left_zone.name = "Left Strip";
                left_zone.type = ZONE_TYPE_LINEAR;
                std::vector<int> left_zone_map;
                for(int i = 0; i < 7; i++)
                {
                    left_zone_map.push_back(i);
                }
                left_zone.map.push_back(left_zone_map);
                zones.push_back(left_zone);

                zone right_zone;
                right_zone.name = "Right Strip";
                right_zone.type = ZONE_TYPE_LINEAR;
                std::vector<int> right_zone_map;
                for(int i = 7; i < 14; i++)
                {
                    right_zone_map.push_back(i);
                }
                right_zone.map.push_back(right_zone_map);
                zones.push_back(right_zone);

                zone logo_zone;
                logo_zone.name = "Logo";
                logo_zone.type = ZONE_TYPE_SINGLE;
                std::vector<int> logo_zone_map;
                logo_zone_map.push_back(14);
                logo_zone.map.push_back(logo_zone_map);
                zones.push_back(logo_zone);

                zone wheel_zone;
                wheel_zone.name = "Mouse Wheel";
                wheel_zone.type = ZONE_TYPE_SINGLE;
                std::vector<int> wheel_zone_map;
                wheel_zone_map.push_back(15);
                wheel_zone.map.push_back(wheel_zone_map);
                zones.push_back(wheel_zone);
            }
            break;
        
        case RAZER_DIAMONDBACK_CHROMA:
            {
                SetupMatrixDevice(dev_path, 1, 21);
                
                for (int i = 0; i < 21; i++)
                {
                    RGBColor new_color = 0x00000000;
                    colors.push_back(new_color);
                }

                for (int i = 0; i < 9; i++)
                {
                    led* new_led = new led();
                    new_led->name = "Left Strip";
                    leds.push_back(*new_led);
                }
                for (int i = 7; i < 19; i++)
                {
                    led* new_led = new led();
                    new_led->name = "Right Strip";
                    leds.push_back(*new_led);
                }

                {
                    led* new_led = new led();
                    new_led->name = "Logo";
                    leds.push_back(*new_led);
                }

                {
                    led* new_led = new led();
                    new_led->name = "Mouse Wheel";
                    leds.push_back(*new_led);
                }

                zone left_zone;
                left_zone.name = "Left Strip";
                left_zone.type = ZONE_TYPE_LINEAR;
                std::vector<int> left_zone_map;
                for(int i = 0; i < 9; i++)
                {
                    left_zone_map.push_back(i);
                }
                left_zone.map.push_back(left_zone_map);
                zones.push_back(left_zone);

                zone right_zone;
                right_zone.name = "Right Strip";
                right_zone.type = ZONE_TYPE_LINEAR;
                std::vector<int> right_zone_map;
                for(int i = 7; i < 19; i++)
                {
                    right_zone_map.push_back(i);
                }
                right_zone.map.push_back(right_zone_map);
                zones.push_back(right_zone);

                zone logo_zone;
                logo_zone.name = "Logo";
                logo_zone.type = ZONE_TYPE_SINGLE;
                std::vector<int> logo_zone_map;
                logo_zone_map.push_back(19);
                logo_zone.map.push_back(logo_zone_map);
                zones.push_back(logo_zone);

                zone wheel_zone;
                wheel_zone.name = "Mouse Wheel";
                wheel_zone.type = ZONE_TYPE_SINGLE;
                std::vector<int> wheel_zone_map;
                wheel_zone_map.push_back(20);
                wheel_zone.map.push_back(wheel_zone_map);
                zones.push_back(wheel_zone);
            }
            break;

        case RAZER_DEATHADDER_CHROMA:
            {
                SetupNonMatrixDevice(dev_path);

                for (int i = 0; i < 2; i++)
                {
                    RGBColor new_color = 0x00000000;
                    colors.push_back(new_color);
                }

                led logo_led;
                logo_led.name = "Logo";
                leds.push_back(logo_led);

                led wheel_led;
                wheel_led.name = "Mouse Wheel";
                leds.push_back(wheel_led);

                zone logo_zone;
                logo_zone.name = "Logo";
                logo_zone.type = ZONE_TYPE_SINGLE;
                std::vector<int> logo_zone_map;
                logo_zone_map.push_back(0);
                logo_zone.map.push_back(logo_zone_map);
                zones.push_back(logo_zone);

                zone wheel_zone;
                wheel_zone.name = "Mouse Wheel";
                wheel_zone.type = ZONE_TYPE_SINGLE;
                std::vector<int> wheel_zone_map;
                wheel_zone_map.push_back(1);
                wheel_zone.map.push_back(wheel_zone_map);
                zones.push_back(wheel_zone);
            }
            break;
        
        case RAZER_DEATHSTALKER_CHROMA:
            {
                SetupMatrixDevice(dev_path, 1, 12);

                for(int i = 0; i < 12; i++)
                {
                    RGBColor new_color = 0x00000000;
                    colors.push_back(new_color);
                }

                for (int i = 0; i < 12; i++)
                {
                    led* new_led = new led();
                    new_led->name = "Keyboard";
                    leds.push_back(*new_led);
                }

                zone keyboard_zone;
                keyboard_zone.name = "Keyboard";
                keyboard_zone.type = ZONE_TYPE_LINEAR;
                std::vector<int> keyboard_zone_map;
                for(int i = 0; i < 12; i++)
                {
                    keyboard_zone_map.push_back(i);
                }
                keyboard_zone.map.push_back(keyboard_zone_map);
                zones.push_back(keyboard_zone);
            }
            break;
        
        case RAZER_TARTARUS_CHROMA:
            {
                SetupMatrixDevice(dev_path, 1, 1);

                for (int i = 0; i < 1; i++)
                {
                    RGBColor new_color = 0x00000000;
                    colors.push_back(new_color);
                }

                led logo_led;
                logo_led.name = "Keypad";
                leds.push_back(logo_led);

                zone logo_zone;
                logo_zone.name = "Keypad";
                logo_zone.type = ZONE_TYPE_SINGLE;
                std::vector<int> logo_zone_map;
                logo_zone_map.push_back(0);
                logo_zone.map.push_back(logo_zone_map);
                zones.push_back(logo_zone);
            }
            break;

        case RAZER_CORE:
            {
                SetupMatrixDevice(dev_path, 1, 9);

                for(int i = 0; i < 9; i++)
                {
                    RGBColor new_color = 0x00000000;
                    colors.push_back(new_color);
                }

                for (int i = 0; i < 1; i++)
                {
                    led* new_led = new led();
                    new_led->name = "GPU Lighting";
                    leds.push_back(*new_led);
                }

                for (int i = 0; i < 8; i++)
                {
                    led* new_led = new led();
                    new_led->name = "LED Strip";
                    leds.push_back(*new_led);
                }

                zone zone_1;
                zone_1.name = "GPU Lighting";
                zone_1.type = ZONE_TYPE_SINGLE;
                std::vector<int> zone_1_map;
                for(int i = 0; i < 1; i++)
                {
                    zone_1_map.push_back(i);
                }
                zone_1.map.push_back(zone_1_map);
                zones.push_back(zone_1);

                zone zone_2;
                zone_2.name = "LED Strip";
                zone_2.type = ZONE_TYPE_LINEAR;
                std::vector<int> zone_2_map;
                for(int i = 0; i < 8; i++)
                {
                    zone_2_map.push_back(i);
                }
                zone_2.map.push_back(zone_2_map);
                zones.push_back(zone_2);
            }
            break;

        case RAZER_FIREFLY_CHROMA:
            {
                SetupMatrixDevice(dev_path, 1, 15);

                for(int i = 0; i < 15; i++)
                {
                    RGBColor new_color = 0x00000000;
                    colors.push_back(new_color);
                }

                for (int i = 0; i < 15; i++)
                {
                    led* new_led = new led();
                    new_led->name = "LED Strip";
                    leds.push_back(*new_led);
                }

                zone zone_1;
                zone_1.name = "LED Strip";
                zone_1.type = ZONE_TYPE_LINEAR;
                std::vector<int> zone_1_map;
                for(int i = 0; i < 15; i++)
                {
                    zone_1_map.push_back(i);
                }
                zone_1.map.push_back(zone_1_map);
                zones.push_back(zone_1);
            }
            break;

        case RAZER_GOLIATHUS_EXTENDED_CHROMA:
            {
                SetupMatrixDevice(dev_path, 1, 1);

                for (int i = 0; i < 1; i++)
                {
                    RGBColor new_color = 0x00000000;
                    colors.push_back(new_color);
                }

                led logo_led;
                logo_led.name = "Logo";
                leds.push_back(logo_led);

                zone logo_zone;
                logo_zone.name = "Logo";
                logo_zone.type = ZONE_TYPE_SINGLE;
                std::vector<int> logo_zone_map;
                logo_zone_map.push_back(0);
                logo_zone.map.push_back(logo_zone_map);
                zones.push_back(logo_zone);
            }
            break;

        case RAZER_MUG_HOLDER:
            {
                SetupMatrixDevice(dev_path, 1, 15);

                for(int i = 0; i < 15; i++)
                {
                    RGBColor new_color = 0x00000000;
                    colors.push_back(new_color);
                }

                for (int i = 0; i < 15; i++)
                {
                    led* new_led = new led();
                    new_led->name = "LED Strip";
                    leds.push_back(*new_led);
                }

                zone keyboard_zone;
                keyboard_zone.name = "LED Strip";
                keyboard_zone.type = ZONE_TYPE_LINEAR;
                std::vector<int> keyboard_zone_map;
                for(int i = 0; i < 15; i++)
                {
                    keyboard_zone_map.push_back(i);
                }
                keyboard_zone.map.push_back(keyboard_zone_map);
                zones.push_back(keyboard_zone);
            }
            break;

        case RAZER_KRAKEN_V1:
        case RAZER_KRAKEN_V2:
            {
                SetupMatrixDevice(dev_path, 1, 1);

                for (int i = 0; i < 1; i++)
                {
                    RGBColor new_color = 0x00000000;
                    colors.push_back(new_color);
                }

                led logo_led;
                logo_led.name = "Logo";
                leds.push_back(logo_led);

                zone logo_zone;
                logo_zone.name = "Logo";
                logo_zone.type = ZONE_TYPE_SINGLE;
                std::vector<int> logo_zone_map;
                logo_zone_map.push_back(0);
                logo_zone.map.push_back(logo_zone_map);
                zones.push_back(logo_zone);
            }
            break;
        
        case RAZER_CHROMA_HDK:
            {
                SetupMatrixDevice(dev_path, 4, 16);

                for (int i = 0; i < 64; i++)
                {
                    RGBColor new_color = 0x00000000;
                    colors.push_back(new_color);
                }

                for (int i = 0; i < 64; i++)
                {
                    led* new_led = new led();
                    new_led->name = "LED Strip";
                    leds.push_back(*new_led);
                }

                for (int i = 0; i < 4; i++)
                {
                    zone* new_zone = new zone();
                    new_zone->name = "LED Strip";
                    new_zone->type = ZONE_TYPE_LINEAR;

                    std::vector<int>* new_zone_map = new std::vector<int>();

                    for (int j = 0; j < 16; j++)
                    {
                        new_zone_map->push_back((i * 16) + j);
                    }

                    new_zone->map.push_back(*new_zone_map);

                    zones.push_back(*new_zone);
                }
            }
            break;
    }
}