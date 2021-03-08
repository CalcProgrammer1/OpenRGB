#include "Detector.h"
#include "MSIMysticLight162Controller.h"
#include "MSIMysticLight185Controller.h"
#include "RGBController_MSIMysticLight162.h"
#include "RGBController_MSIMysticLight185.h"

#define MSI_USB_VID 0x1462

/*---------------------------------------------------------------------------------*\
| WARNING!                                                                          |
|                                                                                   |
|   The MSI Mystic Light controller had a bricking risk in the past.                |
|   The code has been tested on a few boards and the bricking issue has been fixed. |
|   Uncomment this line to enable for untested boards.  Do so at your own risk.     |
\*---------------------------------------------------------------------------------*/
//#define ENABLE_UNTESTED_MYSTIC_LIGHT

/******************************************************************************************\
*                                                                                          *
*   DetectMSIMysticLight162Controllers                                                     *
*                                                                                          *
*       Detect MSI Mystic Light (162-byte) devices                                         *
*                                                                                          *
\******************************************************************************************/

void DetectMSIMysticLight162Controllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if( dev )
    {
        MSIMysticLight162Controller*     controller     = new MSIMysticLight162Controller(dev, info->path);
        RGBController_MSIMysticLight162* rgb_controller = new RGBController_MSIMysticLight162(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectMSIMysticLight162Controllers() */

/******************************************************************************************\
*                                                                                          *
*   DetectMSIMysticLight185Controllers                                                     *
*                                                                                          *
*       Detect MSI Mystic Light (185-byte) devices                                         *
*                                                                                          *
\******************************************************************************************/

void DetectMSIMysticLight185Controllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if( dev )
    {
        MSIMysticLight185Controller*     controller     = new MSIMysticLight185Controller(dev, info->path);
        RGBController_MSIMysticLight185* rgb_controller = new RGBController_MSIMysticLight185(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectMSIMysticLight185Controllers() */

/*-------------------------------------------------------------*\
| MSI Mystic Light 162-byte Devices                             |
\*-------------------------------------------------------------*/
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7B85", DetectMSIMysticLight162Controllers,   MSI_USB_VID,    0x7B85);

/*-------------------------------------------------------------*\
| MSI Mystic Light 185-byte Devices                             |
\*-------------------------------------------------------------*/
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7B93", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7B93);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C35", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C35);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C75", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C75);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C91", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C91);

#ifdef ENABLE_UNTESTED_MYSTIC_LIGHT
/*-------------------------------------------------------------*\
| MSI Mystic Light 185-byte Devices                             |
\*-------------------------------------------------------------*/
REGISTER_HID_DETECTOR("MSI Mystic Light MS_3EA4", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x3EA4);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_4459", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x4459);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7B10", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7B10);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7B94", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7B94);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7B96", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7B96);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C34", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C34);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C36", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C36);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C37", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C37);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C42", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C42);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C56", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C56);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C59", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C59);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C60", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C60);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C67", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C67);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C70", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C70);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C71", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C71);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C73", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C73);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C76", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C76);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C77", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C77);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C79", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C79);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C80", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C80);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C81", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C81);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C82", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C82);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C83", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C83);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C84", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C84);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C85", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C85);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C86", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C86);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C87", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C87);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C88", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C88);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C89", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C89);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C90", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C90);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C92", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C92);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C94", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C94);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C95", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C95);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C96", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C96);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C98", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C98);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C99", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x7C99);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_905D", DetectMSIMysticLight185Controllers,   MSI_USB_VID,    0x905D);
#endif
