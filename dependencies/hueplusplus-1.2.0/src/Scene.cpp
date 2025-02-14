/**
    \file Scene.cpp
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

#include <hueplusplus/HueExceptionMacro.h>
#include <hueplusplus/Scene.h>

namespace hueplusplus
{
LightState::LightState(const nlohmann::json& state) : state(state) { }

bool LightState::isOn() const
{
    return state.value("on", false);
}

bool LightState::hasBrightness() const
{
    return state.count("bri");
}

int LightState::getBrightness() const
{
    return state.value("bri", 0);
}

bool LightState::hasHueSat() const
{
    return state.count("hue") && state.count("sat");
}

HueSaturation LightState::getHueSat() const
{
    return HueSaturation {state.value("hue", 0), state.value("sat", 0)};
}

bool LightState::hasXY() const
{
    return state.count("xy");
}

XYBrightness LightState::getXY() const
{
    const nlohmann::json& xy = state.at("xy");
    return XYBrightness {{xy[0].get<float>(), xy[1].get<float>()}, state.at("bri").get<int>() / 255.f};
}

bool LightState::hasCt() const
{
    return state.count("ct");
}

int LightState::getCt() const
{
    return state.value("ct", 0);
}

bool LightState::hasEffect() const
{
    return state.count("effect");
}

bool LightState::getColorloop() const
{
    return state.value("effect", "") == "colorloop";
}

int LightState::getTransitionTime() const
{
    return state.value("transitiontime", 4);
}

nlohmann::json LightState::toJson() const
{
    return state;
}

bool LightState::operator==(const LightState& other) const
{
    return state == other.state;
}

bool LightState::operator!=(const LightState& other) const
{
    return state != other.state;
}

LightStateBuilder& LightStateBuilder::setOn(bool on)
{
    state["on"] = on;
    return *this;
}

LightStateBuilder& LightStateBuilder::setBrightness(int brightness)
{
    state["bri"] = brightness;
    return *this;
}

LightStateBuilder& LightStateBuilder::setHueSat(const HueSaturation& hueSat)
{
    state["hue"] = hueSat.hue;
    state["sat"] = hueSat.saturation;
    return *this;
}

LightStateBuilder& LightStateBuilder::setXY(const XY& xy)
{
    state["xy"] = {xy.x, xy.y};
    return *this;
}

LightStateBuilder& LightStateBuilder::setCt(int mired)
{
    state["ct"] = mired;
    return *this;
}

LightStateBuilder& LightStateBuilder::setColorloop(bool enabled)
{
    state["effect"] = enabled ? "colorloop" : "none";
    return *this;
}

LightStateBuilder& LightStateBuilder::setTransitionTime(int time)
{
    state["transitiontime"] = time;
    return *this;
}

LightState LightStateBuilder::create()
{
    return LightState(state);
}

Scene::Scene(const std::string& id, const std::shared_ptr<APICache>& baseCache)
    : id(id), state(baseCache, id, baseCache->getRefreshDuration())
{ }

Scene::Scene(const std::string& id, const HueCommandAPI& commands, std::chrono::steady_clock::duration refreshDuration,
    const nlohmann::json& currentState)
    : id(id), state("/scenes/" + id, commands, refreshDuration, currentState)
{
    refresh();
}

void Scene::refresh(bool force)
{
    if (force)
    {
        state.refresh();
    }
    else
    {
        state.getValue();
    }
}

void Scene::setRefreshDuration(std::chrono::steady_clock::duration refreshDuration)
{
    state.setRefreshDuration(refreshDuration);
}

std::string Scene::getId() const
{
    return id;
}

std::string Scene::getName() const
{
    return state.getValue().at("name").get<std::string>();
}

void Scene::setName(const std::string& name)
{
    sendPutRequest("", {{"name", name}}, CURRENT_FILE_INFO);
    refresh();
}

Scene::Type Scene::getType() const
{
    std::string type = state.getValue().value("type", "LightScene");
    if (type == "LightScene")
    {
        return Type::lightScene;
    }
    else if (type == "GroupScene")
    {
        return Type::groupScene;
    }
    throw HueException(CURRENT_FILE_INFO, "Unknown scene type: " + type);
}

int Scene::getGroupId() const
{
    return std::stoi(state.getValue().value("group", "0"));
}

std::vector<int> Scene::getLightIds() const
{
    std::vector<int> result;
    for (const nlohmann::json& id : state.getValue().at("lights"))
    {
        result.push_back(std::stoi(id.get<std::string>()));
    }
    return result;
}

void Scene::setLightIds(const std::vector<int>& ids)
{
    nlohmann::json lightsJson;
    for (int id : ids)
    {
        lightsJson.push_back(std::to_string(id));
    }
    sendPutRequest("", {{"lights", std::move(lightsJson)}}, CURRENT_FILE_INFO);
    refresh();
}

std::string Scene::getOwner() const
{
    return state.getValue().at("owner").get<std::string>();
}

bool Scene::getRecycle() const
{
    return state.getValue().at("recycle").get<bool>();
}

bool Scene::isLocked() const
{
    return state.getValue().at("locked").get<bool>();
}

std::string Scene::getAppdata() const
{
    return state.getValue().at("appdata").at("data").get<std::string>();
}

int Scene::getAppdataVersion() const
{
    return state.getValue().at("appdata").at("version").get<int>();
}

void Scene::setAppdata(const std::string& data, int version)
{
    sendPutRequest("", {{"appdata", {{"data", data}, {"version", version}}}}, CURRENT_FILE_INFO);
    refresh();
}

std::string Scene::getPicture() const
{
    return state.getValue().value("picture", "");
}

time::AbsoluteTime Scene::getLastUpdated() const
{
    return time::AbsoluteTime::parseUTC(state.getValue().at("lastupdated").get<std::string>());
}

int Scene::getVersion() const
{
    return state.getValue().at("version").get<int>();
}

std::map<int, LightState> Scene::getLightStates() const
{
    if (state.getValue().count("lightstates") == 0)
    {
        return {};
    }
    const nlohmann::json& lightStates = state.getValue().at("lightstates");
    std::map<int, LightState> result;
    for (auto it = lightStates.begin(); it != lightStates.end(); ++it)
    {
        result.emplace(std::stoi(it.key()), LightState(it.value()));
    }
    return result;
}

void Scene::setLightStates(const std::map<int, LightState>& states)
{
    nlohmann::json lightStates;
    for (const auto& entry : states)
    {
        lightStates[std::to_string(entry.first)] = entry.second.toJson();
    }
    sendPutRequest("", {{"lightstates", std::move(lightStates)}}, CURRENT_FILE_INFO);
    refresh();
}

void Scene::storeCurrentLightState()
{
    sendPutRequest("", {{"storelightstate", true}}, CURRENT_FILE_INFO);
    refresh();
}

void Scene::storeCurrentLightState(int transition)
{
    sendPutRequest("", {{"storelightstate", true}, {"transitiontime", transition}}, CURRENT_FILE_INFO);
    refresh();
}

void Scene::recall()
{
    int groupId = 0;
    if (getType() == Type::groupScene)
    {
        groupId = getGroupId();
    }
    state.getCommandAPI().PUTRequest(
        "/groups/" + std::to_string(groupId) + "/action", {{"scene", id}}, CURRENT_FILE_INFO);
}

void Scene::sendPutRequest(const std::string& path, const nlohmann::json& request, FileInfo fileInfo)
{
    state.getCommandAPI().PUTRequest("/scenes/" + id + path, request, std::move(fileInfo));
}

CreateScene& CreateScene::setName(const std::string& name)
{
    request["name"] = name;
    return *this;
}

CreateScene& CreateScene::setGroupId(int id)
{
    if (request.count("lights"))
    {
        throw HueException(CURRENT_FILE_INFO, "Can only set either group or lights");
    }
    request["group"] = std::to_string(id);
    request["type"] = "GroupScene";
    return *this;
}

CreateScene& CreateScene::setLightIds(const std::vector<int>& ids)
{
    if (request.count("group"))
    {
        throw HueException(CURRENT_FILE_INFO, "Can only set either group or lights");
    }
    nlohmann::json lights;
    for (int id : ids)
    {
        lights.push_back(std::to_string(id));
    }
    request["lights"] = std::move(lights);
    request["type"] = "LightScene";
    return *this;
}

CreateScene& CreateScene::setRecycle(bool recycle)
{
    request["recycle"] = recycle;
    return *this;
}

CreateScene& CreateScene::setAppdata(const std::string& data, int version)
{
    request["appdata"] = {{"data", data}, {"version", version}};
    return *this;
}

CreateScene& CreateScene::setLightStates(const std::map<int, LightState>& states)
{
    nlohmann::json statesJson;
    for (const auto& entry : states)
    {
        statesJson[std::to_string(entry.first)] = entry.second.toJson();
    }
    request["lightstates"] = std::move(statesJson);
    return *this;
}

nlohmann::json CreateScene::getRequest() const
{
    return request;
}
} // namespace hueplusplus