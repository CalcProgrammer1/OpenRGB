/**
    \file test_Group.cpp
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

#include <gtest/gtest.h>

#include "testhelper.h"

#include "hueplusplus/Group.h"
#include "mocks/mock_HttpHandler.h"

using namespace hueplusplus;
using namespace testing;

class GroupTest : public Test
{
protected:
    const std::string groupName = "Group 1";
    const std::string type = "Room";
    const std::string roomType = "Bedroom";
    const bool on = true;
    const int bri = 254;
    const int hue = 10000;
    const int sat = 254;
    const std::string effect = "none";
    const float x = 0.5f;
    const float y = 0.6f;
    const int ct = 250;
    const std::string alert = "none";
    const std::string colormode = "ct";
    const bool any_on = true;
    const bool all_on = false;

    std::shared_ptr<MockHttpHandler> handler;
    HueCommandAPI commands;
    nlohmann::json groupState;

protected:
    GroupTest()
        : handler(std::make_shared<MockHttpHandler>()),
          commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler),
          groupState({{"name", groupName}, {"type", type}, {"class", roomType}, {"lights", {"1", "2", "4"}},
              {"action",
                  {{"on", on}, {"bri", bri}, {"hue", hue}, {"sat", sat}, {"effect", effect},
                      {"xy", nlohmann::json::array({x, y})}, {"ct", ct}, {"alert", alert}, {"colormode", colormode}}},
              {"state", {{"any_on", any_on}, {"all_on", all_on}}}})
    {}

    void expectGetState(int id)
    {
        EXPECT_CALL(*handler,
            GETJson("/api/" + getBridgeUsername() + "/groups/" + std::to_string(id), nlohmann::json::object(),
                getBridgeIp(), getBridgePort()))
            .WillOnce(Return(groupState));
    }
};

TEST_F(GroupTest, Construtor)
{
    {
        const int id = 12;
        expectGetState(id);
        Group group(id, commands, std::chrono::seconds(0), nullptr);
        EXPECT_EQ(id, group.getId());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    {
        const int id = 0;
        expectGetState(id);
        Group group(id, commands, std::chrono::seconds(0), nullptr);
        EXPECT_EQ(id, group.getId());
        Mock::VerifyAndClearExpectations(handler.get());
    }
}

TEST_F(GroupTest, getName)
{
    const int id = 1;
    expectGetState(id);
    Group group(id, commands, std::chrono::seconds(0), nullptr);
    EXPECT_EQ(groupName, Const(group).getName());
}

TEST_F(GroupTest, getType)
{
    const int id = 1;
    expectGetState(id);
    Group group(id, commands, std::chrono::seconds(0), nullptr);
    EXPECT_EQ(type, Const(group).getType());
}

TEST_F(GroupTest, getLightIds)
{
    const int id = 1;
    expectGetState(id);
    Group group(id, commands, std::chrono::seconds(0), nullptr);
    EXPECT_EQ(std::vector<int>({1, 2, 4}), Const(group).getLightIds());
}

TEST_F(GroupTest, getRoomType)
{
    const int id = 1;
    expectGetState(id);
    Group group(id, commands, std::chrono::seconds(0), nullptr);
    EXPECT_EQ(roomType, Const(group).getRoomType());
}

TEST_F(GroupTest, getAllOn)
{
    const int id = 1;
    expectGetState(id);
    Group group(id, commands, std::chrono::steady_clock::duration::max(), nullptr);
    EXPECT_EQ(all_on, group.getAllOn());
    EXPECT_EQ(all_on, Const(group).getAllOn());
}

TEST_F(GroupTest, getAnyOn)
{
    const int id = 1;
    expectGetState(id);
    Group group(id, commands, std::chrono::steady_clock::duration::max(), nullptr);
    EXPECT_EQ(any_on, group.getAnyOn());
    EXPECT_EQ(any_on, Const(group).getAnyOn());
}

TEST_F(GroupTest, getActionOn)
{
    const int id = 1;
    expectGetState(id);
    Group group(id, commands, std::chrono::steady_clock::duration::max(), nullptr);
    EXPECT_EQ(on, group.getActionOn());
    EXPECT_EQ(on, Const(group).getActionOn());
}

TEST_F(GroupTest, getActionHueSaturation)
{
    const int id = 1;
    expectGetState(id);
    Group group(id, commands, std::chrono::steady_clock::duration::max(), nullptr);
    std::pair<uint16_t, uint8_t> hueSat {hue, sat};
    EXPECT_EQ(hueSat, group.getActionHueSaturation());
    EXPECT_EQ(hueSat, Const(group).getActionHueSaturation());
}

TEST_F(GroupTest, getActionBrightness)
{
    const int id = 1;
    expectGetState(id);
    Group group(id, commands, std::chrono::steady_clock::duration::max(), nullptr);
    EXPECT_EQ(bri, group.getActionBrightness());
    EXPECT_EQ(bri, Const(group).getActionBrightness());
}

TEST_F(GroupTest, getActionColorTemperature)
{
    const int id = 1;
    expectGetState(id);
    Group group(id, commands, std::chrono::steady_clock::duration::max(), nullptr);
    EXPECT_EQ(ct, group.getActionColorTemperature());
    EXPECT_EQ(ct, Const(group).getActionColorTemperature());
}

TEST_F(GroupTest, getActionColorXY)
{
    const int id = 1;
    expectGetState(id);
    Group group(id, commands, std::chrono::steady_clock::duration::max(), nullptr);
    std::pair<float, float> xy {x, y};
    EXPECT_EQ(xy, group.getActionColorXY());
    EXPECT_EQ(xy, Const(group).getActionColorXY());
}

TEST_F(GroupTest, getActionColorMode)
{
    const int id = 1;
    expectGetState(id);
    Group group(id, commands, std::chrono::steady_clock::duration::max(), nullptr);
    EXPECT_EQ(colormode, group.getActionColorMode());
    EXPECT_EQ(colormode, Const(group).getActionColorMode());
}

TEST_F(GroupTest, setName)
{
    const int id = 1;
    expectGetState(id);
    Group group(id, commands, std::chrono::steady_clock::duration::max(), nullptr);
    const std::string name = "Test group";
    nlohmann::json request = {{"name", name}};
    nlohmann::json response = {{"success", {"/groups/1/name", name}}};
    EXPECT_CALL(*handler, PUTJson("/api/" + getBridgeUsername() + "/groups/1", request, getBridgeIp(), getBridgePort()))
        .WillOnce(Return(response));
    expectGetState(id);
    group.setName(name);
}

TEST_F(GroupTest, setLights)
{
    const int id = 1;
    expectGetState(id);
    Group group(id, commands, std::chrono::steady_clock::duration::max(), nullptr);
    const nlohmann::json lights = {"2", "4", "5"};
    nlohmann::json request = {{"lights", lights}};
    nlohmann::json response = {{"success", {"/groups/1/lights", lights}}};
    EXPECT_CALL(*handler, PUTJson("/api/" + getBridgeUsername() + "/groups/1", request, getBridgeIp(), getBridgePort()))
        .WillOnce(Return(response));
    expectGetState(id);
    group.setLights(std::vector<int> {2, 4, 5});
}

TEST_F(GroupTest, setRoomType)
{
    const int id = 1;
    expectGetState(id);
    Group group(id, commands, std::chrono::steady_clock::duration::max(), nullptr);
    const std::string type = "LivingRoom";
    nlohmann::json request = {{"class", type}};
    nlohmann::json response = {{"success", {"/groups/1/class", type}}};
    EXPECT_CALL(*handler, PUTJson("/api/" + getBridgeUsername() + "/groups/1", request, getBridgeIp(), getBridgePort()))
        .WillOnce(Return(response));
    expectGetState(id);
    group.setRoomType(type);
}

TEST_F(GroupTest, setScene)
{
    const int id = 1;
    expectGetState(id);
    Group group(id, commands, std::chrono::steady_clock::duration::max(), nullptr);
    const std::string scene = "testScene";
    nlohmann::json request = {{"scene", scene}};
    nlohmann::json response = {{"success", {"/groups/1/action/scene", scene}}};
    EXPECT_CALL(
        *handler, PUTJson("/api/" + getBridgeUsername() + "/groups/1/action", request, getBridgeIp(), getBridgePort()))
        .WillOnce(Return(response));
    group.setScene(scene);
}

TEST_F(GroupTest, createSceneAction)
{
    const int id = 1;
    expectGetState(id);
    const Group group(id, commands, std::chrono::steady_clock::duration::max(), nullptr);
    const std::string scene = "testScene";
    nlohmann::json request = { {"scene", scene} };
    hueplusplus::Action command = group.createSceneAction(scene);
    EXPECT_EQ(hueplusplus::Action::Method::put, command.getMethod());
    EXPECT_EQ("/api/" + getBridgeUsername() + "/groups/1/action", command.getAddress());
    EXPECT_EQ(request, command.getBody());
}

TEST(CreateGroup, LightGroup)
{
    EXPECT_EQ(nlohmann::json({{"lights", {"1"}}, {"type", "LightGroup"}, {"name", "Name"}, {"class", "Other"}}),
        CreateGroup::LightGroup({1}, "Name").getRequest());
    EXPECT_EQ(nlohmann::json({{"lights", {"2", "4"}}, {"type", "LightGroup"}, {"class", "Other"}}),
        CreateGroup::LightGroup({2, 4}).getRequest());
}

TEST(CreateGroup, Entertainment)
{
    EXPECT_EQ(nlohmann::json({{"lights", {"1"}}, {"type", "Entertainment"}, {"name", "Name"}, {"class", "Other"}}),
        CreateGroup::Entertainment({1}, "Name").getRequest());
    EXPECT_EQ(nlohmann::json({{"lights", {"2", "4"}}, {"type", "Entertainment"}, {"class", "Other"}}),
        CreateGroup::Entertainment({2, 4}).getRequest());
}

TEST(CreateGroup, Zone)
{
    EXPECT_EQ(nlohmann::json({{"lights", {"1"}}, {"type", "Zone"}, {"name", "Name"}, {"class", "Other"}}),
        CreateGroup::Zone({1}, "Name").getRequest());
    EXPECT_EQ(nlohmann::json({{"lights", {"2", "4"}}, {"type", "Zone"}, {"class", "Other"}}),
        CreateGroup::Zone({2, 4}).getRequest());
}

TEST(CreateGroup, Room)
{
    EXPECT_EQ(nlohmann::json({{"lights", {"1"}}, {"type", "Room"}, {"name", "Name"}, {"class", "Bedroom"}}),
        CreateGroup::Room({1}, "Name", "Bedroom").getRequest());
    EXPECT_EQ(nlohmann::json({{"lights", {"1"}}, {"type", "Room"}, {"name", "Name"}, {"class", "Other"}}),
        CreateGroup::Room({1}, "Name").getRequest());
    EXPECT_EQ(nlohmann::json({{"lights", {"2", "4"}}, {"type", "Room"}, {"class", "Other"}}),
        CreateGroup::Room({2, 4}).getRequest());
}
