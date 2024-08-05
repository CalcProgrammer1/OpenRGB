/**
    \file UsernameConfig.cpp
    Copyright Notice\n
    Copyright (C) 2021  Jan Rogall		- developer\n

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

    \example{lineno} UsernameConfig.cpp
    This example reads the username and mac address from a config file.
**/

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>

#include <hueplusplus/Bridge.h>

#ifdef _MSC_VER
#include <hueplusplus/WinHttpHandler.h>

using SystemHttpHandler = hueplusplus::WinHttpHandler;

#else
#include <hueplusplus/LinHttpHandler.h>

using SystemHttpHandler = hueplusplus::LinHttpHandler;

#endif

namespace hue = hueplusplus;

// Reads a json config file.
// filename: Path to the config file
// returns parsed json or an empty object if not successful.
nlohmann::json readConfigFile(const std::string& filename)
{
    std::ifstream stream(filename);
    try
    {
        nlohmann::json result = nlohmann::json::object();
        if (stream)
        {
            stream >> result;
        }
        return result;
    }
    catch (const nlohmann::json::exception&)
    {
        // Ignore parse errors
        return nlohmann::json::object();
    }
}

// Saves a json file.
// filename: Path to the config file
// config: Json value to save
void saveConfigFile(const std::string& filename, const nlohmann::json& config)
{
    std::ofstream stream(filename);
    stream << std::setw(4) << config;
}

// Connects to a bridge and returns it
// username: Already existing username, can be left empty.
// macAddress: MAC address of the bridge, can be left empty.
// throws std::runtime_error when the bridge was not found.
// returns a connected bridge.
hue::Bridge connectToBridge(const std::string& username, const std::string& macAddress)
{
    hue::BridgeFinder finder(std::make_shared<SystemHttpHandler>());

    std::vector<hue::BridgeFinder::BridgeIdentification> bridges = finder.findBridges();

    for (const auto& bridge : bridges)
    {
        std::cout << "Bridge: " << bridge.mac << " at " << bridge.ip << '\n';
    }
    if (bridges.empty())
    {
        std::cout << "Found no bridges\n";
        throw std::runtime_error("no bridges found");
    }

    if (macAddress.empty())
    {
        std::cout << "No bridge given, connecting to first one.\n";
        return finder.getBridge(bridges.front());
    }
    if (!username.empty())
    {
        finder.addUsername(macAddress, username);
    }
    auto it = std::find_if(
        bridges.begin(), bridges.end(), [&](const auto& identification) { return identification.mac == macAddress; });
    if (it == bridges.end())
    {
        std::cout << "Given bridge not found\n";
        throw std::runtime_error("bridge not found");
    }
    return finder.getBridge(*it);
}

// Connects to a bridge. The steps are:
// - read "config.json" for an existing config
// - connect to the bridge
// - save the username to the config file for the next run
// 
// Also prints out the IP and username.
int main(int argc, char** argv)
{

    const std::string filename = "config.json";
    try
    {

        nlohmann::json config = readConfigFile(filename);
        const std::string username = config.value("username", "");
        const std::string macAddress = config.value("mac", "");
        hue::Bridge hue = connectToBridge(username, macAddress);

        // Store updated information into file
        config["username"] = hue.getUsername();
        config["mac"] = hue.config().getMACAddress();
        saveConfigFile(filename, config);

        std::cout << "Connected to bridge. IP: " << hue.getBridgeIP() << ", username: " << hue.getUsername() << '\n';
    }
    catch (...)
    { }

    std::cout << "Press enter to exit\n";
    std::cin.get();

    return 0;
}