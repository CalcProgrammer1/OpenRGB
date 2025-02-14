/**
    \file BaseDevice.cpp
    Copyright Notice\n
    Copyright (C) 2020  Stefan Herbrechtsmeier - developer\n

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

#include "hueplusplus/BaseDevice.h"

#include <cmath>
#include <iostream>
#include <thread>

#include "hueplusplus/HueExceptionMacro.h"
#include "hueplusplus/Utils.h"
#include <nlohmann/json.hpp>

namespace hueplusplus
{
int BaseDevice::getId() const
{
    return id;
}

std::string BaseDevice::getType() const
{
    return state.getValue().at("type").get<std::string>();
}

std::string BaseDevice::getName()
{
    return state.getValue().at("name").get<std::string>();
}

std::string BaseDevice::getName() const
{
    return state.getValue().at("name").get<std::string>();
}

std::string BaseDevice::getModelId() const
{
    return state.getValue().at("modelid").get<std::string>();
}

std::string BaseDevice::getUId() const
{
    return state.getValue().value("uniqueid", "");
}

std::string BaseDevice::getManufacturername() const
{
    return state.getValue().value("manufacturername", "");
}

std::string BaseDevice::getProductname() const
{
    return state.getValue().value("productname", "");
}

std::string BaseDevice::getSwVersion()
{
    return state.getValue().at("swversion").get<std::string>();
}

std::string BaseDevice::getSwVersion() const
{
    return state.getValue().at("swversion").get<std::string>();
}

bool BaseDevice::setName(const std::string& name)
{
    nlohmann::json request = {{"name", name}};
    nlohmann::json reply = sendPutRequest("/name", request, CURRENT_FILE_INFO);

    // Check whether request was successful (returned name is not necessarily the actually set name)
    // If it already exists, a number is added, if it is too long to be returned, "Updated" is returned
    return utils::safeGetMember(reply, 0, "success", "/lights/" + std::to_string(id) + "/name").is_string();
}

BaseDevice::BaseDevice(int id, const std::shared_ptr<APICache>& baseCache)
    : id(id), state(baseCache, std::to_string(id), baseCache->getRefreshDuration())
{ }

BaseDevice::BaseDevice(
    int id, const HueCommandAPI& commands, const std::string& path, std::chrono::steady_clock::duration refreshDuration, const nlohmann::json& currentState)
    : id(id), state(path + std::to_string(id), commands, refreshDuration, currentState)
{
    // Initialize value if not null
    state.getValue();
}

nlohmann::json BaseDevice::sendPutRequest(const std::string& subPath, const nlohmann::json& request, FileInfo fileInfo)
{
    return state.getCommandAPI().PUTRequest(state.getRequestPath() + subPath, request, std::move(fileInfo));
}

void BaseDevice::refresh(bool force)
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

void BaseDevice::setRefreshDuration(std::chrono::steady_clock::duration refreshDuration)
{
    state.setRefreshDuration(refreshDuration);
}

} // namespace hueplusplus
