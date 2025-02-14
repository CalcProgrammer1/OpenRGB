/**
    \file test_SimpleColorHuewStrategy.cpp
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

#include "TestTransaction.h"
#include "testhelper.h"

#include "hueplusplus/SimpleColorHueStrategy.h"
#include <nlohmann/json.hpp>
#include "mocks/mock_HttpHandler.h"
#include "mocks/mock_Light.h"

using namespace hueplusplus;

TEST(SimpleColorHueStrategy, setColorHue)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler(std::make_shared<MockHttpHandler>());
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername() + "/lights/1", nlohmann::json::object(), getBridgeIp(), 80))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(nlohmann::json::object()));
    MockLight test_light(handler);

    const std::string statePath = "/api/" + getBridgeUsername() + "/lights/1/state";

    nlohmann::json prep_ret;
    prep_ret = nlohmann::json::array();
    prep_ret[0] = nlohmann::json::object();
    prep_ret[0]["success"] = nlohmann::json::object();
    prep_ret[0]["success"]["/lights/1/state/transitiontime"] = 6;
    prep_ret[1] = nlohmann::json::object();
    prep_ret[1]["success"] = nlohmann::json::object();
    prep_ret[1]["success"]["/lights/1/state/on"] = true;
    prep_ret[2] = nlohmann::json::object();
    prep_ret[2]["success"] = nlohmann::json::object();
    prep_ret[2]["success"]["/lights/1/state/hue"] = 30500;
    EXPECT_CALL(*handler, PUTJson(statePath, _, getBridgeIp(), getBridgePort())).Times(1).WillOnce(Return(prep_ret));

    test_light.getState()["state"]["on"] = true;
    test_light.getState()["state"]["hue"] = 200;
    test_light.getState()["state"]["colormode"] = "hs";
    EXPECT_EQ(true, SimpleColorHueStrategy().setColorHue(200, 4, test_light));

    test_light.getState()["state"]["on"] = false;
    EXPECT_EQ(true, SimpleColorHueStrategy().setColorHue(30500, 6, test_light));
}

TEST(SimpleColorHueStrategy, setColorSaturation)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler(std::make_shared<MockHttpHandler>());
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername() + "/lights/1", nlohmann::json::object(), getBridgeIp(), 80))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(nlohmann::json::object()));
    MockLight test_light(handler);

    const std::string statePath = "/api/" + getBridgeUsername() + "/lights/1/state";

    nlohmann::json prep_ret;
    prep_ret = nlohmann::json::array();
    prep_ret[0] = nlohmann::json::object();
    prep_ret[0]["success"] = nlohmann::json::object();
    prep_ret[0]["success"]["/lights/1/state/transitiontime"] = 6;
    prep_ret[1] = nlohmann::json::object();
    prep_ret[1]["success"] = nlohmann::json::object();
    prep_ret[1]["success"]["/lights/1/state/on"] = true;
    prep_ret[2] = nlohmann::json::object();
    prep_ret[2]["success"] = nlohmann::json::object();
    prep_ret[2]["success"]["/lights/1/state/sat"] = 254;
    EXPECT_CALL(*handler, PUTJson(statePath, _, getBridgeIp(), getBridgePort())).Times(1).WillOnce(Return(prep_ret));

    test_light.getState()["state"]["on"] = true;
    test_light.getState()["state"]["sat"] = 100;
    test_light.getState()["state"]["colormode"] = "hs";
    EXPECT_EQ(true, SimpleColorHueStrategy().setColorSaturation(100, 4, test_light));

    test_light.getState()["state"]["on"] = false;
    EXPECT_EQ(true, SimpleColorHueStrategy().setColorSaturation(255, 6, test_light));
}

TEST(SimpleColorHueStrategy, setColorHueSaturation)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler(std::make_shared<MockHttpHandler>());
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername() + "/lights/1", nlohmann::json::object(), getBridgeIp(), 80))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(nlohmann::json::object()));
    MockLight test_light(handler);

    const std::string statePath = "/api/" + getBridgeUsername() + "/lights/1/state";

    nlohmann::json prep_ret;
    prep_ret = nlohmann::json::array();
    prep_ret[0] = nlohmann::json::object();
    prep_ret[0]["success"] = nlohmann::json::object();
    prep_ret[0]["success"]["/lights/1/state/transitiontime"] = 6;
    prep_ret[1] = nlohmann::json::object();
    prep_ret[1]["success"] = nlohmann::json::object();
    prep_ret[1]["success"]["/lights/1/state/on"] = true;
    prep_ret[2] = nlohmann::json::object();
    prep_ret[2]["success"] = nlohmann::json::object();
    prep_ret[2]["success"]["/lights/1/state/hue"] = 30500;
    prep_ret[3] = nlohmann::json::object();
    prep_ret[3]["success"] = nlohmann::json::object();
    prep_ret[3]["success"]["/lights/1/state/sat"] = 254;
    EXPECT_CALL(*handler, PUTJson(statePath, _, getBridgeIp(), getBridgePort())).Times(1).WillOnce(Return(prep_ret));

    test_light.getState()["state"]["on"] = true;
    test_light.getState()["state"]["sat"] = 100;
    test_light.getState()["state"]["hue"] = 200;
    test_light.getState()["state"]["colormode"] = "hs";
    EXPECT_EQ(true, SimpleColorHueStrategy().setColorHueSaturation({200, 100}, 4, test_light));

    test_light.getState()["state"]["on"] = false;
    EXPECT_EQ(true, SimpleColorHueStrategy().setColorHueSaturation({30500, 255}, 6, test_light));
}

TEST(SimpleColorHueStrategy, setColorXY)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler(std::make_shared<MockHttpHandler>());
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername() + "/lights/1", nlohmann::json::object(), getBridgeIp(), 80))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(nlohmann::json::object()));
    MockLight test_light(handler);

    const std::string statePath = "/api/" + getBridgeUsername() + "/lights/1/state";

    nlohmann::json prep_ret
        = {{{"success", {{"/lights/1/state/transitiontime", 6}}}}, {{"success", {{"/lights/1/state/on", true}}}},
            {{"success", {{"/lights/1/state/xy", {0.2355, 0.1234}}}}}, {{"success", {{"/lights/1/state/bri", 254}}}}};
    EXPECT_CALL(*handler, PUTJson(statePath, _, getBridgeIp(), getBridgePort())).Times(1).WillOnce(Return(prep_ret));

    test_light.getState()["state"]["on"] = true;
    test_light.getState()["state"]["xy"][0] = 0.1f;
    test_light.getState()["state"]["xy"][1] = 0.1f;
    test_light.getState()["state"]["bri"] = 254;
    test_light.getState()["state"]["colormode"] = "xy";
    EXPECT_EQ(true, SimpleColorHueStrategy().setColorXY({{0.1f, 0.1f}, 1.f}, 4, test_light));

    test_light.getState()["state"]["on"] = false;
    test_light.getState()["state"]["bri"] = 0;
    EXPECT_EQ(true, SimpleColorHueStrategy().setColorXY({{0.2355f, 0.1234f}, 1.f}, 6, test_light));
}

TEST(SimpleColorHueStrategy, setColorLoop)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler(std::make_shared<MockHttpHandler>());
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername() + "/lights/1", nlohmann::json::object(), getBridgeIp(), 80))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(nlohmann::json::object()));
    MockLight test_light(handler);

    const std::string statePath = "/api/" + getBridgeUsername() + "/lights/1/state";

    nlohmann::json prep_ret;
    prep_ret = nlohmann::json::array();
    prep_ret[0] = nlohmann::json::object();
    prep_ret[0]["success"] = nlohmann::json::object();
    prep_ret[0]["success"]["/lights/1/state/on"] = true;
    prep_ret[1] = nlohmann::json::object();
    prep_ret[1]["success"] = nlohmann::json::object();
    prep_ret[1]["success"]["/lights/1/state/effect"] = "colorloop";
    EXPECT_CALL(*handler, PUTJson(statePath, _, getBridgeIp(), getBridgePort())).Times(1).WillOnce(Return(prep_ret));

    test_light.getState()["state"]["on"] = true;
    test_light.getState()["state"]["effect"] = "colorloop";
    EXPECT_EQ(true, SimpleColorHueStrategy().setColorLoop(true, test_light));

    test_light.getState()["state"]["on"] = false;
    test_light.getState()["state"]["effect"] = "none";
    EXPECT_EQ(true, SimpleColorHueStrategy().setColorLoop(true, test_light));
}

TEST(SimpleColorHueStrategy, alertHueSaturation)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler(std::make_shared<MockHttpHandler>());
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername() + "/lights/1", nlohmann::json::object(), getBridgeIp(), 80))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(nlohmann::json::object()));
    MockLight light(handler);

    // Invalid colormode
    {
        light.getState()["state"]["colormode"] = "invalid";
        light.getState()["state"]["on"] = false;
        EXPECT_EQ(false, SimpleColorHueStrategy().alertHueSaturation({30000, 128}, light));
    }
    const HueSaturation hueSat {200, 100};
    // Needs to update the state so transactions are correctly trimmed
    const auto setColorLambda = [&](const HueSaturation& hueSat, int transition) {
        light.getState()["state"]["colormode"] = "hs";
        light.getState()["state"]["on"] = true;
        light.getState()["state"]["hue"] = hueSat.hue;
        light.getState()["state"]["sat"] = hueSat.saturation;
        return true;
    };
    // Colormode hs
    {
        const nlohmann::json state
            = {{"colormode", "hs"}, {"on", true}, {"xy", {0.1, 0.1}}, {"hue", 300}, {"sat", 100}, {"bri", 254}};
        light.getState()["state"] = state;
        EXPECT_CALL(Const(light), getBrightness()).Times(AnyNumber()).WillRepeatedly(Return(254));
        EXPECT_CALL(Const(light), getColorHueSaturation())
            .Times(AnyNumber())
            .WillRepeatedly(Return(HueSaturation {300, 100}));

        TestTransaction reverseTransaction = light.transaction().setColorHue(300).setTransition(1);
        // Set color fails
        {
            EXPECT_CALL(light, setColorHueSaturation(hueSat, 1)).WillOnce(Return(false));
            EXPECT_FALSE(SimpleColorHueStrategy().alertHueSaturation(hueSat, light));
        }
        // Alert call fails
        {
            InSequence s;
            EXPECT_CALL(light, setColorHueSaturation(hueSat, 1)).WillOnce(Invoke(setColorLambda));
            EXPECT_CALL(light, alert()).WillOnce(Return(false));
            EXPECT_FALSE(SimpleColorHueStrategy().alertHueSaturation(hueSat, light));
        }
        light.getState()["state"] = state;
        // Reverse transaction fails
        {
            InSequence s;
            EXPECT_CALL(light, setColorHueSaturation(hueSat, 1)).WillOnce(Invoke(setColorLambda));
            EXPECT_CALL(light, alert()).WillOnce(Return(true));
            reverseTransaction.expectPut(handler).WillOnce(Return(nlohmann::json::object()));
            EXPECT_FALSE(SimpleColorHueStrategy().alertHueSaturation(hueSat, light));
        }
        light.getState()["state"] = state;
        Mock::VerifyAndClearExpectations(handler.get());
        // Successful
        {
            InSequence s;
            EXPECT_CALL(light, setColorHueSaturation(hueSat, 1)).WillOnce(Invoke(setColorLambda));
            EXPECT_CALL(light, alert()).WillOnce(Return(true));
            reverseTransaction.expectSuccessfulPut(handler, Exactly(1));
            EXPECT_TRUE(SimpleColorHueStrategy().alertHueSaturation(hueSat, light));
        }
        Mock::VerifyAndClearExpectations(handler.get());
    }

    // Colormode hs, off
    {
        const nlohmann::json state
            = {{"colormode", "hs"}, {"on", false}, {"xy", {0.1, 0.1}}, {"hue", 300}, {"sat", 100}, {"bri", 254}};
        light.getState()["state"] = state;
        TestTransaction reverseTransaction = light.transaction().setColorHue(300).setOn(false).setTransition(1);
        InSequence s;
        EXPECT_CALL(light, setColorHueSaturation(hueSat, 1)).WillOnce(Invoke(setColorLambda));
        EXPECT_CALL(light, alert()).WillOnce(Return(true));
        reverseTransaction.expectSuccessfulPut(handler, Exactly(1));
        EXPECT_TRUE(SimpleColorHueStrategy().alertHueSaturation(hueSat, light));
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Colormode xy
    {
        const nlohmann::json state
            = {{"colormode", "xy"}, {"on", true}, {"xy", {0.1, 0.1}}, {"sat", 100}, {"bri", 254}};
        light.getState()["state"] = state;
        EXPECT_CALL(Const(light), getColorXY())
            .Times(AnyNumber())
            .WillRepeatedly(Return(XYBrightness {{0.1f, 0.1f}, 1.f}));
        TestTransaction reverseTransaction = light.transaction().setColor(XY {0.1f, 0.1f}).setTransition(1);

        InSequence s;
        EXPECT_CALL(light, setColorHueSaturation(hueSat, 1)).WillOnce(Invoke(setColorLambda));
        EXPECT_CALL(light, alert()).WillOnce(Return(false));
        EXPECT_FALSE(SimpleColorHueStrategy().alertHueSaturation(hueSat, light));
        light.getState()["state"] = state;
        Mock::VerifyAndClearExpectations(handler.get());

        EXPECT_CALL(light, setColorHueSaturation(hueSat, 1)).WillOnce(Invoke(setColorLambda));
        EXPECT_CALL(light, alert()).WillOnce(Return(true));
        reverseTransaction.expectSuccessfulPut(handler, Exactly(1));
        EXPECT_TRUE(SimpleColorHueStrategy().alertHueSaturation(hueSat, light));
        Mock::VerifyAndClearExpectations(handler.get());
    }

    // Colormode xy, off
    {
        const nlohmann::json state = {{"colormode", "xy"}, {"on", false}, {"xy", {0., 1.}}, {"sat", 100}, {"bri", 254}};
        EXPECT_CALL(Const(light), getColorXY())
            .Times(AnyNumber())
            .WillRepeatedly(Return(XYBrightness {{0.f, 1.f}, 1.f}));
        light.getState()["state"] = state;

        TestTransaction reverseTransaction = light.transaction().setColor(XY {0.f, 1.f}).setOn(false).setTransition(1);
        InSequence s;
        EXPECT_CALL(light, setColorHueSaturation(hueSat, 1)).WillOnce(Invoke(setColorLambda));
        EXPECT_CALL(light, alert()).WillOnce(Return(true));
        reverseTransaction.expectSuccessfulPut(handler, Exactly(1));
        EXPECT_TRUE(SimpleColorHueStrategy().alertHueSaturation(hueSat, light));
        Mock::VerifyAndClearExpectations(handler.get());
    }
}

TEST(SimpleColorHueStrategy, alertXY)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler(std::make_shared<MockHttpHandler>());
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername() + "/lights/1", nlohmann::json::object(), getBridgeIp(), 80))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(nlohmann::json::object()));
    MockLight light(handler);

    // Invalid colormode
    {
        light.getState()["state"]["colormode"] = "invalid";
        light.getState()["state"]["on"] = false;
        EXPECT_EQ(false, SimpleColorHueStrategy().alertXY({{0.1f, 0.1f}, 1.f}, light));
    }
    const XYBrightness xy {{0.1f, 0.1f}, 1.f};
    // Needs to update the state so transactions are correctly trimmed
    const auto setColorLambda = [&](const XYBrightness& xy, int transition) {
        light.getState()["state"]["colormode"] = "xy";
        light.getState()["state"]["on"] = true;
        light.getState()["state"]["xy"] = {xy.xy.x, xy.xy.y};
        light.getState()["state"]["bri"] = static_cast<int>(std::round(xy.brightness * 254.f));
        return true;
    };
    // Colormode hs
    {
        const nlohmann::json state
            = {{"colormode", "hs"}, {"on", true}, {"xy", {0.1, 0.1}}, {"hue", 200}, {"sat", 100}, {"bri", 254}};
        light.getState()["state"] = state;
        EXPECT_CALL(Const(light), getBrightness()).Times(AnyNumber()).WillRepeatedly(Return(254));
        HueSaturation hueSat {200, 100};
        EXPECT_CALL(Const(light), getColorHueSaturation()).Times(AnyNumber()).WillRepeatedly(Return(hueSat));

        TestTransaction reverseTransaction = light.transaction().setColor(hueSat).setTransition(1);
        // Set color fails
        {
            EXPECT_CALL(light, setColorXY(xy, 1)).WillOnce(Return(false));
            EXPECT_FALSE(SimpleColorHueStrategy().alertXY(xy, light));
        }
        // Alert call fails
        {
            InSequence s;
            EXPECT_CALL(light, setColorXY(xy, 1)).WillOnce(Invoke(setColorLambda));
            EXPECT_CALL(light, alert()).WillOnce(Return(false));
            EXPECT_FALSE(SimpleColorHueStrategy().alertXY(xy, light));
        }
        light.getState()["state"] = state;
        // Reverse transaction fails
        {
            InSequence s;
            EXPECT_CALL(light, setColorXY(xy, 1)).WillOnce(Invoke(setColorLambda));
            EXPECT_CALL(light, alert()).WillOnce(Return(true));
            reverseTransaction.expectPut(handler).WillOnce(Return(nlohmann::json::object()));
            EXPECT_FALSE(SimpleColorHueStrategy().alertXY(xy, light));
        }
        light.getState()["state"] = state;
        Mock::VerifyAndClearExpectations(handler.get());
        // Successful
        {
            InSequence s;
            EXPECT_CALL(light, setColorXY(xy, 1)).WillOnce(Invoke(setColorLambda));
            EXPECT_CALL(light, alert()).WillOnce(Return(true));
            reverseTransaction.expectSuccessfulPut(handler, Exactly(1));
            EXPECT_TRUE(SimpleColorHueStrategy().alertXY(xy, light));
        }
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Colormode hs, off
    {
        const nlohmann::json state
            = {{"colormode", "hs"}, {"on", false}, {"xy", {0.1, 0.1}}, {"hue", 200}, {"sat", 100}, {"bri", 254}};
        light.getState()["state"] = state;
        TestTransaction reverseTransaction
            = light.transaction().setColor(HueSaturation {200, 100}).setOn(false).setTransition(1);
        InSequence s;
        EXPECT_CALL(light, setColorXY(xy, 1)).WillOnce(Invoke(setColorLambda));
        EXPECT_CALL(light, alert()).WillOnce(Return(true));
        reverseTransaction.expectSuccessfulPut(handler, Exactly(1));
        EXPECT_TRUE(SimpleColorHueStrategy().alertXY(xy, light));
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // Colormode xy
    {
        const nlohmann::json state
            = {{"colormode", "xy"}, {"on", true}, {"xy", {0.1, 0.1}}, {"sat", 100}, {"bri", 254}};
        light.getState()["state"] = state;
        EXPECT_CALL(Const(light), getColorXY()).Times(AnyNumber()).WillRepeatedly(Return(xy));
        // No reverse transaction sent, because color already matches
        EXPECT_CALL(light, setColorXY(xy, 1)).WillOnce(Invoke(setColorLambda));
        EXPECT_CALL(light, alert()).WillOnce(Return(false));
        EXPECT_FALSE(SimpleColorHueStrategy().alertXY(xy, light));
        light.getState()["state"] = state;
        Mock::VerifyAndClearExpectations(handler.get());

        EXPECT_CALL(light, setColorXY(xy, 1)).WillOnce(Invoke(setColorLambda));
        EXPECT_CALL(light, alert()).WillOnce(Return(true));
        EXPECT_TRUE(SimpleColorHueStrategy().alertXY(xy, light));
        Mock::VerifyAndClearExpectations(handler.get());
    }

    // Colormode xy, off
    {
        const nlohmann::json state = {{"colormode", "xy"}, {"on", false}, {"xy", {0., 1.}}, {"sat", 100}, {"bri", 254}};
        EXPECT_CALL(Const(light), getColorXY())
            .Times(AnyNumber())
            .WillRepeatedly(Return(XYBrightness {{0.f, 1.f}, 1.f}));
        light.getState()["state"] = state;

        // Brightness unchanged, so not requested
        TestTransaction reverseTransaction = light.transaction().setColor(XY {0.f, 1.f}).setOn(false).setTransition(1);
        InSequence s;
        EXPECT_CALL(light, setColorXY(xy, 1)).WillOnce(Invoke(setColorLambda));
        EXPECT_CALL(light, alert()).WillOnce(Return(true));
        reverseTransaction.expectSuccessfulPut(handler, Exactly(1));
        EXPECT_TRUE(SimpleColorHueStrategy().alertXY(xy, light));
        Mock::VerifyAndClearExpectations(handler.get());
    }
}

TEST(SimpleColorHueStrategy, getColorHueSaturation)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler(std::make_shared<MockHttpHandler>());
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername() + "/lights/1", nlohmann::json::object(), getBridgeIp(), 80))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(nlohmann::json::object()));
    MockLight test_light(handler);

    test_light.getState()["state"]["hue"] = 5000;
    test_light.getState()["state"]["sat"] = 128;
    EXPECT_EQ((HueSaturation {5000, 128}), SimpleColorHueStrategy().getColorHueSaturation(test_light));
    test_light.getState()["state"]["hue"] = 50000;
    test_light.getState()["state"]["sat"] = 158;
    EXPECT_EQ((HueSaturation {50000, 158}),
        SimpleColorHueStrategy().getColorHueSaturation(static_cast<const Light>(test_light)));
}

TEST(SimpleColorHueStrategy, getColorXY)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler(std::make_shared<MockHttpHandler>());
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername() + "/lights/1", nlohmann::json::object(), getBridgeIp(), 80))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(nlohmann::json::object()));
    MockLight test_light(handler);

    test_light.getState()["state"]["xy"][0] = 0.1234;
    test_light.getState()["state"]["xy"][1] = 0.1234;
    test_light.getState()["state"]["bri"] = 254;
    EXPECT_EQ((XYBrightness {{0.1234f, 0.1234f}, 1.f}), SimpleColorHueStrategy().getColorXY(test_light));
    test_light.getState()["state"]["xy"][0] = 0.12;
    test_light.getState()["state"]["xy"][1] = 0.6458;
    EXPECT_EQ((XYBrightness {{0.12f, 0.6458f}, 1.f}), SimpleColorHueStrategy().getColorXY(Const(test_light)));
}
