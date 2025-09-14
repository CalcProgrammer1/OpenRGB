/*---------------------------------------------------------*\
| RGBController_ElgatoStreamDeck.cpp                        |
|                                                           |
|   RGBController for Elgato Stream Deck MK.2               |
|                                                           |
|   Ferréol DUBOIS COLI (Fefe_du_973)           23 Jan 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "RGBController_ElgatoStreamDeck.h"
#include "stb_image_write.h"

/**------------------------------------------------------------------*\
    @name Elgato Stream Deck MK.2 15 Buttons
    @category Accessory
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectElgatoStreamDeckControllers
    @comment 
\*-------------------------------------------------------------------*/

RGBController_ElgatoStreamDeck::RGBController_ElgatoStreamDeck(ElgatoStreamDeckController *controller_ptr) : controller(controller_ptr)
{
    name = "Elgato Stream Deck MK.2";
    vendor = "Elgato";
    type = DEVICE_TYPE_ACCESSORY;
    description = "Stream Deck MK.2 Controller";
    location = controller->GetLocation();

    mode Direct;
    Direct.name = "Direct";
    Direct.value = 0;
    Direct.flags = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_ElgatoStreamDeck::~RGBController_ElgatoStreamDeck()
{
    delete controller;
}

void RGBController_ElgatoStreamDeck::SetupZones()
{
    zone deck_zone;
    deck_zone.name = "Button Matrix";
    deck_zone.type = ZONE_TYPE_MATRIX;
    deck_zone.leds_min = 15;
    deck_zone.leds_max = 15;
    deck_zone.leds_count = 15;
    deck_zone.matrix_map = new matrix_map_type;
    deck_zone.matrix_map->height = 3;
    deck_zone.matrix_map->width = 5;
    deck_zone.matrix_map->map = new unsigned int[15]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};

    zones.push_back(deck_zone);

    for(unsigned int i = 0; i < 15; i++)
    {
        led new_led;
        new_led.name = "Button " + std::to_string(i + 1);
        leds.push_back(new_led);
    }

    SetupColors();
}

std::vector<unsigned char> RGBController_ElgatoStreamDeck::CreateButtonImage(const RGBColor &color)
{
    const int width = 72;
    const int height = 72;
    std::vector<unsigned char> pixels(width * height * 3);

    unsigned char r = RGBGetRValue(color);
    unsigned char g = RGBGetGValue(color);
    unsigned char b = RGBGetBValue(color);

    for(int i = 0; i < width * height; i++)
    {
        pixels[i * 3 + 0] = r;
        pixels[i * 3 + 1] = g;
        pixels[i * 3 + 2] = b;
    }

    std::vector<unsigned char> jpegData;
    stbi_write_jpg_to_func([](void *context, void *data, int size)
                           {
        std::vector<unsigned char>* vec = static_cast<std::vector<unsigned char>*>(context);
        vec->insert(vec->end(), static_cast<unsigned char*>(data), static_cast<unsigned char*>(data) + size); }, &jpegData, width, height, 3, pixels.data(), 95); // Quality 95

    return jpegData;
}

void RGBController_ElgatoStreamDeck::DeviceUpdateLEDs()
{
    std::vector<std::vector<unsigned char>> buttonImages;
    for(unsigned int i = 0; i < leds.size(); i++)
    {
        buttonImages.push_back(CreateButtonImage(colors[i]));
    }
    controller->SendFullFrame(buttonImages);
}

void RGBController_ElgatoStreamDeck::UpdateZoneLEDs(int zone)
{
    DeviceUpdateLEDs();
}

void RGBController_ElgatoStreamDeck::UpdateSingleLED(int led)
{
    DeviceUpdateLEDs();
}

void RGBController_ElgatoStreamDeck::ResizeZone(int, int) {}
void RGBController_ElgatoStreamDeck::DeviceUpdateMode() {}
