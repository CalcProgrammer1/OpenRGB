/**
    \file Rule.h
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

#ifndef INCLUDE_HUEPLUSPLUS_RULE_H
#define INCLUDE_HUEPLUSPLUS_RULE_H

#include <string>

#include "APICache.h"
#include "Action.h"
#include "Condition.h"
#include "HueCommandAPI.h"
#include "TimePattern.h"

namespace hueplusplus
{

//! \brief Rule stored in the bridge.
//!
//! Rules are used to automatically trigger Action%s when certain events happen.
//! The bridge can only support a limited number of rules, conditions and actions.
//!
//! They are deactivated if any errors occur when they are evaluated.
class Rule
{
public:
    //! \brief Creates rule with shared cache
    //! \param id Rule id in the bridge
    //! \param baseCache Cache of the rule list.
    Rule(int id, const std::shared_ptr<APICache>& baseCache);
    //! \brief Creates rule with id
    //! \param id Rule id in the bridge
    //! \param commands HueCommandAPI for requests
    //! \param refreshDuration Time between refreshing the cached state.
    //! \param currentState The current state, may be null.
    Rule(int id, const HueCommandAPI& commands, std::chrono::steady_clock::duration refreshDuration, const nlohmann::json& currentState);

    //! \brief Refreshes internal cached state.
    //! \param force \c true forces a refresh, regardless of how long the last refresh was ago.
    //! \c false to only refresh when enough time has passed (needed e.g. when calling only const methods).
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void refresh(bool force = false);

    //! \brief Sets custom refresh interval for this rule.
    //! \param refreshDuration The new minimum duration between refreshes. May be 0 or \ref c_refreshNever.
    void setRefreshDuration(std::chrono::steady_clock::duration refreshDuration);

    //! \brief Get rule identifier
    int getId() const;

    //! \brief Get rule name
    //!
    //! The rule name is always unique for the bridge.
    std::string getName() const;

    //! \brief Set rule name.
    //! \param name New name for the rule.
    //! Must be unique for all rules, otherwise a number is added.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setName(const std::string& name);

    //! \brief Get created time
    time::AbsoluteTime getCreated() const;

    //! \brief Get time the rule was last triggered
    time::AbsoluteTime getLastTriggered() const;

    //! \brief Get the number of times the rule was triggered
    int getTimesTriggered() const;

    //! \brief Get whether rule is enabled or disabled
    bool isEnabled() const;
    //! \brief Enable or disable rule.
    //! \param enabled whether the rule is triggered.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setEnabled(bool enabled);

    //! \brief Get user that created or last changed the rule.
    std::string getOwner() const;

    //! \brief Get the conditions that have to be met
    //!
    //! The rule triggers the actions when all conditions are true.
    //! At least one condition must exist.
    std::vector<Condition> getConditions() const;
    //! \brief Get the actions that are executed
    //!
    //! At least one action must exist.
    std::vector<Action> getActions() const;

    //! \brief Set conditions for the rule
    //! \param conditions All conditions that need to be fulfilled. Must not be empty.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setConditions(const std::vector<Condition>& conditions);
    //! \brief Set actions for the rule
    //! \param actions The actions that are triggered when the conditions are met.
    //! Must not be empty.
    void setActions(const std::vector<Action>& actions);

private:
    //! \brief Utility function to send a put request to the group.
    //!
    //! \param request The request to send
    //! \param fileInfo FileInfo from calling function for exception details.
    //! \returns The parsed reply
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    nlohmann::json sendPutRequest(const nlohmann::json& request, FileInfo fileInfo);

private:
    int id;
    APICache state;
};

//! \brief Parameters for creating a new Rule.
//!
//! Can be used like a builder object with chained calls.
class CreateRule
{
public:
    //! \brief Construct with necessary parameters
    //! \param conditions Conditions for the rule. Must not be empty
    //! \param actions Actions for the rule. Must not be empty
    CreateRule(const std::vector<Condition>& conditions, const std::vector<Action>& actions);
    //! \brief Set name
    //! \see Rule::setName
    CreateRule& setName(const std::string& name);

    //! \brief Set status
    //! \see Rule::setEnabled
    CreateRule& setStatus(bool enabled);

    //! \brief Get request to create the rule.
    //! \returns JSON request for a POST to create the new rule.
    nlohmann::json getRequest() const;

private:
    nlohmann::json request;
};

} // namespace hueplusplus

#endif
