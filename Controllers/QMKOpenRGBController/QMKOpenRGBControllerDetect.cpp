/*-------------------------------------------------------------------*\
|  QMKOpenRGBControllerDetect.cpp                                     |
|                                                                     |
|  Driver for QMK keyboards using OpenRGB Protocol                    |
|                                                                     |
|  Kasper       10th Octobber 2020                                    |
|  Jath03       28th May 2021                                         |
\*-------------------------------------------------------------------*/

#include "Detector.h"
#include "QMKOpenRGBController.h"
#include "RGBController.h"
#include "RGBController_QMKOpenRGB.h"
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Protocol version                                      |
\*-----------------------------------------------------*/
#define QMK_OPENRGB_PROTOCOL_VERSION            0x09

/*-----------------------------------------------------*\
| Usage and Usage Page                                  |
\*-----------------------------------------------------*/
#define QMK_USAGE_PAGE                          0xFF60
#define QMK_USAGE                               0x61

/*-----------------------------------------------------*\
| Massdrop                                              |
\*-----------------------------------------------------*/
#define MASSDROP_VID                            0x04D8
#define MASSDROP_ALT_PID                        0xEED3
#define MASSDROP_CTRL_PID                       0xEED2

/*-----------------------------------------------------*\
| SonixQMK                                              |
\*-----------------------------------------------------*/
#define SONIXQMK_VID                            0x0C45
#define SONIXQMK_5004_PID                       0x5004
#define SONIXQMK_5104_PID                       0x5104

/*-----------------------------------------------------*\
| KBDFans                                              |
\*-----------------------------------------------------*/
#define KBDFANS_VID                             0x4B42
#define KBDFANS_K67_MKII_RGB_PID                0x1225

void DetectQMKOpenRGBControllers(hid_device_info *info, const std::string&)
{
    hid_device *dev = hid_open_path(info->path);

    if(dev)
    {
        QMKOpenRGBController* controller                = new QMKOpenRGBController(dev, info->path);
        unsigned int version                            = controller->GetProtocolVersion();

        if(version == QMK_OPENRGB_PROTOCOL_VERSION)
        {
            RGBController_QMKOpenRGB* rgb_controller    = new RGBController_QMKOpenRGB(controller);
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
        else
        {
            delete controller;
        }
    }
}

REGISTER_HID_DETECTOR_IPU("Massdrop Alt",           DetectQMKOpenRGBControllers, MASSDROP_VID, MASSDROP_ALT_PID,            1, QMK_USAGE_PAGE, QMK_USAGE);
REGISTER_HID_DETECTOR_IPU("Massdrop Ctrl",          DetectQMKOpenRGBControllers, MASSDROP_VID, MASSDROP_CTRL_PID,           1, QMK_USAGE_PAGE, QMK_USAGE);
REGISTER_HID_DETECTOR_IPU("SonixQMK 0C45:5004",     DetectQMKOpenRGBControllers, SONIXQMK_VID, SONIXQMK_5004_PID,           1, QMK_USAGE_PAGE, QMK_USAGE);
REGISTER_HID_DETECTOR_IPU("SonixQMK 0C45:5104",     DetectQMKOpenRGBControllers, SONIXQMK_VID, SONIXQMK_5104_PID,           1, QMK_USAGE_PAGE, QMK_USAGE);
REGISTER_HID_DETECTOR_IPU("KBDFans KBD67 MKII RGB", DetectQMKOpenRGBControllers, KBDFANS_VID,  KBDFANS_K67_MKII_RGB_PID,    1, QMK_USAGE_PAGE, QMK_USAGE);
