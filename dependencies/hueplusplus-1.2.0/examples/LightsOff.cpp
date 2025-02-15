/**
    \file LightsOff.cpp
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

    \example{lineno} LightsOff.cpp
    This example turns off all lights for 20 seconds, then turns them on again.
**/

#include <thread>

#include <hueplusplus/Bridge.h>

#ifdef _MSC_VER
#include <hueplusplus/WinHttpHandler.h>

using SystemHttpHandler = hueplusplus::WinHttpHandler;

#else
#include <hueplusplus/LinHttpHandler.h>

using SystemHttpHandler = hueplusplus::LinHttpHandler;

#endif

namespace hue = hueplusplus;

// Configure existing connections here, or leave empty for new connection
const std::string macAddress = "";
const std::string username = "";

// Connects to a bridge and returns it.
hue::Bridge connectToBridge()
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

// Turns off the lights on the bridge for 20 seconds. 
// Only turns the lights back on that were on before.
void lightsOff(hue::Bridge& hue)
{
    std::vector<hue::Light> lights = hue.lights().getAll();

    // Save current on state of the lights
    std::map<int, bool> onMap;
    for (hue::Light& l : lights)
    {
        onMap.emplace(l.getId(), l.isOn());
        l.off();
    }

    // This would be preferrable, but does not work because it also resets the brightness of all lights
    // Group 0 contains all lights, turn all off with a transition of 1 second
    // hue.groups().get(0).setOn(false, 10);
    // -------------------------------------

    std::cout << "Turned off all lights\n";

    std::this_thread::sleep_for(std::chrono::seconds(20));

    // Restore the original state of the lights
    for (hue::Light& l : lights)
    {
        if (onMap[l.getId()])
        {
            l.on();
        }
    }

    std::cout << "Turned lights back on\n";
}

int main(int argc, char** argv)
{

    try
    {
        hue::Bridge hue = connectToBridge();

        std::cout << "Connected to bridge. IP: " << hue.getBridgeIP() << ", username: " << hue.getUsername() << '\n';

        lightsOff(hue);
    }
    catch (...)
    { }

    std::cout << "Press enter to exit\n";
    std::cin.get();

    return 0;
}