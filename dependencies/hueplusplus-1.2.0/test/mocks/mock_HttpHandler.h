/**
    \file mock_HttpHandler.h
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

#ifndef _MOCK_HTTPHANDLER_H
#define _MOCK_HTTPHANDLER_H

#include <string>
#include <vector>

#include <gmock/gmock.h>

#include "hueplusplus/IHttpHandler.h"
#include <nlohmann/json.hpp>

//! Mock Class
class MockHttpHandler : public hueplusplus::IHttpHandler
{
public:
    MOCK_CONST_METHOD3(send, std::string(const std::string& msg, const std::string& adr, int port));

    MOCK_CONST_METHOD3(sendGetHTTPBody, std::string(const std::string& msg, const std::string& adr, int port));

    MOCK_CONST_METHOD4(sendMulticast,
        std::vector<std::string>(
            const std::string& msg, const std::string& adr, int port, std::chrono::steady_clock::duration timeout));

    MOCK_CONST_METHOD6(sendHTTPRequest,
        std::string(const std::string& method, const std::string& uri, const std::string& content_type,
            const std::string& body, const std::string& adr, int port));

    MOCK_CONST_METHOD5(GETString,
        std::string(const std::string& uri, const std::string& content_type, const std::string& body,
            const std::string& adr, int port));

    MOCK_CONST_METHOD5(POSTString,
        std::string(const std::string& uri, const std::string& content_type, const std::string& body,
            const std::string& adr, int port));

    MOCK_CONST_METHOD5(PUTString,
        std::string(const std::string& uri, const std::string& content_type, const std::string& body,
            const std::string& adr, int port));

    MOCK_CONST_METHOD5(DELETEString,
        std::string(const std::string& uri, const std::string& content_type, const std::string& body,
            const std::string& adr, int port));

    MOCK_CONST_METHOD4(
        GETJson, nlohmann::json(const std::string& uri, const nlohmann::json& body, const std::string& adr, int port));

    MOCK_CONST_METHOD4(
        POSTJson, nlohmann::json(const std::string& uri, const nlohmann::json& body, const std::string& adr, int port));

    MOCK_CONST_METHOD4(
        PUTJson, nlohmann::json(const std::string& uri, const nlohmann::json& body, const std::string& adr, int port));

    MOCK_CONST_METHOD4(DELETEJson,
        nlohmann::json(const std::string& uri, const nlohmann::json& body, const std::string& adr, int port));
};

#endif
