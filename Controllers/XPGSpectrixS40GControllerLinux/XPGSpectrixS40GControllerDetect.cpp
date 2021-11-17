/*-------------------------------------------------------------------*\
|  XPGSpectrixS40GControllerDetect.cpp                                |
|                                                                     |
|  Driver for XPG's Spectrix S40G NVMe                                |
|                                                                     |
|  NicolasNewman          25th Mar 2021                               |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "Detector.h"
#include "XPGSpectrixS40GController.h"
#include "RGBController.h"
#include "RGBController_XPGSpectrixS40G.h"
#include <vector>

#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/ioctl.h>

void DetectSpectrixS40GControllers(std::vector<RGBController*>& rgb_controllers)
{
    XPGSpectrixS40GController*      new_controller;
    RGBController_XPGSpectrixS40G*  new_rgbcontroller;

    int nvme_fd = open("/dev/nvme0", O_RDWR);

    if(nvme_fd > 0)
    {
        new_controller      = new XPGSpectrixS40GController(nvme_fd, 0x67);
        new_rgbcontroller   = new RGBController_XPGSpectrixS40G(new_controller);
        
        rgb_controllers.push_back(new_rgbcontroller);
    }

}   /* DetectSpectrixS40GControllers() */

REGISTER_DETECTOR("XPG Spectrix S40G NVMe", DetectSpectrixS40GControllers);
