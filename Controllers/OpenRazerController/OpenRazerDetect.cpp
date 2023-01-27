#include "Detector.h"
#include "LogManager.h"
#include "RGBController.h"
#include "RGBController_OpenRazer.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

/******************************************************************************************\
*                                                                                          *
*   DetectOpenRazerControllers                                                             *
*                                                                                          *
*       Detect devices supported by the OpenRazer kernel drivers                           *
*                                                                                          *
\******************************************************************************************/

void DetectOpenRazerControllers()
{
    char driver_path[512];
    DIR *dir;
    struct dirent *ent;
    bool done = false;
    int driver_to_read = 0;

    while(driver_to_read < 8)
    {
        switch(driver_to_read)
        {
        case 0:
            strcpy(driver_path, "/sys/bus/hid/drivers/razerkbd/");
            break;

        case 1:
            strcpy(driver_path, "/sys/bus/hid/drivers/razermouse/");
            break;

        case 2:
            strcpy(driver_path, "/sys/bus/hid/drivers/razerfirefly/");
            break;

        case 3:
            strcpy(driver_path, "/sys/bus/hid/drivers/razermug/");
            break;

        case 4:
            strcpy(driver_path, "/sys/bus/hid/drivers/razercore/");
            break;

        case 5:
            strcpy(driver_path, "/sys/bus/hid/drivers/razerkraken/");
            break;

        case 6:
            strcpy(driver_path, "/sys/bus/hid/drivers/razermousemat/");
            break;

        case 7:
            strcpy(driver_path, "/sys/bus/hid/drivers/razeraccessory/");
            break;
        }

        done = false;

        dir = opendir(driver_path);

        LOG_DEBUG("[OpenRazer] Folder %s is %s", driver_path, (dir == NULL)?"not found":"found look for driver..." );

        if(dir == NULL)
        {
            driver_to_read++;
            continue;
        }

        ent = readdir(dir);

        while(ent != NULL)
        {
            if(ent->d_type == DT_DIR || ent->d_type == DT_LNK)
            {
                if(!strcmp(ent->d_name, "."))
                {
                    if(done == false)
                    {
                        done = true;
                    }
                    else
                    {
                        break;
                    }
                }
                else if(!strcmp(ent->d_name, "..") || !strcmp(ent->d_name, "module"))
                {
                }
                else
                {
                    char device_string[1024];
                    strcpy(device_string, driver_path);
                    strcat(device_string, ent->d_name);

                    RGBController_OpenRazer * razer_rgb = new RGBController_OpenRazer(device_string);

                    if(razer_rgb->device_index != -1)
                    {
                        ResourceManager::get()->RegisterRGBController(razer_rgb);
                    }
                    else
                    {
                        LOG_DEBUG("[OpenRazer] Device index is not -1 delete controller");
                        delete razer_rgb;
                    }

                }
            }

            ent = readdir(dir);
        }

        closedir(dir);

        driver_to_read++;
    }

}   /* DetectOpenRazerControllers() */

REGISTER_DETECTOR("OpenRazer", DetectOpenRazerControllers);
