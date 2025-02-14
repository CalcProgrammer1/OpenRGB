/**
    \file test_HueCommandAPI.cpp
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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "testhelper.h"

#include "hueplusplus/Bridge.h"
#include <nlohmann/json.hpp>
#include "mocks/mock_HttpHandler.h"

using namespace hueplusplus;

TEST(HueCommandAPI, PUTRequest)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> httpHandler = std::make_shared<MockHttpHandler>();

    HueCommandAPI api(getBridgeIp(), getBridgePort(), getBridgeUsername(), httpHandler);
    nlohmann::json request;
    nlohmann::json result = nlohmann::json::object();
    result["ok"] = true;

    // empty path
    {
        EXPECT_CALL(*httpHandler, PUTJson("/api/" + getBridgeUsername(), request, getBridgeIp(), 80))
            .WillOnce(Return(result));
        EXPECT_EQ(result, api.PUTRequest("", request));
        Mock::VerifyAndClearExpectations(httpHandler.get());
    }
    // not empty path, starting with slash
    {
        const std::string path = "/test";
        EXPECT_CALL(*httpHandler, PUTJson("/api/" + getBridgeUsername() + path, request, getBridgeIp(), 80))
            .WillOnce(Return(result));
        EXPECT_EQ(result, api.PUTRequest(path, request));
        Mock::VerifyAndClearExpectations(httpHandler.get());
    }
    // not empty path, not starting with slash
    {
        const std::string path = "test";
        EXPECT_CALL(*httpHandler, PUTJson("/api/" + getBridgeUsername() + '/' + path, request, getBridgeIp(), 80))
            .WillOnce(Return(result));
        EXPECT_EQ(result, api.PUTRequest(path, request));
        Mock::VerifyAndClearExpectations(httpHandler.get());
    }
    // recoverable error
    {
        const std::string path = "/test";
        EXPECT_CALL(*httpHandler, PUTJson("/api/" + getBridgeUsername() + path, request, getBridgeIp(), 80))
            .WillOnce(Throw(std::system_error(std::make_error_code(std::errc::connection_reset))))
            .WillOnce(Return(result));
        EXPECT_EQ(result, api.PUTRequest(path, request));
        Mock::VerifyAndClearExpectations(httpHandler.get());
    }
    // recoverable error x2
    {
        const std::string path = "/test";
        EXPECT_CALL(*httpHandler, PUTJson("/api/" + getBridgeUsername() + path, request, getBridgeIp(), 80))
            .WillOnce(Throw(std::system_error(std::make_error_code(std::errc::connection_reset))))
            .WillOnce(Throw(std::system_error(std::make_error_code(std::errc::connection_reset))));
        EXPECT_THROW(api.PUTRequest(path, request), std::system_error);
        Mock::VerifyAndClearExpectations(httpHandler.get());
    }
    // unrecoverable error
    {
        const std::string path = "/test";
        EXPECT_CALL(*httpHandler, PUTJson("/api/" + getBridgeUsername() + path, request, getBridgeIp(), 80))
            .WillOnce(Throw(std::system_error(std::make_error_code(std::errc::not_enough_memory))));
        EXPECT_THROW(api.PUTRequest(path, request), std::system_error);
        Mock::VerifyAndClearExpectations(httpHandler.get());
    }
    // api returns error
    {
        const std::string path = "/test";
        const nlohmann::json errorResponse{{"error", {{"type", 10}, {"address", path}, {"description", "Stuff"}}}};
        EXPECT_CALL(*httpHandler, PUTJson("/api/" + getBridgeUsername() + path, request, getBridgeIp(), 80))
            .WillOnce(Return(errorResponse));
        EXPECT_THROW(api.PUTRequest(path, request), HueAPIResponseException);
        Mock::VerifyAndClearExpectations(httpHandler.get());
    }
}

TEST(HueCommandAPI, GETRequest)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> httpHandler = std::make_shared<MockHttpHandler>();

    HueCommandAPI api(getBridgeIp(), getBridgePort(), getBridgeUsername(), httpHandler);
    nlohmann::json request;
    nlohmann::json result = nlohmann::json::object();
    result["ok"] = true;

    // empty path
    {
        EXPECT_CALL(*httpHandler, GETJson("/api/" + getBridgeUsername(), request, getBridgeIp(), 80))
            .WillOnce(Return(result));
        EXPECT_EQ(result, api.GETRequest("", request));
        Mock::VerifyAndClearExpectations(httpHandler.get());
    }
    // not empty path, starting with slash
    {
        const std::string path = "/test";
        EXPECT_CALL(*httpHandler, GETJson("/api/" + getBridgeUsername() + path, request, getBridgeIp(), 80))
            .WillOnce(Return(result));
        EXPECT_EQ(result, api.GETRequest(path, request));
        Mock::VerifyAndClearExpectations(httpHandler.get());
    }
    // not empty path, not starting with slash
    {
        const std::string path = "test";
        EXPECT_CALL(*httpHandler, GETJson("/api/" + getBridgeUsername() + '/' + path, request, getBridgeIp(), 80))
            .WillOnce(Return(result));
        EXPECT_EQ(result, api.GETRequest(path, request));
        Mock::VerifyAndClearExpectations(httpHandler.get());
    }
    // recoverable error
    {
        const std::string path = "/test";
        EXPECT_CALL(*httpHandler, GETJson("/api/" + getBridgeUsername() + path, request, getBridgeIp(), 80))
            .WillOnce(Throw(std::system_error(std::make_error_code(std::errc::connection_reset))))
            .WillOnce(Return(result));
        EXPECT_EQ(result, api.GETRequest(path, request));
        Mock::VerifyAndClearExpectations(httpHandler.get());
    }
    // recoverable error x2
    {
        const std::string path = "/test";
        EXPECT_CALL(*httpHandler, GETJson("/api/" + getBridgeUsername() + path, request, getBridgeIp(), 80))
            .WillOnce(Throw(std::system_error(std::make_error_code(std::errc::connection_reset))))
            .WillOnce(Throw(std::system_error(std::make_error_code(std::errc::connection_reset))));
        EXPECT_THROW(api.GETRequest(path, request), std::system_error);
        Mock::VerifyAndClearExpectations(httpHandler.get());
    }
    // unrecoverable error
    {
        const std::string path = "/test";
        EXPECT_CALL(*httpHandler, GETJson("/api/" + getBridgeUsername() + path, request, getBridgeIp(), 80))
            .WillOnce(Throw(std::system_error(std::make_error_code(std::errc::not_enough_memory))));
        EXPECT_THROW(api.GETRequest(path, request), std::system_error);
        Mock::VerifyAndClearExpectations(httpHandler.get());
    }
    // api returns error
    {
        const std::string path = "/test";
        const nlohmann::json errorResponse{{"error", {{"type", 10}, {"address", path}, {"description", "Stuff"}}}};
        EXPECT_CALL(*httpHandler, GETJson("/api/" + getBridgeUsername() + path, request, getBridgeIp(), 80))
            .WillOnce(Return(errorResponse));
        EXPECT_THROW(api.GETRequest(path, request), HueAPIResponseException);
        Mock::VerifyAndClearExpectations(httpHandler.get());
    }
}

TEST(HueCommandAPI, DELETERequest)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> httpHandler = std::make_shared<MockHttpHandler>();

    HueCommandAPI api(getBridgeIp(), getBridgePort(), getBridgeUsername(), httpHandler);
    nlohmann::json request;
    nlohmann::json result = nlohmann::json::object();
    result["ok"] = true;

    // empty path
    {
        EXPECT_CALL(*httpHandler, DELETEJson("/api/" + getBridgeUsername(), request, getBridgeIp(), 80))
            .WillOnce(Return(result));
        EXPECT_EQ(result, api.DELETERequest("", request));
        Mock::VerifyAndClearExpectations(httpHandler.get());
    }
    // not empty path, starting with slash
    {
        const std::string path = "/test";
        EXPECT_CALL(*httpHandler, DELETEJson("/api/" + getBridgeUsername() + path, request, getBridgeIp(), 80))
            .WillOnce(Return(result));
        EXPECT_EQ(result, api.DELETERequest(path, request));
        Mock::VerifyAndClearExpectations(httpHandler.get());
    }
    // not empty path, not starting with slash
    {
        const std::string path = "test";
        EXPECT_CALL(*httpHandler, DELETEJson("/api/" + getBridgeUsername() + '/' + path, request, getBridgeIp(), 80))
            .WillOnce(Return(result));
        EXPECT_EQ(result, api.DELETERequest(path, request));
        Mock::VerifyAndClearExpectations(httpHandler.get());
    }
    // recoverable error
    {
        const std::string path = "/test";
        EXPECT_CALL(*httpHandler, DELETEJson("/api/" + getBridgeUsername() + path, request, getBridgeIp(), 80))
            .WillOnce(Throw(std::system_error(std::make_error_code(std::errc::connection_reset))))
            .WillOnce(Return(result));
        EXPECT_EQ(result, api.DELETERequest(path, request));
        Mock::VerifyAndClearExpectations(httpHandler.get());
    }
    // recoverable error x2
    {
        const std::string path = "/test";
        EXPECT_CALL(*httpHandler, DELETEJson("/api/" + getBridgeUsername() + path, request, getBridgeIp(), 80))
            .WillOnce(Throw(std::system_error(std::make_error_code(std::errc::connection_reset))))
            .WillOnce(Throw(std::system_error(std::make_error_code(std::errc::connection_reset))));
        EXPECT_THROW(api.DELETERequest(path, request), std::system_error);
        Mock::VerifyAndClearExpectations(httpHandler.get());
    }
    // unrecoverable error
    {
        const std::string path = "/test";
        EXPECT_CALL(*httpHandler, DELETEJson("/api/" + getBridgeUsername() + path, request, getBridgeIp(), 80))
            .WillOnce(Throw(std::system_error(std::make_error_code(std::errc::not_enough_memory))));
        EXPECT_THROW(api.DELETERequest(path, request), std::system_error);
        Mock::VerifyAndClearExpectations(httpHandler.get());
    }
    // api returns error
    {
        const std::string path = "/test";
        const nlohmann::json errorResponse{{"error", {{"type", 10}, {"address", path}, {"description", "Stuff"}}}};
        EXPECT_CALL(*httpHandler, DELETEJson("/api/" + getBridgeUsername() + path, request, getBridgeIp(), 80))
            .WillOnce(Return(errorResponse));
        EXPECT_THROW(api.DELETERequest(path, request), HueAPIResponseException);
        Mock::VerifyAndClearExpectations(httpHandler.get());
    }
}
