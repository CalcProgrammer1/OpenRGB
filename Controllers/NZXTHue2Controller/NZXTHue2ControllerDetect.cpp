#include "Detector.h"
#include "NZXTHue2Controller.h"
#include "RGBController.h"
#include "RGBController_NZXTHue2.h"
#include <hidapi/hidapi.h>

#define NZXT_VID                        0x1E71
#define NZXT_HUE_2_PID                  0x2001
#define NZXT_HUE_2_AMBIENT_PID          0x2002
#define NZXT_MOTHERBOARD_DEVICE_PID     0x2005
#define NZXT_MOTHERBOARD_DEVICE_2_PID   0x200B
#define NZXT_SMART_DEVICE_V2_PID        0x2006
#define NZXT_KRAKEN_X3_SERIES_PID       0x2007
#define NZXT_KRAKEN_X3_SERIES_RGB_PID   0x2014
#define NZXT_RGB_FAN_CONTROLLER_PID     0x2009
#define NZXT_RGB_FAN_CONTROLLER2_PID    0x2010
#define NZXT_RGB_FAN_CONTROLLER3_PID    0x200E
#define NZXT_RGB_FAN_CONTROLLER4_PID    0x2011
#define NZXT_RGB_CONTROLLER_PID         0x2012
#define NZXT_SMART_DEVICE_V2_1_PID      0x200D
#define NZXT_SMART_DEVICE_V2_2_PID      0x200F

static void spawn_hue(hid_device_info* info, const std::string& name, int rgb_channels, int fan_channels)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        NZXTHue2Controller*     controller     = new NZXTHue2Controller(dev, rgb_channels, fan_channels, info->path);
        RGBController_NZXTHue2* rgb_controller = new RGBController_NZXTHue2(controller);
        rgb_controller->name                   = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectNZXTHue2(hid_device_info* info, const std::string& name)
{
    spawn_hue(info, name, 4, 0);
}

void DetectNZXTHue2Ambient(hid_device_info* info, const std::string& name)
{
    spawn_hue(info, name, 2, 0);
}

void DetectNZXTHue2Motherboard(hid_device_info* info, const std::string& name)
{
    spawn_hue(info, name, 2, 3);
}

void DetectNZXTSmartDeviceV2(hid_device_info* info, const std::string& name)
{
    spawn_hue(info, name, 2, 3);
}

void DetectNZXTKrakenX3(hid_device_info* info, const std::string& name)
{
    spawn_hue(info, name, 3, 0);
}

void DetectNZXTFanController(hid_device_info* info, const std::string& name)
{
    spawn_hue(info, name, 2, 3);
}

void DetectNZXTFanController6Channel(hid_device_info* info, const std::string& name)
{
    spawn_hue(info, name, 6, 3);
}

void DetectNZXTRGBController(hid_device_info* info, const std::string& name)
{
    spawn_hue(info, name, 3, 0);
}

REGISTER_HID_DETECTOR("NZXT Hue 2",                DetectNZXTHue2,                  NZXT_VID, NZXT_HUE_2_PID);
REGISTER_HID_DETECTOR("NZXT Hue 2 Ambient",        DetectNZXTHue2Ambient,           NZXT_VID, NZXT_HUE_2_AMBIENT_PID);
REGISTER_HID_DETECTOR("NZXT Hue 2 Motherboard",    DetectNZXTHue2Motherboard,       NZXT_VID, NZXT_MOTHERBOARD_DEVICE_PID);
REGISTER_HID_DETECTOR("NZXT Hue 2 Motherboard",    DetectNZXTHue2Motherboard,       NZXT_VID, NZXT_MOTHERBOARD_DEVICE_2_PID);
REGISTER_HID_DETECTOR("NZXT Smart Device V2",      DetectNZXTSmartDeviceV2,         NZXT_VID, NZXT_SMART_DEVICE_V2_PID);
REGISTER_HID_DETECTOR("NZXT Kraken X3 Series",     DetectNZXTKrakenX3,              NZXT_VID, NZXT_KRAKEN_X3_SERIES_PID);
REGISTER_HID_DETECTOR("NZXT Kraken X3 Series RGB", DetectNZXTKrakenX3,              NZXT_VID, NZXT_KRAKEN_X3_SERIES_RGB_PID);
REGISTER_HID_DETECTOR("NZXT RGB & Fan Controller", DetectNZXTFanController,         NZXT_VID, NZXT_RGB_FAN_CONTROLLER_PID);
REGISTER_HID_DETECTOR("NZXT RGB & Fan Controller", DetectNZXTFanController,         NZXT_VID, NZXT_RGB_FAN_CONTROLLER2_PID);
REGISTER_HID_DETECTOR("NZXT RGB & Fan Controller", DetectNZXTFanController,         NZXT_VID, NZXT_RGB_FAN_CONTROLLER3_PID);
REGISTER_HID_DETECTOR("NZXT RGB & Fan Controller", DetectNZXTFanController6Channel, NZXT_VID, NZXT_RGB_FAN_CONTROLLER4_PID);
REGISTER_HID_DETECTOR("NZXT RGB Controller",       DetectNZXTRGBController,         NZXT_VID, NZXT_RGB_CONTROLLER_PID);
REGISTER_HID_DETECTOR("NZXT Smart Device V2",      DetectNZXTSmartDeviceV2,         NZXT_VID, NZXT_SMART_DEVICE_V2_1_PID);
REGISTER_HID_DETECTOR("NZXT Smart Device V2",      DetectNZXTSmartDeviceV2,         NZXT_VID, NZXT_SMART_DEVICE_V2_2_PID);
