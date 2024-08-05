/**
    \file BridgeConfig.cpp
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

#include <hueplusplus/BridgeConfig.h>
#include <hueplusplus/HueExceptionMacro.h>

namespace hueplusplus
{
BridgeConfig::BridgeConfig(std::shared_ptr<APICache> baseCache, std::chrono::steady_clock::duration refreshDuration)
    : cache(std::move(baseCache), "config", refreshDuration)
{ }

void BridgeConfig::refresh(bool force)
{
    if (force)
    {
        cache.refresh();
    }
    else
    {
        cache.getValue();
    }
}

void BridgeConfig::setRefreshDuration(std::chrono::steady_clock::duration refreshDuration)
{
    cache.setRefreshDuration(refreshDuration);
}

std::vector<WhitelistedUser> BridgeConfig::getWhitelistedUsers() const
{
    const nlohmann::json& whitelist = cache.getValue().at("whitelist");
    std::vector<WhitelistedUser> users;
    for (auto it = whitelist.begin(); it != whitelist.end(); ++it)
    {
        users.push_back({it.key(), it->at("name").get<std::string>(),
            time::AbsoluteTime::parseUTC(it->at("last use date").get<std::string>()),
            time::AbsoluteTime::parseUTC(it->at("create date").get<std::string>())});
    }
    return users;
}
void BridgeConfig::removeUser(const std::string& userKey)
{
    cache.getCommandAPI().DELETERequest("/config/whitelist/" + userKey, nlohmann::json::object());
}
bool BridgeConfig::getLinkButton() const
{
    return cache.getValue().at("linkbutton").get<bool>();
}
void BridgeConfig::pressLinkButton()
{
    cache.getCommandAPI().PUTRequest("/config", {{"linkbutton", true}}, CURRENT_FILE_INFO);
}
void BridgeConfig::touchLink()
{
    cache.getCommandAPI().PUTRequest("/config", {{"touchlink", true}}, CURRENT_FILE_INFO);
}
std::string BridgeConfig::getMACAddress() const
{
    return cache.getValue().at("mac").get<std::string>();
}
time::AbsoluteTime BridgeConfig::getUTCTime() const
{
    return time::AbsoluteTime::parseUTC(cache.getValue().at("UTC").get<std::string>());
}
std::string BridgeConfig::getTimezone() const
{
    return cache.getValue().at("timezone").get<std::string>();
}
} // namespace hueplusplus