/*-----------------------------------------*\
|  HyperXQuadcastSControllerDetect.cpp      |
|                                           |
|  Implementation for the HyperX            |
|  Quadcast S RGB microphone                |
|                                           |
|  Matt Silva (thesilvanator) 2022          |
\*-----------------------------------------*/

#include "Detector.h"
#include "HyperXQuadcastSController.h"
#include "RGBController.h"
#include "RGBController_HyperXQuadcastS.h"
#include <vector>
#include <LogManager.h>

#ifdef __linux__
#include <dlfcn.h>
#endif

#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| HyperX microphone vendor and product IDs              |
\*-----------------------------------------------------*/
#define HYPERX_VID          0x0951
#define HYPERX_HP_VID       0x03F0

#define HYPERX_QS_PID       0x171F

#define HYPERX_QS_PID_HP_1  0x0F8B
#define HYPERX_QS_PID_HP_2  0x068C

const char* name = "HyperX Quadcast S";

#ifdef __linux__
void FindAndAddHyperXQuadcastSDevice(unsigned int vid, unsigned int pid,
                                     HXQS_HIDAPI_WRAPPER wrapper,
                                     std::vector<RGBController*>& rgb_controllers)
{
    std::string path;

    hid_device_info* devs = NULL;
    hid_device *dev       = NULL;

    HyperXQuadcastSController*     controller;
    RGBController_HyperXQuadcastS* rgb_controller;

    /*-----------------------------------------*\
    |  Iterate over devices with corresponding  |
    |  VID and PID and check if there is one    |
    |  with interface 0 as that is what we use  |
    \*-----------------------------------------*/
    if(!(devs = wrapper.enumerate(vid, pid)))
    {
        LOG_DEBUG("[%s] Dynamic call to hid_enumerate failed or couldn't find a device (Linux only)", name);
        return;
    }

    hid_device_info *curr = devs;
    while(curr)
    {
        if(curr->interface_number == 0)
        {
            path = curr->path;
            LOG_DEBUG("[%s] Found device with correct 0 interface (Linux only): %s", name, path.c_str());
            break;
        }
        curr = curr->next;
    }

    wrapper.free_enumeration(devs);

    if(!curr)
    {
        LOG_ERROR("[%s] Unable to find device with 0 interface (Linux only)", name);
        return;
    }


    if(!(dev = wrapper.open_path(path.c_str())))
    {
        LOG_ERROR("[%s] Dynamic call to hid_open_path failed (Linux only)", name);
        return;
    }

    LOG_DEBUG("[%s] Sucessfully opened device as a hidapi_device (Linux only)", name);


    controller     = new HyperXQuadcastSController(dev, wrapper, path);
    rgb_controller = new RGBController_HyperXQuadcastS(controller);

    rgb_controller->name = name;
    rgb_controllers.push_back(rgb_controller);
}

void DetectHyperXQuadcastSControllers(std::vector<RGBController*>& rgb_controllers)
{
    /*-----------------------------------------*\
    |  Dynamically load hidapi-libusb and setup |
    |  wrapper for Linux platforms              |
    \*-----------------------------------------*/
    void* dyn_handle = NULL;
    HXQS_HIDAPI_WRAPPER wrapper;

    if(!(dyn_handle = dlopen("libhidapi-libusb.so", RTLD_NOW | RTLD_NODELETE | RTLD_DEEPBIND)))
    {
        LOG_ERROR("[%s] Couldn't dynamically load hidapi-libusb (Linux only): %s", name, dlerror());
        return;
    }

    wrapper =
    {
        .dyn_handle             = dyn_handle,
        .send_feature_report    = (HXQS_Report_Send_t)          dlsym(dyn_handle,"hid_send_feature_report"),
        .get_feature_report     = (HXQS_Report_Get_t)           dlsym(dyn_handle,"hid_get_feature_report"),
        .get_serial_num_string  = (HXQS_Get_Serial_t)           dlsym(dyn_handle,"hid_get_serial_number_string"),
        .open_path              = (HXQS_hid_open_path_t)        dlsym(dyn_handle,"hid_open_path"),
        .enumerate              = (HXQS_hid_enumerate_t)        dlsym(dyn_handle,"hid_enumerate"),
        .free_enumeration       = (HXQS_hid_free_enumeration_t) dlsym(dyn_handle,"hid_free_enumeration"),
        .close                  = (HXQS_hid_close_t)            dlsym(dyn_handle,"hid_close"),
        .error                  = (HXQS_hid_error_t)            dlsym(dyn_handle,"hid_free_enumeration")
    };

    if(!(wrapper.send_feature_report && wrapper.get_feature_report &&
         wrapper.open_path && wrapper.enumerate && wrapper.free_enumeration &&
         wrapper.close && wrapper.error && wrapper.get_serial_num_string))
    {
        LOG_ERROR("[%s] Couldn't dynamically load one of hidapi-libusb functions for the wrapper (Linux only)", name);
        return;
    }

    FindAndAddHyperXQuadcastSDevice(HYPERX_VID,    HYPERX_QS_PID,      wrapper, rgb_controllers);
    FindAndAddHyperXQuadcastSDevice(HYPERX_HP_VID, HYPERX_QS_PID_HP_1, wrapper, rgb_controllers);
    FindAndAddHyperXQuadcastSDevice(HYPERX_HP_VID, HYPERX_QS_PID_HP_2, wrapper, rgb_controllers);
}

REGISTER_DETECTOR("HyperX Quadcast S", DetectHyperXQuadcastSControllers);
#else
void DetectHyperXQuadcastSControllers(hid_device_info* info, const std::string& name)
{
    hid_device *dev = hid_open_path(info->path);

    if(!dev)
    {
        LOG_ERROR("[%s] Unable to open device via hid_open_path(%s): %ls", name.c_str(), info->path, hid_error(dev));
        return;
    }

    /*-----------------------------------------*\
    |  Setup wrapper for Windows platforms just |
    |  using the already linked in hidapi       |
    |  functions                                |
    \*-----------------------------------------*/
    HXQS_HIDAPI_WRAPPER wrapper =
    {
        NULL,                       // dyn_handle
        hid_send_feature_report,
        hid_get_feature_report,
        hid_get_serial_number_string,
        hid_open_path,
        hid_enumerate,
        hid_free_enumeration,
        hid_close,
        hid_error
    };


    HyperXQuadcastSController* controller         = new HyperXQuadcastSController(dev, wrapper, info->path);
    RGBController_HyperXQuadcastS *rgb_controller = new RGBController_HyperXQuadcastS(controller);

    rgb_controller->name = name;
    ResourceManager::get()->RegisterRGBController(rgb_controller);

}
REGISTER_HID_DETECTOR_IPU("HyperX Quadcast S", DetectHyperXQuadcastSControllers, HYPERX_VID,    HYPERX_QS_PID,      0, 0xFF90, 0xFF00);
REGISTER_HID_DETECTOR_IPU("HyperX Quadcast S", DetectHyperXQuadcastSControllers, HYPERX_HP_VID, HYPERX_QS_PID_HP_1, 0, 0xFF90, 0xFF00);
REGISTER_HID_DETECTOR_IPU("HyperX Quadcast S", DetectHyperXQuadcastSControllers, HYPERX_HP_VID, HYPERX_QS_PID_HP_2, 0, 0xFF90, 0xFF00);
#endif
