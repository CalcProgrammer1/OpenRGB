/**
    \file Condition.h
    Copyright Notice\n
    Copyright (C) 2020  Jan Rogall          	- developer\n

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

#ifndef INCLUDE_HUEPLUSPLUS_CONDITION_H
#define INCLUDE_HUEPLUSPLUS_CONDITION_H

#include "TimePattern.h"

#include <nlohmann/json.hpp>

namespace hueplusplus
{
//! \brief Condition for a Rule
//!
//! The condition checks whether a resource attribute (usually a Sensor value) matches the
//! specified Operator.
//!
//! Conditions from sensors can be created more easily using the makeCondition() helper functions.
class Condition
{
public:
    //! \brief Specifies which operation is used to check the condition
    enum class Operator
    {
        eq, //!< Attribute is equal to specified value (for bool and int)
        gt, //!< Attribute is greater than specified value (for int)
        lt, //!< Attribute is less than specified value (for int)
        dx, //!< Attribute has changed (no value given)
        ddx, //!< Delayed attribute has changed (no value given)
        stable, //!< Stable for a given time. Does not trigger a rule change
        notStable, //!< Not stable for a given time. Does not trigger a rule change
        in, //!< Time is in the given interval (triggered on start time, local time)
        notIn //!< Time is not in the interval (triggered on end time, local time)
    };

public:
    //! \brief Create a condition from any address on the bridge
    //! \param address Path to an attribute of the bridge
    //! \param op Operator used for comparison.
    //! \param value String representation of the value to check against. Empty for some operators.
    Condition(const std::string& address, Operator op, const std::string& value);

    //! \brief Get address on the bridge
    std::string getAddress() const;
    //! \brief Get used operator
    Operator getOperator() const;
    //! \brief Get value the attribute is checked against
    std::string getValue() const;

    //! \brief Create the json form of the condition
    //! \returns A json object with address, operator and value
    nlohmann::json toJson() const;

    //! \brief Parse condition from json value
    //! \param json Json object with address, operator and value
    //! \returns The parsed condition with the same values
    //! \throws HueException when the operator is unknown.
    static Condition parse(const nlohmann::json& json);

private:
    std::string address;
    Operator op;
    std::string value;
};

namespace detail
{
//! Helper class to make creating conditions more convenient.
//! Specializations for each data type provide methods for the supported operators.
//! This allows the user to write <code>makeCondition(sensor).eq(value)</code>
template <typename T>
class ConditionHelper
{ };

//! General operators supported by all data types
class GeneralConditionHelper
{
public:
    explicit GeneralConditionHelper(const std::string& address) : address(address) { }

    Condition dx() { return Condition(address, Condition::Operator::dx, ""); }
    Condition ddx() { return Condition(address, Condition::Operator::ddx, ""); }
    //! Docs does not say anything about format of stable value
    //! \todo Change to either duration or int for seconds
    Condition stable(const std::string& value) { return Condition(address, Condition::Operator::dx, value); }

protected:
    std::string address;
};

//! Operators supported by int conditions
template <>
class ConditionHelper<int> : public GeneralConditionHelper
{
public:
    using GeneralConditionHelper::GeneralConditionHelper;

    Condition eq(int value) { return create(Condition::Operator::eq, value); }
    Condition gt(int value) { return create(Condition::Operator::gt, value); }
    Condition lt(int value) { return create(Condition::Operator::eq, value); }

    Condition create(Condition::Operator op, int value) { return Condition(address, op, std::to_string(value)); }
};

//! Operators supported by bool conditions
template <>
class ConditionHelper<bool> : public GeneralConditionHelper
{
public:
    using GeneralConditionHelper::GeneralConditionHelper;

    Condition eq(bool value) { return create(Condition::Operator::eq, value); }

    Condition create(Condition::Operator op, bool value) { return Condition(address, op, value ? "true" : "false"); }
};

//! Operators supported by timestamp conditions
template <>
class ConditionHelper<time::AbsoluteTime> : public GeneralConditionHelper
{
public:
    using GeneralConditionHelper::GeneralConditionHelper;

    Condition in(const time::TimeInterval& interval) { return create(Condition::Operator::in, interval); }
    Condition notIn(const time::TimeInterval& interval) { return create(Condition::Operator::notIn, interval); }

    Condition create(Condition::Operator op, const time::AbsoluteTime& value)
    {
        return Condition(address, op, value.toString());
    }
    Condition create(Condition::Operator op, const time::TimeInterval& interval)
    {
        return Condition(address, op, interval.toString());
    }
};

template <typename... Ts>
struct make_void
{
    typedef void type;
};
//! c++17 void_t
template <typename... Ts>
using void_t = typename make_void<Ts...>::type;

} // namespace detail

} // namespace hueplusplus

#endif
