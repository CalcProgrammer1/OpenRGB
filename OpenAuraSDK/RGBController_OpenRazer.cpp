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

}

void RGBController_OpenRazer::SetAllLEDs(RGBColor color)
{
    for(int i = 0; i < color_buffer.size(); i++)
    {
        color_buffer[i] = color;
    }

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

void RGBController_OpenRazer::SetAllZoneLEDs(int zone, RGBColor color)
{
    for (int x = 0; x < zones[zone].map.size(); x++)
    {
        for (int y = 0; y < zones[zone].map[x].size(); y++)
        {
            color_buffer[zones[zone].map[x][y]] = color;
        }
    }

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

void RGBController_OpenRazer::SetLED(int led, RGBColor color)
{
    color_buffer[led] = color;

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
}

RGBController_OpenRazer::RGBController_OpenRazer(std::string dev_path)
{
    name = GetDeviceTypeString(dev_path);

    type = GetTypeFromDeviceName(name);

    matrix_custom_frame.open(dev_path + "/matrix_custom_frame");
    matrix_effect_custom.open(dev_path + "/matrix_effect_custom");

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

    switch(type)
    {
        case RAZER_MAMBA_TOURNAMENT_EDITION_CHROMA:
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

            zone left_zone, right_zone, logo_zone, wheel_zone;

            left_zone.name = "Left Strip";
            left_zone.type = ZONE_TYPE_LINEAR;
            std::vector<int> left_zone_map;
            for(int i = 0; i < 7; i++)
            {
                left_zone_map.push_back(i);
            }
            left_zone.map.push_back(left_zone_map);
            zones.push_back(left_zone);

            right_zone.name = "Right Strip";
            right_zone.type = ZONE_TYPE_LINEAR;
            std::vector<int> right_zone_map;
            for(int i = 7; i < 14; i++)
            {
                right_zone_map.push_back(i);
            }
            right_zone.map.push_back(right_zone_map);
            zones.push_back(right_zone);

            logo_zone.name = "Logo";
            logo_zone.type = ZONE_TYPE_SINGLE;
            std::vector<int> logo_zone_map;
            logo_zone_map.push_back(14);
            logo_zone.map.push_back(logo_zone_map);
            zones.push_back(logo_zone);

            wheel_zone.name = "Mouse Wheel";
            wheel_zone.type = ZONE_TYPE_SINGLE;
            std::vector<int> wheel_zone_map;
            wheel_zone_map.push_back(15);
            wheel_zone.map.push_back(wheel_zone_map);
            zones.push_back(wheel_zone);
            break;
            
    }
}