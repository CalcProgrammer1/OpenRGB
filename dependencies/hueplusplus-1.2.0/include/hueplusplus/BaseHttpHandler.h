/**
    \file BaseHttpHandler.h
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

#ifndef INCLUDE_HUEPLUSPLUS_BASE_HTTP_HANDLER_H
#define INCLUDE_HUEPLUSPLUS_BASE_HTTP_HANDLER_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "IHttpHandler.h"

#include <nlohmann/json.hpp>

namespace hueplusplus
{
//! Base class for classes that handle http requests and multicast requests
class BaseHttpHandler : public IHttpHandler
{
public:
    //! \brief Virtual dtor
    virtual ~BaseHttpHandler() = default;

    //! \brief Send a message to a specified host and return the body of the response.
    //!
    //! \param msg The message that should sent to the specified address
    //! \param adr Ip or hostname in dotted decimal notation like "192.168.2.1"
    //! \param port Optional port the request is sent to, default is 80
    //! \return The body of the response of the host as a string
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    std::string sendGetHTTPBody(const std::string& msg, const std::string& adr, int port = 80) const override;

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
    std::string sendHTTPRequest(const std::string& method, const std::string& uri, const std::string& contentType,
        const std::string& body, const std::string& adr, int port = 80) const override;

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
    std::string GETString(const std::string& uri, const std::string& contentType, const std::string& body,
        const std::string& adr, int port = 80) const override;

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
    std::string POSTString(const std::string& uri, const std::string& contentType, const std::string& body,
        const std::string& adr, int port = 80) const override;

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
    std::string PUTString(const std::string& uri, const std::string& contentType, const std::string& body,
        const std::string& adr, int port = 80) const override;

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
    std::string DELETEString(const std::string& uri, const std::string& contentType, const std::string& body,
        const std::string& adr, int port = 80) const override;

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
    nlohmann::json GETJson(
        const std::string& uri, const nlohmann::json& body, const std::string& adr, int port = 80) const override;

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
    nlohmann::json POSTJson(
        const std::string& uri, const nlohmann::json& body, const std::string& adr, int port = 80) const override;

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
    nlohmann::json PUTJson(
        const std::string& uri, const nlohmann::json& body, const std::string& adr, int port = 80) const override;

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
    nlohmann::json DELETEJson(
        const std::string& uri, const nlohmann::json& body, const std::string& adr, int port = 80) const override;
};
} // namespace hueplusplus

#endif
