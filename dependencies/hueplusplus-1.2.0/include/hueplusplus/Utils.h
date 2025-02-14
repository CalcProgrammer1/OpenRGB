/**
    \file Utils.h
    Copyright Notice\n
    Copyright (C) 2020  Jan Rogall		- developer\n
    Copyright (C) 2020  Moritz Wirger	- developer\n

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

#ifndef INCLUDE_HUEPLUSPLUS_UTILS_H
#define INCLUDE_HUEPLUSPLUS_UTILS_H

#include <nlohmann/json.hpp>

namespace hueplusplus
{
//! \brief Utility functions used in multiple places.
namespace utils
{
namespace detail
{
// Forward declaration
template <typename KeyT, typename... Paths>
nlohmann::json safeGetMemberHelper(const nlohmann::json& json, std::size_t index, Paths&&... otherPaths);

inline nlohmann::json safeGetMemberHelper(const nlohmann::json& json)
{
    return json;
}

template <typename KeyT, typename... Paths,
    std::enable_if_t<!std::is_integral<std::remove_reference_t<KeyT>>::value>* = nullptr>
nlohmann::json safeGetMemberHelper(const nlohmann::json& json, KeyT&& key, Paths&&... otherPaths)
{
    auto memberIt = json.find(std::forward<KeyT>(key));
    if (memberIt == json.end())
    {
        return nullptr;
    }
    return safeGetMemberHelper(*memberIt, std::forward<Paths>(otherPaths)...);
}

// Needs to be after the other safeGetMemberHelper, otherwise another forward declaration is needed
template <typename... Paths>
nlohmann::json safeGetMemberHelper(const nlohmann::json& json, std::size_t index, Paths&&... otherPaths)
{
    if (!json.is_array() || json.size() <= index)
    {
        return nullptr;
    }
    return safeGetMemberHelper(json[index], std::forward<Paths>(otherPaths)...);
}
} // namespace detail

//! \brief Function for validating that a request was executed correctly
//!
//! \param path The path the PUT request was made to
//! \param request The request that was sent initially
//! \param reply The reply that was received
//! \return True if request was executed correctly
bool validatePUTReply(const std::string& path, const nlohmann::json& request, const nlohmann::json& reply);

bool validateReplyForLight(const nlohmann::json& request, const nlohmann::json& reply, int lightId);

//! \brief Checks equality to 4 decimal places
//!
//! Floats in Hue json responses are rounded to 4 decimal places.
inline bool floatEquals(float lhs, float rhs)
{
    return std::abs(lhs - rhs) <= 1E-4f;
}

//! \brief Returns the object/array member or null if it does not exist
//!
//! \param json The base json value
//! \param paths Any number of child accesses (e.g. 0, "key" would access json[0]["key"])
//! \returns The specified member or null if any intermediate object does not contain the specified child.
template <typename... Paths>
nlohmann::json safeGetMember(const nlohmann::json& json, Paths&&... paths)
{
    return detail::safeGetMemberHelper(json, std::forward<Paths>(paths)...);
}

} // namespace utils

namespace detail
{
//! \brief Makes a class with protected copy constructor copyable.
//!
//! Used in private members to expose mutable references to \c T
//! while not allowing them to be assigned to.
//! Make sure \c T is actually designed to be used this way!
template <typename T>
class MakeCopyable : public T
{
public:
    // Make copy constructor and assignment operator public
    using T::T;
    using T::operator=;
};
} // namespace detail
} // namespace hueplusplus

#endif