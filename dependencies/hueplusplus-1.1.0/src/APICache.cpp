#include "hueplusplus/APICache.h"
/**
    \file BaseHttpHandler.cpp
    Copyright Notice\n
    Copyright (C) 2020  Jan Rogall		- developer\n
    Copyright (C) 2020  Moritz Wirger	- developer\n

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

#include "hueplusplus/HueExceptionMacro.h"

namespace hueplusplus
{

APICache::APICache(
    std::shared_ptr<APICache> baseCache, const std::string& subEntry, std::chrono::steady_clock::duration refresh)
    : base(baseCache),
      path(subEntry),
      commands(baseCache->commands),
      refreshDuration(refresh),
      lastRefresh(baseCache->lastRefresh)
{ }

APICache::APICache(const std::string& path, const HueCommandAPI& commands, std::chrono::steady_clock::duration refresh,
    const nlohmann::json& initial)
    : path(path),
      commands(commands),
      refreshDuration(refresh),
      lastRefresh(initial.is_null() ? std::chrono::steady_clock::time_point() : std::chrono::steady_clock::now()),
      value(initial)
{ }

void APICache::refresh()
{
    // Only refresh part of the cache, because that is more efficient
    if (base && base->needsRefresh())
    {
        base->refresh();
    }
    else
    {
        nlohmann::json result = commands.GETRequest(getRequestPath(), nlohmann::json::object(), CURRENT_FILE_INFO);
        lastRefresh = std::chrono::steady_clock::now();
        if (base)
        {
            base->value[path] = std::move(result);
        }
        else
        {
            value = std::move(result);
        }
    }
}

nlohmann::json& APICache::getValue()
{
    if (needsRefresh())
    {
        refresh();
    }
    if (base)
    {
        // Do not call getValue here, because that could cause another refresh
        // if base has refresh duration 0
        nlohmann::json& baseState = base->value;
        auto pos = baseState.find(path);
        if (pos != baseState.end())
        {
            return *pos;
        }
        else
        {
            throw HueException(CURRENT_FILE_INFO, "Child path not present in base cache");
        }
    }
    else
    {
        return value;
    }
}

const nlohmann::json& APICache::getValue() const
{
    if (base)
    {
        // Make const reference to not refresh
        const APICache& b = *base;
        return b.getValue().at(path);
    }
    else
    {
        if (lastRefresh.time_since_epoch().count() == 0)
        {
            // No value has been requested yet
            throw HueException(CURRENT_FILE_INFO,
                "Tried to call const getValue(), but no value was cached. "
                "Call refresh() or non-const getValue() first.");
        }
        return value;
    }
}

void APICache::setRefreshDuration(std::chrono::steady_clock::duration refreshDuration)
{
    this->refreshDuration = refreshDuration;
}

std::chrono::steady_clock::duration APICache::getRefreshDuration() const
{
    return refreshDuration;
}

HueCommandAPI& APICache::getCommandAPI()
{
    return commands;
}

const HueCommandAPI& APICache::getCommandAPI() const
{
    return commands;
}

bool APICache::needsRefresh()
{
    using clock = std::chrono::steady_clock;
    if (base)
    {
        // Update lastRefresh in case base was refreshed
        lastRefresh = std::max(lastRefresh, base->lastRefresh);
    }

    // Explicitly check for zero in case refreshDuration is duration::max()
    // Negative duration causes overflow check to overflow itself
    if (lastRefresh.time_since_epoch().count() == 0 || refreshDuration.count() < 0)
    {
        // No value set yet
        return true;
    }
    // Check if nextRefresh would overflow (assumes lastRefresh is not negative, which it should not be).
    // If addition would overflow, do not refresh
    else if (clock::duration::max() - refreshDuration > lastRefresh.time_since_epoch())
    {
        clock::time_point nextRefresh = lastRefresh + refreshDuration;
        if (clock::now() >= nextRefresh)
        {
            return true;
        }
    }
    return false;
}

std::string APICache::getRequestPath() const
{
    std::string result;
    if (base)
    {
        result = base->getRequestPath();
        result.push_back('/');
    }
    result.append(path);
    return result;
}
} // namespace hueplusplus
