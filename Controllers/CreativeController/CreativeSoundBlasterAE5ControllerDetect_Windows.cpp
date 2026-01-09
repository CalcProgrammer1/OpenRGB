/*---------------------------------------------------------*\
| CreativeSoundBlasterAE5ControllerDetect_Windows.cpp       |
|                                                           |
|   Detector for Creative SoundBlaster AE-5 (Windows)       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "CreativeSoundBlasterAE5Controller_Windows.h"
#include "RGBController_CreativeSoundBlasterAE5_Windows.h"
#include "LogManager.h"

void DetectCreativeAE5Device()
{
    LOG_INFO("[Creative SoundBlaster AE-5] Windows detection function called");

    CreativeSoundBlasterAE5Controller_Windows* controller = new CreativeSoundBlasterAE5Controller_Windows();

    if(controller->Initialize())
    {
        LOG_INFO("[Creative SoundBlaster AE-5] Device initialized successfully, registering controller");
        RGBController_CreativeSoundBlasterAE5* rgb_controller = new RGBController_CreativeSoundBlasterAE5(controller);
        DetectionManager::get()->RegisterRGBController(rgb_controller);
    }
    else
    {
        LOG_WARNING("[Creative SoundBlaster AE-5] Device initialization failed");
        delete controller;
    }
}

REGISTER_DETECTOR("Creative SoundBlaster AE-5", DetectCreativeAE5Device);
