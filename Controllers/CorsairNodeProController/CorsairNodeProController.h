/*---------------------------------------------------------*\
|  Definitions for Corsair Lighting Node Pro                |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 1/12/2020        |
\*---------------------------------------------------------*/

#include "RGBController.h"
#include <vector>
#include <libusb-1.0/libusb.h>

#pragma once

enum
{
    CORSAIR_NODE_PRO_CHANNEL_1      = 0x00,     /* Channel 1                    */
    CORSAIR_NODE_PRO_CHANNEL_2      = 0x01,     /* Channel 2                    */
    CORSAIR_NODE_PRO_NUM_CHANNELS   = 0x02      /* Number of channels           */
};

class CorsairNodeProController
{
public:
    CorsairNodeProController(libusb_device_handle* dev_handle);
    ~CorsairNodeProController();
    
    char*           GetLEDString();
    unsigned int    GetStripsOnChannel(unsigned int channel);
    void            SetChannelEffect(unsigned int channel, unsigned int mode, std::vector<RGBColor> colors);
    void            SetChannelLEDs(unsigned int channel, std::vector<RGBColor> colors);
    void            SendKeepalive();
    unsigned int    channel_leds[CORSAIR_NODE_PRO_NUM_CHANNELS];

    void            KeepaliveThread();

private:
    libusb_device_handle*   dev;
};
