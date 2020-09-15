/**
    \file test_ColorUnits.cpp
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

#include <hueplusplus/ColorUnits.h>

#include <gtest/gtest.h>

using namespace hueplusplus;

TEST(ColorGamut, contains)
{
    ColorGamut gamut = gamut::maxGamut;

    EXPECT_TRUE(gamut.contains({0.f, 0.5f}));
    EXPECT_TRUE(gamut.contains({1.f, 0.f}));
    EXPECT_TRUE(gamut.contains({0.5f, 0.5f}));
    EXPECT_TRUE(gamut.contains({0.f, 1.f}));
    EXPECT_TRUE(gamut.contains({0.f, 0.f}));
    EXPECT_FALSE(gamut.contains({1.f, 1.f}));
    EXPECT_FALSE(gamut.contains({-1.f, 1.f}));
}

TEST(ColorGamut, corrected)
{
    ColorGamut gamut = gamut::maxGamut;

    {
        const XY xy {0.f, 0.5f};
        const XY result = gamut.corrected(xy);
        EXPECT_FLOAT_EQ(xy.x, result.x);
        EXPECT_FLOAT_EQ(xy.y, result.y);
    }
    {
        const XY xy {0.f, 1.f};
        const XY result = gamut.corrected(xy);
        EXPECT_FLOAT_EQ(xy.x, result.x);
        EXPECT_FLOAT_EQ(xy.y, result.y);
    }
    {
        const XY xy {1.f, 1.f};
        const XY result = gamut.corrected(xy);
        EXPECT_FLOAT_EQ(0.5f, result.x);
        EXPECT_FLOAT_EQ(0.5f, result.y);
    }
    {
        const XY xy {1.f, -1.f};
        const XY result = gamut.corrected(xy);
        EXPECT_FLOAT_EQ(1.f, result.x);
        EXPECT_FLOAT_EQ(0.f, result.y);
    }
}

TEST(RGB, toXY)
{
    {
        const RGB red {255, 0, 0};
        XYBrightness xy = red.toXY();
        EXPECT_FLOAT_EQ(xy.xy.x, 0.70060623f);
        EXPECT_FLOAT_EQ(xy.xy.y, 0.299301f);
        EXPECT_FLOAT_EQ(xy.brightness, 0.28388101f);
    }
    {
        const RGB red {255, 0, 0};
        XYBrightness xy = red.toXY(gamut::gamutC);
        EXPECT_FLOAT_EQ(xy.xy.x, 0.69557756f);
        EXPECT_FLOAT_EQ(xy.xy.y, 0.30972576f);
        EXPECT_FLOAT_EQ(xy.brightness, 0.28388101f);
    }
    {
        const RGB white {255, 255, 255};
        XYBrightness xy = white.toXY();
        EXPECT_FLOAT_EQ(xy.xy.x, 0.32272673f);
        EXPECT_FLOAT_EQ(xy.xy.y, 0.32902291f);
        EXPECT_FLOAT_EQ(xy.brightness, 0.99999905f);
    }
    {
        const RGB white {255, 255, 255};
        XYBrightness xy = white.toXY(gamut::gamutA);
        EXPECT_FLOAT_EQ(xy.xy.x, 0.32272673f);
        EXPECT_FLOAT_EQ(xy.xy.y, 0.32902291f);
        EXPECT_FLOAT_EQ(xy.brightness, 0.99999905f);
    }
    {
        const RGB white {255, 255, 255};
        XYBrightness xy = white.toXY(gamut::gamutB);
        EXPECT_FLOAT_EQ(xy.xy.x, 0.32272673f);
        EXPECT_FLOAT_EQ(xy.xy.y, 0.32902291f);
        EXPECT_FLOAT_EQ(xy.brightness, 0.99999905f);
    }
}

TEST(RGB, fromXY)
{
    {
        const XYBrightness xyRed {{0.70060623f, 0.299301f}, 0.28388101f};
        const RGB red = RGB::fromXY(xyRed);
        EXPECT_EQ(255, red.r);
        EXPECT_EQ(0, red.g);
        EXPECT_EQ(0, red.b);
        const XYBrightness reversed = red.toXY();
        EXPECT_FLOAT_EQ(xyRed.xy.x, reversed.xy.x);
        EXPECT_FLOAT_EQ(xyRed.xy.y, reversed.xy.y);
        EXPECT_FLOAT_EQ(xyRed.brightness, reversed.brightness);
    }
    {
        const XYBrightness xyRed {{0.70060623f, 0.299301f}, 0.28388101f};
        const RGB red = RGB::fromXY(xyRed, gamut::gamutB);
        EXPECT_EQ(242, red.r);
        EXPECT_EQ(63, red.g);
        EXPECT_EQ(208, red.b);
    }
}
