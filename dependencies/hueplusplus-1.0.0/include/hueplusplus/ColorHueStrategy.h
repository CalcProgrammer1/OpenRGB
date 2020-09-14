/**
    \file ColorHueStrategy.h
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

#ifndef _COLOR_HUE_STRATEGY_H
#define _COLOR_HUE_STRATEGY_H

#include <cstdint>
#include <memory>

#include "ColorUnits.h"

namespace hueplusplus
{
class Light;

//! Virtual base class for all ColorHueStrategies
class ColorHueStrategy
{
public:
    //! \brief Virtual function for changing a lights color in hue with a
    //! specified transition.
    //!
    //! The hue ranges from 0 to 65535, whereas 65535 and 0 are red, 25500 is
    //! green and 46920 is blue. \param hue The hue of the color \param transition
    //! The time it takes to fade to the new color in multiples of 100ms, 4 =
    //! 400ms and should be seen as the default \param light A reference of the
    //! light
    virtual bool setColorHue(uint16_t hue, uint8_t transition, Light& light) const = 0;
    //! \brief Virtual function for changing a lights color in saturation with a
    //! specified transition.
    //!
    //! The saturation ranges from 0 to 254, whereas 0 is least saturated (white)
    //! and 254 is most saturated (vibrant). \param sat The saturation of the
    //! color \param transition The time it takes to fade to the new color in
    //! multiples of 100ms, 4 = 400ms and should be seen as the default \param
    //! light A reference of the light
    virtual bool setColorSaturation(uint8_t sat, uint8_t transition, Light& light) const = 0;
    //! \brief Virtual function for changing a lights color in hue and saturation
    //! format with a specified transition.
    //!
    //! \param hueSat Color in hue and satuation.
    //! \param transition The time it takes to fade to the new color in multiples of
    //! 100ms, 4 = 400ms and should be seen as the default
    //! \param light A reference of the light
    virtual bool setColorHueSaturation(const HueSaturation& hueSat, uint8_t transition, Light& light) const = 0;
    //! \brief Virtual function for changing a lights color in CIE format with a
    //! specified transition.
    //!
    //! \param xy The color in XY and brightness
    //! \param transition The time it takes to fade to the new color in multiples
    //! of 100ms, 4 = 400ms and should be seen as the default \param light A
    //! reference of the light
    virtual bool setColorXY(const XYBrightness& xy, uint8_t transition, Light& light) const = 0;

    //! \brief Virtual function for turning on/off the color loop feature of a
    //! light.
    //!
    //! Can be theoretically set for any light, but it only works for lights that
    //! support this feature. When this feature is activated the light will fade
    //! through every color on the current hue and saturation settings. Notice
    //! that none of the setter functions check whether this feature is enabled
    //! and the colorloop can only be disabled with this function or by simply
    //! calling off() and then on(), so you could
    //! alternatively call off() and then use any of the setter functions. \param
    //! on Boolean to turn this feature on or off, true/1 for on and false/0 for
    //! off \param light A reference of the light
    virtual bool setColorLoop(bool on, Light& light) const = 0;
    //! \brief Virtual function that lets the light perform one breath cycle in
    //! the specified color.
    //!
    //! \param hueSat The color in hue and saturation
    //! \param light A reference of the light
    virtual bool alertHueSaturation(const HueSaturation& hueSat, Light& light) const = 0;
    //! \brief Virtual function that lets the light perform one breath cycle in
    //! the specified color.
    //!
    //! \param xy The color in XY and brightness
    //! \param light A reference of the light
    virtual bool alertXY(const XYBrightness& xy, Light& light) const = 0;
    //! \brief Virtual function that returns the current color of the light as hue
    //! and saturation
    //!
    //! Should update the lights state by calling refreshState()
    //! \param light A reference of the light
    virtual HueSaturation getColorHueSaturation(Light& light) const = 0;
    //! \brief Virtual function that returns the current color of the light as hue
    //! and saturation
    //!
    //! \note This should not update the lights state
    //! \param light A const reference of the light
    virtual HueSaturation getColorHueSaturation(const Light& light) const = 0;
    //! \brief Virtual function that returns the current color of the light as xy
    //!
    //! Should update the lights state by calling refreshState()
    //! \param light A reference of the light
    //! \return XY and brightness of current color
    virtual XYBrightness getColorXY(Light& light) const = 0;
    //! \brief Virtual function that returns the current color of the light as xy
    //!
    //! \note This should not update the lights state
    //! \param light A const reference of the light
    //! \return XY and brightness of current color
    virtual XYBrightness getColorXY(const Light& light) const = 0;
    //! \brief Virtual dtor
    virtual ~ColorHueStrategy() = default;
};
} // namespace hueplusplus

#endif
