/**
    \file ColorUnits.h
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

#ifndef INCLUDE_HUEPLUSPLUS_UNITS_H
#define INCLUDE_HUEPLUSPLUS_UNITS_H

#include <cstdint>

namespace hueplusplus
{
//! \brief Color in hue and saturation
struct HueSaturation
{
    //! \brief Color hue
    //!
    //! Ranges from 0 to 65535 (16 bit), where 65535 and 0 are red, 25500 is green and 46920 is blue.
    int hue;
    //! \brief Color saturation
    //!
    //! Ranges from 0 to 254 (8 bit), where 0 is least saturated (white) and 254 is most saturated (vibrant).
    int saturation;

    bool operator==(const HueSaturation& other) const { return hue == other.hue && saturation == other.saturation; }
    bool operator!=(const HueSaturation& other) const { return !(*this == other); }
};

//! \brief Color in CIE x and y coordinates
struct XY
{
    //! \brief x coordinate in CIE, 0 to 1
    float x;
    //! \brief y coordinate in CIE, 0 to 1
    float y;

    bool operator==(const XY& other) const { return x == other.x && y == other.y; }
    bool operator!=(const XY& other) const { return !(*this == other); }
};

//! \brief Color and brightness in CIE
//!
//! The brightness is needed to convert back to RGB colors if necessary.
//! \note brightness is not the actual luminance of the color, but instead the brightness the light is set to.
struct XYBrightness
{
    //! \brief XY color
    XY xy;
    //! \brief Brightness from 0 to 1
    float brightness;

    bool operator==(const XYBrightness& other) const { return xy == other.xy && brightness == other.brightness; }
    bool operator!=(const XYBrightness& other) const { return !(*this == other); }
};

//! \brief Triangle of representable colors in CIE
//!
//! \note Red, green and blue corner are oriented counter clockwise.
//! \see https://en.wikipedia.org/wiki/Chromaticity
struct ColorGamut
{
    //! \brief Red corner in the color triangle
    XY redCorner;
    //! \brief Green corner in the color triangle
    XY greenCorner;
    //! \brief Blue corner in the color triangle
    XY blueCorner;

    //! \brief Check whether \c xy is representable.
    bool contains(const XY& xy) const;
    //! \brief Correct \c xy to closest representable color.
    //! \returns \c xy if it is in the triangle, otherwise the closest point on the border.
    XY corrected(const XY& xy) const;
};

//! \brief Predefined ColorGamut%s for Hue API
namespace gamut
{
//! \brief Gamut A, used by most Color Lights
constexpr ColorGamut gamutA {{0.704f, 0.296f}, {0.2151f, 0.7106f}, {0.138f, 0.08f}};
//! \brief Gamut B, used by older Extended Color Lights
constexpr ColorGamut gamutB {{0.675f, 0.322f}, {0.409f, 0.518f}, {0.167f, 0.04f}};
//! \brief Gamut C, used by newer Extended Color Lights
constexpr ColorGamut gamutC {{0.692f, 0.308f}, {0.17f, 0.7f}, {0.153f, 0.048f}};
//! \brief Maximal gamut to be used when unknown
//!
//! \note Most of this triangle is outside of visible colors.
constexpr ColorGamut maxGamut {{1.f, 0.f}, {0.f, 1.f}, {0.f, 0.f}};
} // namespace gamut

//! \brief Color in RGB
struct RGB
{
    //! \brief Red amount from 0 to 255
    uint8_t r;
    //! \brief Green amount from 0 to 255
    uint8_t g;
    //! \brief Blue amount from 0 to 255
    uint8_t b;

    bool operator==(const RGB& other) const { return r == other.r && g == other.g && b == other.b; }
    bool operator!=(const RGB& other) const { return !(*this == other); }

    //! \brief Convert to XYBrightness without clamping
    //!
    //! Performs gamma correction so the light color matches the screen color better.
    XYBrightness toXY() const;
    //! \brief Convert to XYBrightness and clip to \c gamut
    //!
    //! Performs gamma correction so the light color matches the screen color better.
    XYBrightness toXY(const ColorGamut& gamut) const;

    //! \brief Convert to HueSaturation
    //!
    //! To get the correct color, set brightness to max(r,g,b).
    HueSaturation toHueSaturation() const;

    //! \brief Create from XYBrightness
    //!
    //! Performs gamma correction so the light color matches the screen color better.
    //! \note The conversion formula is not exact, it can be off by up to 9 for each channel.
    //! This is because the color luminosity is not saved.
    static RGB fromXY(const XYBrightness& xy);
    //! \brief Create from XYBrightness and clip to \c gamut
    //!
    //! A light may have XY set out of its range. Then this function returns the actual color
    //! the light shows rather than what it is set to.
    //! Performs gamma correction so the light color matches the screen color better.
    //! \note The conversion formula is not exact, it can be off by up to 9 for each channel.
    //! This is because the color luminosity is not saved.
    static RGB fromXY(const XYBrightness& xy, const ColorGamut& gamut);
};

//! \brief Const function that converts Kelvin to Mired.
//!
//! \param kelvin Unsigned integer value in Kelvin
//! \return Unsigned integer value in Mired
unsigned int kelvinToMired(unsigned int kelvin);

//! \brief Const function that converts Mired to Kelvin.
//!
//! \param mired Unsigned integer value in Mired
//! \return Unsigned integer value in Kelvin
unsigned int miredToKelvin(unsigned int mired);
} // namespace hueplusplus

#endif
