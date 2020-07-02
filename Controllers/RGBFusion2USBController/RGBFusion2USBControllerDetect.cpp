#include "RGBFusion2USBController.h"
#include "RGBController_RGBFusion2USB.h"
#include "dependencies/dmiinfo.h"

#define IT8297_VID              0x048D
#define IT8297_IFC              0
#define IT8297_UPG              0xFF89
#define COUNT_RGBFUSION2_PIDS   (sizeof(RGBFusion2_pids) / sizeof(RGBFusion2_pids[ 0 ]))

static const unsigned short RGBFusion2_pids[] =
{
    0x8297,     //PID for the ITE 8595 found on the X570
    0x5702      //PID for the ITE 8595 found on the B550
};

/******************************************************************************************\
*                                                                                          *
*   DetectRGBFusion2USBControllers                                                         *
*                                                                                          *
*       Detect RGB Fusion 2 devices that use IT8297 RGB controller                         *
*                                                                                          *
\******************************************************************************************/

void DetectRGBFusion2USBControllers(std::vector<RGBController*> &rgb_controllers)
{
    hid_device_info*    info;
    hid_device*         dev;
    DMIInfo             MB_info;
    unsigned short      tmpPID;

    if (hid_init() < 0)
    {
        return;
    }

    for(int dev_idx = 0; dev_idx < COUNT_RGBFUSION2_PIDS; dev_idx++)
    {
        dev = NULL;

        tmpPID = RGBFusion2_pids[dev_idx];
        info = hid_enumerate(IT8297_VID, tmpPID);

        while(info)
        {
            if((info->vendor_id        == IT8297_VID)
            && (info->interface_number == IT8297_IFC)
#ifdef USE_HID_USAGE
            &&(info->product_id        == tmpPID)
            &&(info->usage_page        == IT8297_UPG))
#else
            &&(info->product_id        == tmpPID))
#endif
            {
                hid_device * dev = hid_open_path(info->path);
                if (dev)
                {
                    RGBFusion2USBController * controller = new RGBFusion2USBController(dev, info->path, MB_info.getMainboard());
                    RGBController_RGBFusion2USB * rgb_controller = new RGBController_RGBFusion2USB(controller);
                    rgb_controllers.push_back(rgb_controller);
                }
            }
            info = info->next;
        }
    }

    hid_free_enumeration(info);
}
