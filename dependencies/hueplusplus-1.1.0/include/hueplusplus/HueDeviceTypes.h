/**
    \file HueDeviceTypes.h
    Copyright Notice\n
    Copyright (C) 2020  Jan Rogall      - developer\n
    Copyright (C) 2020  Moritz Wirger   - developer\n

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

#ifndef INCLUDE_HUEPLUSPLUS_HUEDEVICETYPES_H
#define INCLUDE_HUEPLUSPLUS_HUEDEVICETYPES_H

#include <memory>
#include <string>

#include "Light.h"

namespace hueplusplus
{
class LightFactory
{
public:
    //! \brief Create a factory for Light%s
    //! \param commands HueCommandAPI for communication with the bridge
    //! \param refreshDuration Time between refreshing the cached light state.
    LightFactory(const HueCommandAPI& commands, std::chrono::steady_clock::duration refreshDuration);

    //! \brief Create a Light with the correct type from the JSON state.
    //! \param lightState Light JSON as returned from the bridge (not only the "state" part of it).
    //! \param id Light id.
    //! \param baseCache Optional shared cache for the light.
    //! \returns Light with matching id, strategies and \ref ColorType.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when light type is unknown
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    Light createLight(const nlohmann::json& lightState, int id, const std::shared_ptr<APICache>& baseCache = {});

private:
    //! \brief Get color type from light JSON.
    //! \param lightState Light JSON as returned from the bridge (not only the "state" part of it).
    //! \param hasCt Whether the light has color temperature control.
    //! \returns The color gamut specified in the light capabilities or,
    //! if that does not exist, from a set of known models. Returns GAMUT_X_TEMPERATURE when \ref hasCt is true.
    //! \throws HueException when the light has no capabilities and the model is not known.
    ColorType getColorType(const nlohmann::json& lightState, bool hasCt) const;

private:
    HueCommandAPI commands;
    std::chrono::steady_clock::duration refreshDuration;
    std::shared_ptr<BrightnessStrategy> simpleBrightness;
    std::shared_ptr<ColorTemperatureStrategy> simpleColorTemperature;
    std::shared_ptr<ColorTemperatureStrategy> extendedColorTemperature;
    std::shared_ptr<ColorHueStrategy> simpleColorHue;
    std::shared_ptr<ColorHueStrategy> extendedColorHue;
};
} // namespace hueplusplus

#endif
