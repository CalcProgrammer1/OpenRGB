#include "AMDWraithPrismController.h"
#include "RGBController.h"
#include "RGBController_AMDWraithPrism.h"
#include <vector>
#include <hidapi/hidapi.h>
#define AMD_WRAITH_PRISM_VID 0x2516
#define AMD_WRAITH_PRISM_PID 0x0051

/******************************************************************************************\
*                                                                                          *
*   DetectAMDWraithPrismControllers                                                        *
*                                                                                          *
*       Tests the USB address to see if an AMD Wraith Prism controller exists there.       *
*                                                                                          *
\******************************************************************************************/

void DetectAMDWraithPrismControllers(std::vector<RGBController*>& rgb_controllers)
{
    hid_device_info* info;
    hid_device* dev = NULL;

    hid_init();

    info = hid_enumerate(AMD_WRAITH_PRISM_VID, AMD_WRAITH_PRISM_PID);

    //Look for AMD Wraith Prism
    while(info)
    {
        if((info->vendor_id == AMD_WRAITH_PRISM_VID)
         &&(info->product_id == AMD_WRAITH_PRISM_PID)
#if USE_HID_USAGE
         &&(info->interface_number == 1)
         &&(info->usage_page == 0xFF00))
#else
         &&(info->interface_number == 1))
#endif
        {
            dev = hid_open_path(info->path);
        
            if( dev )
            {
                AMDWraithPrismController* controller = new AMDWraithPrismController(dev);

                RGBController_AMDWraithPrism* rgb_controller = new RGBController_AMDWraithPrism(controller);

                rgb_controllers.push_back(rgb_controller);
            }
        }
        info = info->next;
    }
}
