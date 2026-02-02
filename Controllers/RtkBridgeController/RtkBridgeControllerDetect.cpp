#ifndef __FreeBSD__

#include "Detector.h"
#include "RGBController_RtkBridge.h"

#define RTK_BRIDGE_VID                 0x0BDA
#define RTK_BRIDGE_PID0                0x9220
#define RTK_BRIDGE_PID1                0x9201

#define RTK_HID2SCSI_PG                0xFF00
#define RTK_HID2SCSI_USAGE             0x0001

static struct usb_id id[] = {
    {RTK_BRIDGE_VID, RTK_BRIDGE_PID0},
    {RTK_BRIDGE_VID, RTK_BRIDGE_PID1},
   };

/******************************************************************************************\
*                                                                                          *
*   DetectRtkBridgeControllers                                                               *
*                                                                                          *
*       Tests the USB address to see if an Realtek Bridge controller exists there           *
*                                                                                          *
\******************************************************************************************/
void DetectRtkBridgeControllers(hid_device_info *info, const std::string &name)
{
    RtkBridgeWrapper *wrapper = NULL;
    RGBController_RtkBridge *controller = NULL;
    dHandle fd = NULL_DHANDLE;
    struct led_mem *led_mem_dev = NULL;
    char path[256] = {0};

    sprintf(path, "%s:%d", info->path, 0);
    fd = bridge_open_handle(path);
    if (INVALID_DHANDLE(fd)) {
        goto exit;
    }

    if (!led_get_ic_support_sync_light(fd, SYNC_METHOD_OPENRGB)) {
        goto exit;
    }

    if (!bridge_check_target(fd)) {
        goto exit;
    }

    if (bridge_write_unlock(fd)) {
        goto exit;
    }

    led_mem_dev = (struct led_mem *)malloc(sizeof(struct led_mem));
    if (!led_mem_dev) {
        goto exit;
    }

    memset(led_mem_dev, 0, sizeof(struct led_mem));
    if (led_mem_init(fd, led_mem_dev)) {
        free(led_mem_dev);
        goto exit;
    }
    led_mem_dev->sync_method = SYNC_METHOD_OPENRGB;

    wrapper = new RtkBridgeWrapper(fd, led_mem_dev, info);
    controller = new RGBController_RtkBridge(wrapper);
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

REGISTER_HID_DETECTOR_PU   ("RTL9220", DetectRtkBridgeControllers, RTK_BRIDGE_VID, RTK_BRIDGE_PID0, RTK_HID2SCSI_PG, RTK_HID2SCSI_USAGE);
REGISTER_HID_DETECTOR_PU   ("RTL9201", DetectRtkBridgeControllers, RTK_BRIDGE_VID, RTK_BRIDGE_PID1, RTK_HID2SCSI_PG, RTK_HID2SCSI_USAGE);

#endif
