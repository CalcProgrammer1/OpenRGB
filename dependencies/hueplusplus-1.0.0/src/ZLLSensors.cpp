/**
    \file ZLLSensors.cpp
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
 */

#include "hueplusplus/ZLLSensors.h"

#include "hueplusplus/HueExceptionMacro.h"

namespace hueplusplus
{
namespace sensors
{

constexpr int ZGPSwitch::c_button1;
constexpr int ZGPSwitch::c_button2;
constexpr int ZGPSwitch::c_button3;
constexpr int ZGPSwitch::c_button4;
constexpr const char* ZGPSwitch::typeStr;

bool ZGPSwitch::isOn() const
{
    return state.getValue().at("config").at("on").get<bool>();
}

void ZGPSwitch::setOn(bool on)
{
    sendPutRequest("/config", nlohmann::json {{"on", on}}, CURRENT_FILE_INFO);
}

int ZGPSwitch::getButtonEvent() const
{
    return state.getValue().at("state").at("buttonevent").get<int>();
}

constexpr int ZLLSwitch::c_ON_INITIAL_PRESS;
constexpr int ZLLSwitch::c_ON_HOLD;
constexpr int ZLLSwitch::c_ON_SHORT_RELEASED;
constexpr int ZLLSwitch::c_ON_LONG_RELEASED;
constexpr int ZLLSwitch::c_UP_INITIAL_PRESS;
constexpr int ZLLSwitch::c_UP_HOLD;
constexpr int ZLLSwitch::c_UP_SHORT_RELEASED;
constexpr int ZLLSwitch::c_UP_LONG_RELEASED;
constexpr int ZLLSwitch::c_DOWN_INITIAL_PRESS;
constexpr int ZLLSwitch::c_DOWN_HOLD;
constexpr int ZLLSwitch::c_DOWN_SHORT_RELEASED;
constexpr int ZLLSwitch::c_DOWN_LONG_RELEASED;
constexpr int ZLLSwitch::c_OFF_INITIAL_PRESS;
constexpr int ZLLSwitch::c_OFF_HOLD;
constexpr int ZLLSwitch::c_OFF_SHORT_RELEASED;
constexpr int ZLLSwitch::c_OFF_LONG_RELEASED;
constexpr const char* ZLLSwitch::typeStr;

bool ZLLSwitch::isOn() const
{
    return state.getValue().at("config").at("on").get<bool>();
}

void ZLLSwitch::setOn(bool on)
{
    sendPutRequest("/config", nlohmann::json {{"on", on}}, CURRENT_FILE_INFO);
}
bool ZLLSwitch::hasBatteryState() const
{
    return state.getValue().at("config").count("battery") != 0;
}
int ZLLSwitch::getBatteryState() const
{
    return state.getValue().at("config").at("battery").get<int>();
}

Alert ZLLSwitch::getLastAlert() const
{
    std::string alert = state.getValue().at("config").at("alert").get<std::string>();
    return alertFromString(alert);
}
void ZLLSwitch::sendAlert(Alert type)
{
    sendPutRequest("/config", nlohmann::json {{"alert", alertToString(type)}}, CURRENT_FILE_INFO);
}
bool ZLLSwitch::isReachable() const
{
    return state.getValue().at("config").at("reachable").get<bool>();
}
int ZLLSwitch::getButtonEvent() const
{
    return state.getValue().at("state").at("buttonevent").get<int>();
}

time::AbsoluteTime ZLLSwitch::getLastUpdated() const
{
    const nlohmann::json& stateJson = state.getValue().at("state");
    auto it = stateJson.find("lastupdated");
    if (it == stateJson.end() || !it->is_string() || *it == "none")
    {
        return time::AbsoluteTime(std::chrono::system_clock::time_point(std::chrono::seconds {0}));
    }
    return time::AbsoluteTime::parseUTC(it->get<std::string>());
}

constexpr const char* ZLLPresence::typeStr;

bool ZLLPresence::isOn() const
{
    return state.getValue().at("config").at("on").get<bool>();
}

void ZLLPresence::setOn(bool on)
{
    sendPutRequest("/config", nlohmann::json {{"on", on}}, CURRENT_FILE_INFO);
}
bool ZLLPresence::hasBatteryState() const
{
    return state.getValue().at("config").count("battery") != 0;
}
int ZLLPresence::getBatteryState() const
{
    return state.getValue().at("config").at("battery").get<int>();
}

Alert ZLLPresence::getLastAlert() const
{
    std::string alert = state.getValue().at("config").at("alert").get<std::string>();
    return alertFromString(alert);
}
void ZLLPresence::sendAlert(Alert type)
{
    sendPutRequest("/config", nlohmann::json {{"alert", alertToString(type)}}, CURRENT_FILE_INFO);
}
bool ZLLPresence::isReachable() const
{
    return state.getValue().at("config").at("reachable").get<bool>();
}

int ZLLPresence::getSensitivity() const
{
    return state.getValue().at("config").at("sensitivity").get<int>();
}
int ZLLPresence::getMaxSensitivity() const
{
    return state.getValue().at("config").at("sensitivitymax").get<int>();
}
void ZLLPresence::setSensitivity(int sensitivity)
{
    sendPutRequest("/config", nlohmann::json {{"sensitivity", sensitivity}}, CURRENT_FILE_INFO);
}
bool ZLLPresence::getPresence() const
{
    return state.getValue().at("state").at("presence").get<bool>();
}

time::AbsoluteTime ZLLPresence::getLastUpdated() const
{
    const nlohmann::json& stateJson = state.getValue().at("state");
    auto it = stateJson.find("lastupdated");
    if (it == stateJson.end() || !it->is_string() || *it == "none")
    {
        return time::AbsoluteTime(std::chrono::system_clock::time_point(std::chrono::seconds {0}));
    }
    return time::AbsoluteTime::parseUTC(it->get<std::string>());
}

constexpr const char* ZLLTemperature::typeStr;

bool ZLLTemperature::isOn() const
{
    return state.getValue().at("config").at("on").get<bool>();
}

void ZLLTemperature::setOn(bool on)
{
    sendPutRequest("/config", {{"on", on}}, CURRENT_FILE_INFO);
}
bool ZLLTemperature::hasBatteryState() const
{
    return state.getValue().at("config").count("battery") != 0;
}
int ZLLTemperature::getBatteryState() const
{
    return state.getValue().at("config").at("battery").get<int>();
}

Alert ZLLTemperature::getLastAlert() const
{
    std::string alert = state.getValue().at("config").at("alert").get<std::string>();
    return alertFromString(alert);
}
void ZLLTemperature::sendAlert(Alert type)
{
    sendPutRequest("/config", nlohmann::json {{"alert", alertToString(type)}}, CURRENT_FILE_INFO);
}
bool ZLLTemperature::isReachable() const
{
    return state.getValue().at("config").at("reachable").get<bool>();
}

int ZLLTemperature::getTemperature() const
{
    return state.getValue().at("state").at("temperature").get<int>();
}

time::AbsoluteTime ZLLTemperature::getLastUpdated() const
{
    const nlohmann::json& stateJson = state.getValue().at("state");
    auto it = stateJson.find("lastupdated");
    if (it == stateJson.end() || !it->is_string() || *it == "none")
    {
        return time::AbsoluteTime(std::chrono::system_clock::time_point(std::chrono::seconds{ 0 }));
    }
    return time::AbsoluteTime::parseUTC(it->get<std::string>());
}

constexpr const char* ZLLLightLevel::typeStr;

bool ZLLLightLevel::isOn() const
{
    return state.getValue().at("config").at("on").get<bool>();
}

void ZLLLightLevel::setOn(bool on)
{
    sendPutRequest("/config", {{"on", on}}, CURRENT_FILE_INFO);
}
bool ZLLLightLevel::hasBatteryState() const
{
    return state.getValue().at("config").count("battery") != 0;
}
int ZLLLightLevel::getBatteryState() const
{
    return state.getValue().at("config").at("battery").get<int>();
}
bool ZLLLightLevel::isReachable() const
{
    return state.getValue().at("config").at("reachable").get<bool>();
}
int ZLLLightLevel::getDarkThreshold() const
{
    return state.getValue().at("config").at("tholddark").get<int>();
}

void ZLLLightLevel::setDarkThreshold(int threshold)
{
    sendPutRequest("/config", nlohmann::json {{"tholddark", threshold}}, CURRENT_FILE_INFO);
}
int ZLLLightLevel::getThresholdOffset() const
{
    return state.getValue().at("config").at("tholdoffset").get<int>();
}

void ZLLLightLevel::setThresholdOffset(int offset)
{
    sendPutRequest("/config", nlohmann::json {{"tholdoffset", offset}}, CURRENT_FILE_INFO);
}

int ZLLLightLevel::getLightLevel() const
{
    return state.getValue().at("state").at("lightlevel").get<int>();
}

bool ZLLLightLevel::isDark() const
{
    return state.getValue().at("state").at("dark").get<bool>();
}

bool ZLLLightLevel::isDaylight() const
{
    return state.getValue().at("state").at("daylight").get<bool>();
}

time::AbsoluteTime ZLLLightLevel::getLastUpdated() const
{
    const nlohmann::json& stateJson = state.getValue().at("state");
    auto it = stateJson.find("lastupdated");
    if (it == stateJson.end() || !it->is_string() || *it == "none")
    {
        return time::AbsoluteTime(std::chrono::system_clock::time_point(std::chrono::seconds {0}));
    }
    return time::AbsoluteTime::parseUTC(it->get<std::string>());
}

detail::ConditionHelper<bool> makeConditionDark(const ZLLLightLevel& sensor)
{
    return detail::ConditionHelper<bool>("/sensors/" + std::to_string(sensor.getId()) + "/state/dark");
}

detail::ConditionHelper<bool> makeConditionDaylight(const ZLLLightLevel& sensor)
{
    return detail::ConditionHelper<bool>("/sensors/" + std::to_string(sensor.getId()) + "/state/daylight");
}

detail::ConditionHelper<int> makeConditionLightLevel(const ZLLLightLevel& sensor)
{
    return detail::ConditionHelper<int>("/sensors/" + std::to_string(sensor.getId()) + "/state/lightlevel");
}
} // namespace sensors
} // namespace hueplusplus