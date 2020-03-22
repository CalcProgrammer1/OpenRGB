/******************************************************************************************\
*                                                                                          *
*   main.cpp                                                                               *
*                                                                                          *
*       Main function for OpenAuraSDK GUI project                                          *
*                                                                                          *
\******************************************************************************************/

#include "OpenRGB.h"
#include "ProfileManager.h"
#include "RGBController.h"
#include "i2c_smbus.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#include "OpenRGBDialog2.h"


extern std::vector<i2c_smbus_interface*> busses;
extern std::vector<RGBController*> rgb_controllers;

// See cli.cpp
extern int cli_main(int argc, char *argv[], std::vector<RGBController *> rgb_controllers_in, ProfileManager* profile_manager_in);

/******************************************************************************************\
*                                                                                          *
*   main                                                                                   *
*                                                                                          *
*       Main function.  Detects busses and Aura controllers, then opens the main window    *
*                                                                                          *
\******************************************************************************************/

int main(int argc, char* argv[])
{
    ProfileManager profile_manager(rgb_controllers);
    
    DetectRGBControllers();

    profile_manager.LoadSizeFromProfile("sizes.ors");

    if (argc > 1 && strcmp(argv[1], "--gui"))
    {
        return cli_main(argc, argv, rgb_controllers, &profile_manager);
    }

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    Ui::OpenRGBDialog2 dlg(busses, rgb_controllers, profile_manager);
    dlg.show();

    return a.exec();
}
