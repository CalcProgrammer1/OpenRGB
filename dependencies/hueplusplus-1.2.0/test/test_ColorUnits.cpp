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

#include <random>

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
        EXPECT_FLOAT_EQ(xy.brightness, 1.f);
    }
    {
        const RGB red {255, 0, 0};
        XYBrightness xy = red.toXY(gamut::gamutC);
        EXPECT_FLOAT_EQ(xy.xy.x, 0.69557756f);
        EXPECT_FLOAT_EQ(xy.xy.y, 0.30972576f);
        EXPECT_FLOAT_EQ(xy.brightness, 1.f);
    }
    {
        const RGB white {255, 255, 255};
        XYBrightness xy = white.toXY();
        EXPECT_FLOAT_EQ(xy.xy.x, 0.32272673f);
        EXPECT_FLOAT_EQ(xy.xy.y, 0.32902291f);
        EXPECT_FLOAT_EQ(xy.brightness, 1.f);
    }
    {
        const RGB white {255, 255, 255};
        XYBrightness xy = white.toXY(gamut::gamutA);
        EXPECT_FLOAT_EQ(xy.xy.x, 0.32272673f);
        EXPECT_FLOAT_EQ(xy.xy.y, 0.32902291f);
        EXPECT_FLOAT_EQ(xy.brightness, 1.f);
    }
    {
        const RGB white {255, 255, 255};
        XYBrightness xy = white.toXY(gamut::gamutB);
        EXPECT_FLOAT_EQ(xy.xy.x, 0.32272673f);
        EXPECT_FLOAT_EQ(xy.xy.y, 0.32902291f);
        EXPECT_FLOAT_EQ(xy.brightness, 1.f);
    }
    {
        const RGB black {0, 0, 0};
        XYBrightness xy = black.toXY(gamut::maxGamut);
        EXPECT_FLOAT_EQ(xy.xy.x, 0.32272673f);
        EXPECT_FLOAT_EQ(xy.xy.y, 0.32902291f);
        EXPECT_FLOAT_EQ(xy.brightness, 0.0f);
    }
}

TEST(RGB, toHueSaturation)
{
    {
        const RGB red {255, 0, 0};
        HueSaturation hs = red.toHueSaturation();
        EXPECT_EQ(0, hs.hue);
        EXPECT_EQ(254, hs.saturation);
    }
    {
        const RGB darkGreen {64, 128, 128};
        HueSaturation hs = darkGreen.toHueSaturation();
        EXPECT_EQ(38250, hs.hue);
        EXPECT_EQ(127, hs.saturation);
    }
    {
        const RGB white {255, 255, 255};
        HueSaturation hs = white.toHueSaturation();
        EXPECT_EQ(0, hs.hue);
        EXPECT_EQ(0, hs.saturation);
    }
    {
        const RGB black {0, 0, 0};
        HueSaturation hs = black.toHueSaturation();
        EXPECT_EQ(0, hs.hue);
        EXPECT_EQ(0, hs.saturation);
    }
}

TEST(RGB, fromXY)
{
    {
        const XYBrightness xyRed {{0.70060623f, 0.299301f}, 1.f};
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
        const XYBrightness xyWhite {{0.32272673f, 0.32902291f}, 1.f};
        const RGB white = RGB::fromXY(xyWhite);
        EXPECT_EQ(255, white.r);
        EXPECT_EQ(255, white.g);
        EXPECT_EQ(255, white.b);
        const XYBrightness reversed = white.toXY();
        EXPECT_FLOAT_EQ(xyWhite.xy.x, reversed.xy.x);
        EXPECT_FLOAT_EQ(xyWhite.xy.y, reversed.xy.y);
        EXPECT_FLOAT_EQ(xyWhite.brightness, reversed.brightness);
    }
    {
        const XYBrightness xyRed {{0.70060623f, 0.299301f}, 1.f};
        const RGB red = RGB::fromXY(xyRed, gamut::gamutB);
        const RGB red2 = RGB::fromXY({gamut::gamutB.corrected(xyRed.xy), xyRed.brightness});
        EXPECT_EQ(red2.r, red.r);
        EXPECT_EQ(red2.g, red.g);
        EXPECT_EQ(red2.b, red.b);
    }

    // Statistical tests of conversion accuracy
    // Fixed seed so the tests dont fail randomly
    std::mt19937 rng {12374682};
    std::uniform_int_distribution<int> dist(0, 255);

    uint64_t N = 1000;

    uint64_t totalDiffR = 0;
    uint64_t totalDiffG = 0;
    uint64_t totalDiffB = 0;
    int maxDiffR = 0;
    int maxDiffG = 0;
    int maxDiffB = 0;
    for (int i = 0; i < N; ++i)
    {
        const RGB rgb {
            static_cast<uint8_t>(dist(rng)), static_cast<uint8_t>(dist(rng)), static_cast<uint8_t>(dist(rng))};
        const XYBrightness xy = rgb.toXY();
        const RGB back = RGB::fromXY(xy);
        int diffR = (rgb.r - back.r) * (rgb.r - back.r);
        int diffG = (rgb.g - back.g) * (rgb.g - back.g);
        int diffB = (rgb.b - back.b) * (rgb.b - back.b);
        totalDiffR += diffR;
        totalDiffG += diffG;
        totalDiffB += diffB;
        maxDiffR = std::max(diffR, maxDiffR);
        maxDiffG = std::max(diffG, maxDiffG);
        maxDiffB = std::max(diffB, maxDiffB);
    }
    float varR = (float)totalDiffR / N;
    float varG = (float)totalDiffG / N;
    float varB = (float)totalDiffB / N;
    EXPECT_LT(varR, 5.f);
    EXPECT_LT(varG, 5.f);
    EXPECT_LT(varB, 4.f);
    EXPECT_LE(maxDiffR, 81);
    EXPECT_LE(maxDiffG, 81);
    EXPECT_LE(maxDiffB, 64);
}

TEST(ColorUnits, kelvinToMired)
{
    EXPECT_EQ(10000, kelvinToMired(100));
    EXPECT_EQ(500, kelvinToMired(2000));
    EXPECT_EQ(303, kelvinToMired(3300));
    EXPECT_EQ(250, kelvinToMired(4000));
    EXPECT_EQ(200, kelvinToMired(5000));
    EXPECT_EQ(167, kelvinToMired(6000));
}

TEST(ColorUnits, miredToKelvin)
{
    EXPECT_EQ(100, miredToKelvin(10000));
    EXPECT_EQ(2000, miredToKelvin(500));
    EXPECT_EQ(3300, miredToKelvin(303));
    EXPECT_EQ(4000, miredToKelvin(250));
    EXPECT_EQ(5000, miredToKelvin(200));
    EXPECT_EQ(6024, miredToKelvin(166)); // 6000 kelvin should be 166 mired, but is rounded
}