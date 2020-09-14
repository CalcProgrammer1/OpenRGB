/**
    \file TimePattern.h
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

#ifndef INCLUDE_HUEPLUSPLUS_TIME_PATTERN
#define INCLUDE_HUEPLUSPLUS_TIME_PATTERN

#include <chrono>
#include <string>
#include <cstddef>

namespace hueplusplus
{
//! \brief Namespace for time/date related classes and functions
namespace time
{
//! \brief Converts a time_point to a timestamp string
//! \param time Time to convert
//! \returns Date and time in the format
//! <code>YYYY-MM-DD</code><strong>T</strong><code>hh:mm:ss</code>.
//!
//! Returns the time in the local time zone.
//! \throws HueException when time could not be converted
std::string timepointToTimestamp(std::chrono::system_clock::time_point time);

//! \brief Converts a timestamp to a time_point
//! \param timestamp Timestamp from the local time zone in the format
//! <code>YYYY-MM-DD</code><strong>T</strong><code>hh:mm:ss</code>
//! \returns time_point of the local system clock
//! \throws std::invalid_argument when integer conversion fails
//! \throws HueException when time cannot be represented as time_point
std::chrono::system_clock::time_point parseTimestamp(const std::string& timestamp);

//! \brief Converts an UTC timestamp to a time_point
//! \param timestamp UTC Timestamp the format
//! <code>YYYY-MM-DD</code><strong>T</strong><code>hh:mm:ss</code>
//! \returns time_point of the local system clock
//! \throws std::invalid_argument when integer conversion fails
//! \throws HueException when time cannot be represented as time_point
std::chrono::system_clock::time_point parseUTCTimestamp(const std::string& timestamp);

//! \brief Converts duration to a time string
//! \param duration Duration or time of day to format. Must be less than 24 hours
//! \returns Duration string in the format <code>hh:mm:ss</code>
//! \throws HueException when \c duration longer than 24 hours.
std::string durationTo_hh_mm_ss(std::chrono::system_clock::duration duration);

//! \brief Converts time string to a duration
//! \param hourMinSec Time/duration in the format <code>hh:mm:ss</code>
//! \returns Duration (hours, minutes and seconds) from the string
//! \throws std::invalid_argument when integer conversion fails
std::chrono::system_clock::duration parseDuration(const std::string& hourMinSec);

//! \brief One-time, absolute time point
class AbsoluteTime
{
    using clock = std::chrono::system_clock;

public:
    //! \brief Create absolute time point
    //! \param baseTime Absolute time point
    explicit AbsoluteTime(clock::time_point baseTime);

    //! \brief Get base time point
    //!
    //! Can be used for calculation with other system_clock time_points
    clock::time_point getBaseTime() const;

    //! \brief Get formatted string as expected by Hue API
    //! \returns Timestamp in the format
    //! <code>YYYY-MM-DD</code><strong>T</strong><code>hh:mm:ss</code> in local timezone
    std::string toString() const;

    //! \brief Parse AbsoluteTime from formatted string in local timezone
    //! \param s Timestamp in the same format as returned by \ref toString()
    //! \returns AbsoluteTime with base time and variation from \c s
    static AbsoluteTime parse(const std::string& s);

    //! \brief Parse AbsoluteTime from formatted UTC string
    //! \param s Timestamp in the same format as returned by \ref toString()
    //! \returns AbsoluteTime with base time and variation from \c s
    static AbsoluteTime parseUTC(const std::string& s);

private:
    clock::time_point base;
};
//! One-time, absolute time point with possible random variation
//!
//! Can be either used to represent a specific date and time,
//! or a date and time with a random variation.
class AbsoluteVariedTime : public AbsoluteTime
{
    using clock = std::chrono::system_clock;

public:
    //! \brief Create absolute time point
    //! \param baseTime Absolute time point
    //! \param variation Random variation, optional. When not zero, the time is randomly chosen between
    //! <code>baseTime - variation</code> and <code>baseTime + variation</code>
    explicit AbsoluteVariedTime(clock::time_point baseTime, clock::duration variation = std::chrono::seconds(0));

    //! \brief Get random variation or zero
    //!
    //! The time can vary up to this amount in both directions.
    clock::duration getRandomVariation() const;

    //! \brief Get formatted string as expected by Hue API
    //! \returns when variation is 0: Timestamp in the format
    //! <code>YYYY-MM-DD</code><strong>T</strong><code>hh:mm:ss</code>
    //! \returns when there is variation: Timestamp in the format
    //! <code>YYYY-MM-DD</code><strong>T</strong><code>hh:mm:ss</code><strong>A</strong><code>hh:mm:ss</code>
    //! with base time first, variation second
    std::string toString() const;

    //! \brief Parse AbsoluteTime from formatted string in local timezone
    //! \param s Timestamp in the same format as returned by \ref toString()
    //! \returns AbsoluteVariedTime with base time and variation from \c s
    static AbsoluteVariedTime parse(const std::string& s);

private:
    clock::duration variation;
};

//! \brief Any number of days of the week
//!
//! Can be used to represent weekly repetitions only on certain days.
class Weekdays
{
public:
    //! \brief Create with no days
    Weekdays() : bitmask(0) { }
    //! \brief Create with the day \c num
    //! \param num Day of the week, from monday (0) to sunday (6)
    explicit Weekdays(int num) : bitmask(1 << num) { }

    //! \brief Check if no days are set
    bool isNone() const;
    //! \brief Check if all days are set
    bool isAll() const;
    //! \brief Check if Monday is contained
    bool isMonday() const;
    //! \brief Check if Tuesday is contained
    bool isTuesday() const;
    //! \brief Check if Wednesday is contained
    bool isWednesday() const;
    //! \brief Check if Thursday is contained
    bool isThursday() const;
    //! \brief Check if Friday is contained
    bool isFriday() const;
    //! \brief Check if Saturday is contained
    bool isSaturday() const;
    //! \brief Check if Sunday is contained
    bool isSunday() const;

    //! \brief Create set union with other Weekdays
    //! \param other Second set of days to combine with
    //! \returns A set of days containing all days of either \c this or \c other
    Weekdays unionWith(Weekdays other) const;
    //! \brief Create set union with other Weekdays
    //! \see unionWith
    Weekdays operator|(Weekdays other) const { return unionWith(other); }

    //! \brief Create a formatted, numeric string
    //! \returns A three digit code for the days as a bitmask
    std::string toString() const;

    //! \brief Creates an empty Weekdays
    static Weekdays none();
    //! \brief Creates set of all days
    static Weekdays all();
    //! \brief Creates Monday
    static Weekdays monday();
    //! \brief Creates Tuesday
    static Weekdays tuesday();
    //! \brief Creates Wednesday
    static Weekdays wednesday();
    //! \brief Creates Thursday
    static Weekdays thursday();
    //! \brief Creates Friday
    static Weekdays friday();
    //! \brief Creates Saturday
    static Weekdays saturday();
    //! \brief Creates Sunday
    static Weekdays sunday();

    //! \brief Parse from three digit code
    //! \param s Bitmask of days as a string
    //! \returns Parsed set of weekdays
    static Weekdays parse(const std::string& s);

    //! \brief Check whether all days are equal
    bool operator==(const Weekdays& other) const { return bitmask == other.bitmask; }
    //! \brief Check whether not all days are equal
    bool operator!=(const Weekdays& other) const { return bitmask != other.bitmask; }

private:
    int bitmask;
};

//! \brief Time repeated weekly to daily, with possible random variation.
//!
//! Can be used to represent a time on one or multiple days per week.
//! It can also have a random variation of up to 12 hours.
class RecurringTime
{
    using clock = std::chrono::system_clock;

public:
    //! \brief Create recurring time
    //! \param daytime Time of day, duration from the start of the day.
    //! \param days Days to repeat on, should not be Weekdays::none()
    //! \param variation Random variation, optional. Must be less than 12 hours. When not zero, the time is randomly
    //! chosen between <code>daytime - variation</code> and <code>daytime + variation</code>
    explicit RecurringTime(clock::duration daytime, Weekdays days, clock::duration variation = std::chrono::seconds(0));

    //! \brief Get time of day
    clock::duration getDaytime() const;
    //! \brief Get random variation
    //!
    //! The time can vary up to this amount in both directions.
    clock::duration getRandomVariation() const;
    //! \brief Get days on which the repetition will happen
    Weekdays getWeekdays() const;

    //! \brief Get formatted string as expected by Hue API
    //! \returns with no variation:
    //! <strong>W</strong><code>bbb</code><strong>/T</strong><code>hh:mm:ss</code>
    //! \returns with variation:
    //! <strong>W</strong><code>bbb</code><strong>/T</strong><code>hh:mm:ss</code><strong>A</strong><code>hh:mm:ss</code>,
    //! where daytime is first and variation is second.
    std::string toString() const;

private:
    clock::duration time;
    clock::duration variation;
    Weekdays days;
};

//! \brief Time interval repeated daily to weekly.
//!
//! Can be used to represent an interval of time on one or multiple days per week.
//! The maximum interval length is 23 hours.
class TimeInterval
{
    using clock = std::chrono::system_clock;

public:
    //! \brief Create time interval
    //! \param start Start time, duration from the start of the day
    //! \param end End time, duration from the start of the day
    //! \param days Active days, optional. Defaults to daily repetition.
    TimeInterval(clock::duration start, clock::duration end, Weekdays days = Weekdays::all());

    //! \brief Get start time of the interval
    clock::duration getStartTime() const;
    //! \brief Get end time of the interval
    clock::duration getEndTime() const;
    //! \brief Get active days
    Weekdays getWeekdays() const;

    //! \brief Get formatted string as expected by Hue API
    //! \returns with daily repetition:
    //! <strong>T</strong><code>hh:mm:ss</code><strong>/T</strong><code>hh:mm:ss</code>,
    //! with start time first and end time second.
    //! \returns with repetition that is not daily:
    //! <strong>W</strong><code>bbb</code><strong>/T</strong><code>hh:mm:ss</code><strong>/T</strong><code>hh:mm:ss</code>
    std::string toString() const;

private:
    clock::duration start;
    clock::duration end;
    Weekdays days;
};

//! \brief Timer that is started and triggers after specified delay
//!
//! The timer can have a random variation in the expiry time.
//! It can be one-off, repeated a set number of times or repeated indefinitely.
class Timer
{
    using clock = std::chrono::system_clock;

public:
    // \brief Used to represent infinite repetitions
    static constexpr int infiniteExecutions = 0;

    //! \brief Create one-off timer
    //! \param duration Expiry time of the timer, max 24 hours.
    //! \param variation Random variation of expiry time, optional.
    Timer(clock::duration duration, clock::duration variation = std::chrono::seconds(0));
    //! \brief Create a repeated timer.
    //! \param duration Expiry time of the timer, max 24 hours.
    //! \param numExecutions Number of executions, 1 or higher, or \ref infiniteExecutions to always repeat.
    //! \param variation Random variation of expiry time, optional.
    Timer(clock::duration duration, int numExecutions, clock::duration variation = std::chrono::seconds(0));

    //! \brief Returns true when the timer is executed more than once
    bool isRecurring() const;

    //! \brief Get number of executions
    //! \returns Number of executions, or \ref infiniteExecutions
    int getNumberOfExecutions() const;
    //! \brief Get expiry time
    clock::duration getExpiryTime() const;
    //! \brief Get random variation of expiry time
    //!
    //! The expiry time can vary up to this value in both directions.
    clock::duration getRandomVariation() const;

    //! \brief Get formatted string as expected by Hue API
    //! \returns one-off timer: <strong>PT</strong><code>hh:mm:ss</code>
    //! \returns one-off timer with variation:
    //! <strong>PT</strong><code>hh:mm:ss</code><strong>A</strong><code>hh:mm:ss</code>,
    //! with expiry time first and variation second.
    //! \returns recurring timer: <strong>R/PT</strong><code>hh:mm:ss</code>
    //! \returns recurring timer with n repetitions:
    //! <strong>R</strong><code>nn</code><strong>/PT</strong><code>hh:mm:ss</code>
    //! \returns recurring timer with random variation:
    //! <strong>R</strong><code>nn</code><strong>/PT</strong><code>hh:mm:ss</code><strong>A</strong><code>hh:mm:ss</code>
    //! \returns infinite recurring timer with random variation:
    //! <strong>R</strong><strong>/PT</strong><code>hh:mm:ss</code><strong>A</strong><code>hh:mm:ss</code>
    std::string toString() const;

private:
    clock::duration expires;
    clock::duration variation;
    int numExecutions;
};

//! \brief Holds different time representations
//!
//! Holds either AbsoluteTime, RecurringTime, TimeInterval, Timer or an undefined state.
//! TimePattern is used to specify the occurrance of Schedule%s.
class TimePattern
{
public:
    //! \brief Currently active type
    enum class Type
    {
        undefined, //!< \brief No active type
        absolute, //!< \brief Active type is AbsoluteVariedTime
        recurring, //!< \brief Active type is RecurringTime
        interval, //!< \brief Active type is TimeInterval
        timer //!< \brief Active type is Timer
    };

    //! \brief Create empty TimePattern
    TimePattern();
    //! \brief Destructor for union.
    ~TimePattern();
    //! \brief Create TimePattern from AbsoluteVariedTime
    explicit TimePattern(const AbsoluteVariedTime& absolute);
    //! \brief Create TimePattern from RecurringTime
    explicit TimePattern(const RecurringTime& recurring);
    //! \brief Create TimePattern from TimeInterval
    explicit TimePattern(const TimeInterval& interval);
    //! \brief Create TimePattern from Timer
    explicit TimePattern(const Timer& timer);

    //! \brief Copy constructor for union
    TimePattern(const TimePattern& other);

    //! \brief Copy assignment for union
    TimePattern& operator=(const TimePattern& other);

    //! \brief Get currently active type
    //! \note Only the currently active type may be accessed,
    //! anything else is undefined behavior.
    Type getType() const;

    //! \brief Get contained absolute time
    //! \pre getType() == Type::absolute
    AbsoluteVariedTime asAbsolute() const;

    //! \brief Get contained recurring time
    //! \pre getType() == Type::recurring
    RecurringTime asRecurring() const;

    //! \brief Get contained time interval
    //! \pre getType() == Type::interval
    TimeInterval asInterval() const;

    //! \brief Get contained timer
    //! \pre getType() == Type::timer
    Timer asTimer() const;

    //! \brief Get formatted string of the contained value as expected by Hue API
    //! \returns Empty string when type is undefined, otherwise toString() of the active type.
    //! \see AbsoluteTime::toString, RecurringTime::toString, TimeInterval::toString, Timer::toString
    std::string toString() const;

    //! \brief Parses TimePattern from formatted string as returned by Hue API
    //! \param s Empty string, "none", or in one of the formats the contained types
    //! return in their toString() method.
    //! \returns TimePattern with the matching type that is given in \c s
    //! \see AbsoluteTime::toString, RecurringTime::toString, TimeInterval::toString, Timer::toString
    //! \throws HueException when the format does not match or a parsing error occurs
    //! \throws std::invalid_argument when an integer conversion fails
    static TimePattern parse(const std::string& s);

private:
    //! \brief Calls destructor of active union member
    void destroy();

private:
    Type type;
    union
    {
        std::nullptr_t undefined;
        AbsoluteVariedTime absolute;
        RecurringTime recurring;
        TimeInterval interval;
        Timer timer;
    };
};
} // namespace time
} // namespace hueplusplus

#endif