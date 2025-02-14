/**
    \file SimpleColorHueStrategy.cpp
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

#include "hueplusplus/SimpleColorHueStrategy.h"

#include <cmath>
#include <iostream>
#include <thread>

#include "hueplusplus/LibConfig.h"
#include "hueplusplus/HueExceptionMacro.h"
#include "hueplusplus/Utils.h"

namespace hueplusplus
{
bool SimpleColorHueStrategy::setColorHue(uint16_t hue, uint8_t transition, Light& light) const
{
    return light.transaction().setColorHue(hue).setTransition(transition).commit();
}

bool SimpleColorHueStrategy::setColorSaturation(uint8_t sat, uint8_t transition, Light& light) const
{
    return light.transaction().setColorSaturation(sat).setTransition(transition).commit();
}

bool SimpleColorHueStrategy::setColorHueSaturation(
    const HueSaturation& hueSat, uint8_t transition, Light& light) const
{
    return light.transaction().setColor(hueSat).setTransition(transition).commit();
}

bool SimpleColorHueStrategy::setColorXY(const XYBrightness& xy, uint8_t transition, Light& light) const
{
    return light.transaction().setColor(xy).setTransition(transition).commit();
}

bool SimpleColorHueStrategy::setColorLoop(bool on, Light& light) const
{
    return light.transaction().setColorLoop(on).commit();
}

bool SimpleColorHueStrategy::alertHueSaturation(const HueSaturation& hueSat, Light& light) const
{
    // Careful, only use state until any light function might refresh the value and invalidate the reference
    const nlohmann::json& state = light.state.getValue()["state"];
    std::string cType = state["colormode"].get<std::string>();
    bool on = state["on"].get<bool>();
    const Light& cLight = light;
    if (cType == "hs")
    {
        HueSaturation oldHueSat = cLight.getColorHueSaturation();
        if (!light.setColorHueSaturation(hueSat, 1))
        {
            return false;
        }
        std::this_thread::sleep_for(Config::instance().getPreAlertDelay());
        if (!light.alert())
        {
            return false;
        }
        std::this_thread::sleep_for(Config::instance().getPostAlertDelay());
        return light.transaction().setColor(oldHueSat).setOn(on).setTransition(1).commit();
    }
    else if (cType == "xy")
    {
        XYBrightness oldXY = cLight.getColorXY();
        if (!light.setColorHueSaturation(hueSat, 1))
        {
            return false;
        }
        std::this_thread::sleep_for(Config::instance().getPreAlertDelay());
        if (!light.alert())
        {
            return false;
        }
        std::this_thread::sleep_for(Config::instance().getPostAlertDelay());
        return light.transaction().setColor(oldXY).setOn(on).setTransition(1).commit();
    }
    else
    {
        return false;
    }
}

bool SimpleColorHueStrategy::alertXY(const XYBrightness& xy, Light& light) const
{
    // Careful, only use state until any light function might refresh the value and invalidate the reference
    const nlohmann::json& state = light.state.getValue()["state"];
    std::string cType = state["colormode"].get<std::string>();
    bool on = state["on"].get<bool>();
    // const reference to prevent refreshes
    const Light& cLight = light;
    if (cType == "hs")
    {
        HueSaturation oldHueSat = cLight.getColorHueSaturation();
        uint8_t oldBrightness = cLight.getBrightness();
        if (!light.setColorXY(xy, 1))
        {
            return false;
        }
        std::this_thread::sleep_for(Config::instance().getPreAlertDelay());
        if (!light.alert())
        {
            return false;
        }
        std::this_thread::sleep_for(Config::instance().getPostAlertDelay());
        return light.transaction().setColor(oldHueSat).setBrightness(oldBrightness).setOn(on).setTransition(1).commit();
    }
    else if (cType == "xy")
    {
        XYBrightness oldXY = cLight.getColorXY();
        if (!light.setColorXY(xy, 1))
        {
            return false;
        }
        std::this_thread::sleep_for(Config::instance().getPreAlertDelay());
        if (!light.alert())
        {
            return false;
        }
        std::this_thread::sleep_for(Config::instance().getPostAlertDelay());
        return light.transaction().setColor(oldXY).setOn(on).setTransition(1).commit();
    }
    else
    {
        return false;
    }
}

HueSaturation SimpleColorHueStrategy::getColorHueSaturation(Light& light) const
{
    // Save value, so there are no inconsistent results if it is refreshed between two calls
    const nlohmann::json& state = light.state.getValue()["state"];
    return HueSaturation {state["hue"].get<int>(), state["sat"].get<int>()};
}

HueSaturation SimpleColorHueStrategy::getColorHueSaturation(const Light& light) const
{
    return HueSaturation {
        light.state.getValue()["state"]["hue"].get<int>(), light.state.getValue()["state"]["sat"].get<int>()};
}

XYBrightness SimpleColorHueStrategy::getColorXY(Light& light) const
{
    // Save value, so there are no inconsistent results if it is refreshed between two calls
    const nlohmann::json& state = light.state.getValue()["state"];
    return XYBrightness {{state["xy"][0].get<float>(), state["xy"][1].get<float>()}, state["bri"].get<int>() / 254.f};
}

XYBrightness SimpleColorHueStrategy::getColorXY(const Light& light) const
{
    const nlohmann::json& state = light.state.getValue()["state"];
    return XYBrightness {{state["xy"][0].get<float>(), state["xy"][1].get<float>()}, state["bri"].get<int>() / 254.f};
}

} // namespace hueplusplus
