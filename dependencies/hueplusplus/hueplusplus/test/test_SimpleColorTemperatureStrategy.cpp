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

#include "testhelper.h"

#include "../include/SimpleColorTemperatureStrategy.h"
#include "../include/json/json.hpp"
#include "mocks/mock_HttpHandler.h"
#include "mocks/mock_HueLight.h"

TEST(SimpleColorTemperatureStrategy, setColorTemperature)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler(std::make_shared<MockHttpHandler>());
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername() + "/lights/1", nlohmann::json::object(), getBridgeIp(), 80))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(nlohmann::json::object()));
    MockHueLight test_light(handler);
    EXPECT_CALL(test_light, refreshState()).Times(AtLeast(1)).WillRepeatedly(Return());
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
    EXPECT_CALL(test_light, SendPutRequest(_, "/state", _)).Times(1).WillOnce(Return(prep_ret));

    test_light.getState()["state"]["on"] = true;
    test_light.getState()["state"]["ct"] = 200;
    EXPECT_EQ(true, SimpleColorTemperatureStrategy().setColorTemperature(200, 4, test_light));

    test_light.getState()["state"]["on"] = false;
    EXPECT_EQ(true, SimpleColorTemperatureStrategy().setColorTemperature(155, 6, test_light));

    prep_ret[2]["success"]["/lights/1/state/ct"] = 153;
    EXPECT_CALL(test_light, SendPutRequest(_, "/state", _)).Times(1).WillOnce(Return(prep_ret));
    EXPECT_EQ(true, SimpleColorTemperatureStrategy().setColorTemperature(0, 6, test_light));

    prep_ret[2]["success"]["/lights/1/state/ct"] = 500;
    EXPECT_CALL(test_light, SendPutRequest(_, "/state", _)).Times(1).WillOnce(Return(prep_ret));
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
    MockHueLight test_light(handler);
    EXPECT_CALL(test_light, refreshState()).Times(AtLeast(1)).WillRepeatedly(Return());

    test_light.getState()["state"]["colormode"] = "invalid";
    test_light.getState()["state"]["on"] = false;
    EXPECT_EQ(false, SimpleColorTemperatureStrategy().alertTemperature(400, test_light));

    EXPECT_CALL(test_light, setColorTemperature(_, _))
        .Times(AtLeast(2))
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true));
    test_light.getState()["state"]["colormode"] = "ct";
    test_light.getState()["state"]["on"] = true;
    test_light.getState()["state"]["ct"] = 200;
    EXPECT_EQ(false, SimpleColorTemperatureStrategy().alertTemperature(400, test_light));

    EXPECT_CALL(test_light, alert()).Times(AtLeast(2)).WillOnce(Return(false)).WillRepeatedly(Return(true));
    EXPECT_EQ(false, SimpleColorTemperatureStrategy().alertTemperature(400, test_light));

    EXPECT_EQ(true, SimpleColorTemperatureStrategy().alertTemperature(400, test_light));

    EXPECT_CALL(test_light, OffNoRefresh(_)).Times(AtLeast(1)).WillRepeatedly(Return(true));
    test_light.getState()["state"]["on"] = false;
    EXPECT_EQ(true, SimpleColorTemperatureStrategy().alertTemperature(400, test_light));
}

TEST(SimpleColorTemperatureStrategy, getColorTemperature)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler(std::make_shared<MockHttpHandler>());
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername() + "/lights/1", nlohmann::json::object(), getBridgeIp(), 80))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(nlohmann::json::object()));
    MockHueLight test_light(handler);
    EXPECT_CALL(test_light, refreshState()).Times(AtLeast(1)).WillRepeatedly(Return());

    test_light.getState()["state"]["ct"] = 200;
    EXPECT_EQ(200, SimpleColorTemperatureStrategy().getColorTemperature(test_light));
    test_light.getState()["state"]["ct"] = 500;
    EXPECT_EQ(500, SimpleColorTemperatureStrategy().getColorTemperature(static_cast<const HueLight>(test_light)));
}
