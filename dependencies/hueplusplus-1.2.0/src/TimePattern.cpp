/**
    \file TimePattern.cpp
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

#include <cstdlib>
#include <ctime>

#include <hueplusplus/HueExceptionMacro.h>
#include <hueplusplus/TimePattern.h>

namespace hueplusplus
{
namespace time
{
namespace
{
std::tm timestampToTm(const std::string& timestamp)
{
    try
    {
        std::tm tm {};
        tm.tm_year = std::stoi(timestamp.substr(0, 4)) - 1900;
        tm.tm_mon = std::stoi(timestamp.substr(5, 2)) - 1;
        tm.tm_mday = std::stoi(timestamp.substr(8, 2));
        tm.tm_hour = std::stoi(timestamp.substr(11, 2));
        tm.tm_min = std::stoi(timestamp.substr(14, 2));
        tm.tm_sec = std::stoi(timestamp.substr(17, 2));
        // Auto detect daylight savings time
        tm.tm_isdst = -1;
        return tm;
    }
    catch (const std::invalid_argument& e)
    {
        throw HueException(CURRENT_FILE_INFO, std::string("Invalid argument: ") + e.what());
    }
}
} // namespace

using std::chrono::system_clock;
// Full name needed for doxygen
std::string timepointToTimestamp(std::chrono::system_clock::time_point time)
{
    using namespace std::chrono;
    std::time_t ctime = system_clock::to_time_t(time);

    std::tm* pLocaltime = std::localtime(&ctime);
    if (pLocaltime == nullptr)
    {
        throw HueException(CURRENT_FILE_INFO, "localtime failed");
    }
    std::tm localtime = *pLocaltime;
    char buf[32];

    std::size_t result = std::strftime(buf, sizeof(buf), "%FT%T", &localtime);
    if (result == 0)
    {
        throw HueException(CURRENT_FILE_INFO, "strftime failed");
    }
    return std::string(buf);
}

system_clock::time_point parseTimestamp(const std::string& timestamp)
{
    std::tm tm = timestampToTm(timestamp);
    std::time_t ctime = std::mktime(&tm);
    if (ctime == -1)
    {
        throw HueException(CURRENT_FILE_INFO, "mktime failed");
    }
    return system_clock::from_time_t(ctime);
}

std::chrono::system_clock::time_point parseUTCTimestamp(const std::string& timestamp)
{
    std::tm tm = timestampToTm(timestamp);
#ifdef _MSC_VER
    std::time_t ctime = _mkgmtime(&tm);
#else
    // Non-standard, but POSIX compliant
    // (also not officially thread-safe, but none of the time functions are)
    // Set local timezone to UTC and then set it back
    char* tz = std::getenv("TZ");
    if (tz)
    {
        tz = strdup(tz);
    }
    setenv("TZ", "", 1);
    tzset();
    std::time_t ctime = std::mktime(&tm);
    if (tz)
    {
        setenv("TZ", tz, 1);
        free(tz);
    }
    else
    {
        unsetenv("TZ");
    }
    tzset();
#endif
    if (ctime == -1)
    {
        throw HueException(CURRENT_FILE_INFO, "timegm failed");
    }
    return system_clock::from_time_t(ctime);
}

// Full name needed for doxygen
std::string durationTo_hh_mm_ss(std::chrono::system_clock::duration duration)
{
    using namespace std::chrono;
    if (duration > hours(24))
    {
        throw HueException(CURRENT_FILE_INFO, "Duration parameter longer than 1 day");
    }
    unsigned int numH = static_cast<unsigned int>(duration_cast<hours>(duration).count());
    duration -= hours(numH);
    unsigned int numM = static_cast<unsigned int>(duration_cast<minutes>(duration).count());
    duration -= minutes(numM);
    unsigned int numS = static_cast<unsigned int>(duration_cast<seconds>(duration).count());

    char result[9];
    std::snprintf(result, 9, "%02u:%02u:%02u", numH, numM, numS);
    return std::string(result);
}

system_clock::duration parseDuration(const std::string& s)
{
    using namespace std::chrono;
    const hours hour(std::stoi(s.substr(0, 2)));
    const minutes min(std::stoi(s.substr(3, 2)));
    const seconds sec(std::stoi(s.substr(6, 2)));
    return hour + min + sec;
}

AbsoluteTime::AbsoluteTime(clock::time_point baseTime) : base(baseTime) { }

system_clock::time_point AbsoluteTime::getBaseTime() const
{
    return base;
}
std::string AbsoluteTime::toString() const
{
    return timepointToTimestamp(base);
}

AbsoluteTime AbsoluteTime::parse(const std::string& s)
{
    // Absolute time
    clock::time_point time = parseTimestamp(s);
    return AbsoluteTime(time);
}

AbsoluteTime AbsoluteTime::parseUTC(const std::string& s)
{
    // Absolute time
    clock::time_point time = parseUTCTimestamp(s);
    return AbsoluteTime(time);
}

AbsoluteVariedTime::AbsoluteVariedTime(clock::time_point baseTime, clock::duration variation)
    : AbsoluteTime(baseTime), variation(variation)
{ }

system_clock::duration AbsoluteVariedTime::getRandomVariation() const
{
    return variation;
}

AbsoluteVariedTime AbsoluteVariedTime::parse(const std::string& s)
{
    // Absolute time
    clock::time_point time = parseTimestamp(s);
    clock::duration variation {0};
    if (s.size() > 19 && s[19] == 'A')
    {
        // Random variation
        variation = parseDuration(s.substr(20));
    }
    return AbsoluteVariedTime(time, variation);
}

std::string AbsoluteVariedTime::toString() const
{
    std::string result = timepointToTimestamp(getBaseTime());
    if (variation.count() != 0)
    {
        result.push_back('A');
        result.append(durationTo_hh_mm_ss(variation));
    }
    return result;
}

bool Weekdays::isNone() const
{
    return bitmask == 0;
}

bool Weekdays::isAll() const
{
    // Check all 7 bits are set
    return bitmask == (1 << 7) - 1;
}

bool Weekdays::isMonday() const
{
    return (bitmask & 1) != 0;
}

bool Weekdays::isTuesday() const
{
    return (bitmask & 2) != 0;
}

bool Weekdays::isWednesday() const
{
    return (bitmask & 4) != 0;
}

bool Weekdays::isThursday() const
{
    return (bitmask & 8) != 0;
}

bool Weekdays::isFriday() const
{
    return (bitmask & 16) != 0;
}

bool Weekdays::isSaturday() const
{
    return (bitmask & 32) != 0;
}

bool Weekdays::isSunday() const
{
    return (bitmask & 64) != 0;
}

std::string Weekdays::toString() const
{
    std::string result = std::to_string(bitmask);
    if (result.size() < 3)
    {
        result.insert(0, 3 - result.size(), '0');
    }
    return result;
}

Weekdays Weekdays::unionWith(Weekdays other) const
{
    other.bitmask |= bitmask;
    return other;
}

Weekdays Weekdays::none()
{
    return Weekdays();
}

Weekdays Weekdays::all()
{
    Weekdays result;
    result.bitmask = (1 << 7) - 1;
    return result;
}

Weekdays Weekdays::monday()
{
    return Weekdays(0);
}

Weekdays Weekdays::tuesday()
{
    return Weekdays(1);
}

Weekdays Weekdays::wednesday()
{
    return Weekdays(2);
}

Weekdays Weekdays::thursday()
{
    return Weekdays(3);
}

Weekdays Weekdays::friday()
{
    return Weekdays(4);
}

Weekdays Weekdays::saturday()
{
    return Weekdays(5);
}

Weekdays Weekdays::sunday()
{
    return Weekdays(6);
}

Weekdays Weekdays::parse(const std::string& s)
{
    Weekdays result;
    result.bitmask = std::stoi(s);
    return result;
}

RecurringTime::RecurringTime(clock::duration daytime, Weekdays days, clock::duration variation)
    : time(daytime), variation(variation), days(days)
{ }

system_clock::duration RecurringTime::getDaytime() const
{
    return time;
}

system_clock::duration RecurringTime::getRandomVariation() const
{
    return variation;
}

Weekdays RecurringTime::getWeekdays() const
{
    return days;
}

std::string RecurringTime::toString() const
{
    std::string result = "W";
    result.append(days.toString());
    result.append("/T");
    result.append(durationTo_hh_mm_ss(time));
    if (variation.count() != 0)
    {
        result.push_back('A');
        result.append(durationTo_hh_mm_ss(variation));
    }
    return result;
}

TimeInterval::TimeInterval(clock::duration start, clock::duration end, Weekdays days)
    : start(start), end(end), days(days)
{ }

system_clock::duration TimeInterval::getStartTime() const
{
    return start;
}

system_clock::duration TimeInterval::getEndTime() const
{
    return end;
}

Weekdays TimeInterval::getWeekdays() const
{
    return days;
}

std::string TimeInterval::toString() const
{
    std::string result;
    if (!days.isAll())
    {
        result.append("W");
        result.append(days.toString());
        result.append("/");
    }
    result.push_back('T');
    result.append(durationTo_hh_mm_ss(start));
    result.append("/T");
    result.append(durationTo_hh_mm_ss(end));

    return result;
}

Timer::Timer(clock::duration duration, clock::duration variation)
    : expires(duration), variation(variation), numExecutions(1)
{ }

Timer::Timer(clock::duration duration, int numExecutions, clock::duration variation)
    : expires(duration), variation(variation), numExecutions(numExecutions)
{ }

bool Timer::isRecurring() const
{
    return numExecutions != 1;
}

int Timer::getNumberOfExecutions() const
{
    return numExecutions;
}

system_clock::duration Timer::getExpiryTime() const
{
    return expires;
}

system_clock::duration Timer::getRandomVariation() const
{
    return variation;
}

std::string Timer::toString() const
{
    std::string result;
    if (numExecutions != 1)
    {
        result.push_back('R');
        if (numExecutions != infiniteExecutions)
        {
            std::string s = std::to_string(numExecutions);
            // Pad to two digits
            if (s.size() < 2)
            {
                result.push_back('0');
            }
            result.append(s);
        }
        result.push_back('/');
    }
    result.append("PT");
    result.append(durationTo_hh_mm_ss(expires));
    if (variation.count() != 0)
    {
        result.push_back('A');
        result.append(durationTo_hh_mm_ss(variation));
    }
    return result;
}

TimePattern::TimePattern() : type(Type::undefined), undefined(nullptr) { }

TimePattern::~TimePattern()
{
    destroy();
}

TimePattern::TimePattern(const AbsoluteVariedTime& absolute) : type(Type::absolute), absolute(absolute) { }

TimePattern::TimePattern(const RecurringTime& recurring) : type(Type::recurring), recurring(recurring) { }

TimePattern::TimePattern(const TimeInterval& interval) : type(Type::interval), interval(interval) { }

TimePattern::TimePattern(const Timer& timer) : type(Type::timer), timer(timer) { }

TimePattern::TimePattern(const TimePattern& other) : type(Type::undefined), undefined(nullptr)
{
    *this = other;
}

TimePattern& TimePattern::operator=(const TimePattern& other)
{
    if (this == &other)
    {
        return *this;
    }
    destroy();
    try
    {
        type = other.type;
        switch (type)
        {
        case Type::undefined:
            undefined = nullptr;
            break;
        case Type::absolute:
            new (&absolute) AbsoluteTime(other.absolute);
            break;
        case Type::recurring:
            new (&recurring) RecurringTime(other.recurring);
            break;
        case Type::interval:
            new (&interval) TimeInterval(other.interval);
            break;
        case Type::timer:
            new (&timer) Timer(other.timer);
            break;
        }
    }
    catch (...)
    {
        // Catch any throws from constructors to stay in valid state
        type = Type::undefined;
        undefined = nullptr;
        throw;
    }
    return *this;
}

TimePattern::Type TimePattern::getType() const
{
    return type;
}

AbsoluteVariedTime TimePattern::asAbsolute() const
{
    return absolute;
}

RecurringTime TimePattern::asRecurring() const
{
    return recurring;
}

TimeInterval TimePattern::asInterval() const
{
    return interval;
}

Timer TimePattern::asTimer() const
{
    return timer;
}

std::string TimePattern::toString() const
{
    switch (type)
    {
    case Type::undefined:
        return std::string();
    case Type::absolute:
        return absolute.toString();
    case Type::recurring:
        return recurring.toString();
    case Type::interval:
        return interval.toString();
    case Type::timer:
        return timer.toString();
    default:
        throw HueException(CURRENT_FILE_INFO, "TimePattern has wrong type");
    }
}

TimePattern TimePattern::parse(const std::string& s)
{
    if (s.empty() || s == "none")
    {
        return TimePattern();
    }
    else if (std::isdigit(s.front()))
    {
        return TimePattern(AbsoluteVariedTime::parse(s));
    }
    else if (s.front() == 'R' || s.front() == 'P')
    {
        // (Recurring) timer
        int numRepetitions = 1;
        if (s.front() == 'R')
        {
            if (s.at(1) == '/')
            {
                // Infinite
                numRepetitions = 0;
            }
            else
            {
                numRepetitions = std::stoi(s.substr(1, 2));
            }
        }
        std::size_t start = s.find('T') + 1;
        std::size_t randomStart = s.find('A');
        system_clock::duration expires = parseDuration(s.substr(start, randomStart - start));
        system_clock::duration variance = std::chrono::seconds(0);
        if (randomStart != std::string::npos)
        {
            variance = parseDuration(s.substr(randomStart + 1));
        }
        return TimePattern(Timer(expires, numRepetitions, variance));
    }
    else if (s.front() == 'W' && std::count(s.begin(), s.end(), '/') == 1)
    {
        // Recurring time
        Weekdays days = Weekdays::parse(s.substr(1, 3));
        system_clock::duration time = parseDuration(s.substr(6));
        system_clock::duration variation {0};
        if (s.size() > 14)
        {
            variation = parseDuration(s.substr(15));
        }
        return TimePattern(RecurringTime(time, days, variation));
    }
    else if (s.front() == 'T' || s.front() == 'W')
    {
        Weekdays days = Weekdays::all();
        if (s.front() == 'W')
        {
            // Time interval with weekdays
            days = Weekdays::parse(s.substr(1, 3));
        }
        // Time interval
        std::size_t start = s.find('T') + 1;
        std::size_t end = s.find('/', start);
        system_clock::duration startTime = parseDuration(s.substr(start, end - start));
        system_clock::duration endTime = parseDuration(s.substr(end + 2));
        return TimePattern(TimeInterval(startTime, endTime, days));
    }
    throw HueException(CURRENT_FILE_INFO, "Unable to parse time string: " + s);
}

void TimePattern::destroy()
{
    switch (type)
    {
    case Type::absolute:
        absolute.~AbsoluteVariedTime();
        break;
    case Type::recurring:
        recurring.~RecurringTime();
        break;
    case Type::interval:
        interval.~TimeInterval();
        break;
    case Type::timer:
        timer.~Timer();
        break;
    default:
        // Do not throw exception, because it is called in destructor
        // just ignore
        break;
    }
    type = Type::undefined;
    undefined = nullptr;
}

} // namespace time
} // namespace hueplusplus
