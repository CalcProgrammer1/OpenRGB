/**
    \file test_SensorList.cpp
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

#include <gtest/gtest.h>

#include "testhelper.h"

#include "hueplusplus/SensorList.h"
#include "mocks/mock_HttpHandler.h"

using namespace hueplusplus;
using namespace testing;

class BlaSensor
{
public:
    BlaSensor(Sensor s) { }

    static constexpr const char* typeStr = "bla";
};

TEST(SensorList, getAsType)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);

    SensorList sensors {commands, "/sensors", std::chrono::steady_clock::duration::max()};

    const int id = 2;
    const nlohmann::json response = {{std::to_string(id), {{"type", "Daylight"}}}};

    EXPECT_CALL(*handler,
        GETJson("/api/" + getBridgeUsername() + "/sensors", nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .WillOnce(Return(response));

    sensors::DaylightSensor daylightSensor = sensors.getAsType<sensors::DaylightSensor>(id);
    EXPECT_THROW(sensors.getAsType<BlaSensor>(2), HueException);
}

TEST(SensorList, getAllByType)
{

    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);

    SensorList sensors {commands, "/sensors", std::chrono::steady_clock::duration::max()};

    // Empty
    {
        const nlohmann::json response = nlohmann::json::object();

        EXPECT_CALL(*handler,
            GETJson(
                "/api/" + getBridgeUsername() + "/sensors", nlohmann::json::object(), getBridgeIp(), getBridgePort()))
            .WillOnce(Return(response));
        EXPECT_TRUE(sensors.getAllByType<sensors::DaylightSensor>().empty());
    }
    // Not matching
    {
        const nlohmann::json response = {{"1", {{"type", "stuff"}}}};
        EXPECT_CALL(*handler,
            GETJson(
                "/api/" + getBridgeUsername() + "/sensors", nlohmann::json::object(), getBridgeIp(), getBridgePort()))
            .WillOnce(Return(response));
        sensors.refresh();
        EXPECT_TRUE(sensors.getAllByType<sensors::DaylightSensor>().empty());
    }
    // Some matching (daylight maybe not the best example, because there is always exactly one)
    {
        const nlohmann::json response = {{"1", {{"type", "stuff"}}}, {"2", {{"type", "Daylight"}}},
            {"3", {{"type", "stuff"}}}, {"4", {{"type", "Daylight"}}}};
        EXPECT_CALL(*handler,
            GETJson(
                "/api/" + getBridgeUsername() + "/sensors", nlohmann::json::object(), getBridgeIp(), getBridgePort()))
            .WillOnce(Return(response));
        sensors.refresh();
        std::vector<sensors::DaylightSensor> result = sensors.getAllByType<sensors::DaylightSensor>();
        EXPECT_THAT(result,
            UnorderedElementsAre(Truly([](const auto& s) { return s.getId() == 2; }),
                Truly([](const auto& s) { return s.getId() == 4; })));
    }
}
