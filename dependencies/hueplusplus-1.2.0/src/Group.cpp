#include "hueplusplus/Group.h"

#include "hueplusplus/HueExceptionMacro.h"

namespace hueplusplus
{
Group::Group(int id, const std::shared_ptr<APICache>& baseCache)
    : id(id), state(baseCache, std::to_string(id), baseCache->getRefreshDuration())
{ }

Group::Group(int id, const HueCommandAPI& commands, std::chrono::steady_clock::duration refreshDuration, const nlohmann::json& currentState)
    : id(id), state("/groups/" + std::to_string(id), commands, refreshDuration, currentState)
{
    // Initialize value if not null
    state.getValue();
}

void Group::refresh(bool force)
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

void Group::setRefreshDuration(std::chrono::steady_clock::duration refreshDuration)
{
    state.setRefreshDuration(refreshDuration);
}


int Group::getId() const
{
    return id;
}

std::string Group::getName() const
{
    return state.getValue().at("name").get<std::string>();
}

std::string Group::getType() const
{
    return state.getValue().at("type").get<std::string>();
}

std::vector<int> Group::getLightIds() const
{
    const nlohmann::json& lights = state.getValue().at("lights");
    std::vector<int> ids;
    ids.reserve(lights.size());
    for (const nlohmann::json& id : lights)
    {
        // Luminaires can have null ids if not all light have been added
        if (!id.is_null())
        {
            ids.push_back(std::stoi(id.get<std::string>()));
        }
    }
    return ids;
}

void Group::setName(const std::string& name)
{
    nlohmann::json request = {{"name", name}};
    sendPutRequest("", request, CURRENT_FILE_INFO);
    refresh(true);
}

void Group::setLights(const std::vector<int>& ids)
{
    nlohmann::json lights = nlohmann::json::array();
    for (int id : ids)
    {
        lights.push_back(std::to_string(id));
    }
    sendPutRequest("", {{"lights", lights}}, CURRENT_FILE_INFO);
    refresh(true);
}

bool Group::getAllOn()
{
    return state.getValue().at("state").at("all_on").get<bool>();
}
bool Group::getAllOn() const
{
    return state.getValue().at("state").at("all_on").get<bool>();
}

bool Group::getAnyOn()
{
    return state.getValue().at("state").at("any_on").get<bool>();
}
bool Group::getAnyOn() const
{
    return state.getValue().at("state").at("any_on").get<bool>();
}

bool Group::getActionOn()
{
    return state.getValue().at("action").at("on").get<bool>();
}
bool Group::getActionOn() const
{
    return state.getValue().at("action").at("on").get<bool>();
}

std::pair<uint16_t, uint8_t> Group::getActionHueSaturation()
{
    const nlohmann::json& action = state.getValue().at("action");

    return std::make_pair(action.at("hue").get<int>(), action.at("sat").get<int>());
}
std::pair<uint16_t, uint8_t> Group::getActionHueSaturation() const
{
    const nlohmann::json& action = state.getValue().at("action");

    return std::make_pair(action.at("hue").get<int>(), action.at("sat").get<int>());
}

unsigned int Group::getActionBrightness()
{
    return state.getValue().at("action").at("bri").get<int>();
}
unsigned int Group::getActionBrightness() const
{
    return state.getValue().at("action").at("bri").get<int>();
}

unsigned int Group::getActionColorTemperature()
{
    return state.getValue().at("action").at("ct").get<int>();
}
unsigned int Group::getActionColorTemperature() const
{
    return state.getValue().at("action").at("ct").get<int>();
}

std::pair<float, float> Group::getActionColorXY()
{
    const nlohmann::json& xy = state.getValue().at("action").at("xy");
    return std::pair<float, float>(xy[0].get<float>(), xy[1].get<float>());
}
std::pair<float, float> Group::getActionColorXY() const
{
    const nlohmann::json& xy = state.getValue().at("action").at("xy");
    return std::pair<float, float>(xy[0].get<float>(), xy[1].get<float>());
}

std::string Group::getActionColorMode()
{
    return state.getValue().at("action").at("colormode").get<std::string>();
}
std::string Group::getActionColorMode() const
{
    return state.getValue().at("action").at("colormode").get<std::string>();
}

StateTransaction Group::transaction()
{
    // Do not pass state, because it is not the state of ALL lights in the group
    return StateTransaction(state.getCommandAPI(), "/groups/" + std::to_string(id) + "/action", nullptr);
}

void Group::setOn(bool on, uint8_t transition)
{
    transaction().setOn(on).setTransition(transition).commit();
}

void Group::setBrightness(uint8_t brightness, uint8_t transition)
{
    transaction().setBrightness(brightness).setTransition(transition).commit();
}

void Group::setColor(const HueSaturation& hueSat, uint8_t transition)
{
    transaction().setColor(hueSat).setTransition(transition).commit();
}

void Group::setColor(const XYBrightness& xy, uint8_t transition)
{
    transaction().setColor(xy).setTransition(transition).commit();
}

void Group::setColorTemperature(unsigned int mired, uint8_t transition)
{
    transaction().setColorTemperature(mired).setTransition(transition).commit();
}

void Group::setColorLoop(bool on, uint8_t transition)
{
    transaction().setColorLoop(on).setTransition(transition);
}

void Group::setScene(const std::string& scene)
{
    sendPutRequest("/action", {{"scene", scene}}, CURRENT_FILE_INFO);
}

Action Group::createSceneAction(const std::string& scene) const
{
    const nlohmann::json command {{"method", "PUT"},
        {"address", state.getCommandAPI().combinedPath("/groups/" + std::to_string(id) + "/action")},
        {"body", {{"scene", scene}}}};
    return Action(command);
}

nlohmann::json Group::sendPutRequest(const std::string& subPath, const nlohmann::json& request, FileInfo fileInfo)
{
    return state.getCommandAPI().PUTRequest("/groups/" + std::to_string(id) + subPath, request, std::move(fileInfo));
}

std::string Group::getRoomType() const
{
    return state.getValue().at("class").get<std::string>();
}

void Group::setRoomType(const std::string& type)
{
    sendPutRequest("", {{"class", type}}, CURRENT_FILE_INFO);
    refresh(true);
}

std::string Group::getModelId() const
{
    return state.getValue().at("modelid").get<std::string>();
}

std::string Group::getUniqueId() const
{
    return state.getValue().at("uniqueid").get<std::string>();
}

CreateGroup CreateGroup::LightGroup(const std::vector<int>& lights, const std::string& name)
{
    return CreateGroup(lights, name, "LightGroup", "");
}

CreateGroup CreateGroup::Room(const std::vector<int>& lights, const std::string& name, const std::string& roomType)
{
    return CreateGroup(lights, name, "Room", roomType);
}

CreateGroup CreateGroup::Entertainment(const std::vector<int>& lights, const std::string& name)
{
    return CreateGroup(lights, name, "Entertainment", "");
}

CreateGroup CreateGroup::Zone(const std::vector<int>& lights, const std::string& name)
{
    return CreateGroup(lights, name, "Zone", "");
}

nlohmann::json CreateGroup::getRequest() const
{
    nlohmann::json lightStrings = nlohmann::json::array();
    for (int light : lights)
    {
        lightStrings.push_back(std::to_string(light));
    }
    nlohmann::json result = {{"lights", lightStrings}, {"type", type}};
    if (!name.empty())
    {
        result["name"] = name;
    }
    result["class"] = roomType.empty() ? "Other" : roomType;
    return result;
}

CreateGroup::CreateGroup(
    const std::vector<int>& lights, const std::string& name, const std::string& type, const std::string& roomType)
    : lights(lights), name(name), type(type), roomType(roomType)
{ }

} // namespace hueplusplus
