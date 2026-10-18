/*---------------------------------------------------------*\
| FrameworkDesktopController_Linux.cpp                      |
|                                                           |
|   Driver for the Framework Desktop ARGB header,           |
|   controlled through the ChromeOS Embedded Controller     |
|   (cros_ec)                                               |
|                                                           |
|   Francis De Brabandere (francisdb@gmail.com) 16 Oct 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cerrno>
#include <cstdint>
#include <cstring>
#include <sys/ioctl.h>
#include <unistd.h>
#include "FrameworkDesktopController_Linux.h"
#include "LogManager.h"

/*---------------------------------------------------------*\
| cros_ec character device interface, as defined by the     |
| kernel in include/linux/platform_data/cros_ec_chardev.h   |
| (not exported to userspace headers).  The kernel struct   |
| ends in a flexible data array, so the ioctl number only   |
| encodes the size of the fixed header.                     |
\*---------------------------------------------------------*/
struct cros_ec_command_v2_header
{
    uint32_t    version;
    uint32_t    command;
    uint32_t    outsize;
    uint32_t    insize;
    uint32_t    result;
};

struct cros_ec_command_v2
{
    cros_ec_command_v2_header   header;
    uint8_t                     data[256];
};

#define CROS_EC_DEV_IOC_V2          0xEC
#define CROS_EC_DEV_IOCXCMD_V2      _IOWR(CROS_EC_DEV_IOC_V2, 0, struct cros_ec_command_v2_header)

/*---------------------------------------------------------*\
| EC_CMD_RGBKBD_SET_COLOR parameters:                       |
|   uint8_t start_key                                       |
|   uint8_t length                                          |
|   struct { uint8_t r, g, b; } color[length]               |
|                                                           |
| This is the command sent by framework_tool --rgbkbd       |
| https://github.com/FrameworkComputer/framework-system     |
\*---------------------------------------------------------*/
#define EC_CMD_RGBKBD_SET_COLOR     0x013A

FrameworkDesktopController::FrameworkDesktopController(int fd, std::string dev_path)
{
    this->fd        = fd;
    location        = dev_path;
    last_led_count  = 0;
}

FrameworkDesktopController::~FrameworkDesktopController()
{
    close(fd);
}

std::string FrameworkDesktopController::GetLocation()
{
    return(location);
}

void FrameworkDesktopController::SetLEDs(const std::vector<RGBColor>& colors)
{
    std::vector<RGBColor> send_colors = colors;

    /*-----------------------------------------------------*\
    | The EC keeps refreshing every LED it has been sent    |
    | since it booted.  When the LED count is reduced, turn |
    | the LEDs that are no longer used off once.            |
    \*-----------------------------------------------------*/
    if(send_colors.size() < last_led_count)
    {
        send_colors.resize(last_led_count, ToRGBColor(0, 0, 0));
    }

    if(send_colors.size() > FRAMEWORK_DESKTOP_MAX_LEDS)
    {
        send_colors.resize(FRAMEWORK_DESKTOP_MAX_LEDS);
    }

    last_led_count = colors.size();

    for(std::size_t start = 0; start < send_colors.size(); start += FRAMEWORK_DESKTOP_LEDS_PER_CMD)
    {
        std::size_t count = send_colors.size() - start;

        if(count > FRAMEWORK_DESKTOP_LEDS_PER_CMD)
        {
            count = FRAMEWORK_DESKTOP_LEDS_PER_CMD;
        }

        SendColors((unsigned char)start, &send_colors[start], (unsigned char)count);
    }
}

void FrameworkDesktopController::SendColors(unsigned char start, const RGBColor* colors, unsigned char count)
{
    cros_ec_command_v2 cmd;

    memset(&cmd, 0, sizeof(cmd));

    cmd.header.version  = 0;
    cmd.header.command  = EC_CMD_RGBKBD_SET_COLOR;
    cmd.header.outsize  = (uint32_t)(2 + count * 3);
    cmd.header.insize   = 0;

    cmd.data[0]         = start;
    cmd.data[1]         = count;

    for(unsigned int led_idx = 0; led_idx < count; led_idx++)
    {
        cmd.data[2 + led_idx * 3 + 0] = RGBGetRValue(colors[led_idx]);
        cmd.data[2 + led_idx * 3 + 1] = RGBGetGValue(colors[led_idx]);
        cmd.data[2 + led_idx * 3 + 2] = RGBGetBValue(colors[led_idx]);
    }

    if(ioctl(fd, CROS_EC_DEV_IOCXCMD_V2, &cmd) < 0)
    {
        LOG_ERROR("[Framework Desktop] EC_CMD_RGBKBD_SET_COLOR ioctl failed: %s", strerror(errno));
    }
    else if(cmd.header.result != 0)
    {
        LOG_ERROR("[Framework Desktop] EC_CMD_RGBKBD_SET_COLOR returned EC error %u", cmd.header.result);
    }
}
