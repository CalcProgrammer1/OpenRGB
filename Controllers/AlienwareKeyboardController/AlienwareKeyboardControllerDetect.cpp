#include "Detector.h"
#include "AlienwareAW510KController.h"
#include "RGBController.h"
#include "RGBController_AlienwareAW510K.h"
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Alienware vendor ID                                   |
\*-----------------------------------------------------*/
#define ALIENWARE_VID           0x04F2

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define ALIENWARE_AW510K_PID    0x1830

/******************************************************************************************\
*                                                                                          *
*   DetectAlienwareKeyboardControllers                                                     *
*                                                                                          *
*       Tests the USB address to see if a Alienware RGB Keyboard controller exists there.  *
*                                                                                          *
\******************************************************************************************/

void DetectAlienwareAW510KControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if( dev )
    {
        AlienwareAW510KController*     controller     = new AlienwareAW510KController(dev, info->path);
        RGBController_AlienwareAW510K* rgb_controller = new RGBController_AlienwareAW510K(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectAlienwareKeyboardControllers() */


REGISTER_HID_DETECTOR_IPU("Alienware AW510K",   DetectAlienwareAW510KControllers,   ALIENWARE_VID,  ALIENWARE_AW510K_PID,   0x02,   0xFF00, 0x01);
