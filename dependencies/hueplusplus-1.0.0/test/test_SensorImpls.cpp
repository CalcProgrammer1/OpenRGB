/**
    \file test_SensorImpls.cpp
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

#include <hueplusplus/CLIPSensors.h>
#include <hueplusplus/ZLLSensors.h>

#include <gtest/gtest.h>

#include "testhelper.h"

#include "mocks/mock_HttpHandler.h"

using namespace testing;
using namespace hueplusplus;
using namespace hueplusplus::sensors;

// Many sensor classes contain duplicate methods, with the type parameterized tests at least the test cases
// do not have to be duplicated
template <typename T>
class SensorImplTest : public Test
{
protected:
    std::shared_ptr<MockHttpHandler> handler;
    HueCommandAPI commands;
    nlohmann::json state;

protected:
    SensorImplTest()
        : handler(std::make_shared<MockHttpHandler>()),
          commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler),
          state({{"type", T::typeStr}, {"config", nlohmann::json::object()}, {"state", nlohmann::json::object()}})
    { }

    void expectConfigSet(const std::string& key, const nlohmann::json& value)
    {
        EXPECT_CALL(*handler,
            PUTJson("/api/" + getBridgeUsername() + "/sensors/1/config", nlohmann::json({{key, value}}), getBridgeIp(),
                getBridgePort()))
            .WillOnce(Return(nlohmann::json {{{"success", {{"/sensors/1/config/" + key, value}}}}}));
    }
    void expectStateSet(const std::string& key, const nlohmann::json& value)
    {
        EXPECT_CALL(*handler,
            PUTJson("/api/" + getBridgeUsername() + "/sensors/1/state", nlohmann::json({{key, value}}), getBridgeIp(),
                getBridgePort()))
            .WillOnce(Return(nlohmann::json {{{"success", {{"/sensors/1/state/" + key, value}}}}}));
    }

    T getSensor()
    {
        EXPECT_CALL(*handler, GETJson("/api/" + getBridgeUsername() + "/sensors/1", _, getBridgeIp(), getBridgePort()))
            .WillOnce(Return(state));
        return T(Sensor(1, commands, std::chrono::steady_clock::duration::max(), nullptr));
    }
};

// Sensors with shared methods

template <typename T>
class SensorOnTest : public SensorImplTest<T>
{ };
// Only need to test one CLIP type, because they share the basic methods
using SensorOnTypes
    = Types<CLIPSwitch, ZGPSwitch, ZLLSwitch, ZLLPresence, ZLLTemperature, ZLLLightLevel, DaylightSensor>;
TYPED_TEST_SUITE(SensorOnTest, SensorOnTypes);

template <typename T>
class SensorBatteryTest : public SensorImplTest<T>
{ };
using SensorBatteryTypes = Types<CLIPSwitch, ZLLSwitch, ZLLPresence, ZLLTemperature, ZLLLightLevel, DaylightSensor>;
TYPED_TEST_SUITE(SensorBatteryTest, SensorBatteryTypes);

template <typename T>
class SensorReachableTest : public SensorImplTest<T>
{ };
using SensorReachableTypes = Types<CLIPSwitch, ZLLSwitch, ZLLPresence, ZLLTemperature, ZLLLightLevel>;
TYPED_TEST_SUITE(SensorReachableTest, SensorReachableTypes);

template <typename T>
class SensorUpdateTest : public SensorImplTest<T>
{ };
using SensorUpdateTypes = Types<CLIPSwitch, ZLLSwitch, ZLLPresence, ZLLLightLevel, ZLLTemperature, DaylightSensor>;
TYPED_TEST_SUITE(SensorUpdateTest, SensorUpdateTypes);

template <typename T>
class SensorAlertTest : public SensorImplTest<T>
{ };
using SensorAlertTypes = Types<ZLLSwitch, ZLLPresence, ZLLTemperature>;
TYPED_TEST_SUITE(SensorAlertTest, SensorAlertTypes);

template <typename T>
class SensorButtonTest : public SensorImplTest<T>
{ };
using SensorButtonTypes = Types<CLIPSwitch, ZLLSwitch, ZGPSwitch>;
TYPED_TEST_SUITE(SensorButtonTest, SensorButtonTypes);

template <typename T>
class SensorTemperatureTest : public SensorImplTest<T>
{ };
using SensorTemperatureTypes = Types<CLIPTemperature, ZLLTemperature>;
TYPED_TEST_SUITE(SensorTemperatureTest, SensorTemperatureTypes);

template <typename T>
class SensorLightLevelTest : public SensorImplTest<T>
{ };
using SensorLightLevelTypes = Types<CLIPLightLevel, ZLLLightLevel>;
TYPED_TEST_SUITE(SensorLightLevelTest, SensorLightLevelTypes);

template <typename T>
class SensorPresenceTest : public SensorImplTest<T>
{ };
using SensorPresenceTypes = Types<CLIPPresence, ZLLPresence>;
TYPED_TEST_SUITE(SensorPresenceTest, SensorPresenceTypes);

// Sensors with unique methods

class DaylightSensorTest : public SensorImplTest<DaylightSensor>
{ };

class ZLLPresenceTest : public SensorImplTest<ZLLPresence>
{ };

class CLIPSwitchTest : public SensorImplTest<CLIPSwitch>
{ };

class CLIPOpenCloseTest : public SensorImplTest<CLIPOpenClose>
{ };

class CLIPPresenceTest : public SensorImplTest<CLIPPresence>
{ };

class CLIPTemperatureTest : public SensorImplTest<CLIPTemperature>
{ };

class CLIPHumidityTest : public SensorImplTest<CLIPHumidity>
{ };

class CLIPGenericFlagTest : public SensorImplTest<CLIPGenericFlag>
{ };

class CLIPGenericStatusTest : public SensorImplTest<CLIPGenericStatus>
{ };

TYPED_TEST(SensorOnTest, on)
{
    this->state["config"]["on"] = false;
    EXPECT_FALSE(this->getSensor().isOn());
    this->state["config"]["on"] = true;
    EXPECT_TRUE(this->getSensor().isOn());

    this->expectConfigSet("on", false);
    this->getSensor().setOn(false);
}

TYPED_TEST(SensorBatteryTest, BatteryState)
{
    EXPECT_FALSE(this->getSensor().hasBatteryState());
    this->state["config"]["battery"] = 90;
    EXPECT_TRUE(this->getSensor().hasBatteryState());
    EXPECT_EQ(90, this->getSensor().getBatteryState());
}

TYPED_TEST(SensorReachableTest, Reachable)
{
    this->state["config"]["reachable"] = true;
    EXPECT_TRUE(this->getSensor().isReachable());
}

TYPED_TEST(SensorUpdateTest, getLastUpdated)
{
    time::AbsoluteTime none = this->getSensor().getLastUpdated();
    EXPECT_EQ(std::chrono::seconds(0), none.getBaseTime().time_since_epoch());

    const std::string timestamp = "2020-05-02T12:00:01";
    this->state["state"]["lastupdated"] = timestamp;
    time::AbsoluteTime time = time::AbsoluteTime::parseUTC(timestamp);
    EXPECT_EQ(time.getBaseTime(), this->getSensor().getLastUpdated().getBaseTime());
}

TYPED_TEST(SensorAlertTest, Alert)
{
    this->state["config"]["alert"] = "none";
    EXPECT_EQ(Alert::none, this->getSensor().getLastAlert());

    this->expectConfigSet("alert", "lselect");
    this->getSensor().sendAlert(Alert::lselect);
}

TYPED_TEST(SensorButtonTest, ButtonEvent)
{
    int code = 12;
    this->state["state"]["buttonevent"] = code;
    EXPECT_EQ(code, this->getSensor().getButtonEvent());
}

TYPED_TEST(SensorTemperatureTest, Temperature)
{
    int temperature = 1200;
    this->state["state"]["temperature"] = temperature;
    EXPECT_EQ(temperature, this->getSensor().getTemperature());
}

TYPED_TEST(SensorLightLevelTest, LightLevel)
{
    int lightLevel = 1200;
    this->state["state"] = {{"lightlevel", lightLevel}, {"dark", true}, {"daylight", false}};
    EXPECT_EQ(lightLevel, this->getSensor().getLightLevel());
    EXPECT_TRUE(this->getSensor().isDark());
    EXPECT_FALSE(this->getSensor().isDaylight());
}

TYPED_TEST(SensorLightLevelTest, DarkThreshold)
{
    int darkThreshold = 12000;
    this->state["config"]["tholddark"] = darkThreshold;
    EXPECT_EQ(darkThreshold, this->getSensor().getDarkThreshold());

    int newThreshold = 10;
    this->expectConfigSet("tholddark", newThreshold);
    this->getSensor().setDarkThreshold(newThreshold);
}

TYPED_TEST(SensorLightLevelTest, ThresholdOffset)
{
    int offset = 12000;
    this->state["config"]["tholdoffset"] = offset;
    EXPECT_EQ(offset, this->getSensor().getThresholdOffset());

    int newOffset = 10;
    this->expectConfigSet("tholdoffset", newOffset);
    this->getSensor().setThresholdOffset(newOffset);
}

TYPED_TEST(SensorPresenceTest, Presence)
{
    this->state["state"]["presence"] = true;
    EXPECT_TRUE(this->getSensor().getPresence());
}

TEST_F(DaylightSensorTest, Coordinates)
{
    const std::string lat = "000.0000N";
    const std::string lon = "000.0000E";
    EXPECT_CALL(*handler,
        PUTJson("/api/" + getBridgeUsername() + "/sensors/1/config", nlohmann::json({{"lat", lat}, {"long", lon}}),
            getBridgeIp(), getBridgePort()))
        .WillOnce(Return(nlohmann::json {
            {{"success", {{"/sensors/1/config/lat", lat}}}}, {{"success", {{"/sensors/1/config/long", lon}}}}}));
    getSensor().setCoordinates(lat, lon);
    state["config"]["configured"] = true;
    EXPECT_TRUE(getSensor().isConfigured());
}

TEST_F(DaylightSensorTest, SunriseOffset)
{
    int offset = 10;
    state["config"]["sunriseoffset"] = offset;
    EXPECT_EQ(offset, getSensor().getSunriseOffset());

    int newOffset = 20;
    expectConfigSet("sunriseoffset", newOffset);
    getSensor().setSunriseOffset(newOffset);
}

TEST_F(DaylightSensorTest, SunsetOffset)
{
    int offset = 10;
    state["config"]["sunsetoffset"] = offset;
    EXPECT_EQ(offset, getSensor().getSunsetOffset());

    int newOffset = 20;
    expectConfigSet("sunsetoffset", newOffset);
    getSensor().setSunsetOffset(newOffset);
}

TEST_F(DaylightSensorTest, isDaylight)
{
    state["state"]["daylight"] = true;
    EXPECT_TRUE(getSensor().isDaylight());
}

TEST_F(ZLLPresenceTest, Sensitivity)
{
    int sensitivity = 1000;
    state["config"]["sensitivity"] = sensitivity;
    int maxSensitivity = 10000;
    state["config"]["sensitivitymax"] = maxSensitivity;
    EXPECT_EQ(sensitivity, getSensor().getSensitivity());
    EXPECT_EQ(maxSensitivity, getSensor().getMaxSensitivity());

    int newSensitivity = 10;
    expectConfigSet("sensitivity", newSensitivity);
    this->getSensor().setSensitivity(newSensitivity);
}

TEST_F(CLIPSwitchTest, setBatteryState)
{
    int percent = 10;
    expectConfigSet("battery", percent);
    this->getSensor().setBatteryState(percent);
}

TEST_F(CLIPSwitchTest, URL)
{
    EXPECT_FALSE(getSensor().hasURL());
    const std::string url = "https://abc";
    state["config"]["url"] = url;
    EXPECT_TRUE(getSensor().hasURL());
    EXPECT_EQ(url, getSensor().getURL());

    std::string newUrl = "https://cde";
    expectConfigSet("url", newUrl);
    getSensor().setURL(newUrl);
}

TEST_F(CLIPSwitchTest, setButtonEvent)
{
    int code = 10;
    expectStateSet("buttonevent", code);
    this->getSensor().setButtonEvent(code);
}

TEST_F(CLIPOpenCloseTest, Open)
{
    state["state"]["open"] = true;
    EXPECT_TRUE(getSensor().isOpen());

    bool open = false;
    expectStateSet("open", open);
    getSensor().setOpen(open);
}

TEST_F(CLIPPresenceTest, setPresence)
{
    bool presence = false;
    expectStateSet("presence", presence);
    getSensor().setPresence(presence);
}

TEST_F(CLIPTemperatureTest, setPresence)
{
    int temperature = 1100;
    expectStateSet("temperature", temperature);
    getSensor().setTemperature(temperature);
}

TEST_F(CLIPHumidityTest, Humidity)
{
    int humidity = 100;
    state["state"]["humidity"] = humidity;
    EXPECT_EQ(humidity, getSensor().getHumidity());

    int newHumidity = 1100;
    expectStateSet("humidity", newHumidity);
    getSensor().setHumidity(newHumidity);
}

TEST_F(CLIPGenericFlagTest, Flag)
{
    state["state"]["flag"] = true;
    EXPECT_TRUE(getSensor().getFlag());
    expectStateSet("flag", false);
    getSensor().setFlag(false);
}

TEST_F(CLIPGenericStatusTest, Status)
{
    int status = 32;
    state["state"]["status"] = status;
    EXPECT_EQ(status, getSensor().getStatus());
    int newStatus = 52;
    expectStateSet("status", newStatus);
    getSensor().setStatus(newStatus);
}
