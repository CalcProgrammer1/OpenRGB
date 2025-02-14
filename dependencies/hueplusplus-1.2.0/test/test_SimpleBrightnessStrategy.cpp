/**
    \file test_SimpleBrightnessStrategy.cpp
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

#include <iostream>
#include <memory>
#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "testhelper.h"

#include "hueplusplus/SimpleBrightnessStrategy.h"
#include <nlohmann/json.hpp>
#include "mocks/mock_HttpHandler.h"
#include "mocks/mock_Light.h"

using namespace hueplusplus;

TEST(SimpleBrightnessStrategy, setBrightness)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler(std::make_shared<MockHttpHandler>());
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername() + "/lights/1", nlohmann::json::object(), getBridgeIp(), 80))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(nlohmann::json::object()));
    MockLight test_light(handler);

    const std::string statePath = "/api/" + getBridgeUsername() + "/lights/1/state";

    nlohmann::json prep_ret
        = {{{"success", {{"/lights/1/state/on", false}}}}, {{"success", {{"/lights/1/state/bri", 0}}}}};
    EXPECT_CALL(*handler, PUTJson(statePath, _, getBridgeIp(), getBridgePort())).Times(1).WillOnce(Return(prep_ret));
    test_light.getState()["state"]["on"] = true;
    EXPECT_EQ(true, SimpleBrightnessStrategy().setBrightness(0, 4, test_light));
    // Only set brightness, already off
    test_light.getState()["state"]["on"] = false;
    test_light.getState()["state"].erase("bri");
    prep_ret = {{{"success", {{"/lights/1/state/bri", 0}}}}};
    EXPECT_CALL(*handler, PUTJson(statePath, _, getBridgeIp(), getBridgePort())).Times(1).WillOnce(Return(prep_ret));
    EXPECT_EQ(true, SimpleBrightnessStrategy().setBrightness(0, 4, test_light));

    prep_ret = {{{"success", {{"/lights/1/state/on", true}}}}, {{"success", {{"/lights/1/state/bri", 50}}}}};
    EXPECT_CALL(*handler, PUTJson(statePath, _, getBridgeIp(), getBridgePort())).Times(1).WillOnce(Return(prep_ret));
    test_light.getState()["state"]["bri"] = 0;
    EXPECT_EQ(true, SimpleBrightnessStrategy().setBrightness(50, 6, test_light));
    test_light.getState()["state"]["on"] = true;
    test_light.getState()["state"]["bri"] = 50;
    // No request because state matches
    EXPECT_EQ(true, SimpleBrightnessStrategy().setBrightness(50, 6, test_light));

    prep_ret[1]["success"]["/lights/1/state/bri"] = 254;
    EXPECT_CALL(*handler, PUTJson(statePath, _, getBridgeIp(), getBridgePort())).Times(1).WillOnce(Return(prep_ret));
    test_light.getState()["state"]["on"] = false;
    test_light.getState()["state"]["bri"] = 50;
    EXPECT_EQ(true, SimpleBrightnessStrategy().setBrightness(255, 6, test_light));
}

TEST(SimpleBrightnessStrategy, getBrightness)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler(std::make_shared<MockHttpHandler>());
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername() + "/lights/1", nlohmann::json::object(), getBridgeIp(), 80))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(nlohmann::json::object()));
    MockLight test_light(handler);

    test_light.getState()["state"]["bri"] = 200;
    EXPECT_EQ(200, SimpleBrightnessStrategy().getBrightness(test_light));
    test_light.getState()["state"]["bri"] = 0;
    EXPECT_EQ(0, SimpleBrightnessStrategy().getBrightness(static_cast<const Light>(test_light)));
}
