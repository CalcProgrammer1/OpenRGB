#include "Detector.h"
#include "ASRockPolychromeUSBController.h"
#include "RGBController.h"
#include "RGBController_ASRockPolychromeUSB.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| ASRock vendor ID                                      |
\*-----------------------------------------------------*/
#define ASROCK_VID                   0x26CE

/*-----------------------------------------------------*\
| ASRock product IDs                                    |
\*-----------------------------------------------------*/
#define ASROCK_MOTHERBOARD_1_PID     0x01A2

void DetectPolychromeUSBControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        PolychromeUSBController* controller = new PolychromeUSBController(dev, info->path);
        RGBController_PolychromeUSB* rgb_controller = new RGBController_PolychromeUSB(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR("ASrock Polychrome USBMotherboard", DetectPolychromeUSBControllers, ASROCK_VID, ASROCK_MOTHERBOARD_1_PID);
