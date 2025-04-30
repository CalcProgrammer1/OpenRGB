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
#include <climits>
#include "hsv.h"

using json = nlohmann::json;
using namespace std::chrono_literals;

LIFXController::LIFXController(std::string ip, std::string name, bool multizone, bool extended_multizone)
{
    this->name               = name;
    this->zone_count         = 1;
    this->multizone          = multizone;
    this->extended_multizone = extended_multizone;

    /*-----------------------------------------------------------------*\
    | Fill in location string with device's IP address                  |
    \*-----------------------------------------------------------------*/
    location = "IP: " + ip;

    /*-----------------------------------------------------------------*\
    | Open a UDP client sending to the device's IP, port 56700          |
    \*-----------------------------------------------------------------*/
    port.udp_client(ip.c_str(), LIFX_UDP_PORT);
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
    return(DEFAULT_MANUFACTURER);
}

std::string LIFXController::GetUniqueID()
{
    return(module_mac);
}

unsigned int LIFXController::GetZoneCount()
{
    return(zone_count);
}

void LIFXController::SetColors(std::vector<RGBColor> colors)
{
    /*-------------------------*\
    | Non-multizone lifx device |
    \*-------------------------*/
    if(!multizone)
    {
        SetColor(colors[0]);

        return;
    }

    /*-------------------------------------------*\
    | Multizone lifx device with extended support |
    \*-------------------------------------------*/
    if(extended_multizone)
    {
        SetZoneColors(colors);

        return;
    }

    /*----------------------------------------------*\
    | Multizone lifx device without extended support |
    \*----------------------------------------------*/
    for(size_t i = 0; i < zone_count; i++)
    {
        /*-----------------------------------------------------------------*\
        | Utilize caching to avoid setting all zones when 1 zone is changed |
        \*-----------------------------------------------------------------*/
        if(cached_colors[i] == colors[i])
        {
            continue;
        }

        SetZoneColor(colors[i], i);
        cached_colors[i] = colors[i];
    }
}

void LIFXController::FetchZoneCount()
{
    if(!multizone)
    {
        return;
    }

    /*---------------------------*\
    | Send get color zones packet |
    \*---------------------------*/
    data_buf_size = LIFX_PACKET_HEADER_LENGTH + LIFX_GET_COLOR_ZONES_PACKET_LENGTH;
    data = new unsigned char[data_buf_size];
    memset(data, 0, data_buf_size);

    HeaderPacketSetDefaults(LIFX_PACKET_TYPE_GET_COLOR_ZONES);

    GetColorZonesPacketSetStartIndex(0);
    GetColorZonesPacketSetEndIndex(0);

    port.udp_write((char*)data, data_buf_size);
    delete[] data;

    /*----------------------------*\
    | Listen for state zone packet |
    \*----------------------------*/
    data_buf_size = LIFX_PACKET_HEADER_LENGTH + LIFX_STATE_ZONE_PACKET_LENGTH;
    data = new unsigned char[data_buf_size];
    memset(data, 0, data_buf_size);

    port.udp_listen_timeout((char*)data, data_buf_size, 5, 0);

    /*-----------------*\
    | Validate response |
    \*-----------------*/
    if(HeaderPacketGetSize() != data_buf_size || HeaderPacketGetProtocol() != LIFX_PROTOCOL || HeaderPacketGetPacketType() != LIFX_PACKET_TYPE_STATE_ZONE)
    {
        return;
    }

    zone_count = StateZonePacketGetZonesCount();
    delete[] data;
}

void LIFXController::SetColor(RGBColor color)
{
    /*---------------------*\
    | Send set color packet |
    \*---------------------*/
    data_buf_size = LIFX_PACKET_HEADER_LENGTH + LIFX_SET_COLOR_PACKET_LENGTH;
    data = new unsigned char[data_buf_size];
    memset(data, 0, data_buf_size);

    HeaderPacketSetDefaults(LIFX_PACKET_TYPE_SET_COLOR);

    hsbk_t hsbk;
    RGBColorToHSBK(color, &hsbk);

    SetColorPacketSetHSBK(&hsbk);
    SetColorPacketSetDuration(0);

    port.udp_write((char*)data, data_buf_size);
    delete[] data;
}

void LIFXController::SetZoneColor(RGBColor color, unsigned int zone)
{
    /*---------------------------*\
    | Send set color zones packet |
    \*---------------------------*/
    data_buf_size = LIFX_PACKET_HEADER_LENGTH + LIFX_SET_COLOR_ZONES_PACKET_LENGTH;
    data = new unsigned char[data_buf_size];
    memset(data, 0, data_buf_size);

    HeaderPacketSetDefaults(LIFX_PACKET_TYPE_SET_COLOR_ZONES);

    SetColorZonesPacketSetStartIndex(zone);
    SetColorZonesPacketSetEndIndex(zone);

    hsbk_t hsbk;
    RGBColorToHSBK(color, &hsbk);

    SetColorZonesPacketSetHSBK(&hsbk);
    SetColorZonesPacketSetDuration(0);
    SetColorZonesPacketSetApply(LIFX_MULTIZONE_APPLICATION_REQUEST_APPLY);

    port.udp_write((char*)data, data_buf_size);
    delete[] data;
}

void LIFXController::SetZoneColors(std::vector<RGBColor> colors)
{
    /*------------------------------------*\
    | Send set extended color zones packet |
    \*------------------------------------*/
    data_buf_size = LIFX_PACKET_HEADER_LENGTH + LIFX_SET_EXTENDED_COLOR_ZONES_PACKET_LENGTH;
    data = new unsigned char[data_buf_size];
    memset(data, 0, data_buf_size);

    HeaderPacketSetDefaults(LIFX_PACKET_TYPE_SET_EXTENDED_COLOR_ZONES);

    SetExtendedColorZonesPacketSetDuration(0);
    SetExtendedColorZonesPacketSetApply(LIFX_MULTIZONE_APPLICATION_REQUEST_APPLY);
    SetExtendedColorZonesPacketSetZoneIndex(0);
    SetExtendedColorZonesPacketSetColors(colors);

    port.udp_write((char*)data, data_buf_size);
    delete[] data;
}

void LIFXController::RGBColorToHSBK(RGBColor color, hsbk_t* hsbk)
{
    hsv_t hsv;
    rgb2hsv(color, &hsv);

    hsbk->hue = hsv.hue * (USHRT_MAX/360);
    hsbk->saturation = hsv.saturation * (USHRT_MAX/256);
    hsbk->brightness = hsv.value * (USHRT_MAX/256);
    hsbk->kelvin = DEFAULT_KELVIN;
}

/*----------------------------*\
| Header packet helper methods |
\*----------------------------*/
void LIFXController::HeaderPacketSetDefaults(unsigned short packet_type)
{
    /*-----*\
    | Frame |
    \*-----*/
    HeaderPacketSetSize(data_buf_size);
    HeaderPacketSetProtocol();
    HeaderPacketSetAddressable(true);
    HeaderPacketSetTagged(false);
    HeaderPacketSetOrigin(0);
    HeaderPacketSetSource(2);

    /*-------------*\
    | Frame address |
    \*-------------*/
    unsigned char target[TARGET_LENGTH] = {0};
    HeaderPacketSetTarget(target);
    HeaderPacketSetResponseRequired(false);
    HeaderPacketSetAcknowledgeRequired(false);
    HeaderPacketSetSequence(++sequence);

    /*---------------*\
    | Protocol header |
    \*---------------*/
    HeaderPacketSetPacketType(packet_type);
}

unsigned short LIFXController::HeaderPacketGetSize()
{
    return data[LIFX_HEADER_PACKET_OFFSET_SIZE];
}

void LIFXController::HeaderPacketSetSize(unsigned short size)
{
    memcpy(&data[LIFX_HEADER_PACKET_OFFSET_SIZE], &size, sizeof(unsigned short));
}

unsigned short LIFXController::HeaderPacketGetProtocol()
{
    unsigned short protocol;
    memcpy(&protocol, &data[LIFX_HEADER_PACKET_OFFSET_PROTOCOL], sizeof(unsigned short));
    return protocol & 0x0FFF;
}

void LIFXController::HeaderPacketSetProtocol(unsigned short protocol)
{
    data[LIFX_HEADER_PACKET_OFFSET_PROTOCOL] = protocol & 0xFF;
    unsigned char current = data[LIFX_HEADER_PACKET_OFFSET_ADDRESSABLE_TAGGED_ORIGIN];
    data[LIFX_HEADER_PACKET_OFFSET_ADDRESSABLE_TAGGED_ORIGIN] = (current & 0xF0) | ((protocol >> 8) & 0x0F);
}

void LIFXController::HeaderPacketSetAddressable(bool addressable)
{
    if(addressable)
        data[LIFX_HEADER_PACKET_OFFSET_ADDRESSABLE_TAGGED_ORIGIN] |= 0x10;
    else
        data[LIFX_HEADER_PACKET_OFFSET_ADDRESSABLE_TAGGED_ORIGIN] &= ~0x10;
}

void LIFXController::HeaderPacketSetTagged(bool tagged)
{
    if(tagged)
        data[LIFX_HEADER_PACKET_OFFSET_ADDRESSABLE_TAGGED_ORIGIN] |= 0x20;
    else
        data[LIFX_HEADER_PACKET_OFFSET_ADDRESSABLE_TAGGED_ORIGIN] &= ~0x20;
}

void LIFXController::HeaderPacketSetOrigin(unsigned char origin)
{
    data[LIFX_HEADER_PACKET_OFFSET_ADDRESSABLE_TAGGED_ORIGIN] =
        (data[LIFX_HEADER_PACKET_OFFSET_ADDRESSABLE_TAGGED_ORIGIN] & 0xFC) | (origin & 0x03);
}

void LIFXController::HeaderPacketSetSource(unsigned int source)
{
    memcpy(&data[LIFX_HEADER_PACKET_OFFSET_SOURCE], &source, sizeof(unsigned int));
}

void LIFXController::HeaderPacketSetTarget(unsigned char* target)
{
    memcpy(&data[LIFX_HEADER_PACKET_OFFSET_TARGET], target, TARGET_LENGTH);
}

void LIFXController::HeaderPacketSetResponseRequired(bool response_required)
{
    if(response_required)
        data[LIFX_HEADER_PACKET_OFFSET_RESPONSE_REQUIRED_ACKNOWLEDGE_REQUIRED] |= 0x01;
    else
        data[LIFX_HEADER_PACKET_OFFSET_RESPONSE_REQUIRED_ACKNOWLEDGE_REQUIRED] &= ~0x01;
}

void LIFXController::HeaderPacketSetAcknowledgeRequired(bool acknowledge_required)
{
    if(acknowledge_required)
        data[LIFX_HEADER_PACKET_OFFSET_RESPONSE_REQUIRED_ACKNOWLEDGE_REQUIRED] |= 0x02;
    else
        data[LIFX_HEADER_PACKET_OFFSET_RESPONSE_REQUIRED_ACKNOWLEDGE_REQUIRED] &= ~0x02;
}

void LIFXController::HeaderPacketSetSequence(unsigned char sequence)
{
    data[LIFX_HEADER_PACKET_OFFSET_SEQUENCE] = sequence;
}

unsigned short LIFXController::HeaderPacketGetPacketType()
{
    unsigned short packet_type_value;
    memcpy(&packet_type_value, &data[LIFX_HEADER_PACKET_OFFSET_PACKET_TYPE], sizeof(unsigned short));

    return packet_type_value;
}

void LIFXController::HeaderPacketSetPacketType(unsigned short packet_type)
{
    memcpy(&data[LIFX_HEADER_PACKET_OFFSET_PACKET_TYPE], &packet_type, sizeof(unsigned short));
}

/*-------------------------------*\
| Set color packet helper methods |
\*-------------------------------*/
void LIFXController::SetColorPacketSetHSBK(hsbk_t* hsbk)
{
    memcpy(&data[LIFX_SET_COLOR_PACKET_OFFSET_HUE],        &hsbk->hue,        sizeof(unsigned short));
    memcpy(&data[LIFX_SET_COLOR_PACKET_OFFSET_SATURATION], &hsbk->saturation, sizeof(unsigned short));
    memcpy(&data[LIFX_SET_COLOR_PACKET_OFFSET_BRIGHTNESS], &hsbk->brightness, sizeof(unsigned short));
    memcpy(&data[LIFX_SET_COLOR_PACKET_OFFSET_KELVIN],     &hsbk->kelvin,     sizeof(unsigned short));
}

void LIFXController::SetColorPacketSetDuration(unsigned int duration)
{
    memcpy(&data[LIFX_SET_COLOR_PACKET_OFFSET_DURATION], &duration, sizeof(unsigned int));
}

/*-------------------------------------*\
| Set color zones packet helper methods |
\*-------------------------------------*/
void LIFXController::SetColorZonesPacketSetStartIndex(unsigned char start_index)
{
    memcpy(&data[LIFX_SET_COLOR_ZONES_PACKET_OFFSET_START_INDEX], &start_index, sizeof(unsigned char));
}

void LIFXController::SetColorZonesPacketSetEndIndex(unsigned char end_index)
{
    memcpy(&data[LIFX_SET_COLOR_ZONES_PACKET_OFFSET_END_INDEX], &end_index, sizeof(unsigned char));
}

void LIFXController::SetColorZonesPacketSetHSBK(hsbk_t* hsbk)
{
    memcpy(&data[LIFX_SET_COLOR_ZONES_PACKET_OFFSET_HUE],        &hsbk->hue,        sizeof(unsigned short));
    memcpy(&data[LIFX_SET_COLOR_ZONES_PACKET_OFFSET_SATURATION], &hsbk->saturation, sizeof(unsigned short));
    memcpy(&data[LIFX_SET_COLOR_ZONES_PACKET_OFFSET_BRIGHTNESS], &hsbk->brightness, sizeof(unsigned short));
    memcpy(&data[LIFX_SET_COLOR_ZONES_PACKET_OFFSET_KELVIN],     &hsbk->kelvin,     sizeof(unsigned short));
}

void LIFXController::SetColorZonesPacketSetDuration(unsigned int duration)
{
    memcpy(&data[LIFX_SET_COLOR_ZONES_PACKET_OFFSET_DURATION], &duration, sizeof(unsigned int));
}

void LIFXController::SetColorZonesPacketSetApply(unsigned char apply)
{
    memcpy(&data[LIFX_SET_COLOR_ZONES_PACKET_OFFSET_APPLY], &apply, sizeof(unsigned char));
}

/*-------------------------------------*\
| Get color zones packet helper methods |
\*-------------------------------------*/
void LIFXController::GetColorZonesPacketSetStartIndex(unsigned char start_index)
{
    memcpy(&data[LIFX_GET_COLOR_ZONES_PACKET_OFFSET_START_INDEX], &start_index, sizeof(unsigned char));
}

void LIFXController::GetColorZonesPacketSetEndIndex(unsigned char end_index)
{
    memcpy(&data[LIFX_GET_COLOR_ZONES_PACKET_OFFSET_END_INDEX], &end_index, sizeof(unsigned char));
}

/*--------------------------------*\
| State zone packet helper methods |
\*--------------------------------*/
unsigned char LIFXController::StateZonePacketGetZonesCount()
{
    unsigned char zones_count;
    memcpy(&zones_count, &data[LIFX_STATE_ZONE_PACKET_OFFSET_ZONES_COUNT], sizeof(unsigned char));

    return zones_count;
}

/*----------------------------------------------*\
| Set extended color zones packet helper methods |
\*----------------------------------------------*/
void LIFXController::SetExtendedColorZonesPacketSetDuration(unsigned int duration)
{
    memcpy(&data[LIFX_SET_EXTENDED_COLOR_ZONES_PACKET_OFFSET_DURATION], &duration, sizeof(unsigned int));
}

void LIFXController::SetExtendedColorZonesPacketSetApply(unsigned char apply)
{
    memcpy(&data[LIFX_SET_EXTENDED_COLOR_ZONES_PACKET_OFFSET_APPLY], &apply, sizeof(unsigned char));
}

void LIFXController::SetExtendedColorZonesPacketSetZoneIndex(unsigned short zone_index)
{
    memcpy(&data[LIFX_SET_EXTENDED_COLOR_ZONES_PACKET_OFFSET_ZONE_INDEX], &zone_index, sizeof(unsigned short));
}

void LIFXController::SetExtendedColorZonesPacketSetColors(std::vector<RGBColor> colors)
{
    unsigned char colors_count = colors.size();
    memcpy(&data[LIFX_SET_EXTENDED_COLOR_ZONES_PACKET_OFFSET_COLORS_COUNT], &colors_count, sizeof(unsigned char));

    for(size_t i = 0; i < colors.size(); i++)
    {
        hsbk_t hsbk;
        RGBColorToHSBK(colors.at(i), &hsbk);

        size_t current_color_offset = LIFX_SET_EXTENDED_COLOR_ZONES_PACKET_OFFSET_COLORS + (i * HSBK_LENGTH);

        size_t hue_offset =        current_color_offset;
        size_t saturation_offset = hue_offset + sizeof(unsigned short);
        size_t brightness_offset = saturation_offset + sizeof(unsigned short);
        size_t kelvin_offset =     brightness_offset + sizeof(unsigned short);

        memcpy(&data[hue_offset],        &hsbk.hue,        sizeof(unsigned short));
        memcpy(&data[saturation_offset], &hsbk.saturation, sizeof(unsigned short));
        memcpy(&data[brightness_offset], &hsbk.brightness, sizeof(unsigned short));
        memcpy(&data[kelvin_offset],     &hsbk.kelvin,     sizeof(unsigned short));
    }
}
