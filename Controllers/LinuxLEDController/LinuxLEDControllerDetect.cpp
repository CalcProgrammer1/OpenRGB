#include "Detector.h"
#include "LinuxLEDController.h"
#include "RGBController.h"
#include "RGBController_LinuxLED.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <string>

#ifndef WIN32
#include <unistd.h>
#include <dirent.h>
#else
#include <windows.h>
#endif

#ifndef WIN32
#define LPSTR           char *
#define strtok_s        strtok_r
#endif


/******************************************************************************************\
*                                                                                          *
*   DetectLinuxLEDControllers                                                              *
*                                                                                          *
*       Detect devices supported by the LinuxLED driver                                    *
*                                                                                          *
\******************************************************************************************/

void DetectLinuxLEDControllers(std::vector<RGBController*> &rgb_controllers)
{
    std::ifstream   infile;
    char            arg1[64];
    bool            new_device = false;
    std::string     new_name;

    //Open settings file
    infile.open("linuxled.txt");

    LinuxLEDController*     new_controller;
    RGBController_LinuxLED* new_rgbcontroller;

    if (infile.good())
    {
        for (std::string line; std::getline(infile, line); )
        {
            if (new_device)
            {
                new_name    = line;
                new_device  = false;
                continue;
            }

            if (line == "")
            {
                continue;
            }

            if ((line[0] != ';') && (line[0] != '#') && (line[0] != '/'))
            {
                char * argument;
                char * value;

                value = (char *)line.c_str();

                argument = strtok_s(value, "=", &value);

                //Strip off new line characters if present
                argument = strtok(argument, "\r\n");
                value    = strtok(value, "\r\n");

                if(argument)
                {
                    if (strcmp(argument, "linux_led_start") == 0)
                    {
                        new_controller      = new LinuxLEDController();
                        new_device          = true;
                    }
                    else if(strcmp(argument, "red_path") == 0)
                    {
                        new_controller->OpenRedPath(value);
                    }
                    else if(strcmp(argument, "green_path") == 0)
                    {
                        new_controller->OpenGreenPath(value);
                    }
                    else if(strcmp(argument, "blue_path") == 0)
                    {
                        new_controller->OpenBluePath(value);
                    }
                    else if(strcmp(argument, "linux_led_end") == 0)
                    {
                        new_rgbcontroller       = new RGBController_LinuxLED(new_controller);
                        new_rgbcontroller->name = new_name;
                        rgb_controllers.push_back(new_rgbcontroller);
                    }
                }
            }
        }
    }
}   /* DetectLinuxLEDControllers() */

REGISTER_DETECTOR("Linux LED", DetectLinuxLEDControllers);
