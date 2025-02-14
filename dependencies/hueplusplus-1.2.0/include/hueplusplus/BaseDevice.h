/**
    \file BaseDevice.h
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

#ifndef INCLUDE_HUEPLUSPLUS_HUE_THING_H
#define INCLUDE_HUEPLUSPLUS_HUE_THING_H

#include <memory>

#include "APICache.h"

#include <nlohmann/json.hpp>

namespace hueplusplus
{
//! \brief Base class for physical devices connected to the bridge (sensor or light).
class BaseDevice
{
public:
    //! \brief Virtual destructor
    virtual ~BaseDevice() = default;

    //! \brief Const function that returns the id of this device
    //!
    //! \return integer representing the device id
    virtual int getId() const;

    //! \brief Const function that returns the device type
    //!
    //! \return String containing the type
    virtual std::string getType() const;

    //! \brief Function that returns the name of the device.
    //!
    //! \return String containig the name of the device
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual std::string getName();

    //! \brief Const function that returns the name of the device.
    //!
    //! \note This will not refresh the device state
    //! \return String containig the name of the thing
    virtual std::string getName() const;

    //! \brief Const function that returns the modelid of the device
    //!
    //! \return String containing the modelid
    virtual std::string getModelId() const;

    //! \brief Const function that returns the uniqueid of the device
    //!
    //! \note Only working on bridges with versions starting at 1.4
    //! \return String containing the uniqueid or an empty string when the function is not supported
    virtual std::string getUId() const;

    //! \brief Const function that returns the manufacturername of the device
    //!
    //! \note Only working on bridges with versions starting at 1.7
    //! \return String containing the manufacturername or an empty string when the function is not supported
    virtual std::string getManufacturername() const;

    //! \brief Const function that returns the productname of the device
    //!
    //! \note Only working on bridges with versions starting at 1.24
    //! \return String containing the productname or an empty string when the function is not supported
    virtual std::string getProductname() const;

    //! \brief Function that returns the software version of the device
    //!
    //! \return String containing the software version
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual std::string getSwVersion();

    //! \brief Const function that returns the software version of the device
    //!
    //! \note This will not refresh the device state
    //! \return String containing the software version
    virtual std::string getSwVersion() const;

    //! \brief Function that sets the name of the device
    //!
    //! \return Bool that is true on success
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual bool setName(const std::string& name);

    //! \brief Refreshes internal cached state.
    //! \param force \c true forces a refresh, regardless of how long the last refresh was ago.
    //! \c false to only refresh when enough time has passed (needed e.g. when calling only const methods).
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual void refresh(bool force = false);

    //! \brief Sets custom refresh interval for this device.
    //! \param refreshDuration The new minimum duration between refreshes. May be 0 or \ref c_refreshNever.
    virtual void setRefreshDuration(std::chrono::steady_clock::duration refreshDuration);

protected:
    //! \brief Protected ctor that is used by subclasses, construct with shared cache.
    //! \param id Integer that specifies the id of this device
    //! \param baseCache Cache of the ResourceList containing this device (must not be null).
    BaseDevice(int id, const std::shared_ptr<APICache>& baseCache);
    //! \brief Protected ctor that is used by subclasses.
    //!
    //! \param id Integer that specifies the id of this device
    //! \param commands HueCommandAPI for communication with the bridge
    //! \param path Base path for the resource type, ending with a '/'. Example: \c "/lights/"
    //! \param refreshDuration Time between refreshing the cached state.
    //! \param currentState Current state of the device, may be null.
    BaseDevice(int id, const HueCommandAPI& commands, const std::string& path,
        std::chrono::steady_clock::duration refreshDuration, const nlohmann::json& currentState);

    //! \brief Utility function to send a put request to the device.
    //!
    //! \param subPath A path that is appended to the uri, note it should always start with a slash ("/")
    //! \param request A nlohmann::json aka the request to send
    //! \param fileInfo FileInfo from calling function for exception details.
    //! \return The parsed reply
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual nlohmann::json sendPutRequest(const std::string& subPath, const nlohmann::json& request, FileInfo fileInfo);

protected:
    int id; //!< holds the id of the device
    APICache state; //!< holds the current state of the device
};
} // namespace hueplusplus

#endif
