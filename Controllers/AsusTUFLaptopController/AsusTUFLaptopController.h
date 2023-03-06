#ifdef _WIN32

#ifndef ASUSTUFLAPTOPCONTROLLER_H
#define ASUSTUFLAPTOPCONTROLLER_H

#define ASUS_WMI_DEVID_KBD_BACKLIGHT      0x00050021
#define ASUS_WMI_DEVID_TUF_RGB_MODE       0x00100056
#define ASUS_WMI_DEVID_TUF_RGB_STATE      0x00100057

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

class AsusTUFLaptopController
{
public:
    AsusTUFLaptopController();
    ~AsusTUFLaptopController();

    void            SetMode(unsigned char red,
                            unsigned char green,
                            unsigned char blue,
                            unsigned char mode,
                            unsigned char speed,
                            bool save);

    unsigned char   GetBrightness();
    void            SetBrightness(unsigned char brightness);

    void            SetPowerState(bool boot,
                                  bool awake,
                                  bool sleep,
                                  bool shutdown,
                                  bool save);
};

#endif // ASUSTUFLAPTOPCONTROLLER_H

#endif // _WIN32
