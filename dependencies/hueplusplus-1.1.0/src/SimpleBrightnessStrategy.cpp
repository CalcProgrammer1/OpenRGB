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

#include "hueplusplus/SimpleBrightnessStrategy.h"

#include <cmath>
#include <iostream>
#include <thread>

#include "hueplusplus/HueExceptionMacro.h"
#include "hueplusplus/Utils.h"

namespace hueplusplus
{
bool SimpleBrightnessStrategy::setBrightness(unsigned int bri, uint8_t transition, Light& light) const
{
    // Careful, only use state until any light function might refresh the value and invalidate the reference
    return light.transaction().setBrightness(bri).setTransition(transition).commit();
}

unsigned int SimpleBrightnessStrategy::getBrightness(Light& light) const
{
    return light.state.getValue()["state"]["bri"].get<unsigned int>();
}

unsigned int SimpleBrightnessStrategy::getBrightness(const Light& light) const
{
    return light.state.getValue()["state"]["bri"].get<unsigned int>();
}
} // namespace hueplusplus
