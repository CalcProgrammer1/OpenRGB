/*---------------------------------------------------------*\
| MSILaptopController.h                                     |
|                                                           |
|   Driver for MSI laptop SteelSeries KLC/ALC RGB devices   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <hidapi.h>
#include "RGBController.h"

typedef enum
{
    MSI_LAPTOP_KLC,
    MSI_LAPTOP_ALC,

} msi_laptop_device;

typedef struct
{
    const char*     name;
    unsigned char   id;

} msi_laptop_led;

struct MSILaptopModel
{
    const char*             sys_vendor;
    const char*             product_name;
    
    /* Keyboard layout */
    const msi_laptop_led*   klc_leds;
    unsigned int            klc_leds_count;
    unsigned int            klc_matrix_height;
    unsigned int            klc_matrix_width;
    const unsigned int*     klc_matrix_map;
    
    /* Lightbar layout */
    const msi_laptop_led*   alc_leds;
    unsigned int            alc_leds_count;
    unsigned int            alc_lightbar_leds;
};

class MSILaptopController
{
public:
    MSILaptopController(hid_device* dev_handle, const char* path, std::string dev_name, msi_laptop_device device_type);
    ~MSILaptopController();

    std::string                 GetDeviceLocation();
    std::string                 GetDeviceName();
    std::string                 GetSerialString();
    msi_laptop_device  GetDeviceType();

    void                        SetLEDs(std::vector<led> leds, std::vector<RGBColor> colors);

private:
    hid_device*                 dev;
    std::string                 location;
    std::string                 name;
    msi_laptop_device  type;
};
