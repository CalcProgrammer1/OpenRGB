/**
    \file test_Scene.cpp
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

#include <hueplusplus/Scene.h>

#include <gtest/gtest.h>

#include "testhelper.h"

#include "mocks/mock_HttpHandler.h"

using namespace hueplusplus;
using namespace testing;

TEST(LightState, on)
{
    EXPECT_FALSE(LightState(nlohmann::json::object()).isOn());
    EXPECT_TRUE(LightState(nlohmann::json {{"on", true}}).isOn());
    EXPECT_FALSE(LightState(nlohmann::json {{"on", false}}).isOn());
}

TEST(LightState, Brightness)
{
    EXPECT_FALSE(LightState(nlohmann::json::object()).hasBrightness());
    const int bri = 125;
    nlohmann::json json {{"bri", bri}};
    const LightState state {json};
    EXPECT_TRUE(state.hasBrightness());
    EXPECT_EQ(bri, state.getBrightness());
}

TEST(LightState, HueSat)
{
    EXPECT_FALSE(LightState(nlohmann::json::object()).hasHueSat());
    EXPECT_FALSE(LightState(nlohmann::json {{"hue", 0}}).hasHueSat());
    EXPECT_FALSE(LightState(nlohmann::json {{"sat", 0}}).hasHueSat());
    const int hue = 12553;
    const int sat = 240;
    nlohmann::json json {{"hue", hue}, {"sat", sat}};
    const LightState state {json};
    EXPECT_TRUE(state.hasHueSat());
    EXPECT_EQ(hue, state.getHueSat().hue);
    EXPECT_EQ(sat, state.getHueSat().saturation);
}

TEST(LightState, XY)
{
    EXPECT_FALSE(LightState(nlohmann::json::object()).hasXY());
    const float x = 0.6f;
    const float y = 0.3f;
    nlohmann::json json {{"xy", {x, y}}, {"bri", 255}};
    const LightState state {json};
    EXPECT_TRUE(state.hasXY());
    EXPECT_FLOAT_EQ(x, state.getXY().xy.x);
    EXPECT_FLOAT_EQ(y, state.getXY().xy.y);
    EXPECT_FLOAT_EQ(1.f, state.getXY().brightness);
}

TEST(LightState, Ct)
{
    EXPECT_FALSE(LightState(nlohmann::json::object()).hasCt());
    const int ct = 260;
    nlohmann::json json {{"ct", ct}};
    const LightState state {json};
    EXPECT_TRUE(state.hasCt());
    EXPECT_EQ(ct, state.getCt());
}

TEST(LightState, Effect)
{
    EXPECT_FALSE(LightState(nlohmann::json::object()).hasEffect());
    nlohmann::json json {{"effect", "colorloop"}};
    const LightState state {json};
    EXPECT_TRUE(state.hasEffect());
    EXPECT_TRUE(state.getColorloop());
    EXPECT_FALSE(LightState(nlohmann::json {{"effect", "none"}}).getColorloop());
}

TEST(LightState, TransitionTime)
{
    EXPECT_EQ(4, LightState(nlohmann::json::object()).getTransitionTime());
    EXPECT_EQ(0, LightState(nlohmann::json {{"transitiontime", 0}}).getTransitionTime());
}

TEST(LightState, toJson)
{
    const nlohmann::json json {{"on", false}, {"bri", 254}, {"ct", 400}};
    EXPECT_EQ(json, LightState(json).toJson());
}

TEST(LightStateBuilder, create)
{
    {
        const nlohmann::json json {{"on", false}, {"bri", 254}, {"ct", 400}, {"effect", "colorloop"}};
        EXPECT_EQ(
            json, LightStateBuilder().setOn(false).setBrightness(254).setCt(400).setColorloop(true).create().toJson());
    }
    {
        const nlohmann::json json {{"xy", {0.5f, 0.5f}}, {"effect", "none"}};
        EXPECT_EQ(json, LightStateBuilder().setXY({0.5f, 0.5f}).setColorloop(false).create().toJson());
    }
    {
        const nlohmann::json json {{"hue", 360}, {"sat", 230}, {"transitiontime", 4}};
        EXPECT_EQ(json, LightStateBuilder().setHueSat({360, 230}).setTransitionTime(4).create().toJson());
    }
}

class SceneTest : public Test
{
public:
    std::shared_ptr<MockHttpHandler> handler;
    HueCommandAPI commands;
    nlohmann::json sceneState;

    SceneTest()
        : handler(std::make_shared<MockHttpHandler>()),
          commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler),
          sceneState({{"name", "Test scene"}, {"type", "GroupScene"}, {"group", "4"}, {"lights", {"3", "4", "5"}},
              {"owner", "testowner"}, {"recycle", false}, {"locked", false},
              {"appdata", {{"data", "test-data"}, {"version", 2}}}, {"picture", ""},
              {"lastupdated", "2020-04-23T12:00:04"}, {"version", 2},
              {"lightstates",
                  {{"3", {{"on", false}, {"bri", 100}, {"xy", {0.3, 0.2}}}},
                      {"4", {{"on", true}, {"bri", 200}, {"xy", {0.3, 0.2}}, {"effect", "colorloop"}}},
                      {"5", {{"on", true}, {"bri", 100}, {"xy", {0.3, 0.2}}}}}}})
    { }

    void expectGetState(const std::string& id)
    {
        EXPECT_CALL(*handler,
            GETJson("/api/" + getBridgeUsername() + "/scenes/" + id, nlohmann::json::object(), getBridgeIp(),
                getBridgePort()))
            .WillOnce(Return(sceneState));
    }
};

TEST_F(SceneTest, Constructor)
{
    const std::string id = "asd89263";
    expectGetState(id);
    const Scene scene(id, commands, std::chrono::seconds(0), nullptr);
    EXPECT_EQ(id, scene.getId());
}

TEST_F(SceneTest, getName)
{
    const std::string id = "125abets8912";
    const std::string name = "Scene name";
    sceneState["name"] = name;
    expectGetState(id);
    const Scene scene(id, commands, std::chrono::seconds(0), nullptr);
    EXPECT_EQ(name, scene.getName());
}

TEST_F(SceneTest, getType)
{
    const std::string id = "125abets8912";
    {
        sceneState["type"] = "GroupScene";
        expectGetState(id);
        const Scene scene(id, commands, std::chrono::seconds(0), nullptr);
        EXPECT_EQ(Scene::Type::groupScene, scene.getType());
    }
    {
        sceneState["type"] = "LightScene";
        expectGetState(id);
        const Scene scene(id, commands, std::chrono::seconds(0), nullptr);
        EXPECT_EQ(Scene::Type::lightScene, scene.getType());
    }
}

TEST_F(SceneTest, getGroupId)
{
    const std::string id = "125abets8912";
    {
        sceneState["group"] = "3";
        expectGetState(id);
        const Scene scene(id, commands, std::chrono::seconds(0), nullptr);
        EXPECT_EQ(3, scene.getGroupId());
    }
    {
        sceneState["type"] = "LightScene";
        sceneState.erase("group");
        expectGetState(id);
        const Scene scene(id, commands, std::chrono::seconds(0), nullptr);
        EXPECT_EQ(0, scene.getGroupId());
    }
}

TEST_F(SceneTest, getLightIds)
{
    const std::string id = "125asav3";
    sceneState["lights"] = {"3", "4", "5"};
    expectGetState(id);
    const Scene scene(id, commands, std::chrono::seconds(0), nullptr);
    EXPECT_THAT(scene.getLightIds(), UnorderedElementsAre(3, 4, 5));
}

TEST_F(SceneTest, getOwner)
{
    const std::string id = "125asav3";
    const std::string owner = "testowner";
    sceneState["owner"] = owner;
    expectGetState(id);
    const Scene scene(id, commands, std::chrono::seconds(0), nullptr);
    EXPECT_EQ(owner, scene.getOwner());
}

TEST_F(SceneTest, getRecycle)
{
    const std::string id = "125asav3";
    const bool recycle = true;
    sceneState["recycle"] = recycle;
    expectGetState(id);
    const Scene scene(id, commands, std::chrono::seconds(0), nullptr);
    EXPECT_EQ(recycle, scene.getRecycle());
}

TEST_F(SceneTest, isLocked)
{
    const std::string id = "125asav3";
    const bool locked = true;
    sceneState["locked"] = locked;
    expectGetState(id);
    const Scene scene(id, commands, std::chrono::seconds(0), nullptr);
    EXPECT_EQ(locked, scene.isLocked());
}

TEST_F(SceneTest, getAppdata)
{
    const std::string id = "125asav3";
    const std::string appdata = "some data";
    const int version = 10;
    sceneState["appdata"] = {{"version", version}, {"data", appdata}};
    expectGetState(id);
    const Scene scene(id, commands, std::chrono::seconds(0), nullptr);
    EXPECT_EQ(version, scene.getAppdataVersion());
    EXPECT_EQ(appdata, scene.getAppdata());
}

TEST_F(SceneTest, getPicture)
{
    const std::string id = "125asav3";
    const std::string picture = "abcpicture";
    sceneState["picture"] = picture;
    expectGetState(id);
    const Scene scene(id, commands, std::chrono::seconds(0), nullptr);
    EXPECT_EQ(picture, scene.getPicture());
}

TEST_F(SceneTest, getLastUpdated)
{
    const std::string id = "125asav3";
    expectGetState(id);
    const Scene scene(id, commands, std::chrono::seconds(0), nullptr);
    const time::AbsoluteTime lastUpdated = scene.getLastUpdated();
    EXPECT_EQ(time::parseUTCTimestamp("2020-04-23T12:00:04"), lastUpdated.getBaseTime());
}

TEST_F(SceneTest, getVersion)
{
    const std::string id = "125asav3";
    const int version = 2;
    sceneState["version"] = version;
    expectGetState(id);
    const Scene scene(id, commands, std::chrono::seconds(0), nullptr);
    EXPECT_EQ(version, scene.getVersion());
}

TEST_F(SceneTest, getLightstates)
{
    const std::string id = "125asav3";
    {
        const std::map<int, LightState> lightstates {
            {3, LightStateBuilder().setOn(false).setBrightness(100).setXY({0.3, 0.2}).create()},
            {4, LightStateBuilder().setOn(false).setBrightness(200).setXY({0.3, 0.2}).setColorloop(true).create()},
            {5, LightStateBuilder().setOn(true).setBrightness(100).setXY({0.3, 0.2}).create()}};
        nlohmann::json lightstatesJson;
        for (const auto& entry : lightstates)
        {
            lightstatesJson[std::to_string(entry.first)] = entry.second.toJson();
        }
        sceneState["lightstates"] = lightstatesJson;
        expectGetState(id);
        const Scene scene(id, commands, std::chrono::seconds(0), nullptr);
        const std::map<int, LightState> result = scene.getLightStates();
        EXPECT_EQ(lightstates, result);
    }
    // No lightstates (old scene)
    {
        sceneState.erase("lightstates");
        expectGetState(id);
        const Scene scene(id, commands, std::chrono::seconds(0), nullptr);
        EXPECT_TRUE(scene.getLightStates().empty());
    }
}

TEST_F(SceneTest, refresh)
{
    const std::string id = "125asav3";
    expectGetState(id);
    Scene scene(id, commands, std::chrono::seconds(0), nullptr);
    expectGetState(id);
    scene.refresh(true);
}

TEST_F(SceneTest, setName)
{
    const std::string id = "125asav3";
    expectGetState(id);
    Scene scene(id, commands, std::chrono::seconds(0), nullptr);
    const std::string name = "Scene name";
    nlohmann::json request = {{"name", name}};
    nlohmann::json response = {{"success", {"/scenes/" + id + "/name", name}}};
    EXPECT_CALL(
        *handler, PUTJson("/api/" + getBridgeUsername() + "/scenes/" + id, request, getBridgeIp(), getBridgePort()))
        .WillOnce(Return(response));
    expectGetState(id);
    scene.setName(name);
}

TEST_F(SceneTest, setLightIds)
{
    const std::string id = "125asav3";
    expectGetState(id);
    Scene scene(id, commands, std::chrono::seconds(0), nullptr);
    const std::vector<int> lightIds = {3, 4, 6, 8};
    nlohmann::json request = {{"lights", {"3", "4", "6", "8"}}};
    nlohmann::json response = {{"success", {"/scenes/" + id + "/lights", {"3", "4", "6", "8"}}}};
    EXPECT_CALL(
        *handler, PUTJson("/api/" + getBridgeUsername() + "/scenes/" + id, request, getBridgeIp(), getBridgePort()))
        .WillOnce(Return(response));
    expectGetState(id);
    scene.setLightIds(lightIds);
}

TEST_F(SceneTest, setAppdata)
{
    const std::string id = "125asav3";
    expectGetState(id);
    Scene scene(id, commands, std::chrono::seconds(0), nullptr);
    const std::string appdata = "New appdata";
    const int version = 3;
    nlohmann::json request = {{"appdata", {{"version", version}, {"data", appdata}}}};
    nlohmann::json response = {{"success", {"/scenes/" + id + "/appdata/version", version}},
        {"success", {"/scenes/" + id + "/appdata/data", appdata}}};
    EXPECT_CALL(
        *handler, PUTJson("/api/" + getBridgeUsername() + "/scenes/" + id, request, getBridgeIp(), getBridgePort()))
        .WillOnce(Return(response));
    expectGetState(id);
    scene.setAppdata(appdata, version);
}

TEST_F(SceneTest, setLightStates)
{
    const std::string id = "125asav3";
    const std::map<int, LightState> lightstates {
        {3, LightStateBuilder().setOn(false).setBrightness(100).setCt(200).create()},
        {5, LightStateBuilder().setOn(true).setBrightness(200).setXY({0.3, 0.2}).create()}};
    nlohmann::json lightstatesJson;
    for (const auto& entry : lightstates)
    {
        lightstatesJson[std::to_string(entry.first)] = entry.second.toJson();
    }
    expectGetState(id);
    Scene scene(id, commands, std::chrono::seconds(0), nullptr);
    nlohmann::json request = {{"lightstates", lightstatesJson}};
    nlohmann::json response = {{"success", {"/scenes/" + id + "/lights/3/state/on", false}},
        {"success", {"/scenes/" + id + "/lights/3/state/bri", 100}},
        {"success", {"/scenes/" + id + "/lights/3/state/ct", 200}},
        {"success", {"/scenes/" + id + "/lights/5/state/on", true}},
        {"success", {"/scenes/" + id + "/lights/5/state/bri", 200}},
        {"success", {"/scenes/" + id + "/lights/5/state/xy", {0.3, 0.2}}}};
    EXPECT_CALL(
        *handler, PUTJson("/api/" + getBridgeUsername() + "/scenes/" + id, request, getBridgeIp(), getBridgePort()))
        .WillOnce(Return(response));
    expectGetState(id);
    scene.setLightStates(lightstates);
}

TEST_F(SceneTest, storeCurrentLightState)
{
    const std::string id = "125asav3";
    expectGetState(id);
    Scene scene(id, commands, std::chrono::seconds(0), nullptr);
    {
        nlohmann::json request = {{"storelightstate", true}};
        nlohmann::json response = {{"success", {"/scenes/" + id + "/storelightstate", true}}};
        EXPECT_CALL(
            *handler, PUTJson("/api/" + getBridgeUsername() + "/scenes/" + id, request, getBridgeIp(), getBridgePort()))
            .WillOnce(Return(response));
        expectGetState(id);
        scene.storeCurrentLightState();
    }
    {
        const int transitiontime = 3;
        nlohmann::json request = {{"storelightstate", true}, {"transitiontime", 3}};
        nlohmann::json response = {{"success", {"/scenes/" + id + "/storelightstate", true}}};
        EXPECT_CALL(
            *handler, PUTJson("/api/" + getBridgeUsername() + "/scenes/" + id, request, getBridgeIp(), getBridgePort()))
            .WillOnce(Return(response));
        expectGetState(id);
        scene.storeCurrentLightState(transitiontime);
    }
}

TEST_F(SceneTest, recall)
{
    const std::string id = "125asav3";
    // LightScene
    {
        sceneState["type"] = "LightScene";
        expectGetState(id);
        nlohmann::json request = {{"scene", id}};
        nlohmann::json response = {{"success", {"/groups/0/action/scene", id}}};
        Scene scene(id, commands, std::chrono::seconds(0), nullptr);
        EXPECT_CALL(*handler,
            PUTJson("/api/" + getBridgeUsername() + "/groups/0/action", request, getBridgeIp(), getBridgePort()))
            .WillOnce(Return(response));
        scene.recall();
        Mock::VerifyAndClearExpectations(handler.get());
    }
    // GroupScene
    {
        sceneState["type"] = "GroupScene";
        std::string groupId = "3";
        sceneState["group"] = groupId;
        expectGetState(id);
        nlohmann::json request = {{"scene", id}};
        nlohmann::json response = {{"success", {"/groups/" + groupId + "/action/scene", id}}};
        Scene scene(id, commands, std::chrono::seconds(0), nullptr);
        EXPECT_CALL(*handler,
            PUTJson("/api/" + getBridgeUsername() + "/groups/" + groupId + "/action", request, getBridgeIp(),
                getBridgePort()))
            .WillOnce(Return(response));
        scene.recall();
    }
}

TEST(CreateScene, setName)
{
    const std::string name = "New scene";
    const nlohmann::json request = {{"name", name}};
    EXPECT_EQ(request, CreateScene().setName(name).getRequest());
}

TEST(CreateScene, setGroupId)
{
    const int groupId = 23;
    const nlohmann::json request = {{"group", "23"}, {"type", "GroupScene"}};
    EXPECT_EQ(request, CreateScene().setGroupId(groupId).getRequest());
    EXPECT_THROW(CreateScene().setGroupId(2).setLightIds({1}), HueException);
}

TEST(CreateScene, setLightIds)
{
    const std::vector<int> lightIds = {3, 4, 5, 9};
    const nlohmann::json request = {{"lights", {"3", "4", "5", "9"}}, {"type", "LightScene"}};
    EXPECT_EQ(request, CreateScene().setLightIds(lightIds).getRequest());
    EXPECT_THROW(CreateScene().setLightIds(lightIds).setGroupId(1), HueException);
}

TEST(CreateScene, setRecycle)
{
    const nlohmann::json request = {{"recycle", true}};
    EXPECT_EQ(request, CreateScene().setRecycle(true).getRequest());
}

TEST(CreateScene, setAppdata)
{
    const std::string data = "testdata";
    const int version = 3;
    const nlohmann::json request = {{"appdata", {{"data", data}, {"version", version}}}};
    EXPECT_EQ(request, CreateScene().setAppdata(data, version).getRequest());
}

TEST(CreateScene, setLightStates)
{
    const std::map<int, LightState> lightStates
        = {{1, LightStateBuilder().setOn(true).create()}, {5, LightStateBuilder().setCt(300).create()}};
    const nlohmann::json request = {{"lightstates", {{"1", {{"on", true}}}, {"5", {{"ct", 300}}}}}};
    EXPECT_EQ(request, CreateScene().setLightStates(lightStates).getRequest());
}