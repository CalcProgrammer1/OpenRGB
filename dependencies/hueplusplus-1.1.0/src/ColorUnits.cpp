/**
    \file ColorUnits.cpp
    Copyright Notice\n
    Copyright (C) 2020  Jan Rogall		- developer\n

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

#include <algorithm>
#include <cmath>

#include <hueplusplus/ColorUnits.h>

namespace hueplusplus
{
namespace
{
float sign(const XY& p0, const XY& p1, const XY& p2)
{
    return (p0.x - p2.x) * (p1.y - p2.y) - (p1.x - p2.x) * (p0.y - p2.y);
}

bool isRightOf(const XY& xy, const XY& p1, const XY& p2)
{
    return sign(xy, p1, p2) < 0;
}

XY projectOntoLine(const XY& xy, const XY& p1, const XY& p2)
{
    // Using dot product to project onto line
    // Vector AB = B - A
    // Vector AX = X - A
    // Projected length l = (AX dot AB) / len(AB)
    // Result: E = A + l*AB/len(AB) = A + AB * (AX dot AB) / (len(AB))^2

    const float abX = p2.x - p1.x;
    const float abY = p2.y - p1.y;
    const float lenABSquared = abX * abX + abY * abY;

    const float dot = (xy.x - p1.x) * abX + (xy.y - p1.y) * abY;
    const float eX = p1.x + abX * dot / lenABSquared;
    const float eY = p1.y + abY * dot / lenABSquared;
    return XY {eX, eY};
}
} // namespace

bool ColorGamut::contains(const XY& xy) const
{
    return !isRightOf(xy, redCorner, greenCorner) && !isRightOf(xy, greenCorner, blueCorner)
        && !isRightOf(xy, blueCorner, redCorner);
}

XY ColorGamut::corrected(const XY& xy) const
{
    // red, green and blue are in counterclockwise orientation
    if (isRightOf(xy, redCorner, greenCorner))
    {
        // Outside of triangle, check whether to use nearest corner or point on line
        if (isRightOf(xy, greenCorner, blueCorner))
        {
            // Point is outside of red-green line, closest to green corner
            return greenCorner;
        }
        else if (isRightOf(xy, blueCorner, redCorner))
        {
            // Point is outside of red-green line, closest to red corner
            return redCorner;
        }
        else
        {
            // Point is closest to line, project onto it
            return projectOntoLine(xy, redCorner, greenCorner);
        }
    }
    else if (isRightOf(xy, greenCorner, blueCorner))
    {
        // Green corner already checked above
        if (isRightOf(xy, blueCorner, redCorner))
        {
            // Point is outside of green-blue line, closest to blue corner
            return blueCorner;
        }
        else
        {
            return projectOntoLine(xy, greenCorner, blueCorner);
        }
    }
    else if (isRightOf(xy, blueCorner, redCorner))
    {
        // All corners already checked
        return projectOntoLine(xy, blueCorner, redCorner);
    }
    return xy;
}

XYBrightness RGB::toXY() const
{
    if (r == 0 && g == 0 && b == 0)
    {
        // Return white with minimum brightness
        return XYBrightness {XY {0.32272673f, 0.32902291f}, 0.f};
    }
    const float red = r / 255.f;
    const float green = g / 255.f;
    const float blue = b / 255.f;

    const float redCorrected = (red > 0.04045f) ? pow((red + 0.055f) / (1.0f + 0.055f), 2.4f) : (red / 12.92f);
    const float greenCorrected = (green > 0.04045f) ? pow((green + 0.055f) / (1.0f + 0.055f), 2.4f) : (green / 12.92f);
    const float blueCorrected = (blue > 0.04045f) ? pow((blue + 0.055f) / (1.0f + 0.055f), 2.4f) : (blue / 12.92f);

    const float X = redCorrected * 0.664511f + greenCorrected * 0.154324f + blueCorrected * 0.162028f;
    const float Y = redCorrected * 0.283881f + greenCorrected * 0.668433f + blueCorrected * 0.047685f;
    const float Z = redCorrected * 0.000088f + greenCorrected * 0.072310f + blueCorrected * 0.986039f;

    const float x = X / (X + Y + Z);
    const float y = Y / (X + Y + Z);
    // Set brightness to the brightest channel value (rather than average of them),
    // so full red/green/blue can be displayed
    return XYBrightness {XY {x, y}, std::max({red, green, blue})};
}

XYBrightness RGB::toXY(const ColorGamut& gamut) const
{
    XYBrightness xy = toXY();
    if (!gamut.contains(xy.xy))
    {
        xy.xy = gamut.corrected(xy.xy);
    }
    return xy;
}

HueSaturation RGB::toHueSaturation() const
{
    const uint8_t cmax = std::max(r, std::max(g, b));
    const uint8_t cmin = std::min(r, std::min(g, b));
    const float diff = cmax - cmin;

    int h = -1;
    int s = -1;

    if (cmax == cmin)
    {
        h = 0;
    }
    else if (cmax == r)
    {
        h = (int)(9307 * ((g - b) / diff) + 65535) % 65535;
    }
    else if (cmax == g)
    {
        h = (int)(12750 * ((b - r) / diff) + 25500) % 65535;
    }
    else if (cmax == b)
    {
        h = (int)(10710 * ((r - g) / diff) + 46920) % 65535;
    }

    if (cmax == 0)
    {
        s = 0;
    }
    else
    {
        s = std::round((diff / cmax) * 254);
    }

    return {h, s};
}

RGB RGB::fromXY(const XYBrightness& xy)
{
    if (xy.brightness < 1e-4)
    {
        return RGB {0, 0, 0};
    }
    const float z = 1.f - xy.xy.x - xy.xy.y;
    // use a fixed luminosity and rescale the resulting rgb values using brightness
    // randomly sampled conversions shown a minimum difference between original values
    // and values after rgb -> xy -> rgb conversion for Y = 0.3
    // (r-r')^2, (g-g')^2, (b-b')^2:
    // 4.48214,  4.72039,  3.12141
    // Max. Difference:
    // 9,        9,        8
    const float Y = 0.3f;
    const float X = (Y / xy.xy.y) * xy.xy.x;
    const float Z = (Y / xy.xy.y) * z;

    const float r = X * 1.656492f - Y * 0.354851f - Z * 0.255038f;
    const float g = -X * 0.707196f + Y * 1.655397f + Z * 0.036152f;
    const float b = X * 0.051713f - Y * 0.121364f + Z * 1.011530f;

    // Reverse gamma correction
    const float gammaR = r <= 0.0031308f ? 12.92f * r : (1.0f + 0.055f) * pow(r, (1.0f / 2.4f)) - 0.055f;
    const float gammaG = g <= 0.0031308f ? 12.92f * g : (1.0f + 0.055f) * pow(g, (1.0f / 2.4f)) - 0.055f;
    const float gammaB = b <= 0.0031308f ? 12.92f * b : (1.0f + 0.055f) * pow(b, (1.0f / 2.4f)) - 0.055f;

    // Scale color values so that the brightness matches
    const float maxColor = std::max({gammaR, gammaG, gammaB});
    if (maxColor < 1e-4)
    {
        // Low color values, out of gamut?
        return RGB {0, 0, 0};
    }
    const float rScaled = gammaR / maxColor * xy.brightness * 255.f;
    const float gScaled = gammaG / maxColor * xy.brightness * 255.f;
    const float bScaled = gammaB / maxColor * xy.brightness * 255.f;

    return RGB {static_cast<uint8_t>(std::round(std::max(0.f, rScaled))),
        static_cast<uint8_t>(std::round(std::max(0.f, gScaled))),
        static_cast<uint8_t>(std::round(std::max(0.f, bScaled)))};
}

RGB RGB::fromXY(const XYBrightness& xy, const ColorGamut& gamut)
{
    if (gamut.contains(xy.xy))
    {
        return fromXY(xy);
    }
    else
    {
        return fromXY(XYBrightness {gamut.corrected(xy.xy), xy.brightness});
    }
}

unsigned int kelvinToMired(unsigned int kelvin)
{
    return int(std::round(1000000.f / kelvin));
}

unsigned int miredToKelvin(unsigned int mired)
{
    return int(std::round(1000000.f / mired));
}

} // namespace hueplusplus