#include "Detector.h"
#include "MSIMysticLightController.h"
#include "RGBController_MSIMysticLight.h"

#define MSI_USB_VID 0x1462

#define NUM_MSI_PIDS 41

static const unsigned short msi_pid_table[] =
{
    0x3EA4, // MS_3EA4
    0x4559, // MS_4459

    0x7B10, // MS_7B10
    0x7B93, // MSI B450 Gaming Pro Carbon AC
    0x7B94, // MS_7B94
    0x7B96, // MS_7B96

    0x7C34, // MS_7C34
    0x7C35, // MS_7C35
    0x7C36, // MS_7C36
    0x7C37, // MS_7C37
    0x7C42, // MS_7C42
    0x7C56, // MS_7C56
    0x7C59, // MS_7C59
    0x7C60, // MS_7C60
    0x7C67, // MS_7C67
    0x7C70, // MS_7C70
    0x7C71, // MS_7C71
    0x7C73, // MS_7C73
    0x7C75, // MS_7C75
    0x7C76, // MS_7C76
    0x7C77, // MS_7C77
    0x7C79, // MS_7C79
    0x7C80, // MS_7C80
    0x7C81, // MS_7C81
    0x7C82, // MS_7C82
    0x7C83, // MS_7C83
    0x7C84, // MS_7C84
    0x7C85, // MS_7C85
    0x7C86, // MS_7C86
    0x7C87, // MS_7C87
    0x7C88, // MS_7C88
    0x7C89, // MS_7C89
    0x7C90, // MS_7C90
    0x7C91, // MS_7C91
    0x7C92, // MS_7C92
    0x7C94, // MS_7C94
    0x7C95, // MS_7C95
    0x7C96, // MS_7C96
    0x7C98, // MS_7C98
    0x7C99, // MS_7C99

    0x905D  // MS_905D
};

/******************************************************************************************\
*                                                                                          *
*   DetectMSIMysticLightControllers                                                        *
*                                                                                          *
*       Detect MSI Mystic Light devices that use NCT6775 RGB controllers                   *
*                                                                                          *
\******************************************************************************************/

void DetectMSIMysticLightControllers(std::vector<RGBController*> &rgb_controllers)
{
    hid_device_info* info;
    hid_device* dev;

    hid_init();

    for(int device_idx = 0; device_idx < NUM_MSI_PIDS; device_idx++)
    {
        dev = NULL;

        info = hid_enumerate(MSI_USB_VID, msi_pid_table[device_idx]);

        //Look for MSI Mystic Light Controller
        while(info)
        {
            if((info->vendor_id == MSI_USB_VID)
            &&(info->product_id == msi_pid_table[device_idx]))
            {
                dev = hid_open_path(info->path);
                break;
            }
            else
            {
                info = info->next;
            }
        }

        if( dev )
        {
            MSIMysticLightController * controller = new MSIMysticLightController(dev, info->path);

            RGBController_MSIMysticLight * rgb_controller = new RGBController_MSIMysticLight(controller);
            
            rgb_controllers.push_back(rgb_controller);
        }
    }
}   /* DetectMSIMysticLightControllers() */

// The MSI Mystic Light controller is disabled due to bricking risk
// Uncomment this line to enable.  Do so at your own risk.
//REGISTER_DETECTOR("MSI Mystic Light", DetectMSIMysticLightControllers);
