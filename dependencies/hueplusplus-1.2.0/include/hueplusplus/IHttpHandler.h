/**
    \file IHttpHandler.h
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

#ifndef INCLUDE_HUEPLUSPLUS_IHTTPHANDLER_H
#define INCLUDE_HUEPLUSPLUS_IHTTPHANDLER_H

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

namespace hueplusplus
{
//! Abstract class for classes that handle http requests and multicast requests
class IHttpHandler
{
public:
    //! \brief Virtual dtor
    virtual ~IHttpHandler() = default;

    //! \brief Send a message to a specified host and return the response.
    //!
    //! \param msg The message that should be sent to the specified address
    //! \param adr Ip or hostname in dotted decimal notation like "192.168.2.1"
    //! \param port Optional port the request is sent to, default is 80
    //! \return The response of the host as a string
    //! \throws std::system_error when system or socket operations fail
    virtual std::string send(const std::string& msg, const std::string& adr, int port = 80) const = 0;

    //! \brief Send a message to a specified host and return the body of the response.
    //!
    //! \param msg The message that should sent to the specified address
    //! \param adr Ip or hostname in dotted decimal notation like "192.168.2.1"
    //! \param port Optional port the request is sent to, default is 80
    //! \return The body of the response of the host as a string
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    virtual std::string sendGetHTTPBody(const std::string& msg, const std::string& adr, int port = 80) const = 0;

    //! \brief Send a multicast request with a specified message.
    //!
    //! \param msg The message that should sent to the specified multicast address
    //! \param adr Optional ip or hostname in dotted decimal notation, default is "239.255.255.250"
    //! \param port Optional port the request is sent to, default is 1900
    //! \param timeout Optional time to wait for responses, default is 5 seconds
    //!
    //! Blocks for the duration of the timeout.
    //!
    //! \return vector of strings containing each received answer
    //! \throws std::system_error when system or socket operations fail
    virtual std::vector<std::string> sendMulticast(const std::string& msg, const std::string& adr = "239.255.255.250",
        int port = 1900, std::chrono::steady_clock::duration timeout = std::chrono::seconds(5)) const = 0;

    //! \brief Send a HTTP request with the given method to the specified host and return the body of the response.
    //!
    //! \param method HTTP method type e.g. GET, HEAD, POST, PUT, DELETE, ...
    //! \param uri Uniform Resource Identifier in the request
    //! \param contentType MIME type of the body data e.g. "text/html", "application/json", ...
    //! \param body Request body, may be empty
    //! \param adr Ip or hostname in dotted decimal notation like "192.168.2.1"
    //! \param port Optional port the request is sent to, default is 80
    //! \return Body of the response of the host
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    virtual std::string sendHTTPRequest(const std::string& method, const std::string& uri,
        const std::string& contentType, const std::string& body, const std::string& adr, int port = 80) const = 0;

    //! \brief Send a HTTP GET request to the specified host and return the body of the response.
    //!
    //! \param uri Uniform Resource Identifier in the request
    //! \param contentType MIME type of the body data e.g. "text/html", "application/json", ...
    //! \param body Request body, may be empty
    //! \param adr Ip or hostname in dotted decimal notation like "192.168.2.1"
    //! \param port Optional port the request is sent to, default is 80
    //! that specifies the port to which the request is sent to. Default is 80
    //! \return Body of the response of the host
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    virtual std::string GETString(const std::string& uri, const std::string& contentType, const std::string& body,
        const std::string& adr, int port = 80) const = 0;

    //! \brief Send a HTTP POST request to the specified host and return the body of the response.
    //!
    //! \param uri Uniform Resource Identifier in the request
    //! \param contentType MIME type of the body data e.g. "text/html", "application/json", ...
    //! \param body Request body, may be empty
    //! \param adr Ip or hostname in dotted decimal notation like "192.168.2.1"
    //! \param port Optional port the request is sent to, default is 80
    //! that specifies the port to which the request is sent to. Default is 80
    //! \return Body of the response of the host
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    virtual std::string POSTString(const std::string& uri, const std::string& contentType, const std::string& body,
        const std::string& adr, int port = 80) const = 0;

    //! \brief Send a HTTP PUT request to the specified host and return the body of the response.
    //!
    //! \param uri Uniform Resource Identifier in the request
    //! \param contentType MIME type of the body data e.g. "text/html", "application/json", ...
    //! \param body Request body, may be empty
    //! \param adr Ip or hostname in dotted decimal notation like "192.168.2.1"
    //! \param port Optional port the request is sent to, default is 80
    //! that specifies the port to which the request is sent to. Default is 80
    //! \return Body of the response of the host
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    virtual std::string PUTString(const std::string& uri, const std::string& contentType, const std::string& body,
        const std::string& adr, int port = 80) const = 0;

    //! \brief Send a HTTP DELETE request to the specified host and return the body of the response.
    //!
    //! \param uri Uniform Resource Identifier in the request
    //! \param contentType MIME type of the body data e.g. "text/html", "application/json", ...
    //! \param body Request body, may be empty
    //! \param adr Ip or hostname in dotted decimal notation like "192.168.2.1"
    //! \param port Optional port the request is sent to, default is 80
    //! that specifies the port to which the request is sent to. Default is 80
    //! \return Body of the response of the host
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    virtual std::string DELETEString(const std::string& uri, const std::string& contentType, const std::string& body,
        const std::string& adr, int port = 80) const = 0;

    //! \brief Send a HTTP GET request to the specified host and return the body of the response parsed as JSON.
    //!
    //! \param uri Uniform Resource Identifier in the request
    //! \param body Request body, may be empty
    //! \param adr Ip or hostname in dotted decimal notation like "192.168.2.1"
    //! \param port Optional port the request is sent to, default is 80
    //! \return Parsed body of the response of the host
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws nlohmann::json::parse_error when the body could not be parsed
    virtual nlohmann::json GETJson(
        const std::string& uri, const nlohmann::json& body, const std::string& adr, int port = 80) const = 0;

    //! \brief Send a HTTP POST request to the specified host and return the body of the response parsed as JSON.
    //!
    //! \param uri Uniform Resource Identifier in the request
    //! \param body Request body, may be empty
    //! \param adr Ip or hostname in dotted decimal notation like "192.168.2.1"
    //! \param port Optional port the request is sent to, default is 80
    //! \return Parsed body of the response of the host
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws nlohmann::json::parse_error when the body could not be parsed
    virtual nlohmann::json POSTJson(
        const std::string& uri, const nlohmann::json& body, const std::string& adr, int port = 80) const = 0;

    //! \brief Send a HTTP PUT request to the specified host and return the body of the response parsed as JSON.
    //!
    //! \param uri Uniform Resource Identifier in the request
    //! \param body Request body, may be empty
    //! \param adr Ip or hostname in dotted decimal notation like "192.168.2.1"
    //! \param port Optional port the request is sent to, default is 80
    //! \return Parsed body of the response of the host
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws nlohmann::json::parse_error when the body could not be parsed
    virtual nlohmann::json PUTJson(
        const std::string& uri, const nlohmann::json& body, const std::string& adr, int port = 80) const = 0;

    //! \brief Send a HTTP DELETE request to the specified host and return the body of the response parsed as JSON.
    //!
    //! \param uri Uniform Resource Identifier in the request
    //! \param body Request body, may be empty
    //! \param adr Ip or hostname in dotted decimal notation like "192.168.2.1"
    //! \param port Optional port the request is sent to, default is 80
    //! \return Parsed body of the response of the host
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws nlohmann::json::parse_error when the body could not be parsed
    virtual nlohmann::json DELETEJson(
        const std::string& uri, const nlohmann::json& body, const std::string& adr, int port = 80) const = 0;
};
} // namespace hueplusplus

#endif
