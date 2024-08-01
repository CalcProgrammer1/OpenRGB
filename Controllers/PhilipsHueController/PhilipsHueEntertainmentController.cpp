/*---------------------------------------------------------*\
| PhilipsHueEntertainmentController.cpp                     |
|                                                           |
|   Detector for Philips Hue Entertainment Mode             |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      06 Nov 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController.h"
#include "PhilipsHueEntertainmentController.h"

PhilipsHueEntertainmentController::PhilipsHueEntertainmentController(hueplusplus::Bridge& bridge_ptr, hueplusplus::Group group_ptr):bridge(bridge_ptr),group(group_ptr)
{
    /*-------------------------------------------------*\
    | Fill in location string with bridge IP            |
    \*-------------------------------------------------*/
    location                            = "IP: " + bridge.getBridgeIP();
    num_leds                            = (unsigned int)group.getLightIds().size();
    connected                           = false;
}

PhilipsHueEntertainmentController::~PhilipsHueEntertainmentController()
{

}

std::string PhilipsHueEntertainmentController::GetLocation()
{
    return(location);
}

std::string PhilipsHueEntertainmentController::GetName()
{
    return(group.getName());
}

std::string PhilipsHueEntertainmentController::GetVersion()
{
    return("");
}

std::string PhilipsHueEntertainmentController::GetManufacturer()
{
    return("");
}

std::string PhilipsHueEntertainmentController::GetUniqueID()
{
    return("");
}

unsigned int PhilipsHueEntertainmentController::GetNumLEDs()
{
    return(num_leds);
}

void PhilipsHueEntertainmentController::SetColor(RGBColor* colors)
{
    if(connected)
    {
        /*-------------------------------------------------*\
        | Fill in Entertainment Mode light data             |
        \*-------------------------------------------------*/
        for(unsigned int light_idx = 0; light_idx < num_leds; light_idx++)
        {
            RGBColor color                  = colors[light_idx];
            unsigned char red               = RGBGetRValue(color);
            unsigned char green             = RGBGetGValue(color);
            unsigned char blue              = RGBGetBValue(color);

            entertainment->setColorRGB(light_idx, red, green, blue);
        }

        entertainment->update();
    }
}

void PhilipsHueEntertainmentController::Connect()
{
    if(!connected)
    {
        /*-------------------------------------------------*\
        | Create Entertainment Mode from bridge and group   |
        \*-------------------------------------------------*/
        entertainment = new hueplusplus::EntertainmentMode(bridge, group);

        /*-------------------------------------------------*\
        | Connect Hue Entertainment Mode                    |
        \*-------------------------------------------------*/
        entertainment->connect();
        connected = true;
    }
}

void PhilipsHueEntertainmentController::Disconnect()
{
    if(connected)
    {
        /*-------------------------------------------------*\
        | Disconnect Hue Entertainment Mode                 |
        \*-------------------------------------------------*/
        entertainment->disconnect();
        connected = false;

        delete entertainment;
    }
}
