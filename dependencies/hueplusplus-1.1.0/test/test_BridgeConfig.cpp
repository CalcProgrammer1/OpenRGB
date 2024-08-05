/**
    \file test_BridgeConfig.cpp
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

#include <hueplusplus/BridgeConfig.h>

#include <gtest/gtest.h>

#include "testhelper.h"

#include "mocks/mock_HttpHandler.h"

using namespace hueplusplus;
using namespace testing;

TEST(BridgeConfig, refresh)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);
    auto baseCache = std::make_shared<APICache>("", commands, std::chrono::steady_clock::duration::max(), nullptr);
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername(), nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .WillOnce(Return(nlohmann::json::object()));
    baseCache->refresh();
    BridgeConfig config(baseCache, std::chrono::steady_clock::duration::max());

    EXPECT_CALL(*handler,
        GETJson("/api/" + getBridgeUsername() + "/config", nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .WillOnce(Return(nlohmann::json::object()));
    config.refresh(true);
}

TEST(BridgeConfig, getWhitelistedUsers)
{
    const nlohmann::json state {{"config",
        {{"whitelist",
            {{"abcd",
                 {{"name", "User A"}, {"last use date", "2020-04-01T10:00:04"},
                     {"create date", "2020-01-01T12:00:00"}}},
                {"cdef",
                    {{"name", "User B"}, {"last use date", "2020-03-05T14:00:00"},
                        {"create date", "2020-02-01T02:03:40"}}}}}}}};
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);
    auto baseCache = std::make_shared<APICache>("", commands, std::chrono::steady_clock::duration::max(), nullptr);
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername(), nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .WillOnce(Return(state));
    baseCache->refresh();
    BridgeConfig config(baseCache, std::chrono::steady_clock::duration::max());

    std::vector<WhitelistedUser> users = config.getWhitelistedUsers();
    EXPECT_THAT(users,
        UnorderedElementsAre(Truly([](const WhitelistedUser& u) { return u.key == "abcd" && u.name == "User A"; }),
            Truly([](const WhitelistedUser& u) { return u.key == "cdef" && u.name == "User B"; })));
}

TEST(BridgeConfig, removeUser)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);
    auto baseCache = std::make_shared<APICache>("", commands, std::chrono::steady_clock::duration::max(), nullptr);
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername(), nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .WillOnce(Return(nlohmann::json::object()));
    baseCache->refresh();
    BridgeConfig config(baseCache, std::chrono::steady_clock::duration::max());

    const std::string userKey = "abcd";
    EXPECT_CALL(*handler,
        DELETEJson("/api/" + getBridgeUsername() + "/config/whitelist/" + userKey, nlohmann::json::object(),
            getBridgeIp(), getBridgePort()))
        .WillOnce(Return(nlohmann::json {"/config/whitelist/" + userKey + " deleted"}));
    config.removeUser(userKey);
}

TEST(BridgeConfig, getLinkButton)
{
    const nlohmann::json state {{"config", {{"linkbutton", true}}}};
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);
    auto baseCache = std::make_shared<APICache>("", commands, std::chrono::steady_clock::duration::max(), nullptr);
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername(), nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .WillOnce(Return(state));
    baseCache->refresh();
    BridgeConfig config(baseCache, std::chrono::steady_clock::duration::max());

    EXPECT_TRUE(config.getLinkButton());
    EXPECT_CALL(*handler,
        GETJson("/api/" + getBridgeUsername() + "/config", nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .WillOnce(Return(nlohmann::json {{"linkbutton", false}}));
    config.refresh(true);
    EXPECT_FALSE(config.getLinkButton());
}

TEST(BridgeConfig, pressLinkButton)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);
    auto baseCache = std::make_shared<APICache>("", commands, std::chrono::steady_clock::duration::max(), nullptr);
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername(), nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .WillOnce(Return(nlohmann::json::object()));
    baseCache->refresh();
    BridgeConfig config(baseCache, std::chrono::steady_clock::duration::max());

    EXPECT_CALL(*handler,
        PUTJson("/api/" + getBridgeUsername() + "/config", nlohmann::json {{"linkbutton", true}}, getBridgeIp(),
            getBridgePort()))
        .WillOnce(Return(nlohmann::json {{{"success", {{"/config/linkbutton", true}}}}}));
    config.pressLinkButton();
}

TEST(BridgeConfig, touchLink)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);
    auto baseCache = std::make_shared<APICache>("", commands, std::chrono::steady_clock::duration::max(), nullptr);
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername(), nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .WillOnce(Return(nlohmann::json::object()));
    baseCache->refresh();
    BridgeConfig config(baseCache, std::chrono::steady_clock::duration::max());

    EXPECT_CALL(*handler,
        PUTJson("/api/" + getBridgeUsername() + "/config", nlohmann::json {{"touchlink", true}}, getBridgeIp(),
            getBridgePort()))
        .WillOnce(Return(nlohmann::json {{{"success", {{"/config/touchlink", true}}}}}));
    config.touchLink();
}

TEST(BridgeConfig, getMACAddress)
{
    const nlohmann::json state {{"config", {{"mac", getBridgeMac()}}}};
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);
    auto baseCache = std::make_shared<APICache>("", commands, std::chrono::steady_clock::duration::max(), nullptr);
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername(), nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .WillOnce(Return(state));
    baseCache->refresh();
    BridgeConfig config(baseCache, std::chrono::steady_clock::duration::max());

    EXPECT_EQ(getBridgeMac(), config.getMACAddress());
}

TEST(BridgeConfig, getUTCTime)
{
    const std::string utc = "2020-06-01T10:00:00";
    const nlohmann::json state {{"config", {{"UTC", utc}}}};
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);
    auto baseCache = std::make_shared<APICache>("", commands, std::chrono::steady_clock::duration::max(), nullptr);
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername(), nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .WillOnce(Return(state));
    baseCache->refresh();
    BridgeConfig config(baseCache, std::chrono::steady_clock::duration::max());

    EXPECT_EQ(time::AbsoluteTime::parseUTC(utc).getBaseTime(), config.getUTCTime().getBaseTime());
}

TEST(BridgeConfig, getTimezone)
{
    const std::string timezone = "ab";
    const nlohmann::json state {{"config", {{"timezone", timezone}}}};
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);
    auto baseCache = std::make_shared<APICache>("", commands, std::chrono::steady_clock::duration::max(), nullptr);
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername(), nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .WillOnce(Return(state));
    baseCache->refresh();
    BridgeConfig config(baseCache, std::chrono::steady_clock::duration::max());

    EXPECT_EQ(timezone, config.getTimezone());
}
