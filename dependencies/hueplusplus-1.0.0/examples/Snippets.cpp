/**
    \file Snippets.cpp
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

    \brief Contains code snippets used in the documentation, performs no useful functions.
**/

#include <hueplusplus/Bridge.h>
#include <hueplusplus/CLIPSensors.h>
#include <hueplusplus/ZLLSensors.h>

#ifdef _MSC_VER
#include <hueplusplus/WinHttpHandler.h>

namespace hueplusplus
{
// Dirty hack to make the snippets compile under windows. Dont do this,
// instead use your own alias which is set to either type like in BridgeSetup.cpp!
using LinHttpHandler = WinHttpHandler;
} // namespace hueplusplus

#else
#include <hueplusplus/LinHttpHandler.h>

#endif

void snippet1()
{
    // Main page
    //! [search-bridge]
    // For windows use std::make_shared<hueplusplus::WinHttpHandler>();
    auto handler = std::make_shared<hueplusplus::LinHttpHandler>();
    hueplusplus::BridgeFinder finder(handler);
    std::vector<hueplusplus::BridgeFinder::BridgeIdentification> bridges = finder.findBridges();
    if (bridges.empty())
    {
        std::cerr << "No bridges found\n";
        return;
    }
    //! [search-bridge]
    //! [get-bridge-2]
    finder.addUsername(bridges[0].mac, "<username>");
    //! [get-bridge-1]
    hueplusplus::Bridge bridge = finder.getBridge(bridges[0]);
    //! [get-bridge-1]
    //! [get-bridge-2]
    //! [light-1]
    hueplusplus::Light light1 = bridge.lights().get(1);
    //! [light-1]
    //! [light-2]
    std::vector<hueplusplus::Light> lights = bridge.lights().getAll();
    //! [light-2]
    //! [light-3]
    light1.on();
    light1.setBrightness(120);
    light1.alertHueSaturation({25500, 254});
    light1.setColorLoop(true);
    light1.setColorRGB({255, 128, 0});
    lights[1].off();
    lights.at(1).setColorHue(4562);
    //! [light-3]
    //! [light-4]
    hueplusplus::ColorType type1 = light1.getColorType();
    //! [light-4]
    //! [light-5]
    light1.hasBrightnessControl();
    light1.hasTemperatureControl();
    light1.hasColorControl();
    //! [light-5]

    // Getting started
    //! [control-lights]
    hueplusplus::Light light = bridge.lights().get(1);
    light.on();
    light.off();
    //! [control-lights]
    //! [control-groups]
    hueplusplus::Group group = bridge.groups().get(1);
    group.setOn(true);
    //! [control-groups]

    // Sensors
    //! [sensor-conditions]
    //! [known-sensor-1]
    hueplusplus::sensors::ZLLSwitch switchSensor = bridge.sensors().getAsType<hueplusplus::sensors::ZLLSwitch>(2);
    //! [known-sensor-1]
    // ZLLSwitch conditions operate on `buttonEvent`, use makeConditionLastUpdate()
    // to trigger on the last update time.

    // Some examples:
    hueplusplus::Condition upPressed
        = makeCondition(switchSensor).eq(hueplusplus::sensors::ZLLSwitch::c_UP_INITIAL_PRESS);
    hueplusplus::Condition buttonChanged = makeCondition(switchSensor).dx();

    hueplusplus::time::TimeInterval interval(std::chrono::hours(12), std::chrono::hours(13));
    hueplusplus::Condition updatedAtNoon = makeConditionLastUpdate(switchSensor).in(interval);
    //! [sensor-conditions]
    //! [known-sensor-2]
    std::vector<hueplusplus::sensors::ZLLSwitch> allSwitches
        = bridge.sensors().getAllByType<hueplusplus::sensors::ZLLSwitch>();
    //! [known-sensor-2]
    //! [generic-sensor-1]
    hueplusplus::Sensor genericSensor = bridge.sensors().get(1);
    if (genericSensor.hasOn())
    {
        // Now can check whether it is on
        if (genericSensor.isOn())
        {
            // ...
        }
    }
    //! [generic-sensor-1]

    // Transactions
    //! [transaction-lights]
    light.transaction().setOn(true).setBrightness(29).setColorHue(3000).setColorSaturation(128).commit();
    //! [transaction-lights]
    bool shouldTurnOn = true;
    //! [transaction-advanced]
    hueplusplus::StateTransaction t = light.transaction();
    if (shouldTurnOn)
        t.setOn(true);
    t.commit();
    //! [transaction-advanced]
    //! [transaction-groups]
    group.transaction().setOn(true).setBrightness(64).commit();
    //! [transaction-groups]
    hueplusplus::Schedule schedule = bridge.schedules().get(1);
    //! [transaction-action]
    hueplusplus::Action action = light.transaction().setOn(true).setBrightness(254).toAction();
    schedule.setCommand(action);
    //! [transaction-action]
}
void snippet2()
{
    // Main page
    //! [get-bridge-3]
    // For windows use std::make_shared<hueplusplus::WinHttpHandler>();
    auto handler = std::make_shared<hueplusplus::LinHttpHandler>();
    hueplusplus::Bridge bridge("192.168.2.102", 80, "<username>", handler);
    //! [get-bridge-3]

    // Sensors
    //! [generic-sensor-2]
    hueplusplus::Sensor genericSensor = bridge.sensors().get(1);
    if (genericSensor.getType() == hueplusplus::sensors::ZLLSwitch::typeStr)
    {
        hueplusplus::sensors::ZLLSwitch switchSensor = genericSensor.asSensorType<hueplusplus::sensors::ZLLSwitch>();
        // ...
    }
    //! [generic-sensor-2]
}

void snippet3()
{
    // Shared state
    auto handler = std::make_shared<hueplusplus::LinHttpHandler>();
    hueplusplus::BridgeFinder finder(handler);
    std::vector<hueplusplus::BridgeFinder::BridgeIdentification> bridges = finder.findBridges();
    //! [shared-bridge-1]
    hueplusplus::Bridge bridge = finder.getBridge(bridges[0], true);
    //! [shared-bridge-1]
    //! [refresh-example]
    bridge.setRefreshDuration(std::chrono::minutes(1));
    bridge.lights().setRefreshDuration(std::chrono::seconds(30));
    hueplusplus::Light light = bridge.lights().get(1);
    // ... wait some time
    bool on = light.isOn();
    //! [refresh-example]
}
void snippet4()
{
    // Shared state
    auto handler = std::make_shared<hueplusplus::LinHttpHandler>();
    //! [shared-bridge-2]
    hueplusplus::Bridge bridge("192.168.2.102", 80, "<username>", handler, "", std::chrono::seconds(10), true);
    //! [shared-bridge-2]
}

int main(int argc, char** argv)
{
    return 0;
}