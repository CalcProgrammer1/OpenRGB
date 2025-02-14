/**
    \file SimpleColorTemperatureStrategy.cpp
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

#include "hueplusplus/SimpleColorTemperatureStrategy.h"

#include <cmath>
#include <iostream>
#include <thread>

#include "hueplusplus/LibConfig.h"
#include "hueplusplus/HueExceptionMacro.h"
#include "hueplusplus/Utils.h"

namespace hueplusplus
{
bool SimpleColorTemperatureStrategy::setColorTemperature(unsigned int mired, uint8_t transition, Light& light) const
{
    return light.transaction().setColorTemperature(mired).setTransition(transition).commit();
}

bool SimpleColorTemperatureStrategy::alertTemperature(unsigned int mired, Light& light) const
{
    // Careful, only use state until any light function might refresh the value and invalidate the reference
    const nlohmann::json& state = light.state.getValue()["state"];
    std::string cType = state["colormode"].get<std::string>();
    bool on = state["on"].get<bool>();
    if (cType == "ct")
    {
        uint16_t oldCT = state["ct"].get<uint16_t>();
        if (!light.setColorTemperature(mired, 1))
        {
            return false;
        }
        std::this_thread::sleep_for(Config::instance().getPreAlertDelay());
        if (!light.alert())
        {
            return false;
        }
        std::this_thread::sleep_for(Config::instance().getPostAlertDelay());
        return light.transaction().setColorTemperature(oldCT).setOn(on).setTransition(1).commit();
    }
    else
    {
        return false;
    }
}

unsigned int SimpleColorTemperatureStrategy::getColorTemperature(Light& light) const
{
    return light.state.getValue()["state"]["ct"].get<unsigned int>();
}

unsigned int SimpleColorTemperatureStrategy::getColorTemperature(const Light& light) const
{
    return light.state.getValue()["state"]["ct"].get<unsigned int>();
}
} // namespace hueplusplus
