/**
    \file Schedule.cpp
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
#include <hueplusplus/Schedule.h>

namespace hueplusplus
{

ScheduleCommand::ScheduleCommand(const nlohmann::json& json) : json(json) {}

std::string ScheduleCommand::getAddress() const
{
    return json.at("address").get<std::string>();
}

ScheduleCommand::Method ScheduleCommand::getMethod() const
{
    return parseMethod(json.at("method").get<std::string>());
}

const nlohmann::json& ScheduleCommand::getBody() const
{
    return json.at("body");
}

const nlohmann::json& ScheduleCommand::toJson() const
{
    return json;
}

ScheduleCommand::Method ScheduleCommand::parseMethod(const std::string& s)
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

std::string ScheduleCommand::methodToString(Method m)
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

Schedule::Schedule(int id, const HueCommandAPI& commands, std::chrono::steady_clock::duration refreshDuration)
    : id(id), state("/schedules/" + std::to_string(id), commands, refreshDuration)
{
    state.refresh();
}

void Schedule::refresh()
{
    state.refresh();
}

int Schedule::getId() const
{
    return id;
}

std::string Schedule::getName() const
{
    return state.getValue().at("name").get<std::string>();
}

std::string Schedule::getDescription() const
{
    return state.getValue().at("description").get<std::string>();
}

ScheduleCommand Schedule::getCommand() const
{
    return ScheduleCommand(state.getValue().at("command"));
}

time::TimePattern Schedule::getTime() const
{
    return time::TimePattern::parse(state.getValue().at("localtime").get<std::string>());
    // time requires UTC parsing, which is not yet supported
    // return time::TimePattern::parse(state.getValue().at("time").get<std::string>());
}

Schedule::Status Schedule::getStatus() const
{
    if (state.getValue().at("status").get<std::string>() == "enabled")
    {
        return Status::enabled;
    }
    return Status::disabled;
}

bool Schedule::getAutodelete() const
{
    return state.getValue().at("autodelete").get<bool>();
}

time::AbsoluteTime Schedule::getCreated() const
{
    return time::AbsoluteTime::parse(state.getValue().at("created").get<std::string>());
}

time::AbsoluteTime Schedule::getStartTime() const
{
    return time::AbsoluteTime::parse(state.getValue().at("starttime").get<std::string>());
}

void Schedule::setName(const std::string& name)
{
    sendPutRequest({{"name", name}}, CURRENT_FILE_INFO);
    refresh();
}

void Schedule::setDescription(const std::string& description)
{
    sendPutRequest({{"description", description}}, CURRENT_FILE_INFO);
    refresh();
}

void Schedule::setCommand(const ScheduleCommand& command)
{
    sendPutRequest({{"command", command.toJson()}}, CURRENT_FILE_INFO);
    refresh();
}

void Schedule::setTime(const time::TimePattern& timePattern)
{
    // if (state.getValue().count("localtime"))
    //{
    sendPutRequest({{"localtime", timePattern.toString()}}, CURRENT_FILE_INFO);
    // Time requires UTC time, which is not yet supported
    //}
    // else
    //{
    //    sendPutRequest({{"time", timePattern.toString()}}, CURRENT_FILE_INFO);
    //}
    refresh();
}

void Schedule::setStatus(Status status)
{
    sendPutRequest({{"status", status == Status::enabled ? "enabled" : "disabled"}}, CURRENT_FILE_INFO);
    refresh();
}

void Schedule::setAutodelete(bool autodelete)
{
    sendPutRequest({{"autodelete", autodelete}}, CURRENT_FILE_INFO);
    refresh();
}

void Schedule::sendPutRequest(const nlohmann::json& request, FileInfo fileInfo)
{
    state.getCommandAPI().PUTRequest("/schedules/" + std::to_string(id), request, std::move(fileInfo));
}

CreateSchedule& CreateSchedule::setName(const std::string& name)
{
    request["name"] = name;
    return *this;
}

CreateSchedule& CreateSchedule::setDescription(const std::string& description)
{
    request["description"] = description;
    return *this;
}

CreateSchedule& CreateSchedule::setCommand(const ScheduleCommand& command)
{
    request["command"] = command.toJson();
    return *this;
}

CreateSchedule& CreateSchedule::setTime(const time::TimePattern& time)
{
    request["localtime"] = time.toString();
    return *this;
}

CreateSchedule& CreateSchedule::setStatus(Schedule::Status status)
{
    request["status"] = (status == Schedule::Status::enabled) ? "enabled" : "disabled";
    return *this;
}

CreateSchedule& CreateSchedule::setAutodelete(bool autodelete)
{
    request["autodelete"] = autodelete;
    return *this;
}

CreateSchedule& CreateSchedule::setRecycle(bool recycle)
{
    request["recycle"] = recycle;
    return *this;
}

nlohmann::json CreateSchedule::getRequest() const
{
    return request;
}
} // namespace hueplusplus