#ifndef __FreeBSD__

#include "Detector.h"
#include "RGBController.h"
#include "RGBController_RtkArgb.h"

#define RTK_HID2SCSI_PG                0xFF00
#define RTK_HID2SCSI_USAGE             0x0001

/******************************************************************************************\
*                                                                                          *
*   DetectRtkArgbControllers                                                               *
*                                                                                          *
*       Tests the USB address to see if an Realtek Argb controller exists there           *
*                                                                                          *
\******************************************************************************************/
void DetectRtkArgbControllers(hid_device_info *info, const std::string &name)
{
    RGBController_RtkArgb *controller = NULL;
    RtkArgbWrapper *wrapper = NULL;
    static struct argb_device *dev = NULL;
    argbCtl *argb_ctl = NULL;
    int ret = -1;

    dev = argb_path_open(info->path, BRINTF_TYPE_HID, NULL);
    if (!dev) {
        goto exit;
    }

    if (!led_argb_get_ic_support_sync_light(dev, SYNC_METHOD_OPENRGB)) {
        goto exit;
    }

    if (bridge_write_unlock(dev)) {
        goto exit;
    }

    argb_ctl = (argbCtl*)malloc(sizeof(argbCtl));
    if (!argb_ctl) {
        goto exit;
    }

    memset(argb_ctl, 0, sizeof(argbCtl));
    if (led_argb_init(dev, argb_ctl)) {
        free(argb_ctl);
        goto exit;
    }
    argb_ctl->sync_method = SYNC_METHOD_OPENRGB;

    wrapper = new RtkArgbWrapper(dev, argb_ctl, info);
    controller = new RGBController_RtkArgb(wrapper);
    if(controller->type != DEVICE_TYPE_UNKNOWN)
    {
        ResourceManager::get()->RegisterRGBController(controller);
    }
    else
    {
        delete wrapper;
        delete controller;
    }
exit:
    return;
}

REGISTER_HID_DETECTOR_PU   ("RTL9209", DetectRtkArgbControllers, RTK_ARGB_VID, RTK_ARGB_PID, RTK_HID2SCSI_PG, RTK_HID2SCSI_USAGE);

#endif
