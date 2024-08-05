/**
    \file test_Sensor.cpp
    Copyright Notice\n
    Copyright (C) 2020  Jan Rogall	- developer\n

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

#include <hueplusplus/Sensor.h>

#include <gtest/gtest.h>

#include "testhelper.h"

#include "mocks/mock_HttpHandler.h"

using namespace hueplusplus;
using namespace testing;

class SensorTest : public Test
{
protected:
    std::shared_ptr<MockHttpHandler> handler;
    HueCommandAPI commands;
    nlohmann::json state;

protected:
    SensorTest()
        : handler(std::make_shared<MockHttpHandler>()),
          commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler),
          state({{"type", "testSensor"}, {"name", "Test sensor"}, {"swversion", "1.2.3.4"}, {"modelid", "test"},
              {"manufacturername", "testManuf"}, {"uniqueid", "00:00:00:00:00:00:00:00-00"},
              {"productname", "Test sensor"}, {"config", nlohmann::json::object()},
              {"state", nlohmann::json::object()}})
    { }

    Sensor getSensor(int id = 1)
    {
        EXPECT_CALL(*handler,
            GETJson(
                "/api/" + getBridgeUsername() + "/sensors/" + std::to_string(id), _, getBridgeIp(), getBridgePort()))
            .WillOnce(Return(state));
        return Sensor(id, commands, std::chrono::steady_clock::duration::max(), nullptr);
    }
};

TEST(Alert, alertFromString)
{
    EXPECT_EQ(Alert::none, alertFromString("none"));
    EXPECT_EQ(Alert::select, alertFromString("select"));
    EXPECT_EQ(Alert::lselect, alertFromString("lselect"));
    EXPECT_EQ(Alert::none, alertFromString("anything"));
}

TEST(Alert, alertToString)
{
    EXPECT_EQ("none", alertToString(Alert::none));
    EXPECT_EQ("select", alertToString(Alert::select));
    EXPECT_EQ("lselect", alertToString(Alert::lselect));
}

TEST_F(SensorTest, on)
{
    EXPECT_FALSE(getSensor().hasOn());
    state["config"]["on"] = true;
    EXPECT_TRUE(getSensor().hasOn());
    EXPECT_TRUE(getSensor().isOn());

    EXPECT_CALL(*handler,
        PUTJson("/api/" + getBridgeUsername() + "/sensors/1/config", nlohmann::json({{"on", false}}), getBridgeIp(),
            getBridgePort()))
        .WillOnce(Return(nlohmann::json {{{"success", {{"/sensors/1/config/on", false}}}}}));
    getSensor().setOn(false);
}

TEST_F(SensorTest, BatteryState)
{
    EXPECT_FALSE(getSensor().hasBatteryState());
    state["config"]["battery"] = 90;
    EXPECT_TRUE(getSensor().hasBatteryState());
    EXPECT_EQ(90, getSensor().getBatteryState());

    int percent = 10;
    EXPECT_CALL(*handler,
        PUTJson("/api/" + getBridgeUsername() + "/sensors/1/config", nlohmann::json({{"battery", percent}}),
            getBridgeIp(), getBridgePort()))
        .WillOnce(Return(nlohmann::json {{{"success", {{"/sensors/1/config/battery", percent}}}}}));
    getSensor().setBatteryState(percent);
}

TEST_F(SensorTest, Alert)
{
    EXPECT_FALSE(getSensor().hasAlert());
    state["config"]["alert"] = "none";
    EXPECT_TRUE(getSensor().hasAlert());
    EXPECT_EQ(Alert::none, getSensor().getLastAlert());

    std::string alert = "lselect";
    EXPECT_CALL(*handler,
        PUTJson("/api/" + getBridgeUsername() + "/sensors/1/config", nlohmann::json({{"alert", alert}}), getBridgeIp(),
            getBridgePort()))
        .WillOnce(Return(nlohmann::json {{{"success", {{"/sensors/1/config/alert", alert}}}}}));
    getSensor().sendAlert(Alert::lselect);
}

TEST_F(SensorTest, Reachable)
{
    EXPECT_FALSE(getSensor().hasReachable());
    state["config"]["reachable"] = false;
    EXPECT_TRUE(getSensor().hasReachable());
    EXPECT_FALSE(getSensor().isReachable());
}

TEST_F(SensorTest, UserTest)
{
    EXPECT_FALSE(getSensor().hasUserTest());
    state["config"]["usertest"] = false;
    EXPECT_TRUE(getSensor().hasUserTest());

    EXPECT_CALL(*handler,
        PUTJson("/api/" + getBridgeUsername() + "/sensors/1/config", nlohmann::json({{"usertest", true}}),
            getBridgeIp(), getBridgePort()))
        .WillOnce(Return(nlohmann::json {{{"success", {{"/sensors/1/config/usertest", true}}}}}));
    getSensor().setUserTest(true);
}

TEST_F(SensorTest, URL)
{
    EXPECT_FALSE(getSensor().hasURL());
    const std::string url = "https://abc";
    state["config"]["url"] = url;
    EXPECT_TRUE(getSensor().hasURL());
    EXPECT_EQ(url, getSensor().getURL());

    std::string newUrl = "https://cde";
    EXPECT_CALL(*handler,
        PUTJson("/api/" + getBridgeUsername() + "/sensors/1/config", nlohmann::json({{"url", newUrl}}), getBridgeIp(),
            getBridgePort()))
        .WillOnce(Return(nlohmann::json {{{"success", {{"/sensors/1/config/url", newUrl}}}}}));
    getSensor().setURL(newUrl);
}

TEST_F(SensorTest, getPendingConfig)
{
    EXPECT_TRUE(getSensor().getPendingConfig().empty());
    state["config"]["pending"] = nullptr;
    EXPECT_TRUE(getSensor().getPendingConfig().empty());

    state["config"]["pending"] = {"abc", "cde", "def"};

    EXPECT_THAT(getSensor().getPendingConfig(), UnorderedElementsAre("abc", "cde", "def"));
}

TEST_F(SensorTest, LEDIndication)
{
    EXPECT_FALSE(getSensor().hasLEDIndication());
    state["config"]["ledindication"] = true;
    EXPECT_TRUE(getSensor().hasLEDIndication());
    EXPECT_TRUE(getSensor().getLEDIndication());

    EXPECT_CALL(*handler,
        PUTJson("/api/" + getBridgeUsername() + "/sensors/1/config", nlohmann::json({{"ledindication", false}}),
            getBridgeIp(), getBridgePort()))
        .WillOnce(Return(nlohmann::json {{{"success", {{"/sensors/1/config/ledindication", false}}}}}));
    getSensor().setLEDIndication(false);
}

TEST_F(SensorTest, getConfig)
{
    EXPECT_EQ(state["config"], getSensor().getConfig());
    state["config"]["attribute"] = false;
    EXPECT_EQ(state["config"], getSensor().getConfig());
}

TEST_F(SensorTest, setConfigAttribute)
{
    const std::string key = "attribute";
    const nlohmann::json value = "some value";
    EXPECT_CALL(*handler,
        PUTJson("/api/" + getBridgeUsername() + "/sensors/1/config", nlohmann::json({{key, value}}), getBridgeIp(),
            getBridgePort()))
        .WillOnce(Return(nlohmann::json {{{"success", {{"/sensors/1/config/" + key, value}}}}}));
    getSensor().setConfigAttribute(key, value);
}

TEST_F(SensorTest, getLastUpdated)
{
    time::AbsoluteTime none = getSensor().getLastUpdated();
    EXPECT_EQ(std::chrono::seconds(0), none.getBaseTime().time_since_epoch());

    const std::string timestamp = "2020-05-02T12:00:01";
    state["state"]["lastupdated"] = timestamp;
    time::AbsoluteTime time = time::AbsoluteTime::parseUTC(timestamp);
    EXPECT_EQ(time.getBaseTime(), getSensor().getLastUpdated().getBaseTime());
}

TEST_F(SensorTest, getState)
{
    nlohmann::json stateContent = {{"bla", "bla"}};
    state["state"] = stateContent;
    EXPECT_EQ(stateContent, getSensor().getState());
}

TEST_F(SensorTest, setStateAttribute)
{
    const std::string key = "attribute";
    const nlohmann::json value = "some value";
    EXPECT_CALL(*handler,
        PUTJson("/api/" + getBridgeUsername() + "/sensors/1/state", nlohmann::json({{key, value}}), getBridgeIp(),
            getBridgePort()))
        .WillOnce(Return(nlohmann::json {{{"success", {{"/sensors/1/state/" + key, value}}}}}));
    getSensor().setStateAttribute(key, value);
}

TEST_F(SensorTest, isCertified)
{
    EXPECT_FALSE(getSensor().isCertified());
    state["capabilities"]["certified"] = true;
    EXPECT_TRUE(getSensor().isCertified());
}

TEST_F(SensorTest, isPrimary)
{
    EXPECT_FALSE(getSensor().isPrimary());
    state["capabilities"]["primary"] = true;
    EXPECT_TRUE(getSensor().isPrimary());
}

TEST_F(SensorTest, asSensorType)
{
    // Test both rvalue and const access
    {
        const Sensor s = getSensor();
        EXPECT_THROW(s.asSensorType<sensors::DaylightSensor>(), HueException);
    }
    EXPECT_THROW(getSensor().asSensorType<sensors::DaylightSensor>(), HueException);

    state["type"] = sensors::DaylightSensor::typeStr;
    sensors::DaylightSensor ds = getSensor().asSensorType<sensors::DaylightSensor>();
    EXPECT_EQ(1, ds.getId());
    const Sensor s = getSensor();
    ds = s.asSensorType<sensors::DaylightSensor>();
    EXPECT_EQ(s.getId(), ds.getId());
}
