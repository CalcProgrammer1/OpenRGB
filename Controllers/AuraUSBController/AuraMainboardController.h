/*-----------------------------------------*\
|  AuraMainboardController.h                |
|                                           |
|  Definitions and types for ASUS Aura      |
|  USB Mainboard RGB lighting controller    |
|                                           |
|  Martin Hartl (inlart) 4/25/2020          |
\*-----------------------------------------*/

#include "RGBController.h"
#include "AuraUSBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    AURA_MAINBOARD_CONTROL_MODE_EFFECT            = 0x35,     /* Effect control mode                  */
    AURA_MAINBOARD_CONTROL_MODE_DIRECT            = 0x36,     /* Direct control mode                  */
    AURA_MAINBOARD_CONTROL_MODE_COMMIT            = 0x3F,     /* Direct control mode                  */
};

class AuraMainboardController : public AuraUSBController
{
public:
    AuraMainboardController(hid_device* dev_handle);
    ~AuraMainboardController();

    void SetChannelLEDs
        (
        unsigned char   channel,
        RGBColor *      colors,
        unsigned int    num_colors
        );

    void SetMode
        (
        unsigned char   channel,
        unsigned char   mode,
        unsigned char   red,
        unsigned char   grn,
        unsigned char   blu
        );

private:
    unsigned int mode;

    void SendEffect
        (
        unsigned char   channel,
        unsigned char   mode
        );

    void SendDirect
        (
        unsigned char   device,
        unsigned char   start_led,
        unsigned char   led_count,
        unsigned char*  led_data
        );

    void SendCommit();
};
