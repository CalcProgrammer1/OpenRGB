#include "RGBController.h"
#include "RGBController_E131.h"
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
#endif

/******************************************************************************************\
*                                                                                          *
*   DetectE131Controllers                                                                  *
*                                                                                          *
*       Detect devices supported by the E131 driver                                        *
*                                                                                          *                                                                                          *
\******************************************************************************************/

void DetectE131Controllers(std::vector<RGBController*> &rgb_controllers)
{
    RGBController_E131* new_controller;

    //Get file path in executable directory
    std::ifstream infile;
    char filename[2048];
    char arg1[64];

#ifdef WIN32
    GetModuleFileName(NULL, filename, 2048);
    strcpy(filename, std::string(filename).substr(0, std::string(filename).find_last_of("\\/")).c_str());
    strcat(filename, "\\settings.txt");
#else
    snprintf(arg1, 64, "/proc/%d/exe", getpid());
    readlink(arg1, filename, 1024);
    strcpy(filename, std::string(filename).substr(0, std::string(filename).find_last_of("\\/")).c_str());
    strcat(filename, "/settings.txt");
#endif

	E131Device dev;

	dev.name = "Test";
	dev.type = ZONE_TYPE_SINGLE;
    dev.num_leds = 30;
	dev.start_universe = 20;
	dev.start_channel = 500;

    std::vector<E131Device> devices;

    devices.push_back(dev);
	
	new_controller = new RGBController_E131(devices);
	rgb_controllers.push_back(new_controller);

#if 0
    //Open settings file
    infile.open(filename);

    if (infile.good())
    {
        for (std::string line; std::getline(infile, line); )
        {
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
                    if (strcmp(argument, "ledstrip") == 0)
                    {
                        new_controller = new RGBController_LEDStrip(new_ledstrip);
                        rgb_controllers.push_back(new_controller);
                    }
                }
            }
        }
    }

#endif
}   /* DetectLEDStripControllers() */