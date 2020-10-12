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
*                                                                                          *
\******************************************************************************************/

void DetectE131Controllers(std::vector<RGBController*> &rgb_controllers)
{
    RGBController_E131* new_controller;

    std::ifstream infile;
    char arg1[64];

    std::vector<std::vector<E131Device>> device_lists; 
	E131Device dev;
	
    bool new_device = false;

    //Clear E131 device data
    dev.name          = "";
    dev.type          = ZONE_TYPE_SINGLE;
    dev.num_leds      = 0;
    dev.rgb_order     = E131_RGB_ORDER_RBG;
    dev.matrix_order  = E131_MATRIX_ORDER_HORIZONTAL_TOP_LEFT;
    dev.matrix_width  = 0;
    dev.matrix_height = 0;
    
    //Open settings file
    infile.open("e131.txt");

    if (infile.good())
    {
        for (std::string line; std::getline(infile, line); )
        {
            if (new_device)
            {
                dev.name   = line;
                new_device = false;
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
                    else if(strcmp(argument, "keepalive_time") == 0)
                    {
                        dev.keepalive_time = atoi(value);
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
                    else if(strcmp(argument, "matrix_order") == 0)
                    {
                        if(strcmp(value, "HORIZONTAL_TOP_LEFT") == 0)
                        {
                            dev.matrix_order = E131_MATRIX_ORDER_HORIZONTAL_TOP_LEFT;
                        }
                        else if(strcmp(value, "HORIZONTAL_TOP_RIGHT") == 0)
                        {
                            dev.matrix_order = E131_MATRIX_ORDER_HORIZONTAL_TOP_RIGHT;
                        }
                        else if(strcmp(value, "HORIZONTAL_BOTTOM_LEFT") == 0)
                        {
                            dev.matrix_order = E131_MATRIX_ORDER_HORIZONTAL_BOTTOM_LEFT;
                        }
                        else if(strcmp(value, "HORIZONTAL_BOTTOM_RIGHT") == 0)
                        {
                            dev.matrix_order = E131_MATRIX_ORDER_HORIZONTAL_BOTTOM_RIGHT;
                        }
                        else if(strcmp(value, "VERTICAL_TOP_LEFT") == 0)
                        {
                            dev.matrix_order = E131_MATRIX_ORDER_VERTICAL_TOP_LEFT;
                        }
                        else if(strcmp(value, "VERTICAL_TOP_RIGHT") == 0)
                        {
                            dev.matrix_order = E131_MATRIX_ORDER_VERTICAL_TOP_RIGHT;
                        }
                        else if(strcmp(value, "VERTICAL_BOTTOM_LEFT") == 0)
                        {
                            dev.matrix_order = E131_MATRIX_ORDER_VERTICAL_BOTTOM_LEFT;
                        }
                        else if(strcmp(value, "VERTICAL_BOTTOM_RIGHT") == 0)
                        {
                            dev.matrix_order = E131_MATRIX_ORDER_VERTICAL_BOTTOM_RIGHT; 
                        }
                        else
                        {
                            dev.matrix_order = atoi(value);
                        }
                    }
                    else if(strcmp(argument, "matrix_width") == 0)
                    {
                        dev.matrix_width = atoi(value);
                    }
                    else if(strcmp(argument, "matrix_height") == 0)
                    {
                        dev.matrix_height = atoi(value);
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
                        /*---------------------------------------------------------*\
                        | Determine whether to create a new list or add this device |
                        | to an existing list.  A device is added to an existing    |
                        | list if both devices share one or more universes for the  |
                        | same output destination                                   |
                        \*---------------------------------------------------------*/
                        bool device_added_to_existing_list = false;

                        for(unsigned int list_idx = 0; list_idx < device_lists.size(); list_idx++)
                        {
                            for(unsigned int device_idx = 0; device_idx < device_lists[list_idx].size(); device_idx++)
                            {
                                /*---------------------------------------------------------*\
                                | Check if any universes used by this new device exist in   |
                                | the existing device.  If so, add the new device to the    |
                                | existing list.                                            |
                                \*---------------------------------------------------------*/
                                if(dev.start_universe == device_lists[list_idx][device_idx].start_universe)
                                {
                                    device_lists[list_idx].push_back(dev);
                                    device_added_to_existing_list = true;
                                    break;
                                }
                            }

                            if(device_added_to_existing_list)
                            {
                                break;
                            }
                        }

                        /*---------------------------------------------------------*\
                        | If the device did not overlap with existing devices,      |
                        | create a new list for it                                  |
                        \*---------------------------------------------------------*/
                        if(!device_added_to_existing_list)
                        {
                            std::vector<E131Device> new_list;

                            new_list.push_back(dev);

                            device_lists.push_back(new_list);
                        }
                    }
                }
            }
        }

        for(unsigned int list_idx = 0; list_idx < device_lists.size(); list_idx++)
        {
            RGBController_E131* rgb_controller;
            rgb_controller = new RGBController_E131(device_lists[list_idx]);
            rgb_controllers.push_back(rgb_controller);
        }
    }

}   /* DetectE131Controllers() */

REGISTER_DETECTOR("E1.31", DetectE131Controllers);
