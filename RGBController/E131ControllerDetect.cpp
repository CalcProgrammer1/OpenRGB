#include "Detector.h"
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
#else
#include <windows.h>
#endif

#ifndef WIN32
#define LPSTR           char *
#define strtok_s        strtok_r
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
    strcat(filename, "\\e131.txt");
#else
    snprintf(arg1, 64, "/proc/%d/exe", getpid());
    readlink(arg1, filename, 1024);
    strcpy(filename, std::string(filename).substr(0, std::string(filename).find_last_of("\\/")).c_str());
    strcat(filename, "/e131.txt");
#endif

	E131Device dev;
    std::vector<E131Device> devices;
	
    bool new_device = false;

    //Open settings file
    infile.open(filename);

    if (infile.good())
    {
        for (std::string line; std::getline(infile, line); )
        {
            if (new_device)
            {
                dev.name   = line;
                new_device = false;
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
                    if (strcmp(argument, "e131_device_start") == 0)
                    {
                        new_device = true;
                    }
                    else if(strcmp(argument, "num_leds") == 0)
                    {
                        dev.num_leds = atoi(value);
                    }
                    else if(strcmp(argument, "start_universe") == 0)
                    {
                        dev.start_universe = atoi(value);
                    }
                    else if(strcmp(argument, "start_channel") == 0)
                    {
                        dev.start_channel = atoi(value);
                    }
                    else if(strcmp(argument, "rgb_order") == 0)
                    {
                        if(strcmp(value, "RGB") == 0)
                        {
                            dev.rgb_order = E131_RGB_ORDER_RGB;
                        }
                        else if(strcmp(value, "RBG") == 0)
                        {
                            dev.rgb_order = E131_RGB_ORDER_RBG;
                        }
                        else if(strcmp(value, "GRB") == 0)
                        {
                            dev.rgb_order = E131_RGB_ORDER_GRB;
                        }
                        else if(strcmp(value, "GBR") == 0)
                        {
                            dev.rgb_order = E131_RGB_ORDER_GBR;
                        }
                        else if(strcmp(value, "BRG") == 0)
                        {
                            dev.rgb_order = E131_RGB_ORDER_BRG;
                        }
                        else if(strcmp(value, "BGR") == 0)
                        {
                            dev.rgb_order = E131_RGB_ORDER_BGR;
                        }
                        else
                        {
                            dev.rgb_order = atoi(value);
                        }
                    }
                    else if(strcmp(argument, "type") == 0)
                    {
                        if(strcmp(value, "SINGLE") == 0)
                        {
                            dev.type = ZONE_TYPE_SINGLE;
                        }
                        else if(strcmp(value, "LINEAR") == 0)
                        {
                            dev.type = ZONE_TYPE_LINEAR;
                        }
                        else if(strcmp(value, "MATRIX") == 0)
                        {
                            dev.type = ZONE_TYPE_MATRIX;
                        }
                        else
                        {
                            dev.type = atoi(value);
                        }   
                    }
                    else if(strcmp(argument, "e131_device_end") == 0)
                    {
                        devices.push_back(dev);
                    }
                }
            }
        }

        if(devices.size() > 0)
            {
            new_controller = new RGBController_E131(devices);
            rgb_controllers.push_back(new_controller);
            }
    }

}   /* DetectE131Controllers() */

REGISTER_DETECTOR("E1.31", DetectE131Controllers);
