/*---------------------------------------------------------*\
| RGBController_AcerNitroHidKeyboard.cpp                    |
|                                                           |
|   Driver for Acer Nitro Keyboard                          |
|                                                           |
|   Marek Pokropiński                           Jul 27 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "AcerNitroHidKeyboardController.h"

AcerNitroHidKeyboardController::AcerNitroHidKeyboardController(hid_device *dev_handle, const char *path)
{
    this->location  = path;
    this->dev       = dev_handle;

    device_name     = "Acer Nitro HID Keyboard";    
}

AcerNitroHidKeyboardController::~AcerNitroHidKeyboardController()
{
    if(dev)
    {
        hid_close(dev);
    }
}

std::string AcerNitroHidKeyboardController::GetLocation() const
{
    return("HID: " + location);
}

std::string AcerNitroHidKeyboardController::GetDeviceName() const
{
    return(device_name);
}

void AcerNitroHidKeyboardController::SetLEDs(const std::vector<RGBColor>& colors, unsigned char brightness, unsigned char device_id)
{
    std::lock_guard<std::mutex> guard(device_mutex);

    for(unsigned int led_idx = 0; (led_idx < colors.size()) && (led_idx < ACER_HID_NUM_ZONES); led_idx++)
    {
        /*-------------------------------------------------*\
        | Convert colors to RGB format                      |
        \*-------------------------------------------------*/
        unsigned char r = RGBGetRValue(colors[led_idx]);
        unsigned char g = RGBGetGValue(colors[led_idx]);        
        unsigned char b = RGBGetBValue(colors[led_idx]);

        /*-------------------------------------------------*\
        | Send packet                                       |
        \*-------------------------------------------------*/
        unsigned char zone_select                   = (unsigned char)(1<<led_idx);
        unsigned char packet[ACER_HID_PACKET_SIZE]  = {0xA4, device_id, ACER_HID_MODE_STATIC, brightness, 0, 1, r, g, b, zone_select, 0};
        hid_send_feature_report(dev, packet, ACER_HID_PACKET_SIZE);
    }
}

void AcerNitroHidKeyboardController::SetAllLEDs(const RGBColor &color, unsigned char brightness, unsigned char speed, unsigned char direction, unsigned char mode, unsigned char device_id)
{
    std::lock_guard<std::mutex> guard(device_mutex);

    /*-----------------------------------------------------*\
    | Convert color to RGB format                           |
    \*-----------------------------------------------------*/
    unsigned char r = RGBGetRValue(color);
    unsigned char g = RGBGetGValue(color);        
    unsigned char b = RGBGetBValue(color);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    | Zone select = All zones                               |
    \*-----------------------------------------------------*/
    unsigned char zone_select                   = 0x0F;
    unsigned char packet[ACER_HID_PACKET_SIZE]  = {0xA4, device_id, mode, brightness, speed, direction, r, g, b, zone_select, 0};
    hid_send_feature_report(dev, packet, ACER_HID_PACKET_SIZE);
}
