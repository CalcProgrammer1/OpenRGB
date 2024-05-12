/*---------------------------------------------------------*\
| AsusTUFLaptopController_Linux.cpp                         |
|                                                           |
|   Driver for ASUS TUF laptop                              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string>
#include "AsusTUFLaptopController_Linux.h"

void AsusTUFLaptopLinuxController::SendUpdate
    (
    unsigned char   mode,
    unsigned char   speed,
    unsigned char   save,
    unsigned char   red,
    unsigned char   green,
    unsigned char   blue
    )
{
    std::string s = "";
    s.append(ASUS_KBD_BACKLIGHT_BASE_PATH);
    s.append(ASUS_KBD_BACKLIGHT_MODE_PATH);
    FILE *controller = fopen(s.c_str(), "w");

    s = "";
    s.append(std::to_string(save));
    s.append(" ");
    s.append(std::to_string(mode));
    s.append(" ");
    s.append(std::to_string(red));
    s.append(" ");
    s.append(std::to_string(green));
    s.append(" ");
    s.append(std::to_string(blue));
    s.append(" ");
    s.append(std::to_string(speed));

    fputs(s.c_str(), controller);

    fclose(controller);
}

void AsusTUFLaptopLinuxController::SendBrightness
    (
    unsigned char   brightness
    )
{
    std::string s = "";
    s.append(ASUS_KBD_BACKLIGHT_BASE_PATH);
    s.append(ASUS_KBD_BACKLIGHT_BRIGHTNESS_PATH);
    FILE *controller = fopen(s.c_str(), "w");

    fputs(std::to_string(brightness).c_str(), controller);

    fclose(controller);
}
