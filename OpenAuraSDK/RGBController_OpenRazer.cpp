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

enum
{
    RAZER_NO_DEVICE,
    RAZER_BLACKWIDOW_CHROMA,
    RAZER_DEATHSTALKER_CHROMA,
    RAZER_ORNATA_CHROMA,
    RAZER_BLADE_STEALTH,
    RAZER_BLADE_PRO,
    RAZER_TARTARUS_CHROMA,
    RAZER_DEATHADDER_CHROMA,
    RAZER_DEATHADDER_ELITE,
    RAZER_NAGA_CHROMA,
    RAZER_DIAMONDBACK_CHROMA,
    RAZER_MAMBA_TOURNAMENT_EDITION_CHROMA,
    RAZER_FIREFLY_CHROMA,
    RAZER_MUG_HOLDER,
    RAZER_CORE,
    RAZER_KRAKEN_V1,
    RAZER_KRAKEN_V2,
    RAZER_NUM_DEVICES
};

enum
{
    RAZER_MODE_CUSTOM,
    RAZER_MODE_OFF,
    RAZER_MODE_STATIC,
    RAZER_MODE_BREATHING,
    RAZER_MODE_SPECTRUM_CYCLE,
    RAZER_MODE_WAVE,
    RAZER_MODE_REACTIVE,
    RAZER_NUM_MODES
};

int RGBController_OpenRazer::GetMode()
{
    return(0);
}

void RGBController_OpenRazer::SetMode(int mode)
{
    char update_value = 1;

    if(matrix_device)
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
    else
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

void RGBController_OpenRazer::SetAllLEDs(RGBColor color)
{
    for(int i = 0; i < color_buffer.size(); i++)
    {
        color_buffer[i] = color;
    }

    if(matrix_device)
    {
        unsigned int output_array_size = 3 + (color_buffer.size() * 3);
        char output_array[output_array_size];
        char update_value = 1;

        output_array[0] = 0;
        output_array[1] = 0;
        output_array[2] = color_buffer.size() - 1;

        for(int i = 0; i < color_buffer.size(); i++)
        {
            output_array[(i * 3) + 3] = (char)RGBGetRValue(color_buffer[i]);
            output_array[(i * 3) + 4] = (char)RGBGetGValue(color_buffer[i]);
            output_array[(i * 3) + 5] = (char)RGBGetBValue(color_buffer[i]);
        }
        
        matrix_custom_frame.write(output_array, output_array_size);
        matrix_custom_frame.flush();
        matrix_effect_custom.write(&update_value, 1);
        matrix_effect_custom.flush();
    }
    else
    {
        unsigned int output_array_size = 3;
        char output_array[output_array_size];
        char update_value = 0;

        output_array[0] = (char)RGBGetRValue(color_buffer[0]);
        output_array[1] = (char)RGBGetGValue(color_buffer[0]);
        output_array[2] = (char)RGBGetBValue(color_buffer[0]);
        logo_led_rgb.write(output_array, output_array_size);

        output_array[0] = (char)RGBGetRValue(color_buffer[1]);
        output_array[1] = (char)RGBGetGValue(color_buffer[1]);
        output_array[2] = (char)RGBGetBValue(color_buffer[1]);
        scroll_led_rgb.write(output_array, output_array_size);

        logo_led_rgb.flush();
        scroll_led_rgb.flush();

        logo_led_effect.write(&update_value, 1);
        scroll_led_effect.write(&update_value, 1);
        logo_led_effect.flush();
        scroll_led_effect.flush();
    }
}

void RGBController_OpenRazer::SetAllZoneLEDs(int zone, RGBColor color)
{
    for (int x = 0; x < zones[zone].map.size(); x++)
    {
        for (int y = 0; y < zones[zone].map[x].size(); y++)
        {
            color_buffer[zones[zone].map[x][y]] = color;
        }
    }

    if(matrix_device)
    {
        unsigned int output_array_size = 3 + (color_buffer.size() * 3);
        char output_array[output_array_size];
        char update_value = 1;

        output_array[0] = 0;
        output_array[1] = 0;
        output_array[2] = color_buffer.size() - 1;

        for(int i = 0; i < color_buffer.size(); i++)
        {
            output_array[(i * 3) + 3] = (char)RGBGetRValue(color_buffer[i]);
            output_array[(i * 3) + 4] = (char)RGBGetGValue(color_buffer[i]);
            output_array[(i * 3) + 5] = (char)RGBGetBValue(color_buffer[i]);
        }
        
        matrix_custom_frame.write(output_array, output_array_size);
        matrix_custom_frame.flush();
        matrix_effect_custom.write(&update_value, 1);
        matrix_effect_custom.flush();
    }
    else
    {
        unsigned int output_array_size = 3;
        char output_array[output_array_size];
        char update_value = 0;

        output_array[0] = (char)RGBGetRValue(color_buffer[0]);
        output_array[1] = (char)RGBGetGValue(color_buffer[0]);
        output_array[2] = (char)RGBGetBValue(color_buffer[0]);
        logo_led_rgb.write(output_array, output_array_size);

        output_array[0] = (char)RGBGetRValue(color_buffer[1]);
        output_array[1] = (char)RGBGetGValue(color_buffer[1]);
        output_array[2] = (char)RGBGetBValue(color_buffer[1]);
        scroll_led_rgb.write(output_array, output_array_size);

        logo_led_rgb.flush();
        scroll_led_rgb.flush();

        logo_led_effect.write(&update_value, 1);
        scroll_led_effect.write(&update_value, 1);
        logo_led_effect.flush();
        scroll_led_effect.flush();
    }
}

void RGBController_OpenRazer::SetLED(int led, RGBColor color)
{
    color_buffer[led] = color;

    if(matrix_device)
    {
        unsigned int output_array_size = 3 + (color_buffer.size() * 3);
        char output_array[output_array_size];
        char update_value = 1;

        output_array[0] = 0;
        output_array[1] = 0;
        output_array[2] = color_buffer.size() - 1;

        for(int i = 0; i < color_buffer.size(); i++)
        {
            output_array[(i * 3) + 3] = (char)RGBGetRValue(color_buffer[i]);
            output_array[(i * 3) + 4] = (char)RGBGetGValue(color_buffer[i]);
            output_array[(i * 3) + 5] = (char)RGBGetBValue(color_buffer[i]);
        }
        
        matrix_custom_frame.write(output_array, output_array_size);
        matrix_custom_frame.flush();
        matrix_effect_custom.write(&update_value, 1);
        matrix_effect_custom.flush();
    }
    else
    {
        unsigned int output_array_size = 3;
        char output_array[output_array_size];
        char update_value = 0;

        output_array[0] = (char)RGBGetRValue(color_buffer[0]);
        output_array[1] = (char)RGBGetGValue(color_buffer[0]);
        output_array[2] = (char)RGBGetBValue(color_buffer[0]);
        logo_led_rgb.write(output_array, output_array_size);

        output_array[0] = (char)RGBGetRValue(color_buffer[1]);
        output_array[1] = (char)RGBGetGValue(color_buffer[1]);
        output_array[2] = (char)RGBGetBValue(color_buffer[1]);
        scroll_led_rgb.write(output_array, output_array_size);

        logo_led_rgb.flush();
        scroll_led_rgb.flush();

        logo_led_effect.write(&update_value, 1);
        scroll_led_effect.write(&update_value, 1);
        logo_led_effect.flush();
        scroll_led_effect.flush();
    }
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

static unsigned int GetTypeFromDeviceName(std::string dev_name)
{
    if(dev_name == "Razer Mamba Tournament Edition")
    {
        return(RAZER_MAMBA_TOURNAMENT_EDITION_CHROMA);
    }

    if(dev_name == "Razer DeathAdder Chroma")
    {
        return(RAZER_DEATHADDER_CHROMA);
    }

    if(dev_name == "Razer DeathStalker Chroma")
    {
        return(RAZER_DEATHSTALKER_CHROMA);
    }
}

void RGBController_OpenRazer::SetupMatrixDevice(std::string dev_path)
{
    matrix_custom_frame.open(dev_path + "/matrix_custom_frame");
    matrix_effect_custom.open(dev_path + "/matrix_effect_custom");
    matrix_effect_breath.open(dev_path + "/matrix_effect_breath");
    matrix_effect_none.open(dev_path + "/matrix_effect_none");
    matrix_effect_reactive.open(dev_path + "/matrix_effect_reactive");
    matrix_effect_spectrum.open(dev_path + "/matrix_effect_spectrum");
    matrix_effect_static.open(dev_path + "/matrix_effect_static");
    matrix_effect_wave.open(dev_path + "/matrix_effect_wave");    

    matrix_device = true;
}

void RGBController_OpenRazer::SetupNonMatrixDevice(std::string dev_path)
{
    logo_led_effect.open(dev_path + "/logo_led_effect");
    logo_led_rgb.open(dev_path + "/logo_led_rgb");
    scroll_led_effect.open(dev_path + "/scroll_led_effect");
    scroll_led_rgb.open(dev_path + "/scroll_led_rgb");

    matrix_device = false;
}

RGBController_OpenRazer::RGBController_OpenRazer(std::string dev_path)
{
    name = GetDeviceTypeString(dev_path);

    type = GetTypeFromDeviceName(name);

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

    matrix_device = false;

    switch(type)
    {
        case RAZER_MAMBA_TOURNAMENT_EDITION_CHROMA:
            {
                SetupMatrixDevice(dev_path);
                
                for (int i = 0; i < 16; i++)
                {
                    RGBColor new_color = 0x00000000;
                    color_buffer.push_back(new_color);
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
        
        case RAZER_DEATHADDER_CHROMA:
            {
                SetupNonMatrixDevice(dev_path);

                for (int i = 0; i < 2; i++)
                {
                    RGBColor new_color = 0x00000000;
                    color_buffer.push_back(new_color);
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
                SetupMatrixDevice(dev_path);

                for(int i = 0; i < 12; i++)
                {
                    RGBColor new_color = 0x00000000;
                    color_buffer.push_back(new_color);
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
    }
}