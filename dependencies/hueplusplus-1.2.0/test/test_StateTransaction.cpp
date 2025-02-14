/**
    \file test_StateTransaction.cpp
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

#include <hueplusplus/StateTransaction.h>

#include <gtest/gtest.h>

#include "testhelper.h"

#include "mocks/mock_HttpHandler.h"

using namespace hueplusplus;
using namespace testing;

TEST(StateTransaction, commit)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);
    const std::string requestPath = "/api/" + getBridgeUsername() + "/path";
    {
        EXPECT_CALL(*handler, PUTJson(_, _, getBridgeIp(), getBridgePort())).Times(0);
        EXPECT_TRUE(StateTransaction(commands, "/path", nullptr).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Explicit off overrides brightness
    {
        nlohmann::json request = {{"on", false}, {"bri", 100}};
        nlohmann::json response = {{{"success", {{"/path/on", false}}}}, {{"success", {{"/path/bri", 100}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", nullptr).setOn(false).setBrightness(100).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Do not trim
    {
        const nlohmann::json request = {{"on", false}, {"bri", 100}};
        nlohmann::json state = {{"on", false}, {"bri", 100}};
        nlohmann::json response = {{{"success", {{"/path/on", false}}}}, {{"success", {{"/path/bri", 100}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", &state).setOn(false).setBrightness(100).commit(false));
        Mock::VerifyAndClearExpectations(handler.get());
    }
}

TEST(StateTransaction, toAction)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);
    const std::string requestPath = "/api/" + getBridgeUsername() + "/path";
    nlohmann::json request = {{"on", false}, {"bri", 100}};

    hueplusplus::Action command
        = StateTransaction(commands, "/path", nullptr).setOn(false).setBrightness(100).toAction();
    Mock::VerifyAndClearExpectations(handler.get());
    EXPECT_EQ(hueplusplus::Action::Method::put, command.getMethod());
    EXPECT_EQ(request, command.getBody());
    EXPECT_EQ(requestPath, command.getAddress());
}

TEST(StateTransaction, setOn)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);
    const std::string requestPath = "/api/" + getBridgeUsername() + "/path";
    // Set on
    {
        nlohmann::json request = {{"on", true}};
        nlohmann::json response = {{{"success", {{"/path/on", true}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", nullptr).setOn(true).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Set off
    {
        nlohmann::json request = {{"on", false}};
        nlohmann::json response = {{{"success", {{"/path/on", false}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", nullptr).setOn(false).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Fail
    {
        nlohmann::json request = {{"on", false}};
        nlohmann::json response = {{{"success", {{"/path/on", true}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_FALSE(StateTransaction(commands, "/path", nullptr).setOn(false).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // No change requested
    {
        nlohmann::json state = {{"on", false}};
        EXPECT_CALL(*handler, PUTJson(_, _, getBridgeIp(), getBridgePort())).Times(0);
        EXPECT_TRUE(StateTransaction(commands, "/path", &state).setOn(false).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
}

TEST(StateTransaction, setBrightness)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);
    const std::string requestPath = "/api/" + getBridgeUsername() + "/path";
    // No state
    {
        const int bri = 128;
        nlohmann::json request = {{"on", true}, {"bri", bri}};
        nlohmann::json response = {{{"success", {{"/path/on", true}}}}, {{"success", {{"/path/bri", bri}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", nullptr).setBrightness(bri).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Clamp to 254
    {
        const int bri = 254;
        nlohmann::json request = {{"on", true}, {"bri", bri}};
        nlohmann::json response = {{{"success", {{"/path/on", true}}}}, {{"success", {{"/path/bri", bri}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", nullptr).setBrightness(255).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Already off
    {
        const int bri = 0;
        nlohmann::json request = {{"bri", bri}};
        nlohmann::json response = {{{"success", {{"/path/bri", bri}}}}};
        nlohmann::json state = {{"on", false}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", &state).setBrightness(bri).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // No change requested
    {
        const int bri = 120;
        nlohmann::json state = {{"on", true}, {"bri", bri}};
        EXPECT_CALL(*handler, PUTJson(_, _, getBridgeIp(), getBridgePort())).Times(0);
        EXPECT_TRUE(StateTransaction(commands, "/path", &state).setBrightness(bri).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Fixed unexpected on/off flickering
    {
        const int hue = 32;
        nlohmann::json state = {{"on", false}, {"bri", 20}};
        nlohmann::json request = { {"hue", hue}, {"bri", 0} };
        nlohmann::json response = {{{"success", {{"/path/hue", hue}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", &state).setBrightness(0).setColorHue(hue).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
}

TEST(StateTransaction, setColorHue)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);
    const std::string requestPath = "/api/" + getBridgeUsername() + "/path";
    // No state
    {
        const int hue = 2159;
        nlohmann::json request = {{"on", true}, {"hue", hue}};
        nlohmann::json response = {{{"success", {{"/path/on", true}}}}, {{"success", {{"/path/hue", hue}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", nullptr).setColorHue(hue).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Already on
    {
        const int hue = 2159;
        nlohmann::json request = {{"hue", hue}};
        nlohmann::json response = {{{"success", {{"/path/hue", hue}}}}};
        nlohmann::json state = {{"on", true}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", &state).setColorHue(hue).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Wrong colormode
    {
        const int hue = 2159;
        nlohmann::json request = {{"hue", hue}};
        nlohmann::json response = {{{"success", {{"/path/hue", hue}}}}};
        nlohmann::json state = {{"on", true}, {"hue", hue}, {"colormode", "ct"}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", &state).setColorHue(hue).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // No request
    {
        const int hue = 2159;
        nlohmann::json state = {{"on", true}, {"hue", hue}, {"colormode", "hs"}};
        EXPECT_CALL(*handler, PUTJson(_, _, getBridgeIp(), getBridgePort())).Times(0);
        EXPECT_TRUE(StateTransaction(commands, "/path", &state).setColorHue(hue).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
}

TEST(StateTransaction, setColorSaturation)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);
    const std::string requestPath = "/api/" + getBridgeUsername() + "/path";
    // No state
    {
        const int sat = 125;
        nlohmann::json request = {{"on", true}, {"sat", sat}};
        nlohmann::json response = {{{"success", {{"/path/on", true}}}}, {{"success", {{"/path/sat", sat}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", nullptr).setColorSaturation(sat).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Clamp to 254
    {
        const int sat = 254;
        nlohmann::json request = {{"on", true}, {"sat", sat}};
        nlohmann::json response = {{{"success", {{"/path/on", true}}}}, {{"success", {{"/path/sat", sat}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", nullptr).setColorSaturation(255).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Already on
    {
        const int sat = 125;
        nlohmann::json request = {{"sat", sat}};
        nlohmann::json response = {{{"success", {{"/path/sat", sat}}}}};
        nlohmann::json state = {{"on", true}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", &state).setColorSaturation(sat).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Wrong colormode
    {
        const int sat = 125;
        nlohmann::json request = {{"sat", sat}};
        nlohmann::json response = {{{"success", {{"/path/sat", sat}}}}};
        nlohmann::json state = {{"on", true}, {"sat", sat}, {"colormode", "ct"}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", &state).setColorSaturation(sat).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // No request
    {
        const int sat = 125;
        nlohmann::json state = {{"on", true}, {"sat", sat}, {"colormode", "hs"}};
        EXPECT_CALL(*handler, PUTJson(_, _, getBridgeIp(), getBridgePort())).Times(0);
        EXPECT_TRUE(StateTransaction(commands, "/path", &state).setColorSaturation(sat).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
}

TEST(StateTransaction, setColorXY)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);
    const std::string requestPath = "/api/" + getBridgeUsername() + "/path";
    // No state
    {
        const float x = 0.5f;
        const float y = 0.8f;
        nlohmann::json request = {{"on", true}, {"xy", {x, y}}};
        nlohmann::json response = {{{"success", {{"/path/on", true}}}}, {{"success", {{"/path/xy", {x, y}}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", nullptr).setColor(XY {x, y}).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Clamp
    {
        const float x = 1.f;
        const float y = 0.f;
        nlohmann::json request = {{"on", true}, {"xy", {x, y}}};
        nlohmann::json response = {{{"success", {{"/path/on", true}}}}, {{"success", {{"/path/xy", {x, y}}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", nullptr).setColor(XY {2.f, -1.f}).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Already on
    {
        const float x = 0.5f;
        const float y = 0.8f;
        nlohmann::json request = {{"xy", {x, y}}};
        nlohmann::json response = {{{"success", {{"/path/xy", {x, y}}}}}};
        nlohmann::json state = {{"on", true}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", &state).setColor(XY {x, y}).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Wrong colormode
    {
        const float x = 0.5f;
        const float y = 0.8f;
        nlohmann::json request = {{"xy", {x, y}}};
        nlohmann::json response = {{{"success", {{"/path/xy", {x, y}}}}}};
        nlohmann::json state = {{"on", true},
            {"xy",
                {
                    x,
                    y,
                }},
            {"colormode", "hs"}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", &state).setColor(XY {x, y}).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // No request
    {
        const float x = 0.5f;
        const float y = 0.8f;
        nlohmann::json state = {{"on", true},
            {"xy",
                {
                    x,
                    y,
                }},
            {"colormode", "xy"}};
        EXPECT_CALL(*handler, PUTJson(_, _, getBridgeIp(), getBridgePort())).Times(0);
        EXPECT_TRUE(StateTransaction(commands, "/path", &state).setColor(XY {x, y}).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
}

TEST(StateTransaction, setColorTemperature)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);
    const std::string requestPath = "/api/" + getBridgeUsername() + "/path";
    // No state
    {
        const int ct = 240;
        nlohmann::json request = {{"on", true}, {"ct", ct}};
        nlohmann::json response = {{{"success", {{"/path/on", true}}}}, {{"success", {{"/path/ct", ct}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", nullptr).setColorTemperature(ct).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Clamp
    {
        const int ct = 500;
        nlohmann::json request = {{"ct", ct}};
        nlohmann::json response = {{{"success", {{"/path/ct", ct}}}}};
        nlohmann::json state = {{"on", true}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", &state).setColorTemperature(520).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Already on
    {
        const int ct = 240;
        nlohmann::json request = {{"ct", ct}};
        nlohmann::json response = {{{"success", {{"/path/ct", ct}}}}};
        nlohmann::json state = {{"on", true}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", &state).setColorTemperature(ct).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Wrong colormode
    {
        const int ct = 240;
        nlohmann::json request = {{"ct", ct}};
        nlohmann::json response = {{{"success", {{"/path/ct", ct}}}}};
        nlohmann::json state = {{"on", true}, {"ct", ct}, {"colormode", "hs"}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", &state).setColorTemperature(ct).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // No request
    {
        const int ct = 240;
        nlohmann::json state = {{"on", true}, {"ct", ct}, {"colormode", "ct"}};
        EXPECT_CALL(*handler, PUTJson(_, _, getBridgeIp(), getBridgePort())).Times(0);
        EXPECT_TRUE(StateTransaction(commands, "/path", &state).setColorTemperature(ct).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
}

TEST(StateTransaction, setColorLoop)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);
    const std::string requestPath = "/api/" + getBridgeUsername() + "/path";
    // Set on
    {
        nlohmann::json request = {{"on", true}, {"effect", "colorloop"}};
        nlohmann::json response = {{{"success", {{"/path/on", true}}}}, {{"success", {{"/path/effect", "colorloop"}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", nullptr).setColorLoop(true).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Set off
    {
        nlohmann::json request = {{"on", true}, {"effect", "none"}};
        nlohmann::json response = {{{"success", {{"/path/on", true}}}}, {{"success", {{"/path/effect", "none"}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", nullptr).setColorLoop(false).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // No request
    {
        nlohmann::json state = {{"on", true}, {"effect", "colorloop"}};
        EXPECT_CALL(*handler, PUTJson(_, _, getBridgeIp(), getBridgePort())).Times(0);
        EXPECT_TRUE(StateTransaction(commands, "/path", &state).setColorLoop(true).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
}

TEST(StateTransaction, incrementBrightness)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);
    const std::string requestPath = "/api/" + getBridgeUsername() + "/path";
    {
        const int inc = 20;
        nlohmann::json request = {{"bri_inc", inc}};
        nlohmann::json response = {{{"success", {{"/path/bri_inc", inc}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", nullptr).incrementBrightness(inc).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Clamp
    {
        const int inc = -254;
        nlohmann::json request = {{"bri_inc", inc}};
        nlohmann::json response = {{{"success", {{"/path/bri_inc", inc}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", nullptr).incrementBrightness(-300).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
}

TEST(StateTransaction, incrementSaturation)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);
    const std::string requestPath = "/api/" + getBridgeUsername() + "/path";
    {
        const int inc = 20;
        nlohmann::json request = {{"sat_inc", inc}};
        nlohmann::json response = {{{"success", {{"/path/sat_inc", inc}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", nullptr).incrementSaturation(inc).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Clamp
    {
        const int inc = -254;
        nlohmann::json request = {{"sat_inc", inc}};
        nlohmann::json response = {{{"success", {{"/path/sat_inc", inc}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", nullptr).incrementSaturation(-300).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
}

TEST(StateTransaction, incrementHue)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);
    const std::string requestPath = "/api/" + getBridgeUsername() + "/path";
    {
        const int inc = 20;
        nlohmann::json request = {{"hue_inc", inc}};
        nlohmann::json response = {{{"success", {{"/path/hue_inc", inc}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", nullptr).incrementHue(inc).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Clamp
    {
        const int inc = -65534;
        nlohmann::json request = {{"hue_inc", inc}};
        nlohmann::json response = {{{"success", {{"/path/hue_inc", inc}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", nullptr).incrementHue(-300000).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
}

TEST(StateTransaction, incrementColorTemperature)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);
    const std::string requestPath = "/api/" + getBridgeUsername() + "/path";
    {
        const int inc = 20;
        nlohmann::json request = {{"ct_inc", inc}};
        nlohmann::json response = {{{"success", {{"/path/ct_inc", inc}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", nullptr).incrementColorTemperature(inc).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Clamp
    {
        const int inc = -65534;
        nlohmann::json request = {{"ct_inc", inc}};
        nlohmann::json response = {{{"success", {{"/path/ct_inc", inc}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", nullptr).incrementColorTemperature(-300000).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
}

TEST(StateTransaction, incrementColorXY)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);
    const std::string requestPath = "/api/" + getBridgeUsername() + "/path";
    {
        const float incX = 0.2f;
        const float incY = -0.4f;
        nlohmann::json request = {{"xy_inc", {incX, incY}}};
        nlohmann::json response = {{{"success", {{"/path/xy_inc", {incX, incY}}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", nullptr).incrementColorXY(incX, incY).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Clamp
    {
        const float incX = 0.5f;
        const float incY = -0.5f;
        nlohmann::json request = {{"xy_inc", {incX, incY}}};
        nlohmann::json response = {{{"success", {{"/path/xy_inc", {incX, incY}}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", nullptr).incrementColorXY(1.f, -1.f).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
}

TEST(StateTransaction, setTransition)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);
    const std::string requestPath = "/api/" + getBridgeUsername() + "/path";
    {
        nlohmann::json request = {{"on", true}, {"transitiontime", 2}};
        nlohmann::json response = {{{"success", {{"/path/on", true}}}}, {{"success", {{"/path/transitiontime", 2}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", nullptr).setOn(true).setTransition(2).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // No transition time 4
    {
        nlohmann::json request = {{"on", true}};
        nlohmann::json response = {{{"success", {{"/path/on", true}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", nullptr).setOn(true).setTransition(4).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // No request with only transition
    {
        EXPECT_CALL(*handler, PUTJson(_, _, getBridgeIp(), getBridgePort())).Times(0);
        EXPECT_TRUE(StateTransaction(commands, "/path", nullptr).setTransition(2).commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
}

TEST(StateTransaction, alert)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);
    const std::string requestPath = "/api/" + getBridgeUsername() + "/path";
    {
        nlohmann::json request = {{"alert", "select"}};
        nlohmann::json response = {{{"success", {{"/path/alert", "select"}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", nullptr).alert().commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Also alert when in state
    {
        nlohmann::json request = {{"alert", "select"}};
        nlohmann::json response = {{{"success", {{"/path/alert", "select"}}}}};
        nlohmann::json state = {{"alert", "select"}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", &state).alert().commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
}

TEST(StateTransaction, longAlert)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);
    const std::string requestPath = "/api/" + getBridgeUsername() + "/path";
    {
        nlohmann::json request = {{"alert", "lselect"}};
        nlohmann::json response = {{{"success", {{"/path/alert", "lselect"}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", nullptr).longAlert().commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Also alert when in state
    {
        nlohmann::json request = {{"alert", "lselect"}};
        nlohmann::json response = {{{"success", {{"/path/alert", "lselect"}}}}};
        nlohmann::json state = {{"alert", "lselect"}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", &state).longAlert().commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
}

TEST(StateTransaction, stopAlert)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);
    const std::string requestPath = "/api/" + getBridgeUsername() + "/path";
    {
        nlohmann::json request = {{"alert", "none"}};
        nlohmann::json response = {{{"success", {{"/path/alert", "none"}}}}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", nullptr).stopAlert().commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Also alert when in state
    {
        nlohmann::json request = {{"alert", "none"}};
        nlohmann::json response = {{{"success", {{"/path/alert", "none"}}}}};
        nlohmann::json state = {{"alert", "none"}};
        EXPECT_CALL(*handler, PUTJson(requestPath, request, getBridgeIp(), getBridgePort())).WillOnce(Return(response));
        EXPECT_TRUE(StateTransaction(commands, "/path", &state).stopAlert().commit());
        Mock::VerifyAndClearExpectations(handler.get());
    }
}
