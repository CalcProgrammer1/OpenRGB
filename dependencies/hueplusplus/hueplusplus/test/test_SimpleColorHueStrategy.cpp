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

#include "testhelper.h"

#include "../include/SimpleColorHueStrategy.h"
#include "../include/json/json.hpp"
#include "mocks/mock_HttpHandler.h"
#include "mocks/mock_HueLight.h"

TEST(SimpleColorHueStrategy, setColorHue)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler(std::make_shared<MockHttpHandler>());
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername() + "/lights/1", nlohmann::json::object(), getBridgeIp(), 80))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(nlohmann::json::object()));
    MockHueLight test_light(handler);
    EXPECT_CALL(test_light, refreshState()).Times(AtLeast(1)).WillRepeatedly(Return());
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
    EXPECT_CALL(test_light, SendPutRequest(_, "/state", _)).Times(1).WillOnce(Return(prep_ret));

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
    MockHueLight test_light(handler);
    EXPECT_CALL(test_light, refreshState()).Times(AtLeast(1)).WillRepeatedly(Return());
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
    EXPECT_CALL(test_light, SendPutRequest(_, "/state", _)).Times(1).WillOnce(Return(prep_ret));

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
    MockHueLight test_light(handler);
    EXPECT_CALL(test_light, refreshState()).Times(AtLeast(1)).WillRepeatedly(Return());
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
    EXPECT_CALL(test_light, SendPutRequest(_, "/state", _)).Times(1).WillOnce(Return(prep_ret));

    test_light.getState()["state"]["on"] = true;
    test_light.getState()["state"]["sat"] = 100;
    test_light.getState()["state"]["hue"] = 200;
    test_light.getState()["state"]["colormode"] = "hs";
    EXPECT_EQ(true, SimpleColorHueStrategy().setColorHueSaturation(200, 100, 4, test_light));

    test_light.getState()["state"]["on"] = false;
    EXPECT_EQ(true, SimpleColorHueStrategy().setColorHueSaturation(30500, 255, 6, test_light));
}

TEST(SimpleColorHueStrategy, setColorXY)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler(std::make_shared<MockHttpHandler>());
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername() + "/lights/1", nlohmann::json::object(), getBridgeIp(), 80))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(nlohmann::json::object()));
    MockHueLight test_light(handler);
    EXPECT_CALL(test_light, refreshState()).Times(AtLeast(1)).WillRepeatedly(Return());
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
    prep_ret[2]["success"]["/lights/1/state/xy"][0] = 0.2355;
    prep_ret[2]["success"]["/lights/1/state/xy"][1] = 0.1234;
    EXPECT_CALL(test_light, SendPutRequest(_, "/state", _)).Times(1).WillOnce(Return(prep_ret));

    test_light.getState()["state"]["on"] = true;
    test_light.getState()["state"]["xy"][0] = 0.1f;
    test_light.getState()["state"]["xy"][1] = 0.1f;
    test_light.getState()["state"]["colormode"] = "xy";
    EXPECT_EQ(true, SimpleColorHueStrategy().setColorXY(0.1f, 0.1f, 4, test_light));

    test_light.getState()["state"]["on"] = false;
    EXPECT_EQ(true, SimpleColorHueStrategy().setColorXY(0.2355f, 0.1234f, 6, test_light));
}

TEST(SimpleColorHueStrategy, setColorRGB)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler(std::make_shared<MockHttpHandler>());
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername() + "/lights/1", nlohmann::json::object(), getBridgeIp(), 80))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(nlohmann::json::object()));
    MockHueLight test_light(handler);
    EXPECT_CALL(test_light, setColorXY(_, _, 4)).Times(2).WillRepeatedly(Return(true));

    EXPECT_EQ(true, SimpleColorHueStrategy().setColorRGB(128, 128, 128, 4, test_light));

    EXPECT_EQ(true, SimpleColorHueStrategy().setColorRGB(255, 255, 255, 4, test_light));

    EXPECT_CALL(test_light, OffNoRefresh(4)).Times(1).WillOnce(Return(true));
    EXPECT_EQ(true, SimpleColorHueStrategy().setColorRGB(0, 0, 0, 4, test_light));
}

TEST(SimpleColorHueStrategy, setColorLoop)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler(std::make_shared<MockHttpHandler>());
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername() + "/lights/1", nlohmann::json::object(), getBridgeIp(), 80))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(nlohmann::json::object()));
    MockHueLight test_light(handler);
    EXPECT_CALL(test_light, refreshState()).Times(AtLeast(1)).WillRepeatedly(Return());
    nlohmann::json prep_ret;
    prep_ret = nlohmann::json::array();
    prep_ret[0] = nlohmann::json::object();
    prep_ret[0]["success"] = nlohmann::json::object();
    prep_ret[0]["success"]["/lights/1/state/on"] = true;
    prep_ret[1] = nlohmann::json::object();
    prep_ret[1]["success"] = nlohmann::json::object();
    prep_ret[1]["success"]["/lights/1/state/effect"] = "colorloop";
    EXPECT_CALL(test_light, SendPutRequest(_, "/state", _)).Times(1).WillOnce(Return(prep_ret));

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
    MockHueLight test_light(handler);
    EXPECT_CALL(test_light, refreshState()).Times(AtLeast(1)).WillRepeatedly(Return());

    test_light.getState()["state"]["colormode"] = "invalid";
    test_light.getState()["state"]["on"] = false;
    EXPECT_EQ(false, SimpleColorHueStrategy().alertHueSaturation(30000, 128, test_light));

    EXPECT_CALL(test_light, setColorHueSaturation(_, _, 1))
        .Times(AtLeast(2))
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true));
    test_light.getState()["state"]["colormode"] = "hs";
    test_light.getState()["state"]["on"] = true;
    test_light.getState()["state"]["sat"] = 100;
    test_light.getState()["state"]["hue"] = 200;
    EXPECT_EQ(false, SimpleColorHueStrategy().alertHueSaturation(200, 100, test_light));

    EXPECT_CALL(test_light, alert()).Times(AtLeast(2)).WillOnce(Return(false)).WillRepeatedly(Return(true));
    EXPECT_EQ(false, SimpleColorHueStrategy().alertHueSaturation(200, 100, test_light));

    EXPECT_EQ(true, SimpleColorHueStrategy().alertHueSaturation(200, 100, test_light));

    EXPECT_CALL(test_light, OffNoRefresh(_)).Times(AtLeast(1)).WillRepeatedly(Return(true));
    test_light.getState()["state"]["on"] = false;
    EXPECT_EQ(true, SimpleColorHueStrategy().alertHueSaturation(200, 100, test_light));

    EXPECT_CALL(test_light, setColorHueSaturation(_, _, 1))
        .Times(AtLeast(2))
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true));
    test_light.getState()["state"]["colormode"] = "xy";
    test_light.getState()["state"]["on"] = true;
    test_light.getState()["state"]["xy"][0] = 0.1;
    test_light.getState()["state"]["xy"][1] = 0.1;
    EXPECT_EQ(false, SimpleColorHueStrategy().alertHueSaturation(200, 100, test_light));

    EXPECT_CALL(test_light, alert()).Times(AtLeast(2)).WillOnce(Return(false)).WillRepeatedly(Return(true));
    EXPECT_EQ(false, SimpleColorHueStrategy().alertHueSaturation(200, 100, test_light));

    EXPECT_CALL(test_light, setColorXY(_, _, 1)).Times(AtLeast(2)).WillRepeatedly(Return(true));
    EXPECT_EQ(true, SimpleColorHueStrategy().alertHueSaturation(200, 100, test_light));

    EXPECT_CALL(test_light, OffNoRefresh(_)).Times(AtLeast(1)).WillRepeatedly(Return(true));
    test_light.getState()["state"]["on"] = false;
    EXPECT_EQ(true, SimpleColorHueStrategy().alertHueSaturation(200, 100, test_light));
}

TEST(SimpleColorHueStrategy, alertXY)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler(std::make_shared<MockHttpHandler>());
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername() + "/lights/1", nlohmann::json::object(), getBridgeIp(), 80))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(nlohmann::json::object()));
    MockHueLight test_light(handler);
    EXPECT_CALL(test_light, refreshState()).Times(AtLeast(1)).WillRepeatedly(Return());

    test_light.getState()["state"]["colormode"] = "invalid";
    test_light.getState()["state"]["on"] = false;
    EXPECT_EQ(false, SimpleColorHueStrategy().alertXY(0.1f, 0.1f, test_light));

    EXPECT_CALL(test_light, setColorXY(_, _, 1)).Times(AtLeast(2)).WillOnce(Return(false)).WillRepeatedly(Return(true));
    test_light.getState()["state"]["colormode"] = "hs";
    test_light.getState()["state"]["on"] = true;
    test_light.getState()["state"]["xy"][0] = 0.1;
    test_light.getState()["state"]["xy"][1] = 0.1;
    test_light.getState()["state"]["sat"] = 100;
    test_light.getState()["state"]["hue"] = 200;
    EXPECT_EQ(false, SimpleColorHueStrategy().alertXY(0.1f, 0.1f, test_light));

    EXPECT_CALL(test_light, alert()).Times(AtLeast(2)).WillOnce(Return(false)).WillRepeatedly(Return(true));
    EXPECT_EQ(false, SimpleColorHueStrategy().alertXY(0.1f, 0.1f, test_light));

    EXPECT_CALL(test_light, setColorHueSaturation(_, _, 1)).Times(AtLeast(2)).WillRepeatedly(Return(true));
    EXPECT_EQ(true, SimpleColorHueStrategy().alertXY(0.1f, 0.1f, test_light));

    EXPECT_CALL(test_light, OffNoRefresh(_)).Times(AtLeast(1)).WillRepeatedly(Return(true));
    test_light.getState()["state"]["on"] = false;
    EXPECT_EQ(true, SimpleColorHueStrategy().alertXY(0.1f, 0.1f, test_light));

    EXPECT_CALL(test_light, setColorXY(_, _, 1)).Times(AtLeast(2)).WillOnce(Return(false)).WillRepeatedly(Return(true));
    test_light.getState()["state"]["colormode"] = "xy";
    test_light.getState()["state"]["on"] = true;
    EXPECT_EQ(false, SimpleColorHueStrategy().alertXY(0.1f, 0.1f, test_light));

    EXPECT_CALL(test_light, alert()).Times(AtLeast(2)).WillOnce(Return(false)).WillRepeatedly(Return(true));
    EXPECT_EQ(false, SimpleColorHueStrategy().alertXY(0.1f, 0.1f, test_light));

    EXPECT_EQ(true, SimpleColorHueStrategy().alertXY(0.1f, 0.1f, test_light));

    EXPECT_CALL(test_light, OffNoRefresh(_)).Times(AtLeast(1)).WillRepeatedly(Return(true));
    test_light.getState()["state"]["on"] = false;
    EXPECT_EQ(true, SimpleColorHueStrategy().alertXY(0.1f, 0.1f, test_light));
}

TEST(SimpleColorHueStrategy, alertRGB)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler(std::make_shared<MockHttpHandler>());
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername() + "/lights/1", nlohmann::json::object(), getBridgeIp(), 80))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(nlohmann::json::object()));
    MockHueLight test_light(handler);
    EXPECT_CALL(test_light, refreshState()).Times(AtLeast(1)).WillRepeatedly(Return());

    test_light.getState()["state"]["colormode"] = "invalid";
    test_light.getState()["state"]["on"] = false;
    EXPECT_EQ(false, SimpleColorHueStrategy().alertRGB(128, 128, 128, test_light));

    EXPECT_CALL(test_light, setColorRGB(_, _, _, 1))
        .Times(AtLeast(2))
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true));
    test_light.getState()["state"]["colormode"] = "hs";
    test_light.getState()["state"]["on"] = true;
    test_light.getState()["state"]["sat"] = 100;
    test_light.getState()["state"]["hue"] = 200;
    EXPECT_EQ(false, SimpleColorHueStrategy().alertRGB(128, 128, 128, test_light));

    EXPECT_CALL(test_light, alert()).Times(AtLeast(2)).WillOnce(Return(false)).WillRepeatedly(Return(true));
    EXPECT_EQ(false, SimpleColorHueStrategy().alertRGB(128, 128, 128, test_light));

    EXPECT_CALL(test_light, setColorHueSaturation(_, _, 1)).Times(AtLeast(2)).WillRepeatedly(Return(true));
    EXPECT_EQ(true, SimpleColorHueStrategy().alertRGB(128, 128, 128, test_light));

    EXPECT_CALL(test_light, OffNoRefresh(_)).Times(AtLeast(1)).WillRepeatedly(Return(true));
    test_light.getState()["state"]["on"] = false;
    EXPECT_EQ(true, SimpleColorHueStrategy().alertRGB(128, 128, 128, test_light));

    EXPECT_CALL(test_light, setColorRGB(_, _, _, 1))
        .Times(AtLeast(2))
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true));
    test_light.getState()["state"]["colormode"] = "xy";
    test_light.getState()["state"]["on"] = true;
    test_light.getState()["state"]["xy"][0] = 0.1;
    test_light.getState()["state"]["xy"][1] = 0.1;
    EXPECT_EQ(false, SimpleColorHueStrategy().alertRGB(128, 128, 128, test_light));

    EXPECT_CALL(test_light, alert()).Times(AtLeast(2)).WillOnce(Return(false)).WillRepeatedly(Return(true));
    EXPECT_EQ(false, SimpleColorHueStrategy().alertRGB(128, 128, 128, test_light));

    EXPECT_CALL(test_light, setColorXY(_, _, 1)).Times(AtLeast(2)).WillRepeatedly(Return(true));
    EXPECT_EQ(true, SimpleColorHueStrategy().alertRGB(128, 128, 128, test_light));

    EXPECT_CALL(test_light, OffNoRefresh(_)).Times(AtLeast(1)).WillRepeatedly(Return(true));
    test_light.getState()["state"]["on"] = false;
    EXPECT_EQ(true, SimpleColorHueStrategy().alertRGB(128, 128, 128, test_light));
}

TEST(SimpleColorHueStrategy, getColorHueSaturation)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler(std::make_shared<MockHttpHandler>());
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername() + "/lights/1", nlohmann::json::object(), getBridgeIp(), 80))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(nlohmann::json::object()));
    MockHueLight test_light(handler);
    EXPECT_CALL(test_light, refreshState()).Times(AtLeast(1)).WillRepeatedly(Return());

    test_light.getState()["state"]["hue"] = 5000;
    test_light.getState()["state"]["sat"] = 128;
    EXPECT_EQ(std::make_pair(static_cast<uint16_t>(5000), static_cast<uint8_t>(128)),
        SimpleColorHueStrategy().getColorHueSaturation(test_light));
    test_light.getState()["state"]["hue"] = 50000;
    test_light.getState()["state"]["sat"] = 158;
    EXPECT_EQ(std::make_pair(static_cast<uint16_t>(50000), static_cast<uint8_t>(158)),
        SimpleColorHueStrategy().getColorHueSaturation(static_cast<const HueLight>(test_light)));
}

TEST(SimpleColorHueStrategy, getColorXY)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler(std::make_shared<MockHttpHandler>());
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername() + "/lights/1", nlohmann::json::object(), getBridgeIp(), 80))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(nlohmann::json::object()));
    MockHueLight test_light(handler);
    EXPECT_CALL(test_light, refreshState()).Times(AtLeast(1)).WillRepeatedly(Return());

    test_light.getState()["state"]["xy"][0] = 0.1234;
    test_light.getState()["state"]["xy"][1] = 0.1234;
    EXPECT_EQ(std::make_pair(static_cast<float>(0.1234), static_cast<float>(0.1234)),
        SimpleColorHueStrategy().getColorXY(test_light));
    test_light.getState()["state"]["xy"][0] = 0.12;
    test_light.getState()["state"]["xy"][1] = 0.6458;
    EXPECT_EQ(std::make_pair(static_cast<float>(0.12), static_cast<float>(0.6458)),
        SimpleColorHueStrategy().getColorXY(static_cast<const HueLight>(test_light)));
}
