/**
    \file SimpleBrightnessStrategy.cpp
    Copyright Notice\n
    Copyright (C) 2017  Jan Rogall		- developer\n
    Copyright (C) 2017  Moritz Wirger	- developer\n

    This file is part of hueplusplus.

    hueplusplus is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    hueplusplus is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with hueplusplus.  If not, see <http://www.gnu.org/licenses/>.
**/

#include "include/SimpleBrightnessStrategy.h"

#include <cmath>
#include <iostream>
#include <thread>

#include "include/HueExceptionMacro.h"
#include "include/Utils.h"

bool SimpleBrightnessStrategy::setBrightness(unsigned int bri, uint8_t transition, HueLight& light) const
{
    light.refreshState();
    if (bri == 0)
    {
        if (light.state["state"]["on"] == true)
        {
            return light.OffNoRefresh(transition);
        }
        else
        {
            return true;
        }
    }
    else
    {
        nlohmann::json request = nlohmann::json::object();
        if (transition != 4)
        {
            request["transitiontime"] = transition;
        }
        if (light.state["state"]["on"] != true)
        {
            request["on"] = true;
        }
        if (light.state["state"]["bri"] != bri)
        {
            if (bri > 254)
            {
                bri = 254;
            }
            request["bri"] = bri;
        }

        if (!request.count("on") && !request.count("bri"))
        {
            // Nothing needs to be changed
            return true;
        }

        nlohmann::json reply = light.SendPutRequest(request, "/state", CURRENT_FILE_INFO);

        // Check whether request was successful
        return utils::validateReplyForLight(request, reply, light.id);
    }
}

unsigned int SimpleBrightnessStrategy::getBrightness(HueLight& light) const
{
    light.refreshState();
    return light.state["state"]["bri"].get<unsigned int>();
}

unsigned int SimpleBrightnessStrategy::getBrightness(const HueLight& light) const
{
    return light.state["state"]["bri"].get<unsigned int>();
}
