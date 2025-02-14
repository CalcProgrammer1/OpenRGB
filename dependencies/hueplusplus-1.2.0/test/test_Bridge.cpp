/**
    \file test_Bridge.cpp
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

#include "hueplusplus/Bridge.h"
#include "hueplusplus/LibConfig.h"
#include <nlohmann/json.hpp>
#include "mocks/mock_HttpHandler.h"

using namespace hueplusplus;

class BridgeFinderTest : public ::testing::Test
{
protected:
    std::shared_ptr<MockHttpHandler> handler;

protected:
    BridgeFinderTest() : handler(std::make_shared<MockHttpHandler>())
    {
        using namespace ::testing;

        EXPECT_CALL(*handler,
            sendMulticast("M-SEARCH * HTTP/1.1\r\nHOST: 239.255.255.250:1900\r\nMAN: "
                          "\"ssdp:discover\"\r\nMX: 5\r\nST: ssdp:all\r\n\r\n",
                "239.255.255.250", 1900, Config::instance().getUPnPTimeout()))
            .Times(AtLeast(1))
            .WillRepeatedly(Return(getMulticastReply()));

        EXPECT_CALL(*handler, GETString("/description.xml", "application/xml", "", "192.168.2.1", getBridgePort()))
            .Times(0);

        EXPECT_CALL(*handler, GETString("/description.xml", "application/xml", "", getBridgeIp(), getBridgePort()))
            .Times(AtLeast(1))
            .WillRepeatedly(Return(getBridgeXml()));
    }
    ~BridgeFinderTest() {};
};

TEST_F(BridgeFinderTest, findBridges)
{
    BridgeFinder finder(handler);
    std::vector<BridgeFinder::BridgeIdentification> bridges = finder.findBridges();

    BridgeFinder::BridgeIdentification bridge_to_comp;
    bridge_to_comp.ip = getBridgeIp();
    bridge_to_comp.port = getBridgePort();
    bridge_to_comp.mac = getBridgeMac();

    EXPECT_EQ(bridges.size(), 1) << "BridgeFinder found more than one Bridge";
    EXPECT_EQ(bridges[0].ip, bridge_to_comp.ip) << "BridgeIdentification ip does not match";
    EXPECT_EQ(bridges[0].port, bridge_to_comp.port) << "BridgeIdentification port does not match";
    EXPECT_EQ(bridges[0].mac, bridge_to_comp.mac) << "BridgeIdentification mac does not match";

    // Test invalid description
    EXPECT_CALL(*handler, GETString("/description.xml", "application/xml", "", getBridgeIp(), getBridgePort()))
        .Times(1)
        .WillOnce(::testing::Return("invalid stuff"));
    bridges = finder.findBridges();
    EXPECT_TRUE(bridges.empty());
}

TEST_F(BridgeFinderTest, getBridge)
{
    using namespace ::testing;
    nlohmann::json request {{"devicetype", "HuePlusPlus#User"}, {"generateclientkey", true}};

    nlohmann::json errorResponse
        = {{{"error", {{"type", 101}, {"address", ""}, {"description", "link button not pressed"}}}}};

    EXPECT_CALL(*handler, POSTJson("/api", request, getBridgeIp(), getBridgePort()))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(errorResponse));

    BridgeFinder finder(handler);
    std::vector<BridgeFinder::BridgeIdentification> bridges = finder.findBridges();

    ASSERT_THROW(finder.getBridge(bridges[0]), HueException);

    nlohmann::json successResponse = {{{"success", {{"username", getBridgeUsername()}}}}};

    EXPECT_CALL(*handler, POSTJson("/api", request, getBridgeIp(), getBridgePort()))
        .Times(1)
        .WillOnce(Return(successResponse));

    finder = BridgeFinder(handler);
    bridges = finder.findBridges();

    Bridge test_bridge = finder.getBridge(bridges[0]);

    EXPECT_EQ(test_bridge.getBridgeIP(), getBridgeIp()) << "Bridge IP not matching";
    EXPECT_EQ(test_bridge.getBridgePort(), getBridgePort()) << "Bridge Port not matching";
    EXPECT_EQ(test_bridge.getUsername(), getBridgeUsername()) << "Bridge username not matching";

    Mock::VerifyAndClearExpectations(handler.get());
}

TEST_F(BridgeFinderTest, addUsername)
{
    BridgeFinder finder(handler);
    std::vector<BridgeFinder::BridgeIdentification> bridges = finder.findBridges();

    finder.addUsername(bridges[0].mac, getBridgeUsername());
    Bridge test_bridge = finder.getBridge(bridges[0]);

    EXPECT_EQ(test_bridge.getBridgeIP(), getBridgeIp()) << "Bridge IP not matching";
    EXPECT_EQ(test_bridge.getBridgePort(), getBridgePort()) << "Bridge Port not matching";
    EXPECT_EQ(test_bridge.getUsername(), getBridgeUsername()) << "Bridge username not matching";
}

TEST_F(BridgeFinderTest, getAllUsernames)
{
    BridgeFinder finder(handler);
    std::vector<BridgeFinder::BridgeIdentification> bridges = finder.findBridges();

    finder.addUsername(bridges[0].mac, getBridgeUsername());

    std::map<std::string, std::string> users = finder.getAllUsernames();
    EXPECT_EQ(users[getBridgeMac()], getBridgeUsername()) << "Username of MAC:" << getBridgeMac() << "not matching";
}

TEST(Bridge, Constructor)
{
    std::shared_ptr<MockHttpHandler> handler = std::make_shared<MockHttpHandler>();
    Bridge test_bridge(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);

    EXPECT_EQ(test_bridge.getBridgeIP(), getBridgeIp()) << "Bridge IP not matching";
    EXPECT_EQ(test_bridge.getBridgePort(), getBridgePort()) << "Bridge Port not matching";
    EXPECT_EQ(test_bridge.getUsername(), getBridgeUsername()) << "Bridge username not matching";
}

TEST(Bridge, requestUsername)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler = std::make_shared<MockHttpHandler>();
    nlohmann::json request {{"devicetype", "HuePlusPlus#User"}, {"generateclientkey", true}};

    {
        nlohmann::json errorResponse
            = {{{"error", {{"type", 101}, {"address", ""}, {"description", "link button not pressed"}}}}};

        EXPECT_CALL(*handler, POSTJson("/api", request, getBridgeIp(), getBridgePort()))
            .Times(AtLeast(1))
            .WillRepeatedly(Return(errorResponse));

        Bridge test_bridge(getBridgeIp(), getBridgePort(), "", handler);

        std::string username = test_bridge.requestUsername();
        EXPECT_EQ(username, "") << "Returned username not matching";
        EXPECT_EQ(test_bridge.getUsername(), "") << "Bridge username not matching";
    }

    {
        // Other error code causes exception
        int otherError = 1;
        nlohmann::json exceptionResponse
            = {{{"error", {{"type", otherError}, {"address", ""}, {"description", "some error"}}}}};
        Bridge testBridge(getBridgeIp(), getBridgePort(), "", handler);

        EXPECT_CALL(*handler, POSTJson("/api", request, getBridgeIp(), getBridgePort()))
            .WillOnce(Return(exceptionResponse));

        try
        {
            testBridge.requestUsername();
            FAIL() << "requestUsername did not throw";
        }
        catch (const HueAPIResponseException& e)
        {
            EXPECT_EQ(e.GetErrorNumber(), otherError);
        }
        catch (const std::exception& e)
        {
            FAIL() << "wrong exception: " << e.what();
        }
    }

    {
        nlohmann::json successResponse = {{{"success", {{"username", getBridgeUsername()}}}}};
        EXPECT_CALL(*handler, POSTJson("/api", request, getBridgeIp(), getBridgePort()))
            .Times(1)
            .WillRepeatedly(Return(successResponse));

        Bridge test_bridge(getBridgeIp(), getBridgePort(), "", handler);

        nlohmann::json hue_bridge_state {{"lights", {}}};
        EXPECT_CALL(
            *handler, GETJson("/api/" + getBridgeUsername(), nlohmann::json::object(), getBridgeIp(), getBridgePort()))
            .Times(1)
            .WillOnce(Return(hue_bridge_state));
        std::string username = test_bridge.requestUsername();

        EXPECT_EQ(username, test_bridge.getUsername()) << "Returned username not matching";
        EXPECT_EQ(test_bridge.getBridgeIP(), getBridgeIp()) << "Bridge IP not matching";
        EXPECT_EQ(test_bridge.getUsername(), getBridgeUsername()) << "Bridge username not matching";
    }
}

TEST(Bridge, setIP)
{
    std::shared_ptr<MockHttpHandler> handler = std::make_shared<MockHttpHandler>();
    Bridge test_bridge(getBridgeIp(), getBridgePort(), "", handler);
    EXPECT_EQ(test_bridge.getBridgeIP(), getBridgeIp()) << "Bridge IP not matching after initialization";
    test_bridge.setIP("192.168.2.112");
    EXPECT_EQ(test_bridge.getBridgeIP(), "192.168.2.112") << "Bridge IP not matching after setting it";
}

TEST(Bridge, setPort)
{
    std::shared_ptr<MockHttpHandler> handler = std::make_shared<MockHttpHandler>();
    Bridge test_bridge = Bridge(getBridgeIp(), getBridgePort(), "", handler);
    EXPECT_EQ(test_bridge.getBridgePort(), getBridgePort()) << "Bridge Port not matching after initialization";
    test_bridge.setPort(81);
    EXPECT_EQ(test_bridge.getBridgePort(), 81) << "Bridge Port not matching after setting it";
}

TEST(Bridge, getLight)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler = std::make_shared<MockHttpHandler>();
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername(), nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .Times(1);

    Bridge test_bridge(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);

    // Test exception
    ASSERT_THROW(test_bridge.lights().get(1), HueException);

    nlohmann::json hue_bridge_state {{"lights",
        {{"1",
            {{"state",
                 {{"on", true}, {"bri", 254}, {"ct", 366}, {"alert", "none"}, {"colormode", "ct"},
                     {"reachable", true}}},
                {"swupdate", {{"state", "noupdates"}, {"lastinstall", nullptr}}}, {"type", "Color temperature light"},
                {"name", "Hue ambiance lamp 1"}, {"modelid", "LTW001"}, {"manufacturername", "Philips"},
                {"uniqueid", "00:00:00:00:00:00:00:00-00"}, {"swversion", "5.50.1.19085"}}}}}};

    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername(), nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .Times(1)
        .WillOnce(Return(hue_bridge_state));

    // Refresh cache
    test_bridge = Bridge(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);

    // Test when correct data is sent
    Light test_light_1 = test_bridge.lights().get(1);
    EXPECT_EQ(test_light_1.getName(), "Hue ambiance lamp 1");
    EXPECT_EQ(test_light_1.getColorType(), ColorType::TEMPERATURE);

    // Test again to check whether light is returned directly -> interesting for
    // code coverage test
    test_light_1 = test_bridge.lights().get(1);
    EXPECT_EQ(test_light_1.getName(), "Hue ambiance lamp 1");
    EXPECT_EQ(test_light_1.getColorType(), ColorType::TEMPERATURE);
}

TEST(Bridge, SharedState)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler = std::make_shared<MockHttpHandler>();

    nlohmann::json hue_bridge_state {{"lights",
        {{"1",
            {{"state",
                 {{"on", true}, {"bri", 254}, {"ct", 366}, {"alert", "none"}, {"colormode", "ct"},
                     {"reachable", true}}},
                {"swupdate", {{"state", "noupdates"}, {"lastinstall", nullptr}}}, {"type", "Color temperature light"},
                {"name", "Hue ambiance lamp 1"}, {"modelid", "LTW001"}, {"manufacturername", "Philips"},
                {"uniqueid", "00:00:00:00:00:00:00:00-00"}, {"swversion", "5.50.1.19085"}}}}}};

    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername(), nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .Times(1)
        .WillOnce(Return(hue_bridge_state));
    Bridge test_bridge(
        getBridgeIp(), getBridgePort(), getBridgeUsername(), handler, "", std::chrono::seconds(10), true);

    // Test when correct data is sent
    Light test_light_1 = test_bridge.lights().get(1);

    Light test_light_copy = test_bridge.lights().get(1);
    const std::string newName = "New light name";
    EXPECT_CALL(*handler, PUTJson("/api/" + getBridgeUsername() + "/lights/1/name", _, getBridgeIp(), getBridgePort()))
        .WillOnce(Return(nlohmann::json({{"success", {{{"/lights/1/name", newName}}}}})));
    test_light_1.setName(newName);
    hue_bridge_state["lights"]["1"]["name"] = newName;

    EXPECT_CALL(*handler,
        GETJson("/api/" + getBridgeUsername() + "/lights/1", nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .Times(1)
        .WillOnce(Return(hue_bridge_state["lights"]["1"]));
    test_light_1.refresh(true);

    EXPECT_EQ(newName, test_light_copy.getName());
}

TEST(Bridge, removeLight)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler = std::make_shared<MockHttpHandler>();
    nlohmann::json hue_bridge_state {{"lights",
        {{"1",
            {{"state",
                 {{"on", true}, {"bri", 254}, {"ct", 366}, {"alert", "none"}, {"colormode", "ct"},
                     {"reachable", true}}},
                {"swupdate", {{"state", "noupdates"}, {"lastinstall", nullptr}}}, {"type", "Color temperature light"},
                {"name", "Hue ambiance lamp 1"}, {"modelid", "LTW001"}, {"manufacturername", "Philips"},
                {"uniqueid", "00:00:00:00:00:00:00:00-00"}, {"swversion", "5.50.1.19085"}}}}}};
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername(), nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .Times(1)
        .WillOnce(Return(hue_bridge_state));

    Bridge test_bridge(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);

    nlohmann::json return_answer;
    return_answer = nlohmann::json::array();
    return_answer[0] = nlohmann::json::object();
    return_answer[0]["success"] = "/lights/1 deleted";
    EXPECT_CALL(*handler,
        DELETEJson(
            "/api/" + getBridgeUsername() + "/lights/1", nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .Times(2)
        .WillOnce(Return(return_answer))
        .WillOnce(Return(nlohmann::json()));

    // Test when correct data is sent
    Light test_light_1 = test_bridge.lights().get(1);

    EXPECT_EQ(test_bridge.lights().remove(1), true);

    EXPECT_EQ(test_bridge.lights().remove(1), false);
}

TEST(Bridge, getAllLights)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler = std::make_shared<MockHttpHandler>();
    nlohmann::json hue_bridge_state {{"lights",
        {{"1",
            {{"state",
                 {{"on", true}, {"bri", 254}, {"ct", 366}, {"alert", "none"}, {"colormode", "ct"},
                     {"reachable", true}}},
                {"swupdate", {{"state", "noupdates"}, {"lastinstall", nullptr}}}, {"type", "Color temperature light"},
                {"name", "Hue ambiance lamp 1"}, {"modelid", "LTW001"}, {"manufacturername", "Philips"},
                {"uniqueid", "00:00:00:00:00:00:00:00-00"}, {"swversion", "5.50.1.19085"}}}}}};

    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername(), nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(hue_bridge_state));

    Bridge test_bridge(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);

    std::vector<Light> test_lights = test_bridge.lights().getAll();
    ASSERT_EQ(1, test_lights.size());
    EXPECT_EQ(test_lights[0].getName(), "Hue ambiance lamp 1");
    EXPECT_EQ(test_lights[0].getColorType(), ColorType::TEMPERATURE);
}

TEST(Bridge, lightExists)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler = std::make_shared<MockHttpHandler>();
    nlohmann::json hue_bridge_state {{"lights",
        {{"1",
            {{"state",
                 {{"on", true}, {"bri", 254}, {"ct", 366}, {"alert", "none"}, {"colormode", "ct"},
                     {"reachable", true}}},
                {"swupdate", {{"state", "noupdates"}, {"lastinstall", nullptr}}}, {"type", "Color temperature light"},
                {"name", "Hue ambiance lamp 1"}, {"modelid", "LTW001"}, {"manufacturername", "Philips"},
                {"uniqueid", "00:00:00:00:00:00:00:00-00"}, {"swversion", "5.50.1.19085"}}}}}};
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername(), nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(hue_bridge_state));

    Bridge test_bridge(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);

    test_bridge.refresh();

    EXPECT_TRUE(Const(test_bridge).lights().exists(1));
    EXPECT_FALSE(Const(test_bridge).lights().exists(2));
}

TEST(Bridge, getGroup)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler = std::make_shared<MockHttpHandler>();
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername(), nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .Times(1);

    Bridge test_bridge(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);

    // Test exception
    ASSERT_THROW(test_bridge.groups().get(1), HueException);

    nlohmann::json hue_bridge_state {{"groups",
        {{"1",
            {{"name", "Group 1"}, {"type", "LightGroup"}, {"lights", {"1", "2", "3"}},
                {"action",
                    {{"on", true}, {"bri", 254}, {"ct", 366}, {"alert", "none"}, {"colormode", "ct"}, {"hue", 200},
                        {"sat", 254}, {"effect", "none"}, {"xy", {0.f, 0.f}}}},
                {"state", {{"any_on", true}, {"all_on", true}}}}}}}};

    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername(), nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .Times(1)
        .WillOnce(Return(hue_bridge_state));

    // Refresh cache
    test_bridge = Bridge(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);

    // Test when correct data is sent
    Group test_group_1 = test_bridge.groups().get(1);
    EXPECT_EQ(test_group_1.getName(), "Group 1");
    EXPECT_EQ(test_group_1.getType(), "LightGroup");

    // Test again to check whether group is returned directly
    test_group_1 = test_bridge.groups().get(1);
    EXPECT_EQ(test_group_1.getName(), "Group 1");
    EXPECT_EQ(test_group_1.getType(), "LightGroup");
}

TEST(Bridge, removeGroup)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler = std::make_shared<MockHttpHandler>();
    nlohmann::json hue_bridge_state {{"groups",
        {{"1",
            {{"name", "Group 1"}, {"type", "LightGroup"}, {"lights", {"1", "2", "3"}},
                {"action",
                    {{"on", true}, {"bri", 254}, {"ct", 366}, {"alert", "none"}, {"colormode", "ct"}, {"hue", 200},
                        {"sat", 254}, {"effect", "none"}, {"xy", {0.f, 0.f}}}},
                {"state", {{"any_on", true}, {"all_on", true}}}}}}}};
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername(), nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .Times(1)
        .WillOnce(Return(hue_bridge_state));

    Bridge test_bridge(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);

    nlohmann::json return_answer;
    return_answer = nlohmann::json::array();
    return_answer[0] = nlohmann::json::object();
    return_answer[0]["success"] = "/groups/1 deleted";
    EXPECT_CALL(*handler,
        DELETEJson(
            "/api/" + getBridgeUsername() + "/groups/1", nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .Times(2)
        .WillOnce(Return(return_answer))
        .WillOnce(Return(nlohmann::json()));

    // Test when correct data is sent
    Group test_group_1 = test_bridge.groups().get(1);

    EXPECT_EQ(test_bridge.groups().remove(1), true);

    EXPECT_EQ(test_bridge.groups().remove(1), false);
}

TEST(Bridge, groupExists)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler = std::make_shared<MockHttpHandler>();
    nlohmann::json hue_bridge_state {{"groups",
        {{"1",
            {{"name", "Group 1"}, {"type", "LightGroup"}, {"lights", {"1", "2", "3"}},
                {"action",
                    {{"on", true}, {"bri", 254}, {"ct", 366}, {"alert", "none"}, {"colormode", "ct"}, {"hue", 200},
                        {"sat", 254}, {"effect", "none"}, {"xy", {0.f, 0.f}}}},
                {"state", {{"any_on", true}, {"all_on", true}}}}}}}};
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername(), nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(hue_bridge_state));

    Bridge test_bridge(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);

    test_bridge.refresh();

    EXPECT_EQ(true, Const(test_bridge).groups().exists(1));
    EXPECT_EQ(false, Const(test_bridge).groups().exists(2));
}

TEST(Bridge, getAllGroups)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler = std::make_shared<MockHttpHandler>();
    nlohmann::json hue_bridge_state {{"groups",
        {{"1",
            {{"name", "Group 1"}, {"type", "LightGroup"}, {"lights", {"1", "2", "3"}},
                {"action",
                    {{"on", true}, {"bri", 254}, {"ct", 366}, {"alert", "none"}, {"colormode", "ct"}, {"hue", 200},
                        {"sat", 254}, {"effect", "none"}, {"xy", {0.f, 0.f}}}},
                {"state", {{"any_on", true}, {"all_on", true}}}}}}}};

    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername(), nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(hue_bridge_state));

    Bridge test_bridge(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);

    std::vector<Group> test_groups = test_bridge.groups().getAll();
    ASSERT_EQ(1, test_groups.size());
    EXPECT_EQ(test_groups[0].getName(), "Group 1");
    EXPECT_EQ(test_groups[0].getType(), "LightGroup");
}

TEST(Bridge, createGroup)
{
    using namespace ::testing;
    std::shared_ptr<MockHttpHandler> handler = std::make_shared<MockHttpHandler>();
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername(), nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .Times(AtLeast(1));
    Bridge test_bridge(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);
    CreateGroup create = CreateGroup::Room({2, 3}, "Nice room", "LivingRoom");
    nlohmann::json request = create.getRequest();
    const int id = 4;
    nlohmann::json response = {{{"success", {{"id", std::to_string(id)}}}}};
    EXPECT_CALL(*handler, POSTJson("/api/" + getBridgeUsername() + "/groups", request, getBridgeIp(), getBridgePort()))
        .WillOnce(Return(response));
    EXPECT_EQ(id, test_bridge.groups().create(create));

    response = {};
    EXPECT_CALL(*handler, POSTJson("/api/" + getBridgeUsername() + "/groups", request, getBridgeIp(), getBridgePort()))
        .WillOnce(Return(response));
    EXPECT_EQ(0, test_bridge.groups().create(create));
}

#define IGNORE_EXCEPTIONS(statement)                                                                                   \
    try                                                                                                                \
    {                                                                                                                  \
        statement;                                                                                                     \
    }                                                                                                                  \
    catch (...)                                                                                                        \
    { }

TEST(Bridge, instantiateResourceLists)
{
    // Instantiate all methods on the resource lists, so that compile errors become visible
    using namespace ::testing;
    nlohmann::json bridgeState {{"lights", nlohmann::json::object()}, {"groups", nlohmann::json::object()},
        {"schedules", nlohmann::json::object()}, {"scenes", nlohmann::json::object()},
        {"sensors", nlohmann::json::object()}, {"rules", nlohmann::json::object()}};
    std::shared_ptr<MockHttpHandler> handler = std::make_shared<MockHttpHandler>();
    EXPECT_CALL(*handler, GETJson(_, _, getBridgeIp(), getBridgePort())).Times(AnyNumber());
    EXPECT_CALL(*handler, POSTJson(_, _, getBridgeIp(), getBridgePort())).Times(AnyNumber());
    EXPECT_CALL(*handler, DELETEJson(_, _, getBridgeIp(), getBridgePort())).Times(AnyNumber());
    EXPECT_CALL(
        *handler, GETJson("/api/" + getBridgeUsername(), nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(bridgeState));

    Bridge bridge(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);

    IGNORE_EXCEPTIONS(bridge.lights().getAll());
    IGNORE_EXCEPTIONS(bridge.lights().get(1));
    IGNORE_EXCEPTIONS(bridge.lights().exists(1));
    IGNORE_EXCEPTIONS(bridge.lights().search());
    IGNORE_EXCEPTIONS(bridge.lights().getNewDevices());
    IGNORE_EXCEPTIONS(bridge.lights().remove(1));

    IGNORE_EXCEPTIONS(bridge.groups().getAll());
    IGNORE_EXCEPTIONS(bridge.groups().get(1));
    IGNORE_EXCEPTIONS(bridge.groups().exists(1));
    IGNORE_EXCEPTIONS(bridge.groups().create(CreateGroup::Entertainment({}, "")));
    IGNORE_EXCEPTIONS(bridge.groups().remove(1));

    IGNORE_EXCEPTIONS(bridge.schedules().getAll());
    IGNORE_EXCEPTIONS(bridge.schedules().get(1));
    IGNORE_EXCEPTIONS(bridge.schedules().exists(1));
    IGNORE_EXCEPTIONS(bridge.schedules().create(CreateSchedule()));
    IGNORE_EXCEPTIONS(bridge.schedules().remove(1));

    IGNORE_EXCEPTIONS(bridge.scenes().getAll());
    IGNORE_EXCEPTIONS(bridge.scenes().get("1"));
    IGNORE_EXCEPTIONS(bridge.scenes().exists("1"));
    IGNORE_EXCEPTIONS(bridge.scenes().create(CreateScene()));
    IGNORE_EXCEPTIONS(bridge.scenes().remove("1"));

    IGNORE_EXCEPTIONS(bridge.sensors().getAll());
    IGNORE_EXCEPTIONS(bridge.sensors().get(1));
    IGNORE_EXCEPTIONS(bridge.sensors().exists(1));
    IGNORE_EXCEPTIONS(bridge.sensors().create(CreateSensor("", "", "", "", "", "")));
    IGNORE_EXCEPTIONS(bridge.sensors().search());
    IGNORE_EXCEPTIONS(bridge.sensors().getNewDevices());
    IGNORE_EXCEPTIONS(bridge.sensors().remove(1));

    IGNORE_EXCEPTIONS(bridge.rules().getAll());
    IGNORE_EXCEPTIONS(bridge.rules().get(1));
    IGNORE_EXCEPTIONS(bridge.rules().exists(1));
    IGNORE_EXCEPTIONS(bridge.rules().create(CreateRule({}, {})));
    IGNORE_EXCEPTIONS(bridge.rules().remove(1));
}
