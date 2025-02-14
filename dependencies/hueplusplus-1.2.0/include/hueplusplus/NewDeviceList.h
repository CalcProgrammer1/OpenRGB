/**
    \file NewDeviceList.h
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

#ifndef INCLUDE_HUEPLUSPLUS_NEW_DEVICE_LIST_H
#define INCLUDE_HUEPLUSPLUS_NEW_DEVICE_LIST_H

#include <map>
#include <string>

#include "TimePattern.h"

#include <nlohmann/json.hpp>

namespace hueplusplus
{
//! \brief List of new devices found during the last scan
class NewDeviceList
{
public:
    //! \brief Construct from data
    NewDeviceList(const std::string& lastScan, const std::map<int, std::string>& devices);

    //! \brief Get a map of id and name of new devices
    const std::map<int, std::string>& getNewDevices() const;

    //! \brief Get whether a last scan time is available
    //!
    //! This can be false if there was no scan since the last restart
    //! or if the scan is still running.
    bool hasLastScanTime() const;
    //! \brief Get whether scan is currently active
    //!
    //! When scan is active, no last scan time is available
    bool isScanActive();
    //! \brief Get time when last scan was completed
    //! \throws HueException when no time is available or timestamp is invalid
    //! \note Must only be called when \ref hasLastScanTime() is true.
    time::AbsoluteTime getLastScanTime() const;

    //! \brief Parse from json response
    //! \throws std::invalid_argument when json is invalid.
    //! \throws nlohmann::json::exception when json is invalid.
    static NewDeviceList parse(const nlohmann::json& json);

private:
    std::string lastScan;
    std::map<int, std::string> devices;
};
} // namespace hueplusplus

#endif
