/*-------------------------------------------------------------------*\
|  LenovoUSBDetect.h                                                  |
|                                                                     |
|  Describes zones for various Lenovo Legion Devices                  |
|                                                                     |
|  Cooper Hall (geobot19)          17 Apr 2022                        |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "Detector.h"
#include "LogManager.h"
#include "RGBController.h"
#include "LenovoUSBController.h"
#include "LenovoDevices.h"
#include "RGBController_LenovoUSB.h"
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| vendor IDs                                            |
\*-----------------------------------------------------*/
#define ITE_VID                                 0x048D

/*-----------------------------------------------------*\
| Interface, Usage, and Usage Page                      |
\*-----------------------------------------------------*/
enum
{
    LENOVO_PAGE   = 0xFF89,
    LENOVO_USEAGE = 0x07
};

void DetectLenovoLegionUSBControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        LenovoUSBController*     controller      = new LenovoUSBController(dev, info->path, info->product_id);
        RGBController_LenovoUSB* rgb_controller  = new RGBController_LenovoUSB(controller);
        rgb_controller->name                     = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_PU("Lenovo Legion Y740",                DetectLenovoLegionUSBControllers,  ITE_VID,  LEGION_Y740,           LENOVO_PAGE, LENOVO_USEAGE);
REGISTER_HID_DETECTOR_PU("Lenovo Legion 7 gen 5",             DetectLenovoLegionUSBControllers,  ITE_VID,  LEGION_Y750,           LENOVO_PAGE, LENOVO_USEAGE);
REGISTER_HID_DETECTOR_PU("Lenovo Legion 7S gen 5",            DetectLenovoLegionUSBControllers,  ITE_VID,  LEGION_Y750S,          LENOVO_PAGE, LENOVO_USEAGE);
REGISTER_HID_DETECTOR_PU("Lenovo Legion 7 gen 6",             DetectLenovoLegionUSBControllers,  ITE_VID,  LEGION_Y760,           LENOVO_PAGE, LENOVO_USEAGE);
REGISTER_HID_DETECTOR_PU("Lenovo Legion 7S gen 6",            DetectLenovoLegionUSBControllers,  ITE_VID,  LEGION_Y760S,          LENOVO_PAGE, LENOVO_USEAGE);
