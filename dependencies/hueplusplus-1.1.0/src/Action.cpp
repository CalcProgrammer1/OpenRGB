/**
    \file Action.cpp
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

#include <hueplusplus/Action.h>
#include <hueplusplus/HueExceptionMacro.h>

namespace hueplusplus
{

Action::Action(const nlohmann::json& json) : json(json) { }

std::string Action::getAddress() const
{
    return json.at("address").get<std::string>();
}

Action::Method Action::getMethod() const
{
    return parseMethod(json.at("method").get<std::string>());
}

const nlohmann::json& Action::getBody() const
{
    return json.at("body");
}

const nlohmann::json& Action::toJson() const
{
    return json;
}

Action::Method Action::parseMethod(const std::string& s)
{
    if (s == "POST")
    {
        return Method::post;
    }
    else if (s == "PUT")
    {
        return Method::put;
    }
    else if (s == "DELETE")
    {
        return Method::deleteMethod;
    }
    throw HueException(CURRENT_FILE_INFO, "Unknown ScheduleCommand method: " + s);
}

std::string Action::methodToString(Method m)
{
    switch (m)
    {
    case Method::post:
        return "POST";
    case Method::put:
        return "PUT";
    case Method::deleteMethod:
        return "DELETE";
    default:
        throw HueException(
            CURRENT_FILE_INFO, "Unknown ScheduleCommand method enum: " + std::to_string(static_cast<int>(m)));
    }
}

} // namespace hueplusplus