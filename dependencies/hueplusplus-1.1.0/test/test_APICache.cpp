/**
    \file test_Hue.cpp
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

#include <gtest/gtest.h>

#include "testhelper.h"

#include "hueplusplus/APICache.h"
#include "mocks/mock_HttpHandler.h"

using namespace hueplusplus;

TEST(APICache, getRefreshDuration)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);
    {
        std::chrono::steady_clock::duration refresh = std::chrono::seconds(20);
        APICache cache("", commands, refresh, nullptr);
        EXPECT_EQ(refresh, cache.getRefreshDuration());
    }
    {
        std::chrono::steady_clock::duration refresh = std::chrono::seconds(0);
        APICache cache("", commands, refresh, nullptr);
        EXPECT_EQ(refresh, cache.getRefreshDuration());
    }
    {
        std::chrono::steady_clock::duration refresh = c_refreshNever;
        APICache cache("", commands, refresh, nullptr);
        EXPECT_EQ(refresh, cache.getRefreshDuration());
    }
    // With base cache, still independent duration
    {
        auto duration = std::chrono::seconds(5);
        auto baseCache = std::make_shared<APICache>("/test", commands, std::chrono::seconds(0), nullptr);
        APICache c(baseCache, "api", duration);
        EXPECT_EQ(duration, c.getRefreshDuration());
    }
}

TEST(APICache, refresh)
{
    using namespace ::testing;
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);

    {
        std::string path = "/test/abc";
        APICache cache(path, commands, std::chrono::seconds(10), nullptr);
        EXPECT_CALL(*handler,
            GETJson("/api/" + getBridgeUsername() + path, nlohmann::json::object(), getBridgeIp(), getBridgePort()))
            .WillOnce(Return(nlohmann::json::object()));
        cache.refresh();
        Mock::VerifyAndClearExpectations(handler.get());
    }
    {
        std::string path = "";
        APICache cache(path, commands, std::chrono::seconds(10), nullptr);
        EXPECT_CALL(*handler,
            GETJson("/api/" + getBridgeUsername() + path, nlohmann::json::object(), getBridgeIp(), getBridgePort()))
            .Times(2)
            .WillRepeatedly(Return(nlohmann::json::object()));
        cache.refresh();
        cache.refresh();
        Mock::VerifyAndClearExpectations(handler.get());
    }
}

TEST(APICache, refreshBase)
{
    using namespace ::testing;
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);
    const std::string basePath = "/test";
    const std::string childPath = "/test/abc";
    // Base cache with max duration
    {
        auto baseCache
            = std::make_shared<APICache>(basePath, commands, c_refreshNever, nullptr);
        APICache cache(baseCache, "abc", std::chrono::seconds(0));

        // First call refreshes base, second call only child
        InSequence s;
        EXPECT_CALL(*handler,
            GETJson("/api/" + getBridgeUsername() + basePath, nlohmann::json::object(), getBridgeIp(), getBridgePort()))
            .WillOnce(Return(nlohmann::json::object()));
        EXPECT_CALL(*handler,
            GETJson(
                "/api/" + getBridgeUsername() + childPath, nlohmann::json::object(), getBridgeIp(), getBridgePort()))
            .WillOnce(Return(nlohmann::json::object()));
        cache.refresh();
        cache.refresh();
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Base cache with min duration
    {
        auto baseCache = std::make_shared<APICache>(basePath, commands, std::chrono::seconds(0), nullptr);
        APICache cache(baseCache, "abc", std::chrono::seconds(0));

        // Both calls refresh base
        EXPECT_CALL(*handler,
            GETJson("/api/" + getBridgeUsername() + basePath, nlohmann::json::object(), getBridgeIp(), getBridgePort()))
            .Times(2)
            .WillRepeatedly(Return(nlohmann::json::object()));
        cache.refresh();
        cache.refresh();
        Mock::VerifyAndClearExpectations(handler.get());
    }
}

TEST(APICache, getValue)
{
    using namespace ::testing;
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);

    // Always refresh
    {
        std::string path = "/test/abc";
        APICache cache(path, commands, std::chrono::seconds(0), nullptr);
        nlohmann::json value = {{"a", "b"}};
        EXPECT_CALL(*handler,
            GETJson("/api/" + getBridgeUsername() + path, nlohmann::json::object(), getBridgeIp(), getBridgePort()))
            .Times(2)
            .WillRepeatedly(Return(value));
        EXPECT_EQ(value, cache.getValue());
        EXPECT_EQ(value, cache.getValue());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Only refresh once
    {
        std::string path = "/test/abc";
        APICache cache(path, commands, c_refreshNever, nullptr);
        nlohmann::json value = {{"a", "b"}};
        EXPECT_CALL(*handler,
            GETJson("/api/" + getBridgeUsername() + path, nlohmann::json::object(), getBridgeIp(), getBridgePort()))
            .WillOnce(Return(value));
        EXPECT_EQ(value, cache.getValue());
        EXPECT_EQ(value, cache.getValue());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Only refresh once
    {
        std::string path = "/test/abc";
        APICache cache(path, commands, std::chrono::seconds(0), nullptr);
        nlohmann::json value = {{"a", "b"}};
        EXPECT_CALL(*handler,
            GETJson("/api/" + getBridgeUsername() + path, nlohmann::json::object(), getBridgeIp(), getBridgePort()))
            .WillOnce(Return(value));
        EXPECT_EQ(value, cache.getValue());
        EXPECT_EQ(value, Const(cache).getValue());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // No refresh with const throws exception
    {
        std::string path = "/test/abc";
        const APICache cache(path, commands, c_refreshNever, nullptr);
        nlohmann::json value = {{"a", "b"}};
        EXPECT_CALL(*handler,
            GETJson("/api/" + getBridgeUsername() + path, nlohmann::json::object(), getBridgeIp(), getBridgePort()))
            .Times(0);
        EXPECT_THROW(cache.getValue(), HueException);
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // No refresh with initial value
    {
        std::string path = "/test/abc";
        nlohmann::json value = {{"a", "b"}};
        APICache cache(path, commands, c_refreshNever, value);
        EXPECT_CALL(*handler,
            GETJson("/api/" + getBridgeUsername() + path, nlohmann::json::object(), getBridgeIp(), getBridgePort()))
            .Times(0);
        EXPECT_EQ(value, cache.getValue());
        EXPECT_EQ(value, cache.getValue());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // No refresh with const but initial value
    {
        std::string path = "/test/abc";
        nlohmann::json value = {{"a", "b"}};
        const APICache cache(path, commands, c_refreshNever, value);
        EXPECT_CALL(*handler,
            GETJson("/api/" + getBridgeUsername() + path, nlohmann::json::object(), getBridgeIp(), getBridgePort()))
            .Times(0);
        EXPECT_EQ(value, cache.getValue());
        Mock::VerifyAndClearExpectations(handler.get());
    }
}

TEST(APICache, getValueBase)
{
    using namespace ::testing;
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);

    const std::string basePath = "/test";
    const std::string childPath = "/test/abc";
    const nlohmann::json childValue = {{"test", "value"}};
    const nlohmann::json baseValue = {{"abc", childValue}};
    // Always refresh base
    {
        auto baseCache = std::make_shared<APICache>(basePath, commands, std::chrono::seconds(0), nullptr);
        APICache cache(baseCache, "abc", std::chrono::seconds(0));
        EXPECT_CALL(*handler,
            GETJson("/api/" + getBridgeUsername() + basePath, nlohmann::json::object(), getBridgeIp(), getBridgePort()))
            .Times(2)
            .WillRepeatedly(Return(baseValue));
        EXPECT_EQ(childValue, cache.getValue());
        EXPECT_EQ(childValue, cache.getValue());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Child duration > base duration
    {
        auto baseCache = std::make_shared<APICache>(basePath, commands, std::chrono::seconds(0), nullptr);
        APICache cache(baseCache, "abc", c_refreshNever);
        EXPECT_CALL(*handler,
            GETJson("/api/" + getBridgeUsername() + basePath, nlohmann::json::object(), getBridgeIp(), getBridgePort()))
            .Times(1)
            .WillRepeatedly(Return(baseValue));
        EXPECT_EQ(childValue, cache.getValue());
        EXPECT_EQ(childValue, cache.getValue());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Child duration < base duration
    {
        auto baseCache
            = std::make_shared<APICache>(basePath, commands, c_refreshNever, nullptr);
        APICache cache(baseCache, "abc", std::chrono::seconds(0));
        const nlohmann::json updateChildValue = {{"test", "updated"}};
        InSequence s;
        EXPECT_CALL(*handler,
            GETJson("/api/" + getBridgeUsername() + basePath, nlohmann::json::object(), getBridgeIp(), getBridgePort()))
            .Times(1)
            .WillRepeatedly(Return(baseValue));
        EXPECT_CALL(*handler,
            GETJson(
                "/api/" + getBridgeUsername() + childPath, nlohmann::json::object(), getBridgeIp(), getBridgePort()))
            .Times(1)
            .WillRepeatedly(Return(updateChildValue));
        EXPECT_EQ(childValue, cache.getValue());
        EXPECT_EQ(updateChildValue, cache.getValue());
        // Base cache is updated
        EXPECT_EQ(updateChildValue, baseCache->getValue()["abc"]);
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Only refresh once
    {
        auto baseCache
            = std::make_shared<APICache>(basePath, commands, c_refreshNever, nullptr);
        APICache cache(baseCache, "abc", c_refreshNever);
        EXPECT_CALL(*handler,
            GETJson("/api/" + getBridgeUsername() + basePath, nlohmann::json::object(), getBridgeIp(), getBridgePort()))
            .Times(1)
            .WillRepeatedly(Return(baseValue));
        EXPECT_EQ(childValue, cache.getValue());
        EXPECT_EQ(childValue, cache.getValue());
        Mock::VerifyAndClearExpectations(handler.get());
    }
}


TEST(APICache, setRefreshDuration)
{
    using namespace ::testing;
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);

    {
        std::string path = "/test/abc";
        APICache cache(path, commands, std::chrono::seconds(0), nullptr);
        nlohmann::json value = { {"a", "b"} };
        EXPECT_CALL(*handler,
            GETJson("/api/" + getBridgeUsername() + path, nlohmann::json::object(), getBridgeIp(), getBridgePort()))
            .Times(1)
            .WillOnce(Return(value));
        EXPECT_EQ(value, cache.getValue());
        cache.setRefreshDuration(c_refreshNever);
        EXPECT_EQ(c_refreshNever, cache.getRefreshDuration());
        // Next getValue does not refresh
        EXPECT_EQ(value, cache.getValue());
        Mock::VerifyAndClearExpectations(handler.get());
    }
}


TEST(APICache, getRequestPath)
{
    using namespace ::testing;
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);

    // No base cache
    {
        std::string path = "/test/api";
        APICache c(path, commands, std::chrono::seconds(0), nullptr);
        EXPECT_EQ(path, c.getRequestPath());
    }
    // With base cache
    {
        auto baseCache = std::make_shared<APICache>("/test", commands, std::chrono::seconds(0), nullptr);
        APICache c(baseCache, "api", std::chrono::seconds(0));
        EXPECT_EQ("/test/api", c.getRequestPath());
    }
}
