#include "HuePlusController.h"
#include "RGBController.h"
#include "RGBController_HuePlus.h"
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
*   DetectHuePlusControllers                                                               *
*                                                                                          *
*       Detect devices supported by the HuePlus driver                                     *
*                                                                                          *                                                                                          *
\******************************************************************************************/

void DetectHuePlusControllers(std::vector<RGBController*> &rgb_controllers)
{
    HuePlusController* new_hueplus;
    RGBController_HuePlus* new_controller;

    //Get file path in executable directory
    std::ifstream infile;
    char filename[2048];
    char arg1[64];

#ifdef WIN32
    GetModuleFileName(NULL, filename, 2048);
    strcpy(filename, std::string(filename).substr(0, std::string(filename).find_last_of("\\/")).c_str());
#else
    snprintf(arg1, 64, "/proc/%d/exe", getpid());
    readlink(arg1, filename, 1024);
    strcpy(filename, std::string(filename).substr(0, std::string(filename).find_last_of("\\/")).c_str());
#endif

    strcat(filename, "/settings.txt");

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
                    if (strcmp(argument, "hueplus") == 0)
                    {
                        new_hueplus = new HuePlusController();
                        new_hueplus->Initialize(value);

                        new_controller = new RGBController_HuePlus(new_hueplus);
                        rgb_controllers.push_back(new_controller);
                    }
                }
            }
        }
    }


}   /* DetectHuePlusControllers() */
