/*---------------------------------------------------------*\
| FrameworkDesktopController_Linux.h                        |
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

#pragma once

#include <string>
#include <vector>
#include "RGBControllerInterface.h"

#define FRAMEWORK_DESKTOP_CROS_EC_DEV   "/dev/cros_ec"

/*---------------------------------------------------------*\
| The EC accepts up to 128 LEDs (EC_RGBKBD_MAX_KEY_COUNT),  |
| sent in chunks of 64 to fit in one host command packet.   |
| See the EC firmware, Framework Desktop (dogwood) branch:  |
| https://github.com/FrameworkComputer/EmbeddedController   |
| zephyr/program/framework/src/ws2812_npcx_bitbang.c        |
\*---------------------------------------------------------*/
#define FRAMEWORK_DESKTOP_MAX_LEDS      128
#define FRAMEWORK_DESKTOP_LEDS_PER_CMD  64

class FrameworkDesktopController
{
public:
    FrameworkDesktopController(int fd, std::string dev_path);
    ~FrameworkDesktopController();

    std::string GetLocation();

    void        SetLEDs(const std::vector<RGBColor>& colors);

private:
    int         fd;
    std::string location;
    std::size_t last_led_count;

    void        SendColors(unsigned char start, const RGBColor* colors, unsigned char count);
};
