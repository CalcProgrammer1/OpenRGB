/*---------------------------------------------------------*\
| LIFXController.cpp                                        |
|                                                           |
|   Driver for LIFX                                         |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      05 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "LIFXController.h"
#include <nlohmann/json.hpp>
#include "hsv.h"

using json = nlohmann::json;
using namespace std::chrono_literals;

LIFXController::LIFXController(std::string ip, std::string name)
{
    this->name  = name;

    /*-----------------------------------------------------------------*\
    | Fill in location string with device's IP address                  |
    \*-----------------------------------------------------------------*/
    location    = "IP: " + ip;

    /*-----------------------------------------------------------------*\
    | Open a UDP client sending to the device's IP, port 56700          |
    \*-----------------------------------------------------------------*/
    port.udp_client(ip.c_str(), "56700");
}

LIFXController::~LIFXController()
{

}

std::string LIFXController::GetLocation()
{
    return(location);
}

std::string LIFXController::GetName()
{
    return(name);
}

std::string LIFXController::GetVersion()
{
    return(module_name + " " + firmware_version);
}

std::string LIFXController::GetManufacturer()
{
    return("LIFX");
}

std::string LIFXController::GetUniqueID()
{
    return(module_mac);
}

void LIFXController::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    RGBColor color = ToRGBColor(red, green, blue);
    hsv_t hsv;
    rgb2hsv(color, &hsv);

    data = data_buf;
    memset( data, 0, 49 );

    source      = 2;
    sequence    = 1;

    unsigned char target[8] = {0};

    FrameHeader( 49, true, false, 0, source );
    FrameAddress( target, false, false, sequence );
    ProtocolAddress( 102 );

    unsigned char * set_color   = &data[36];

    unsigned short hue          = hsv.hue        * (65536/360);
    unsigned short saturation   = hsv.saturation * (65536/256);
    unsigned short brightness   = hsv.value      * (65536/256);
    unsigned short kelvin       = 3500;
    unsigned int   duration     = 0;

    memcpy(&set_color[LIFX_SET_COLOR_OFFSET_HUE],        &hue,        sizeof(unsigned short));
    memcpy(&set_color[LIFX_SET_COLOR_OFFSET_SATURATION], &saturation, sizeof(unsigned short));
    memcpy(&set_color[LIFX_SET_COLOR_OFFSET_BRIGHTNESS], &brightness, sizeof(unsigned short));
    memcpy(&set_color[LIFX_SET_COLOR_OFFSET_KELVIN],     &kelvin,     sizeof(unsigned short));
    memcpy(&set_color[LIFX_SET_COLOR_OFFSET_DURATION],   &duration,   sizeof(unsigned int));

    port.udp_write((char *)data, 49);
}

void LIFXController::FrameHeader
    (
    unsigned short  size,
    bool            addressable,
    bool            tagged,
    unsigned char   origin,
    unsigned int    source
    )
{
    unsigned short protocol = 1024;
    memcpy(&data[LIFX_FRAME_HEADER_OFFSET_SIZE],     &size,     sizeof(unsigned short));
    memcpy(&data[LIFX_FRAME_HEADER_OFFSET_PROTOCOL], &protocol, sizeof(unsigned short));

    if(addressable)
    {
        data[LIFX_FRAME_HEADER_OFFSET_FLAGS] |= (1 << 4);
    }
    if(tagged)
    {
        data[LIFX_FRAME_HEADER_OFFSET_FLAGS] |= (1 << 5);
    }

    data[LIFX_FRAME_HEADER_OFFSET_FLAGS]     |= (origin << 6);

    memcpy(&data[LIFX_FRAME_HEADER_OFFSET_SOURCE],   &source, sizeof(unsigned int));
}

void LIFXController::FrameAddress
    (
    unsigned char * target,
    bool            res_required,
    bool            ack_required,
    unsigned char   sequence
    )
{
    memcpy(&data[LIFX_FRAME_ADDRESS_OFFSET_TARGET], target, 8);

    data[LIFX_FRAME_ADDRESS_OFFSET_FLAGS] = 0;

    if(res_required)
    {
        data[LIFX_FRAME_ADDRESS_OFFSET_FLAGS] |= (1 << 0);
    }

    if(ack_required)
    {
        data[LIFX_FRAME_ADDRESS_OFFSET_FLAGS] |= (1 << 1);
    }

    data[LIFX_FRAME_ADDRESS_OFFSET_SEQUENCE]    = sequence;
}

void LIFXController::ProtocolAddress
    (
    unsigned short  type
    )
{
    memcpy(&data[LIFX_PROTOCOL_HEADER_OFFSET_TYPE], &type, sizeof(unsigned short));
}
