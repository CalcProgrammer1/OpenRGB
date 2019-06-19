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

#ifdef WIN32

#include "OpenAuraSDKDialog.h"

#else /* WIN32 */

#include "OpenAuraSDKQtDialog.h"

#endif /* WIN32 */

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

#if WIN32
    OpenAuraSDKDialog dlg(busses, rgb_controllers);
    dlg.DoModal();

    return 0;

#else
    QApplication a(argc, argv);

    Ui::OpenAuraSDKQtDialog dlg(busses, rgb_controllers);
    dlg.show();

    return a.exec();
#endif
}