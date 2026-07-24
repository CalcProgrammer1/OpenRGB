/*---------------------------------------------------------*\
| LinuxRGBLEDControllerDetect_Linux.cpp                        |
|                                                             |
|   Detector for Linux RGB sysfs LEDs (multi_intensity)       |
|                                                             |
|   This driver detects Valve-style sysfs LED devices at      |
|   /sys/class/leds/valve-leds* and similar interfaces that   |
|   expose multi_intensity sysfs files for RGB control.       |
|                                                             |
|   Devices are configured via the LinuxRGBLEDDevices         |
|   settings section.  Each device entry specifies a name     |
|   and a list of led_paths pointing to the sysfs LED         |
|   directories.  All LEDs are grouped into one linear zone.  |
|                                                             |
|   Adam Honse (calcprogrammer1@gmail.com)      25 Sep 2020   |
|                                                             |
|   This file is part of the OpenRGB project                  |
|   SPDX-License-Identifier: GPL-2.0-or-later                 |
\*---------------------------------------------------------*/

#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include "DetectionManager.h"
#include "filesystem.h"
#include "RGBController_LinuxRGBLED_Linux.h"

/*---------------------------------------------------------*\
| Helper function to extract the numeric index enclosed     |
| in brackets from a LED directory name for proper          |
| numerical sorting.                                        |
| For example, "valve-leds[2]" -> 2, "valve-leds[10]" -> 10.|
\*---------------------------------------------------------*/
static int GetLEDNumber(const std::string& path)
{
    /*-----------------------------------------------------*\
    | Extract the directory name from the full path         |
    \*-----------------------------------------------------*/
    std::string dir_name = filesystem::path(path).filename().string();

    /*-----------------------------------------------------*\
    | Find the last '[' and ']' in the directory name to    |
    | extract the numeric index enclosed in brackets.       |
    | For example, "valve-leds[10]" -> 10.                  |
    \*-----------------------------------------------------*/
    size_t bracket_start = dir_name.rfind('[');
    size_t bracket_end   = dir_name.rfind(']');

    if(bracket_start != std::string::npos &&
       bracket_end   != std::string::npos &&
       bracket_end   >  bracket_start)
    {
        std::string num_str = dir_name.substr(bracket_start + 1, bracket_end - bracket_start - 1);

        /*-------------------------------------------------*\
        | Verify the extracted substring is all digits      |
        | before converting, to avoid exceptions from       |
        | std::stoi.                                        |
        \*-------------------------------------------------*/
        bool all_digits = true;
        for(char c : num_str)
        {
            if(!std::isdigit(static_cast<unsigned char>(c)))
            {
                all_digits = false;
                break;
            }
        }

        if(all_digits && !num_str.empty())
        {
            return(std::stoi(num_str));
        }
    }

    return(0);
}

/*---------------------------------------------------------*\
| Comparison function to sort LED paths by their bracket    |
| enclosed numeric index, ensuring [0], [1], [2], ... [10], |
| [16] ordering instead of alphabetical (0, 1, 10, 11, ...  |
| 16, 2).                                                   |
\*---------------------------------------------------------*/
static bool CompareLEDPaths(const std::string& a, const std::string& b)
{
    return(GetLEDNumber(a) < GetLEDNumber(b));
}

/*---------------------------------------------------------*\
| Detect leds-valve sysfs LEDs by scanning for              |
| /sys/class/leds/ directories that contain both            |
| brightness and multi_intensity files                      |
\*---------------------------------------------------------*/
void DetectValveLEDs(std::vector<std::string>& led_paths)
{
    const std::string leds_class_path = "/sys/class/leds";

    if(!filesystem::exists(leds_class_path))
    {
        return;
    }

    for(const filesystem::directory_entry& entry : filesystem::directory_iterator(leds_class_path))
    {
        std::string led_dir = entry.path().string();

        /*-------------------------------------------------*\
        | Check for brightness and multi_intensity files    |
        \*-------------------------------------------------*/
        std::string brightness_path = led_dir + "/brightness";
        std::string multi_intensity_path = led_dir + "/multi_intensity";

        if(filesystem::exists(brightness_path) && filesystem::exists(multi_intensity_path))
        {
            if(led_dir.find("valve-leds") != std::string::npos)
            {
                led_paths.push_back(led_dir);
            }
        }
    }

    /*-----------------------------------------------------*\
    | Sort the LED paths numerically by their bracket       |
    | enclosed index to ensure correct ordering from [0]    |
    | to [N]                                                |
    \*-----------------------------------------------------*/
    std::sort(led_paths.begin(), led_paths.end(), CompareLEDPaths);
}

DetectedControllers DetectLinuxRGBLEDControllers()
{
    DetectedControllers detected_controllers;

    /*-----------------------------------------------------*\
    | Detect Steam Machine (leds-valve) LED controller      |
    \*-----------------------------------------------------*/
    if(detected_controllers.size() == 0)
    {
        std::vector<std::string> valve_led_paths;
        DetectValveLEDs(valve_led_paths);

        if(valve_led_paths.size() > 0)
        {
            LinuxRGBLEDController* controller = new LinuxRGBLEDController("Valve LEDs");

            for(unsigned int led_idx = 0; led_idx < valve_led_paths.size(); led_idx++)
            {
                controller->AddLED(valve_led_paths[led_idx]);
            }

            RGBController_LinuxRGBLED* rgb_controller = new RGBController_LinuxRGBLED(controller);

            detected_controllers.push_back(rgb_controller);
        }
    }

    return(detected_controllers);
}

REGISTER_DETECTOR("Linux RGB LED", DetectLinuxRGBLEDControllers);