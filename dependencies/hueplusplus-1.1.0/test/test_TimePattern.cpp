/**
    \file test_TimePattern.cpp
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

#include <hueplusplus/HueException.h>
#include <hueplusplus/TimePattern.h>

#include <gtest/gtest.h>

using namespace hueplusplus::time;
using std::chrono::system_clock;
using namespace std::chrono_literals;

TEST(Time, parseTimestamp)
{
    std::tm tm {};
    tm.tm_year = 2020 - 1900;
    tm.tm_mon = 3 - 1;
    tm.tm_mday = 24;
    tm.tm_hour = 12;
    tm.tm_min = 45;
    tm.tm_sec = 0;
    // Auto detect daylight savings time
    tm.tm_isdst = -1;
    const std::time_t ctime = std::mktime(&tm);
    const auto timePoint = system_clock::from_time_t(ctime);
    EXPECT_EQ(timePoint, parseTimestamp("2020-03-24T12:45:00"));
}

TEST(Time, timepointToTimestamp)
{
    std::tm tm {};
    tm.tm_year = 2020 - 1900;
    tm.tm_mon = 3 - 1;
    tm.tm_mday = 24;
    tm.tm_hour = 12;
    tm.tm_min = 45;
    tm.tm_sec = 0;
    // Auto detect daylight savings time
    tm.tm_isdst = -1;
    const std::time_t ctime = std::mktime(&tm);
    const auto timePoint = system_clock::from_time_t(ctime);
    EXPECT_EQ("2020-03-24T12:45:00", timepointToTimestamp(timePoint));

    EXPECT_EQ(timePoint, parseTimestamp(timepointToTimestamp(timePoint)));
}

TEST(Time, parseDuration)
{
    EXPECT_EQ(1h + 24min + 1s, parseDuration("01:24:01"));
    EXPECT_EQ(22h + 59min + 49s, parseDuration("22:59:49"));
    EXPECT_EQ(0s, parseDuration("00:00:00"));
}

TEST(Time, durationTo_hh_mm_ss)
{
    EXPECT_EQ("00:00:00", durationTo_hh_mm_ss(0s));
    EXPECT_EQ("01:32:05", durationTo_hh_mm_ss(1h + 32min + 5s));
    EXPECT_EQ("20:45:13", durationTo_hh_mm_ss(20h + 45min + 13s));
    const auto duration = 20h + 45min + 13s;
    EXPECT_EQ(duration, parseDuration(durationTo_hh_mm_ss(duration)));
}

TEST(AbsoluteVariedTime, Constructor)
{
    system_clock::time_point now = system_clock::now();
    {
        AbsoluteVariedTime time(now);
        EXPECT_EQ(now, time.getBaseTime());
        EXPECT_EQ(0s, time.getRandomVariation());
    }
    system_clock::duration variation = 4h + 2min;
    {
        AbsoluteVariedTime time(now, variation);
        EXPECT_EQ(now, time.getBaseTime());
        EXPECT_EQ(variation, time.getRandomVariation());
    }
}

TEST(AbsoluteVariedTime, toString)
{
    const system_clock::time_point timePoint = parseTimestamp("2020-03-03T20:53:03");

    EXPECT_EQ("2020-03-03T20:53:03", AbsoluteVariedTime(timePoint).toString());

    const system_clock::duration noVariation = 0s;
    EXPECT_EQ("2020-03-03T20:53:03", AbsoluteVariedTime(timePoint, noVariation).toString());

    const system_clock::duration variation = 1h + 2min + 1s;
    EXPECT_EQ("2020-03-03T20:53:03A01:02:01", AbsoluteVariedTime(timePoint, variation).toString());
}

TEST(AbsoluteTime, parseUTC)
{
    AbsoluteTime absolute = AbsoluteTime::parseUTC("2020-03-03T20:53:03");
    std::time_t ctime = system_clock::to_time_t(absolute.getBaseTime());
    std::tm* pTm = std::gmtime(&ctime);
    ASSERT_NE(nullptr, pTm);
    std::tm tm = *pTm;
    EXPECT_EQ(2020 - 1900, tm.tm_year);
    EXPECT_EQ(3 - 1, tm.tm_mon);
    EXPECT_EQ(20, tm.tm_hour);
    EXPECT_EQ(53, tm.tm_min);
    EXPECT_EQ(3, tm.tm_sec);
}

TEST(Weekdays, Constructor)
{
    EXPECT_TRUE(Weekdays().isNone());
    EXPECT_TRUE(Weekdays(0).isMonday());
    EXPECT_TRUE(Weekdays(6).isSunday());
}

TEST(Weekdays, isXXX)
{
    Weekdays none = Weekdays::none();
    EXPECT_TRUE(none.isNone());
    EXPECT_FALSE(none.isAll());
    EXPECT_FALSE(none.isMonday());
    EXPECT_FALSE(none.isTuesday());
    EXPECT_FALSE(none.isWednesday());
    EXPECT_FALSE(none.isThursday());
    EXPECT_FALSE(none.isFriday());
    EXPECT_FALSE(none.isSaturday());
    EXPECT_FALSE(none.isSunday());

    Weekdays all = Weekdays::all();
    EXPECT_FALSE(all.isNone());
    EXPECT_TRUE(all.isAll());
    EXPECT_TRUE(all.isMonday());
    EXPECT_TRUE(all.isTuesday());
    EXPECT_TRUE(all.isWednesday());
    EXPECT_TRUE(all.isThursday());
    EXPECT_TRUE(all.isFriday());
    EXPECT_TRUE(all.isSaturday());
    EXPECT_TRUE(all.isSunday());

    // Test that for all days, only their own isXXX function is true
    std::vector<Weekdays> days {Weekdays::monday(), Weekdays::tuesday(), Weekdays::wednesday(), Weekdays::thursday(),
        Weekdays::friday(), Weekdays::saturday(), Weekdays::sunday()};
    using BoolGetter = bool (Weekdays::*)() const;
    std::vector<BoolGetter> getters {&Weekdays::isMonday, &Weekdays::isTuesday, &Weekdays::isWednesday,
        &Weekdays::isThursday, &Weekdays::isFriday, &Weekdays::isSaturday, &Weekdays::isSunday};
    for (int i = 0; i < days.size(); ++i)
    {
        Weekdays day = days[i];
        EXPECT_FALSE(day.isNone());
        EXPECT_FALSE(day.isAll());
        for (int j = 0; j < getters.size(); ++j)
        {
            EXPECT_EQ(j == i, (day.*getters[j])()) << "on Day " << i << ": getter for day " << j << " has wrong result";
        }
    }
}

TEST(Weekdays, unionWith)
{
    Weekdays day = Weekdays::monday().unionWith(Weekdays::saturday());
    EXPECT_TRUE(day.isMonday());
    EXPECT_TRUE(day.isSaturday());

    day = Weekdays::monday() | Weekdays::tuesday() | Weekdays::all();
    EXPECT_TRUE(day.isAll());
}

TEST(Weekdays, equals)
{
    EXPECT_EQ(Weekdays::monday(), Weekdays(0));
    EXPECT_EQ(Weekdays::none(), Weekdays());
    EXPECT_EQ(Weekdays::monday() | Weekdays::tuesday(), Weekdays::monday().unionWith(Weekdays::tuesday()));

    EXPECT_NE(Weekdays::none(), Weekdays(0));
    EXPECT_NE(Weekdays::all(), Weekdays::monday());
}

TEST(Weekdays, toString)
{
    EXPECT_EQ("001", Weekdays(0).toString());
    EXPECT_EQ("064", Weekdays(6).toString());
    EXPECT_EQ("112", (Weekdays(6) | Weekdays(5) | Weekdays(4)).toString());
}

TEST(RecurringTime, Constructor)
{
    {
        const auto time = 6h + 4min;
        const Weekdays days = Weekdays::all();
        const RecurringTime recurring(time, days);

        EXPECT_EQ(time, recurring.getDaytime());
        EXPECT_EQ(0s, recurring.getRandomVariation());
        EXPECT_EQ(days, recurring.getWeekdays());
    }
    {
        const auto time = 2h + 3min + 2s;
        const Weekdays days = Weekdays::monday() | Weekdays::friday();
        const auto variation = 40min;
        const RecurringTime recurring(time, days, variation);

        EXPECT_EQ(time, recurring.getDaytime());
        EXPECT_EQ(variation, recurring.getRandomVariation());
        EXPECT_EQ(days, recurring.getWeekdays());
    }
}

TEST(RecurringTime, toString)
{
    const auto time = 0h + 4min;
    const RecurringTime recurring(time, Weekdays::monday());
    EXPECT_EQ("W001/T00:04:00", recurring.toString());

    const RecurringTime variation(time, Weekdays::monday(), 1s);
    EXPECT_EQ("W001/T00:04:00A00:00:01", variation.toString());
}

TEST(TimeInterval, Constructor)
{
    {
        const auto start = 1h + 40min;
        const auto end = 11h + 25s;
        const TimeInterval interval(start, end);

        EXPECT_EQ(start, interval.getStartTime());
        EXPECT_EQ(end, interval.getEndTime());
        EXPECT_EQ(Weekdays::all(), interval.getWeekdays());
    }
    {
        const auto start = 0s;
        const auto end = 20h;
        const Weekdays days = Weekdays::friday() | Weekdays::saturday();
        const TimeInterval interval(start, end, days);
        EXPECT_EQ(start, interval.getStartTime());
        EXPECT_EQ(end, interval.getEndTime());
        EXPECT_EQ(days, interval.getWeekdays());
    }
}

TEST(TimeInterval, toString)
{
    {
        const TimeInterval interval(1h + 40min, 11h + 25s);
        EXPECT_EQ("T01:40:00/T11:00:25", interval.toString());
    }
    {
        const TimeInterval interval(0h, 20h + 1s, Weekdays::monday());
        EXPECT_EQ("W001/T00:00:00/T20:00:01", interval.toString());
    }
}

TEST(Timer, Constructor)
{
    {
        const auto duration = 1min + 20s;
        const Timer timer(duration);
        EXPECT_FALSE(timer.isRecurring());
        EXPECT_EQ(1, timer.getNumberOfExecutions());
        EXPECT_EQ(duration, timer.getExpiryTime());
        EXPECT_EQ(0s, timer.getRandomVariation());
    }
    {
        const auto duration = 1min + 20s;
        const auto variation = 1h;
        const Timer timer(duration, variation);
        EXPECT_FALSE(timer.isRecurring());
        EXPECT_EQ(1, timer.getNumberOfExecutions());
        EXPECT_EQ(duration, timer.getExpiryTime());
        EXPECT_EQ(variation, timer.getRandomVariation());
    }
    {
        const auto duration = 1min + 20s;
        const int num = 0;
        const Timer timer(duration, num);
        EXPECT_TRUE(timer.isRecurring());
        EXPECT_EQ(num, timer.getNumberOfExecutions());
        EXPECT_EQ(duration, timer.getExpiryTime());
        EXPECT_EQ(0s, timer.getRandomVariation());
    }
    {
        const auto duration = 1min + 20s;
        const int num = 10;
        const auto variation = 20min;
        const Timer timer(duration, num, variation);
        EXPECT_TRUE(timer.isRecurring());
        EXPECT_EQ(num, timer.getNumberOfExecutions());
        EXPECT_EQ(duration, timer.getExpiryTime());
        EXPECT_EQ(variation, timer.getRandomVariation());
    }
}

TEST(Timer, toString)
{
    {
        const Timer timer(1min + 20s);
        EXPECT_EQ("PT00:01:20", timer.toString());
    }
    {
        const Timer timer(1min + 20s, 1h);
        EXPECT_EQ("PT00:01:20A01:00:00", timer.toString());
    }
    {
        const Timer timer(1min + 20s, Timer::infiniteExecutions);
        EXPECT_EQ("R/PT00:01:20", timer.toString());
    }
    {
        const Timer timer(1min + 20s, 1);
        EXPECT_EQ("PT00:01:20", timer.toString());
    }
    {
        const Timer timer(1min + 20s, 15);
        EXPECT_EQ("R15/PT00:01:20", timer.toString());
    }
    {
        const Timer timer(1min + 20s, 5, 1h);
        EXPECT_EQ("R05/PT00:01:20A01:00:00", timer.toString());
    }
    {
        const Timer timer(1min + 20s, Timer::infiniteExecutions, 1h);
        EXPECT_EQ("R/PT00:01:20A01:00:00", timer.toString());
    }
}

TEST(TimePattern, Undefined)
{
    {
        TimePattern pattern;
        EXPECT_EQ(TimePattern::Type::undefined, pattern.getType());
    }
    {
        TimePattern pattern = TimePattern::parse("");
        EXPECT_EQ(TimePattern::Type::undefined, pattern.getType());
    }
    {
        TimePattern pattern = TimePattern::parse("none");
        EXPECT_EQ(TimePattern::Type::undefined, pattern.getType());
    }
    EXPECT_THROW(TimePattern::parse("bla"), hueplusplus::HueException);
}

TEST(TimePattern, CopyConstructor)
{
    {
        TimePattern pattern;
        TimePattern copy = pattern;
        EXPECT_EQ(TimePattern::Type::undefined, copy.getType());
    }
    {
        const AbsoluteVariedTime abs(system_clock::now());
        const TimePattern pattern(abs);
        const TimePattern copy(pattern);
        ASSERT_EQ(TimePattern::Type::absolute, copy.getType());
        EXPECT_EQ(abs.getBaseTime(), copy.asAbsolute().getBaseTime());
    }
    {
        const RecurringTime rec(12h + 30min, Weekdays::monday(), 1h);
        const TimePattern pattern(rec);
        const TimePattern copy(pattern);
        ASSERT_EQ(TimePattern::Type::recurring, copy.getType());
        EXPECT_EQ(rec.getDaytime(), copy.asRecurring().getDaytime());
        EXPECT_EQ(rec.getWeekdays(), copy.asRecurring().getWeekdays());
        EXPECT_EQ(rec.getRandomVariation(), copy.asRecurring().getRandomVariation());
    }
    {
        const TimeInterval interval(12h + 30min, 13h + 20min, Weekdays::friday());
        const TimePattern pattern(interval);
        const TimePattern copy(pattern);
        ASSERT_EQ(TimePattern::Type::interval, copy.getType());
        EXPECT_EQ(interval.getStartTime(), copy.asInterval().getStartTime());
        EXPECT_EQ(interval.getEndTime(), copy.asInterval().getEndTime());
        EXPECT_EQ(interval.getWeekdays(), copy.asInterval().getWeekdays());
    }
    {
        const Timer timer(1h + 30min, 5, 20s);
        const TimePattern pattern(timer);
        const TimePattern copy(pattern);
        ASSERT_EQ(TimePattern::Type::timer, copy.getType());
        EXPECT_EQ(timer.getExpiryTime(), copy.asTimer().getExpiryTime());
        EXPECT_EQ(timer.getRandomVariation(), copy.asTimer().getRandomVariation());
        EXPECT_EQ(timer.getNumberOfExecutions(), copy.asTimer().getNumberOfExecutions());
    }
}

TEST(TimePattern, Absolute)
{
    {
        const AbsoluteVariedTime abs(system_clock::now(), 20s);
        const TimePattern pattern(abs);
        ASSERT_EQ(TimePattern::Type::absolute, pattern.getType());
        EXPECT_EQ(abs.getBaseTime(), pattern.asAbsolute().getBaseTime());
        EXPECT_EQ(abs.getRandomVariation(), pattern.asAbsolute().getRandomVariation());
    }

    const system_clock::time_point timePoint = parseTimestamp("2020-03-03T20:53:03");
    {
        const TimePattern pattern = TimePattern::parse("2020-03-03T20:53:03");
        const AbsoluteVariedTime expected(timePoint);
        ASSERT_EQ(TimePattern::Type::absolute, pattern.getType());
        EXPECT_EQ(expected.getBaseTime(), pattern.asAbsolute().getBaseTime());
        EXPECT_EQ(expected.getRandomVariation(), pattern.asAbsolute().getRandomVariation());
    }
    {
        const system_clock::duration variation = 1h + 2min + 1s;
        const TimePattern pattern = TimePattern::parse("2020-03-03T20:53:03A01:02:01");
        const AbsoluteVariedTime expected(timePoint, variation);
        ASSERT_EQ(TimePattern::Type::absolute, pattern.getType());
        EXPECT_EQ(expected.getBaseTime(), pattern.asAbsolute().getBaseTime());
        EXPECT_EQ(expected.getRandomVariation(), pattern.asAbsolute().getRandomVariation());
    }
}

TEST(TimePattern, Recurring)
{
    {
        const RecurringTime rec(12h + 30min, Weekdays::monday(), 1h);
        const TimePattern pattern(rec);
        ASSERT_EQ(TimePattern::Type::recurring, pattern.getType());
        EXPECT_EQ(rec.getDaytime(), pattern.asRecurring().getDaytime());
        EXPECT_EQ(rec.getWeekdays(), pattern.asRecurring().getWeekdays());
        EXPECT_EQ(rec.getRandomVariation(), pattern.asRecurring().getRandomVariation());
    }
    {
        const TimePattern pattern = TimePattern::parse("W001/T12:30:00");
        const RecurringTime expected(12h + 30min, Weekdays::monday());

        ASSERT_EQ(TimePattern::Type::recurring, pattern.getType());
        EXPECT_EQ(expected.getDaytime(), pattern.asRecurring().getDaytime());
        EXPECT_EQ(expected.getWeekdays(), pattern.asRecurring().getWeekdays());
        EXPECT_EQ(expected.getRandomVariation(), pattern.asRecurring().getRandomVariation());
    }
    {
        const TimePattern pattern = TimePattern::parse("W001/T12:30:00A01:00:00");
        const RecurringTime expected(12h + 30min, Weekdays::monday(), 1h);

        ASSERT_EQ(TimePattern::Type::recurring, pattern.getType());
        EXPECT_EQ(expected.getDaytime(), pattern.asRecurring().getDaytime());
        EXPECT_EQ(expected.getWeekdays(), pattern.asRecurring().getWeekdays());
        EXPECT_EQ(expected.getRandomVariation(), pattern.asRecurring().getRandomVariation());
    }
}

TEST(TimePattern, Interval)
{
    {
        const TimeInterval interval(12h + 30min, 13h + 20min, Weekdays::friday());
        const TimePattern pattern(interval);
        ASSERT_EQ(TimePattern::Type::interval, pattern.getType());
        EXPECT_EQ(interval.getStartTime(), pattern.asInterval().getStartTime());
        EXPECT_EQ(interval.getEndTime(), pattern.asInterval().getEndTime());
        EXPECT_EQ(interval.getWeekdays(), pattern.asInterval().getWeekdays());
    }
    {
        const TimeInterval expected(12h + 30min, 13h + 20min + 12s);
        const TimePattern pattern = TimePattern::parse("T12:30:00/T13:20:12");
        ASSERT_EQ(TimePattern::Type::interval, pattern.getType());
        EXPECT_EQ(expected.getStartTime(), pattern.asInterval().getStartTime());
        EXPECT_EQ(expected.getEndTime(), pattern.asInterval().getEndTime());
        EXPECT_EQ(expected.getWeekdays(), pattern.asInterval().getWeekdays());
    }
    {
        const TimeInterval expected(12h + 30min, 13h + 20min + 12s, Weekdays::monday());
        const TimePattern pattern = TimePattern::parse("W001/T12:30:00/T13:20:12");
        ASSERT_EQ(TimePattern::Type::interval, pattern.getType());
        EXPECT_EQ(expected.getStartTime(), pattern.asInterval().getStartTime());
        EXPECT_EQ(expected.getEndTime(), pattern.asInterval().getEndTime());
        EXPECT_EQ(expected.getWeekdays(), pattern.asInterval().getWeekdays());
    }
}

TEST(TimePattern, Timer)
{
    {
        const Timer timer(1h + 30min, 5, 20s);
        const TimePattern pattern(timer);
        ASSERT_EQ(TimePattern::Type::timer, pattern.getType());
        EXPECT_EQ(timer.getExpiryTime(), pattern.asTimer().getExpiryTime());
        EXPECT_EQ(timer.getRandomVariation(), pattern.asTimer().getRandomVariation());
        EXPECT_EQ(timer.getNumberOfExecutions(), pattern.asTimer().getNumberOfExecutions());
    }
    {
        const Timer expected(1h + 30min + 20s);
        const TimePattern pattern = TimePattern::parse("PT01:30:20");
        ASSERT_EQ(TimePattern::Type::timer, pattern.getType());
        EXPECT_EQ(expected.getExpiryTime(), pattern.asTimer().getExpiryTime());
        EXPECT_EQ(expected.getRandomVariation(), pattern.asTimer().getRandomVariation());
        EXPECT_EQ(expected.getNumberOfExecutions(), pattern.asTimer().getNumberOfExecutions());
    }
    {
        const Timer expected(1h + 30min + 20s, 20s);
        const TimePattern pattern = TimePattern::parse("PT01:30:20A00:00:20");
        ASSERT_EQ(TimePattern::Type::timer, pattern.getType());
        EXPECT_EQ(expected.getExpiryTime(), pattern.asTimer().getExpiryTime());
        EXPECT_EQ(expected.getRandomVariation(), pattern.asTimer().getRandomVariation());
        EXPECT_EQ(expected.getNumberOfExecutions(), pattern.asTimer().getNumberOfExecutions());
    }
    {
        const Timer expected(1h + 30min + 20s, Timer::infiniteExecutions);
        const TimePattern pattern = TimePattern::parse("R/PT01:30:20");
        ASSERT_EQ(TimePattern::Type::timer, pattern.getType());
        EXPECT_EQ(expected.getExpiryTime(), pattern.asTimer().getExpiryTime());
        EXPECT_EQ(expected.getRandomVariation(), pattern.asTimer().getRandomVariation());
        EXPECT_EQ(expected.getNumberOfExecutions(), pattern.asTimer().getNumberOfExecutions());
    }
    {
        const Timer expected(1h + 30min + 20s, Timer::infiniteExecutions, 20s);
        const TimePattern pattern = TimePattern::parse("R/PT01:30:20A00:00:20");
        ASSERT_EQ(TimePattern::Type::timer, pattern.getType());
        EXPECT_EQ(expected.getExpiryTime(), pattern.asTimer().getExpiryTime());
        EXPECT_EQ(expected.getRandomVariation(), pattern.asTimer().getRandomVariation());
        EXPECT_EQ(expected.getNumberOfExecutions(), pattern.asTimer().getNumberOfExecutions());
    }
    {
        const Timer expected(1h + 30min + 20s, 5);
        const TimePattern pattern = TimePattern::parse("R05/PT01:30:20");
        ASSERT_EQ(TimePattern::Type::timer, pattern.getType());
        EXPECT_EQ(expected.getExpiryTime(), pattern.asTimer().getExpiryTime());
        EXPECT_EQ(expected.getRandomVariation(), pattern.asTimer().getRandomVariation());
        EXPECT_EQ(expected.getNumberOfExecutions(), pattern.asTimer().getNumberOfExecutions());
    }
    {
        const Timer expected(1h + 30min + 20s, 5, 20s);
        const TimePattern pattern = TimePattern::parse("R05/PT01:30:20A00:00:20");
        ASSERT_EQ(TimePattern::Type::timer, pattern.getType());
        EXPECT_EQ(expected.getExpiryTime(), pattern.asTimer().getExpiryTime());
        EXPECT_EQ(expected.getRandomVariation(), pattern.asTimer().getRandomVariation());
        EXPECT_EQ(expected.getNumberOfExecutions(), pattern.asTimer().getNumberOfExecutions());
    }
}