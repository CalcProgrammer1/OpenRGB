/**
    \file test_ExtendedColorTemperatureStrategy.cpp
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

#include "hueplusplus/ExtendedColorTemperatureStrategy.h"
#include <nlohmann/json.hpp>
#include "mocks/mock_HttpHandler.h"
#include "mocks/mock_Light.h"

using namespace hueplusplus;

TEST(ExtendedColorTemperatureStrategy, alertTemperature)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler(std::make_shared<MockHttpHandler>());
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername() + "/lights/1", nlohmann::json::object(), getBridgeIp(), 80))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(nlohmann::json::object()));
    MockLight light(handler);

    const auto setCTLambda = [&](unsigned int ct, int transition) {
        light.getState()["state"]["colormode"] = "ct";
        light.getState()["state"]["on"] = true;
        light.getState()["state"]["ct"] = ct;
        return true;
    };

    // Invalid colormode
    {
        light.getState()["state"]["colormode"] = "invalid";
        light.getState()["state"]["on"] = false;
        EXPECT_EQ(false, ExtendedColorTemperatureStrategy().alertTemperature(400, light));
    }
    // Colormode ct forwarded to SimpleColorTemperatureStrategy
    {
        const nlohmann::json state = {{"colormode", "ct"}, {"on", true}, {"ct", 200}, {"xy", {0.1, 0.1}}, {"hue", 300},
            {"sat", 100}, {"bri", 254}};
        light.getState()["state"] = state;
        TestTransaction reverseTransaction = light.transaction().setColorTemperature(200).setTransition(1);

        light.getState()["state"] = state;
        EXPECT_CALL(light, setColorTemperature(400, 1)).WillOnce(Invoke(setCTLambda));
        EXPECT_CALL(light, alert()).WillOnce(Return(true));
        reverseTransaction.expectSuccessfulPut(handler, Exactly(1));
        EXPECT_TRUE(ExtendedColorTemperatureStrategy().alertTemperature(400, light));
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Colormode xy
    {
        const nlohmann::json state = {{"colormode", "xy"}, {"on", true}, {"ct", 200}, {"xy", {0.1, 0.1}}, {"hue", 300},
            {"sat", 100}, {"bri", 254}};
        light.getState()["state"] = state;
        EXPECT_CALL(Const(light), getColorXY()).Times(AnyNumber()).WillRepeatedly(Return(XYBrightness{ {0.1f,0.1f},1.f }));
        TestTransaction reverseTransaction = light.transaction().setColor(XY{ 0.1f,0.1f }).setTransition(1);

        EXPECT_CALL(light, setColorTemperature(400, 1)).WillOnce(Return(false));
        EXPECT_FALSE(ExtendedColorTemperatureStrategy().alertTemperature(400, light));

        InSequence s;
        EXPECT_CALL(light, setColorTemperature(400, 1)).WillOnce(Invoke(setCTLambda));
        EXPECT_CALL(light, alert()).WillOnce(Return(false));
        EXPECT_FALSE(ExtendedColorTemperatureStrategy().alertTemperature(400, light));

        light.getState()["state"] = state;
        EXPECT_CALL(light, setColorTemperature(400, 1)).WillOnce(Invoke(setCTLambda));
        EXPECT_CALL(light, alert()).WillOnce(Return(true));
        reverseTransaction.expectSuccessfulPut(handler, Exactly(1));
        EXPECT_TRUE(ExtendedColorTemperatureStrategy().alertTemperature(400, light));
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Colormode hs
    {
        const nlohmann::json state = { {"colormode", "hs"}, {"on", true}, {"ct", 200}, {"xy", {0.1, 0.1}}, {"hue", 300},
            {"sat", 100}, {"bri", 254} };
        light.getState()["state"] = state;
        EXPECT_CALL(Const(light), getColorHueSaturation()).Times(AnyNumber()).WillRepeatedly(Return(HueSaturation{ 300,200 }));
        TestTransaction reverseTransaction = light.transaction().setColor(HueSaturation{ 300,200 }).setTransition(1);

        EXPECT_CALL(light, setColorTemperature(400, 1)).WillOnce(Return(false));
        EXPECT_FALSE(ExtendedColorTemperatureStrategy().alertTemperature(400, light));

        InSequence s;
        EXPECT_CALL(light, setColorTemperature(400, 1)).WillOnce(Invoke(setCTLambda));
        EXPECT_CALL(light, alert()).WillOnce(Return(false));
        EXPECT_FALSE(ExtendedColorTemperatureStrategy().alertTemperature(400, light));

        light.getState()["state"] = state;
        EXPECT_CALL(light, setColorTemperature(400, 1)).WillOnce(Invoke(setCTLambda));
        EXPECT_CALL(light, alert()).WillOnce(Return(true));
        reverseTransaction.expectSuccessfulPut(handler, Exactly(1));
        EXPECT_TRUE(ExtendedColorTemperatureStrategy().alertTemperature(400, light));
        Mock::VerifyAndClearExpectations(handler.get());
    }
}
