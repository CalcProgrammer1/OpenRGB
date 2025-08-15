/*---------------------------------------------------------*\
| ZETBladeOpticalController.cpp                             |
|                                                           |
|   Driver for ZET Blade                                    |
|                                                           |
|   Based on HyperX Alloy Elite2 implementation by          |
|   KundaPanda                                              |
|                                                           |
|   Moon_darker (Vaker)                         23 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "StringUtils.h"
#include "ZETBladeOpticalController.h"

using namespace std::chrono_literals;

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

/*-----------------------------------------*\
| Skip these indices in the color output    |
\*-----------------------------------------*/
static const unsigned int SKIP_INDICES[] = { 1, 17, 18, 19, 20, 75, 77, 78, 79, 83, 85, 96, 98, 100, 108, 109, 111, 112, 113, 116, 123, 125 };


ZETBladeOpticalController::ZETBladeOpticalController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;

    effect_mode = ZET_BLADE_OPTICAL_MODE_STATIC;
}

ZETBladeOpticalController::~ZETBladeOpticalController()
{
    hid_close(dev);
}

std::string ZETBladeOpticalController::GetDeviceLocation()
{
    return("HID " + location);
}

std::string ZETBladeOpticalController::GetNameString()
{
    return(name);
}

std::string ZETBladeOpticalController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void ZETBladeOpticalController::PrepareHeader(unsigned char* packet, unsigned char brightness)
{
    PrepareHeader(packet, 0x1C, 0x02, brightness, 0xFF); // Custom 2, 2, -, separator
}

void ZETBladeOpticalController::PrepareHeader(unsigned char* packet, unsigned char mode, unsigned char speed, unsigned char brightness, unsigned char color)
{
    /*-----------------------------------------------------*\
    | Prepare packet header                                 |
    \*-----------------------------------------------------*/
    packet[0x00]   = 0x04; // Report ID
    packet[0x01]   = 0xAE; // RGB Control Packet (KB = 0xA0)
    packet[0x02]   = 0x01; // unk
    packet[0x05]   = mode; // Mode
    packet[0x06]   = speed; // Speed, 0-4
    packet[0x07]   = brightness; // Brightness, 0-4
    packet[0x08]   = color; // Separator FF or Color, 0-7 (0-6 in static color mode) (Rainbow,) R, G, B, Y, M, C, W
}

void ZETBladeOpticalController::SetLEDDirect(const std::vector<RGBColor>& colors, unsigned char brightness)
{
    unsigned char buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer and prepare packet header             |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));
    PrepareHeader(buf, brightness);

    /*-----------------------------------------------------*\
    | Variables to keep track of color sending and skipping |
    \*-----------------------------------------------------*/
    size_t buf_idx                  = ZET_BLADE_OPTICAL_HEADER_LEN;
    size_t color_idx                = 0;
    size_t packets_sent             = 0;
    size_t skipped                  = 0;
    const unsigned int* skip_idx    = &SKIP_INDICES[0];
    bool last_color                 = false;
    bool ending_flag                = false;

    /*-----------------------------------------------------*\
    | Continue filling and sending packets while color data |
    | remains                                               |
    \*-----------------------------------------------------*/
    while(color_idx < colors.size())
    {
        /*-------------------------------------------------*\
        | If at a skipped index, increment skipped count    |
        | and index                                         |
        \*-------------------------------------------------*/
        if(*skip_idx == color_idx + skipped)
        {
            skip_idx++;

            if(skip_idx >= SKIP_INDICES + sizeof(SKIP_INDICES) / sizeof(unsigned int))
            {
                skip_idx = SKIP_INDICES;
            }

            skipped++;
            continue;
        }

        /*-------------------------------------------------*\
        | Packets have colors in groups of 4 bytes, with    |
        | the first byte being key id and then R, G, B.     |
        \*-------------------------------------------------*/
        buf[buf_idx]        = (unsigned char)(color_idx + skipped + ZET_BLADE_OPTICAL_KEY_OFFSET);
        buf[buf_idx + 1]    = RGBGetRValue(colors[color_idx]);
        buf[buf_idx + 2]    = RGBGetGValue(colors[color_idx]);
        buf[buf_idx + 3]    = RGBGetBValue(colors[color_idx]);

        /*-------------------------------------------------*\
        | Increment packet buffer index by 4 bytes          |
        \*-------------------------------------------------*/
        buf_idx += ZET_BLADE_OPTICAL_COLOR_LEN;
        color_idx++;
        last_color = (color_idx == colors.size());

        /*-------------------------------------------------*\
        | If the packet buffer is full, send it and reset   |
        | buffer indexing                                   |
        | OR                                                |
        | If all colors have been filled into the buffer,   |
        | send the packet                                   |
        \*-------------------------------------------------*/
        if((buf_idx + ZET_BLADE_OPTICAL_HEADER_LEN >= sizeof(buf)) || last_color)
        {
            /*---------------------------------------------*\
            | If we still have place for an                 |
            | ending sequence - squeeze it in!              |
            \*---------------------------------------------*/
            if(last_color && (buf_idx + ZET_BLADE_OPTICAL_COLOR_LEN < sizeof(buf)))
            {
                buf[buf_idx] = 0xFF;
                ending_flag = true;
            }

            /*---------------------------------------------*\
            | Send packet                                   |
            \*---------------------------------------------*/
            hid_write(dev, buf, sizeof(buf));

            /*---------------------------------------------*\
            | Wait for the poor slowpoke to process packet  |
            \*---------------------------------------------*/
            std::this_thread::sleep_for(ZET_BLADE_OPTICAL_DELAY);

            /*---------------------------------------------*\
            | Zero out buffer, reset index, prepare header  |
            \*---------------------------------------------*/
            memset(buf, 0x00, sizeof(buf));
            buf_idx = ZET_BLADE_OPTICAL_HEADER_LEN;
            PrepareHeader(buf, brightness);

            /*---------------------------------------------*\
            | Increment packet counter                      |
            \*---------------------------------------------*/
            packets_sent++;
        }
    }

    /*---------------------------------------------*\
    | If there's anything left to send - send it    |
    \*---------------------------------------------*/
    if(!ending_flag)
    {
        buf[buf_idx] = 0xFF;
        hid_write(dev, buf, sizeof(buf));
        std::this_thread::sleep_for(ZET_BLADE_OPTICAL_DELAY);
    }
}

unsigned char ZETBladeOpticalController::RGBToPalette(unsigned char red,
                                                      unsigned char grn,
                                                      unsigned char blu
                                                     )
{
    /*------------------------*\
    | 0 0 1 (1) -> (1) Red     |
    | 0 1 0 (2) -> (2) Green   |
    | 0 1 1 (3) -> (4) Yellow  |
    | 1 0 0 (4) -> (3) Blue    |
    | 1 0 1 (5) -> (5) Magenta |
    | 1 1 0 (6) -> (6) Cyan    |
    | 1 1 1 (7) -> (7) White   |
    \*------------------------*/
    unsigned char color_mask = ((blu > 127) << 2 & 4) | ((grn > 127) << 1 & 2) | ((red > 127) & 1);

    switch(color_mask) // (Rainbow/Off,) R, G, B, Y, M, C, W
    {
        case 3:
            return 4;
        case 4:
            return 3;
        default:
            return color_mask;
    }
}

void ZETBladeOpticalController::SetEffect(unsigned char mode,
                                     unsigned char speed,
                                     unsigned char brightness,
                                     bool          random,
                                     unsigned char red,
                                     unsigned char grn,
                                     unsigned char blu
                                    )
{
    /*-------------------------------------------------------------*\
    | Prep some status variables and return if we're in custom mode |
    \*-------------------------------------------------------------*/
    bool static_mode = (mode == ZET_BLADE_OPTICAL_MODE_STATIC);
    effect_mode = mode;
    custom_mode = (effect_mode == ZET_BLADE_OPTICAL_MODE_CUSTOM);

    if(custom_mode)
    {
        return;
    }

    unsigned char color = RGBToPalette(red, grn, blu);
    unsigned char buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer and prepare packet                    |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    brightness = (static_mode && color == 0) ? 0 : brightness;
    color = (static_mode && color > 0) ? (color - 1) : color;
    color = random ? 0 : color;
    PrepareHeader(buf, mode, speed, brightness, color);

    /*---------------------------------------------*\
    | Send packet... and wait                       |
    \*---------------------------------------------*/
    hid_write(dev, buf, sizeof(buf));
    std::this_thread::sleep_for(ZET_BLADE_OPTICAL_DELAY);
}

