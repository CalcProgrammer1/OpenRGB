/*-----------------------------------------*\
|  JginYueInternalUSBController.h           |
|                                           |
|  Driver for JginYue internal USB          |
|  lighting controller                      |
|                                           |
|                                           |
|                                           |
\*-----------------------------------------*/

#include "RGBController.h"
#include <hidapi/hidapi.h>
#include <string>

#pragma once


enum
{
    JGINYUE_USB_MODE_OFF                       =0x10,
    JGINYUE_USB_MODE_STATIC                    =0x11,
    JGINYUE_USB_MODE_BREATHING                 =0x12,
    JGINYUE_USB_MODE_STROBE                    =0x13,
    JGINYUE_USB_MODE_CYCLING                   =0x14,
    JGINYUE_USB_MODE_RANDOM                    =0x15,
    JGINYUE_USB_MODE_MUSIC                     =0x16,       /*music mode,not support yet                                    */
    JGINYUE_USB_MODE_WAVE                      =0x17,
    JGINYUE_USB_MODE_SPRING                    =0x18,
    JGINYUE_USB_MODE_WATER                     =0x19,
    JGINYUE_USB_MODE_RAINBOW                   =0x1A,
    JGINYUE_USB_MODE_DIRECT                    =0x20,       /*Not the exact USB protrol  - but need a way to differentiate  */
};

enum
{
    JGINYUE_USB_SPEED_MAX                      =0xFF,
    JGINYUE_USB_SPEED_MIN                      =0x00,
    JGINYUE_USB_SPEED_DEFAULT                  =0x80
};



enum
{
    JGINYUE_USB_BRIGHTNESS_MAX                  =0xFF,
    JGINYUE_USB_BRIGHTNESS_MIN                  =0x00,
    JGINYUE_USB_BRIGHTNESS_DEFAULT              =0xFF
};






class JginYueInternalUSBController
{
public:
   unsigned char   zone_led_count[8];

    JginYueInternalUSBController(hid_device* dev_handle, const char* path);
    ~JginYueInternalUSBController();

    unsigned int                                GetChannelCount();
    std::string                                 GetDeviceLocation();
    std::string                                 GetDeviceName();
    std::string                                 GetSerialString();





private:


protected:

};
