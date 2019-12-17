/******************************************************************************************\
*                                                                                          *
*   main.cpp                                                                               *
*                                                                                          *
*       Main function for OpenAuraSDK GUI project                                          *
*                                                                                          *
\******************************************************************************************/

#include "OpenAuraSDK.h"
#include "RGBController.h"
#include "i2c_smbus.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#include "OpenAuraSDKQtDialog.h"


extern std::vector<i2c_smbus_interface*> busses;
extern std::vector<RGBController*> rgb_controllers;

/******************************************************************************************\
*                                                                                          *
*   main                                                                                   *
*                                                                                          *
*       Main function.  Detects busses and Aura controllers, then opens the main window    *
*                                                                                          *
\******************************************************************************************/

int main(int argc, char* argv[])
{
    DetectRGBControllers();

    QApplication a(argc, argv);

    Ui::OpenAuraSDKQtDialog dlg(busses, rgb_controllers);
    dlg.show();

    return a.exec();
}

/******************************************************************************************\
*                                                                                          *
*   wWinMain                                                                               *
*                                                                                          *
*       Entry point for Windows.  Calls main().                                            *
*                                                                                          *
\******************************************************************************************/

#ifdef WIN32
//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
//{
//    main(0, NULL);
//}
#endif
