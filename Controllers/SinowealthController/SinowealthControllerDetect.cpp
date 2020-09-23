#include "Detector.h"
#include "SinowealthController.h"
#include "RGBController.h"
#include "RGBController_Sinowealth.h"
#include <vector>
#include <hidapi/hidapi.h>

#define SINOWEALTH_VID       0x258A
#define Glorious_Model_O_PID 0x0036
#define Glorious_Model_D_PID 0x0033
#define Everest_GT100_PID    0x0029

typedef struct
{
    unsigned short  usb_vid;
    unsigned short  usb_pid;
    const char *    name;
} sinowealth_device;

#define SINOWEALTH_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[ 0 ]))

static const sinowealth_device device_list[] =
{
    { SINOWEALTH_VID, Glorious_Model_O_PID, "Glorious Model O / O- " },
    { SINOWEALTH_VID, Glorious_Model_D_PID, "Glorious Model D / D- " },
    { SINOWEALTH_VID, Everest_GT100_PID   , "Everest GT-100 RGB"     },
};

/******************************************************************************************\
*                                                                                          *
*   DetectSinowealthControllers                                                            *
*                                                                                          *
*       Tests the USB address to see if a Sinowealth controller exists there.              *
*                                                                                          *
\******************************************************************************************/

void DetectSinowealthControllers(std::vector<RGBController*>& rgb_controllers)
{
    hid_device_info* info;
    int pass = 0;

    hid_init();
    for(std::size_t device_idx = 0; device_idx < SINOWEALTH_NUM_DEVICES; device_idx++)
    {

        info = hid_enumerate(device_list[device_idx].usb_vid, device_list[device_idx].usb_pid);

        /*-------------------------------------------------------------------------------------------------*\
        | Sinowealth devices use 3 different Report IDs on the same Usage Page.                             |
        | The 4 on 0xFF00 is for RGB, 7 is Unknown and 5 is for Commands.                                   |
        \*-------------------------------------------------------------------------------------------------*/

        //Look for Sinowealth Peripheral
        while(info)
        {
            if((info->vendor_id == device_list[device_idx].usb_vid)
#ifdef USE_HID_USAGE
            &&(info->product_id == device_list[device_idx].usb_pid)
            &&(info->usage_page == 0xFF00))
#else
            &&(info->product_id == device_list[device_idx].usb_pid)
            &&(info->interface_number == 1))
#endif
            {

                //Open first
                pass = 1;
                hid_device* dev_report_id_4 = hid_open_path(info->path);

                if( dev_report_id_4 )
                {

#ifdef USE_HID_USAGE


                    hid_device_info* tmp_info_id_7 = info;

                    while(tmp_info_id_7)
                    {
                        if((tmp_info_id_7->vendor_id == device_list[device_idx].usb_vid)
                        &&(tmp_info_id_7->product_id == device_list[device_idx].usb_pid)
                        &&(tmp_info_id_7->usage_page == 0xFF00))
                        {
                            //skip second
                            tmp_info_id_7 = tmp_info_id_7->next;
                            hid_device_info* tmp_info_id_5 = tmp_info_id_7;

                            while(tmp_info_id_5)
                                {
                                    if((tmp_info_id_5->vendor_id == device_list[device_idx].usb_vid)
                                    &&(tmp_info_id_5->product_id == device_list[device_idx].usb_pid)
                                    &&(tmp_info_id_5->usage_page == 0xFF00))
                                    {
                                        //Open third
                                        hid_device* dev_report_id_5 = hid_open_path(tmp_info_id_5->path);
                                        if(dev_report_id_5)
                                        {
                                            SinowealthController* controller = new SinowealthController(dev_report_id_4, dev_report_id_5, info->path);

                                            RGBController_Sinowealth* rgb_controller = new RGBController_Sinowealth(controller);

                                            rgb_controller->name = device_list[device_idx].name;

                                            rgb_controllers.push_back(rgb_controller);

                                            int pass = 3;
                                        }
                                    }
                                    if (pass == 2)
                                    {
                                        tmp_info_id_5 = tmp_info_id_5->next;
                                    }
                                    else break;
                                }


                        }
                        if (pass == 1)
                        {
                            tmp_info_id_7 = tmp_info_id_7->next;
                        }
                        else break;
                    }


#else
                    SinowealthController* controller = new SinowealthController(dev_report_id_4, dev_report_id_4, info->path);

                    RGBController_Sinowealth* rgb_controller = new RGBController_Sinowealth(controller);

                    rgb_controller->name = device_list[device_idx].name;

                    rgb_controllers.push_back(rgb_controller);
#endif

                }
            }
            if (pass == 0)
            {
                info = info->next;
            }
            else break;
        }
    }
}

REGISTER_DETECTOR("Sinowealth Device", DetectSinowealthControllers);
