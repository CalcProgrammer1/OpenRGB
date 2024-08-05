/**
    \file LibConfig.h
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

#ifndef INCLUDE_HUEPLUSPLUS_HUE_CONFIG_H
#define INCLUDE_HUEPLUSPLUS_HUE_CONFIG_H

#include <chrono>

namespace hueplusplus
{
//! \brief Configurable delays
//!
//! Used to set all delays to zero when running tests.
class Config
{
private:
    using duration = std::chrono::steady_clock::duration;

public:
    //! \brief Delay for advanced alerts before the actual alert
    duration getPreAlertDelay() const { return preAlertDelay; }
    //! \brief Delay for advanced alerts after the actual alert
    duration getPostAlertDelay() const { return postAlertDelay; }

    //! \brief Timeout for UPnP multicast request
    duration getUPnPTimeout() const { return upnpTimeout; }

    //! \brief Delay between bridge requests
    duration getBridgeRequestDelay() const { return bridgeRequestDelay; }

    //! \brief Timeout for Bridge::requestUsername, waits until link button was pressed
    duration getRequestUsernameTimeout() const { return requestUsernameDelay; }

    //! \brief Interval in which username requests are attempted
    duration getRequestUsernameAttemptInterval() const { return requestUsernameAttemptInterval; }

    //! \brief Get config instance
    static Config& instance()
    {
        static Config c;
        return c;
    }
protected:
    Config() = default;

protected:
    duration preAlertDelay = std::chrono::milliseconds(120);
    duration postAlertDelay = std::chrono::milliseconds(1600);
    duration upnpTimeout = std::chrono::seconds(5);
    duration bridgeRequestDelay = std::chrono::milliseconds(100);
    duration requestUsernameDelay = std::chrono::seconds(35);
    duration requestUsernameAttemptInterval = std::chrono::seconds(1);
};
} // namespace hueplusplus

#endif
