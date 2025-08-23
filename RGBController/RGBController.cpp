/*---------------------------------------------------------*\
| RGBController.cpp                                         |
|                                                           |
|   OpenRGB's RGB controller hardware abstration layer,     |
|   provides a generic representation of an RGB device      |
|                                                           |
|   Adam Honse (CalcProgrammer1)                02 Jun 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "RGBController.h"

using namespace std::chrono_literals;

mode::mode()
{
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

zone::zone()
{
    name        = "";
    type        = 0;
    leds        = NULL;
    colors      = NULL;
    start_idx   = 0;
    leds_count  = 0;
    leds_min    = 0;
    leds_max    = 0;
    matrix_map  = NULL;
    flags       = 0;
}

zone::~zone()
{

}

RGBController::RGBController()
{
    flags       = 0;
    DeviceThreadRunning = true;
    DeviceCallThread = new std::thread(&RGBController::DeviceCallThreadFunction, this);
}

RGBController::~RGBController()
{
    DeviceThreadRunning = false;
    DeviceCallThread->join();
    delete DeviceCallThread;

    leds.clear();
    colors.clear();
    zones.clear();
    modes.clear();
}

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

std::string RGBController::GetModeName(unsigned int mode)
{
    return(modes[mode].name);
}

std::string RGBController::GetZoneName(unsigned int zone)
{
    return(zones[zone].name);
}

std::string RGBController::GetLEDName(unsigned int led)
{
    if(led < led_alt_names.size())
    {
        if(led_alt_names[led] != "")
        {
            return(led_alt_names[led]);
        }
    }

    return(leds[led].name);
}

unsigned char * RGBController::GetDeviceDescription(unsigned int protocol_version)
{
    unsigned int data_ptr = 0;
    unsigned int data_size = 0;

    /*---------------------------------------------------------*\
    | Calculate data size                                       |
    \*---------------------------------------------------------*/
    unsigned short name_len         = (unsigned short)strlen(name.c_str())        + 1;
    unsigned short vendor_len       = (unsigned short)strlen(vendor.c_str())      + 1;
    unsigned short description_len  = (unsigned short)strlen(description.c_str()) + 1;
    unsigned short version_len      = (unsigned short)strlen(version.c_str())     + 1;
    unsigned short serial_len       = (unsigned short)strlen(serial.c_str())      + 1;
    unsigned short location_len     = (unsigned short)strlen(location.c_str())    + 1;
    unsigned short num_modes        = (unsigned short)modes.size();
    unsigned short num_zones        = (unsigned short)zones.size();
    unsigned short num_leds         = (unsigned short)leds.size();
    unsigned short num_colors       = (unsigned short)colors.size();
    unsigned short num_led_alt_names= (unsigned short)led_alt_names.size();

    unsigned short *mode_name_len   = new unsigned short[num_modes];
    unsigned short *zone_name_len   = new unsigned short[num_zones];
    unsigned short *led_name_len    = new unsigned short[num_leds];

    unsigned short *zone_matrix_len = new unsigned short[num_zones];
    unsigned short *mode_num_colors = new unsigned short[num_modes];

    data_size += sizeof(data_size);
    data_size += sizeof(device_type);
    data_size += name_len           + sizeof(name_len);

    if(protocol_version >= 1)
    {
        data_size += vendor_len     + sizeof(vendor_len);
    }

    data_size += description_len    + sizeof(description_len);
    data_size += version_len        + sizeof(version_len);
    data_size += serial_len         + sizeof(serial_len);
    data_size += location_len       + sizeof(location_len);

    data_size += sizeof(num_modes);
    data_size += sizeof(active_mode);

    for(int mode_index = 0; mode_index < num_modes; mode_index++)
    {
        mode_name_len[mode_index]   = (unsigned short)strlen(modes[mode_index].name.c_str()) + 1;
        mode_num_colors[mode_index] = (unsigned short)modes[mode_index].colors.size();

        data_size += mode_name_len[mode_index] + sizeof(mode_name_len[mode_index]);
        data_size += sizeof(modes[mode_index].value);
        data_size += sizeof(modes[mode_index].flags);
        data_size += sizeof(modes[mode_index].speed_min);
        data_size += sizeof(modes[mode_index].speed_max);
        if(protocol_version >= 3)
        {
            data_size += sizeof(modes[mode_index].brightness_min);
            data_size += sizeof(modes[mode_index].brightness_max);
        }
        data_size += sizeof(modes[mode_index].colors_min);
        data_size += sizeof(modes[mode_index].colors_max);
        data_size += sizeof(modes[mode_index].speed);
        if(protocol_version >= 3)
        {
            data_size += sizeof(modes[mode_index].brightness);
        }
        data_size += sizeof(modes[mode_index].direction);
        data_size += sizeof(modes[mode_index].color_mode);
        data_size += sizeof(mode_num_colors[mode_index]);
        data_size += (mode_num_colors[mode_index] * sizeof(RGBColor));
    }

    data_size += sizeof(num_zones);

    for(int zone_index = 0; zone_index < num_zones; zone_index++)
    {
        zone_name_len[zone_index]   = (unsigned short)strlen(zones[zone_index].name.c_str()) + 1;

        data_size += zone_name_len[zone_index] + sizeof(zone_name_len[zone_index]);
        data_size += sizeof(zones[zone_index].type);
        data_size += sizeof(zones[zone_index].leds_min);
        data_size += sizeof(zones[zone_index].leds_max);
        data_size += sizeof(zones[zone_index].leds_count);

        if(zones[zone_index].matrix_map == NULL)
        {
            zone_matrix_len[zone_index] = 0;
        }
        else
        {
            zone_matrix_len[zone_index] = (unsigned short)((2 * sizeof(unsigned int)) + (zones[zone_index].matrix_map->height * zones[zone_index].matrix_map->width * sizeof(unsigned int)));
        }

        data_size += sizeof(zone_matrix_len[zone_index]);
        data_size += zone_matrix_len[zone_index];

        if(protocol_version >= 4)
        {
            /*---------------------------------------------------------*\
            | Number of segments in zone                                |
            \*---------------------------------------------------------*/
            data_size += sizeof(unsigned short);

            for(size_t segment_index = 0; segment_index < zones[zone_index].segments.size(); segment_index++)
            {
                /*---------------------------------------------------------*\
                | Length of segment name string                             |
                \*---------------------------------------------------------*/
                data_size += sizeof(unsigned short);

                /*---------------------------------------------------------*\
                | Segment name string data                                  |
                \*---------------------------------------------------------*/
                data_size += (unsigned int)strlen(zones[zone_index].segments[segment_index].name.c_str()) + 1;

                data_size += sizeof(zones[zone_index].segments[segment_index].type);
                data_size += sizeof(zones[zone_index].segments[segment_index].start_idx);
                data_size += sizeof(zones[zone_index].segments[segment_index].leds_count);
            }
        }

        /*---------------------------------------------------------*\
        | Zone flags                                                |
        \*---------------------------------------------------------*/
        if(protocol_version >= 5)
        {
            data_size += sizeof(unsigned int);
        }
    }

    data_size += sizeof(num_leds);

    for(int led_index = 0; led_index < num_leds; led_index++)
    {
        led_name_len[led_index] = (unsigned short)strlen(leds[led_index].name.c_str()) + 1;

        data_size += led_name_len[led_index] + sizeof(led_name_len[led_index]);

        data_size += sizeof(leds[led_index].value);
    }

    /*---------------------------------------------------------*\
    | LED alternate names                                       |
    \*---------------------------------------------------------*/
    if(protocol_version >= 5)
    {
        /*-----------------------------------------------------*\
        | Number of LED alternate names                         |
        \*-----------------------------------------------------*/
        data_size += sizeof(num_led_alt_names);

        /*-----------------------------------------------------*\
        | LED alternate name strings                            |
        \*-----------------------------------------------------*/
        for(std::size_t led_idx = 0; led_idx < led_alt_names.size(); led_idx++)
        {
            data_size += sizeof(unsigned short);
            data_size += (unsigned int)strlen(led_alt_names[led_idx].c_str()) + 1;
        }
    }

    /*---------------------------------------------------------*\
    | Controller flags                                          |
    \*---------------------------------------------------------*/
    if(protocol_version >= 5)
    {
        data_size += sizeof(flags);
    }

    data_size += sizeof(num_colors);
    data_size += num_colors * sizeof(RGBColor);

    /*---------------------------------------------------------*\
    | Create data buffer                                        |
    \*---------------------------------------------------------*/
    unsigned char *data_buf = new unsigned char[data_size];

    /*---------------------------------------------------------*\
    | Copy in data size                                         |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &data_size, sizeof(data_size));
    data_ptr += sizeof(data_size);

    /*---------------------------------------------------------*\
    | Copy in type                                              |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &type, sizeof(device_type));
    data_ptr += sizeof(device_type);

    /*---------------------------------------------------------*\
    | Copy in name (size+data)                                  |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &name_len, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    strcpy((char *)&data_buf[data_ptr], name.c_str());
    data_ptr += name_len;

    /*---------------------------------------------------------*\
    | Copy in vendor (size+data) if protocol 1 or higher        |
    \*---------------------------------------------------------*/
    if(protocol_version >= 1)
    {
        memcpy(&data_buf[data_ptr], &vendor_len, sizeof(unsigned short));
        data_ptr += sizeof(unsigned short);

        strcpy((char *)&data_buf[data_ptr], vendor.c_str());
        data_ptr += vendor_len;
    }

    /*---------------------------------------------------------*\
    | Copy in description (size+data)                           |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &description_len, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    strcpy((char *)&data_buf[data_ptr], description.c_str());
    data_ptr += description_len;

    /*---------------------------------------------------------*\
    | Copy in version (size+data)                               |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &version_len, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    strcpy((char *)&data_buf[data_ptr], version.c_str());
    data_ptr += version_len;

    /*---------------------------------------------------------*\
    | Copy in serial (size+data)                                |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &serial_len, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    strcpy((char *)&data_buf[data_ptr], serial.c_str());
    data_ptr += serial_len;

    /*---------------------------------------------------------*\
    | Copy in location (size+data)                              |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &location_len, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    strcpy((char *)&data_buf[data_ptr], location.c_str());
    data_ptr += location_len;

    /*---------------------------------------------------------*\
    | Copy in number of modes (data)                            |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &num_modes, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*---------------------------------------------------------*\
    | Copy in active mode (data)                                |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &active_mode, sizeof(active_mode));
    data_ptr += sizeof(active_mode);

    /*---------------------------------------------------------*\
    | Copy in modes                                             |
    \*---------------------------------------------------------*/
    for(int mode_index = 0; mode_index < num_modes; mode_index++)
    {
        /*---------------------------------------------------------*\
        | Copy in mode name (size+data)                             |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &mode_name_len[mode_index], sizeof(unsigned short));
        data_ptr += sizeof(unsigned short);

        strcpy((char *)&data_buf[data_ptr], modes[mode_index].name.c_str());
        data_ptr += mode_name_len[mode_index];

        /*---------------------------------------------------------*\
        | Copy in mode value (data)                                 |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &modes[mode_index].value, sizeof(modes[mode_index].value));
        data_ptr += sizeof(modes[mode_index].value);

        /*---------------------------------------------------------*\
        | Copy in mode flags (data)                                 |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &modes[mode_index].flags, sizeof(modes[mode_index].flags));
        data_ptr += sizeof(modes[mode_index].flags);

        /*---------------------------------------------------------*\
        | Copy in mode speed_min (data)                             |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &modes[mode_index].speed_min, sizeof(modes[mode_index].speed_min));
        data_ptr += sizeof(modes[mode_index].speed_min);

        /*---------------------------------------------------------*\
        | Copy in mode speed_max (data)                             |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &modes[mode_index].speed_max, sizeof(modes[mode_index].speed_max));
        data_ptr += sizeof(modes[mode_index].speed_max);

        /*---------------------------------------------------------*\
        | Copy in mode brightness_min and brightness_max (data) if  |
        | protocol 3 or higher                                      |
        \*---------------------------------------------------------*/
        if(protocol_version >= 3)
        {
            memcpy(&data_buf[data_ptr], &modes[mode_index].brightness_min, sizeof(modes[mode_index].brightness_min));
            data_ptr += sizeof(modes[mode_index].brightness_min);

            memcpy(&data_buf[data_ptr], &modes[mode_index].brightness_max, sizeof(modes[mode_index].brightness_max));
            data_ptr += sizeof(modes[mode_index].brightness_max);
        }

        /*---------------------------------------------------------*\
        | Copy in mode colors_min (data)                            |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &modes[mode_index].colors_min, sizeof(modes[mode_index].colors_min));
        data_ptr += sizeof(modes[mode_index].colors_min);

        /*---------------------------------------------------------*\
        | Copy in mode colors_max (data)                            |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &modes[mode_index].colors_max, sizeof(modes[mode_index].colors_max));
        data_ptr += sizeof(modes[mode_index].colors_max);

        /*---------------------------------------------------------*\
        | Copy in mode speed (data)                                 |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &modes[mode_index].speed, sizeof(modes[mode_index].speed));
        data_ptr += sizeof(modes[mode_index].speed);

        /*---------------------------------------------------------*\
        | Copy in mode brightness (data) if protocol 3 or higher    |
        \*---------------------------------------------------------*/
        if(protocol_version >= 3)
        {
            memcpy(&data_buf[data_ptr], &modes[mode_index].brightness, sizeof(modes[mode_index].brightness));
            data_ptr += sizeof(modes[mode_index].brightness);
        }

        /*---------------------------------------------------------*\
        | Copy in mode direction (data)                             |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &modes[mode_index].direction, sizeof(modes[mode_index].direction));
        data_ptr += sizeof(modes[mode_index].direction);

        /*---------------------------------------------------------*\
        | Copy in mode color_mode (data)                            |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &modes[mode_index].color_mode, sizeof(modes[mode_index].color_mode));
        data_ptr += sizeof(modes[mode_index].color_mode);

        /*---------------------------------------------------------*\
        | Copy in mode number of colors                             |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &mode_num_colors[mode_index], sizeof(unsigned short));
        data_ptr += sizeof(unsigned short);

        /*---------------------------------------------------------*\
        | Copy in mode mode colors                                  |
        \*---------------------------------------------------------*/
        for(int color_index = 0; color_index < mode_num_colors[mode_index]; color_index++)
        {
            /*---------------------------------------------------------*\
            | Copy in color (data)                                      |
            \*---------------------------------------------------------*/
            memcpy(&data_buf[data_ptr], &modes[mode_index].colors[color_index], sizeof(modes[mode_index].colors[color_index]));
            data_ptr += sizeof(modes[mode_index].colors[color_index]);
        }
    }

    /*---------------------------------------------------------*\
    | Copy in number of zones (data)                            |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &num_zones, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*---------------------------------------------------------*\
    | Copy in zones                                             |
    \*---------------------------------------------------------*/
    for(int zone_index = 0; zone_index < num_zones; zone_index++)
    {
        /*---------------------------------------------------------*\
        | Copy in zone name (size+data)                             |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &zone_name_len[zone_index], sizeof(unsigned short));
        data_ptr += sizeof(unsigned short);

        strcpy((char *)&data_buf[data_ptr], zones[zone_index].name.c_str());
        data_ptr += zone_name_len[zone_index];

        /*---------------------------------------------------------*\
        | Copy in zone type (data)                                  |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &zones[zone_index].type, sizeof(zones[zone_index].type));
        data_ptr += sizeof(zones[zone_index].type);

        /*---------------------------------------------------------*\
        | Check for resizable effects-only zone.  For protocol      |
        | versions that do not support this feature, we have to     |
        | overwrite the leds_min/max/count parameters to 1 so that  |
        | the zone appears a fixed size to older clients.           |
        \*---------------------------------------------------------*/
        if((zones[zone_index].flags & ZONE_FLAG_RESIZE_EFFECTS_ONLY) && (protocol_version < 5))
        {
            /*---------------------------------------------------------*\
            | Create a temporary variable to hold the fixed value of 1  |
            \*---------------------------------------------------------*/
            unsigned int tmp_size = 1;

            /*---------------------------------------------------------*\
            | Copy in temporary minimum LED count (data)                |
            \*---------------------------------------------------------*/
            memcpy(&data_buf[data_ptr], &tmp_size, sizeof(tmp_size));
            data_ptr += sizeof(tmp_size);

            /*---------------------------------------------------------*\
            | Copy in temporary maximum LED count (data)                |
            \*---------------------------------------------------------*/
            memcpy(&data_buf[data_ptr], &tmp_size, sizeof(tmp_size));
            data_ptr += sizeof(tmp_size);

            /*---------------------------------------------------------*\
            | Copy in temporary LED count (data)                        |
            \*---------------------------------------------------------*/
            memcpy(&data_buf[data_ptr], &tmp_size, sizeof(tmp_size));
            data_ptr += sizeof(tmp_size);
        }
        else
        {
            /*---------------------------------------------------------*\
            | Copy in zone minimum LED count (data)                     |
            \*---------------------------------------------------------*/
            memcpy(&data_buf[data_ptr], &zones[zone_index].leds_min, sizeof(zones[zone_index].leds_min));
            data_ptr += sizeof(zones[zone_index].leds_min);

            /*---------------------------------------------------------*\
            | Copy in zone maximum LED count (data)                     |
            \*---------------------------------------------------------*/
            memcpy(&data_buf[data_ptr], &zones[zone_index].leds_max, sizeof(zones[zone_index].leds_max));
            data_ptr += sizeof(zones[zone_index].leds_max);

            /*---------------------------------------------------------*\
            | Copy in zone LED count (data)                             |
            \*---------------------------------------------------------*/
            memcpy(&data_buf[data_ptr], &zones[zone_index].leds_count, sizeof(zones[zone_index].leds_count));
            data_ptr += sizeof(zones[zone_index].leds_count);
        }

        /*---------------------------------------------------------*\
        | Copy in size of zone matrix                               |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &zone_matrix_len[zone_index], sizeof(zone_matrix_len[zone_index]));
        data_ptr += sizeof(zone_matrix_len[zone_index]);

        /*---------------------------------------------------------*\
        | Copy in matrix data if size is nonzero                    |
        \*---------------------------------------------------------*/
        if(zone_matrix_len[zone_index] > 0)
        {
            /*---------------------------------------------------------*\
            | Copy in matrix height                                     |
            \*---------------------------------------------------------*/
            memcpy(&data_buf[data_ptr], &zones[zone_index].matrix_map->height, sizeof(zones[zone_index].matrix_map->height));
            data_ptr += sizeof(zones[zone_index].matrix_map->height);

            /*---------------------------------------------------------*\
            | Copy in matrix width                                      |
            \*---------------------------------------------------------*/
            memcpy(&data_buf[data_ptr], &zones[zone_index].matrix_map->width, sizeof(zones[zone_index].matrix_map->width));
            data_ptr += sizeof(zones[zone_index].matrix_map->width);

            /*---------------------------------------------------------*\
            | Copy in matrix map                                        |
            \*---------------------------------------------------------*/
            for(unsigned int matrix_idx = 0; matrix_idx < (zones[zone_index].matrix_map->height * zones[zone_index].matrix_map->width); matrix_idx++)
            {
                memcpy(&data_buf[data_ptr], &zones[zone_index].matrix_map->map[matrix_idx], sizeof(zones[zone_index].matrix_map->map[matrix_idx]));
                data_ptr += sizeof(zones[zone_index].matrix_map->map[matrix_idx]);
            }
        }

        /*---------------------------------------------------------*\
        | Copy in segments                                          |
        \*---------------------------------------------------------*/
        if(protocol_version >= 4)
        {
            unsigned short num_segments = (unsigned short)zones[zone_index].segments.size();

            /*---------------------------------------------------------*\
            | Number of segments in zone                                |
            \*---------------------------------------------------------*/
            memcpy(&data_buf[data_ptr], &num_segments, sizeof(num_segments));
            data_ptr += sizeof(num_segments);

            for(int segment_index = 0; segment_index < num_segments; segment_index++)
            {
                /*---------------------------------------------------------*\
                | Length of segment name string                             |
                \*---------------------------------------------------------*/
                unsigned short segment_name_length = (unsigned short)strlen(zones[zone_index].segments[segment_index].name.c_str()) + 1;

                memcpy(&data_buf[data_ptr], &segment_name_length, sizeof(segment_name_length));
                data_ptr += sizeof(segment_name_length);

                /*---------------------------------------------------------*\
                | Segment name string data                                  |
                \*---------------------------------------------------------*/
                strcpy((char *)&data_buf[data_ptr], zones[zone_index].segments[segment_index].name.c_str());
                data_ptr += segment_name_length;

                /*---------------------------------------------------------*\
                | Segment type data                                         |
                \*---------------------------------------------------------*/
                memcpy(&data_buf[data_ptr], &zones[zone_index].segments[segment_index].type, sizeof(zones[zone_index].segments[segment_index].type));
                data_ptr += sizeof(zones[zone_index].segments[segment_index].type);

                /*---------------------------------------------------------*\
                | Segment start index data                                  |
                \*---------------------------------------------------------*/
                memcpy(&data_buf[data_ptr], &zones[zone_index].segments[segment_index].start_idx, sizeof(zones[zone_index].segments[segment_index].start_idx));
                data_ptr += sizeof(zones[zone_index].segments[segment_index].start_idx);

                /*---------------------------------------------------------*\
                | Segment LED count data                                    |
                \*---------------------------------------------------------*/
                memcpy(&data_buf[data_ptr], &zones[zone_index].segments[segment_index].leds_count, sizeof(zones[zone_index].segments[segment_index].leds_count));
                data_ptr += sizeof(zones[zone_index].segments[segment_index].leds_count);
            }
        }

        /*---------------------------------------------------------*\
        | Copy in zone flags                                        |
        \*---------------------------------------------------------*/
        if(protocol_version >= 5)
        {
            /*---------------------------------------------------------*\
            | Zone flags                                                |
            \*---------------------------------------------------------*/
            memcpy(&data_buf[data_ptr], &zones[zone_index].flags, sizeof(zones[zone_index].flags));
            data_ptr += sizeof(zones[zone_index].flags);
        }
    }

    /*---------------------------------------------------------*\
    | Copy in number of LEDs (data)                             |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &num_leds, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*---------------------------------------------------------*\
    | Copy in LEDs                                              |
    \*---------------------------------------------------------*/
    for(int led_index = 0; led_index < num_leds; led_index++)
    {
        /*---------------------------------------------------------*\
        | Copy in LED name (size+data)                              |
        \*---------------------------------------------------------*/
        unsigned short ledname_len = (unsigned short)strlen(leds[led_index].name.c_str()) + 1;
        memcpy(&data_buf[data_ptr], &ledname_len, sizeof(unsigned short));
        data_ptr += sizeof(unsigned short);

        strcpy((char *)&data_buf[data_ptr], leds[led_index].name.c_str());
        data_ptr += ledname_len;

        /*---------------------------------------------------------*\
        | Copy in LED value (data)                                  |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &leds[led_index].value, sizeof(leds[led_index].value));
        data_ptr += sizeof(leds[led_index].value);
    }

    /*---------------------------------------------------------*\
    | Copy in number of colors (data)                           |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &num_colors, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*---------------------------------------------------------*\
    | Copy in colors                                            |
    \*---------------------------------------------------------*/
    for(int color_index = 0; color_index < num_colors; color_index++)
    {
        /*---------------------------------------------------------*\
        | Copy in color (data)                                      |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &colors[color_index], sizeof(colors[color_index]));
        data_ptr += sizeof(colors[color_index]);
    }

    /*---------------------------------------------------------*\
    | LED alternate names data                                  |
    \*---------------------------------------------------------*/
    if(protocol_version >= 5)
    {
        /*---------------------------------------------------------*\
        | Number of LED alternate name strings                      |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &num_led_alt_names, sizeof(num_led_alt_names));
        data_ptr += sizeof(num_led_alt_names);

        for(std::size_t led_idx = 0; led_idx < led_alt_names.size(); led_idx++)
        {
            /*---------------------------------------------------------*\
            | Copy in LED alternate name (size+data)                    |
            \*---------------------------------------------------------*/
            unsigned short string_length = (unsigned short)strlen(led_alt_names[led_idx].c_str()) + 1;

            memcpy(&data_buf[data_ptr], &string_length, sizeof(string_length));
            data_ptr += sizeof(string_length);

            strcpy((char *)&data_buf[data_ptr], led_alt_names[led_idx].c_str());
            data_ptr += string_length;
        }
    }

    /*---------------------------------------------------------*\
    | Controller flags data                                     |
    \*---------------------------------------------------------*/
    if(protocol_version >= 5)
    {
        memcpy(&data_buf[data_ptr], &flags, sizeof(flags));
        data_ptr += sizeof(flags);
    }

    delete[] mode_name_len;
    delete[] zone_name_len;
    delete[] led_name_len;

    delete[] zone_matrix_len;
    delete[] mode_num_colors;

    return(data_buf);
}

void RGBController::ReadDeviceDescription(unsigned char* data_buf, unsigned int protocol_version)
{
    unsigned int data_ptr = 0;

    data_ptr += sizeof(unsigned int);

    /*---------------------------------------------------------*\
    | Copy in type                                              |
    \*---------------------------------------------------------*/
    memcpy(&type, &data_buf[data_ptr], sizeof(device_type));
    data_ptr += sizeof(device_type);

    /*---------------------------------------------------------*\
    | Copy in name                                              |
    \*---------------------------------------------------------*/
    unsigned short name_len;
    memcpy(&name_len, &data_buf[data_ptr], sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    name = (char *)&data_buf[data_ptr];
    data_ptr += name_len;

    /*---------------------------------------------------------*\
    | Copy in vendor if protocol version is 1 or higher         |
    \*---------------------------------------------------------*/
    if(protocol_version >= 1)
    {
        unsigned short vendor_len;
        memcpy(&vendor_len, &data_buf[data_ptr], sizeof(unsigned short));
        data_ptr += sizeof(unsigned short);

        vendor = (char *)&data_buf[data_ptr];
        data_ptr += vendor_len;
    }

    /*---------------------------------------------------------*\
    | Copy in description                                       |
    \*---------------------------------------------------------*/
    unsigned short description_len;
    memcpy(&description_len, &data_buf[data_ptr], sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    description = (char *)&data_buf[data_ptr];
    data_ptr += description_len;

    /*---------------------------------------------------------*\
    | Copy in version                                           |
    \*---------------------------------------------------------*/
    unsigned short version_len;
    memcpy(&version_len, &data_buf[data_ptr], sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    version = (char *)&data_buf[data_ptr];
    data_ptr += version_len;

    /*---------------------------------------------------------*\
    | Copy in serial                                            |
    \*---------------------------------------------------------*/
    unsigned short serial_len;
    memcpy(&serial_len, &data_buf[data_ptr], sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    serial = (char *)&data_buf[data_ptr];
    data_ptr += serial_len;

    /*---------------------------------------------------------*\
    | Copy in location                                          |
    \*---------------------------------------------------------*/
    unsigned short location_len;
    memcpy(&location_len, &data_buf[data_ptr], sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    location = (char *)&data_buf[data_ptr];
    data_ptr += location_len;

    /*---------------------------------------------------------*\
    | Copy in number of modes (data)                            |
    \*---------------------------------------------------------*/
    unsigned short num_modes;
    memcpy(&num_modes, &data_buf[data_ptr], sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*---------------------------------------------------------*\
    | Copy in active mode (data)                                |
    \*---------------------------------------------------------*/
    memcpy(&active_mode, &data_buf[data_ptr], sizeof(active_mode));
    data_ptr += sizeof(active_mode);

    /*---------------------------------------------------------*\
    | Copy in modes                                             |
    \*---------------------------------------------------------*/
    for(int mode_index = 0; mode_index < num_modes; mode_index++)
    {
        mode new_mode;

        /*---------------------------------------------------------*\
        | Copy in mode name (size+data)                             |
        \*---------------------------------------------------------*/
        unsigned short modename_len;
        memcpy(&modename_len, &data_buf[data_ptr], sizeof(unsigned short));
        data_ptr += sizeof(unsigned short);

        new_mode.name = (char *)&data_buf[data_ptr];
        data_ptr += modename_len;

        /*---------------------------------------------------------*\
        | Copy in mode value (data)                                 |
        \*---------------------------------------------------------*/
        memcpy(&new_mode.value, &data_buf[data_ptr], sizeof(new_mode.value));
        data_ptr += sizeof(new_mode.value);

        /*---------------------------------------------------------*\
        | Copy in mode flags (data)                                 |
        \*---------------------------------------------------------*/
        memcpy(&new_mode.flags, &data_buf[data_ptr], sizeof(new_mode.flags));
        data_ptr += sizeof(new_mode.flags);

        /*---------------------------------------------------------*\
        | Copy in mode speed_min (data)                             |
        \*---------------------------------------------------------*/
        memcpy(&new_mode.speed_min, &data_buf[data_ptr], sizeof(new_mode.speed_min));
        data_ptr += sizeof(new_mode.speed_min);

        /*---------------------------------------------------------*\
        | Copy in mode speed_max (data)                             |
        \*---------------------------------------------------------*/
        memcpy(&new_mode.speed_max, &data_buf[data_ptr], sizeof(new_mode.speed_max));
        data_ptr += sizeof(new_mode.speed_max);

        /*---------------------------------------------------------*\
        | Copy in mode brightness min and max if protocol version   |
        | is 3 or higher                                            |
        \*---------------------------------------------------------*/
        if(protocol_version >= 3)
        {
            memcpy(&new_mode.brightness_min, &data_buf[data_ptr], sizeof(new_mode.brightness_min));
            data_ptr += sizeof(new_mode.brightness_min);

            memcpy(&new_mode.brightness_max, &data_buf[data_ptr], sizeof(new_mode.brightness_max));
            data_ptr += sizeof(new_mode.brightness_max);
        }

        /*---------------------------------------------------------*\
        | Copy in mode colors_min (data)                            |
        \*---------------------------------------------------------*/
        memcpy(&new_mode.colors_min, &data_buf[data_ptr], sizeof(new_mode.colors_min));
        data_ptr += sizeof(new_mode.colors_min);

        /*---------------------------------------------------------*\
        | Copy in mode colors_max (data)                            |
        \*---------------------------------------------------------*/
        memcpy(&new_mode.colors_max, &data_buf[data_ptr], sizeof(new_mode.colors_max));
        data_ptr += sizeof(new_mode.colors_max);

        /*---------------------------------------------------------*\
        | Copy in mode speed (data)                                 |
        \*---------------------------------------------------------*/
        memcpy(&new_mode.speed, &data_buf[data_ptr], sizeof(new_mode.speed));
        data_ptr += sizeof(new_mode.speed);

        /*---------------------------------------------------------*\
        | Copy in mode brightness if protocol version is 3 or higher|
        \*---------------------------------------------------------*/
        if(protocol_version >= 3)
        {
            memcpy(&new_mode.brightness, &data_buf[data_ptr], sizeof(new_mode.brightness));
            data_ptr += sizeof(new_mode.brightness);
        }

        /*---------------------------------------------------------*\
        | Copy in mode direction (data)                             |
        \*---------------------------------------------------------*/
        memcpy(&new_mode.direction, &data_buf[data_ptr], sizeof(new_mode.direction));
        data_ptr += sizeof(new_mode.direction);

        /*---------------------------------------------------------*\
        | Copy in mode color_mode (data)                            |
        \*---------------------------------------------------------*/
        memcpy(&new_mode.color_mode, &data_buf[data_ptr], sizeof(new_mode.color_mode));
        data_ptr += sizeof(new_mode.color_mode);

        /*---------------------------------------------------------*\
        | Copy in mode number of colors                             |
        \*---------------------------------------------------------*/
        unsigned short mode_num_colors;
        memcpy(&mode_num_colors, &data_buf[data_ptr], sizeof(unsigned short));
        data_ptr += sizeof(unsigned short);

        /*---------------------------------------------------------*\
        | Copy in mode mode colors                                  |
        \*---------------------------------------------------------*/
        for(int color_index = 0; color_index < mode_num_colors; color_index++)
        {
            /*---------------------------------------------------------*\
            | Copy in color (data)                                      |
            \*---------------------------------------------------------*/
            RGBColor new_color;
            memcpy(&new_color, &data_buf[data_ptr], sizeof(RGBColor));
            data_ptr += sizeof(modes[mode_index].colors[color_index]);

            new_mode.colors.push_back(new_color);
        }

        modes.push_back(new_mode);
    }

    /*---------------------------------------------------------*\
    | Copy in number of zones (data)                            |
    \*---------------------------------------------------------*/
    unsigned short num_zones;
    memcpy(&num_zones, &data_buf[data_ptr], sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*---------------------------------------------------------*\
    | Copy in zones                                             |
    \*---------------------------------------------------------*/
    for(int zone_index = 0; zone_index < num_zones; zone_index++)
    {
        zone new_zone;

        /*---------------------------------------------------------*\
        | Copy in zone name (size+data)                             |
        \*---------------------------------------------------------*/
        unsigned short zonename_len;
        memcpy(&zonename_len, &data_buf[data_ptr], sizeof(unsigned short));
        data_ptr += sizeof(unsigned short);

        new_zone.name = (char *)&data_buf[data_ptr];
        data_ptr += zonename_len;

        /*---------------------------------------------------------*\
        | Copy in zone type (data)                                  |
        \*---------------------------------------------------------*/
        memcpy(&new_zone.type, &data_buf[data_ptr], sizeof(new_zone.type));
        data_ptr += sizeof(new_zone.type);

        /*---------------------------------------------------------*\
        | Copy in zone minimum LED count (data)                     |
        \*---------------------------------------------------------*/
        memcpy(&new_zone.leds_min, &data_buf[data_ptr], sizeof(new_zone.leds_min));
        data_ptr += sizeof(new_zone.leds_min);

        /*---------------------------------------------------------*\
        | Copy in zone maximum LED count (data)                     |
        \*---------------------------------------------------------*/
        memcpy(&new_zone.leds_max, &data_buf[data_ptr], sizeof(new_zone.leds_max));
        data_ptr += sizeof(new_zone.leds_max);

        /*---------------------------------------------------------*\
        | Copy in zone LED count (data)                             |
        \*---------------------------------------------------------*/
        memcpy(&new_zone.leds_count, &data_buf[data_ptr], sizeof(new_zone.leds_count));
        data_ptr += sizeof(new_zone.leds_count);

        /*---------------------------------------------------------*\
        | Copy in size of zone matrix                               |
        \*---------------------------------------------------------*/
        unsigned short zone_matrix_len;
        memcpy(&zone_matrix_len, &data_buf[data_ptr], sizeof(zone_matrix_len));
        data_ptr += sizeof(zone_matrix_len);

        /*---------------------------------------------------------*\
        | Copy in matrix data if size is nonzero                    |
        \*---------------------------------------------------------*/
        if(zone_matrix_len > 0)
        {
            /*---------------------------------------------------------*\
            | Create a map data structure to fill in and attach it to   |
            | the new zone                                              |
            \*---------------------------------------------------------*/
            matrix_map_type * new_map = new matrix_map_type;

            new_zone.matrix_map = new_map;

            /*---------------------------------------------------------*\
            | Copy in matrix height                                     |
            \*---------------------------------------------------------*/
            memcpy(&new_map->height, &data_buf[data_ptr], sizeof(new_map->height));
            data_ptr += sizeof(new_map->height);

            /*---------------------------------------------------------*\
            | Copy in matrix width                                      |
            \*---------------------------------------------------------*/
            memcpy(&new_map->width, &data_buf[data_ptr], sizeof(new_map->width));
            data_ptr += sizeof(new_map->width);

            /*---------------------------------------------------------*\
            | Copy in matrix map                                        |
            \*---------------------------------------------------------*/
            new_map->map = new unsigned int[new_map->height * new_map->width];

            for(unsigned int matrix_idx = 0; matrix_idx < (new_map->height * new_map->width); matrix_idx++)
            {
                memcpy(&new_map->map[matrix_idx], &data_buf[data_ptr], sizeof(new_map->map[matrix_idx]));
                data_ptr += sizeof(new_map->map[matrix_idx]);
            }
        }
        else
        {
            new_zone.matrix_map = NULL;
        }

        /*---------------------------------------------------------*\
        | Copy in segments                                          |
        \*---------------------------------------------------------*/
        if(protocol_version >= 4)
        {
            unsigned short num_segments = 0;

            /*---------------------------------------------------------*\
            | Number of segments in zone                                |
            \*---------------------------------------------------------*/
            memcpy(&num_segments, &data_buf[data_ptr], sizeof(num_segments));
            data_ptr += sizeof(num_segments);

            for(int segment_index = 0; segment_index < num_segments; segment_index++)
            {
                segment new_segment;

                /*---------------------------------------------------------*\
                | Copy in segment name (size+data)                          |
                \*---------------------------------------------------------*/
                unsigned short segmentname_len;
                memcpy(&segmentname_len, &data_buf[data_ptr], sizeof(unsigned short));
                data_ptr += sizeof(unsigned short);

                new_segment.name = (char *)&data_buf[data_ptr];
                data_ptr += segmentname_len;

                /*---------------------------------------------------------*\
                | Segment type data                                         |
                \*---------------------------------------------------------*/
                memcpy(&new_segment.type, &data_buf[data_ptr], sizeof(new_segment.type));
                data_ptr += sizeof(new_segment.type);

                /*---------------------------------------------------------*\
                | Segment start index data                                  |
                \*---------------------------------------------------------*/
                memcpy(&new_segment.start_idx, &data_buf[data_ptr], sizeof(new_segment.start_idx));
                data_ptr += sizeof(new_segment.start_idx);

                /*---------------------------------------------------------*\
                | Segment LED count data                                    |
                \*---------------------------------------------------------*/
                memcpy(&new_segment.leds_count, &data_buf[data_ptr], sizeof(new_segment.leds_count));
                data_ptr += sizeof(new_segment.leds_count);

                new_zone.segments.push_back(new_segment);
            }
        }

        /*---------------------------------------------------------*\
        | Copy in zone flags                                        |
        \*---------------------------------------------------------*/
        if(protocol_version >= 5)
        {
            memcpy(&new_zone.flags, &data_buf[data_ptr], sizeof(new_zone.flags));
            data_ptr += sizeof(new_zone.flags);
        }

        zones.push_back(new_zone);
    }

    /*---------------------------------------------------------*\
    | Copy in number of LEDs (data)                             |
    \*---------------------------------------------------------*/
    unsigned short num_leds;
    memcpy(&num_leds, &data_buf[data_ptr], sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*---------------------------------------------------------*\
    | Copy in LEDs                                              |
    \*---------------------------------------------------------*/
    for(int led_index = 0; led_index < num_leds; led_index++)
    {
        led new_led;

        /*---------------------------------------------------------*\
        | Copy in LED name (size+data)                              |
        \*---------------------------------------------------------*/
        unsigned short ledname_len;
        memcpy(&ledname_len, &data_buf[data_ptr], sizeof(unsigned short));
        data_ptr += sizeof(unsigned short);

        new_led.name = (char *)&data_buf[data_ptr];
        data_ptr += ledname_len;

        /*---------------------------------------------------------*\
        | Copy in LED value (data)                                  |
        \*---------------------------------------------------------*/
        memcpy(&new_led.value, &data_buf[data_ptr], sizeof(new_led.value));
        data_ptr += sizeof(new_led.value);

        leds.push_back(new_led);
    }

    /*---------------------------------------------------------*\
    | Copy in number of colors (data)                           |
    \*---------------------------------------------------------*/
    unsigned short num_colors;
    memcpy(&num_colors, &data_buf[data_ptr], sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*---------------------------------------------------------*\
    | Copy in colors                                            |
    \*---------------------------------------------------------*/
    for(int color_index = 0; color_index < num_colors; color_index++)
    {
        RGBColor new_color;

        /*---------------------------------------------------------*\
        | Copy in color (data)                                      |
        \*---------------------------------------------------------*/
        memcpy(&new_color, &data_buf[data_ptr], sizeof(RGBColor));
        data_ptr += sizeof(RGBColor);

        colors.push_back(new_color);
    }

    /*---------------------------------------------------------*\
    | Copy in LED alternate names data                          |
    \*---------------------------------------------------------*/
    if(protocol_version >= 5)
    {
        /*---------------------------------------------------------*\
        | Copy in number of LED alternate names                     |
        \*---------------------------------------------------------*/
        unsigned short num_led_alt_names;

        memcpy(&num_led_alt_names, &data_buf[data_ptr], sizeof(num_led_alt_names));
        data_ptr += sizeof(num_led_alt_names);

        for(int led_idx = 0; led_idx < num_led_alt_names; led_idx++)
        {
            unsigned short string_length = 0;

            /*---------------------------------------------------------*\
            | Copy in LED alternate name string (size+data)             |
            \*---------------------------------------------------------*/
            memcpy(&string_length, &data_buf[data_ptr], sizeof(string_length));
            data_ptr += sizeof(string_length);

            led_alt_names.push_back((char *)&data_buf[data_ptr]);
            data_ptr += string_length;
        }
    }

    /*---------------------------------------------------------*\
    | Copy in controller flags data                             |
    \*---------------------------------------------------------*/
    if(protocol_version >= 5)
    {
        memcpy(&flags, &data_buf[data_ptr], sizeof(flags));
        data_ptr += sizeof(flags);
    }

    /*---------------------------------------------------------*\
    | Setup colors                                              |
    \*---------------------------------------------------------*/
    SetupColors();
}

unsigned char * RGBController::GetModeDescription(int mode, unsigned int protocol_version)
{
    unsigned int data_ptr = 0;
    unsigned int data_size = 0;

    unsigned short mode_name_len;
    unsigned short mode_num_colors;

    /*---------------------------------------------------------*\
    | Calculate data size                                       |
    \*---------------------------------------------------------*/
    mode_name_len   = (unsigned short)strlen(modes[mode].name.c_str()) + 1;
    mode_num_colors = (unsigned short)modes[mode].colors.size();

    data_size += sizeof(data_size);
    data_size += sizeof(mode);
    data_size += sizeof(mode_name_len);
    data_size += mode_name_len;
    data_size += sizeof(modes[mode].value);
    data_size += sizeof(modes[mode].flags);
    data_size += sizeof(modes[mode].speed_min);
    data_size += sizeof(modes[mode].speed_max);
    if(protocol_version >= 3)
    {
        data_size += sizeof(modes[mode].brightness_min);
        data_size += sizeof(modes[mode].brightness_max);
    }
    data_size += sizeof(modes[mode].colors_min);
    data_size += sizeof(modes[mode].colors_max);
    data_size += sizeof(modes[mode].speed);
    if(protocol_version >= 3)
    {
        data_size += sizeof(modes[mode].brightness);
    }
    data_size += sizeof(modes[mode].direction);
    data_size += sizeof(modes[mode].color_mode);
    data_size += sizeof(mode_num_colors);
    data_size += (mode_num_colors * sizeof(RGBColor));

    /*---------------------------------------------------------*\
    | Create data buffer                                        |
    \*---------------------------------------------------------*/
    unsigned char *data_buf = new unsigned char[data_size];

    /*---------------------------------------------------------*\
    | Copy in data size                                         |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &data_size, sizeof(data_size));
    data_ptr += sizeof(data_size);

    /*---------------------------------------------------------*\
    | Copy in mode index                                        |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &mode, sizeof(int));
    data_ptr += sizeof(int);

    /*---------------------------------------------------------*\
    | Copy in mode name (size+data)                             |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &mode_name_len, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    strcpy((char *)&data_buf[data_ptr], modes[mode].name.c_str());
    data_ptr += mode_name_len;

    /*---------------------------------------------------------*\
    | Copy in mode value (data)                                 |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &modes[mode].value, sizeof(modes[mode].value));
    data_ptr += sizeof(modes[mode].value);

    /*---------------------------------------------------------*\
    | Copy in mode flags (data)                                 |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &modes[mode].flags, sizeof(modes[mode].flags));
    data_ptr += sizeof(modes[mode].flags);

    /*---------------------------------------------------------*\
    | Copy in mode speed_min (data)                             |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &modes[mode].speed_min, sizeof(modes[mode].speed_min));
    data_ptr += sizeof(modes[mode].speed_min);

    /*---------------------------------------------------------*\
    | Copy in mode speed_max (data)                             |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &modes[mode].speed_max, sizeof(modes[mode].speed_max));
    data_ptr += sizeof(modes[mode].speed_max);

    /*---------------------------------------------------------*\
    | Copy in mode brightness min and max if protocol version   |
    | is 3 or higher                                            |
    \*---------------------------------------------------------*/
    if(protocol_version >= 3)
    {
        memcpy(&data_buf[data_ptr], &modes[mode].brightness_min, sizeof(modes[mode].brightness_min));
        data_ptr += sizeof(modes[mode].brightness_min);

        memcpy(&data_buf[data_ptr], &modes[mode].brightness_max, sizeof(modes[mode].brightness_max));
        data_ptr += sizeof(modes[mode].brightness_max);
    }

    /*---------------------------------------------------------*\
    | Copy in mode colors_min (data)                            |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &modes[mode].colors_min, sizeof(modes[mode].colors_min));
    data_ptr += sizeof(modes[mode].colors_min);

    /*---------------------------------------------------------*\
    | Copy in mode colors_max (data)                            |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &modes[mode].colors_max, sizeof(modes[mode].colors_max));
    data_ptr += sizeof(modes[mode].colors_max);

    /*---------------------------------------------------------*\
    | Copy in mode speed (data)                                 |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &modes[mode].speed, sizeof(modes[mode].speed));
    data_ptr += sizeof(modes[mode].speed);

    /*---------------------------------------------------------*\
    | Copy in mode brightness if protocol version is 3 or higher|
    \*---------------------------------------------------------*/
    if(protocol_version >= 3)
    {
        memcpy(&data_buf[data_ptr], &modes[mode].brightness, sizeof(modes[mode].brightness));
        data_ptr += sizeof(modes[mode].brightness);
    }

    /*---------------------------------------------------------*\
    | Copy in mode direction (data)                             |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &modes[mode].direction, sizeof(modes[mode].direction));
    data_ptr += sizeof(modes[mode].direction);

    /*---------------------------------------------------------*\
    | Copy in mode color_mode (data)                            |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &modes[mode].color_mode, sizeof(modes[mode].color_mode));
    data_ptr += sizeof(modes[mode].color_mode);

    /*---------------------------------------------------------*\
    | Copy in mode number of colors                             |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &mode_num_colors, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*---------------------------------------------------------*\
    | Copy in mode mode colors                                  |
    \*---------------------------------------------------------*/
    for(int color_index = 0; color_index < mode_num_colors; color_index++)
    {
        /*---------------------------------------------------------*\
        | Copy in color (data)                                      |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &modes[mode].colors[color_index], sizeof(modes[mode].colors[color_index]));
        data_ptr += sizeof(modes[mode].colors[color_index]);
    }

    return(data_buf);
}

void RGBController::SetModeDescription(unsigned char* data_buf, unsigned int protocol_version)
{
    int mode_idx;
    unsigned int data_ptr = sizeof(unsigned int);

    /*---------------------------------------------------------*\
    | Copy in mode index                                        |
    \*---------------------------------------------------------*/
    memcpy(&mode_idx, &data_buf[data_ptr], sizeof(int));
    data_ptr += sizeof(int);

    /*---------------------------------------------------------*\
    | Check if we aren't reading beyond the list of modes.      |
    \*---------------------------------------------------------*/
    if(((size_t) mode_idx) >  modes.size())
    {
        return;
    }

    /*---------------------------------------------------------*\
    | Get pointer to target mode                                |
    \*---------------------------------------------------------*/
    mode * new_mode = &modes[mode_idx];

    /*---------------------------------------------------------*\
    | Set active mode to the new mode                           |
    \*---------------------------------------------------------*/
    active_mode = mode_idx;

    /*---------------------------------------------------------*\
    | Copy in mode name (size+data)                             |
    \*---------------------------------------------------------*/
    unsigned short modename_len;
    memcpy(&modename_len, &data_buf[data_ptr], sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    new_mode->name = (char *)&data_buf[data_ptr];
    data_ptr += modename_len;

    /*---------------------------------------------------------*\
    | Copy in mode value (data)                                 |
    \*---------------------------------------------------------*/
    memcpy(&new_mode->value, &data_buf[data_ptr], sizeof(new_mode->value));
    data_ptr += sizeof(new_mode->value);

    /*---------------------------------------------------------*\
    | Copy in mode flags (data)                                 |
    \*---------------------------------------------------------*/
    memcpy(&new_mode->flags, &data_buf[data_ptr], sizeof(new_mode->flags));
    data_ptr += sizeof(new_mode->flags);

    /*---------------------------------------------------------*\
    | Copy in mode speed_min (data)                             |
    \*---------------------------------------------------------*/
    memcpy(&new_mode->speed_min, &data_buf[data_ptr], sizeof(new_mode->speed_min));
    data_ptr += sizeof(new_mode->speed_min);

    /*---------------------------------------------------------*\
    | Copy in mode speed_max (data)                             |
    \*---------------------------------------------------------*/
    memcpy(&new_mode->speed_max, &data_buf[data_ptr], sizeof(new_mode->speed_max));
    data_ptr += sizeof(new_mode->speed_max);

    /*---------------------------------------------------------*\
    | Copy in mode brightness_min and brightness_max (data) if  |
    | protocol 3 or higher                                      |
    \*---------------------------------------------------------*/
    if(protocol_version >= 3)
    {
        memcpy(&new_mode->brightness_min, &data_buf[data_ptr], sizeof(new_mode->brightness_min));
        data_ptr += sizeof(new_mode->brightness_min);

        memcpy(&new_mode->brightness_max, &data_buf[data_ptr], sizeof(new_mode->brightness_max));
        data_ptr += sizeof(new_mode->brightness_max);
    }

    /*---------------------------------------------------------*\
    | Copy in mode colors_min (data)                            |
    \*---------------------------------------------------------*/
    memcpy(&new_mode->colors_min, &data_buf[data_ptr], sizeof(new_mode->colors_min));
    data_ptr += sizeof(new_mode->colors_min);

    /*---------------------------------------------------------*\
    | Copy in mode colors_max (data)                            |
    \*---------------------------------------------------------*/
    memcpy(&new_mode->colors_max, &data_buf[data_ptr], sizeof(new_mode->colors_max));
    data_ptr += sizeof(new_mode->colors_max);

    /*---------------------------------------------------------*\
    | Copy in mode speed (data)                                 |
    \*---------------------------------------------------------*/
    memcpy(&new_mode->speed, &data_buf[data_ptr], sizeof(new_mode->speed));
    data_ptr += sizeof(new_mode->speed);

    /*---------------------------------------------------------*\
    | Copy in mode brightness (data) if protocol 3 or higher    |
    \*---------------------------------------------------------*/
    if(protocol_version >= 3)
    {
        memcpy(&new_mode->brightness, &data_buf[data_ptr], sizeof(new_mode->brightness));
        data_ptr += sizeof(new_mode->brightness);
    }

    /*---------------------------------------------------------*\
    | Copy in mode direction (data)                             |
    \*---------------------------------------------------------*/
    memcpy(&new_mode->direction, &data_buf[data_ptr], sizeof(new_mode->direction));
    data_ptr += sizeof(new_mode->direction);

    /*---------------------------------------------------------*\
    | Copy in mode color_mode (data)                            |
    \*---------------------------------------------------------*/
    memcpy(&new_mode->color_mode, &data_buf[data_ptr], sizeof(new_mode->color_mode));
    data_ptr += sizeof(new_mode->color_mode);

    /*---------------------------------------------------------*\
    | Copy in mode number of colors                             |
    \*---------------------------------------------------------*/
    unsigned short mode_num_colors;
    memcpy(&mode_num_colors, &data_buf[data_ptr], sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*---------------------------------------------------------*\
    | Copy in mode mode colors                                  |
    \*---------------------------------------------------------*/
    new_mode->colors.clear();
    for(int color_index = 0; color_index < mode_num_colors; color_index++)
    {
        /*---------------------------------------------------------*\
        | Copy in color (data)                                      |
        \*---------------------------------------------------------*/
        RGBColor new_color;
        memcpy(&new_color, &data_buf[data_ptr], sizeof(RGBColor));
        data_ptr += sizeof(RGBColor);

        new_mode->colors.push_back(new_color);
    }
}

unsigned char * RGBController::GetColorDescription()
{
    unsigned int data_ptr = 0;
    unsigned int data_size = 0;

    unsigned short num_colors = (unsigned short)colors.size();

    /*---------------------------------------------------------*\
    | Calculate data size                                       |
    \*---------------------------------------------------------*/
    data_size += sizeof(data_size);
    data_size += sizeof(num_colors);
    data_size += num_colors * sizeof(RGBColor);

    /*---------------------------------------------------------*\
    | Create data buffer                                        |
    \*---------------------------------------------------------*/
    unsigned char *data_buf = new unsigned char[data_size];

    /*---------------------------------------------------------*\
    | Copy in data size                                         |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &data_size, sizeof(data_size));
    data_ptr += sizeof(data_size);

    /*---------------------------------------------------------*\
    | Copy in number of colors (data)                           |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &num_colors, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*---------------------------------------------------------*\
    | Copy in colors                                            |
    \*---------------------------------------------------------*/
    for(int color_index = 0; color_index < num_colors; color_index++)
    {
        /*---------------------------------------------------------*\
        | Copy in color (data)                                      |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &colors[color_index], sizeof(colors[color_index]));
        data_ptr += sizeof(colors[color_index]);
    }

    return(data_buf);
}

void RGBController::SetColorDescription(unsigned char* data_buf)
{
    unsigned int data_ptr = sizeof(unsigned int);

    /*---------------------------------------------------------*\
    | Copy in number of colors (data)                           |
    \*---------------------------------------------------------*/
    unsigned short num_colors;
    memcpy(&num_colors, &data_buf[data_ptr], sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*---------------------------------------------------------*\
    | Check if we aren't reading beyond the list of colors.     |
    \*---------------------------------------------------------*/
    if(((size_t)num_colors) > colors.size())
    {
        return;
    }

    /*---------------------------------------------------------*\
    | Copy in colors                                            |
    \*---------------------------------------------------------*/
    for(int color_index = 0; color_index < num_colors; color_index++)
    {
        RGBColor new_color;

        /*---------------------------------------------------------*\
        | Copy in color (data)                                      |
        \*---------------------------------------------------------*/
        memcpy(&new_color, &data_buf[data_ptr], sizeof(RGBColor));
        data_ptr += sizeof(RGBColor);

        colors[color_index] = new_color;
    }
}

unsigned char * RGBController::GetZoneColorDescription(int zone)
{
    unsigned int data_ptr = 0;
    unsigned int data_size = 0;

    unsigned short num_colors = zones[zone].leds_count;

    /*---------------------------------------------------------*\
    | Calculate data size                                       |
    \*---------------------------------------------------------*/
    data_size += sizeof(data_size);
    data_size += sizeof(zone);
    data_size += sizeof(num_colors);
    data_size += num_colors * sizeof(RGBColor);

    /*---------------------------------------------------------*\
    | Create data buffer                                        |
    \*---------------------------------------------------------*/
    unsigned char *data_buf = new unsigned char[data_size];

    /*---------------------------------------------------------*\
    | Copy in data size                                         |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &data_size, sizeof(data_size));
    data_ptr += sizeof(data_size);

    /*---------------------------------------------------------*\
    | Copy in zone index                                        |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &zone, sizeof(zone));
    data_ptr += sizeof(zone);

    /*---------------------------------------------------------*\
    | Copy in number of colors (data)                           |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &num_colors, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*---------------------------------------------------------*\
    | Copy in colors                                            |
    \*---------------------------------------------------------*/
    for(int color_index = 0; color_index < num_colors; color_index++)
    {
        /*---------------------------------------------------------*\
        | Copy in color (data)                                      |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &zones[zone].colors[color_index], sizeof(zones[zone].colors[color_index]));
        data_ptr += sizeof(zones[zone].colors[color_index]);
    }

    return(data_buf);
}

void RGBController::SetZoneColorDescription(unsigned char* data_buf)
{
    unsigned int data_ptr = sizeof(unsigned int);
    unsigned int zone_idx;

    /*---------------------------------------------------------*\
    | Copy in zone index                                        |
    \*---------------------------------------------------------*/
    memcpy(&zone_idx, &data_buf[data_ptr], sizeof(zone_idx));
    data_ptr += sizeof(zone_idx);

    /*---------------------------------------------------------*\
    | Check if we aren't reading beyond the list of zones.      |
    \*---------------------------------------------------------*/
    if(((size_t)zone_idx) > zones.size())
    {
        return;
    }

    /*---------------------------------------------------------*\
    | Copy in number of colors (data)                           |
    \*---------------------------------------------------------*/
    unsigned short num_colors;
    memcpy(&num_colors, &data_buf[data_ptr], sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*---------------------------------------------------------*\
    | Copy in colors                                            |
    \*---------------------------------------------------------*/
    for(int color_index = 0; color_index < num_colors; color_index++)
    {
        RGBColor new_color;

        /*---------------------------------------------------------*\
        | Copy in color (data)                                      |
        \*---------------------------------------------------------*/
        memcpy(&new_color, &data_buf[data_ptr], sizeof(RGBColor));
        data_ptr += sizeof(RGBColor);

        zones[zone_idx].colors[color_index] = new_color;
    }
}

unsigned char * RGBController::GetSingleLEDColorDescription(int led)
{
    /*---------------------------------------------------------*\
    | Fixed size descrption:                                    |
    |       int:      LED index                                 |
    |       RGBColor: LED color                                 |
    \*---------------------------------------------------------*/
    unsigned char *data_buf = new unsigned char[sizeof(int) + sizeof(RGBColor)];

    /*---------------------------------------------------------*\
    | Copy in LED index                                         |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[0], &led, sizeof(int));

    /*---------------------------------------------------------*\
    | Copy in LED color                                         |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[sizeof(led)], &colors[led], sizeof(RGBColor));

    return(data_buf);
}

void RGBController::SetSingleLEDColorDescription(unsigned char* data_buf)
{
    /*---------------------------------------------------------*\
    | Fixed size descrption:                                    |
    |       int:      LED index                                 |
    |       RGBColor: LED color                                 |
    \*---------------------------------------------------------*/
    int led_idx;

    /*---------------------------------------------------------*\
    | Copy in LED index                                         |
    \*---------------------------------------------------------*/
    memcpy(&led_idx, &data_buf[0], sizeof(led_idx));

    /*---------------------------------------------------------*\
    | Check if we aren't reading beyond the list of leds.       |
    \*---------------------------------------------------------*/
    if(((size_t)led_idx) > leds.size())
    {
        return;
    }

    /*---------------------------------------------------------*\
    | Copy in LED color                                         |
    \*---------------------------------------------------------*/
    memcpy(&colors[led_idx], &data_buf[sizeof(led_idx)], sizeof(RGBColor));
}

unsigned char * RGBController::GetSegmentDescription(int zone, segment new_segment)
{
    unsigned int data_ptr = 0;
    unsigned int data_size = 0;

    /*---------------------------------------------------------*\
    | Length of data size                                       |
    \*---------------------------------------------------------*/
    data_size += sizeof(data_size);

    /*---------------------------------------------------------*\
    | Length of zone index                                      |
    \*---------------------------------------------------------*/
    data_size += sizeof(zone);

    /*---------------------------------------------------------*\
    | Length of segment name string                             |
    \*---------------------------------------------------------*/
    data_size += sizeof(unsigned short);

    /*---------------------------------------------------------*\
    | Segment name string data                                  |
    \*---------------------------------------------------------*/
    data_size += (unsigned int)strlen(new_segment.name.c_str()) + 1;

    data_size += sizeof(new_segment.type);
    data_size += sizeof(new_segment.start_idx);
    data_size += sizeof(new_segment.leds_count);

    /*---------------------------------------------------------*\
    | Create data buffer                                        |
    \*---------------------------------------------------------*/
    unsigned char *data_buf = new unsigned char[data_size];

    /*---------------------------------------------------------*\
    | Copy in data size                                         |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &data_size, sizeof(data_size));
    data_ptr += sizeof(data_size);

    /*---------------------------------------------------------*\
    | Copy in zone index                                        |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &zone, sizeof(zone));
    data_ptr += sizeof(zone);

    /*---------------------------------------------------------*\
    | Length of segment name string                             |
    \*---------------------------------------------------------*/
    unsigned short segment_name_length = (unsigned short)strlen(new_segment.name.c_str()) + 1;

    memcpy(&data_buf[data_ptr], &segment_name_length, sizeof(segment_name_length));
    data_ptr += sizeof(segment_name_length);

    /*---------------------------------------------------------*\
    | Segment name string data                                  |
    \*---------------------------------------------------------*/
    strcpy((char *)&data_buf[data_ptr], new_segment.name.c_str());
    data_ptr += segment_name_length;

    /*---------------------------------------------------------*\
    | Segment type data                                         |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &new_segment.type, sizeof(new_segment.type));
    data_ptr += sizeof(new_segment.type);

    /*---------------------------------------------------------*\
    | Segment start index data                                  |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &new_segment.start_idx, sizeof(new_segment.start_idx));
    data_ptr += sizeof(new_segment.start_idx);

    /*---------------------------------------------------------*\
    | Segment LED count data                                    |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &new_segment.leds_count, sizeof(new_segment.leds_count));
    data_ptr += sizeof(new_segment.leds_count);

    return(data_buf);
}

void RGBController::SetSegmentDescription(unsigned char* data_buf)
{
    unsigned int data_ptr = sizeof(unsigned int);

    /*---------------------------------------------------------*\
    | Copy in zone index                                        |
    \*---------------------------------------------------------*/
    unsigned int zone_idx;
    memcpy(&zone_idx, &data_buf[data_ptr], sizeof(zone_idx));
    data_ptr += sizeof(zone_idx);

    /*---------------------------------------------------------*\
    | Length of segment name string                             |
    \*---------------------------------------------------------*/
    unsigned short segment_name_length;
    memcpy(&segment_name_length, &data_buf[data_ptr], sizeof(segment_name_length));
    data_ptr += sizeof(segment_name_length);

    /*---------------------------------------------------------*\
    | Segment name string data                                  |
    \*---------------------------------------------------------*/
    char * segment_name = new char[segment_name_length];
    memcpy(segment_name, &data_buf[data_ptr], segment_name_length);
    data_ptr += segment_name_length;

    /*---------------------------------------------------------*\
    | Segment type data                                         |
    \*---------------------------------------------------------*/
    zone_type segment_type;
    memcpy(&segment_type, &data_buf[data_ptr], sizeof(segment_type));
    data_ptr += sizeof(segment_type);

    /*---------------------------------------------------------*\
    | Segment start index data                                  |
    \*---------------------------------------------------------*/
    unsigned int segment_start_idx;
    memcpy(&segment_start_idx, &data_buf[data_ptr], sizeof(segment_start_idx));
    data_ptr += sizeof(segment_start_idx);

    /*---------------------------------------------------------*\
    | Segment LED count data                                    |
    \*---------------------------------------------------------*/
    unsigned int segment_leds_count;
    memcpy(&segment_leds_count, &data_buf[data_ptr], sizeof(segment_leds_count));
    data_ptr += sizeof(segment_leds_count);

    /*---------------------------------------------------------*\
    | Add new segment                                           |
    \*---------------------------------------------------------*/
    segment new_segment;

    new_segment.name        = segment_name;
    new_segment.type        = segment_type;
    new_segment.start_idx   = segment_start_idx;
    new_segment.leds_count  = segment_leds_count;

    AddSegment(zone_idx, new_segment);

    delete[] segment_name;
}

void RGBController::SetupColors()
{
    unsigned int total_led_count;
    unsigned int zone_led_count;

    /*---------------------------------------------------------*\
    | Determine total number of LEDs on the device              |
    \*---------------------------------------------------------*/
    total_led_count = 0;

    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        total_led_count += GetLEDsInZone((unsigned int)zone_idx);
    }

    /*---------------------------------------------------------*\
    | Set the size of the color buffer to the number of LEDs    |
    \*---------------------------------------------------------*/
    colors.resize(total_led_count);

    /*---------------------------------------------------------*\
    | Set the color buffer pointers on each zone                |
    \*---------------------------------------------------------*/
    total_led_count = 0;

    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].start_idx   = total_led_count;
        zone_led_count              = GetLEDsInZone((unsigned int)zone_idx);

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

unsigned int RGBController::GetLEDsInZone(unsigned int zone)
{
    unsigned int leds_count = zones[zone].leds_count;

    if(zones[zone].flags & ZONE_FLAG_RESIZE_EFFECTS_ONLY)
    {
        if(leds_count > 1)
        {
            leds_count = 1;
        }
    }

    return(leds_count);
}

RGBColor RGBController::GetLED(unsigned int led)
{
    if(led < colors.size())
    {
        return(colors[led]);
    }
    else
    {
        return(0x00000000);
    }
}

void RGBController::SetLED(unsigned int led, RGBColor color)
{
    if(led < colors.size())
    {
        colors[led] = color;
    }
}

void RGBController::SetAllLEDs(RGBColor color)
{
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        SetAllZoneLEDs((int)zone_idx, color);
    }
}

void RGBController::SetAllZoneLEDs(int zone, RGBColor color)
{
    for (std::size_t color_idx = 0; color_idx < GetLEDsInZone(zone); color_idx++)
    {
        zones[zone].colors[color_idx] = color;
    }
}

int RGBController::GetMode()
{
    return(active_mode);
}

void RGBController::SetMode(int mode)
{
    active_mode = mode;

    UpdateMode();
}

void RGBController::RegisterUpdateCallback(RGBControllerCallback new_callback, void * new_callback_arg)
{
    UpdateCallbacks.push_back(new_callback);
    UpdateCallbackArgs.push_back(new_callback_arg);
}

void RGBController::UnregisterUpdateCallback(void * callback_arg)
{
    for(unsigned int callback_idx = 0; callback_idx < UpdateCallbackArgs.size(); callback_idx++ )
    {
        if(UpdateCallbackArgs[callback_idx] == callback_arg)
        {
            UpdateCallbackArgs.erase(UpdateCallbackArgs.begin() + callback_idx);
            UpdateCallbacks.erase(UpdateCallbacks.begin() + callback_idx);

            break;
        }
    }
}

void RGBController::ClearCallbacks()
{
    UpdateCallbacks.clear();
    UpdateCallbackArgs.clear();
}

void RGBController::SignalUpdate()
{
    UpdateMutex.lock();

    /*-------------------------------------------------*\
    | Client info has changed, call the callbacks       |
    \*-------------------------------------------------*/
    for(unsigned int callback_idx = 0; callback_idx < UpdateCallbacks.size(); callback_idx++)
    {
        UpdateCallbacks[callback_idx](UpdateCallbackArgs[callback_idx]);
    }

    UpdateMutex.unlock();
}
void RGBController::UpdateLEDs()
{
    CallFlag_UpdateLEDs = true;

    SignalUpdate();
}

void RGBController::UpdateMode()
{
    CallFlag_UpdateMode = true;
}

void RGBController::SaveMode()
{
    DeviceSaveMode();
}

void RGBController::DeviceUpdateLEDs()
{

}

void RGBController::SetCustomMode()
{
    /*-------------------------------------------------*\
    | Search the Controller's mode list for a suitable  |
    | per-LED custom mode in the following order:       |
    | 1.    Direct                                      |
    | 2.    Custom                                      |
    | 3.    Static                                      |
    \*-------------------------------------------------*/
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
}

void RGBController::DeviceUpdateMode()
{

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
                CallFlag_UpdateMode = false;
                DeviceUpdateMode();
            }
            else
            {
                DeviceUpdateMode();
                CallFlag_UpdateMode = false;
            }
        }
        if(CallFlag_UpdateLEDs.load() == true)
        {
            if(flags & CONTROLLER_FLAG_RESET_BEFORE_UPDATE)
            {
                CallFlag_UpdateLEDs = false;
                DeviceUpdateLEDs();
            }
            else
            {
                DeviceUpdateLEDs();
                CallFlag_UpdateLEDs = false;
            }
        }
        else
        {
           std::this_thread::sleep_for(1ms);
        }
    }
}

void RGBController::DeviceSaveMode()
{
    /*-------------------------------------------------*\
    | If not implemented by controller, does nothing    |
    \*-------------------------------------------------*/
}

void RGBController::ClearSegments(int zone)
{
    zones[zone].segments.clear();
}

void RGBController::AddSegment(int zone, segment new_segment)
{
    zones[zone].segments.push_back(new_segment);
}

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
