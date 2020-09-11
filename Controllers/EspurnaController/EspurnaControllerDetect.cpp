#include "Detector.h"
#include "EspurnaController.h"
#include "RGBController.h"
#include "RGBController_Espurna.h"
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
*   DetectEspurnaControllers                                                               *
*                                                                                          *
*       Detect devices supported by the Espurna driver                                     *
*                                                                                          *
\******************************************************************************************/

void DetectEspurnaControllers(std::vector<RGBController*> &rgb_controllers)
{
    EspurnaController* new_espurna;
    RGBController_Espurna* new_controller;

    std::ifstream infile;
    char arg1[64];

    //Open settings file
    infile.open("espurna.txt");

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
                    if (strcmp(argument, "espurna") == 0)
                    {
                        new_espurna = new EspurnaController();
                        new_espurna->Initialize(value);

                        new_controller = new RGBController_Espurna(new_espurna);
                        rgb_controllers.push_back(new_controller);
                    }
                }
            }
        }
    }

}   /* DetectEspurnaControllers() */

REGISTER_DETECTOR("Espurna", DetectEspurnaControllers);
