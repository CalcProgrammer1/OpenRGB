/*---------------------------------------------------------*\
| RetroidAYNControllerDetect_Linux.cpp                      |
|                                                           |
|   Detector for Retroid/AYN RGB LEDs via Linux sysfs       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <algorithm>
#include <cctype>
#include <fstream>
#include <string>
#include <vector>
#include "DetectionManager.h"
#include "StringUtils.h"
#include "filesystem.h"
#include "RGBController_RetroidAYN_Linux.h"
#include "RetroidAYNController_Linux.h"

static std::string ReadDeviceModel()
{
    std::ifstream model_file("/sys/firmware/devicetree/base/model");

    if(!model_file.is_open())
    {
        return("");
    }

    std::string model;
    std::getline(model_file, model, '\0');
    model_file.close();

    return StringUtils::remove_null_terminating_chars(model);
}

static bool IsRetroidOrAYNDevice(const std::string& model)
{
    if(model.empty())
    {
        return(false);
    }

    std::string lower = StringUtils::remove_null_terminating_chars(model);

    for(char& c : lower)
    {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }

    return(lower.find("retroid") != std::string::npos || lower.find("ayn") != std::string::npos);
}

void DetectRetroidAYNLEDs(std::vector<std::string>& led_paths)
{
    /*-----------------------------------------------------*\
    | First verify this is a Retroid/AYN device             |
    \*-----------------------------------------------------*/
    std::string model = ReadDeviceModel();

    if(!IsRetroidOrAYNDevice(model))
    {
        return;
    }

    const std::string leds_class_path = "/sys/class/leds";

    if(!filesystem::exists(leds_class_path))
    {
        return;
    }

    for(const filesystem::directory_entry& entry : filesystem::directory_iterator(leds_class_path))
    {
        std::string led_dir = entry.path().string();
        std::string led_name = filesystem::path(led_dir).filename().string();

        /*-------------------------------------------------*\
        | Match LED directories starting with "rgb:"        |
        \*-------------------------------------------------*/
        if(led_name.rfind("rgb:", 0) == 0)
        {
            /*---------------------------------------------*\
            | Verify this LED supports multi_intensity      |
            \*---------------------------------------------*/
            std::string multi_intensity_path = led_dir + "/multi_intensity";
            if(filesystem::exists(multi_intensity_path))
            {
                led_paths.push_back(led_dir);
            }
        }
    }

    /*-----------------------------------------------------*\
    | Sort the LED paths alphabetically for consistent      |
    | ordering (l1, l2, l3, l4, r1, r2, r3, r4)             |
    \*-----------------------------------------------------*/
    std::sort(led_paths.begin(), led_paths.end());
}

DetectedControllers DetectRetroidAYNControllers()
{
    DetectedControllers detected_controllers;

    /*-----------------------------------------------------*\
    | Read the actual device model name for the controller  |
    | display name (e.g. "Retroid Pocket Nova")             |
    \*-----------------------------------------------------*/
    std::string device_model = ReadDeviceModel();

    /*-----------------------------------------------------*\
    | Detect Retroid/AYN (rgb:*) multicolor LED controller  |
    \*-----------------------------------------------------*/
    std::vector<std::string> retroid_ayn_led_paths;

    DetectRetroidAYNLEDs(retroid_ayn_led_paths);

    if(retroid_ayn_led_paths.size() > 0)
    {
        RetroidAYNController* controller = new RetroidAYNController(device_model);

        for(unsigned int led_idx = 0; led_idx < retroid_ayn_led_paths.size(); led_idx++)
        {
            controller->AddLED(retroid_ayn_led_paths[led_idx]);
        }

        RGBController_RetroidAYN* rgb_controller = new RGBController_RetroidAYN(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_DETECTOR("Retroid/AYN RGB", DetectRetroidAYNControllers);

REGISTER_CUSTOM_UDEV_RULE(retroid_ayn_rgb, "Retroid/AYN RGB", "SUBSYSTEM==\\\"leds\\\", KERNEL==\\\"rgb:*\\\", TAG+=\\\"uaccess\\\"");
