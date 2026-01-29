/*---------------------------------------------------------*\
| RGBController.cpp                                         |
|                                                           |
|   OpenRGB's RGB controller hardware abstration layer,     |
|   provides a generic representation of an RGB device      |
|                                                           |
|   Adam Honse (CalcProgrammer1)                02 Jun 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include "nlohmann/json.hpp"
#include "RGBController.h"
#include "StringUtils.h"

using namespace std::chrono_literals;

matrix_map_type::matrix_map_type()
{
    Reset();
}

matrix_map_type::matrix_map_type(unsigned int height, unsigned int width, unsigned int * map)
{
    Set(height, width, map);
}

matrix_map_type::~matrix_map_type()
{
}

void matrix_map_type::Reset()
{
    Set(0, 0, NULL);
}

void matrix_map_type::Set(unsigned int height, unsigned int width, unsigned int * map)
{
    this->height        = height;
    this->width         = width;
    this->map.resize(height * width);

    if(map != NULL)
    {
        memcpy(this->map.data(), map, (height * width * sizeof(unsigned int)));
    }
    else
    {
        memset(this->map.data(), 0, (height * width * sizeof(unsigned int)));
    }
}

mode::mode()
{
    /*-----------------------------------------------------*\
    | Initialize mode variables                             |
    \*-----------------------------------------------------*/
    name           = "";
    value          = 0;
    flags          = 0;
    speed_min      = 0;
    speed_max      = 0;
    brightness_min = 0;
    brightness_max = 0;
    colors_min     = 0;
    colors_max     = 0;
    speed          = 0;
    brightness     = 0;
    direction      = 0;
    color_mode     = 0;
}

mode::~mode()
{
    colors.clear();
}

segment::segment()
{
    /*-----------------------------------------------------*\
    | Initialize zone variables                             |
    \*-----------------------------------------------------*/
    name        = "";
    type        = 0;
    start_idx   = 0;
    leds_count  = 0;
}

segment::~segment()
{
}

zone::zone()
{
    /*-----------------------------------------------------*\
    | Initialize zone variables                             |
    \*-----------------------------------------------------*/
    name        = "";
    type        = 0;
    leds        = NULL;
    colors      = NULL;
    start_idx   = 0;
    leds_count  = 0;
    leds_min    = 0;
    leds_max    = 0;
    flags       = 0;
    active_mode = -1;
}

zone::~zone()
{
    segments.clear();
}

RGBController::RGBController()
{
    /*-----------------------------------------------------*\
    | Initialize RGBController variables                    |
    \*-----------------------------------------------------*/
    description         = "";
    location            = "";
    name                = "";
    serial              = "";
    vendor              = "";
    version             = "";
    active_mode         = 0;
    flags               = 0;
    type                = DEVICE_TYPE_UNKNOWN;

    /*-----------------------------------------------------*\
    | Initialize device thread                              |
    \*-----------------------------------------------------*/
    DeviceThreadRunning = true;
    DeviceCallThread    = new std::thread(&RGBController::DeviceCallThreadFunction, this);
}

RGBController::~RGBController()
{
    /*-----------------------------------------------------*\
    | Stop device thread                                    |
    \*-----------------------------------------------------*/
    DeviceThreadRunning = false;
    DeviceCallThread->join();
    delete DeviceCallThread;

    /*-----------------------------------------------------*\
    | Clear member vectors                                  |
    \*-----------------------------------------------------*/
    led_alt_names.clear();
    leds.clear();
    colors.clear();
    zones.clear();
    modes.clear();
}

/*---------------------------------------------------------*\
| Controller Information Functions                          |
\*---------------------------------------------------------*/
std::string RGBController::GetName()
{
    return(name);
}

std::string RGBController::GetVendor()
{
    return(vendor);
}

std::string RGBController::GetDescription()
{
    return(description);
}

std::string RGBController::GetVersion()
{
    return(version);
}

std::string RGBController::GetSerial()
{
    return(serial);
}

std::string RGBController::GetLocation()
{
    return(location);
}

device_type RGBController::GetDeviceType()
{
    return(type);
}

unsigned int RGBController::GetFlags()
{
    return(flags);
}

/*---------------------------------------------------------*\
| Hidden Flag Functions                                     |
\*---------------------------------------------------------*/
bool RGBController::GetHidden()
{
    return(flags & CONTROLLER_FLAG_HIDDEN);
}

void RGBController::SetHidden(bool hidden)
{
    if(hidden)
    {
        flags |= CONTROLLER_FLAG_HIDDEN;
        SignalUpdate(RGBCONTROLLER_UPDATE_REASON_HIDDEN);
    }
    else
    {
        flags &= ~CONTROLLER_FLAG_HIDDEN;
        SignalUpdate(RGBCONTROLLER_UPDATE_REASON_UNHIDDEN);
    }
}

/*---------------------------------------------------------*\
| Zone Functions                                            |
\*---------------------------------------------------------*/
int RGBController::GetZoneActiveMode(unsigned int zone)
{
    int active_mode;

    AccessMutex.lock_shared();
    if((zone < zones.size()) && (zones[zone].active_mode < (int)zones[zone].modes.size()))
    {
        active_mode = zones[zone].active_mode;
    }
    else
    {
        active_mode = -1;
    }
    AccessMutex.unlock_shared();

    return(active_mode);
}

RGBColor RGBController::GetZoneColor(unsigned int zone, unsigned int color_index)
{
    RGBColor color;

    AccessMutex.lock_shared();
    if((zone < zones.size()) && (color_index < zones[zone].leds_count))
    {
        color = zones[zone].colors[color_index];
    }
    else
    {
        color = 0;
    }
    AccessMutex.unlock_shared();

    return(color);
}

RGBColor* RGBController::GetZoneColorsPointer(unsigned int zone)
{
    /*-----------------------------------------------------*\
    | This function assumes the caller has locked the mutex |
    | before calling and will unlock the mutex after        |
    | updating the colors.                                  |
    \*-----------------------------------------------------*/
    if(zone < zones.size())
    {
        return(zones[zone].colors);
    }
    else
    {
        return(NULL);
    }
}

std::size_t RGBController::GetZoneCount()
{
    return(zones.size());
}

unsigned int RGBController::GetZoneFlags(unsigned int zone)
{
    unsigned int flags;

    AccessMutex.lock_shared();
    if(zone < zones.size())
    {
        flags = zones[zone].flags;
    }
    else
    {
        flags = 0;
    }
    AccessMutex.unlock_shared();

    return(flags);
}

unsigned int RGBController::GetZoneLEDsCount(unsigned int zone)
{
    unsigned int leds_count;

    AccessMutex.lock_shared();
    if(zone < zones.size())
    {
        leds_count = zones[zone].leds_count;
    }
    else
    {
        leds_count = 0;
    }
    AccessMutex.unlock_shared();

    return(leds_count);
}

unsigned int RGBController::GetZoneLEDsMax(unsigned int zone)
{
    unsigned int leds_max;

    AccessMutex.lock_shared();
    if(zone < zones.size())
    {
        leds_max = zones[zone].leds_max;
    }
    else
    {
        leds_max = 0;
    }
    AccessMutex.unlock_shared();

    return(leds_max);
}

unsigned int RGBController::GetZoneLEDsMin(unsigned int zone)
{
    unsigned int leds_min;

    AccessMutex.lock_shared();
    if(zone < zones.size())
    {
        leds_min = zones[zone].leds_min;
    }
    else
    {
        leds_min = 0;
    }
    AccessMutex.unlock_shared();

    return(leds_min);
}

unsigned int RGBController::GetZoneMatrixMapHeight(unsigned int zone)
{
    unsigned int height;

    AccessMutex.lock_shared();
    if(zone < zones.size())
    {
        height = zones[zone].matrix_map.height;
    }
    else
    {
        height = 0;
    }
    AccessMutex.unlock_shared();

    return(height);
}

const unsigned int* RGBController::GetZoneMatrixMap(unsigned int zone)
{
    unsigned int* map;

    AccessMutex.lock_shared();
    if(zone < zones.size())
    {
        map = zones[zone].matrix_map.map.data();
    }
    else
    {
        map = 0;
    }
    AccessMutex.unlock_shared();

    return(map);
}

unsigned int RGBController::GetZoneMatrixMapWidth(unsigned int zone)
{
    unsigned int width;

    AccessMutex.lock_shared();
    if(zone < zones.size())
    {
        width = zones[zone].matrix_map.width;
    }
    else
    {
        width = 0;
    }
    AccessMutex.unlock_shared();

    return(width);
}

std::size_t RGBController::GetZoneModeCount(unsigned int zone)
{
    std::size_t mode_count;

    AccessMutex.lock_shared();
    if(zone < zones.size())
    {
        mode_count = zones[zone].modes.size();
    }
    else
    {
        mode_count = 0;
    }
    AccessMutex.unlock_shared();

    return(mode_count);
}

unsigned int RGBController::GetZoneModeBrightness(unsigned int zone, unsigned int mode)
{
    unsigned int brightness;

    AccessMutex.lock_shared();
    if((zone < zones.size()) && (mode < zones[zone].modes.size()))
    {
        brightness = zones[zone].modes[mode].brightness;
    }
    else
    {
        brightness = 0;
    }
    AccessMutex.unlock_shared();

    return(brightness);
}

unsigned int RGBController::GetZoneModeBrightnessMax(unsigned int zone, unsigned int mode)
{
    unsigned int brightness_max;

    AccessMutex.lock_shared();
    if((zone < zones.size()) && (mode < zones[zone].modes.size()))
    {
        brightness_max = zones[zone].modes[mode].brightness_max;
    }
    else
    {
        brightness_max = 0;
    }
    AccessMutex.unlock_shared();

    return(brightness_max);
}

unsigned int RGBController::GetZoneModeBrightnessMin(unsigned int zone, unsigned int mode)
{
    unsigned int brightness_min;

    AccessMutex.lock_shared();
    if((zone < zones.size()) && (mode < zones[zone].modes.size()))
    {
        brightness_min = zones[zone].modes[mode].brightness_min;
    }
    else
    {
        brightness_min = 0;
    }
    AccessMutex.unlock_shared();

    return(brightness_min);
}

RGBColor RGBController::GetZoneModeColor(unsigned int zone, unsigned int mode, unsigned int color_index)
{
    RGBColor color;

    AccessMutex.lock_shared();
    if((zone < zones.size()) && (mode < zones[zone].modes.size()) && (color_index < zones[zone].modes[mode].colors.size()))
    {
        color = zones[zone].modes[mode].colors[color_index];
    }
    else
    {
        color = 0;
    }
    AccessMutex.unlock_shared();

    return(color);
}

unsigned int RGBController::GetZoneModeColorMode(unsigned int zone, unsigned int mode)
{
    unsigned int color_mode;

    AccessMutex.lock_shared();
    if((zone < zones.size()) && (mode < zones[zone].modes.size()))
    {
        color_mode = zones[zone].modes[mode].color_mode;
    }
    else
    {
        color_mode = 0;
    }
    AccessMutex.unlock_shared();

    return(color_mode);
}

std::size_t RGBController::GetZoneModeColorsCount(unsigned int zone, unsigned int mode)
{
    std::size_t colors_count;

    AccessMutex.lock_shared();
    if((zone < zones.size()) && (mode < zones[zone].modes.size()))
    {
        colors_count = zones[zone].modes[mode].colors.size();
    }
    else
    {
        colors_count = 0;
    }
    AccessMutex.unlock_shared();

    return(colors_count);
}

unsigned int RGBController::GetZoneModeColorsMax(unsigned int zone, unsigned int mode)
{
    unsigned int colors_max;

    AccessMutex.lock_shared();
    if((zone < zones.size()) && (mode < zones[zone].modes.size()))
    {
        colors_max = zones[zone].modes[mode].colors_max;
    }
    else
    {
        colors_max = 0;
    }
    AccessMutex.unlock_shared();

    return(colors_max);
}

unsigned int RGBController::GetZoneModeColorsMin(unsigned int zone, unsigned int mode)
{
    unsigned int colors_min;

    AccessMutex.lock_shared();
    if((zone < zones.size()) && (mode < zones[zone].modes.size()))
    {
        colors_min = zones[zone].modes[mode].colors_min;
    }
    else
    {
        colors_min = 0;
    }
    AccessMutex.unlock_shared();

    return(colors_min);
}

unsigned int RGBController::GetZoneModeDirection(unsigned int zone, unsigned int mode)
{
    unsigned int direction;

    AccessMutex.lock_shared();
    if((zone < zones.size()) && (mode < zones[zone].modes.size()))
    {
        direction = zones[zone].modes[mode].direction;
    }
    else
    {
        direction = 0;
    }
    AccessMutex.unlock_shared();

    return(direction);
}

unsigned int RGBController::GetZoneModeFlags(unsigned int zone, unsigned int mode)
{
    unsigned int flags;

    AccessMutex.lock_shared();
    if((zone < zones.size()) && (mode < zones[zone].modes.size()))
    {
        flags = zones[zone].modes[mode].flags;
    }
    else
    {
        flags = 0;
    }
    AccessMutex.unlock_shared();

    return(flags);
}

std::string RGBController::GetZoneModeName(unsigned int zone, unsigned int mode)
{
    std::string name;

    AccessMutex.lock_shared();
    if((zone < zones.size()) && (mode < zones[zone].modes.size()))
    {
        name = zones[zone].modes[mode].name;
    }
    else
    {
        name = "";
    }
    AccessMutex.unlock_shared();

    return(name);
}

unsigned int RGBController::GetZoneModeSpeed(unsigned int zone, unsigned int mode)
{
    unsigned int speed;

    AccessMutex.lock_shared();
    if((zone < zones.size()) && (mode < zones[zone].modes.size()))
    {
        speed = zones[zone].modes[mode].speed;
    }
    else
    {
        speed = 0;
    }
    AccessMutex.unlock_shared();

    return(speed);
}

unsigned int RGBController::GetZoneModeSpeedMax(unsigned int zone, unsigned int mode)
{
    unsigned int speed_max;

    AccessMutex.lock_shared();
    if((zone < zones.size()) && (mode < zones[zone].modes.size()))
    {
        speed_max = zones[zone].modes[mode].speed_max;
    }
    else
    {
        speed_max = 0;
    }
    AccessMutex.unlock_shared();

    return(speed_max);
}

unsigned int RGBController::GetZoneModeSpeedMin(unsigned int zone, unsigned int mode)
{
    unsigned int speed_min;

    AccessMutex.lock_shared();
    if((zone < zones.size()) && (mode < zones[zone].modes.size()))
    {
        speed_min = zones[zone].modes[mode].speed_min;
    }
    else
    {
        speed_min = 0;
    }
    AccessMutex.unlock_shared();

    return(speed_min);
}

int RGBController::GetZoneModeValue(unsigned int zone, unsigned int mode)
{
    unsigned int value;

    AccessMutex.lock_shared();
    if((zone < zones.size()) && (mode < zones[zone].modes.size()))
    {
        value = zones[zone].modes[mode].value;
    }
    else
    {
        value = 0;
    }
    AccessMutex.unlock_shared();

    return(value);
}

std::string RGBController::GetZoneName(unsigned int zone)
{
    std::string name;

    AccessMutex.lock_shared();
    if(zone < zones.size())
    {
        name = zones[zone].name;
    }
    else
    {
        name = "";
    }
    AccessMutex.unlock_shared();

    return(name);
}

std::size_t RGBController::GetZoneSegmentCount(unsigned int zone)
{
    std::size_t count;

    AccessMutex.lock_shared();
    if(zone < zones.size())
    {
        count = zones[zone].segments.size();
    }
    else
    {
        count = 0;
    }
    AccessMutex.unlock_shared();

    return(count);
}

unsigned int RGBController::GetZoneSegmentLEDsCount(unsigned int zone, unsigned int segment)
{
    unsigned int leds_count;

    AccessMutex.lock_shared();
    if((zone < zones.size()) && (segment < zones[zone].segments.size()))
    {
        leds_count = zones[zone].segments[segment].leds_count;
    }
    else
    {
        leds_count = 0;
    }
    AccessMutex.unlock_shared();

    return(leds_count);
}

unsigned int RGBController::GetZoneSegmentMatrixMapHeight(unsigned int zone, unsigned int segment)
{
    unsigned int height;

    if(zone < zones.size())
    {
        if(segment < zones[zone].segments.size())
        {
            height = zones[zone].segments[segment].matrix_map.height;
        }
        else
        {
            height = 0;
        }
    }
    else
    {
        height = 0;
    }

    return(height);
}

const unsigned int * RGBController::GetZoneSegmentMatrixMap(unsigned int zone, unsigned int segment)
{
    unsigned int* map;

    if(zone < zones.size())
    {
        if(segment < zones[zone].segments.size())
        {
            map = zones[zone].segments[segment].matrix_map.map.data();
        }
        else
        {
            map = 0;
        }
    }
    else
    {
        map = 0;
    }

    return(map);
}

unsigned int RGBController::GetZoneSegmentMatrixMapWidth(unsigned int zone, unsigned int segment)
{
    unsigned int width;

    if(zone < zones.size())
    {
        if(segment < zones[zone].segments.size())
        {
            width = zones[zone].segments[segment].matrix_map.width;
        }
        else
        {
            width = 0;
        }
    }
    else
    {
        width = 0;
    }

    return(width);
}

std::string RGBController::GetZoneSegmentName(unsigned int zone, unsigned int segment)
{
    std::string name;

    AccessMutex.lock_shared();
    if((zone < zones.size()) && (segment < zones[zone].segments.size()))
    {
        name = zones[zone].segments[segment].name;
    }
    else
    {
        name = "";
    }
    AccessMutex.unlock_shared();

    return(name);
}

unsigned int RGBController::GetZoneSegmentStartIndex(unsigned int zone, unsigned int segment)
{
    unsigned int start_idx;

    AccessMutex.lock_shared();
    if((zone < zones.size()) && (segment < zones[zone].segments.size()))
    {
        start_idx = zones[zone].segments[segment].start_idx;
    }
    else
    {
        start_idx = 0;
    }
    AccessMutex.unlock_shared();

    return(start_idx);
}

unsigned int RGBController::GetZoneSegmentType(unsigned int zone, unsigned int segment)
{
    zone_type type;

    AccessMutex.lock_shared();
    if((zone < zones.size()) && (segment < zones[zone].segments.size()))
    {
        type = zones[zone].segments[segment].type;
    }
    else
    {
        type = 0;
    }
    AccessMutex.unlock_shared();

    return(type);
}

unsigned int RGBController::GetZoneStartIndex(unsigned int zone)
{
    unsigned int start_idx;

    AccessMutex.lock_shared();
    if(zone < zones.size())
    {
        start_idx = zones[zone].start_idx;
    }
    else
    {
        start_idx = 0;
    }
    AccessMutex.unlock_shared();

    return(start_idx);
}

zone_type RGBController::GetZoneType(unsigned int zone)
{
    zone_type type;

    AccessMutex.lock_shared();
    if(zone < zones.size())
    {
        type = zones[zone].type;
    }
    else
    {
        type = 0;
    }
    AccessMutex.unlock_shared();

    return(type);
}

unsigned int RGBController::GetLEDsInZone(unsigned int zone)
{
    unsigned int leds_count;

    AccessMutex.lock_shared();
    if(zone < zones.size())
    {
        leds_count = LEDsInZone(zone);
    }
    else
    {
        leds_count = 0;
    }
    AccessMutex.unlock_shared();

    return(leds_count);
}

void RGBController::SetZoneActiveMode(unsigned int zone, int mode)
{
    AccessMutex.lock();
    if((zone < zones.size()) && (mode >= -1 ) && (mode < (int)zones[zone].modes.size()))
    {
        zones[zone].active_mode = mode;
        AccessMutex.unlock();

        UpdateZoneMode(zone);
    }
    else
    {
        AccessMutex.unlock();
    }
}

void RGBController::SetZoneColor(unsigned int zone, unsigned int color_index, RGBColor color)
{
    AccessMutex.lock();
    if((zone < zones.size()) && (color_index < zones[zone].leds_count))
    {
        zones[zone].colors[color_index] = color;
    }
    AccessMutex.unlock();
}

void RGBController::SetZoneModeBrightness(unsigned int zone, unsigned int mode, unsigned int brightness)
{
    AccessMutex.lock();
    if((zone < zones.size())
    && (mode < zones[zone].modes.size())
    && (((zones[zone].modes[mode].brightness_max >= zones[zone].modes[mode].brightness_min) && (brightness >= zones[zone].modes[mode].brightness_min) && (brightness <= zones[zone].modes[mode].brightness_max))
     || ((zones[zone].modes[mode].brightness_max <= zones[zone].modes[mode].brightness_min) && (brightness <= zones[zone].modes[mode].brightness_min) && (brightness >= zones[zone].modes[mode].brightness_max))))
    {
        zones[zone].modes[mode].brightness = brightness;
    }
    AccessMutex.unlock();
}

void RGBController::SetZoneModeColor(unsigned int zone, unsigned int mode, unsigned int color_index, RGBColor color)
{
    AccessMutex.lock();
    if((zone < zones.size()) && (mode < zones[zone].modes.size()) && (color_index < zones[zone].modes[mode].colors.size()))
    {
        zones[zone].modes[mode].colors[color_index] = color;
    }
    AccessMutex.unlock();
}

void RGBController::SetZoneModeColorMode(unsigned int zone, unsigned int mode, unsigned int color_mode)
{
    AccessMutex.lock();
    if((zone < zones.size()) && (mode < zones[zone].modes.size()))
    {
        zones[zone].modes[mode].color_mode = color_mode;
    }
    AccessMutex.unlock();
}

void RGBController::SetZoneModeColorsCount(unsigned int zone, unsigned int mode, std::size_t count)
{
    AccessMutex.lock();
    if((zone < zones.size()) && (mode < zones[zone].modes.size()) && (count >= zones[zone].modes[mode].colors_min) && (count <= zones[zone].modes[mode].colors_max))
    {
        zones[zone].modes[mode].colors.resize(count);
    }
    AccessMutex.unlock();
}

void RGBController::SetZoneModeDirection(unsigned int zone, unsigned int mode, unsigned int direction)
{
    AccessMutex.lock();
    if((zone < zones.size())
    && (mode < zones[zone].modes.size())
    && (((zones[zone].modes[mode].flags & MODE_FLAG_HAS_DIRECTION_HV) && ((direction == MODE_DIRECTION_HORIZONTAL) || (direction == MODE_DIRECTION_VERTICAL)))
     || ((zones[zone].modes[mode].flags & MODE_FLAG_HAS_DIRECTION_LR) && ((direction == MODE_DIRECTION_LEFT) || (direction == MODE_DIRECTION_RIGHT)))
     || ((zones[zone].modes[mode].flags & MODE_FLAG_HAS_DIRECTION_UD) && ((direction == MODE_DIRECTION_UP) || (direction == MODE_DIRECTION_DOWN)))))
    {
        zones[zone].modes[mode].direction = direction;
    }
    AccessMutex.unlock();
}

void RGBController::SetZoneModeSpeed(unsigned int zone, unsigned int mode, unsigned int speed)
{
    AccessMutex.lock();
    if((zone < zones.size())
    && (mode < zones[zone].modes.size())
    && (((zones[zone].modes[mode].speed_max >= zones[zone].modes[mode].speed_min) && (speed >= zones[zone].modes[mode].speed_min) && (speed <= zones[zone].modes[mode].speed_max))
     || ((zones[zone].modes[mode].speed_max <= zones[zone].modes[mode].speed_min) && (speed <= zones[zone].modes[mode].speed_min) && (speed >= zones[zone].modes[mode].speed_max))))
    {
        zones[zone].modes[mode].speed = speed;
    }
    AccessMutex.unlock();
}

bool RGBController::SupportsPerZoneModes()
{
    bool supports_per_zone_modes = false;

    AccessMutex.lock_shared();
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        if(zones[zone_idx].modes.size() > 0)
        {
            supports_per_zone_modes = true;
            break;
        }
    }
    AccessMutex.unlock_shared();

    return(supports_per_zone_modes);
}

/*---------------------------------------------------------*\
| Mode Functions                                            |
\*---------------------------------------------------------*/
std::size_t RGBController::GetModeCount()
{
    return(modes.size());
}

unsigned int RGBController::GetModeBrightness(unsigned int mode)
{
    unsigned int brightness;

    AccessMutex.lock_shared();
    if(mode < modes.size())
    {
        brightness = modes[mode].brightness;
    }
    else
    {
        brightness = 0;
    }
    AccessMutex.unlock_shared();

    return(brightness);
}

unsigned int RGBController::GetModeBrightnessMax(unsigned int mode)
{
    unsigned int brightness_max;

    AccessMutex.lock_shared();
    if(mode < modes.size())
    {
        brightness_max = modes[mode].brightness_max;
    }
    else
    {
        brightness_max = 0;
    }
    AccessMutex.unlock_shared();

    return(brightness_max);
}

unsigned int RGBController::GetModeBrightnessMin(unsigned int mode)
{
    unsigned int brightness_min;

    AccessMutex.lock_shared();
    if(mode < modes.size())
    {
        brightness_min = modes[mode].brightness_min;
    }
    else
    {
        brightness_min = 0;
    }
    AccessMutex.unlock_shared();

    return(brightness_min);
}

RGBColor RGBController::GetModeColor(unsigned int mode, unsigned int color_index)
{
    RGBColor color;

    AccessMutex.lock_shared();
    if((mode < modes.size()) && (color_index < modes[mode].colors.size()))
    {
        color = modes[mode].colors[color_index];
    }
    else
    {
        color = 0;
    }
    AccessMutex.unlock_shared();

    return(color);
}

unsigned int RGBController::GetModeColorMode(unsigned int mode)
{
    unsigned int color_mode;

    AccessMutex.lock_shared();
    if(mode < modes.size())
    {
        color_mode = modes[mode].color_mode;
    }
    else
    {
        color_mode = 0;
    }
    AccessMutex.unlock_shared();

    return(color_mode);
}

std::size_t RGBController::GetModeColorsCount(unsigned int mode)
{
    std::size_t count;

    AccessMutex.lock_shared();
    if(mode < modes.size())
    {
        count = modes[mode].colors.size();
    }
    else
    {
        count = 0;
    }
    AccessMutex.unlock_shared();

    return(count);
}

unsigned int RGBController::GetModeColorsMax(unsigned int mode)
{
    unsigned int colors_max;

    AccessMutex.lock_shared();
    if(mode < modes.size())
    {
        colors_max = modes[mode].colors_max;
    }
    else
    {
        colors_max = 0;
    }
    AccessMutex.unlock_shared();

    return(colors_max);
}

unsigned int RGBController::GetModeColorsMin(unsigned int mode)
{
    unsigned int colors_min;

    AccessMutex.lock_shared();
    if(mode < modes.size())
    {
        colors_min = modes[mode].colors_min;
    }
    else
    {
        colors_min = 0;
    }
    AccessMutex.unlock_shared();

    return(colors_min);
}

unsigned int RGBController::GetModeDirection(unsigned int mode)
{
    unsigned int direction;

    AccessMutex.lock_shared();
    if(mode < modes.size())
    {
        direction = modes[mode].direction;
    }
    else
    {
        direction = 0;
    }
    AccessMutex.unlock_shared();

    return(direction);
}

unsigned int RGBController::GetModeFlags(unsigned int mode)
{
    unsigned int flags;

    AccessMutex.lock_shared();
    if(mode < modes.size())
    {
        flags = modes[mode].flags;
    }
    else
    {
        flags = 0;
    }
    AccessMutex.unlock_shared();

    return(flags);
}

std::string RGBController::GetModeName(unsigned int mode)
{
    return(modes[mode].name);
}

unsigned int RGBController::GetModeSpeed(unsigned int mode)
{
    unsigned int speed;

    AccessMutex.lock_shared();
    if(mode < modes.size())
    {
        speed = modes[mode].speed;
    }
    else
    {
        speed = 0;
    }
    AccessMutex.unlock_shared();

    return(speed);
}

unsigned int RGBController::GetModeSpeedMax(unsigned int mode)
{
    unsigned int speed_max;

    AccessMutex.lock_shared();
    if(mode < modes.size())
    {
        speed_max = modes[mode].speed_max;
    }
    else
    {
        speed_max = 0;
    }
    AccessMutex.unlock_shared();

    return(speed_max);
}

unsigned int RGBController::GetModeSpeedMin(unsigned int mode)
{
    unsigned int speed_min;

    AccessMutex.lock_shared();
    if(mode < modes.size())
    {
        speed_min = modes[mode].speed_min;
    }
    else
    {
        speed_min = 0;
    }
    AccessMutex.unlock_shared();

    return(speed_min);
}

int RGBController::GetModeValue(unsigned int mode)
{
    int value;

    AccessMutex.lock_shared();
    if(mode < modes.size())
    {
        value = modes[mode].value;
    }
    else
    {
        value = 0;
    }
    AccessMutex.unlock_shared();

    return(value);
}

void RGBController::SetModeBrightness(unsigned int mode, unsigned int brightness)
{
    AccessMutex.lock();
    if((mode < modes.size())
    && (((modes[mode].brightness_max >= modes[mode].brightness_min) && (brightness >= modes[mode].brightness_min) && (brightness <= modes[mode].brightness_max))
     || ((modes[mode].brightness_max <= modes[mode].brightness_min) && (brightness <= modes[mode].brightness_min) && (brightness >= modes[mode].brightness_max))))
    {
        modes[mode].brightness = brightness;
    }
    AccessMutex.unlock();
}

void RGBController::SetModeColor(unsigned int mode, unsigned int color_index, RGBColor color)
{
    AccessMutex.lock();
    if((mode < modes.size()) && (color_index < modes[mode].colors.size()))
    {
        modes[mode].colors[color_index] = color;
    }
    AccessMutex.unlock();
}

void RGBController::SetModeColorMode(unsigned int mode, unsigned int color_mode)
{
    AccessMutex.lock();
    if(mode < modes.size())
    {
        modes[mode].color_mode = color_mode;
    }
    AccessMutex.unlock();
}

void RGBController::SetModeColorsCount(unsigned int mode, std::size_t count)
{
    AccessMutex.lock();
    if((mode < modes.size()) && (count >= modes[mode].colors_min) && (count <= modes[mode].colors_max))
    {
        modes[mode].colors.resize(count);
    }
    AccessMutex.unlock();
}

void RGBController::SetModeDirection(unsigned int mode, unsigned int direction)
{
    AccessMutex.lock();
    if((mode < modes.size())
    && (((modes[mode].flags & MODE_FLAG_HAS_DIRECTION_HV) && ((direction == MODE_DIRECTION_HORIZONTAL) || (direction == MODE_DIRECTION_VERTICAL)))
     || ((modes[mode].flags & MODE_FLAG_HAS_DIRECTION_LR) && ((direction == MODE_DIRECTION_LEFT) || (direction == MODE_DIRECTION_RIGHT)))
     || ((modes[mode].flags & MODE_FLAG_HAS_DIRECTION_UD) && ((direction == MODE_DIRECTION_UP) || (direction == MODE_DIRECTION_DOWN)))))
    {
        modes[mode].direction = direction;
    }
    AccessMutex.unlock();
}

void RGBController::SetModeSpeed(unsigned int mode, unsigned int speed)
{
    AccessMutex.lock();
    if((mode < modes.size())
    && (((modes[mode].speed_max >= modes[mode].speed_min) && (speed >= modes[mode].speed_min) && (speed <= modes[mode].speed_max))
     || ((modes[mode].speed_max <= modes[mode].speed_min) && (speed <= modes[mode].speed_min) && (speed >= modes[mode].speed_max))))
    {
        modes[mode].speed = speed;
    }
    AccessMutex.unlock();
}

int RGBController::GetActiveMode()
{
    int active_mode_val;

    AccessMutex.lock_shared();
    if(active_mode < (int)modes.size())
    {
        active_mode_val = active_mode;
    }
    else
    {
        active_mode_val = 0;
    }
    AccessMutex.unlock_shared();

    return(active_mode_val);
}

void RGBController::SetActiveMode(int mode)
{
    /*-----------------------------------------------------*\
    | Ensure new active mode is within range                |
    \*-----------------------------------------------------*/
    AccessMutex.lock_shared();
    if((mode < 0) || (mode >= (int)modes.size()))
    {
        AccessMutex.unlock_shared();
        return;
    }
    AccessMutex.unlock_shared();

    /*-----------------------------------------------------*\
    | If the newly set active mode requires entire device,  |
    | set all the per-zone active modes to the entire       |
    | device mode.                                          |
    \*-----------------------------------------------------*/
    if(modes[mode].flags & MODE_FLAG_REQUIRES_ENTIRE_DEVICE)
    {
        for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
        {
            SetZoneActiveMode(zone_idx, -1);
        }
    }

    AccessMutex.lock();
    active_mode = mode;

    AccessMutex.unlock();

    UpdateMode();
}

void RGBController::SetCustomMode()
{
    /*-----------------------------------------------------*\
    | Search the Controller's mode list for a suitable      |
    | per-LED custom mode in the following order:           |
    | 1.    Direct                                          |
    | 2.    Custom                                          |
    | 3.    Static                                          |
    \*-----------------------------------------------------*/
    #define NUM_CUSTOM_MODE_NAMES 3

    const std::string custom_mode_names[] =
    {
        "Direct",
        "Custom",
        "Static"
    };

    for(unsigned int custom_mode_idx = 0; custom_mode_idx < NUM_CUSTOM_MODE_NAMES; custom_mode_idx++)
    {
        for(unsigned int mode_idx = 0; mode_idx < modes.size(); mode_idx++)
        {
            if((modes[mode_idx].name == custom_mode_names[custom_mode_idx])
            && ((modes[mode_idx].color_mode == MODE_COLORS_PER_LED)
             || (modes[mode_idx].color_mode == MODE_COLORS_MODE_SPECIFIC)))
            {
                active_mode = mode_idx;
                return;
            }
        }
    }

    UpdateMode();
}

/*---------------------------------------------------------*\
| LED Functions                                             |
\*---------------------------------------------------------*/
std::size_t RGBController::GetLEDCount()
{
    return(leds.size());
}

std::string RGBController::GetLEDName(unsigned int led)
{
    std::string name;

    AccessMutex.lock_shared();
    if(led < leds.size())
    {
        name = leds[led].name;
    }
    else
    {
        name = "";
    }
    AccessMutex.unlock_shared();

    return(name);
}

unsigned int RGBController::GetLEDValue(unsigned int led)
{
    unsigned int value;

    AccessMutex.lock_shared();
    if(led < leds.size())
    {
        value = leds[led].value;
    }
    else
    {
        value = 0;
    }
    AccessMutex.unlock_shared();

    return(value);
}

std::string RGBController::GetLEDDisplayName(unsigned int led)
{
    std::string name;

    AccessMutex.lock_shared();
    if((led < led_alt_names.size()) && (led_alt_names[led] != ""))
    {
        name = led_alt_names[led];
    }
    else if(led < leds.size())
    {
        name = leds[led].name;
    }
    else
    {
        name = "";
    }
    AccessMutex.unlock_shared();

    return(name);
}

/*---------------------------------------------------------*\
| Color Functions                                           |
\*---------------------------------------------------------*/
RGBColor RGBController::GetColor(unsigned int led)
{
    RGBColor color;

    AccessMutex.lock_shared();
    if(led < colors.size())
    {
        color = colors[led];
    }
    else
    {
        color = 0;
    }
    AccessMutex.unlock_shared();

    return(color);
}

RGBColor* RGBController::GetColorsPointer()
{
    return(&colors[0]);
}

void RGBController::SetColor(unsigned int led, RGBColor color)
{
    AccessMutex.lock();
    if(led < colors.size())
    {
        colors[led] = color;
    }
    AccessMutex.unlock();
}

void RGBController::SetAllColors(RGBColor color)
{
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        SetAllZoneColors((int)zone_idx, color);
    }
}

void RGBController::SetAllZoneColors(int zone, RGBColor color)
{
    AccessMutex.lock();
    for(std::size_t color_idx = 0; color_idx < LEDsInZone(zone); color_idx++)
    {
        zones[zone].colors[color_idx] = color;
    }
    AccessMutex.unlock();
}

/*---------------------------------------------------------*\
| Serialized Description Functions                          |
\*---------------------------------------------------------*/
unsigned char * RGBController::GetColorDescriptionData(unsigned char* data_ptr, unsigned int /*protocol_version*/)
{
    /*-----------------------------------------------------*\
    | Initialize variables                                  |
    \*-----------------------------------------------------*/
    unsigned short num_colors               = (unsigned short)colors.size();

    /*-----------------------------------------------------*\
    | Copy in number of colors                              |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &num_colors, sizeof(num_colors));
    data_ptr += sizeof(num_colors);

    /*-----------------------------------------------------*\
    | Copy in colors                                        |
    \*-----------------------------------------------------*/
    for(int color_index = 0; color_index < num_colors; color_index++)
    {
        memcpy(data_ptr, &colors[color_index], sizeof(colors[color_index]));
        data_ptr += sizeof(colors[color_index]);
    }

    return(data_ptr);
}

unsigned int RGBController::GetColorDescriptionSize(unsigned int /*protocol_version*/)
{
    /*-----------------------------------------------------*\
    | Initialize variables                                  |
    \*-----------------------------------------------------*/
    unsigned int data_size                  = 0;
    unsigned short num_colors               = (unsigned short)colors.size();

    /*-----------------------------------------------------*\
    | Calculate data size                                   |
    \*-----------------------------------------------------*/
    data_size                              += sizeof(num_colors);
    data_size                              += num_colors * sizeof(RGBColor);

    return(data_size);
}

unsigned char * RGBController::GetDeviceDescriptionData(unsigned char* data_ptr, unsigned int protocol_version)
{
    /*-----------------------------------------------------*\
    | Calculate data size                                   |
    \*-----------------------------------------------------*/
    unsigned short name_len                 = (unsigned short)strlen(name.c_str())        + 1;
    unsigned short vendor_len               = (unsigned short)strlen(vendor.c_str())      + 1;
    unsigned short description_len          = (unsigned short)strlen(description.c_str()) + 1;
    unsigned short version_len              = (unsigned short)strlen(version.c_str())     + 1;
    unsigned short serial_len               = (unsigned short)strlen(serial.c_str())      + 1;
    unsigned short location_len             = (unsigned short)strlen(location.c_str())    + 1;
    unsigned short num_modes                = (unsigned short)modes.size();
    unsigned short num_zones                = (unsigned short)zones.size();
    unsigned short num_leds                 = (unsigned short)leds.size();
    unsigned short num_led_alt_names        = (unsigned short)led_alt_names.size();

    /*-----------------------------------------------------*\
    | Copy in type                                          |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &type, sizeof(device_type));
    data_ptr += sizeof(device_type);

    /*-----------------------------------------------------*\
    | Copy in name (size+data)                              |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &name_len, sizeof(name_len));
    data_ptr += sizeof(name_len);

    strcpy((char *)data_ptr, name.c_str());
    data_ptr += name_len;

    /*-----------------------------------------------------*\
    | Copy in vendor (size+data) if protocol 1 or higher    |
    \*-----------------------------------------------------*/
    if(protocol_version >= 1)
    {
        memcpy(data_ptr, &vendor_len, sizeof(vendor_len));
        data_ptr += sizeof(vendor_len);

        strcpy((char *)data_ptr, vendor.c_str());
        data_ptr += vendor_len;
    }

    /*-----------------------------------------------------*\
    | Copy in description (size+data)                       |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &description_len, sizeof(description_len));
    data_ptr += sizeof(description_len);

    strcpy((char *)data_ptr, description.c_str());
    data_ptr += description_len;

    /*-----------------------------------------------------*\
    | Copy in version (size+data)                           |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &version_len, sizeof(version_len));
    data_ptr += sizeof(version_len);

    strcpy((char *)data_ptr, version.c_str());
    data_ptr += version_len;

    /*-----------------------------------------------------*\
    | Copy in serial (size+data)                            |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &serial_len, sizeof(serial_len));
    data_ptr += sizeof(serial_len);

    strcpy((char *)data_ptr, serial.c_str());
    data_ptr += serial_len;

    /*-----------------------------------------------------*\
    | Copy in location (size+data)                          |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &location_len, sizeof(location_len));
    data_ptr += sizeof(location_len);

    strcpy((char *)data_ptr, location.c_str());
    data_ptr += location_len;

    /*-----------------------------------------------------*\
    | Copy in number of modes                               |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &num_modes, sizeof(num_modes));
    data_ptr += sizeof(num_modes);

    /*-----------------------------------------------------*\
    | Copy in active mode                                   |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &active_mode, sizeof(active_mode));
    data_ptr += sizeof(active_mode);

    /*-----------------------------------------------------*\
    | Copy in modes                                         |
    \*-----------------------------------------------------*/
    for(int mode_index = 0; mode_index < num_modes; mode_index++)
    {
        data_ptr = GetModeDescriptionData(data_ptr, modes[mode_index], protocol_version);
    }

    /*-----------------------------------------------------*\
    | Copy in number of zones                               |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &num_zones, sizeof(num_zones));
    data_ptr += sizeof(num_zones);

    /*-----------------------------------------------------*\
    | Copy in zones                                         |
    \*-----------------------------------------------------*/
    for(int zone_index = 0; zone_index < num_zones; zone_index++)
    {
        data_ptr = GetZoneDescriptionData(data_ptr, zones[zone_index], protocol_version);
    }

    /*-----------------------------------------------------*\
    | Copy in number of LEDs                                |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &num_leds, sizeof(num_leds));
    data_ptr += sizeof(num_leds);

    /*-----------------------------------------------------*\
    | Copy in LEDs                                          |
    \*-----------------------------------------------------*/
    for(int led_index = 0; led_index < num_leds; led_index++)
    {
        data_ptr = GetLEDDescriptionData(data_ptr, leds[led_index], protocol_version);
    }

    /*-----------------------------------------------------*\
    | Copy in colors                                        |
    \*-----------------------------------------------------*/
    data_ptr = GetColorDescriptionData(data_ptr, protocol_version);

    /*-----------------------------------------------------*\
    | Copy in LED alternate names                           |
    \*-----------------------------------------------------*/
    if(protocol_version >= 5)
    {
        /*-------------------------------------------------*\
        | Number of LED alternate name strings              |
        \*-------------------------------------------------*/
        memcpy(data_ptr, &num_led_alt_names, sizeof(num_led_alt_names));
        data_ptr += sizeof(num_led_alt_names);

        for(std::size_t led_idx = 0; led_idx < led_alt_names.size(); led_idx++)
        {
            /*---------------------------------------------*\
            | Copy in LED alternate name (size+data)        |
            \*---------------------------------------------*/
            unsigned short string_length    = (unsigned short)strlen(led_alt_names[led_idx].c_str()) + 1;

            memcpy(data_ptr, &string_length, sizeof(string_length));
            data_ptr += sizeof(string_length);

            strcpy((char *)data_ptr, led_alt_names[led_idx].c_str());
            data_ptr += string_length;
        }
    }

    /*-----------------------------------------------------*\
    | Copy in controller flags                              |
    \*-----------------------------------------------------*/
    if(protocol_version >= 5)
    {
        memcpy(data_ptr, &flags, sizeof(flags));
        data_ptr += sizeof(flags);
    }

    return(data_ptr);
}

unsigned int RGBController::GetDeviceDescriptionSize(unsigned int protocol_version)
{
    /*-----------------------------------------------------*\
    | Initialize variables                                  |
    \*-----------------------------------------------------*/
    unsigned int data_size                  = 0;

    /*-----------------------------------------------------*\
    | Calculate data size                                   |
    \*-----------------------------------------------------*/
    unsigned short name_len                 = (unsigned short)strlen(name.c_str())        + 1;
    unsigned short vendor_len               = (unsigned short)strlen(vendor.c_str())      + 1;
    unsigned short description_len          = (unsigned short)strlen(description.c_str()) + 1;
    unsigned short version_len              = (unsigned short)strlen(version.c_str())     + 1;
    unsigned short serial_len               = (unsigned short)strlen(serial.c_str())      + 1;
    unsigned short location_len             = (unsigned short)strlen(location.c_str())    + 1;
    unsigned short num_modes                = (unsigned short)modes.size();
    unsigned short num_zones                = (unsigned short)zones.size();
    unsigned short num_leds                 = (unsigned short)leds.size();
    unsigned short num_led_alt_names        = (unsigned short)led_alt_names.size();

    data_size                              += sizeof(device_type);
    data_size                              += sizeof(name_len);
    data_size                              += name_len;

    if(protocol_version >= 1)
    {
        data_size                          += sizeof(vendor_len);
        data_size                          += vendor_len;
    }

    data_size                              += sizeof(description_len);
    data_size                              += description_len;
    data_size                              += sizeof(version_len);
    data_size                              += version_len;
    data_size                              += sizeof(serial_len);
    data_size                              += serial_len;
    data_size                              += sizeof(location_len);
    data_size                              += location_len;

    data_size                              += sizeof(num_modes);
    data_size                              += sizeof(active_mode);

    for(int mode_index = 0; mode_index < num_modes; mode_index++)
    {
        data_size                          += GetModeDescriptionSize(modes[mode_index], protocol_version);
    }

    data_size                              += sizeof(num_zones);

    for(int zone_index = 0; zone_index < num_zones; zone_index++)
    {
        data_size                          += GetZoneDescriptionSize(zones[zone_index], protocol_version);
    }

    data_size                              += sizeof(num_leds);

    for(int led_index = 0; led_index < num_leds; led_index++)
    {
        data_size                          += GetLEDDescriptionSize(leds[led_index], protocol_version);
    }

    data_size                              += GetColorDescriptionSize(protocol_version);

    if(protocol_version >= 5)
    {
        data_size                          += sizeof(num_led_alt_names);

        for(std::size_t led_idx = 0; led_idx < led_alt_names.size(); led_idx++)
        {
            data_size                      += sizeof(unsigned short);
            data_size                      += (unsigned int)strlen(led_alt_names[led_idx].c_str()) + 1;
        }
    }

    if(protocol_version >= 5)
    {
        data_size                          += sizeof(flags);
    }

    return(data_size);
}

unsigned char * RGBController::GetLEDDescriptionData(unsigned char* data_ptr, led led, unsigned int /*protocol_version*/)
{
    /*-----------------------------------------------------*\
    | Copy in LED name (size+data)                          |
    \*-----------------------------------------------------*/
    unsigned short led_name_len             = (unsigned short)strlen(led.name.c_str()) + 1;

    memcpy(data_ptr, &led_name_len, sizeof(led_name_len));
    data_ptr += sizeof(led_name_len);

    strcpy((char *)data_ptr, led.name.c_str());
    data_ptr += led_name_len;

    /*-----------------------------------------------------*\
    | Copy in LED value                                     |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &led.value, sizeof(led.value));
    data_ptr += sizeof(led.value);

    return(data_ptr);
}

unsigned int RGBController::GetLEDDescriptionSize(led led, unsigned int /*protocol_version*/)
{
    /*-----------------------------------------------------*\
    | Initialize variables                                  |
    \*-----------------------------------------------------*/
    unsigned int data_size                  = 0;

    /*-----------------------------------------------------*\
    | Calculate description data size for the given LED     |
    \*-----------------------------------------------------*/
    unsigned short led_name_len             = (unsigned short)strlen(led.name.c_str()) + 1;

    data_size                              += sizeof(led_name_len);
    data_size                              += led_name_len;
    data_size                              += sizeof(led.value);

    return(data_size);
}

unsigned char * RGBController::GetMatrixMapDescriptionData(unsigned char* data_ptr, matrix_map_type matrix_map, unsigned int /*protocol_version*/)
{
    /*-------------------------------------------------*\
    | Copy in matrix height                             |
    \*-------------------------------------------------*/
    memcpy(data_ptr, &matrix_map.height, sizeof(matrix_map.height));
    data_ptr += sizeof(matrix_map.height);

    /*-------------------------------------------------*\
    | Copy in matrix width                              |
    \*-------------------------------------------------*/
    memcpy(data_ptr, &matrix_map.width, sizeof(matrix_map.width));
    data_ptr += sizeof(matrix_map.width);

    /*-------------------------------------------------*\
    | Copy in matrix map                                |
    \*-------------------------------------------------*/
    for(unsigned int matrix_idx = 0; matrix_idx < (matrix_map.height * matrix_map.width); matrix_idx++)
    {
        memcpy(data_ptr, &matrix_map.map[matrix_idx], sizeof(matrix_map.map[matrix_idx]));
        data_ptr += sizeof(matrix_map.map[matrix_idx]);
    }

    return(data_ptr);
}

unsigned int RGBController::GetMatrixMapDescriptionSize(matrix_map_type matrix_map, unsigned int /*protocol_version*/)
{
    /*-----------------------------------------------------*\
    | Initialize variables                                  |
    \*-----------------------------------------------------*/
    unsigned int data_size                  = 0;

    /*-----------------------------------------------------*\
    | Calculate description data size for the given matrix  |
    | map                                                   |
    \*-----------------------------------------------------*/
    data_size                              += sizeof(matrix_map.height);
    data_size                              += sizeof(matrix_map.width);
    data_size                              += (matrix_map.height * matrix_map.width) * sizeof(unsigned int);

    return(data_size);
}

unsigned int RGBController::GetModeDescriptionSize(mode mode, unsigned int protocol_version)
{
    /*-----------------------------------------------------*\
    | Initialize variables                                  |
    \*-----------------------------------------------------*/
    unsigned int    mode_description_size   = 0;
    unsigned short  mode_name_len           = 0;
    unsigned short  mode_num_colors         = 0;

    /*-----------------------------------------------------*\
    | Calculate description data size for the given mode    |
    \*-----------------------------------------------------*/
    mode_name_len                           = (unsigned short)strlen(mode.name.c_str()) + 1;
    mode_num_colors                         = (unsigned short)mode.colors.size();

    mode_description_size                  += sizeof(mode_name_len);
    mode_description_size                  += mode_name_len;
    mode_description_size                  += sizeof(mode.value);
    mode_description_size                  += sizeof(mode.flags);
    mode_description_size                  += sizeof(mode.speed_min);
    mode_description_size                  += sizeof(mode.speed_max);
    if(protocol_version >= 3)
    {
        mode_description_size              += sizeof(mode.brightness_min);
        mode_description_size              += sizeof(mode.brightness_max);
    }
    mode_description_size                  += sizeof(mode.colors_min);
    mode_description_size                  += sizeof(mode.colors_max);
    mode_description_size                  += sizeof(mode.speed);
    if(protocol_version >= 3)
    {
        mode_description_size              += sizeof(mode.brightness);
    }
    mode_description_size                  += sizeof(mode.direction);
    mode_description_size                  += sizeof(mode.color_mode);
    mode_description_size                  += sizeof(mode_num_colors);
    mode_description_size                  += (mode_num_colors * sizeof(RGBColor));

    return(mode_description_size);
}

unsigned char * RGBController::GetModeDescriptionData(unsigned char* data_ptr, mode mode, unsigned int protocol_version)
{
    /*-----------------------------------------------------*\
    | Initialize variables                                  |
    \*-----------------------------------------------------*/
    unsigned short  mode_name_len           = 0;
    unsigned short  mode_num_colors         = 0;

    /*-----------------------------------------------------*\
    | Calculate mode sizes                                  |
    \*-----------------------------------------------------*/
    mode_name_len                           = (unsigned short)strlen(mode.name.c_str()) + 1;
    mode_num_colors                         = (unsigned short)mode.colors.size();

    /*-----------------------------------------------------*\
    | Copy in mode name (size+data)                         |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &mode_name_len, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    strcpy((char *)data_ptr, mode.name.c_str());
    data_ptr += mode_name_len;

    /*-----------------------------------------------------*\
    | Copy in mode value (data)                             |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &mode.value, sizeof(mode.value));
    data_ptr += sizeof(mode.value);

    /*-----------------------------------------------------*\
    | Copy in mode flags (data)                             |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &mode.flags, sizeof(mode.flags));
    data_ptr += sizeof(mode.flags);

    /*-----------------------------------------------------*\
    | Copy in mode speed_min (data)                         |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &mode.speed_min, sizeof(mode.speed_min));
    data_ptr += sizeof(mode.speed_min);

    /*-----------------------------------------------------*\
    | Copy in mode speed_max (data)                         |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &mode.speed_max, sizeof(mode.speed_max));
    data_ptr += sizeof(mode.speed_max);

    /*-----------------------------------------------------*\
    | Copy in mode brightness min and max if protocol       |
    | version is 3 or higher                                |
    \*-----------------------------------------------------*/
    if(protocol_version >= 3)
    {
        memcpy(data_ptr, &mode.brightness_min, sizeof(mode.brightness_min));
        data_ptr += sizeof(mode.brightness_min);

        memcpy(data_ptr, &mode.brightness_max, sizeof(mode.brightness_max));
        data_ptr += sizeof(mode.brightness_max);
    }

    /*-----------------------------------------------------*\
    | Copy in mode colors_min (data)                        |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &mode.colors_min, sizeof(mode.colors_min));
    data_ptr += sizeof(mode.colors_min);

    /*-----------------------------------------------------*\
    | Copy in mode colors_max (data)                        |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &mode.colors_max, sizeof(mode.colors_max));
    data_ptr += sizeof(mode.colors_max);

    /*-----------------------------------------------------*\
    | Copy in mode speed (data)                             |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &mode.speed, sizeof(mode.speed));
    data_ptr += sizeof(mode.speed);

    /*-----------------------------------------------------*\
    | Copy in mode brightness if protocol version is 3 or   |
    | higher                                                |
    \*-----------------------------------------------------*/
    if(protocol_version >= 3)
    {
        memcpy(data_ptr, &mode.brightness, sizeof(mode.brightness));
        data_ptr += sizeof(mode.brightness);
    }

    /*-----------------------------------------------------*\
    | Copy in mode direction (data)                         |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &mode.direction, sizeof(mode.direction));
    data_ptr += sizeof(mode.direction);

    /*-----------------------------------------------------*\
    | Copy in mode color_mode (data)                        |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &mode.color_mode, sizeof(mode.color_mode));
    data_ptr += sizeof(mode.color_mode);

    /*-----------------------------------------------------*\
    | Copy in mode number of colors                         |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &mode_num_colors, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*-----------------------------------------------------*\
    | Copy in mode mode colors                              |
    \*-----------------------------------------------------*/
    for(int color_index = 0; color_index < mode_num_colors; color_index++)
    {
        /*-------------------------------------------------*\
        | Copy in color (data)                              |
        \*-------------------------------------------------*/
        memcpy(data_ptr, &mode.colors[color_index], sizeof(mode.colors[color_index]));
        data_ptr += sizeof(mode.colors[color_index]);
    }

    return(data_ptr);
}

unsigned char * RGBController::GetSegmentDescriptionData(unsigned char* data_ptr, segment segment, unsigned int protocol_version)
{
    /*-----------------------------------------------------*\
    | Length of segment name string                         |
    \*-----------------------------------------------------*/
    unsigned short segment_name_length          = (unsigned short)strlen(segment.name.c_str()) + 1;

    memcpy(data_ptr, &segment_name_length, sizeof(segment_name_length));
    data_ptr += sizeof(segment_name_length);

    /*-----------------------------------------------------*\
    | Segment name string data                              |
    \*-----------------------------------------------------*/
    strcpy((char *)data_ptr, segment.name.c_str());
    data_ptr += segment_name_length;

    /*-----------------------------------------------------*\
    | Segment type data                                     |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &segment.type, sizeof(segment.type));
    data_ptr += sizeof(segment.type);

    /*-----------------------------------------------------*\
    | Segment start index data                              |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &segment.start_idx, sizeof(segment.start_idx));
    data_ptr += sizeof(segment.start_idx);

    /*-----------------------------------------------------*\
    | Segment LED count data                                |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &segment.leds_count, sizeof(segment.leds_count));
    data_ptr += sizeof(segment.leds_count);

    /*-----------------------------------------------------*\
    | Segment matrix map data                               |
    \*-----------------------------------------------------*/
    if(protocol_version >= 6)
    {
        unsigned short matrix_map_size      = GetMatrixMapDescriptionSize(segment.matrix_map, protocol_version);

        memcpy(data_ptr, &matrix_map_size, sizeof(matrix_map_size));
        data_ptr += sizeof(matrix_map_size);

        data_ptr                            = GetMatrixMapDescriptionData(data_ptr, segment.matrix_map, protocol_version);
    }

    return(data_ptr);
}

unsigned int RGBController::GetSegmentDescriptionSize(segment segment, unsigned int protocol_version)
{
    /*-----------------------------------------------------*\
    | Initialize variables                                  |
    \*-----------------------------------------------------*/
    unsigned int data_size                  = 0;

    /*-----------------------------------------------------*\
    | Calculate description data size for the given segment |
    \*-----------------------------------------------------*/
    data_size                              += sizeof(unsigned short);
    data_size                              += (unsigned int)strlen(segment.name.c_str()) + 1;

    data_size                              += sizeof(segment.type);
    data_size                              += sizeof(segment.start_idx);
    data_size                              += sizeof(segment.leds_count);

    /*-----------------------------------------------------*\
    | Matrix map size                                       |
    \*-----------------------------------------------------*/
    if(protocol_version >= 6)
    {
        data_size                          += sizeof(unsigned short);
        data_size                          += GetMatrixMapDescriptionSize(segment.matrix_map, protocol_version);
    }

    return(data_size);
}

unsigned char * RGBController::GetZoneDescriptionData(unsigned char* data_ptr, zone zone, unsigned int protocol_version)
{
    /*-----------------------------------------------------*\
    | Initialize variables                                  |
    \*-----------------------------------------------------*/
    unsigned short  zone_name_len           = (unsigned short)strlen(zone.name.c_str()) + 1;

    /*-----------------------------------------------------*\
    | Copy in zone name (size+data)                         |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &zone_name_len, sizeof(zone_name_len));
    data_ptr += sizeof(unsigned short);

    strcpy((char *)data_ptr, zone.name.c_str());
    data_ptr += zone_name_len;

    /*-----------------------------------------------------*\
    | Copy in zone type                                     |
    \*-----------------------------------------------------*/
    memcpy(data_ptr, &zone.type, sizeof(zone.type));
    data_ptr += sizeof(zone.type);

    /*-----------------------------------------------------*\
    | Check for resizable effects-only zone.  For protocol  |
    | versions that do not support this feature, we have to |
    | overwrite the leds_min/max/count parameters to 1 so   |
    | that the zone appears a fixed size to older clients.  |
    \*-----------------------------------------------------*/
    if((zone.flags & ZONE_FLAG_RESIZE_EFFECTS_ONLY) && (protocol_version < 5))
    {
        /*-------------------------------------------------*\
        | Create a temporary variable to hold the fixed     |
        | value of 1                                        |
        \*-------------------------------------------------*/
        unsigned int tmp_size = 1;

        /*-------------------------------------------------*\
        | Copy in temporary minimum LED count               |
        \*-------------------------------------------------*/
        memcpy(data_ptr, &tmp_size, sizeof(tmp_size));
        data_ptr += sizeof(tmp_size);

        /*-------------------------------------------------*\
        | Copy in temporary maximum LED count               |
        \*-------------------------------------------------*/
        memcpy(data_ptr, &tmp_size, sizeof(tmp_size));
        data_ptr += sizeof(tmp_size);

        /*-------------------------------------------------*\
        | Copy in temporary LED count                       |
        \*-------------------------------------------------*/
        memcpy(data_ptr, &tmp_size, sizeof(tmp_size));
        data_ptr += sizeof(tmp_size);
    }
    else
    {
        /*-------------------------------------------------*\
        | Copy in zone minimum LED count                    |
        \*-------------------------------------------------*/
        memcpy(data_ptr, &zone.leds_min, sizeof(zone.leds_min));
        data_ptr += sizeof(zone.leds_min);

        /*-------------------------------------------------*\
        | Copy in zone maximum LED count                    |
        \*-------------------------------------------------*/
        memcpy(data_ptr, &zone.leds_max, sizeof(zone.leds_max));
        data_ptr += sizeof(zone.leds_max);

        /*-------------------------------------------------*\
        | Copy in zone LED count                            |
        \*-------------------------------------------------*/
        memcpy(data_ptr, &zone.leds_count, sizeof(zone.leds_count));
        data_ptr += sizeof(zone.leds_count);
    }

    /*-----------------------------------------------------*\
    | Copy in matrix map                                    |
    \*-----------------------------------------------------*/
    unsigned short matrix_map_size          = GetMatrixMapDescriptionSize(zone.matrix_map, protocol_version);

    memcpy(data_ptr, &matrix_map_size, sizeof(matrix_map_size));
    data_ptr += sizeof(matrix_map_size);

    data_ptr                                = GetMatrixMapDescriptionData(data_ptr, zone.matrix_map, protocol_version);

    /*-----------------------------------------------------*\
    | Copy in segments                                      |
    \*-----------------------------------------------------*/
    if(protocol_version >= 4)
    {
        /*-------------------------------------------------*\
        | Number of segments in zone                        |
        \*-------------------------------------------------*/
        unsigned short num_segments         = (unsigned short)zone.segments.size();

        memcpy(data_ptr, &num_segments, sizeof(num_segments));
        data_ptr += sizeof(num_segments);

        for(int segment_index = 0; segment_index < num_segments; segment_index++)
        {
            data_ptr = GetSegmentDescriptionData(data_ptr, zone.segments[segment_index], protocol_version);
        }
    }

    /*-----------------------------------------------------*\
    | Copy in zone flags                                    |
    \*-----------------------------------------------------*/
    if(protocol_version >= 5)
    {
        memcpy(data_ptr, &zone.flags, sizeof(zone.flags));
        data_ptr += sizeof(zone.flags);
    }

    /*-----------------------------------------------------*\
    | Copy in zone modes                                    |
    \*-----------------------------------------------------*/
    if(protocol_version >= 6)
    {
        /*-------------------------------------------------*\
        | Copy in number of modes                           |
        \*-------------------------------------------------*/
        unsigned short num_modes            = (unsigned short)zone.modes.size();

        memcpy(data_ptr, &num_modes, sizeof(num_modes));
        data_ptr += sizeof(num_modes);

        /*-------------------------------------------------*\
        | Copy in active mode                               |
        \*-------------------------------------------------*/
        memcpy(data_ptr, &zone.active_mode, sizeof(zone.active_mode));
        data_ptr += sizeof(zone.active_mode);

        /*-------------------------------------------------*\
        | Copy in modes                                     |
        \*-------------------------------------------------*/
        for(int mode_index = 0; mode_index < num_modes; mode_index++)
        {
            data_ptr = GetModeDescriptionData(data_ptr, zone.modes[mode_index], protocol_version);
        }
    }

    return(data_ptr);
}

unsigned int RGBController::GetZoneDescriptionSize(zone zone, unsigned int protocol_version)
{
    /*-----------------------------------------------------*\
    | Initialize variables                                  |
    \*-----------------------------------------------------*/
    unsigned int data_size                  = 0;

    /*-----------------------------------------------------*\
    | Calculate description data size for the given segment |
    \*-----------------------------------------------------*/
    data_size                              += sizeof(unsigned short);
    data_size                              += (unsigned short)strlen(zone.name.c_str()) + 1;

    data_size                              += sizeof(zone.type);
    data_size                              += sizeof(zone.leds_min);
    data_size                              += sizeof(zone.leds_max);
    data_size                              += sizeof(zone.leds_count);

    /*-----------------------------------------------------*\
    | Matrix map size                                       |
    \*-----------------------------------------------------*/
    data_size                              += sizeof(unsigned short);
    data_size                              += GetMatrixMapDescriptionSize(zone.matrix_map, protocol_version);

    if(protocol_version >= 4)
    {
        /*-------------------------------------------------*\
        | Number of segments in zone                        |
        \*-------------------------------------------------*/
        data_size                          += sizeof(unsigned short);

        for(size_t segment_index = 0; segment_index < zone.segments.size(); segment_index++)
        {
            data_size += GetSegmentDescriptionSize(zone.segments[segment_index], protocol_version);
        }
    }

    /*-----------------------------------------------------*\
    | Zone flags                                            |
    \*-----------------------------------------------------*/
    if(protocol_version >= 5)
    {
        data_size                          += sizeof(zone.flags);
    }

    /*-----------------------------------------------------*\
    | Zone modes                                            |
    \*-----------------------------------------------------*/
    if(protocol_version >= 6)
    {
        unsigned short num_modes            = (unsigned short)zone.modes.size();

        data_size                          += sizeof(num_modes);
        data_size                          += sizeof(zone.active_mode);

        for(int mode_index = 0; mode_index < num_modes; mode_index++)
        {
            data_size                      += GetModeDescriptionSize(zone.modes[mode_index], protocol_version);
        }
    }

    return(data_size);
}

unsigned char* RGBController::SetDeviceDescription(unsigned char* data_ptr, unsigned int protocol_version)
{
    /*-----------------------------------------------------*\
    | Lock access mutex                                     |
    \*-----------------------------------------------------*/
    AccessMutex.lock();

    /*-----------------------------------------------------*\
    | Copy in type                                          |
    \*-----------------------------------------------------*/
    memcpy(&type, data_ptr, sizeof(device_type));
    data_ptr += sizeof(device_type);

    /*-----------------------------------------------------*\
    | Copy in name                                          |
    \*-----------------------------------------------------*/
    unsigned short name_len;
    memcpy(&name_len, data_ptr, sizeof(name_len));
    data_ptr += sizeof(name_len);

    name.assign((char *)data_ptr, name_len);
    name = StringUtils::remove_null_terminating_chars(name);
    data_ptr += name_len;

    /*-----------------------------------------------------*\
    | Copy in vendor if protocol version is 1 or higher     |
    \*-----------------------------------------------------*/
    if(protocol_version >= 1)
    {
        unsigned short vendor_len;
        memcpy(&vendor_len, data_ptr, sizeof(vendor_len));
        data_ptr += sizeof(vendor_len);

        vendor = (char *)data_ptr;
        data_ptr += vendor_len;
    }

    /*-----------------------------------------------------*\
    | Copy in description                                   |
    \*-----------------------------------------------------*/
    unsigned short description_len;
    memcpy(&description_len, data_ptr, sizeof(description_len));
    data_ptr += sizeof(description_len);

    description.assign((char *)data_ptr, description_len);
    description = StringUtils::remove_null_terminating_chars(description);
    data_ptr += description_len;

    /*-----------------------------------------------------*\
    | Copy in version                                       |
    \*-----------------------------------------------------*/
    unsigned short version_len;
    memcpy(&version_len, data_ptr, sizeof(version_len));
    data_ptr += sizeof(version_len);

    version.assign((char *)data_ptr, version_len);
    version = StringUtils::remove_null_terminating_chars(version);
    data_ptr += version_len;

    /*-----------------------------------------------------*\
    | Copy in serial                                        |
    \*-----------------------------------------------------*/
    unsigned short serial_len;
    memcpy(&serial_len, data_ptr, sizeof(serial_len));
    data_ptr += sizeof(serial_len);

    serial.assign((char *)data_ptr, serial_len);
    serial = StringUtils::remove_null_terminating_chars(serial);
    data_ptr += serial_len;

    /*-----------------------------------------------------*\
    | Copy in location                                      |
    \*-----------------------------------------------------*/
    unsigned short location_len;
    memcpy(&location_len, data_ptr, sizeof(location_len));
    data_ptr += sizeof(location_len);

    location.assign((char *)data_ptr, location_len);
    location = StringUtils::remove_null_terminating_chars(location);
    data_ptr += location_len;

    /*-----------------------------------------------------*\
    | Copy in number of modes                               |
    \*-----------------------------------------------------*/
    unsigned short num_modes;
    memcpy(&num_modes, data_ptr, sizeof(num_modes));
    data_ptr += sizeof(num_modes);

    /*-----------------------------------------------------*\
    | Copy in active mode                                   |
    \*-----------------------------------------------------*/
    memcpy(&active_mode, data_ptr, sizeof(active_mode));
    data_ptr += sizeof(active_mode);

    /*-----------------------------------------------------*\
    | Copy in modes                                         |
    \*-----------------------------------------------------*/
    modes.resize(num_modes);

    for(int mode_index = 0; mode_index < num_modes; mode_index++)
    {
        data_ptr = SetModeDescription(data_ptr, &modes[mode_index], protocol_version);
    }

    /*-----------------------------------------------------*\
    | Copy in number of zones                               |
    \*-----------------------------------------------------*/
    unsigned short num_zones;
    memcpy(&num_zones, data_ptr, sizeof(num_zones));
    data_ptr += sizeof(num_zones);

    /*-----------------------------------------------------*\
    | Copy in zones                                         |
    \*-----------------------------------------------------*/
    zones.resize(num_zones);

    for(int zone_index = 0; zone_index < num_zones; zone_index++)
    {
        data_ptr = SetZoneDescription(data_ptr, &zones[zone_index], protocol_version);
    }

    /*-----------------------------------------------------*\
    | Copy in number of LEDs                                |
    \*-----------------------------------------------------*/
    unsigned short num_leds;
    memcpy(&num_leds, data_ptr, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*-----------------------------------------------------*\
    | Copy in LEDs                                          |
    \*-----------------------------------------------------*/
    leds.resize(num_leds);

    for(int led_index = 0; led_index < num_leds; led_index++)
    {
        data_ptr = SetLEDDescription(data_ptr, &leds[led_index], protocol_version);
    }

    /*-----------------------------------------------------*\
    | Copy in colors                                        |
    \*-----------------------------------------------------*/
    data_ptr = SetColorDescription(data_ptr, protocol_version, true);

    /*-----------------------------------------------------*\
    | Copy in LED alternate names data                      |
    \*-----------------------------------------------------*/
    if(protocol_version >= 5)
    {
        /*-------------------------------------------------*\
        | Copy in number of LED alternate names             |
        \*-------------------------------------------------*/
        unsigned short num_led_alt_names;

        memcpy(&num_led_alt_names, data_ptr, sizeof(num_led_alt_names));
        data_ptr += sizeof(num_led_alt_names);

        for(int led_idx = 0; led_idx < num_led_alt_names; led_idx++)
        {
            unsigned short string_length = 0;

            /*---------------------------------------------*\
            | Copy in LED alternate name string (size+data) |
            \*---------------------------------------------*/
            memcpy(&string_length, data_ptr, sizeof(string_length));
            data_ptr += sizeof(string_length);

            std::string new_name((char *)data_ptr, string_length);
            new_name = StringUtils::remove_null_terminating_chars(new_name);

            led_alt_names.push_back(new_name);
            data_ptr += string_length;
        }
    }

    /*-----------------------------------------------------*\
    | Copy in controller flags data                         |
    \*-----------------------------------------------------*/
    if(protocol_version >= 5)
    {
        memcpy(&flags, data_ptr, sizeof(flags));
        data_ptr += sizeof(flags);
    }

    /*-----------------------------------------------------*\
    | Unlock access mutex                                   |
    \*-----------------------------------------------------*/
    AccessMutex.unlock();

    /*-----------------------------------------------------*\
    | Setup colors                                          |
    \*-----------------------------------------------------*/
    SetupColors();

    return(data_ptr);
}

unsigned char* RGBController::SetColorDescription(unsigned char* data_ptr, unsigned int /*protocol_version*/, bool resize)
{
    /*-----------------------------------------------------*\
    | Copy in number of colors (data)                       |
    \*-----------------------------------------------------*/
    unsigned short num_colors;
    memcpy(&num_colors, data_ptr, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    if(resize)
    {
        colors.resize(num_colors);
    }
    else
    {
        /*-------------------------------------------------*\
        | Check if we aren't reading beyond the list of     |
        | colors.                                           |
        \*-------------------------------------------------*/
        if(((size_t)num_colors) > colors.size())
        {
            data_ptr += (num_colors * sizeof(RGBColor));
            return(data_ptr);
        }
    }

    /*-----------------------------------------------------*\
    | Copy in colors                                        |
    \*-----------------------------------------------------*/
    for(int color_index = 0; color_index < num_colors; color_index++)
    {
        memcpy(&colors[color_index], data_ptr, sizeof(colors[color_index]));
        data_ptr += sizeof(colors[color_index]);
    }

    return(data_ptr);
}

unsigned char* RGBController::SetLEDDescription(unsigned char* data_ptr, led* led, unsigned int /*protocol_version*/)
{
    /*-----------------------------------------------------*\
    | Copy in LED name (size+data)                          |
    \*-----------------------------------------------------*/
    unsigned short ledname_len;
    memcpy(&ledname_len, data_ptr, sizeof(ledname_len));
    data_ptr += sizeof(ledname_len);

    led->name.assign((char *)data_ptr, ledname_len);
    led->name = StringUtils::remove_null_terminating_chars(led->name);
    data_ptr += ledname_len;

    /*-----------------------------------------------------*\
    | Copy in LED value                                     |
    \*-----------------------------------------------------*/
    memcpy(&led->value, data_ptr, sizeof(led->value));
    data_ptr += sizeof(led->value);

    return(data_ptr);
}

unsigned char* RGBController::SetModeDescription(unsigned char* data_ptr, mode* mode, unsigned int protocol_version)
{
    /*-----------------------------------------------------*\
    | Copy in mode name (size+data)                         |
    \*-----------------------------------------------------*/
    unsigned short modename_len;
    memcpy(&modename_len, data_ptr, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    mode->name.assign((char *)data_ptr, modename_len);
    mode->name = StringUtils::remove_null_terminating_chars(mode->name);
    data_ptr += modename_len;

    /*-----------------------------------------------------*\
    | Copy in mode value                                    |
    \*-----------------------------------------------------*/
    memcpy(&mode->value, data_ptr, sizeof(mode->value));
    data_ptr += sizeof(mode->value);

    /*-----------------------------------------------------*\
    | Copy in mode flags                                    |
    \*-----------------------------------------------------*/
    memcpy(&mode->flags, data_ptr, sizeof(mode->flags));
    data_ptr += sizeof(mode->flags);

    /*-----------------------------------------------------*\
    | Copy in mode speed_min                                |
    \*-----------------------------------------------------*/
    memcpy(&mode->speed_min, data_ptr, sizeof(mode->speed_min));
    data_ptr += sizeof(mode->speed_min);

    /*-----------------------------------------------------*\
    | Copy in mode speed_max                                |
    \*-----------------------------------------------------*/
    memcpy(&mode->speed_max, data_ptr, sizeof(mode->speed_max));
    data_ptr += sizeof(mode->speed_max);

    /*-----------------------------------------------------*\
    | Copy in mode brightness_min and brightness_max (data) |
    | if protocol 3 or higher                               |
    \*-----------------------------------------------------*/
    if(protocol_version >= 3)
    {
        memcpy(&mode->brightness_min, data_ptr, sizeof(mode->brightness_min));
        data_ptr += sizeof(mode->brightness_min);

        memcpy(&mode->brightness_max, data_ptr, sizeof(mode->brightness_max));
        data_ptr += sizeof(mode->brightness_max);
    }

    /*-----------------------------------------------------*\
    | Copy in mode colors_min                               |
    \*-----------------------------------------------------*/
    memcpy(&mode->colors_min, data_ptr, sizeof(mode->colors_min));
    data_ptr += sizeof(mode->colors_min);

    /*-----------------------------------------------------*\
    | Copy in mode colors_max                               |
    \*-----------------------------------------------------*/
    memcpy(&mode->colors_max, data_ptr, sizeof(mode->colors_max));
    data_ptr += sizeof(mode->colors_max);

    /*-----------------------------------------------------*\
    | Copy in mode speed                                    |
    \*-----------------------------------------------------*/
    memcpy(&mode->speed, data_ptr, sizeof(mode->speed));
    data_ptr += sizeof(mode->speed);

    /*-----------------------------------------------------*\
    | Copy in mode brightness (data) if protocol 3 or higher|
    \*-----------------------------------------------------*/
    if(protocol_version >= 3)
    {
        memcpy(&mode->brightness, data_ptr, sizeof(mode->brightness));
        data_ptr += sizeof(mode->brightness);
    }

    /*-----------------------------------------------------*\
    | Copy in mode direction                                |
    \*-----------------------------------------------------*/
    memcpy(&mode->direction, data_ptr, sizeof(mode->direction));
    data_ptr += sizeof(mode->direction);

    /*-----------------------------------------------------*\
    | Copy in mode color_mode                               |
    \*-----------------------------------------------------*/
    memcpy(&mode->color_mode, data_ptr, sizeof(mode->color_mode));
    data_ptr += sizeof(mode->color_mode);

    /*-----------------------------------------------------*\
    | Copy in mode number of colors                         |
    \*-----------------------------------------------------*/
    unsigned short mode_num_colors;
    memcpy(&mode_num_colors, data_ptr, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*-----------------------------------------------------*\
    | Copy in mode mode colors                              |
    \*-----------------------------------------------------*/
    mode->colors.resize(mode_num_colors);
    for(int color_index = 0; color_index < mode_num_colors; color_index++)
    {
        memcpy(&mode->colors[color_index], data_ptr, sizeof(mode->colors[color_index]));
        data_ptr += sizeof(mode->colors[color_index]);
    }

    return(data_ptr);
}

unsigned char* RGBController::SetSegmentDescription(unsigned char* data_ptr, segment* segment, unsigned int protocol_version)
{
    /*-----------------------------------------------------*\
    | Copy in segment name (size+data)                      |
    \*-----------------------------------------------------*/
    unsigned short segmentname_len;
    memcpy(&segmentname_len, data_ptr, sizeof(segmentname_len));
    data_ptr += sizeof(segmentname_len);

    segment->name.assign((char *)data_ptr, segmentname_len);
    segment->name = StringUtils::remove_null_terminating_chars(segment->name);
    data_ptr += segmentname_len;

    /*-----------------------------------------------------*\
    | Copy in segment type                                  |
    \*-----------------------------------------------------*/
    memcpy(&segment->type, data_ptr, sizeof(segment->type));
    data_ptr += sizeof(segment->type);

    /*-----------------------------------------------------*\
    | Copy in segment start index                           |
    \*-----------------------------------------------------*/
    memcpy(&segment->start_idx, data_ptr, sizeof(segment->start_idx));
    data_ptr += sizeof(segment->start_idx);

    /*-----------------------------------------------------*\
    | Copy in segment LED count                             |
    \*-----------------------------------------------------*/
    memcpy(&segment->leds_count, data_ptr, sizeof(segment->leds_count));
    data_ptr += sizeof(segment->leds_count);

    /*-----------------------------------------------------*\
    | Copy in segment matrix map                            |
    \*-----------------------------------------------------*/
    if(protocol_version >= 6)
    {
        unsigned short matrix_map_len;
        memcpy(&matrix_map_len, data_ptr, sizeof(matrix_map_len));
        data_ptr += sizeof(matrix_map_len);

        /*-------------------------------------------------*\
        | Copy in matrix data if size is nonzero            |
        \*-------------------------------------------------*/
        if(matrix_map_len > 0)
        {
            data_ptr = SetMatrixMapDescription(data_ptr, &segment->matrix_map, protocol_version);
        }
    }

    return(data_ptr);
}

unsigned char* RGBController::SetMatrixMapDescription(unsigned char* data_ptr, matrix_map_type* matrix_map, unsigned int /*protocol_version*/)
{
    /*-----------------------------------------------------*\
    | Copy in matrix height                                 |
    \*-----------------------------------------------------*/
    memcpy(&matrix_map->height, data_ptr, sizeof(matrix_map->height));
    data_ptr += sizeof(matrix_map->height);

    /*-----------------------------------------------------*\
    | Copy in matrix width                                  |
    \*-----------------------------------------------------*/
    memcpy(&matrix_map->width, data_ptr, sizeof(matrix_map->width));
    data_ptr += sizeof(matrix_map->width);

    /*-----------------------------------------------------*\
    | Copy in matrix map                                    |
    \*-----------------------------------------------------*/
    matrix_map->map.resize(matrix_map->height * matrix_map->width);

    for(unsigned int matrix_idx = 0; matrix_idx < (matrix_map->height * matrix_map->width); matrix_idx++)
    {
        memcpy(&matrix_map->map[matrix_idx], data_ptr, sizeof(matrix_map->map[matrix_idx]));
        data_ptr += sizeof(matrix_map->map[matrix_idx]);
    }

    return(data_ptr);
}

unsigned char* RGBController::SetZoneDescription(unsigned char* data_ptr, zone* zone, unsigned int protocol_version)
{
    /*-----------------------------------------------------*\
    | Copy in zone name (size+data)                         |
    \*-----------------------------------------------------*/
    unsigned short zonename_len;
    memcpy(&zonename_len, data_ptr, sizeof(zonename_len));
    data_ptr += sizeof(zonename_len);

    zone->name.assign((char *)data_ptr, zonename_len);
    zone->name = StringUtils::remove_null_terminating_chars(zone->name);
    data_ptr += zonename_len;

    /*-----------------------------------------------------*\
    | Copy in zone type                                     |
    \*-----------------------------------------------------*/
    memcpy(&zone->type, data_ptr, sizeof(zone->type));
    data_ptr += sizeof(zone->type);

    /*-----------------------------------------------------*\
    | Copy in zone minimum LED count                        |
    \*-----------------------------------------------------*/
    memcpy(&zone->leds_min, data_ptr, sizeof(zone->leds_min));
    data_ptr += sizeof(zone->leds_min);

    /*-----------------------------------------------------*\
    | Copy in zone maximum LED count                        |
    \*-----------------------------------------------------*/
    memcpy(&zone->leds_max, data_ptr, sizeof(zone->leds_max));
    data_ptr += sizeof(zone->leds_max);

    /*-----------------------------------------------------*\
    | Copy in zone LED count                                |
    \*-----------------------------------------------------*/
    memcpy(&zone->leds_count, data_ptr, sizeof(zone->leds_count));
    data_ptr += sizeof(zone->leds_count);

    /*-----------------------------------------------------*\
    | Copy in size of matrix map                            |
    \*-----------------------------------------------------*/
    unsigned short matrix_map_len;
    memcpy(&matrix_map_len, data_ptr, sizeof(matrix_map_len));
    data_ptr += sizeof(matrix_map_len);

    /*-----------------------------------------------------*\
    | Copy in matrix data if size is nonzero                |
    \*-----------------------------------------------------*/
    if(matrix_map_len > 0)
    {
        data_ptr = SetMatrixMapDescription(data_ptr, &zone->matrix_map, protocol_version);
    }

    /*-----------------------------------------------------*\
    | Copy in segments                                      |
    \*-----------------------------------------------------*/
    if(protocol_version >= 4)
    {
        unsigned short num_segments = 0;

        /*-------------------------------------------------*\
        | Number of segments in zone                        |
        \*-------------------------------------------------*/
        memcpy(&num_segments, data_ptr, sizeof(num_segments));
        data_ptr += sizeof(num_segments);

        zone->segments.resize(num_segments);

        for(int segment_index = 0; segment_index < num_segments; segment_index++)
        {
            data_ptr = SetSegmentDescription(data_ptr, &zone->segments[segment_index], protocol_version);
        }
    }

    /*-----------------------------------------------------*\
    | Copy in zone flags                                    |
    \*-----------------------------------------------------*/
    if(protocol_version >= 5)
    {
        memcpy(&zone->flags, data_ptr, sizeof(zone->flags));
        data_ptr += sizeof(zone->flags);
    }

    /*-----------------------------------------------------*\
    | Copy in zone modes                                    |
    \*-----------------------------------------------------*/
    if(protocol_version >= 6)
    {
        /*-------------------------------------------------*\
        | Copy in number of modes                           |
        \*-------------------------------------------------*/
        unsigned short num_modes;
        memcpy(&num_modes, data_ptr, sizeof(num_modes));
        data_ptr += sizeof(num_modes);

        /*-------------------------------------------------*\
        | Copy in active mode                               |
        \*-------------------------------------------------*/
        memcpy(&zone->active_mode, data_ptr, sizeof(zone->active_mode));
        data_ptr += sizeof(zone->active_mode);

        /*-------------------------------------------------*\
        | Copy in modes                                     |
        \*-------------------------------------------------*/
        zone->modes.resize(num_modes);

        for(int mode_index = 0; mode_index < num_modes; mode_index++)
        {
            data_ptr = SetModeDescription(data_ptr, &zone->modes[mode_index], protocol_version);
        }
    }

    return(data_ptr);
}

/*---------------------------------------------------------*\
| JSON Description Functions                                |
\*---------------------------------------------------------*/
nlohmann::json RGBController::GetDeviceDescriptionJSON()
{
    nlohmann::json controller_json;

    /*-----------------------------------------------------*\
    | Lock access mutex                                     |
    \*-----------------------------------------------------*/
    AccessMutex.lock_shared();

    /*-----------------------------------------------------*\
    | Controller information strings                        |
    \*-----------------------------------------------------*/
    controller_json["description"]          = description;
    controller_json["location"]             = location;
    controller_json["name"]                 = name;
    controller_json["serial"]               = serial;
    controller_json["vendor"]               = vendor;
    controller_json["version"]              = version;

    /*-----------------------------------------------------*\
    | Controller variables                                  |
    \*-----------------------------------------------------*/
    controller_json["active_mode"]          = active_mode;
    controller_json["flags"]                = flags;
    controller_json["type"]                 = type;

    /*-----------------------------------------------------*\
    | Colors                                                |
    \*-----------------------------------------------------*/
    for(std::size_t color_idx = 0; color_idx < colors.size(); color_idx++)
    {
        controller_json["colors"][color_idx] = colors[color_idx];
    }

    /*-----------------------------------------------------*\
    | LEDs                                                  |
    \*-----------------------------------------------------*/
    for(std::size_t led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        controller_json["leds"][led_idx]  = GetLEDDescriptionJSON(leds[led_idx]);
    }

    /*-----------------------------------------------------*\
    | LED alternate names                                   |
    \*-----------------------------------------------------*/
    for(std::size_t led_alt_name_idx = 0; led_alt_name_idx < led_alt_names.size(); led_alt_name_idx++)
    {
        controller_json["led_alt_names"][led_alt_name_idx] = led_alt_names[led_alt_name_idx];
    }

    /*-----------------------------------------------------*\
    | Modes                                                 |
    \*-----------------------------------------------------*/
    for(std::size_t mode_idx = 0; mode_idx < modes.size(); mode_idx++)
    {
        controller_json["modes"][mode_idx]  = GetModeDescriptionJSON(modes[mode_idx]);
    }

    /*-----------------------------------------------------*\
    | Zones                                                 |
    \*-----------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        controller_json["zones"][zone_idx]  = GetZoneDescriptionJSON(zones[zone_idx]);
    }

    /*-----------------------------------------------------*\
    | Unlock access mutex                                   |
    \*-----------------------------------------------------*/
    AccessMutex.unlock_shared();

    return(controller_json);
}

nlohmann::json RGBController::GetLEDDescriptionJSON(led led)
{
    nlohmann::json led_json;

    /*-----------------------------------------------------*\
    | LED Information                                       |
    \*-----------------------------------------------------*/
    led_json["name"]                    = led.name;
    led_json["value"]                   = led.value;

    return(led_json);
}

nlohmann::json RGBController::GetMatrixMapDescriptionJSON(matrix_map_type matrix_map)
{
    nlohmann::json matrix_map_json;

    matrix_map_json["height"]                                   = matrix_map.height;
    matrix_map_json["width"]                                    = matrix_map.width;

    for(unsigned int matrix_map_idx = 0; matrix_map_idx < (matrix_map.height * matrix_map.width); matrix_map_idx++)
    {
        matrix_map_json["map"][matrix_map_idx]                  = matrix_map.map[matrix_map_idx];
    }

    return(matrix_map_json);
}

nlohmann::json RGBController::GetModeDescriptionJSON(mode mode)
{
    nlohmann::json mode_json;

    /*-----------------------------------------------------*\
    | Mode Information                                      |
    \*-----------------------------------------------------*/
    mode_json["name"]                                           = mode.name;
    mode_json["value"]                                          = mode.value;
    mode_json["flags"]                                          = mode.flags;
    mode_json["speed_min"]                                      = mode.speed_min;
    mode_json["speed_max"]                                      = mode.speed_max;
    mode_json["brightness_min"]                                 = mode.brightness_min;
    mode_json["brightness_max"]                                 = mode.brightness_max;
    mode_json["colors_min"]                                     = mode.colors_min;
    mode_json["colors_max"]                                     = mode.colors_max;

    /*-----------------------------------------------------*\
    | Mode Settings                                         |
    \*-----------------------------------------------------*/
    mode_json["speed"]                                          = mode.speed;
    mode_json["brightness"]                                     = mode.brightness;
    mode_json["direction"]                                      = mode.direction;
    mode_json["color_mode"]                                     = mode.color_mode;

    for(std::size_t color_idx = 0; color_idx < mode.colors.size(); color_idx++)
    {
        mode_json["colors"][color_idx]                          = mode.colors[color_idx];
    }

    return(mode_json);
}

nlohmann::json RGBController::GetSegmentDescriptionJSON(segment segment)
{
    nlohmann::json segment_json;

    /*-----------------------------------------------------*\
    | Segment Information                                   |
    \*-----------------------------------------------------*/
    segment_json["name"]                                        = segment.name;
    segment_json["type"]                                        = segment.type;
    segment_json["start_idx"]                                   = segment.start_idx;
    segment_json["leds_count"]                                  = segment.leds_count;
    segment_json["matrix_map"]                                  = GetMatrixMapDescriptionJSON(segment.matrix_map);

    return(segment_json);
}

nlohmann::json RGBController::GetZoneDescriptionJSON(zone zone)
{
    nlohmann::json zone_json;

    /*-----------------------------------------------------*\
    | Zone Information                                      |
    \*-----------------------------------------------------*/
    zone_json["name"]                                           = zone.name;
    zone_json["type"]                                           = zone.type;
    zone_json["leds_count"]                                     = zone.leds_count;
    zone_json["leds_min"]                                       = zone.leds_min;
    zone_json["leds_max"]                                       = zone.leds_max;
    zone_json["matrix_map"]                                     = GetMatrixMapDescriptionJSON(zone.matrix_map);

    for(std::size_t segment_idx = 0; segment_idx < zone.segments.size(); segment_idx++)
    {
        zone_json["segments"][segment_idx]                      = GetSegmentDescriptionJSON(zone.segments[segment_idx]);
    }

    zone_json["flags"]                                          = zone.flags;

    for(std::size_t mode_idx = 0; mode_idx < zone.modes.size(); mode_idx++)
    {
        zone_json["modes"][mode_idx]                            = GetModeDescriptionJSON(zone.modes[mode_idx]);
    }

    if(zone.modes.size() > 0)
    {
        zone_json["active_mode"]                                = zone.active_mode;
    }

    return(zone_json);
}

void RGBController::SetDeviceDescriptionJSON(nlohmann::json controller_json)
{
    /*-----------------------------------------------------*\
    | Lock access mutex                                     |
    \*-----------------------------------------------------*/
    AccessMutex.lock();

    /*-----------------------------------------------------*\
    | Controller information strings                        |
    \*-----------------------------------------------------*/
    if(controller_json.contains("description"))
    {
        description                         = controller_json["description"];
    }

    if(controller_json.contains("location"))
    {
        location                            = controller_json["location"];
    }

    if(controller_json.contains("name"))
    {
        name                                = controller_json["name"];
    }

    if(controller_json.contains("serial"))
    {
        serial                              = controller_json["serial"];
    }

    if(controller_json.contains("vendor"))
    {
        vendor                              = controller_json["vendor"];
    }

    if(controller_json.contains("version"))
    {
        version                             = controller_json["version"];
    }

    /*-----------------------------------------------------*\
    | Controller variables                                  |
    \*-----------------------------------------------------*/
    if(controller_json.contains("active_mode"))
    {
        active_mode                         = controller_json["active_mode"];
    }

    if(controller_json.contains("flags"))
    {
        flags                               = controller_json["flags"];
    }

    if(controller_json.contains("type"))
    {
        type                                = controller_json["type"];
    }

    /*-----------------------------------------------------*\
    | Colors                                                |
    \*-----------------------------------------------------*/
    if(controller_json.contains("colors"))
    {
        colors.resize(controller_json["colors"].size());

        for(std::size_t color_idx = 0; color_idx < colors.size(); color_idx++)
        {
            colors[color_idx]               = controller_json["colors"][color_idx];
        }
    }

    /*-----------------------------------------------------*\
    | LEDs                                                  |
    \*-----------------------------------------------------*/
    if(controller_json.contains("leds"))
    {
        leds.resize(controller_json["leds"].size());

        for(std::size_t led_idx = 0; led_idx < leds.size(); led_idx++)
        {
            leds[led_idx]                   = SetLEDDescriptionJSON(controller_json["leds"][led_idx]);
        }
    }

    /*-----------------------------------------------------*\
    | LED alternate names                                   |
    \*-----------------------------------------------------*/
    if(controller_json.contains("led_alt_names"))
    {
        led_alt_names.resize(controller_json["led_alt_names"].size());

        for(std::size_t led_alt_name_idx = 0; led_alt_name_idx < led_alt_names.size(); led_alt_name_idx++)
        {
            led_alt_names[led_alt_name_idx] = controller_json["led_alt_names"][led_alt_name_idx];
        }
    }

    /*-----------------------------------------------------*\
    | Modes                                                 |
    \*-----------------------------------------------------*/
    if(controller_json.contains("modes"))
    {
        modes.resize(controller_json["modes"].size());

        for(std::size_t mode_idx = 0; mode_idx < modes.size(); mode_idx++)
        {
            modes[mode_idx]                 = SetModeDescriptionJSON(controller_json["modes"][mode_idx]);
        }
    }

    /*-----------------------------------------------------*\
    | Zones                                                 |
    \*-----------------------------------------------------*/
    if(controller_json.contains("zones"))
    {
        zones.resize(controller_json["zones"].size());

        for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
        {
            zones[zone_idx]                 = SetZoneDescriptionJSON(controller_json["zones"][zone_idx]);
        }
    }

    /*-----------------------------------------------------*\
    | Unlock access mutex                                   |
    \*-----------------------------------------------------*/
    AccessMutex.unlock();

    /*-----------------------------------------------------*\
    | Setup colors                                          |
    \*-----------------------------------------------------*/
    SetupColors();
}

led RGBController::SetLEDDescriptionJSON(nlohmann::json led_json)
{
    led new_led;

    /*-----------------------------------------------------*\
    | LED Information                                       |
    \*-----------------------------------------------------*/
    if(led_json.contains("name"))
    {
        new_led.name                        = led_json["name"];
    }

    if(led_json.contains("value"))
    {
        new_led.value                       = led_json["value"];
    }

    return(new_led);
}

matrix_map_type RGBController::SetMatrixMapDescriptionJSON(nlohmann::json matrix_map_json)
{
    matrix_map_type matrix_map;

    if(matrix_map_json["matrix_map"].contains("width") &&
       matrix_map_json["matrix_map"].contains("height") &&
       matrix_map_json["matrix_map"].contains("map"))
    {
        matrix_map.width                    = matrix_map_json["matrix_map"]["width"];
        matrix_map.height                   = matrix_map_json["matrix_map"]["height"];

        matrix_map.map.resize(matrix_map.width * matrix_map.height);

        for(unsigned int matrix_map_idx = 0; matrix_map_idx < matrix_map.width * matrix_map.height; matrix_map_idx++)
        {
            matrix_map.map[matrix_map_idx]  = matrix_map_json["matrix_map"]["map"][matrix_map_idx];
        }
    }

    return(matrix_map);
}

mode RGBController::SetModeDescriptionJSON(nlohmann::json mode_json)
{
    mode new_mode;

    /*-----------------------------------------------------*\
    | Mode Information                                      |
    \*-----------------------------------------------------*/
    if(mode_json.contains("name"))
    {
        new_mode.name                       = mode_json["name"];
    }

    if(mode_json.contains("value"))
    {
        new_mode.value                      = mode_json["value"];
    }

    if(mode_json.contains("flags"))
    {
        new_mode.flags                      = mode_json["flags"];
    }

    if(mode_json.contains("speed_min"))
    {
        new_mode.speed_min                  = mode_json["speed_min"];
    }

    if(mode_json.contains("speed_max"))
    {
        new_mode.speed_max                  = mode_json["speed_max"];
    }

    if(mode_json.contains("brightness_min"))
    {
        new_mode.brightness_min             = mode_json["brightness_min"];
    }

    if(mode_json.contains("brightness_max"))
    {
        new_mode.brightness_max             = mode_json["brightness_max"];
    }

    if(mode_json.contains("colors_min"))
    {
        new_mode.colors_min                 = mode_json["colors_min"];
    }

    if(mode_json.contains("colors_max"))
    {
        new_mode.colors_max                 = mode_json["colors_max"];
    }

    /*-----------------------------------------------------*\
    | Mode Settings                                         |
    \*-----------------------------------------------------*/
    if(mode_json.contains("speed"))
    {
        new_mode.speed                      = mode_json["speed"];
    }

    if(mode_json.contains("brightness"))
    {
        new_mode.brightness                 = mode_json["brightness"];
    }

    if(mode_json.contains("direction"))
    {
        new_mode.direction                  = mode_json["direction"];
    }

    if(mode_json.contains("color_mode"))
    {
        new_mode.color_mode                 = mode_json["color_mode"];
    }

    if(mode_json.contains("colors"))
    {
        new_mode.colors.resize(mode_json["colors"].size());

        for(std::size_t color_idx = 0; color_idx < new_mode.colors.size(); color_idx++)
        {
            new_mode.colors[color_idx]      = mode_json["colors"][color_idx];
        }
    }

    return(new_mode);
}

segment RGBController::SetSegmentDescriptionJSON(nlohmann::json segment_json)
{
    segment new_segment;

    /*-----------------------------------------------------*\
    | Segment Information                                   |
    \*-----------------------------------------------------*/
    if(segment_json.contains("name"))
    {
        new_segment.name                                    = segment_json["name"];
    }

    if(segment_json.contains("type"))
    {
        new_segment.type                                    = segment_json["type"];
    }

    if(segment_json.contains("start_idx"))
    {
        new_segment.start_idx                               = segment_json["start_idx"];
    }

    if(segment_json.contains("leds_count"))
    {
        new_segment.leds_count                              = segment_json["leds_count"];
    }

    if(segment_json.contains("matrix_map"))
    {
        new_segment.matrix_map                              = SetMatrixMapDescriptionJSON(segment_json["matrix_map"]);
    }

    return(new_segment);
}

zone RGBController::SetZoneDescriptionJSON(nlohmann::json zone_json)
{
    zone new_zone;

    /*-----------------------------------------------------*\
    | Zone Information                                      |
    \*-----------------------------------------------------*/
    if(zone_json.contains("name"))
    {
        new_zone.name                                       = zone_json["name"];
    }

    if(zone_json.contains("type"))
    {
        new_zone.type                                       = zone_json["type"];
    }

    if(zone_json.contains("leds_count"))
    {
        new_zone.leds_count                                 = zone_json["leds_count"];
    }

    if(zone_json.contains("leds_min"))
    {
        new_zone.leds_min                                   = zone_json["leds_min"];
    }

    if(zone_json.contains("leds_max"))
    {
        new_zone.leds_max                                   = zone_json["leds_max"];
    }

    if(zone_json.contains("matrix_map"))
    {
        new_zone.matrix_map                                 = SetMatrixMapDescriptionJSON(zone_json["matrix_map"]);
    }

    if(zone_json.contains("segments"))
    {
        new_zone.segments.resize(zone_json["segments"].size());

        for(std::size_t segment_idx = 0; segment_idx < new_zone.segments.size(); segment_idx++)
        {
            new_zone.segments[segment_idx]                  = SetSegmentDescriptionJSON(zone_json["segments"][segment_idx]);
        }
    }

    if(zone_json.contains("flags"))
    {
        new_zone.flags                                      = zone_json["flags"];
    }

    if(zone_json.contains("modes"))
    {
        new_zone.modes.resize(zone_json["modes"].size());

        for(std::size_t mode_idx = 0; mode_idx < new_zone.modes.size(); mode_idx++)
        {
            new_zone.modes[mode_idx]                        = SetModeDescriptionJSON(zone_json["modes"][mode_idx]);
        }
    }

    if(zone_json.contains("active_mode"))
    {
        new_zone.active_mode                                = zone_json["active_mode"];
    }

    return(new_zone);
}

/*---------------------------------------------------------*\
| Update Callback Functions                                 |
\*---------------------------------------------------------*/
void RGBController::RegisterUpdateCallback(RGBControllerCallback new_callback, void * new_callback_arg)
{
    UpdateMutex.lock();

    /*-----------------------------------------------------*\
    | Check to see if this is already registered to avoid   |
    | registering the same callback multiple times          |
    \*-----------------------------------------------------*/
    for(unsigned int callback_idx = 0; callback_idx < UpdateCallbackArgs.size(); callback_idx++ )
    {
        if((UpdateCallbacks[callback_idx] == new_callback) && (UpdateCallbackArgs[callback_idx] == new_callback_arg))
        {
            UpdateMutex.unlock();
            return;
        }
    }

    UpdateCallbacks.push_back(new_callback);
    UpdateCallbackArgs.push_back(new_callback_arg);
    UpdateMutex.unlock();
}

void RGBController::UnregisterUpdateCallback(void * callback_arg)
{
    UpdateMutex.lock();
    for(unsigned int callback_idx = 0; callback_idx < UpdateCallbackArgs.size(); callback_idx++ )
    {
        if(UpdateCallbackArgs[callback_idx] == callback_arg)
        {
            UpdateCallbackArgs.erase(UpdateCallbackArgs.begin() + callback_idx);
            UpdateCallbacks.erase(UpdateCallbacks.begin() + callback_idx);
        }
    }
    UpdateMutex.unlock();
}

void RGBController::ClearCallbacks()
{
    UpdateMutex.lock();
    UpdateCallbacks.clear();
    UpdateCallbackArgs.clear();
    UpdateMutex.unlock();
}

void RGBController::SignalUpdate(unsigned int update_reason)
{
    UpdateMutex.lock();

    /*-----------------------------------------------------*\
    | Client info has changed, call the callbacks           |
    \*-----------------------------------------------------*/
    for(unsigned int callback_idx = 0; callback_idx < UpdateCallbacks.size(); callback_idx++)
    {
        UpdateCallbacks[callback_idx](UpdateCallbackArgs[callback_idx], update_reason, this);
    }

    UpdateMutex.unlock();
}

/*---------------------------------------------------------*\
| Device Update Functions                                   |
\*---------------------------------------------------------*/
void RGBController::UpdateLEDs()
{
    CallFlag_UpdateLEDs = true;

    SignalUpdate(RGBCONTROLLER_UPDATE_REASON_UPDATELEDS);
}

void RGBController::UpdateZoneLEDs(int zone)
{
    AccessMutex.lock_shared();
    DeviceUpdateZoneLEDs(zone);
    AccessMutex.unlock_shared();
}

void RGBController::UpdateSingleLED(int led)
{
    AccessMutex.lock_shared();
    DeviceUpdateSingleLED(led);
    AccessMutex.unlock_shared();
}

void RGBController::UpdateMode()
{
    CallFlag_UpdateMode = true;

    SignalUpdate(RGBCONTROLLER_UPDATE_REASON_UPDATEMODE);
}

void RGBController::UpdateZoneMode(int zone)
{
    AccessMutex.lock_shared();
    DeviceUpdateZoneMode(zone);
    AccessMutex.unlock_shared();
}

void RGBController::SaveMode()
{
    AccessMutex.lock_shared();
    DeviceSaveMode();
    AccessMutex.unlock_shared();

    SignalUpdate(RGBCONTROLLER_UPDATE_REASON_SAVEMODE);
}

void RGBController::DeviceCallThreadFunction()
{
    CallFlag_UpdateLEDs = false;
    CallFlag_UpdateMode = false;

    while(DeviceThreadRunning.load() == true)
    {
        if(CallFlag_UpdateMode.load() == true)
        {
            if(flags & CONTROLLER_FLAG_RESET_BEFORE_UPDATE)
            {
                AccessMutex.lock_shared();
                CallFlag_UpdateMode = false;
                DeviceUpdateMode();
                AccessMutex.unlock_shared();
            }
            else
            {
                AccessMutex.lock_shared();
                DeviceUpdateMode();
                CallFlag_UpdateMode = false;
                AccessMutex.unlock_shared();
            }
        }
        if(CallFlag_UpdateLEDs.load() == true)
        {
            if(flags & CONTROLLER_FLAG_RESET_BEFORE_UPDATE)
            {
                AccessMutex.lock_shared();
                CallFlag_UpdateLEDs = false;
                DeviceUpdateLEDs();
                AccessMutex.unlock_shared();
            }
            else
            {
                AccessMutex.lock_shared();
                DeviceUpdateLEDs();
                CallFlag_UpdateLEDs = false;
                AccessMutex.unlock_shared();
            }
        }
        else
        {
           std::this_thread::sleep_for(1ms);
        }
    }
}

void RGBController::ClearSegments(int zone)
{
    AccessMutex.lock();
    zones[zone].segments.clear();
    AccessMutex.unlock();

    zones[zone].flags |= ZONE_FLAG_MANUALLY_CONFIGURED;

    SignalUpdate(RGBCONTROLLER_UPDATE_REASON_CLEARSEGMENTS);
}

void RGBController::AddSegment(int zone, segment new_segment)
{
    AccessMutex.lock();
    zones[zone].segments.push_back(new_segment);
    AccessMutex.unlock();

    zones[zone].flags |= ZONE_FLAG_MANUALLY_CONFIGURED;

    SignalUpdate(RGBCONTROLLER_UPDATE_REASON_ADDSEGMENT);
}

void RGBController::ResizeZone(int zone, int new_size)
{
    AccessMutex.lock();
    DeviceResizeZone(zone, new_size);
    AccessMutex.unlock();

    zones[zone].flags |= ZONE_FLAG_MANUALLY_CONFIGURED;

    SignalUpdate(RGBCONTROLLER_UPDATE_REASON_RESIZEZONE);
}

/*---------------------------------------------------------*\
| Functions not part of interface for internal use only     |
\*---------------------------------------------------------*/
unsigned int RGBController::LEDsInZone(unsigned int zone)
{
    unsigned int leds_count;

    leds_count = zones[zone].leds_count;

    if(zones[zone].flags & ZONE_FLAG_RESIZE_EFFECTS_ONLY)
    {
        if(leds_count > 1)
        {
            leds_count = 1;
        }
    }

    return(leds_count);
}

void RGBController::SetupColors()
{
    unsigned int total_led_count;
    unsigned int zone_led_count;

    /*-----------------------------------------------------*\
    | Determine total number of LEDs on the device          |
    \*-----------------------------------------------------*/
    total_led_count = 0;

    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        total_led_count += LEDsInZone((unsigned int)zone_idx);
    }

    /*-----------------------------------------------------*\
    | Set the size of the color buffer to the number of LEDs|
    \*-----------------------------------------------------*/
    colors.resize(total_led_count);

    /*-----------------------------------------------------*\
    | Set the color buffer pointers on each zone            |
    \*-----------------------------------------------------*/
    total_led_count = 0;

    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].start_idx   = total_led_count;
        zone_led_count              = LEDsInZone((unsigned int)zone_idx);

        if((colors.size() > 0) && (zone_led_count > 0))
        {
            zones[zone_idx].colors = &colors[total_led_count];
        }
        else
        {
            zones[zone_idx].colors = NULL;
        }

        if((leds.size() > 0) && (zone_led_count > 0))
        {
            zones[zone_idx].leds   = &leds[total_led_count];
        }
        else
        {
            zones[zone_idx].leds    = NULL;
        }


        total_led_count += zone_led_count;
    }
}

void RGBController::UpdateLEDsInternal()
{
    CallFlag_UpdateLEDs = true;
}

/*---------------------------------------------------------*\
| Functions to be implemented in device implementation      |
\*---------------------------------------------------------*/
void RGBController::DeviceResizeZone(int /*zone*/, int /*new_size*/)
{
    /*-----------------------------------------------------*\
    | If not implemented by controller, does nothing        |
    \*-----------------------------------------------------*/
}

void RGBController::DeviceUpdateLEDs()
{
    /*-----------------------------------------------------*\
    | If not implemented by controller, does nothing        |
    \*-----------------------------------------------------*/
}

void RGBController::DeviceUpdateZoneLEDs(int /*zone*/)
{
    /*-----------------------------------------------------*\
    | If not implemented by controller, does nothing        |
    \*-----------------------------------------------------*/
}

void RGBController::DeviceUpdateSingleLED(int /*led*/)
{
    /*-----------------------------------------------------*\
    | If not implemented by controller, does nothing        |
    \*-----------------------------------------------------*/
}

void RGBController::DeviceUpdateMode()
{
    /*-----------------------------------------------------*\
    | If not implemented by controller, does nothing        |
    \*-----------------------------------------------------*/
}

void RGBController::DeviceUpdateZoneMode(int /*zone*/)
{
    /*-----------------------------------------------------*\
    | If not implemented by controller, does nothing        |
    \*-----------------------------------------------------*/
}

void RGBController::DeviceSaveMode()
{
    /*-----------------------------------------------------*\
    | If not implemented by controller, does nothing        |
    \*-----------------------------------------------------*/
}

/*---------------------------------------------------------*\
| Non-class functions                                       |
\*---------------------------------------------------------*/
std::string device_type_to_str(device_type type)
{
    switch(type)
    {
    case DEVICE_TYPE_MOTHERBOARD:
        return "Motherboard";
    case DEVICE_TYPE_DRAM:
        return "DRAM";
    case DEVICE_TYPE_GPU:
        return "GPU";
    case DEVICE_TYPE_COOLER:
        return "Cooler";
    case DEVICE_TYPE_LEDSTRIP:
        return "LED Strip";
    case DEVICE_TYPE_KEYBOARD:
        return "Keyboard";
    case DEVICE_TYPE_MOUSE:
        return "Mouse";
    case DEVICE_TYPE_MOUSEMAT:
        return "Mousemat";
    case DEVICE_TYPE_HEADSET:
        return "Headset";
    case DEVICE_TYPE_HEADSET_STAND:
        return "Headset Stand";
    case DEVICE_TYPE_GAMEPAD:
        return "Gamepad";
    case DEVICE_TYPE_LIGHT:
        return "Light";
    case DEVICE_TYPE_SPEAKER:
        return "Speaker";
    case DEVICE_TYPE_VIRTUAL:
        return "Virtual";
    case DEVICE_TYPE_STORAGE:
        return "Storage";
    case DEVICE_TYPE_CASE:
        return "Case";
    case DEVICE_TYPE_MICROPHONE:
        return "Microphone";
    case DEVICE_TYPE_ACCESSORY:
        return "Accessory";
    case DEVICE_TYPE_KEYPAD:
        return "Keypad";
    case DEVICE_TYPE_LAPTOP:
        return "Laptop";
    case DEVICE_TYPE_MONITOR:
        return "Monitor";
    default:
        return "Unknown";
    }
}
