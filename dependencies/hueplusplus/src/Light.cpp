/**
    \file Light.cpp
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

#include "hueplusplus/Light.h"

#include <cmath>
#include <iostream>
#include <thread>

#include "hueplusplus/HueExceptionMacro.h"
#include "hueplusplus/Utils.h"
#include "json/json.hpp"

namespace hueplusplus
{
bool Light::On(uint8_t transition)
{
    return transaction().setOn(true).setTransition(transition).commit();
}

bool Light::Off(uint8_t transition)
{
    return transaction().setOn(false).setTransition(transition).commit();
}

bool Light::isOn()
{
    return state.getValue().at("state").at("on").get<bool>();
}

bool Light::isOn() const
{
    return state.getValue().at("state").at("on").get<bool>();
}

std::string Light::getLuminaireUId() const
{
    return state.getValue().value("luminaireuniqueid", std::string());
}

ColorType Light::getColorType() const
{
    return colorType;
}

ColorGamut Light::getColorGamut() const
{
    switch (colorType)
    {
    case ColorType::GAMUT_A:
    case ColorType::GAMUT_A_TEMPERATURE:
        return gamut::gamutA;
    case ColorType::GAMUT_B:
    case ColorType::GAMUT_B_TEMPERATURE:
        return gamut::gamutB;
    case ColorType::GAMUT_C:
    case ColorType::GAMUT_C_TEMPERATURE:
        return gamut::gamutC;
    default: {
        const nlohmann::json& capabilitiesGamut
            = utils::safeGetMember(state.getValue(), "capabilities", "control", "colorgamut");
        if (capabilitiesGamut.is_array() && capabilitiesGamut.size() == 3)
        {
            // Other gamut
            return ColorGamut {{capabilitiesGamut[0].at(0), capabilitiesGamut[0].at(1)},
                {capabilitiesGamut[1].at(0), capabilitiesGamut[1].at(1)},
                {capabilitiesGamut[2].at(0), capabilitiesGamut[2].at(1)}};
        }
        // Unknown or no color light
        return gamut::maxGamut;
    }
    }
}

unsigned int Light::KelvinToMired(unsigned int kelvin) const
{
    return int(0.5f + (1000000 / kelvin));
}

unsigned int Light::MiredToKelvin(unsigned int mired) const
{
    return int(0.5f + (1000000 / mired));
}

bool Light::alert()
{
    return transaction().alert().commit();
}

StateTransaction Light::transaction()
{
    return StateTransaction(
        state.getCommandAPI(), "/lights/" + std::to_string(id) + "/state", &state.getValue().at("state"));
}

Light::Light(int id, const HueCommandAPI& commands) : Light(id, commands, nullptr, nullptr, nullptr) { }

Light::Light(int id, const HueCommandAPI& commands, std::shared_ptr<const BrightnessStrategy> brightnessStrategy,
    std::shared_ptr<const ColorTemperatureStrategy> colorTempStrategy,
    std::shared_ptr<const ColorHueStrategy> colorHueStrategy, std::chrono::steady_clock::duration refreshDuration)
    : BaseDevice(id, commands, "/lights/", refreshDuration),
      colorType(ColorType::NONE),
      brightnessStrategy(std::move(brightnessStrategy)),
      colorTemperatureStrategy(std::move(colorTempStrategy)),
      colorHueStrategy(std::move(colorHueStrategy))
{
}
} // namespace hueplusplus
