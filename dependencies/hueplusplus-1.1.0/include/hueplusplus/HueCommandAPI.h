/**
    \file HueCommandAPI.h
    Copyright Notice\n
    Copyright (C) 2018  Jan Rogall		- developer\n
    Copyright (C) 2018  Moritz Wirger	- developer\n

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

#ifndef INCLUDE_HUEPLUSPLUS_HUECOMMANDAPI_H
#define INCLUDE_HUEPLUSPLUS_HUECOMMANDAPI_H

#include <atomic>
#include <chrono>
#include <mutex>

#include "HueException.h"
#include "IHttpHandler.h"

namespace hueplusplus
{
//! Handles communication to the bridge via IHttpHandler and enforces a timeout
//! between each request
class HueCommandAPI
{
public:
    //! \brief Construct from ip, username and HttpHandler
    //!
    //! \param ip ip address of the Hue bridge in dotted decimal notation like "192.168.2.1"
    //! \param port of the hue bridge
    //! \param username username that is used to control the bridge
    //! \param httpHandler HttpHandler for communication with the bridge
    HueCommandAPI(
        const std::string& ip, int port, const std::string& username, std::shared_ptr<const IHttpHandler> httpHandler);

    //! \brief Copy construct from other HueCommandAPI
    //! \note All copies refer to the same timeout data, so even calls from different objects will be delayed
    HueCommandAPI(const HueCommandAPI&) = default;
    //! \brief Move construct from other HueCommandAPI
    //! \note All copies refer to the same timeout data, so even calls from different objects will be delayed
    HueCommandAPI(HueCommandAPI&&) = default;

    //! \brief Copy assign from other HueCommandAPI
    //! \note All copies refer to the same timeout data, so even calls from different objects will be delayed
    HueCommandAPI& operator=(const HueCommandAPI&) = default;
    //! \brief Move assign from other HueCommandAPI
    //! \note All copies refer to the same timeout data, so even calls from different objects will be delayed
    HueCommandAPI& operator=(HueCommandAPI&&) = default;

    //! \brief Sends a HTTP PUT request to the bridge and returns the response
    //!
    //! This function will block until at least Config::getBridgeRequestDelay() has passed to any previous request
    //! \param path API request path (appended after /api/{username})
    //! \param request Request to the api, may be empty
    //! \param fileInfo File information for thrown exceptions.
    //! \returns The return value of the underlying \ref IHttpHandler::PUTJson call
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contains no body
    //! \throws HueAPIResponseException when response contains an error
    nlohmann::json PUTRequest(const std::string& path, const nlohmann::json& request, FileInfo fileInfo) const;
    //! \overload
    nlohmann::json PUTRequest(const std::string& path, const nlohmann::json& request) const;

    //! \brief Sends a HTTP GET request to the bridge and returns the response
    //!
    //! This function will block until at least Config::getBridgeRequestDelay() has passed to any previous request
    //! \param path API request path (appended after /api/{username})
    //! \param request Request to the api, may be empty
    //! \param fileInfo File information for thrown exceptions.
    //! \returns The return value of the underlying \ref IHttpHandler::GETJson call
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contains no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    nlohmann::json GETRequest(const std::string& path, const nlohmann::json& request, FileInfo fileInfo) const;
    //! \overload
    nlohmann::json GETRequest(const std::string& path, const nlohmann::json& request) const;

    //! \brief Sends a HTTP DELETE request to the bridge and returns the response
    //!
    //! This function will block until at least Config::getBridgeRequestDelay() has passed to any previous request
    //! \param path API request path (appended after /api/{username})
    //! \param request Request to the api, may be empty
    //! \param fileInfo File information for thrown exceptions.
    //! \returns The return value of the underlying \ref IHttpHandler::DELETEJson call
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contains no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    nlohmann::json DELETERequest(const std::string& path, const nlohmann::json& request, FileInfo fileInfo) const;
    //! \overload
    nlohmann::json DELETERequest(const std::string& path, const nlohmann::json& request) const;

    //! \brief Sends a HTTP POST request to the bridge and returns the response
    //!
    //! This function will block until at least Config::getBridgeRequestDelay() has passed to any previous request
    //! \param path API request path (appended after /api/{username})
    //! \param request Request to the api, may be empty
    //! \param fileInfo File information for thrown exceptions.
    //! \returns The return value of the underlying \ref IHttpHandler::POSTJson call
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contains no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    nlohmann::json POSTRequest(const std::string& path, const nlohmann::json& request, FileInfo fileInfo) const;
    //! \overload
    nlohmann::json POSTRequest(const std::string& path, const nlohmann::json& request) const;

    //! \brief Combines path with api prefix and username
    //! \returns "/api/<username>/<path>"
    std::string combinedPath(const std::string& path) const;
private:
    struct TimeoutData
    {
        std::chrono::steady_clock::time_point timeout;
        std::mutex mutex;
    };

    //! \brief Throws an exception if response contains an error, passes though value
    //! \throws HueAPIResponseException when response contains an error
    //! \returns \ref response if there is no error
    nlohmann::json HandleError(FileInfo fileInfo, const nlohmann::json& response) const;

private:
    std::string ip;
    int port;
    std::string username;
    std::shared_ptr<const IHttpHandler> httpHandler;
    std::shared_ptr<TimeoutData> timeout;
};
} // namespace hueplusplus

#endif