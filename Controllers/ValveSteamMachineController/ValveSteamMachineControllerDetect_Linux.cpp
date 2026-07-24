/*---------------------------------------------------------*\
| ValveSteamMachineControllerDetect_Linux.cpp               |
|                                                           |
|   Detector for Valve Steam Machine LEDs                   |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      23 Jul 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include "DetectionManager.h"
#include "filesystem.h"
#include "RGBController_ValveSteamMachine_Linux.h"

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
| Sort from highest to lowest, as Steam Machine LEDs go     |
| from 0 on the right to 16 on the left                     |
\*---------------------------------------------------------*/
static bool CompareLEDPaths(const std::string& a, const std::string& b)
{
    return(GetLEDNumber(a) > GetLEDNumber(b));
}

/*---------------------------------------------------------*\
| Detect leds-valve sysfs LEDs                              |
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
    
        if(led_dir.find("valve-leds") != std::string::npos)
        {
            led_paths.push_back(led_dir);
        }
    }

    /*-----------------------------------------------------*\
    | Sort the LED paths numerically by their bracket       |
    | enclosed index to ensure correct ordering from [N]    |
    | to [0]                                                |
    \*-----------------------------------------------------*/
    std::sort(led_paths.begin(), led_paths.end(), CompareLEDPaths);
}

DetectedControllers DetectValveSteamMachineControllers()
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
            ValveSteamMachineController* controller = new ValveSteamMachineController("Valve Steam Machine");

            for(unsigned int led_idx = 0; led_idx < valve_led_paths.size(); led_idx++)
            {
                controller->AddLED(valve_led_paths[led_idx]);
            }

            RGBController_ValveSteamMachine* rgb_controller = new RGBController_ValveSteamMachine(controller);

            detected_controllers.push_back(rgb_controller);
        }
    }

    return(detected_controllers);
}

REGISTER_DETECTOR("Valve Steam Machine", DetectValveSteamMachineControllers);