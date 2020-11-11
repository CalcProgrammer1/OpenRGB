/*-------------------------------------------------------------------*\
|  QMKRGBMatrixControllerDetect.cpp                                     |
|                                                                     |
|  Driver for QMK keybaords using RGB Matrix                          |
|                                                                     |
|  Kasper24         11th November 2020                                |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "Detector.h"
#include "QMKRGBMatrixController.h"
#include "RGBController.h"
#include "RGBController_QMKRGBMatrix.h"
#include <hidapi/hidapi.h>

#define MASSDROP_VID      0x04D8
#define MASSDROP_ALT_PID  0xEED3
#define MASSDROP_CTRL_PID 0xEED2

typedef struct
{
    unsigned short  usb_vid;
    unsigned short  usb_pid;
    unsigned char   usb_interface;
    const char *    name;
} qmk_rgb_matrix_device;

#define QMK_RGB_MATRIX_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[ 0 ]))

static const qmk_rgb_matrix_device device_list[] =
{
    { MASSDROP_VID, MASSDROP_ALT_PID, 1, "Massdrop Alt" },
    { MASSDROP_VID, MASSDROP_CTRL_PID, 1, "Massdrop Ctrl" },
};

/******************************************************************************************\
*                                                                                          *
*   DetectQMKRGBMatrixControllers                                                          *
*                                                                                          *
*       Tests the USB address to see if any QMK RGB Matrix controllers exists there.       *
*                                                                                          *
\******************************************************************************************/

void DetectQMKRGBMatrixControllers(std::vector<RGBController*>& rgb_controllers)
{
    hid_device_info* info;
    hid_device* dev = NULL;

    hid_init();

    for(std::size_t device_idx = 0; device_idx < QMK_RGB_MATRIX_NUM_DEVICES; device_idx++)
    {
        dev = NULL;

        info = hid_enumerate(device_list[device_idx].usb_vid, device_list[device_idx].usb_pid);

        while(info)
        {
            if
            (
                info->vendor_id == device_list[device_idx].usb_vid &&
                info->product_id == device_list[device_idx].usb_pid &&
                info->interface_number == device_list[device_idx].usb_interface
            )
            {
                dev = hid_open_path(info->path);
                if(dev)
                {
                    QMKRGBMatrixController* controller = new QMKRGBMatrixController(dev, info->path);

                    RGBController_QMKRGBMatrix* rgb_controller = new RGBController_QMKRGBMatrix(controller);

                    rgb_controllers.push_back(rgb_controller);
                }
            }
            info = info->next;
        }
    }
}   

REGISTER_DETECTOR("QMKRGBMatrix", DetectQMKRGBMatrixControllers);
