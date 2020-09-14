/**
    \file SimpleColorHueStrategy.h
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

#ifndef _SIMPLE_COLOR_HUE_STRATEGY_H
#define _SIMPLE_COLOR_HUE_STRATEGY_H

#include "ColorHueStrategy.h"
#include "HueLight.h"

//! Class implementing the functions of ColorHueStrategy
class SimpleColorHueStrategy : public ColorHueStrategy
{
public:
    //! \brief Function for changing a lights color in hue with a specified
    //! transition.
    //!
    //! The hue ranges from 0 to 65535, whereas 65535 and 0 are red, 25500 is
    //! green and 46920 is blue. \param hue The hue of the color \param transition
    //! The time it takes to fade to the new color in multiples of 100ms, 4 =
    //! 400ms and should be seen as the default \param light A reference of the
    //! light
    bool setColorHue(uint16_t hue, uint8_t transition, HueLight& light) const override;
    //! \brief Function for changing a lights color in saturation with a specified
    //! transition.
    //!
    //! The saturation ranges from 0 to 254, whereas 0 is least saturated (white)
    //! and 254 is most saturated (vibrant). \param sat The saturation of the
    //! color \param transition The time it takes to fade to the new color in
    //! multiples of 100ms, 4 = 400ms and should be seen as the default \param
    //! light A reference of the light
    bool setColorSaturation(uint8_t sat, uint8_t transition, HueLight& light) const override;
    //! \brief Function for changing a lights color in hue and saturation format
    //! with a specified transition.
    //!
    //! The hue ranges from 0 to 65535, whereas 65535 and 0 are red, 25500 is
    //! green and 46920 is blue. The saturation ranges from 0 to 254, whereas 0 is
    //! least saturated (white) and 254 is most saturated (vibrant). \param hue
    //! The hue of the color \param sat The saturation of the color \param
    //! transition The time it takes to fade to the new color in multiples of
    //! 100ms, 4 = 400ms and should be seen as the default \param light A
    //! reference of the light
    bool setColorHueSaturation(uint16_t hue, uint8_t sat, uint8_t transition, HueLight& light) const override;
    //! \brief Function for changing a lights color in CIE format with a specified
    //! transition.
    //!
    //! \param x The x coordinate in CIE, ranging from 0 to 1
    //! \param y The y coordinate in CIE, ranging from 0 to 1
    //! \param transition The time it takes to fade to the new color in multiples
    //! of 100ms, 4 = 400ms and should be seen as the default \param light A
    //! reference of the light
    bool setColorXY(float x, float y, uint8_t transition, HueLight& light) const override;
    //! \brief Function for changing a lights color in rgb format with a specified
    //! transition.
    //!
    //! Red, green and blue are ranging from 0 to 255.
    //! \param r The red portion of the color
    //! \param g The green portion of the color
    //! \param b The blue portion of the color
    //! \param transition The time it takes to fade to the new color in multiples
    //! of 100ms, 4 = 400ms and should be seen as the default \param light A
    //! reference of the light
    bool setColorRGB(uint8_t r, uint8_t g, uint8_t b, uint8_t transition, HueLight& light) const override;
    //! \brief Function for turning on/off the color loop feature of a light.
    //!
    //! Can be theoretically set for any light, but it only works for lights that
    //! support this feature. When this feature is activated the light will fade
    //! through every color on the current hue and saturation settings. Notice
    //! that none of the setter functions check whether this feature is enabled
    //! and the colorloop can only be disabled with this function or by simply
    //! calling Off()/OffNoRefresh() and then On()/OnNoRefresh(), so you could
    //! alternatively call Off() and then use any of the setter functions.
    //! \param on Boolean to turn this feature on or off, true/1 for on and
    //! false/0 for off \param light A reference of the light
    bool setColorLoop(bool on, HueLight& light) const override;
    //! \brief Function that lets the light perform one breath cycle in the
    //! specified color.
    //!
    //! \note It uses this_thread::sleep_for to accomodate for the time an \ref
    //! HueLight::alert() needs The hue ranges from 0 to 65535, whereas 65535 and
    //! 0 are red, 25500 is green and 46920 is blue. The saturation ranges from 0
    //! to 254, whereas 0 is least saturated (white) and 254 is most saturated
    //! (vibrant). \param hue The hue of the color \param sat The saturation of
    //! the color \param light A reference of the light
    bool alertHueSaturation(uint16_t hue, uint8_t sat, HueLight& light) const override;
    //! \brief Function that lets the light perform one breath cycle in the
    //! specified color.
    //!
    //! \note It uses this_thread::sleep_for to accomodate for the time an \ref
    //! HueLight::alert() needs \param x The x coordinate in CIE, ranging from 0
    //! to 1 \param y The y coordinate in CIE, ranging from 0 to 1 \param light A
    //! reference of the light
    bool alertXY(float x, float y, HueLight& light) const override;
    //! \brief Function that lets the light perform one breath cycle in the
    //! specified color.
    //!
    //! \note It uses this_thread::sleep_for to accomodate for the time an \ref
    //! HueLight::alert() needs Red, green and blue are ranging from 0 to 255.
    //! \param r The red portion of the color
    //! \param g The green portion of the color
    //! \param b The blue portion of the color
    //! \param light A reference of the light
    bool alertRGB(uint8_t r, uint8_t g, uint8_t b, HueLight& light) const override;
    //! \brief Function that returns the current color of the light as hue and
    //! saturation
    //!
    //! Updates the lights state by calling refreshState()
    //! \param light A reference of the light
    //! \return Pair containing the hue as first value and saturation as second
    //! value
    std::pair<uint16_t, uint8_t> getColorHueSaturation(HueLight& light) const override;
    //! \brief Function that returns the current color of the light as hue and
    //! saturation
    //!
    //! \note This does not update the lights state
    //! \param light A const reference of the light
    //! \return Pair containing the hue as first value and saturation as second
    //! value
    std::pair<uint16_t, uint8_t> getColorHueSaturation(const HueLight& light) const override;
    //! \brief Function that returns the current color of the light as xy
    //!
    //! Updates the lights state by calling refreshState()
    //! \param light A reference of the light
    //! \return Pair containing the x as first value and y as second value
    std::pair<float, float> getColorXY(HueLight& light) const override;
    //! \brief Function that returns the current color of the light as xy
    //!
    //! \note This does not update the lights state
    //! \param light A const reference of the light
    //! \return Pair containing the x as first value and y as second value
    std::pair<float, float> getColorXY(const HueLight& light) const override;
};

#endif
