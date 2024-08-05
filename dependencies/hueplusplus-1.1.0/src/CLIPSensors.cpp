/**
    \file CLIPSensors.cpp
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

#include "hueplusplus/CLIPSensors.h"

#include "hueplusplus/HueExceptionMacro.h"

namespace hueplusplus
{
namespace sensors
{
bool BaseCLIP::isOn() const
{
    return state.getValue().at("config").at("on").get<bool>();
}

void BaseCLIP::setOn(bool on)
{
    sendPutRequest("/config", nlohmann::json {{"on", on}}, CURRENT_FILE_INFO);
}
bool BaseCLIP::hasBatteryState() const
{
    return state.getValue().at("config").count("battery") != 0;
}
int BaseCLIP::getBatteryState() const
{
    return state.getValue().at("config").at("battery").get<int>();
}
void BaseCLIP::setBatteryState(int percent)
{
    sendPutRequest("/config", nlohmann::json {{"battery", percent}}, CURRENT_FILE_INFO);
}
bool BaseCLIP::isReachable() const
{
    return state.getValue().at("config").at("reachable").get<bool>();
}

bool BaseCLIP::hasURL() const
{
    return state.getValue().at("config").count("url") != 0;
}
std::string BaseCLIP::getURL() const
{
    return state.getValue().at("config").at("url").get<std::string>();
}
void BaseCLIP::setURL(const std::string& url)
{
    sendPutRequest("/config", nlohmann::json {{"url", url}}, CURRENT_FILE_INFO);
}

time::AbsoluteTime BaseCLIP::getLastUpdated() const
{
    const nlohmann::json& stateJson = state.getValue().at("state");
    auto it = stateJson.find("lastupdated");
    if (it == stateJson.end() || !it->is_string() || *it == "none")
    {
        return time::AbsoluteTime(std::chrono::system_clock::time_point(std::chrono::seconds {0}));
    }
    return time::AbsoluteTime::parseUTC(it->get<std::string>());
}

constexpr const char* CLIPSwitch::typeStr;

int CLIPSwitch::getButtonEvent() const
{
    return state.getValue().at("state").at("buttonevent").get<int>();
}
void CLIPSwitch::setButtonEvent(int code)
{
    sendPutRequest("/state", nlohmann::json {{"buttonevent", code}}, CURRENT_FILE_INFO);
}

constexpr const char* CLIPOpenClose::typeStr;

bool CLIPOpenClose::isOpen() const
{
    return state.getValue().at("state").at("open").get<bool>();
}
void CLIPOpenClose::setOpen(bool open)
{
    sendPutRequest("/state", nlohmann::json {{"open", open}}, CURRENT_FILE_INFO);
}


detail::ConditionHelper<bool> makeCondition(const CLIPOpenClose& sensor)
{
    return detail::ConditionHelper<bool>("/sensors/" + std::to_string(sensor.getId()) + "/state/open");
}

constexpr const char* CLIPPresence::typeStr;

bool CLIPPresence::getPresence() const
{
    return state.getValue().at("state").at("presence").get<bool>();
}
void CLIPPresence::setPresence(bool presence)
{
    sendPutRequest("/state", nlohmann::json {{"presence", presence}}, CURRENT_FILE_INFO);
}

constexpr const char* CLIPTemperature::typeStr;

int CLIPTemperature::getTemperature() const
{
    return state.getValue().at("state").at("temperature").get<int>();
}
void CLIPTemperature::setTemperature(int temperature)
{
    sendPutRequest("/state", nlohmann::json {{"temperature", temperature}}, CURRENT_FILE_INFO);
}

constexpr const char* CLIPHumidity::typeStr;

int CLIPHumidity::getHumidity() const
{
    return state.getValue().at("state").at("humidity").get<int>();
}
void CLIPHumidity::setHumidity(int humidity)
{
    sendPutRequest("/state", nlohmann::json {{"humidity", humidity}}, CURRENT_FILE_INFO);
}

detail::ConditionHelper<int> makeCondition(const CLIPHumidity& sensor)
{
    return detail::ConditionHelper<int>("/sensors/" + std::to_string(sensor.getId()) + "/state/humidity");
}

constexpr const char* CLIPLightLevel::typeStr;

int CLIPLightLevel::getDarkThreshold() const
{
    return state.getValue().at("config").at("tholddark").get<int>();
}

void CLIPLightLevel::setDarkThreshold(int threshold)
{
    sendPutRequest("/config", nlohmann::json {{"tholddark", threshold}}, CURRENT_FILE_INFO);
}
int CLIPLightLevel::getThresholdOffset() const
{
    return state.getValue().at("config").at("tholdoffset").get<int>();
}

void CLIPLightLevel::setThresholdOffset(int offset)
{
    sendPutRequest("/config", nlohmann::json {{"tholdoffset", offset}}, CURRENT_FILE_INFO);
}

int CLIPLightLevel::getLightLevel() const
{
    return state.getValue().at("state").at("lightlevel").get<int>();
}

void CLIPLightLevel::setLightLevel(int level)
{
    sendPutRequest("/state", nlohmann::json {{"lightlevel", level}}, CURRENT_FILE_INFO);
}

bool CLIPLightLevel::isDark() const
{
    return state.getValue().at("state").at("dark").get<bool>();
}

bool CLIPLightLevel::isDaylight() const
{
    return state.getValue().at("state").at("daylight").get<bool>();
}

constexpr const char* CLIPGenericFlag::typeStr;

bool CLIPGenericFlag::getFlag() const
{
    return state.getValue().at("state").at("flag").get<bool>();
}
void CLIPGenericFlag::setFlag(bool flag)
{
    sendPutRequest("/state", nlohmann::json {{"flag", flag}}, CURRENT_FILE_INFO);
}

detail::ConditionHelper<bool> makeCondition(const CLIPGenericFlag& sensor)
{
    return detail::ConditionHelper<bool>("/sensors/" + std::to_string(sensor.getId()) + "/state/flag");
}

constexpr const char* CLIPGenericStatus::typeStr;

int CLIPGenericStatus::getStatus() const
{
    return state.getValue().at("state").at("status").get<int>();
}

void CLIPGenericStatus::setStatus(int status)
{
    sendPutRequest("/state", nlohmann::json {{"status", status}}, CURRENT_FILE_INFO);
}

detail::ConditionHelper<int> makeCondition(const CLIPGenericStatus& sensor)
{
    return detail::ConditionHelper<int>("/sensors/" + std::to_string(sensor.getId()) + "/state/status");
}
} // namespace sensors
} // namespace hueplusplus