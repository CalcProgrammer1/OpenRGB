/*-----------------------------------------*\
|  RGBController_RazerChromaSDK.h           |
|                                           |
|  Generic RGB Interface for official Razer |
|  Chroma SDK                               |
|                                           |
|  Adam Honse (CalcProgrammer1) 9/2/2019    |
\*-----------------------------------------*/

#include "RGBController_RazerChromaSDK.h"

CREATEEFFECT           RGBController_RazerChromaSDK::CreateEffect           = NULL;
CREATEKEYBOARDEFFECT   RGBController_RazerChromaSDK::CreateKeyboardEffect   = NULL;
CREATEMOUSEEFFECT      RGBController_RazerChromaSDK::CreateMouseEffect      = NULL;
CREATEMOUSEPADEFFECT   RGBController_RazerChromaSDK::CreateMousepadEffect   = NULL;
CREATEHEADSETEFFECT    RGBController_RazerChromaSDK::CreateHeadsetEffect    = NULL;
CREATECHROMALINKEFFECT RGBController_RazerChromaSDK::CreateChromaLinkEffect = NULL;

RGBController_RazerChromaSDK::RGBController_RazerChromaSDK(unsigned int device_type, HMODULE* hModule)
{
    unsigned int led_count = 0;

    for (int i = 0; i < RAZER_NUM_DEVICES; i++)
    {
        if (device_list[i]->type == device_type)
        {
            name = device_list[i]->name;

            for (int zone_id = 0; zone_id < RAZER_MAX_ZONES; zone_id++)
            {
                if (device_list[i]->zones[zone_id] != NULL)
                {
                    device = device_list[i];

                    zone* new_zone = new zone();
                    new_zone->name = device_list[i]->zones[zone_id]->name;

                    std::vector<int>* new_zone_map = new std::vector<int>();

                    for (int led_id = 0; led_id < device_list[i]->zones[zone_id]->leds; led_id++)
                    {
                        RGBColor new_color = 0x00000000;
                        colors.push_back(new_color);

                        led* new_led = new led();
                        new_led->name = device_list[i]->zones[zone_id]->name;
                        leds.push_back(*new_led);

                        new_zone_map->push_back(led_count);

                        led_count++;
                    }

                    zones.push_back(*new_zone);
                }
            }
        }
    }

    if(CreateEffect           == NULL) CreateEffect           = (CREATEEFFECT)GetProcAddress(*hModule, "CreateEffect");
    if(CreateKeyboardEffect   == NULL) CreateKeyboardEffect   = (CREATEKEYBOARDEFFECT)GetProcAddress(*hModule, "CreateKeyboardEffect");
    if(CreateMouseEffect      == NULL) CreateMouseEffect      = (CREATEMOUSEEFFECT)GetProcAddress(*hModule, "CreateMouseEffect");
    if(CreateMousepadEffect   == NULL) CreateMousepadEffect   = (CREATEMOUSEPADEFFECT)GetProcAddress(*hModule, "CreateMousepadEffect");
    if(CreateHeadsetEffect    == NULL) CreateHeadsetEffect    = (CREATEHEADSETEFFECT)GetProcAddress(*hModule, "CreateHeadsetEffect");
    if(CreateChromaLinkEffect == NULL) CreateChromaLinkEffect = (CREATECHROMALINKEFFECT)GetProcAddress(*hModule, "CreateChromaLinkEffect");
}

int RGBController_RazerChromaSDK::GetMode()
{
    return 0;
}

void RGBController_RazerChromaSDK::SetMode(int mode)
{

}

void RGBController_RazerChromaSDK::SetCustomMode()
{

}

void RGBController_RazerChromaSDK::SetAllLEDs(RGBColor color)
{
    for (int i = 0; i < colors.size(); i++)
    {
        colors[i] = color;
    }

    UpdateLEDs();
}

void RGBController_RazerChromaSDK::SetAllZoneLEDs(int zone, RGBColor color)
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

void RGBController_RazerChromaSDK::SetLED(int led, RGBColor color)
{
    colors[led] = color;

    UpdateLEDs();
}

void RGBController_RazerChromaSDK::UpdateLEDs()
{
    switch (device->type)
    {
    case RAZER_FIREFLY_CHROMA:
        {
            //Firefly Chroma
            ChromaSDK::Mousepad::CUSTOM_EFFECT_TYPE FireflyEffect = {};

            for (int x = 0; x < 15; x++)
            {
                FireflyEffect.Color[x] = colors[x];
            }

            CreateMousepadEffect(ChromaSDK::Mousepad::CHROMA_CUSTOM, &FireflyEffect, NULL);
        }
        break;
    }
}