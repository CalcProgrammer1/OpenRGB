/*-------------------------------------------------------------------*\
|  RoccatVulcanKeyboardController.h                                   |
|                                                                     |
|  Driver for Roccat Vulcan Keyboard                                  |
|                                                                     |
|  Mola19 17/12/2021                                                  |
|                                                                     |
\*-------------------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RoccatVulcanKeyboardLayouts.h"

#include <string>
#include <hidapi/hidapi.h>

enum
{
    ROCCAT_VULCAN_100_AIMO_PID  = 0x307A,
    ROCCAT_VULCAN_120_AIMO_PID  = 0x3098,
    ROCCAT_VULCAN_TKL_PID       = 0x2FEE,
    ROCCAT_PYRO_PID             = 0x314C,
    ROCCAT_MAGMA_PID            = 0x3124,
    ROCCAT_MAGMA_MINI_PID       = 0x69A0,
};

enum
{
    ROCCAT_VULCAN_MODE_DIRECT   = 0x0B,
    ROCCAT_VULCAN_MODE_STATIC   = 0x01,
    ROCCAT_VULCAN_MODE_WAVE     = 0x0A,
    /*-------------------------------------------------------------------*\
    |  This mode is not a real mode, it's just the default mode when      |
    |  a mode is software generated, but Swarm is inactive, hence it has  |
    |  no id. Unfortunately 0 is refused by some keyboards, so 2 seems    |
    |  like a good choice as it is not used anywhere else                 |
    \*-------------------------------------------------------------------*/
    ROCCAT_VULCAN_MODE_DEFAULT  = 0x02,
};

enum
{
    ROCCAT_VULCAN_SPEED_MIN             = 0x01,
    ROCCAT_VULCAN_SPEED_MAX             = 0x0B,
    ROCCAT_VULCAN_SPEED_DEFAULT         = 0x06,
    ROCCAT_VULCAN_BRIGHTNESS_MIN        = 0x01,
    ROCCAT_VULCAN_BRIGHTNESS_MAX        = 0x45,
    ROCCAT_VULCAN_BRIGHTNESS_DEFAULT    = 0x45,
};

struct device_info
{
    std::string     version;
    int             layout_type;
};

struct led_color
{
    unsigned int    value;
    RGBColor        color;
};

class RoccatVulcanKeyboardController
{
public:
    RoccatVulcanKeyboardController(hid_device* dev_ctrl_handle, hid_device* dev_led_handle, char *path, uint16_t pid);
    ~RoccatVulcanKeyboardController();

    std::string     GetSerial();
    std::string     GetLocation();
    device_info     InitDeviceInfo();
    device_info     GetDeviceInfo();

    void            EnableDirect(bool on_off_switch);
    void            SendColors(std::vector<led_color> colors);
    void            SendMode(unsigned int mode, unsigned int speed, unsigned int brightness, std::vector<led_color> colors);
    void            WaitUntilReady();
    void            AwaitResponse(int ms);
    void            ClearResponses();

    uint16_t        device_pid;

private:
    std::string     location;
    hid_device*     dev_ctrl;
    hid_device*     dev_led;
    device_info     dev_info;
};
