/**
    \file Action.h
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

#ifndef INCLUDE_HUEPLUSPLUS_ACTION_H
#define INCLUDE_HUEPLUSPLUS_ACTION_H

#include <nlohmann/json.hpp>

namespace hueplusplus
{
//! \brief Action executed by the bridge, e.g. as a Schedule command
//!
//! The action makes either a POST, PUT or DELETE request with a given body
//! to an address on the bridge.
//!
//! The Action can also be created by StateTransaction::toAction().
class Action
{
public:
    //! \brief Create Action from json
    //! \param json JSON object with address, method and body
    explicit Action(const nlohmann::json& json);

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

public:
    //! \brief Parse Method from string
    //! \param s \c POST, \c PUT or \c DELETE
    static Method parseMethod(const std::string& s);
    //! \brief Get string from Method
    //! \returns \c POST, \c PUT or \c DELETE
    static std::string methodToString(Method m);

private:
    nlohmann::json json;
};
} // namespace hueplusplus

#endif
