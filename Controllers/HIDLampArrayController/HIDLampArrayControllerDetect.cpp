#include "Detector.h"
#include "HIDLampArrayController.h"
#include "RGBController.h"
#include "RGBController_HIDLampArray.h"
#include <vector>
#include <hidapi/hidapi.h>

void DetectHIDLampArrayControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        HIDLampArrayController*     controller     = new HIDLampArrayController(dev, info->path);
        RGBController_HIDLampArray* rgb_controller = new RGBController_HIDLampArray(controller);
        rgb_controller->name                     = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectHIDLampArrayControllers() */

REGISTER_HID_DETECTOR("Arduino Zero HID Lamp Array Demo", DetectHIDLampArrayControllers, 0x2341, 0x804D);
