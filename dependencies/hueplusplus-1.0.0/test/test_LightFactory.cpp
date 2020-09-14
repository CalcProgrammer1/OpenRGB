/**
    \file test_HueLightFactory.cpp
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

#include <hueplusplus/HueDeviceTypes.h>

#include <gtest/gtest.h>

#include "testhelper.h"

#include "mocks/mock_HttpHandler.h"

using namespace hueplusplus;

TEST(LightFactory, createLight_noGamut)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler = std::make_shared<MockHttpHandler>();

    LightFactory factory(HueCommandAPI(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler),
        std::chrono::steady_clock::duration::max());

    nlohmann::json lightState
        = {{"state",
               {{"on", true}, {"bri", 254}, {"ct", 366}, {"alert", "none"}, {"colormode", "ct"}, {"reachable", true}}},
            {"swupdate", {{"state", "noupdates"}, {"lastinstall", nullptr}}}, {"type", "Color temperature light"},
            {"name", "Hue ambiance lamp 1"}, {"modelid", "LTW001"}, {"manufacturername", "Philips"},
            {"uniqueid", "00:00:00:00:00:00:00:00-00"}, {"swversion", "5.50.1.19085"}};

    Light test_light_1 = factory.createLight(lightState, 1);
    EXPECT_EQ(test_light_1.getColorType(), ColorType::TEMPERATURE);

    lightState["type"] = "Dimmable light";

    test_light_1 = factory.createLight(lightState, 1);
    EXPECT_EQ(test_light_1.getColorType(), ColorType::NONE);

    lightState["type"] = "On/Off light";

    test_light_1 = factory.createLight(lightState, 1);
    EXPECT_EQ(test_light_1.getColorType(), ColorType::NONE);

    lightState["type"] = "unknown light type";
    ASSERT_THROW(factory.createLight(lightState, 1), HueException);
}

TEST(LightFactory, createLight_gamutCapabilities)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler = std::make_shared<MockHttpHandler>();

    LightFactory factory(HueCommandAPI(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler),
        std::chrono::steady_clock::duration::max());

    nlohmann::json lightState
        = {{"state",
               {{"on", true}, {"bri", 254}, {"ct", 366}, {"alert", "none"}, {"colormode", "ct"}, {"reachable", true}}},
            {"swupdate", {{"state", "noupdates"}, {"lastinstall", nullptr}}}, {"type", "Color light"},
            {"name", "Hue ambiance lamp 1"}, {"modelid", "LTW001"}, {"manufacturername", "Philips"},
            {"uniqueid", "00:00:00:00:00:00:00:00-00"}, {"swversion", "5.50.1.19085"},
            {"capabilities", {{"control", {{"colorgamuttype", "A"}}}}}};

    Light test_light_1 = factory.createLight(lightState, 1);
    EXPECT_EQ(test_light_1.getColorType(), ColorType::GAMUT_A);

    lightState["capabilities"]["control"]["colorgamuttype"] = "B";

    test_light_1 = factory.createLight(lightState, 1);
    EXPECT_EQ(test_light_1.getColorType(), ColorType::GAMUT_B);

    lightState["capabilities"]["control"]["colorgamuttype"] = "C";

    test_light_1 = factory.createLight(lightState, 1);
    EXPECT_EQ(test_light_1.getColorType(), ColorType::GAMUT_C);

    lightState["capabilities"]["control"]["colorgamuttype"] = "Other";

    test_light_1 = factory.createLight(lightState, 1);
    EXPECT_EQ(test_light_1.getColorType(), ColorType::GAMUT_OTHER);

    // With color temperature
    lightState["type"] = "Extended color light";
    lightState["capabilities"]["control"]["colorgamuttype"] = "A";

    test_light_1 = factory.createLight(lightState, 1);
    EXPECT_EQ(test_light_1.getColorType(), ColorType::GAMUT_A_TEMPERATURE);

    lightState["capabilities"]["control"]["colorgamuttype"] = "B";

    test_light_1 = factory.createLight(lightState, 1);
    EXPECT_EQ(test_light_1.getColorType(), ColorType::GAMUT_B_TEMPERATURE);

    lightState["capabilities"]["control"]["colorgamuttype"] = "C";

    test_light_1 = factory.createLight(lightState, 1);
    EXPECT_EQ(test_light_1.getColorType(), ColorType::GAMUT_C_TEMPERATURE);

    lightState["capabilities"]["control"]["colorgamuttype"] = "Other";

    test_light_1 = factory.createLight(lightState, 1);
    EXPECT_EQ(test_light_1.getColorType(), ColorType::GAMUT_OTHER_TEMPERATURE);
}

TEST(LightFactory, createLight_gamutModelid)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler = std::make_shared<MockHttpHandler>();

    LightFactory factory(HueCommandAPI(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler),
        std::chrono::steady_clock::duration::max());

    const std::string gamutAModel = "LST001";
    const std::string gamutBModel = "LCT001";
    const std::string gamutCModel = "LCT010";

    nlohmann::json lightState
        = {{"state",
               {{"on", true}, {"bri", 254}, {"ct", 366}, {"alert", "none"}, {"colormode", "ct"}, {"reachable", true}}},
            {"swupdate", {{"state", "noupdates"}, {"lastinstall", nullptr}}}, {"type", "Color light"},
            {"name", "Hue ambiance lamp 1"}, {"modelid", gamutAModel}, {"manufacturername", "Philips"},
            {"uniqueid", "00:00:00:00:00:00:00:00-00"}, {"swversion", "5.50.1.19085"}};

    Light test_light_1 = factory.createLight(lightState, 1);
    EXPECT_EQ(test_light_1.getColorType(), ColorType::GAMUT_A);

    lightState["modelid"] = gamutBModel;
    test_light_1 = factory.createLight(lightState, 1);
    EXPECT_EQ(test_light_1.getColorType(), ColorType::GAMUT_B);

    lightState["modelid"] = gamutCModel;

    test_light_1 = factory.createLight(lightState, 1);
    EXPECT_EQ(test_light_1.getColorType(), ColorType::GAMUT_C);

    // With color temperature
    lightState["type"] = "Extended color light";
    lightState["modelid"] = gamutAModel;

    test_light_1 = factory.createLight(lightState, 1);
    EXPECT_EQ(test_light_1.getColorType(), ColorType::GAMUT_A_TEMPERATURE);

    lightState["modelid"] = gamutBModel;

    test_light_1 = factory.createLight(lightState, 1);
    EXPECT_EQ(test_light_1.getColorType(), ColorType::GAMUT_B_TEMPERATURE);

    lightState["modelid"] = gamutCModel;

    test_light_1 = factory.createLight(lightState, 1);
    EXPECT_EQ(test_light_1.getColorType(), ColorType::GAMUT_C_TEMPERATURE);

    // Unknown model
    lightState["modelid"] = "Unknown model";
    test_light_1 = factory.createLight(lightState, 1);
    EXPECT_EQ(test_light_1.getColorType(), ColorType::UNDEFINED);
}
