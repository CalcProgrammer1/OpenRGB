#include "Detector.h"
#include <hidapi/hidapi.h>
#include "A4TechBloodyB820RController.h"
#include "RGBController.h"
#include "RGBController_A4TechBloodyB820R.h"

/*---------------------------------------------------------*\
| A4Tech vendor ID                                    |
\*---------------------------------------------------------*/
#define A4Tech_VID                               0x09DA

/*---------------------------------------------------------*\
| Bloody B820R product ID                                    |
\*---------------------------------------------------------*/
#define B820R_PID                               0xFA10

/******************************************************************************************\
*                                                                                          *
*   DetectA4TechBloodyB820RControllers                                                           *
*                                                                                          *
*       Tests the USB address to see if an A4Tech Bloody controller exists there.           *
*                                                                                          *
\******************************************************************************************/

void DetectA4TechBloodyB820R(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        A4TechBloodyB820RController*     controller     = new A4TechBloodyB820RController(dev, info->path);
        RGBController_A4TechBloodyB820R* rgb_controller = new RGBController_A4TechBloodyB820R(controller);
        rgb_controller->name                      = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

//REGISTER_HID_DETECTOR_I("A4Tech Bloody B820R",  DetectA4TechBloodyB820R,   A4Tech_VID,    B820R_PID, 2);
REGISTER_HID_DETECTOR_IPU("A4Tech Bloody B820R SE",  DetectA4TechBloodyB820R,   A4Tech_VID,    B820R_PID, 2, 0xFF52, 0x0210);
