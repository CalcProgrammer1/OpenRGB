/**
    \file test_ExtendedColorHueStrategy.cpp
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

#include <memory>
#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "TestTransaction.h"
#include "testhelper.h"

#include "hueplusplus/ExtendedColorHueStrategy.h"
#include <nlohmann/json.hpp>
#include "mocks/mock_HttpHandler.h"
#include "mocks/mock_Light.h"

using namespace hueplusplus;

TEST(ExtendedColorHueStrategy, alertHueSaturation)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler(std::make_shared<MockHttpHandler>());
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername() + "/lights/1", nlohmann::json::object(), getBridgeIp(), 80))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(nlohmann::json::object()));
    MockLight light(handler);

    const HueSaturation hueSat {200, 100};
    // Needs to update the state so transactions are correctly trimmed
    const auto setColorLambda = [&](const HueSaturation& hueSat, int transition) {
        light.getState()["state"]["colormode"] = "hs";
        light.getState()["state"]["on"] = true;
        light.getState()["state"]["hue"] = hueSat.hue;
        light.getState()["state"]["sat"] = hueSat.saturation;
        return true;
    };
    // Invalid state
    {
        light.getState()["state"]["colormode"] = "invalid";
        light.getState()["state"]["on"] = false;
        EXPECT_FALSE(ExtendedColorHueStrategy().alertHueSaturation(hueSat, light));
    }
    // Colormode not ct is forwarded to SimpleColorHueStrategy
    {
        const nlohmann::json state = {{"colormode", "hs"}, {"on", true}, {"xy", {0.1, 0.1}}, {"hue", 300}, {"sat", 100},
            {"bri", 254}, {"ct", 300}};
        light.getState()["state"] = state;
        EXPECT_CALL(Const(light), getColorHueSaturation())
            .Times(AnyNumber())
            .WillRepeatedly(Return(HueSaturation {300, 100}));
        TestTransaction reverseTransaction = light.transaction().setColorHue(300).setTransition(1);
        InSequence s;
        EXPECT_CALL(light, setColorHueSaturation(hueSat, 1)).WillOnce(Invoke(setColorLambda));
        EXPECT_CALL(light, alert()).WillOnce(Return(true));
        reverseTransaction.expectSuccessfulPut(handler, Exactly(1));
        EXPECT_TRUE(ExtendedColorHueStrategy().alertHueSaturation(hueSat, light));
        Mock::VerifyAndClearExpectations(handler.get());
    }

    // Colormode ct
    {
        const nlohmann::json state
            = {{"colormode", "ct"}, {"on", true}, {"xy", {0.1, 0.1}}, {"sat", 100}, {"bri", 254}, {"ct", 300}};
        light.getState()["state"] = state;
        TestTransaction reverseTransaction = light.transaction().setColorTemperature(300).setTransition(1);

        InSequence s;
        EXPECT_CALL(light, setColorHueSaturation(hueSat, 1)).WillOnce(Invoke(setColorLambda));
        EXPECT_CALL(light, alert()).WillOnce(Return(false));
        EXPECT_FALSE(ExtendedColorHueStrategy().alertHueSaturation(hueSat, light));
        light.getState()["state"] = state;
        Mock::VerifyAndClearExpectations(handler.get());

        EXPECT_CALL(light, setColorHueSaturation(hueSat, 1)).WillOnce(Invoke(setColorLambda));
        EXPECT_CALL(light, alert()).WillOnce(Return(true));
        reverseTransaction.expectSuccessfulPut(handler, Exactly(1));
        EXPECT_TRUE(ExtendedColorHueStrategy().alertHueSaturation(hueSat, light));
        Mock::VerifyAndClearExpectations(handler.get());
    }

    // Colormode ct, off
    {
        const nlohmann::json state
            = {{"colormode", "ct"}, {"on", false}, {"xy", {0., 1.}}, {"sat", 100}, {"bri", 254}, {"ct", 300}};
        light.getState()["state"] = state;

        TestTransaction reverseTransaction = light.transaction().setColorTemperature(300).setOn(false).setTransition(1);
        InSequence s;
        EXPECT_CALL(light, setColorHueSaturation(hueSat, 1)).WillOnce(Invoke(setColorLambda));
        EXPECT_CALL(light, alert()).WillOnce(Return(true));
        reverseTransaction.expectSuccessfulPut(handler, Exactly(1));
        EXPECT_TRUE(ExtendedColorHueStrategy().alertHueSaturation(hueSat, light));
        Mock::VerifyAndClearExpectations(handler.get());
    }
}

TEST(ExtendedColorHueStrategy, alertXY)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler(std::make_shared<MockHttpHandler>());
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername() + "/lights/1", nlohmann::json::object(), getBridgeIp(), 80))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(nlohmann::json::object()));
    MockLight light(handler);

    const XYBrightness xy {{0.1f, 0.1f}, 1.f};
    // Needs to update the state so transactions are correctly trimmed
    const auto setColorLambda = [&](const XYBrightness& xy, int transition) {
        light.getState()["state"]["colormode"] = "xy";
        light.getState()["state"]["on"] = true;
        light.getState()["state"]["xy"] = {xy.xy.x, xy.xy.y};
        light.getState()["state"]["bri"] = static_cast<int>(std::round(xy.brightness * 254.f));
        return true;
    };
    // Invalid colormode
    {
        light.getState()["state"]["colormode"] = "invalid";
        light.getState()["state"]["on"] = false;
        EXPECT_FALSE(ExtendedColorHueStrategy().alertXY({{0.1f, 0.1f}, 1.f}, light));
    }
    // Colormode not ct is forwarded to SimpleColorHueStrategy
    {
        const nlohmann::json state = {{"colormode", "hs"}, {"on", true}, {"xy", {0.1, 0.1}}, {"hue", 200}, {"sat", 100},
            {"bri", 254}, {"ct", 300}};
        light.getState()["state"] = state;
        EXPECT_CALL(Const(light), getBrightness()).Times(AnyNumber()).WillRepeatedly(Return(254));
        HueSaturation hueSat {200, 100};
        EXPECT_CALL(Const(light), getColorHueSaturation()).Times(AnyNumber()).WillRepeatedly(Return(hueSat));

        TestTransaction reverseTransaction = light.transaction().setColor(hueSat).setTransition(1);

        InSequence s;
        EXPECT_CALL(light, setColorXY(xy, 1)).WillOnce(Invoke(setColorLambda));
        EXPECT_CALL(light, alert()).WillOnce(Return(true));
        reverseTransaction.expectSuccessfulPut(handler, Exactly(1));
        EXPECT_TRUE(SimpleColorHueStrategy().alertXY(xy, light));
        Mock::VerifyAndClearExpectations(handler.get());
    }

    // Colormode ct
    {
        const nlohmann::json state
            = { {"colormode", "ct"}, {"on", true}, {"xy", {0.1, 0.1}}, {"sat", 100}, {"bri", 128}, {"ct", 300} };
        light.getState()["state"] = state;        
        EXPECT_CALL(Const(light), getBrightness()).Times(AnyNumber()).WillRepeatedly(Return(128));

        TestTransaction reverseTransaction = light.transaction().setColorTemperature(300).setBrightness(128).setTransition(1);

        InSequence s;
        EXPECT_CALL(light, setColorXY(xy, 1)).WillOnce(Invoke(setColorLambda));
        EXPECT_CALL(light, alert()).WillOnce(Return(false));
        EXPECT_FALSE(ExtendedColorHueStrategy().alertXY(xy, light));
        light.getState()["state"] = state;
        Mock::VerifyAndClearExpectations(handler.get());

        EXPECT_CALL(light, setColorXY(xy, 1)).WillOnce(Invoke(setColorLambda));
        EXPECT_CALL(light, alert()).WillOnce(Return(true));
        reverseTransaction.expectSuccessfulPut(handler, Exactly(1));
        EXPECT_TRUE(ExtendedColorHueStrategy().alertXY(xy, light));
        Mock::VerifyAndClearExpectations(handler.get());
    }

    // Colormode ct, off
    {
        const nlohmann::json state
            = { {"colormode", "ct"}, {"on", false}, {"xy", {0., 1.}}, {"sat", 100}, {"bri", 254}, {"ct", 300} };
        light.getState()["state"] = state;
        EXPECT_CALL(Const(light), getBrightness()).Times(AnyNumber()).WillRepeatedly(Return(254));

        TestTransaction reverseTransaction = light.transaction().setColorTemperature(300).setOn(false).setTransition(1);
        InSequence s;
        EXPECT_CALL(light, setColorXY(xy, 1)).WillOnce(Invoke(setColorLambda));
        EXPECT_CALL(light, alert()).WillOnce(Return(true));
        reverseTransaction.expectSuccessfulPut(handler, Exactly(1));
        EXPECT_TRUE(ExtendedColorHueStrategy().alertXY(xy, light));
        Mock::VerifyAndClearExpectations(handler.get());
    }
}
