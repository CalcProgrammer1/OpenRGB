/**
    \file Schedule.h
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

#ifndef INCLUDE_HUEPLUSPLUS_SCHEDULE_H
#define INCLUDE_HUEPLUSPLUS_SCHEDULE_H

#include "APICache.h"
#include "TimePattern.h"

namespace hueplusplus
{
//! \brief Command executed on a Schedule
//!
//! The command makes either a POST, PUT or DELETE request with a given body
//! to an address on the bridge.
class ScheduleCommand
{
public:
    //! \brief Create ScheduleCommand from json
    //! \param json JSON object with address, method and body
    explicit ScheduleCommand(const nlohmann::json& json);

    //! \brief Method used for the command
    enum class Method
    {
        post, //!< POST request
        put, //!< PUT request
        deleteMethod //!< DELETE request
    };

    //! \brief Get address the request is made to
    std::string getAddress() const;
    //! \brief Get request method
    Method getMethod() const;
    //! \brief Get request body
    const nlohmann::json& getBody() const;

    //! \brief Get json object of command
    const nlohmann::json& toJson() const;

private:
    //! \brief Parse Method from string
    //! \param s \c POST, \c PUT or \c DELETE
    static Method parseMethod(const std::string& s);
    //! \brief Get string from Method
    //! \returns \c POST, \c PUT or \c DELETE
    static std::string methodToString(Method m);

private:
    nlohmann::json json;
};

//! \brief Schedule stored in the bridge
//!
//! A schedule can be created by the user to trigger actions at specific times.
class Schedule
{
public:
    //! \brief Enabled status of the Schedule
    enum class Status
    {
        disabled, //!< Schedule is disabled
        enabled //!< Schedule is enabled
    };

public:
    //! \brief Construct Schedule that exists in the bridge
    //! \param id Schedule ID
    //! \param commands HueCommandAPI for requests
    //! \param refreshDuration Time between refreshing the cached state
    Schedule(int id, const HueCommandAPI& commands, std::chrono::steady_clock::duration refreshDuration);

    //! \brief Refreshes internal cached state
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void refresh();

    //! \brief Get schedule identifier
    int getId() const;

    //! \brief Get schedule name
    //!
    //! The schedule name is always unique for the bridge.
    std::string getName() const;
    //! \brief Get schedule description
    std::string getDescription() const;
    //! \brief Get schedule command
    ScheduleCommand getCommand() const;
    //! \brief Get time when the event(s) will occur
    //! \returns TimePattern in local timezone
    time::TimePattern getTime() const;
    //! \brief Get schedule enabled/disabled status
    Status getStatus() const;
    //! \brief Get autodelete
    //!
    //! When autodelete is set to true, the schedule is removed after it expires.
    //! Only for non-recurring schedules.
    bool getAutodelete() const;
    //! \brief Get created time
    //! \returns AbsoluteTime without variation
    time::AbsoluteTime getCreated() const;
    //! \brief Get start time for timers
    //! \returns AbsoluteTime without variation when the timer was started.
    //! \throws nlohmann::json::out_of_range when the schedule does not have a start time
    time::AbsoluteTime getStartTime() const;

    //! \brief Set schedule name
    //! \param name New name for the schedule. Max size is 32.
    //! Must be unique for all schedules, otherwise a number is added.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setName(const std::string& name);
    //! \brief Set schedule description
    //! \param description New description, may be empty. Max size is 64.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setDescription(const std::string& description);
    //! \brief Set schedule command
    //! \param command New command that is executed when the time event occurs.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setCommand(const ScheduleCommand& command);
    //! \brief Set new time when the event will occur
    //! \param timePattern Any possible value of TimePattern
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setTime(const time::TimePattern& timePattern);
    //! \brief Enable or disable schedule
    //! \param status Enabled or disabled
    //!
    //! Can be used to reset a timer by setting to disabled and enabled again.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setStatus(Status status);
    //! \brief Set autodelete
    //! \param autodelete Whether to delete the schedule after it expires
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setAutodelete(bool autodelete);

private:
    //! \brief Utility function to send put request to the schedule.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void sendPutRequest(const nlohmann::json& request, FileInfo fileInfo);

private:
    int id;
    APICache state;
};

//! \brief Parameters for creating a new Schedule.
//!
//! Can be used like a builder object with chained calls.
class CreateSchedule
{
public:
    //! \brief Set name
    //! \see Schedule::setName
    CreateSchedule& setName(const std::string& name);
    //! \brief Set description
    //! \see Schedule::setDescription
    CreateSchedule& setDescription(const std::string& description);
    //! \brief Set command
    //! \see Schedule::setCommand
    CreateSchedule& setCommand(const ScheduleCommand& command);
    //! \brief Set time
    //! \see Schedule::setTime
    CreateSchedule& setTime(const time::TimePattern& time);
    //! \brief Set status
    //! \see Schedule::setStatus
    CreateSchedule& setStatus(Schedule::Status status);
    //! \brief Set autodelete
    //! \see Schedule::setAutodelete
    CreateSchedule& setAutodelete(bool autodelete);
    //! \brief Set recycle
    //!
    //! When recycle is true, it is deleted when no resourcelinks refer to it.
    CreateSchedule& setRecycle(bool recycle);

    //! \brief Get request to create the schedule.
//! \returns JSON request for a POST to create the new schedule
    nlohmann::json getRequest() const;

private:
    nlohmann::json request;
};

} // namespace hueplusplus

#endif
