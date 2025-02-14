/**
    \file Sensor.cpp
    Copyright Notice\n
    Copyright (C) 2020  Stefan Herbrechtsmeier - developer\n

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

#include "hueplusplus/Sensor.h"

#include "hueplusplus/HueExceptionMacro.h"
#include "hueplusplus/Utils.h"
#include <nlohmann/json.hpp>

namespace hueplusplus
{
std::string alertToString(Alert alert)
{
    switch (alert)
    {
    case Alert::lselect:
        return "lselect";
    case Alert::select:
        return "select";
        break;
    default:
        return "none";
    }
}

Alert alertFromString(const std::string& s)
{
    if (s == "select")
    {
        return Alert::select;
    }
    else if (s == "lselect")
    {
        return Alert::lselect;
    }
    else
    {
        return Alert::none;
    }
}

bool Sensor::hasOn() const
{
    return state.getValue().at("config").count("on") != 0;
}

bool Sensor::isOn() const
{
    return state.getValue().at("config").at("on").get<bool>();
}

void Sensor::setOn(bool on)
{
    sendPutRequest("/config", nlohmann::json {{"on", on}}, CURRENT_FILE_INFO);
}

bool Sensor::hasBatteryState() const
{
    return state.getValue().at("config").count("battery") != 0;
}
int Sensor::getBatteryState() const
{
    return state.getValue().at("config").at("battery").get<int>();
}
void Sensor::setBatteryState(int percent)
{
    sendPutRequest("/config", nlohmann::json {{"battery", percent}}, CURRENT_FILE_INFO);
}
bool Sensor::hasAlert() const
{
    return state.getValue().at("config").count("alert") != 0;
}
Alert Sensor::getLastAlert() const
{
    std::string alert = state.getValue().at("config").at("alert").get<std::string>();
    if (alert == "select")
    {
        return Alert::select;
    }
    else if (alert == "lselect")
    {
        return Alert::lselect;
    }
    else
    {
        return Alert::none;
    }
}
void Sensor::sendAlert(Alert type)
{
    std::string alertStr;
    switch (type)
    {
    case Alert::lselect:
        alertStr = "lselect";
        break;
    case Alert::select:
        alertStr = "select";
        break;
    default:
        alertStr = "none";
        break;
    }
    sendPutRequest("/config", nlohmann::json {{"alert", alertStr}}, CURRENT_FILE_INFO);
}
bool Sensor::hasReachable() const
{
    return state.getValue().at("config").count("reachable") != 0;
}
bool Sensor::isReachable() const
{
    // If not present, always assume it is reachable (for daylight sensor)
    return state.getValue().at("config").value("reachable", true);
}

time::AbsoluteTime Sensor::getLastUpdated() const
{
    const nlohmann::json& stateJson = state.getValue().at("state");
    auto it = stateJson.find("lastupdated");
    if (it == stateJson.end() || !it->is_string() || *it == "none")
    {
        return time::AbsoluteTime(std::chrono::system_clock::time_point(std::chrono::seconds {0}));
    }
    return time::AbsoluteTime::parseUTC(it->get<std::string>());
}

bool Sensor::hasUserTest() const
{
    return state.getValue().at("config").count("usertest") != 0;
}
void Sensor::setUserTest(bool enabled)
{
    sendPutRequest("/config", nlohmann::json {{"usertest", enabled}}, CURRENT_FILE_INFO);
}

bool Sensor::hasURL() const
{
    return state.getValue().at("config").count("url") != 0;
}
std::string Sensor::getURL() const
{
    return state.getValue().at("config").at("url").get<std::string>();
}
void Sensor::setURL(const std::string& url)
{
    sendPutRequest("/config", nlohmann::json {{"url", url}}, CURRENT_FILE_INFO);
}

std::vector<std::string> Sensor::getPendingConfig() const
{
    const nlohmann::json& config = state.getValue().at("config");
    const auto pendingIt = config.find("pending");
    if (pendingIt == config.end() || !pendingIt->is_array())
    {
        return {};
    }
    std::vector<std::string> result;
    result.reserve(pendingIt->size());
    for (const nlohmann::json& pending : *pendingIt)
    {
        result.push_back(pending.get<std::string>());
    }
    return result;
}

bool Sensor::hasLEDIndication() const
{
    return state.getValue().at("config").count("ledindication") != 0;
}
bool Sensor::getLEDIndication() const
{
    return state.getValue().at("config").at("ledindication").get<bool>();
}
void Sensor::setLEDIndication(bool on)
{
    sendPutRequest("/config", nlohmann::json {{"ledindication", on}}, CURRENT_FILE_INFO);
}

nlohmann::json Sensor::getState() const
{
    return state.getValue().at("state");
}
void Sensor::setStateAttribute(const std::string& key, const nlohmann::json& value)
{
    sendPutRequest("/state", nlohmann::json {{key, value}}, CURRENT_FILE_INFO);
}

std::string Sensor::getStateAddress(const std::string& key) const
{
    return state.getRequestPath() + "/state/" + key;
}

nlohmann::json Sensor::getConfig() const
{
    return state.getValue().at("config");
}

void Sensor::setConfigAttribute(const std::string& key, const nlohmann::json& value)
{
    sendPutRequest("/config", nlohmann::json {{key, value}}, CURRENT_FILE_INFO);
}

bool Sensor::isCertified() const
{
    nlohmann::json certified = utils::safeGetMember(state.getValue(), "capabilities", "certified");
    return certified.is_boolean() && certified.get<bool>();
}

bool Sensor::isPrimary() const
{
    nlohmann::json primary = utils::safeGetMember(state.getValue(), "capabilities", "primary");
    return primary.is_boolean() && primary.get<bool>();
}

Sensor::Sensor(int id, const std::shared_ptr<APICache>& baseCache)
    : BaseDevice(id, baseCache)
{ }


Sensor::Sensor(int id, const HueCommandAPI& commands, std::chrono::steady_clock::duration refreshDuration, const nlohmann::json& currentState)
    : BaseDevice(id, commands, "/sensors/", refreshDuration, currentState)
{ }

CreateSensor::CreateSensor(const std::string& name, const std::string& modelid, const std::string& swversion,
    const std::string& type, const std::string& uniqueid, const std::string& manufacturername)
    : request({{"name", name}, {"modelid", modelid}, {"swversion", swversion}, {"type", type}, {"uniqueid", uniqueid},
        {"manufacturername", manufacturername}})
{ }

CreateSensor& CreateSensor::setState(const nlohmann::json& state)
{
    request["state"] = state;
    return *this;
}

CreateSensor& CreateSensor::setConfig(const nlohmann::json& config)
{
    request["config"] = config;
    return *this;
}

CreateSensor& CreateSensor::setRecycle(bool recycle)
{
    request["recycle"] = recycle;
    return *this;
}

nlohmann::json CreateSensor::getRequest() const
{
    return request;
}

namespace sensors
{

constexpr const char* DaylightSensor::typeStr;

bool DaylightSensor::isOn() const
{
    return state.getValue().at("config").at("on").get<bool>();
}

void DaylightSensor::setOn(bool on)
{
    sendPutRequest("/config", {{"on", on}}, CURRENT_FILE_INFO);
}

bool DaylightSensor::hasBatteryState() const
{
    return state.getValue().at("config").count("battery") != 0;
}
int DaylightSensor::getBatteryState() const
{
    return state.getValue().at("config").at("battery").get<int>();
}
void DaylightSensor::setBatteryState(int percent)
{
    sendPutRequest("/config", nlohmann::json {{"battery", percent}}, CURRENT_FILE_INFO);
}
void DaylightSensor::setCoordinates(const std::string& latitude, const std::string& longitude)
{
    nlohmann::json request {{"lat", latitude}, {"long", longitude}};
    // Currently, "none" is supposed to be used for reset; may change to null in the future,
    // so the functionality is implemented already
    if (latitude.empty())
    {
        request["lat"] = nullptr;
    }
    if (longitude.empty())
    {
        request["long"] = nullptr;
    }
    sendPutRequest("/config", request, CURRENT_FILE_INFO);
}
bool DaylightSensor::isConfigured() const
{
    return state.getValue().at("config").at("configured").get<bool>();
}
int DaylightSensor::getSunriseOffset() const
{
    return state.getValue().at("config").at("sunriseoffset").get<int>();
}
void DaylightSensor::setSunriseOffset(int minutes)
{
    sendPutRequest("/config", nlohmann::json {{"sunriseoffset", minutes}}, CURRENT_FILE_INFO);
}

int DaylightSensor::getSunsetOffset() const
{
    return state.getValue().at("config").at("sunsetoffset").get<int>();
}
void DaylightSensor::setSunsetOffset(int minutes)
{
    sendPutRequest("/config", nlohmann::json {{"sunsetoffset", minutes}}, CURRENT_FILE_INFO);
}

bool DaylightSensor::isDaylight() const
{
    return state.getValue().at("state").at("daylight").get<bool>();
}

time::AbsoluteTime DaylightSensor::getLastUpdated() const
{
    const nlohmann::json& stateJson = state.getValue().at("state");
    auto it = stateJson.find("lastupdated");
    if (it == stateJson.end() || !it->is_string() || *it == "none")
    {
        return time::AbsoluteTime(std::chrono::system_clock::time_point(std::chrono::seconds {0}));
    }
    return time::AbsoluteTime::parseUTC(it->get<std::string>());
}

detail::ConditionHelper<bool> makeCondition(const DaylightSensor& sensor)
{
    return detail::ConditionHelper<bool>("/sensors/" + std::to_string(sensor.getId()) + "/state/daylight");
}

} // namespace sensors
} // namespace hueplusplus
