/**
    \file UPnP.cpp
    Copyright Notice\n
    Copyright (C) 2017  Jan Rogall		- developer\n
    Copyright (C) 2017  Moritz Wirger	- developer\n

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

#include "hueplusplus/UPnP.h"

#include <algorithm>
#include <iostream>

#include "hueplusplus/LibConfig.h"

namespace hueplusplus
{
std::vector<std::pair<std::string, std::string>> UPnP::getDevices(std::shared_ptr<const IHttpHandler> handler)
{
    // send UPnP M-Search request
    std::vector<std::string> foundDevices
        = handler->sendMulticast("M-SEARCH * HTTP/1.1\r\nHOST: 239.255.255.250:1900\r\nMAN: "
                                 "\"ssdp:discover\"\r\nMX: 5\r\nST: ssdp:all\r\n\r\n",
            "239.255.255.250", 1900, Config::instance().getUPnPTimeout());

    std::vector<std::pair<std::string, std::string>> devices;

    // filter out devices
    for (const std::string& s : foundDevices)
    {
        std::pair<std::string, std::string> device;
        std::size_t start = s.find("LOCATION:");
        if (start == std::string::npos)
        {
            continue;
        }
        start += 10;
        device.first = s.substr(start, s.find("\r\n", start) - start);
        start = s.find("SERVER:");
        if (start == std::string::npos)
        {
            continue;
        }
        start += 8;
        device.second = s.substr(start, s.find("\r\n", start) - start);
        if (std::find_if(devices.begin(), devices.end(),
                [&](const std::pair<std::string, std::string>& item) { return item.first == device.first; })
            == devices.end())
        {
            devices.push_back(device);

            // std::cout << "Device: \t" << device.first << std::endl;
            // std::cout << "        \t" << device.second << std::endl;
        }
    }
    return devices;
}
} // namespace hueplusplus
