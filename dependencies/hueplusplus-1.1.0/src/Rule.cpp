/**
    \file Rule.cpp
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
#include <hueplusplus/Rule.h>

namespace hueplusplus
{
Condition::Condition(const std::string& address, Operator op, const std::string& value)
    : address(address), op(op), value(value)
{ }
std::string Condition::getAddress() const
{
    return address;
}
Condition::Operator Condition::getOperator() const
{
    return op;
}
std::string Condition::getValue() const
{
    return value;
}

nlohmann::json Condition::toJson() const
{
    std::string opStr;
    switch (op)
    {
    case Operator::eq:
        opStr = "eq";
        break;
    case Operator::gt:
        opStr = "gt";
        break;
    case Operator::lt:
        opStr = "lt";
        break;
    case Operator::dx:
        opStr = "dx";
        break;
    case Operator::ddx:
        opStr = "ddx";
        break;
    case Operator::stable:
        opStr = "stable";
        break;
    case Operator::notStable:
        opStr = "not stable";
        break;
    case Operator::in:
        opStr = "in";
        break;
    case Operator::notIn:
        opStr = "not in";
        break;
    default:
        throw HueException(CURRENT_FILE_INFO, "Invalid operator enum value: " + std::to_string(static_cast<int>(op)));
    }

    nlohmann::json result = {{"address", address}, {"operator", opStr}};
    if (!value.empty())
    {
        result["value"] = value;
    }
    return result;
}

Condition Condition::parse(const nlohmann::json& json)
{
    std::string address = json.at("address").get<std::string>();
    std::string value = json.value("value", "");
    std::string opStr = json.at("operator").get<std::string>();
    Operator op = Operator::eq;
    if (opStr == "gt")
    {
        op = Operator::gt;
    }
    else if (opStr == "lt")
    {
        op = Operator::lt;
    }
    else if (opStr == "dx")
    {
        op = Operator::dx;
    }
    else if (opStr == "ddx")
    {
        op = Operator::ddx;
    }
    else if (opStr == "stable")
    {
        op = Operator::stable;
    }
    else if (opStr == "not stable")
    {
        op = Operator::notStable;
    }
    else if (opStr == "in")
    {
        op = Operator::in;
    }
    else if (opStr == "not in")
    {
        op = Operator::notIn;
    }
    else if (opStr != "eq")
    {
        throw HueException(CURRENT_FILE_INFO, "Unknown condition operator: " + opStr);
    }

    return Condition(address, op, value);
}

Rule::Rule(int id, const std::shared_ptr<APICache>& baseCache)
    : id(id), state(baseCache, std::to_string(id), baseCache->getRefreshDuration())
{ }
Rule::Rule(int id, const HueCommandAPI& commands, std::chrono::steady_clock::duration refreshDuration,
    const nlohmann::json& currentState)
    : id(id), state("/rules/" + std::to_string(id), commands, refreshDuration, currentState)
{
    refresh();
}

void Rule::refresh(bool force)
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

void Rule::setRefreshDuration(std::chrono::steady_clock::duration refreshDuration)
{
    state.setRefreshDuration(refreshDuration);
}

int Rule::getId() const
{
    return id;
}

std::string Rule::getName() const
{
    return state.getValue().at("name").get<std::string>();
}

void Rule::setName(const std::string& name)
{
    nlohmann::json request = {{"name", name}};
    sendPutRequest(request, CURRENT_FILE_INFO);
    refresh(true);
}

time::AbsoluteTime Rule::getCreated() const
{
    return time::AbsoluteTime::parseUTC(state.getValue().at("created").get<std::string>());
}

time::AbsoluteTime Rule::getLastTriggered() const
{
    const std::string lasttriggered = state.getValue().value("lasttriggered", "none");
    if (lasttriggered.empty() || lasttriggered == "none")
    {
        return time::AbsoluteTime(std::chrono::system_clock::time_point(std::chrono::seconds(0)));
    }
    return time::AbsoluteTime::parseUTC(lasttriggered);
}

int Rule::getTimesTriggered() const
{
    return state.getValue().at("timestriggered").get<int>();
}

bool Rule::isEnabled() const
{
    return state.getValue().at("status").get<std::string>() == "enabled";
}

void Rule::setEnabled(bool enabled)
{
    sendPutRequest({{"status", enabled ? "enabled" : "disabled"}}, CURRENT_FILE_INFO);
    refresh(true);
}

std::string Rule::getOwner() const
{
    return state.getValue().at("owner").get<std::string>();
}

std::vector<Condition> Rule::getConditions() const
{
    std::vector<Condition> result;
    const nlohmann::json& conditions = state.getValue().at("conditions");
    for (const nlohmann::json& c : conditions)
    {
        result.emplace_back(Condition::parse(c));
    }
    return result;
}

std::vector<Action> Rule::getActions() const
{
    std::vector<Action> result;
    const nlohmann::json& actions = state.getValue().at("actions");
    for (const nlohmann::json& a : actions)
    {
        result.emplace_back(a);
    }
    return result;
}

void Rule::setConditions(const std::vector<Condition>& conditions)
{
    nlohmann::json json;
    for (const Condition& c : conditions)
    {
        json.push_back(c.toJson());
    }

    sendPutRequest({{"conditions", json}}, CURRENT_FILE_INFO);
    refresh(true);
}

void Rule::setActions(const std::vector<Action>& actions)
{
    nlohmann::json json;
    for (const Action& a : actions)
    {
        json.push_back(a.toJson());
    }

    sendPutRequest({{"actions", json}}, CURRENT_FILE_INFO);
    refresh(true);
}

nlohmann::json Rule::sendPutRequest(const nlohmann::json& request, FileInfo fileInfo)
{
    return state.getCommandAPI().PUTRequest("/rules/" + std::to_string(id), request, std::move(fileInfo));
}

CreateRule::CreateRule(const std::vector<Condition>& conditions, const std::vector<Action>& actions)
{
    nlohmann::json conditionsJson;
    for (const Condition& c : conditions)
    {
        conditionsJson.push_back(c.toJson());
    }
    request["conditions"] = conditionsJson;
    nlohmann::json actionsJson;
    for (const Action& a : actions)
    {
        actionsJson.push_back(a.toJson());
    }
    request["actions"] = actionsJson;
}

CreateRule& CreateRule::setName(const std::string& name)
{
    request["name"] = name;
    return *this;
}

CreateRule& CreateRule::setStatus(bool enabled)
{
    request["status"] = enabled ? "enabled" : "disabled";
    return *this;
}

nlohmann::json CreateRule::getRequest() const
{
    return request;
}

} // namespace hueplusplus