#include "Detector.h"
#include "RGBController.h"
#include "RGBController_Dummy.h"
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
*   DetectDebugControllers                                                                 *
*                                                                                          *
*       Add dummy controllers based on debug.txt                                           *
*                                                                                          *
\******************************************************************************************/

void DetectDebugControllers(std::vector<RGBController*> &rgb_controllers)
{
    std::ifstream infile;
    
    //Open settings file
    infile.open("debug.txt");

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
                    if (strcmp(argument, "debug_motherboard") == 0)
                    {
                        /*---------------------------------------------------------*\
                        | Create a dummy motherboard                                |
                        \*---------------------------------------------------------*/
                        RGBController_Dummy* dummy_motherboard = new RGBController_Dummy();

                        dummy_motherboard->name                     = "Debug Motherboard";
                        dummy_motherboard->type                     = DEVICE_TYPE_MOTHERBOARD;
                        dummy_motherboard->description              = "Debug Motherboard Device";
                        dummy_motherboard->location                 = "Debug Motherboard Location";
                        dummy_motherboard->version                  = "Debug Motherboard Version";
                        dummy_motherboard->serial                   = "Debug Motherboard Serial";

                        /*---------------------------------------------------------*\
                        | Create a direct mode for the dummy motherboard            |
                        \*---------------------------------------------------------*/
                        mode dummy_motherboard_direct_mode;

                        dummy_motherboard_direct_mode.name          = "Direct";
                        dummy_motherboard_direct_mode.value         = 0;
                        dummy_motherboard_direct_mode.flags         = MODE_FLAG_HAS_PER_LED_COLOR;
                        dummy_motherboard_direct_mode.color_mode    = MODE_COLORS_PER_LED;

                        dummy_motherboard->modes.push_back(dummy_motherboard_direct_mode);

                        /*---------------------------------------------------------*\
                        | Create a single zone/LED for the dummy motherboard        |
                        \*---------------------------------------------------------*/
                        zone dummy_motherboard_single_zone;

                        dummy_motherboard_single_zone.name          = "Single Zone";
                        dummy_motherboard_single_zone.type          = ZONE_TYPE_SINGLE;
                        dummy_motherboard_single_zone.leds_min      = 1;
                        dummy_motherboard_single_zone.leds_max      = 1;
                        dummy_motherboard_single_zone.leds_count    = 1;
                        dummy_motherboard_single_zone.matrix_map    = NULL;

                        dummy_motherboard->zones.push_back(dummy_motherboard_single_zone);

                        led dummy_motherboard_single_led;

                        dummy_motherboard_single_led.name           = "Single LED";

                        dummy_motherboard->leds.push_back(dummy_motherboard_single_led);

                        /*---------------------------------------------------------*\
                        | Create a linear zone for the dummy motherboard            |
                        \*---------------------------------------------------------*/
                        zone dummy_motherboard_linear_zone;

                        dummy_motherboard_linear_zone.name          = "Linear Zone";
                        dummy_motherboard_linear_zone.type          = ZONE_TYPE_SINGLE;
                        dummy_motherboard_linear_zone.leds_min      = 10;
                        dummy_motherboard_linear_zone.leds_max      = 10;
                        dummy_motherboard_linear_zone.leds_count    = 10;
                        dummy_motherboard_linear_zone.matrix_map    = NULL;

                        dummy_motherboard->zones.push_back(dummy_motherboard_linear_zone);

                        for(std::size_t led_idx = 0; led_idx < 10; led_idx++)
                        {
                            led dummy_motherboard_linear_led;

                            dummy_motherboard_linear_led.name       = "Linear LED " + std::to_string(led_idx);

                            dummy_motherboard->leds.push_back(dummy_motherboard_linear_led);
                        }

                        dummy_motherboard->SetupColors();

                        /*---------------------------------------------------------*\
                        | Push the dummy motherboard onto the controller list       |
                        \*---------------------------------------------------------*/
                        rgb_controllers.push_back(dummy_motherboard);
                    }
                    else if(strcmp(argument, "debug_dram") == 0)
                    {
                        /*---------------------------------------------------------*\
                        | Create a dummy DRAM                                       |
                        \*---------------------------------------------------------*/
                        RGBController_Dummy* dummy_dram = new RGBController_Dummy();

                        dummy_dram->name                            = "Debug DRAM";
                        dummy_dram->type                            = DEVICE_TYPE_DRAM;
                        dummy_dram->description                     = "Debug DRAM Device";
                        dummy_dram->location                        = "Debug DRAM Location";
                        dummy_dram->version                         = "Debug DRAM Version";
                        dummy_dram->serial                          = "Debug DRAM Serial";

                        /*---------------------------------------------------------*\
                        | Create a direct mode for the dummy DRAM                   |
                        \*---------------------------------------------------------*/
                        mode dummy_dram_direct_mode;

                        dummy_dram_direct_mode.name                 = "Direct";
                        dummy_dram_direct_mode.value                = 0;
                        dummy_dram_direct_mode.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
                        dummy_dram_direct_mode.color_mode           = MODE_COLORS_PER_LED;

                        dummy_dram->modes.push_back(dummy_dram_direct_mode);

                        /*---------------------------------------------------------*\
                        | Create a single zone/LED for the dummy DRAM               |
                        \*---------------------------------------------------------*/
                        zone dummy_dram_single_zone;

                        dummy_dram_single_zone.name                 = "Single Zone";
                        dummy_dram_single_zone.type                 = ZONE_TYPE_SINGLE;
                        dummy_dram_single_zone.leds_min             = 1;
                        dummy_dram_single_zone.leds_max             = 1;
                        dummy_dram_single_zone.leds_count           = 1;
                        dummy_dram_single_zone.matrix_map           = NULL;

                        dummy_dram->zones.push_back(dummy_dram_single_zone);

                        led dummy_dram_single_led;

                        dummy_dram_single_led.name                  = "Single LED";

                        dummy_dram->leds.push_back(dummy_dram_single_led);

                        /*---------------------------------------------------------*\
                        | Create a linear zone for the dummy DRAM                   |
                        \*---------------------------------------------------------*/
                        zone dummy_dram_linear_zone;

                        dummy_dram_linear_zone.name                 = "Linear Zone";
                        dummy_dram_linear_zone.type                 = ZONE_TYPE_SINGLE;
                        dummy_dram_linear_zone.leds_min             = 5;
                        dummy_dram_linear_zone.leds_max             = 5;
                        dummy_dram_linear_zone.leds_count           = 5;
                        dummy_dram_linear_zone.matrix_map           = NULL;

                        dummy_dram->zones.push_back(dummy_dram_linear_zone);

                        for(std::size_t led_idx = 0; led_idx < 5; led_idx++)
                        {
                            led dummy_dram_linear_led;

                            dummy_dram_linear_led.name              = "Linear LED " + std::to_string(led_idx);

                            dummy_dram->leds.push_back(dummy_dram_linear_led);
                        }

                        dummy_dram->SetupColors();

                        /*---------------------------------------------------------*\
                        | Push the dummy DRAM onto the controller list              |
                        \*---------------------------------------------------------*/
                        rgb_controllers.push_back(dummy_dram);
                    }
                    else if(strcmp(argument, "debug_gpu") == 0)
                    {
                        /*---------------------------------------------------------*\
                        | Create a dummy GPU                                        |
                        \*---------------------------------------------------------*/
                        RGBController_Dummy* dummy_gpu = new RGBController_Dummy();

                        dummy_gpu->name                             = "Debug GPU";
                        dummy_gpu->type                             = DEVICE_TYPE_GPU;
                        dummy_gpu->description                      = "Debug GPU Device";
                        dummy_gpu->location                         = "Debug GPU Location";
                        dummy_gpu->version                          = "Debug GPU Version";
                        dummy_gpu->serial                           = "Debug GPU Serial";

                        /*---------------------------------------------------------*\
                        | Create a direct mode for the dummy GPU                    |
                        \*---------------------------------------------------------*/
                        mode dummy_gpu_direct_mode;

                        dummy_gpu_direct_mode.name                  = "Direct";
                        dummy_gpu_direct_mode.value                 = 0;
                        dummy_gpu_direct_mode.flags                 = MODE_FLAG_HAS_PER_LED_COLOR;
                        dummy_gpu_direct_mode.color_mode            = MODE_COLORS_PER_LED;

                        dummy_gpu->modes.push_back(dummy_gpu_direct_mode);

                        /*---------------------------------------------------------*\
                        | Create a single zone/LED for the dummy GPU                |
                        \*---------------------------------------------------------*/
                        zone dummy_gpu_single_zone;

                        dummy_gpu_single_zone.name                  = "Single Zone";
                        dummy_gpu_single_zone.type                  = ZONE_TYPE_SINGLE;
                        dummy_gpu_single_zone.leds_min              = 1;
                        dummy_gpu_single_zone.leds_max              = 1;
                        dummy_gpu_single_zone.leds_count            = 1;
                        dummy_gpu_single_zone.matrix_map            = NULL;

                        dummy_gpu->zones.push_back(dummy_gpu_single_zone);

                        led dummy_gpu_single_led;

                        dummy_gpu_single_led.name                   = "Single LED";

                        dummy_gpu->leds.push_back(dummy_gpu_single_led);

                        /*---------------------------------------------------------*\
                        | Create a linear zone for the dummy GPU                    |
                        \*---------------------------------------------------------*/
                        zone dummy_gpu_linear_zone;

                        dummy_gpu_linear_zone.name                  = "Linear Zone";
                        dummy_gpu_linear_zone.type                  = ZONE_TYPE_SINGLE;
                        dummy_gpu_linear_zone.leds_min              = 15;
                        dummy_gpu_linear_zone.leds_max              = 15;
                        dummy_gpu_linear_zone.leds_count            = 15;
                        dummy_gpu_linear_zone.matrix_map            = NULL;

                        dummy_gpu->zones.push_back(dummy_gpu_linear_zone);

                        for(std::size_t led_idx = 0; led_idx < 15; led_idx++)
                        {
                            led dummy_gpu_linear_led;

                            dummy_gpu_linear_led.name               = "Linear LED " + std::to_string(led_idx);

                            dummy_gpu->leds.push_back(dummy_gpu_linear_led);
                        }

                        dummy_gpu->SetupColors();

                        /*---------------------------------------------------------*\
                        | Push the dummy GPU onto the controller list               |
                        \*---------------------------------------------------------*/
                        rgb_controllers.push_back(dummy_gpu);
                    }
                }
            }
        }
    }
}   /* DetectDebugControllers() */

REGISTER_DETECTOR("Debug Controllers", DetectDebugControllers);
