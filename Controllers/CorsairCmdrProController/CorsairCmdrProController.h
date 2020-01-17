/*---------------------------------------------------------*\
|  Definitions for Corsair Commander Pro                    |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 1/16/2020        |
\*---------------------------------------------------------*/

#include "RGBController.h"
#include <vector>
#include <libusb-1.0/libusb.h>

#pragma once

enum
{
    CORSAIR_CMDR_PRO_CHANNEL_1      = 0x00,     /* Channel 1                    */
    CORSAIR_CMDR_PRO_CHANNEL_2      = 0x01,     /* Channel 2                    */
    CORSAIR_CMDR_PRO_NUM_CHANNELS   = 0x02      /* Number of channels           */
};

class CorsairCmdrProController
{
public:
    CorsairCmdrProController(libusb_device_handle* dev_handle);
    ~CorsairCmdrProController();
    
    char*           GetLEDString();
    unsigned int    GetStripsOnChannel(unsigned int channel);
    void            SetChannelEffect(unsigned int channel, unsigned int mode, std::vector<RGBColor> colors);
    void            SetChannelLEDs(unsigned int channel, std::vector<RGBColor> colors);
    void            SendKeepalive();
    unsigned int    channel_leds[CORSAIR_CMDR_PRO_NUM_CHANNELS];

    void            KeepaliveThread();

private:
    libusb_device_handle*   dev;
};
