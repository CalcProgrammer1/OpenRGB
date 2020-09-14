/**
    \file NewDeviceList.cpp
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

#include <hueplusplus/NewDeviceList.h>

namespace hueplusplus
{
NewDeviceList::NewDeviceList(const std::string& lastScan, const std::map<int, std::string>& devices)
    : lastScan(lastScan), devices(devices)
{ }
const std::map<int, std::string>& NewDeviceList::getNewDevices() const
{
    return devices;
}
bool NewDeviceList::hasLastScanTime() const
{
    return !lastScan.empty() && lastScan != "none" && lastScan != "active";
}
bool NewDeviceList::isScanActive()
{
    return lastScan == "active";
}
time::AbsoluteTime NewDeviceList::getLastScanTime() const
{
    return time::AbsoluteTime::parseUTC(lastScan); // UTC? not clear in docs
}
NewDeviceList NewDeviceList::parse(const nlohmann::json& json)
{
    std::map<int, std::string> devices;
    std::string lastScan;
    for (auto it = json.begin(); it != json.end(); ++it)
    {
        if (it.key() == "lastscan")
        {
            lastScan = it.value().get<std::string>();
        }
        else
        {
            int id = std::stoi(it.key());
            devices.emplace(id, it.value().at("name").get<std::string>());
        }
    }
    return NewDeviceList(lastScan, devices);
}
} // namespace hueplusplus