/**
    \file ExtendedColorTemperatureStrategy.cpp
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

#include "include/ExtendedColorTemperatureStrategy.h"

#include <cmath>
#include <iostream>
#include <thread>

#include "include/HueConfig.h"
#include "include/HueExceptionMacro.h"
#include "include/Utils.h"

bool ExtendedColorTemperatureStrategy::setColorTemperature(
    unsigned int mired, uint8_t transition, HueLight& light) const
{
    light.refreshState();
    nlohmann::json request = nlohmann::json::object();
    if (transition != 4)
    {
        request["transitiontime"] = transition;
    }
    if (light.state["state"]["on"] != true)
    {
        request["on"] = true;
    }
    if (light.state["state"]["ct"] != mired || light.state["state"]["colormode"] != "ct")
    {
        if (mired > 500)
        {
            mired = 500;
        }
        if (mired < 153)
        {
            mired = 153;
        }
        request["ct"] = mired;
    }

    if (!request.count("on") && !request.count("ct"))
    {
        // Nothing needs to be changed
        return true;
    }

    nlohmann::json reply = light.SendPutRequest(request, "/state", CURRENT_FILE_INFO);

    // Check whether request was successful
    return utils::validateReplyForLight(request, reply, light.id);
}

bool ExtendedColorTemperatureStrategy::alertTemperature(unsigned int mired, HueLight& light) const
{
    light.refreshState();
    std::string cType = light.state["state"]["colormode"].get<std::string>();
    bool on = light.state["state"]["on"].get<bool>();
    if (cType == "hs")
    {
        uint16_t oldHue = light.state["state"]["hue"].get<uint16_t>();
        uint8_t oldSat = light.state["state"]["sat"].get<uint8_t>();
        if (!light.setColorTemperature(mired, 1))
        {
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(c_PRE_ALERT_DELAY));
        if (!light.alert())
        {
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(c_POST_ALERT_DELAY));
        if (!on)
        {
            light.setColorHueSaturation(oldHue, oldSat, 1);
            return light.OffNoRefresh(1);
        }
        else
        {
            return light.setColorHueSaturation(oldHue, oldSat, 1);
        }
    }
    else if (cType == "xy")
    {
        float oldX = light.state["state"]["xy"][0].get<float>();
        float oldY = light.state["state"]["xy"][1].get<float>();
        if (!light.setColorTemperature(mired, 1))
        {
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(c_PRE_ALERT_DELAY));
        if (!light.alert())
        {
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(c_POST_ALERT_DELAY));
        if (!on)
        {
            light.setColorXY(oldX, oldY, 1);
            return light.OffNoRefresh(1);
        }
        else
        {
            return light.setColorXY(oldX, oldY, 1);
        }
    }
    else if (cType == "ct")
    {
        uint16_t oldCT = light.state["state"]["ct"].get<uint16_t>();
        if (!light.setColorTemperature(mired, 1))
        {
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(c_PRE_ALERT_DELAY));
        if (!light.alert())
        {
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(c_POST_ALERT_DELAY));
        if (!on)
        {
            light.setColorTemperature(oldCT, 1);
            return light.OffNoRefresh(1);
        }
        else
        {
            return light.setColorTemperature(oldCT, 1);
        }
    }
    else
    {
        return false;
    }
}
