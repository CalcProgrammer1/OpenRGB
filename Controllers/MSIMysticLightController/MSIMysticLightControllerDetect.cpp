#include "Detector.h"
#include "MSIMysticLightController.h"
#include "RGBController_MSIMysticLight.h"

#define MSI_USB_VID 0x1462

/******************************************************************************************\
*                                                                                          *
*   DetectMSIMysticLightControllers                                                        *
*                                                                                          *
*       Detect MSI Mystic Light devices that use NCT6775 RGB controllers                   *
*                                                                                          *
\******************************************************************************************/

//         WARNING!
//
// The MSI Mystic Light controller is disabled due to bricking risk
// Uncomment this line to enable.  Do so at your own risk.
// #define ENABLE_MYSTIC_LIGHT

void DetectMSIMysticLightControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if( dev )
    {
        MSIMysticLightController * controller = new MSIMysticLightController(dev, info->path);
        RGBController_MSIMysticLight * rgb_controller = new RGBController_MSIMysticLight(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectMSIMysticLightControllers() */

#ifdef ENABLE_MYSTIC_LIGHT
REGISTER_HID_DETECTOR("MSI Mystic Light MS_3EA4", DetectMSIMysticLightControllers, MSI_USB_VID, 0x3EA4);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_4459", DetectMSIMysticLightControllers, MSI_USB_VID, 0x4459);

REGISTER_HID_DETECTOR("MSI Mystic Light MS_7B10", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7B10);
REGISTER_HID_DETECTOR("MSI B450 Pro Carbon AC",   DetectMSIMysticLightControllers, MSI_USB_VID, 0x7B93);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7B94", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7B94);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7B96", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7B96);

REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C34", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C34);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C35", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C35);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C36", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C36);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C37", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C37);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C42", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C42);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C56", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C56);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C59", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C59);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C60", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C60);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C67", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C67);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C70", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C70);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C71", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C71);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C73", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C73);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C75", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C75);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C76", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C76);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C77", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C77);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C79", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C79);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C80", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C80);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C81", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C81);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C82", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C82);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C83", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C83);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C84", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C84);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C85", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C85);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C86", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C86);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C87", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C87);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C88", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C88);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C89", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C89);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C90", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C90);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C91", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C91);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C92", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C92);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C94", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C94);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C95", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C95);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C96", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C96);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C98", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C98);
REGISTER_HID_DETECTOR("MSI Mystic Light MS_7C99", DetectMSIMysticLightControllers, MSI_USB_VID, 0x7C99);

REGISTER_HID_DETECTOR("MSI Mystic Light MS_905D", DetectMSIMysticLightControllers, MSI_USB_VID, 0x905D);
#endif
