/**
    \file SensorList.h
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

#ifndef INCLUDE_HUEPLUSPLUS_SENSOR_LIST_H
#define INCLUDE_HUEPLUSPLUS_SENSOR_LIST_H

#include "ResourceList.h"
#include "Sensor.h"

namespace hueplusplus
{
//! \brief Handles a list of Sensor%s with type specific getters
//!
//! Allows to directly get the requested sensor type or all sensors of a given type.
class SensorList : public CreateableResourceList<SearchableResourceList<Sensor>, CreateSensor>
{
public:
    using CreateableResourceList::CreateableResourceList;

    //! \brief Get sensor specified by id, convert to \c T
    //! \param id Sensor id
    //! \tparam T Sensor type to convert to (from \ref sensors)
    //! \returns The sensor matching the id and type
    //! \throws HueException when id does not exist or type does not match
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    template <typename T>
    T getAsType(int id)
    {
        return get(id).asSensorType<T>();
    }
    //! \brief Get all sensors of type \c T
    //! \tparam T Sensor type to get (from \ref sensors)
    //! \returns All sensors matching the type
    //! \throws HueException when response contains no body
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    template <typename T>
    std::vector<T> getAllByType()
    {
        nlohmann::json state = this->stateCache->getValue();
        std::vector<T> result;
        for (auto it = state.begin(); it != state.end(); ++it)
        {
            // Only parse the sensors with the correct type
            if (it->value("type", "") == T::typeStr)
            {
                result.push_back(get(maybeStoi(it.key())).asSensorType<T>());
            }
        }
        return result;
    }

protected:
    //! \brief Protected defaulted move constructor
    SensorList(SensorList&&) = default;
    //! \brief Protected defaulted move assignment
    SensorList& operator=(SensorList&&) = default;
};
} // namespace hueplusplus

#endif
