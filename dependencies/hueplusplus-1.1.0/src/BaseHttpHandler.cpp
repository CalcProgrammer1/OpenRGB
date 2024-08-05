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

#include "hueplusplus/BaseHttpHandler.h"

#include "hueplusplus/HueExceptionMacro.h"

namespace hueplusplus
{
std::string BaseHttpHandler::sendGetHTTPBody(const std::string& msg, const std::string& adr, int port) const
{
    std::string response = send(msg, adr, port);
    size_t start = response.find("\r\n\r\n");
    if (start == std::string::npos)
    {
        std::cerr << "BaseHttpHandler: Failed to find body in response\n";
        std::cerr << "Request:\n";
        std::cerr << "\"" << msg << "\"\n";
        std::cerr << "Response:\n";
        std::cerr << "\"" << response << "\"\n";
        throw HueException(CURRENT_FILE_INFO, "Failed to find body in response");
    }
    response.erase(0, start + 4);
    return response;
}

std::string BaseHttpHandler::sendHTTPRequest(const std::string& method, const std::string& uri,
    const std::string& contentType, const std::string& body, const std::string& adr, int port) const
{
    std::string request;
    // Protocol reference:
    // https://www.w3.org/Protocols/rfc2616/rfc2616-sec5.html Request-Line
    request.append(method); // Method
    request.append(" "); // Separation
    request.append(uri); // Request-URI
    request.append(" "); // Separation
    request.append("HTTP/1.0"); // HTTP-Version
    request.append("\r\n"); // Ending
                            // Entities
    if (!contentType.empty())
    {
        request.append("Content-Type:"); // entity-header
        request.append(" "); // Separation
        request.append(contentType); // media-type
        request.append("\r\n"); // Entity ending
    }
    if (!body.empty())
    {
        request.append("Content-Length:"); // entity-header
        request.append(" "); // Separation
        request.append(std::to_string(body.size())); // length
        request.append("\r\n\r\n"); // Entity ending & Request-Line ending
    }
    request.append(body); // message-body
    request.append("\r\n\r\n"); // Ending

    return sendGetHTTPBody(request.c_str(), adr, port);
}

std::string BaseHttpHandler::GETString(const std::string& uri, const std::string& contentType, const std::string& body,
    const std::string& adr, int port) const
{
    return sendHTTPRequest("GET", uri, contentType, body, adr, port);
}

std::string BaseHttpHandler::POSTString(const std::string& uri, const std::string& contentType, const std::string& body,
    const std::string& adr, int port) const
{
    return sendHTTPRequest("POST", uri, contentType, body, adr, port);
}

std::string BaseHttpHandler::PUTString(const std::string& uri, const std::string& contentType, const std::string& body,
    const std::string& adr, int port) const
{
    return sendHTTPRequest("PUT", uri, contentType, body, adr, port);
}

std::string BaseHttpHandler::DELETEString(const std::string& uri, const std::string& contentType,
    const std::string& body, const std::string& adr, int port) const
{
    return sendHTTPRequest("DELETE", uri, contentType, body, adr, port);
}

nlohmann::json BaseHttpHandler::GETJson(
    const std::string& uri, const nlohmann::json& body, const std::string& adr, int port) const
{
    return nlohmann::json::parse(GETString(uri, "application/json", body.dump(), adr, port));
}

nlohmann::json BaseHttpHandler::POSTJson(
    const std::string& uri, const nlohmann::json& body, const std::string& adr, int port) const
{
    return nlohmann::json::parse(POSTString(uri, "application/json", body.dump(), adr, port));
}

nlohmann::json BaseHttpHandler::PUTJson(
    const std::string& uri, const nlohmann::json& body, const std::string& adr, int port) const
{
    return nlohmann::json::parse(PUTString(uri, "application/json", body.dump(), adr, port));
}

nlohmann::json BaseHttpHandler::DELETEJson(
    const std::string& uri, const nlohmann::json& body, const std::string& adr, int port) const
{
    return nlohmann::json::parse(DELETEString(uri, "application/json", body.dump(), adr, port));
}
} // namespace hueplusplus
