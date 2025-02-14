/**
    \file test_BaseHttpHandler.cpp
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

#include <memory>
#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "testhelper.h"

#include "hueplusplus/HueException.h"
#include <nlohmann/json.hpp>
#include "mocks/mock_BaseHttpHandler.h"

using namespace hueplusplus;

TEST(BaseHttpHandler, sendGetHTTPBody)
{
    using namespace ::testing;
    MockBaseHttpHandler handler;

    EXPECT_CALL(handler, send("testmsg", "192.168.2.1", 90))
        .Times(AtLeast(2))
        .WillOnce(Return(""))
        .WillRepeatedly(Return("\r\n\r\ntestreply"));

    EXPECT_THROW(handler.sendGetHTTPBody("testmsg", "192.168.2.1", 90), HueException);
    EXPECT_EQ("testreply", handler.sendGetHTTPBody("testmsg", "192.168.2.1", 90));
}

TEST(BaseHttpHandler, sendHTTPRequest)
{
    using namespace ::testing;
    MockBaseHttpHandler handler;

    EXPECT_CALL(handler,
        send("GET UrI HTTP/1.0\r\nContent-Type: "
             "text/html\r\nContent-Length: 4\r\n\r\nbody\r\n\r\n",
            "192.168.2.1", 90))
        .Times(AtLeast(2))
        .WillOnce(Return(""))
        .WillRepeatedly(Return("\r\n\r\ntestreply"));

    EXPECT_THROW(handler.sendHTTPRequest("GET", "UrI", "text/html", "body", "192.168.2.1", 90), HueException);
    EXPECT_EQ("testreply", handler.sendHTTPRequest("GET", "UrI", "text/html", "body", "192.168.2.1", 90));
}

TEST(BaseHttpHandler, GETString)
{
    using namespace ::testing;
    MockBaseHttpHandler handler;

    EXPECT_CALL(handler,
        send("GET UrI HTTP/1.0\r\nContent-Type: "
             "text/html\r\nContent-Length: 4\r\n\r\nbody\r\n\r\n",
            "192.168.2.1", 90))
        .Times(AtLeast(2))
        .WillOnce(Return(""))
        .WillRepeatedly(Return("\r\n\r\ntestreply"));

    EXPECT_THROW(handler.GETString("UrI", "text/html", "body", "192.168.2.1", 90), HueException);
    EXPECT_EQ("testreply", handler.GETString("UrI", "text/html", "body", "192.168.2.1", 90));
}

TEST(BaseHttpHandler, POSTString)
{
    using namespace ::testing;
    MockBaseHttpHandler handler;

    EXPECT_CALL(handler,
        send("POST UrI HTTP/1.0\r\nContent-Type: "
             "text/html\r\nContent-Length: 4\r\n\r\nbody\r\n\r\n",
            "192.168.2.1", 90))
        .Times(AtLeast(2))
        .WillOnce(Return(""))
        .WillRepeatedly(Return("\r\n\r\ntestreply"));

    EXPECT_THROW(handler.POSTString("UrI", "text/html", "body", "192.168.2.1", 90), HueException);
    EXPECT_EQ("testreply", handler.POSTString("UrI", "text/html", "body", "192.168.2.1", 90));
}

TEST(BaseHttpHandler, PUTString)
{
    using namespace ::testing;
    MockBaseHttpHandler handler;

    EXPECT_CALL(handler,
        send("PUT UrI HTTP/1.0\r\nContent-Type: "
             "text/html\r\nContent-Length: 4\r\n\r\nbody\r\n\r\n",
            "192.168.2.1", 90))
        .Times(AtLeast(2))
        .WillOnce(Return(""))
        .WillRepeatedly(Return("\r\n\r\ntestreply"));

    EXPECT_THROW(handler.PUTString("UrI", "text/html", "body", "192.168.2.1", 90), HueException);
    EXPECT_EQ("testreply", handler.PUTString("UrI", "text/html", "body", "192.168.2.1", 90));
}

TEST(BaseHttpHandler, DELETEString)
{
    using namespace ::testing;
    MockBaseHttpHandler handler;

    EXPECT_CALL(handler,
        send("DELETE UrI HTTP/1.0\r\nContent-Type: "
             "text/html\r\nContent-Length: 4\r\n\r\nbody\r\n\r\n",
            "192.168.2.1", 90))
        .Times(AtLeast(2))
        .WillOnce(Return(""))
        .WillRepeatedly(Return("\r\n\r\ntestreply"));

    EXPECT_THROW(handler.DELETEString("UrI", "text/html", "body", "192.168.2.1", 90), HueException);
    EXPECT_EQ("testreply", handler.DELETEString("UrI", "text/html", "body", "192.168.2.1", 90));
}

TEST(BaseHttpHandler, GETJson)
{
    using namespace ::testing;
    MockBaseHttpHandler handler;

    nlohmann::json testval;
    testval["test"] = 100;
    std::string expected_call = "GET UrI HTTP/1.0\r\nContent-Type: application/json\r\nContent-Length: ";
    expected_call.append(std::to_string(testval.dump().size()));
    expected_call.append("\r\n\r\n");
    expected_call.append(testval.dump());
    expected_call.append("\r\n\r\n");

    EXPECT_CALL(handler, send(expected_call, "192.168.2.1", 90))
        .Times(AtLeast(2))
        .WillOnce(Return(""))
        .WillOnce(Return("\r\n\r\n"))
        .WillRepeatedly(Return("\r\n\r\n{\"test\" : \"whatever\"}"));
    nlohmann::json expected;
    expected["test"] = "whatever";

    EXPECT_THROW(handler.GETJson("UrI", testval, "192.168.2.1", 90), HueException);
    EXPECT_THROW(handler.GETJson("UrI", testval, "192.168.2.1", 90), nlohmann::json::parse_error);
    EXPECT_EQ(expected, handler.GETJson("UrI", testval, "192.168.2.1", 90));
}

TEST(BaseHttpHandler, POSTJson)
{
    using namespace ::testing;
    MockBaseHttpHandler handler;

    nlohmann::json testval;
    testval["test"] = 100;
    std::string expected_call = "POST UrI HTTP/1.0\r\nContent-Type: application/json\r\nContent-Length: ";
    expected_call.append(std::to_string(testval.dump().size()));
    expected_call.append("\r\n\r\n");
    expected_call.append(testval.dump());
    expected_call.append("\r\n\r\n");

    EXPECT_CALL(handler, send(expected_call, "192.168.2.1", 90))
        .Times(AtLeast(2))
        .WillOnce(Return(""))
        .WillOnce(Return("\r\n\r\n"))
        .WillRepeatedly(Return("\r\n\r\n{\"test\" : \"whatever\"}"));
    nlohmann::json expected;
    expected["test"] = "whatever";

    EXPECT_THROW(handler.POSTJson("UrI", testval, "192.168.2.1", 90), HueException);
    EXPECT_THROW(handler.POSTJson("UrI", testval, "192.168.2.1", 90), nlohmann::json::parse_error);
    EXPECT_EQ(expected, handler.POSTJson("UrI", testval, "192.168.2.1", 90));
}

TEST(BaseHttpHandler, PUTJson)
{
    using namespace ::testing;
    MockBaseHttpHandler handler;

    nlohmann::json testval;
    testval["test"] = 100;
    std::string expected_call = "PUT UrI HTTP/1.0\r\nContent-Type: application/json\r\nContent-Length: ";
    expected_call.append(std::to_string(testval.dump().size()));
    expected_call.append("\r\n\r\n");
    expected_call.append(testval.dump());
    expected_call.append("\r\n\r\n");

    EXPECT_CALL(handler, send(expected_call, "192.168.2.1", 90))
        .Times(AtLeast(2))
        .WillOnce(Return(""))
        .WillOnce(Return("\r\n\r\n"))
        .WillRepeatedly(Return("\r\n\r\n{\"test\" : \"whatever\"}"));
    nlohmann::json expected;
    expected["test"] = "whatever";

    EXPECT_THROW(handler.PUTJson("UrI", testval, "192.168.2.1", 90), HueException);
    EXPECT_THROW(handler.PUTJson("UrI", testval, "192.168.2.1", 90), nlohmann::json::parse_error);
    EXPECT_EQ(expected, handler.PUTJson("UrI", testval, "192.168.2.1", 90));
}

TEST(BaseHttpHandler, DELETEJson)
{
    using namespace ::testing;
    MockBaseHttpHandler handler;

    nlohmann::json testval;
    testval["test"] = 100;
    std::string expected_call = "DELETE UrI HTTP/1.0\r\nContent-Type: "
                                "application/json\r\nContent-Length: ";
    expected_call.append(std::to_string(testval.dump().size()));
    expected_call.append("\r\n\r\n");
    expected_call.append(testval.dump());
    expected_call.append("\r\n\r\n");

    EXPECT_CALL(handler, send(expected_call, "192.168.2.1", 90))
        .Times(AtLeast(2))
        .WillOnce(Return(""))
        .WillOnce(Return("\r\n\r\n"))
        .WillRepeatedly(Return("\r\n\r\n{\"test\" : \"whatever\"}"));
    nlohmann::json expected;
    expected["test"] = "whatever";

    EXPECT_THROW(handler.DELETEJson("UrI", testval, "192.168.2.1", 90), HueException);
    EXPECT_THROW(handler.DELETEJson("UrI", testval, "192.168.2.1", 90), nlohmann::json::parse_error);
    EXPECT_EQ(expected, handler.DELETEJson("UrI", testval, "192.168.2.1", 90));
}
