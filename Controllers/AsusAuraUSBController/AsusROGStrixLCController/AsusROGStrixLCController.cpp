/*---------------------------------------------------------*\
| AsusROGStrixLCController.cpp                              |
|                                                           |
|   Driver for ASUS Aura liquid cooler                      |
|                                                           |
|   Chris M (Dr_No)                             17 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "AsusROGStrixLCController.h"

AsusROGStrixLCController::AsusROGStrixLCController(hid_device* dev_handle, const char* path, std::string dev_name) : AuraUSBController(dev_handle, path, dev_name)
{
    /*-----------------------------------------------------*\
    | Add addressable devices                               |
    |    Manually adding device info for now                |
    |    TODO: Implement config table accurately            |
    |    LC120 - 1F FF 05 05 04 00 00 00                    |
    \*-----------------------------------------------------*/
    device_info.push_back({0x00, 0x00, 4, 0, AuraDeviceType::FIXED});
}

AsusROGStrixLCController::~AsusROGStrixLCController()
{
    // Device will close at AuraUSBController destructor
}

std::string AsusROGStrixLCController::GetLocation()
{
    return("HID: " + location);
}

void AsusROGStrixLCController::SetMode(unsigned char /*channel*/, unsigned char /*mode*/, unsigned char /*red*/, unsigned char /*grn*/, unsigned char /*blu*/)
{
    /*---------------------------------------------------------*\
    | This interface is not used in this controller however is  |
    |   required by the abstract class                          |
    \*---------------------------------------------------------*/
}

void AsusROGStrixLCController::SetMode(unsigned char mode, unsigned char speed, unsigned char direction, RGBColor colour)
{
    bool needs_update       = !( (current_mode == mode) && (ToRGBColor(current_red, current_green, current_blue) == colour) && (current_speed == speed) && (current_direction == direction));

    if (needs_update)
    {
        current_mode        = mode;
        current_speed       = speed;
        current_direction   = direction;
        current_red         = RGBGetRValue(colour);
        current_green       = RGBGetGValue(colour);
        current_blue        = RGBGetBValue(colour);
        SendUpdate();
    }
}

void AsusROGStrixLCController::SetChannelLEDs(unsigned char /*channel*/, RGBColor* /*colors*/, unsigned int /*num_colors*/)
{
    /*---------------------------------------------------------*\
    | This interface is not used in this controller however is  |
    |   required by the abstract class                          |
    \*---------------------------------------------------------*/
}

void AsusROGStrixLCController::SetLedsDirect(RGBColor * led_colours, uint8_t led_count)
{
    uint8_t buffer[write_packet_size]   = { 0xEC, 0x40, 0x00, 0xFF, led_count };

    /*---------------------------------------------------------*\
    | Set the colour bytes in the packet                        |
    \*---------------------------------------------------------*/
    for(uint8_t index = 0; index < led_count; index++)
    {
        uint8_t offset      = (index * 3) + ROGSTRIXLC_GREEN_BYTE;

        buffer[offset + 0]  = RGBGetRValue(led_colours[index]);
        buffer[offset + 1]  = RGBGetGValue(led_colours[index]);
        buffer[offset + 2]  = RGBGetBValue(led_colours[index]);
    }

    /*---------------------------------------------------------*\
    | These 3 bytes might be timing bytes                       |
    \*---------------------------------------------------------*/
    uint8_t offset          = led_count * 3 + ROGSTRIXLC_GREEN_BYTE;
    buffer[offset + 0]      = 0x77;
    buffer[offset + 1]      = 0x10;
    buffer[offset + 2]      = 0xF3;

    hid_write(dev, buffer, write_packet_size);
}

void AsusROGStrixLCController::GetStatus()
{
    uint8_t buffer[write_packet_size]   = { 0xEC, 0x01, 0x02 };

    hid_write(dev, buffer, write_packet_size);
    hid_read_timeout(dev, buffer, read_packet_size, ROGSTRIXLC_CONTROLLER_TIMEOUT);

    current_red             = buffer[ROGSTRIXLC_RED_BYTE - 1];
    current_green           = buffer[ROGSTRIXLC_GREEN_BYTE - 1];
    current_blue            = buffer[ROGSTRIXLC_BLUE_BYTE - 1];
}

void AsusROGStrixLCController::SendUpdate()
{
    uint8_t buffer[write_packet_size];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buffer, 0x00, write_packet_size);

    buffer[ROGSTRIXLC_REPORTID_BYTE]    = rogstrixlc_reportid;
    buffer[ROGSTRIXLC_COMMAND_BYTE]     = rogstrixlc_modefx;
    buffer[ROGSTRIXLC_ZONE_BYTE]        = 0;
    buffer[ROGSTRIXLC_MODE_BYTE]        = current_mode;
    buffer[ROGSTRIXLC_RED_BYTE]         = current_red;
    buffer[ROGSTRIXLC_GREEN_BYTE]       = current_green;
    buffer[ROGSTRIXLC_BLUE_BYTE]        = current_blue;

    buffer[ROGSTRIXLC_DIRECTION_BYTE]   = current_direction;
    buffer[ROGSTRIXLC_SPEED_BYTE]       = current_speed;

    hid_write(dev, buffer, write_packet_size);
}

