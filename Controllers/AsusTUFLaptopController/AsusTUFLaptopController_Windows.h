/*---------------------------------------------------------*\
| AsusTUFLaptopController_Windows.h                         |
|                                                           |
|   Driver for ASUS TUF laptop                              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <windows.h>

#define ASUS_WMI_DEVID_KBD_BACKLIGHT      0x00050021
#define ASUS_WMI_DEVID_TUF_RGB_MODE       0x00100056
#define ASUS_WMI_DEVID_TUF_RGB_STATE      0x00100057

#define ASUS_WMI_DEVID_FAN_BOOST_MODE     0x00110018
#define ASUS_WMI_DEVID_THROTTLE_THERMAL_POLICY 0x00120075

#define ASUS_WMI_KEYBOARD_SPEED_SLOW      0xE1
#define ASUS_WMI_KEYBOARD_SPEED_NORMAL    0xEB
#define ASUS_WMI_KEYBOARD_SPEED_FAST      0xF5

#define ASUS_WMI_KEYBOARD_SPEED_MIN       1
#define ASUS_WMI_KEYBOARD_SPEED_MAX       3

#define ASUS_WMI_KEYBOARD_MODE_STATIC     0x00
#define ASUS_WMI_KEYBOARD_MODE_BREATHING  0x01
#define ASUS_WMI_KEYBOARD_MODE_COLORCYCLE 0x02
#define ASUS_WMI_KEYBOARD_MODE_STROBING   0x0A

#define ASUS_WMI_KEYBOARD_BRIGHTNESS_MIN  0
#define ASUS_WMI_KEYBOARD_BRIGHTNESS_MAX  3

#define ASUS_WMI_KEYBOARD_MAGIC_USE       0xB3
#define ASUS_WMI_KEYBOARD_MAGIC_SAVE      0xB4

#define ASUS_WMI_KEYBOARD_POWER_BOOT      0x03<<16
#define ASUS_WMI_KEYBOARD_POWER_AWAKE     0x0C<<16
#define ASUS_WMI_KEYBOARD_POWER_SLEEP     0x30<<16
#define ASUS_WMI_KEYBOARD_POWER_SHUTDOWN  0xC0<<16

#define ASUS_WMI_KEYBOARD_POWER_SAVE      0x01<<8

#define ASUS_WMI_FAN_SPEED_NORMAL         0
#define ASUS_WMI_FAN_SPEED_TURBO          1
#define ASUS_WMI_FAN_SPEED_SILENT         2

class AsusTUFLaptopController
{
private:
    HANDLE hDevice;
    static int checkWMIType();
    AsusTUFLaptopController();

    bool deviceIoControlWrapper(const void *dataIn, int commandIndex, int dataSizeIn, void *dataOut, int *dataSizeOut);
    bool deviceControl(int a1, int a2);
    bool deviceControl(int a1, int a2, int a3);
    bool getStatus(int a1, int *out);
    bool getStatusExtended(int a1, int a2, int *status1, int *status2, int* status3);

public:
    static AsusTUFLaptopController * checkAndCreate();
    ~AsusTUFLaptopController();

    void            setMode(unsigned char red,
                            unsigned char green,
                            unsigned char blue,
                            unsigned char mode,
                            unsigned char speed,
                            bool save);

    unsigned char   getBrightness();
    void            setBrightness(unsigned char brightness);

    void            setPowerState(bool boot,
                                  bool awake,
                                  bool sleep,
                                  bool shutdown,
                                  bool save);

    void            setFanMode(int mode);
};
