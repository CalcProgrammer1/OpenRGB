/******************************************************************************************\
*                                                                                          *
*   main.cpp                                                                               *
*                                                                                          *
*       Main function for OpenAuraSDK GUI project                                          *
*                                                                                          *
\******************************************************************************************/

#include "NetworkServer.h"
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
extern unsigned int cli_main(int argc, char *argv[], std::vector<RGBController *> rgb_controllers_in, ProfileManager* profile_manager_in);

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

    unsigned int ret_flags = 0;
    if(argc > 1)
    {
        ret_flags = cli_main(argc, argv, rgb_controllers, &profile_manager);
    }

    if(ret_flags && 2)
    {
        //GUI is enabled
    }

    bool show_i2c_tools = false;
    if(ret_flags && 4)
    {
        //I2C Tools is enabled
        show_i2c_tools = true;
    }

    NetworkServer server(rgb_controllers);
    
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    Ui::OpenRGBDialog2 dlg(busses, rgb_controllers, &profile_manager, &server, show_i2c_tools);
    dlg.show();

    return a.exec();
}
