#include "Detector.h"
#include "LEDStripController.h"
#include "RGBController.h"
#include "RGBController_LEDStrip.h"
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
*   DetectLEDStripControllers                                                              *
*                                                                                          *
*       Detect devices supported by the LEDStrip driver                                    *
*                                                                                          *
\******************************************************************************************/

void DetectLEDStripControllers(std::vector<RGBController*> &rgb_controllers)
{
    LEDStripController* new_ledstrip;
    RGBController_LEDStrip* new_controller;

    std::ifstream infile;
    char arg1[64];

    //Open settings file
    infile.open("ledstrip.txt");

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
                        new_ledstrip = new LEDStripController();
                        new_ledstrip->Initialize(value);

                        new_controller = new RGBController_LEDStrip(new_ledstrip);
                        rgb_controllers.push_back(new_controller);
                    }
                }
            }
        }
    }

}   /* DetectLEDStripControllers() */

REGISTER_DETECTOR("LED Strip", DetectLEDStripControllers);
