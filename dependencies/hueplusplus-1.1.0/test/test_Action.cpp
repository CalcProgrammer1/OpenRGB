/**
    \file test_Action.cpp
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

#include <hueplusplus/Action.h>
#include <hueplusplus/HueException.h>

#include <gtest/gtest.h>

using namespace testing;
using hueplusplus::Action;

TEST(Action, Constructor)
{
    const std::string address = "/api/abcd/test";
    const nlohmann::json body = {{"test", "value"}};
    const nlohmann::json json = {{"address", address}, {"method", "PUT"}, {"body", body}};
    Action command(json);

    EXPECT_EQ(address, command.getAddress());
    EXPECT_EQ(Action::Method::put, command.getMethod());
    EXPECT_EQ(body, command.getBody());
    EXPECT_EQ(json, command.toJson());
}

TEST(Action, getMethod)
{
    nlohmann::json json = {{"address", "/test"}, {"method", "PUT"}, {"body", {}}};
    EXPECT_EQ(Action::Method::put, Action(json).getMethod());
    json["method"] = "POST";
    EXPECT_EQ(Action::Method::post, Action(json).getMethod());
    json["method"] = "DELETE";
    EXPECT_EQ(Action::Method::deleteMethod, Action(json).getMethod());
    json["method"] = "unknown";
    EXPECT_THROW(Action(json).getMethod(), hueplusplus::HueException);
}

TEST(Action, parseMethod)
{
    using M = Action::Method;
    EXPECT_EQ(M::put, Action::parseMethod("PUT"));
    EXPECT_EQ(M::post, Action::parseMethod("POST"));
    EXPECT_EQ(M::deleteMethod, Action::parseMethod("DELETE"));
    EXPECT_THROW(Action::parseMethod("unknown"), hueplusplus::HueException);
}

TEST(Action, methodToString)
{
    using M = Action::Method;
    EXPECT_EQ("POST", Action::methodToString(M::post));
    EXPECT_EQ("PUT", Action::methodToString(M::put));
    EXPECT_EQ("DELETE", Action::methodToString(M::deleteMethod));
}
