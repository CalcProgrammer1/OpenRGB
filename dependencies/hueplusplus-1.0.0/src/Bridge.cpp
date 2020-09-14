/**
    \file Bridge.cpp
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

#include "hueplusplus/Bridge.h"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstring>
#include <iostream>
#include <locale>
#include <stdexcept>
#include <thread>

#include "hueplusplus/HueExceptionMacro.h"
#include "hueplusplus/LibConfig.h"
#include "hueplusplus/UPnP.h"
#include "hueplusplus/Utils.h"

namespace hueplusplus
{
BridgeFinder::BridgeFinder(std::shared_ptr<const IHttpHandler> handler) : http_handler(std::move(handler)) { }

std::vector<BridgeFinder::BridgeIdentification> BridgeFinder::findBridges() const
{
    UPnP uplug;
    std::vector<std::pair<std::string, std::string>> foundDevices = uplug.getDevices(http_handler);

    std::vector<BridgeIdentification> foundBridges;
    for (const std::pair<std::string, std::string>& p : foundDevices)
    {
        size_t found = p.second.find("IpBridge");
        if (found != std::string::npos)
        {
            BridgeIdentification bridge;
            size_t start = p.first.find("//") + 2;
            size_t length = p.first.find(":", start) - start;
            bridge.ip = p.first.substr(start, length);
            try
            {
                std::string desc
                    = http_handler->GETString("/description.xml", "application/xml", "", bridge.ip, bridge.port);
                std::string mac = parseDescription(desc);
                if (!mac.empty())
                {
                    bridge.mac = normalizeMac(mac);
                    foundBridges.push_back(std::move(bridge));
                }
            }
            catch (const HueException&)
            {
                // No body found in response, skip this device
            }
        }
    }
    return foundBridges;
}

Bridge BridgeFinder::getBridge(const BridgeIdentification& identification, bool sharedState)
{
    std::string normalizedMac = normalizeMac(identification.mac);
    auto pos = usernames.find(normalizedMac);
    auto key = clientkeys.find(normalizedMac);
    if (pos != usernames.end())
    {
        if (key != clientkeys.end())
        {
            return Bridge(identification.ip, identification.port, pos->second, http_handler, key->second,
                std::chrono::seconds(10), sharedState);
        }
        else
        {
            return Bridge(identification.ip, identification.port, pos->second, http_handler, "",
                std::chrono::seconds(10), sharedState);
        }
    }
    Bridge bridge(identification.ip, identification.port, "", http_handler, "", std::chrono::seconds(10), sharedState);
    bridge.requestUsername();
    if (bridge.getUsername().empty())
    {
        std::cerr << "Failed to request username for ip " << identification.ip << std::endl;
        throw HueException(CURRENT_FILE_INFO, "Failed to request username!");
    }
    addUsername(normalizedMac, bridge.getUsername());
    addClientKey(normalizedMac, bridge.getClientKey());

    return bridge;
}

void BridgeFinder::addUsername(const std::string& mac, const std::string& username)
{
    usernames[normalizeMac(mac)] = username;
}

void BridgeFinder::addClientKey(const std::string& mac, const std::string& clientkey)
{
    clientkeys[normalizeMac(mac)] = clientkey;
}

const std::map<std::string, std::string>& BridgeFinder::getAllUsernames() const
{
    return usernames;
}

std::string BridgeFinder::normalizeMac(std::string input)
{
    // Remove any non alphanumeric characters (e.g. ':' and whitespace)
    input.erase(std::remove_if(input.begin(), input.end(), [](char c) { return !std::isalnum(c, std::locale()); }),
        input.end());
    // Convert to lower case
    std::transform(input.begin(), input.end(), input.begin(), [](char c) { return std::tolower(c, std::locale()); });
    return input;
}

std::string BridgeFinder::parseDescription(const std::string& description)
{
    const char* model = "<modelName>Philips hue bridge";
    const char* serialBegin = "<serialNumber>";
    const char* serialEnd = "</serialNumber>";
    if (description.find(model) != std::string::npos)
    {
        std::size_t begin = description.find(serialBegin);
        std::size_t end = description.find(serialEnd, begin);
        if (begin != std::string::npos && end != std::string::npos)
        {
            begin += std::strlen(serialBegin);
            if (begin < description.size())
            {
                std::string result = description.substr(begin, end - begin);
                return result;
            }
        }
    }
    return std::string();
}

Bridge::Bridge(const std::string& ip, const int port, const std::string& username,
    std::shared_ptr<const IHttpHandler> handler, const std::string& clientkey,
    std::chrono::steady_clock::duration refreshDuration, bool sharedState)
    : ip(ip),
      username(username),
      clientkey(clientkey),
      port(port),
      http_handler(std::move(handler)),
      refreshDuration(refreshDuration),
      stateCache(std::make_shared<APICache>(
          "", HueCommandAPI(ip, port, username, http_handler), std::chrono::steady_clock::duration::max(), nullptr)),
      lightList(stateCache, "lights", refreshDuration, sharedState,
          [factory = LightFactory(stateCache->getCommandAPI(), refreshDuration)](
              int id, const nlohmann::json& state, const std::shared_ptr<APICache>& baseCache) mutable {
              return factory.createLight(state, id, baseCache);
          }),
      groupList(stateCache, "groups", refreshDuration, sharedState),
      scheduleList(stateCache, "schedules", refreshDuration, sharedState),
      sceneList(stateCache, "scenes", refreshDuration, sharedState),
      sensorList(stateCache, "sensors", refreshDuration, sharedState),
      ruleList(stateCache, "rules", refreshDuration, sharedState),
      bridgeConfig(stateCache, refreshDuration),
      sharedState(sharedState)
{ }

void Bridge::refresh()
{
    stateCache->refresh();
}

void Bridge::setRefreshDuration(std::chrono::steady_clock::duration refreshDuration)
{
    stateCache->setRefreshDuration(refreshDuration);
    lightList.setRefreshDuration(refreshDuration);
    groupList.setRefreshDuration(refreshDuration);
    scheduleList.setRefreshDuration(refreshDuration);
    sceneList.setRefreshDuration(refreshDuration);
    sensorList.setRefreshDuration(refreshDuration);
    ruleList.setRefreshDuration(refreshDuration);
    bridgeConfig.setRefreshDuration(refreshDuration);
}

std::string Bridge::getBridgeIP() const
{
    return ip;
}

int Bridge::getBridgePort() const
{
    return port;
}

std::string Bridge::requestUsername()
{
    std::chrono::steady_clock::duration timeout = Config::instance().getRequestUsernameTimeout();
    std::chrono::steady_clock::duration checkInterval = Config::instance().getRequestUsernameAttemptInterval();
    std::cout << "Please press the link Button! You've got "
              << std::chrono::duration_cast<std::chrono::seconds>(timeout).count() << " secs!\n";

    // when the link button was pressed we got 30 seconds to get our username for control
    nlohmann::json request;
    request["devicetype"] = "HuePlusPlus#User";
    request["generateclientkey"] = true;

    nlohmann::json answer;
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    // do-while loop to check at least once when timeout is 0
    do
    {
        std::this_thread::sleep_for(checkInterval);
        answer = http_handler->POSTJson("/api", request, ip, port);
        nlohmann::json jsonUser = utils::safeGetMember(answer, 0, "success", "username");
        nlohmann::json jsonKey = utils::safeGetMember(answer, 0, "success", "clientkey");
        if (jsonUser != nullptr)
        {
            // [{"success":{"username": "<username>"}}]
            username = jsonUser.get<std::string>();
            // Update commands with new username and ip
            setHttpHandler(http_handler);
            std::cout << "Success! Link button was pressed!\n";
            std::cout << "Username is \"" << username << "\"\n";

            if (jsonKey != nullptr)
            {
                clientkey = jsonKey.get<std::string>();
                std::cout << "Client key is \"" << clientkey << "\"\n";
            }
            break;
        }
        else if (answer.size() > 0 && answer[0].count("error"))
        {
            HueAPIResponseException exception = HueAPIResponseException::Create(CURRENT_FILE_INFO, answer[0]);
            // All errors except 101: Link button not pressed
            if (exception.GetErrorNumber() != 101)
            {
                throw exception;
            }
        }
    } while (std::chrono::steady_clock::now() - start < timeout);

    return username;
}

bool Bridge::startStreaming(std::string group_identifier)
{
    if (clientkey.empty())
    {
        throw HueException(CURRENT_FILE_INFO, "Cannot stream without client key!");
    }

    nlohmann::json request;

    request["stream"]["active"] = true;

    nlohmann::json answer;

    std::string uri = "/api/" + username + "/groups/" + group_identifier;

    answer = http_handler->PUTJson(uri, request, ip, port);

    std::string key = "/groups/" + group_identifier + "/stream/active";
    nlohmann::json success = utils::safeGetMember(answer, 0, "success", key);

    return success == true;
}

bool Bridge::stopStreaming(std::string group_identifier)
{
    nlohmann::json request;

    request["stream"]["active"] = false;

    nlohmann::json answer;

    std::string uri = "/api/" + username + "/groups/" + group_identifier;

    answer = http_handler->PUTJson(uri, request, ip, port);

    if (answer[0].contains("success"))
    {
        std::string key = "/groups/" + group_identifier + "/stream/active";

        if (answer[0]["success"].contains(key))
        {
            if (answer[0]["success"][key] == false)
            {
                return true;
            }
        }
    }

    return false;
}

std::string Bridge::getUsername() const
{
    return username;
}

std::string Bridge::getClientKey() const
{
    return clientkey;
}

void Bridge::setIP(const std::string& ip)
{
    this->ip = ip;
}

void Bridge::setPort(const int port)
{
    this->port = port;
}

BridgeConfig& Bridge::config()
{
    return bridgeConfig;
}

const BridgeConfig& Bridge::config() const
{
    return bridgeConfig;
}

Bridge::LightList& Bridge::lights()
{
    return lightList;
}

const Bridge::LightList& Bridge::lights() const
{
    return lightList;
}

Bridge::GroupList& Bridge::groups()
{
    return groupList;
}

const Bridge::GroupList& Bridge::groups() const
{
    return groupList;
}

Bridge::ScheduleList& Bridge::schedules()
{
    return scheduleList;
}

const Bridge::ScheduleList& Bridge::schedules() const
{
    return scheduleList;
}

Bridge::SceneList& Bridge::scenes()
{
    return sceneList;
}

const Bridge::SceneList& Bridge::scenes() const
{
    return sceneList;
}

hueplusplus::SensorList& Bridge::sensors()
{
    return sensorList;
}

const hueplusplus::SensorList& Bridge::sensors() const
{
    return sensorList;
}

Bridge::RuleList& Bridge::rules()
{
    return ruleList;
}

const Bridge::RuleList& Bridge::rules() const
{
    return ruleList;
}

void Bridge::setHttpHandler(std::shared_ptr<const IHttpHandler> handler)
{
    http_handler = handler;
    stateCache = std::make_shared<APICache>("", HueCommandAPI(ip, port, username, handler), refreshDuration, nullptr);
    lightList = LightList(stateCache, "lights", refreshDuration, sharedState,
        [factory = LightFactory(stateCache->getCommandAPI(), refreshDuration)](int id, const nlohmann::json& state,
            const std::shared_ptr<APICache>& baseCache) mutable { return factory.createLight(state, id, baseCache); });
    groupList = GroupList(stateCache, "groups", refreshDuration, sharedState);
    scheduleList = ScheduleList(stateCache, "schedules", refreshDuration, sharedState);
    sceneList = SceneList(stateCache, "scenes", refreshDuration, sharedState);
    sensorList = SensorList(stateCache, "sensors", refreshDuration, sharedState);
    ruleList = RuleList(stateCache, "rules", refreshDuration, sharedState);
    bridgeConfig = BridgeConfig(stateCache, refreshDuration);
    stateCache->refresh();
}
} // namespace hueplusplus
