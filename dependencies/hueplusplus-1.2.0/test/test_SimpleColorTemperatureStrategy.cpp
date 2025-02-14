/**
    \file test_SimpleColorTemperatureStrategy.cpp
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

#include "TestTransaction.h"
#include "testhelper.h"

#include "hueplusplus/SimpleColorTemperatureStrategy.h"
#include <nlohmann/json.hpp>
#include "mocks/mock_HttpHandler.h"
#include "mocks/mock_Light.h"

using namespace hueplusplus;

TEST(SimpleColorTemperatureStrategy, setColorTemperature)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler(std::make_shared<MockHttpHandler>());
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername() + "/lights/1", nlohmann::json::object(), getBridgeIp(), 80))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(nlohmann::json::object()));
    MockLight test_light(handler);

    const std::string statePath = "/api/" + getBridgeUsername() + "/lights/1/state";

    nlohmann::json prep_ret;
    prep_ret = nlohmann::json::array();
    prep_ret[0] = nlohmann::json::object();
    prep_ret[0]["success"] = nlohmann::json::object();
    prep_ret[0]["success"]["/lights/1/state/transitiontime"] = 6;
    prep_ret[1] = nlohmann::json::object();
    prep_ret[1]["success"] = nlohmann::json::object();
    prep_ret[1]["success"]["/lights/1/state/on"] = true;
    prep_ret[2] = nlohmann::json::object();
    prep_ret[2]["success"] = nlohmann::json::object();
    prep_ret[2]["success"]["/lights/1/state/ct"] = 155;
    EXPECT_CALL(*handler, PUTJson(statePath, _, getBridgeIp(), getBridgePort())).Times(1).WillOnce(Return(prep_ret));

    test_light.getState()["state"]["on"] = true;
    test_light.getState()["state"]["ct"] = 200;
    test_light.getState()["state"]["colormode"] = "ct";
    EXPECT_EQ(true, SimpleColorTemperatureStrategy().setColorTemperature(200, 4, test_light));

    test_light.getState()["state"]["on"] = false;
    EXPECT_EQ(true, SimpleColorTemperatureStrategy().setColorTemperature(155, 6, test_light));

    prep_ret = {{{"success", {{"/lights/1/state/transitiontime", 6}}}}, {{"success", {{"/lights/1/state/ct", 153}}}}};
    EXPECT_CALL(*handler, PUTJson(statePath, _, getBridgeIp(), getBridgePort())).Times(1).WillOnce(Return(prep_ret));
    EXPECT_EQ(true, SimpleColorTemperatureStrategy().setColorTemperature(0, 6, test_light));

    prep_ret[1]["success"]["/lights/1/state/ct"] = 500;
    EXPECT_CALL(*handler, PUTJson(statePath, _, getBridgeIp(), getBridgePort())).Times(1).WillOnce(Return(prep_ret));
    EXPECT_EQ(true, SimpleColorTemperatureStrategy().setColorTemperature(600, 6, test_light));
}

TEST(SimpleColorTemperatureStrategy, alertTemperature)
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
        EXPECT_EQ(false, SimpleColorTemperatureStrategy().alertTemperature(400, light));
    }
    // on
    {
        const nlohmann::json state = {{"colormode", "ct"}, {"on", true}, {"ct", 200}};
        light.getState()["state"] = state;
        TestTransaction reverseTransaction = light.transaction().setColorTemperature(200).setTransition(1);

        EXPECT_CALL(light, setColorTemperature(400, 1)).WillOnce(Return(false));
        EXPECT_FALSE(SimpleColorTemperatureStrategy().alertTemperature(400, light));

        InSequence s;
        EXPECT_CALL(light, setColorTemperature(400, 1)).WillOnce(Invoke(setCTLambda));
        EXPECT_CALL(light, alert()).WillOnce(Return(false));
        EXPECT_FALSE(SimpleColorTemperatureStrategy().alertTemperature(400, light));

        light.getState()["state"] = state;
        EXPECT_CALL(light, setColorTemperature(400, 1)).WillOnce(Invoke(setCTLambda));
        EXPECT_CALL(light, alert()).WillOnce(Return(true));
        reverseTransaction.expectSuccessfulPut(handler, Exactly(1));
        EXPECT_TRUE(SimpleColorTemperatureStrategy().alertTemperature(400, light));
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // off
    {
        const nlohmann::json state = {{"colormode", "ct"}, {"on", false}, {"ct", 200}};
        light.getState()["state"] = state;
        TestTransaction reverseTransaction = light.transaction().setColorTemperature(200).setOn(false).setTransition(1);

        EXPECT_CALL(light, setColorTemperature(400, 1)).WillOnce(Invoke(setCTLambda));
        EXPECT_CALL(light, alert()).WillOnce(Return(true));
        reverseTransaction.expectSuccessfulPut(handler, Exactly(1));
        EXPECT_TRUE(SimpleColorTemperatureStrategy().alertTemperature(400, light));
        Mock::VerifyAndClearExpectations(handler.get());
    }
}

TEST(SimpleColorTemperatureStrategy, getColorTemperature)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler(std::make_shared<MockHttpHandler>());
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername() + "/lights/1", nlohmann::json::object(), getBridgeIp(), 80))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(nlohmann::json::object()));
    MockLight test_light(handler);

    test_light.getState()["state"]["ct"] = 200;
    EXPECT_EQ(200, SimpleColorTemperatureStrategy().getColorTemperature(test_light));
    test_light.getState()["state"]["ct"] = 500;
    EXPECT_EQ(500, SimpleColorTemperatureStrategy().getColorTemperature(static_cast<const Light>(test_light)));
}
