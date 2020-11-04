#include "Detector.h"
#include "PhilipsWizController.h"
#include "RGBController.h"
#include "RGBController_PhilipsWiz.h"
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
*   DetectPhilipsWizControllers                                                            *
*                                                                                          *
*       Detect Philips Wiz devices                                                         *
*                                                                                          *
\******************************************************************************************/

void DetectPhilipsWizControllers(std::vector<RGBController*> &rgb_controllers)
{
    PhilipsWizController* new_controller;
    RGBController_PhilipsWiz* new_rgbcontroller;

    std::ifstream infile;
    char arg1[64];

    //Open settings file
    infile.open("wiz.txt");

    if (infile.good())
    {
        for (std::string line; std::getline(infile, line); )
        {
            if (line == "")
            {
                continue;
            }
            else
            {
                new_controller = new PhilipsWizController(line);
                new_rgbcontroller = new RGBController_PhilipsWiz(new_controller);

                rgb_controllers.push_back(new_rgbcontroller);
            }
        }
    }

}   /* DetectPhilipsWizControllers() */

REGISTER_DETECTOR("Philips Wiz", DetectPhilipsWizControllers);
