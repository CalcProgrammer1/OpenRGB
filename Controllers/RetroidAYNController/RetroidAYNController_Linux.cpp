/*---------------------------------------------------------*\
| RetroidAYNController_Linux.cpp                            |
|                                                           |
|   Driver for Retroid and AYN handheld RGB LEDs            |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      08 Nov 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <fstream>
#include <sstream>
#include "RetroidAYNController_Linux.h"

RetroidAYNController::RetroidAYNController(std::string dev_name)
{
    name = dev_name;
}

RetroidAYNController::~RetroidAYNController()
{
}

std::string RetroidAYNController::GetName()
{
    return(name);
}

std::string RetroidAYNController::GetLocation()
{
    return(led_paths[0]);
}

std::size_t RetroidAYNController::GetLEDCount()
{
    return(led_paths.size());
}

void RetroidAYNController::AddLED(std::string led_path)
{
    /*-----------------------------------------------------*\
    | Ensure the path ends with a slash                     |
    \*-----------------------------------------------------*/
    std::string path = led_path;

    if(path.back() != '/')
    {
        path += '/';
    }

    led_paths.push_back(path);

    /*-----------------------------------------------------*\
    | Read the channel order from multi_index               |
    | (e.g. "red green blue" or "blue green red")           |
    \*-----------------------------------------------------*/
    ReadChannelOrder(path);

    /*-----------------------------------------------------*\
    | Open the multi_intensity file for this LED            |
    | This file accepts space-separated values in the       |
    | channel order specified by multi_index                |
    \*-----------------------------------------------------*/
    std::ofstream multi_intensity_file;
    multi_intensity_file.open(path + "multi_intensity");
    led_multi_intensity.push_back(std::move(multi_intensity_file));

    /*-----------------------------------------------------*\
    | Open the brightness file for this LED                 |
    \*-----------------------------------------------------*/
    std::ofstream brightness_file;
    brightness_file.open(path + "brightness");
    led_brightness.push_back(std::move(brightness_file));
}

unsigned int RetroidAYNController::GetBrightness(unsigned int led_idx)
{
    unsigned int    brightness_value = 0;

    if(led_idx < led_paths.size())
    {
        std::ifstream   brightness_file(led_paths[led_idx] + "brightness");

        if(brightness_file.is_open())
        {
            std::string brightness_str;

            if(std::getline(brightness_file, brightness_str))
            {
                brightness_value = std::stoi(brightness_str);
            }

            brightness_file.close();
        }
    }

    return(brightness_value);
}

RGBColor RetroidAYNController::GetLEDColor(unsigned int led_idx)
{
    RGBColor        return_color = 0;
    std::ifstream   color_file(led_paths[led_idx] + "multi_intensity");

    if(color_file.is_open())
    {
        std::string color_str;
        if(std::getline(color_file, color_str))
        {
            std::istringstream iss(color_str);
            std::string token;
            std::vector<unsigned char> raw_color;

            while(iss >> token)
            {
                raw_color.push_back(static_cast<unsigned char>(std::stoi(token)));
            }

            if(raw_color.size() >= 3)
            {
                return_color = ToRGBColor(raw_color[led_channel_order_red[0]], raw_color[led_channel_order_green[0]], raw_color[led_channel_order_blue[0]]);
            }
        }
        color_file.close();
    }

    return(return_color);
}

unsigned int RetroidAYNController::GetMaxBrightness(unsigned int led_idx)
{
    unsigned int    brightness_value = 0;

    if(led_idx < led_paths.size())
    {
        std::ifstream   brightness_file(led_paths[led_idx] + "max_brightness");

        if(brightness_file.is_open())
        {
            std::string brightness_str;

            if(std::getline(brightness_file, brightness_str))
            {
                brightness_value = std::stoi(brightness_str);
            }

            brightness_file.close();
        }
    }

    return(brightness_value);
}

void RetroidAYNController::SetBrightness(unsigned int led_idx, unsigned int brightness)
{
    if(led_idx < led_brightness.size() && led_brightness[led_idx].is_open())
    {
        std::string brightness_str = std::to_string(brightness);
        led_brightness[led_idx].write(brightness_str.c_str(), brightness_str.length());
        led_brightness[led_idx].flush();
    }
}

void RetroidAYNController::SetLEDColor(unsigned int led_idx, RGBColor color)
{
    /*-----------------------------------------------------*\
    | Use multi_intensity to set RGB values.  The values    |
    | are written in the channel order specified by         |
    | multi_index.                                          |
    \*-----------------------------------------------------*/
    if(led_idx < led_multi_intensity.size() && led_multi_intensity[led_idx].is_open())
    {
        unsigned char   red         = RGBGetRValue(color);
        unsigned char   green       = RGBGetGValue(color);
        unsigned char   blue        = RGBGetBValue(color);

        unsigned char   values[3]   = { red, green, blue };
        std::string     color_str   = std::to_string(values[led_channel_order_red[led_idx]]) + " "
                                    + std::to_string(values[led_channel_order_green[led_idx]]) + " "
                                    + std::to_string(values[led_channel_order_blue[led_idx]]);

        led_multi_intensity[led_idx].write(color_str.c_str(), color_str.length());
        led_multi_intensity[led_idx].flush();
    }
}

void RetroidAYNController::ReadChannelOrder(std::string led_path)
{
    /*-----------------------------------------------------*\
    | Default to red=0, green=1, blue=2                     |
    \*-----------------------------------------------------*/
    led_channel_order_red.push_back(0);
    led_channel_order_green.push_back(1);
    led_channel_order_blue.push_back(2);

    std::ifstream index_file(led_path + "multi_index");

    if(!index_file.is_open())
    {
        return;
    }

    std::string index_str;
    std::getline(index_file, index_str);
    index_file.close();

    /*-----------------------------------------------------*\
    | Parse the multi_index string (e.g. "red green blue")  |
    | Each token is a channel name, its position in the     |
    | string is the index in multi_intensity output         |
    \*-----------------------------------------------------*/
    std::istringstream  iss(index_str);
    std::string         token;
    int                 idx = 0;

    while(iss >> token)
    {
        if(token == "red")
        {
            led_channel_order_red.back() = idx;
        }
        else if(token == "green")
        {
            led_channel_order_green.back() = idx;
        }
        else if(token == "blue")
        {
            led_channel_order_blue.back() = idx;
        }
        idx++;
    }
}
