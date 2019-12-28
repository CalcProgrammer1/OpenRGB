#include "RGBController.h"
#include "RGBController_AorusGPU.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>


/******************************************************************************************\
*                                                                                          *
*   DetectAorusGPUControllers                                                              *
*                                                                                          *
*       Detect devices supported by the Aorus GPU driver                                   *
*                                                                                          *                                                                                          *
\******************************************************************************************/

void DetectAorusGPUControllers(std::vector<RGBController*> &rgb_controllers)
{
    RGBController_AorusGPU * aorus_rgb = new RGBController_AorusGPU();

    rgb_controllers.push_back(aorus_rgb);
}   /* DetectLEDStripControllers() */