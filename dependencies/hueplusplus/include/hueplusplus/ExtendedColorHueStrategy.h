/**
    \file ExtendedColorHueStrategy.h
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

#ifndef INCLUDE_HUEPLUSPLUS_EXTENDED_COLOR_HUE_STRATEGY_H
#define INCLUDE_HUEPLUSPLUS_EXTENDED_COLOR_HUE_STRATEGY_H

#include "Light.h"
#include "SimpleColorHueStrategy.h"

namespace hueplusplus
{
//! Class extending the implementation of SimpleColorHueStrategy
//!
//! To be used for lights that have both color and color temperature.
class ExtendedColorHueStrategy : public SimpleColorHueStrategy
{
public:
    //! \brief Function that lets the light perform one breath cycle in the
    //! specified color.
    //! \param hueSat The color in hue and saturation
    //! \param light A reference of the light
    //!
    //! Blocks for the time a \ref Light::alert() needs
    bool alertHueSaturation(const HueSaturation& hueSat, Light& light) const override;
    //! \brief Function that lets the light perform one breath cycle in the
    //! specified color.
    //! \param xy The color in XY and brightness
    //! \param light A reference of the light
    //!
    //! Blocks for the time a \ref Light::alert() needs
    bool alertXY(const XYBrightness& xy, Light& light) const override;
};
} // namespace hueplusplus

#endif
