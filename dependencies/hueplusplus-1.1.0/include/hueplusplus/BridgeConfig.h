/**
    \file BridgeConfig.h
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

#ifndef INCLUDE_HUEPLUSPLUS_BRIDGE_CONFIG_H
#define INCLUDE_HUEPLUSPLUS_BRIDGE_CONFIG_H

#include <string>
#include <vector>

#include "APICache.h"
#include "TimePattern.h"

namespace hueplusplus
{
//! \brief API version consisting of major, minor and patch version
struct Version
{
    int major;
    int minor;
    int patch;
};

//! \brief User that is whitelisted for Hue API usage
struct WhitelistedUser
{
    //! \brief API username of the user
    std::string key;
    //! \brief Name provided on user creation
    std::string name;
    //! \brief Last time the user was used
    time::AbsoluteTime lastUsed;
    //! \brief Time the user was created
    time::AbsoluteTime created;
};

//! \brief General bridge configuration properties.
class BridgeConfig
{
public:
    //! \brief Construct BridgeConfig
    BridgeConfig(std::shared_ptr<APICache> baseCache, std::chrono::steady_clock::duration refreshDuration);

    //! \brief Refreshes internal cached state.
    //! \param force \c true forces a refresh, regardless of how long the last refresh was ago.
    //! \c false to only refresh when enough time has passed (needed e.g. when calling only const methods).
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void refresh(bool force = false);

    //! \brief Sets custom refresh interval for the config.
    //! \param refreshDuration The new minimum duration between refreshes. May be 0 or \ref c_refreshNever.
    void setRefreshDuration(std::chrono::steady_clock::duration refreshDuration);

    //! \brief Get the list of whitelisted users
    //! \returns All users authorized for API access
    std::vector<WhitelistedUser> getWhitelistedUsers() const;
    //! \brief Remove user from the whitelist
    //! \param userKey The API username of the user to remove
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void removeUser(const std::string& userKey);

    //! \brief Get link button state
    //! \returns true when link button was pressed in the last 30 seconds.
    //!
    //! Indicates whether new users can be added currently.
    bool getLinkButton() const;
    //! \brief Set the link button state to pressed
    void pressLinkButton();

    //! \brief Add the closest lamp to the network
    void touchLink();

    //! \brief Get bridge MAC address
    std::string getMACAddress() const;
    //! \brief Get current (of last refresh) UTC time of the bridge
    time::AbsoluteTime getUTCTime() const;
    //! \brief Get configured timezone for the bridge
    //! \note For times not in UTC, the timezone of the program and the bridge are assumed to be identical.
    std::string getTimezone() const;

protected:
    BridgeConfig(const BridgeConfig&) = default;
    BridgeConfig(BridgeConfig&&) = default;
    BridgeConfig& operator=(const BridgeConfig&) = default;
    BridgeConfig& operator=(BridgeConfig&&) = default;

private:
    APICache cache;
};
} // namespace hueplusplus

#endif
