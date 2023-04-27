#include "Detector.h"
#include "CryorigH7QuadLumiController.h"
#include "RGBController.h"
#include "RGBController_CryorigH7QuadLumi.h"
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| CRYORIG/NZXT USB IDs                                  |
\*-----------------------------------------------------*/
#define NZXT_VID                    0x1E71
#define CRYORIG_H7_QUAD_LUMI_PID    0x1712

static void DetectCryorigH7QuadLumi(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        CryorigH7QuadLumiController*     controller     = new CryorigH7QuadLumiController(dev, info->path);
        RGBController_CryorigH7QuadLumi* rgb_controller = new RGBController_CryorigH7QuadLumi(controller);
        rgb_controller->name                            = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR("CRYORIG H7 Quad Lumi",   DetectCryorigH7QuadLumi, NZXT_VID,   CRYORIG_H7_QUAD_LUMI_PID);
