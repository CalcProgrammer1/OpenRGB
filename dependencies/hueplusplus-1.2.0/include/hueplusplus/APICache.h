/**
    \file APICache.h
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

#ifndef INCLUDE_API_CACHE_H
#define INCLUDE_API_CACHE_H

#include <chrono>
#include <string>

#include "HueCommandAPI.h"

namespace hueplusplus
{
//! \brief Maximum duration, used to indicate that the cache should never be refreshed automatically.
constexpr std::chrono::steady_clock::duration c_refreshNever = std::chrono::steady_clock::duration::max();

//! \brief Caches API GET requests and refreshes regularly.
class APICache
{
public:
    //! \brief Constructs APICache which forwards to a base cache
    //! \param baseCache Base cache providing a parent state, must not be nullptr
    //! \param subEntry Key of the child to use in the base cache
    //! \param refresh Interval between cache refreshing. May be 0 to always refresh.
    //! This is independent from the base cache refresh rate.
    //!
    //! Refreshes only part of the base cache.
    APICache(
        std::shared_ptr<APICache> baseCache, const std::string& subEntry, std::chrono::steady_clock::duration refresh);

    //! \brief Constructs APICache with an own internal json cache
    //! \param path URL appended after username, may be empty.
    //! \param commands HueCommandAPI for making API requests.
    //! \param refresh Interval between cache refreshing. May be 0 to always refresh.
    //! \param initial Initial value, may be null. If present, assumes the value is up to date.
    APICache(const std::string& path, const HueCommandAPI& commands, std::chrono::steady_clock::duration refresh,
        const nlohmann::json& initial);

    //! \brief Refresh cache now.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    //!
    //! If there is a base cache, refreshes only the used part of that cache.
    void refresh();

    //! \brief Get cached value, refresh if necessary.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    nlohmann::json& getValue();
    //! \brief Get cached value, does not refresh.
    //! \throws HueException when no previous request was cached
    const nlohmann::json& getValue() const;

    //! \brief Set duration after which the cache is refreshed.
    //! \param refreshDuration Interval between cache refreshing.
    //! May be 0 to always refresh, or \ref c_refreshNever to never refresh.
    //!
    //! If the new refresh duration is exceeded, does not refresh immediately.
    //! Instead, the next non-const getValue() call will refresh the value.
    //! This is to reduce the number of unneccessary requests.
    void setRefreshDuration(std::chrono::steady_clock::duration refreshDuration);

    //! \brief Get duration between refreshes.
    std::chrono::steady_clock::duration getRefreshDuration() const;

    //! \brief Get HueCommandAPI used for requests
    HueCommandAPI& getCommandAPI();
    //! \brief Get HueCommandAPI used for requests
    const HueCommandAPI& getCommandAPI() const;

    //! \brief Get path the cache is refreshed from
    //! \returns Request path as passed to HueCommandAPI::GETRequest
    std::string getRequestPath() const;

private:
    bool needsRefresh();

private:
    std::shared_ptr<APICache> base;
    std::string path;
    HueCommandAPI commands;
    std::chrono::steady_clock::duration refreshDuration;
    std::chrono::steady_clock::time_point lastRefresh;
    nlohmann::json value;
};
} // namespace hueplusplus

#endif
