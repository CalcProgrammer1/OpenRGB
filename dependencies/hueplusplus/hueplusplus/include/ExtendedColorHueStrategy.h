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

#ifndef _EXTENDED_COLOR_HUE_STRATEGY_H
#define _EXTENDED_COLOR_HUE_STRATEGY_H

#include "HueLight.h"
#include "SimpleColorHueStrategy.h"

//! Class extending the implementation of SimpleColorHueStrategy
class ExtendedColorHueStrategy : public SimpleColorHueStrategy
{
public:
    //! \brief Function that lets the light perform one breath cycle in the
    //! specified color.
    //!
    //! It uses this_thread::sleep_for to accomodate for the time an \ref
    //! HueLight::alert() needs The hue ranges from 0 to 65535, whereas 65535 and
    //! 0 are red, 25500 is green and 46920 is blue. The saturation ranges from 0
    //! to 254, whereas 0 is least saturated (white) and 254 is most saturated
    //! (vibrant). \param hue The hue of the color \param sat The saturation of
    //! the color \param light A reference of the light
    bool alertHueSaturation(uint16_t hue, uint8_t sat, HueLight& light) const override;
    //! \brief Function that lets the light perform one breath cycle in the
    //! specified color.
    //!
    //! It uses this_thread::sleep_for to accomodate for the time an \ref
    //! HueLight::alert() needs \param x The x coordinate in CIE, ranging from 0
    //! to 1 \param y The y coordinate in CIE, ranging from 0 to 1 \param light A
    //! reference of the light
    bool alertXY(float x, float y, HueLight& light) const override;
    //! \brief Function that lets the light perform one breath cycle in the
    //! specified color.
    //!
    //! It uses this_thread::sleep_for to accomodate for the time an \ref
    //! HueLight::alert() needs Red, green and blue are ranging from 0 to 255.
    //! \param r The red portion of the color
    //! \param g The green portion of the color
    //! \param b The blue portion of the color
    //! \param light A reference of the light
    bool alertRGB(uint8_t r, uint8_t g, uint8_t b, HueLight& light) const override;
};

#endif
