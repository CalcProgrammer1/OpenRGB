/*---------------------------------------------------------*\
|  Definitions for NZXT Hue 2                               |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 12/29/2016       |
\*---------------------------------------------------------*/

#include "RGBController.h"
#include <vector>
#include <libusb-1.0/libusb.h>

#pragma once

enum
{
    HUE_2_CHANNEL_ALL       = 0x00,     /* All channels                 */
    HUE_2_CHANNEL_1         = 0x01,     /* Channel 1                    */
    HUE_2_CHANNEL_2         = 0x02,     /* Channel 2                    */
    HUE_2_CHANNEL_3         = 0x03,     /* Channel 3                    */
    HUE_2_CHANNEL_4         = 0x04,     /* Channel 4                    */
    HUE_2_NUM_CHANNELS      = 0x04      /* Number of channels           */
};

enum
{
    HUE_2_CHANNEL_1_IDX     = 0x00,     /* Channel 1 array index        */
    HUE_2_CHANNEL_2_IDX     = 0x01,     /* Channel 2 array index        */
    HUE_2_CHANNEL_3_IDX     = 0x01,     /* Channel 3 array index        */
    HUE_2_CHANNEL_4_IDX     = 0x01,     /* Channel 4 array index        */
};

enum
{
    HUE_2_MODE_FIXED            = 0x00,     /* Fixed colors mode            */
    HUE_2_MODE_FADING           = 0x01,     /* Fading mode                  */
    HUE_2_MODE_SPECTRUM         = 0x02,     /* Spectrum cycle mode          */
    HUE_2_MODE_MARQUEE          = 0x03,     /* Marquee mode                 */
    HUE_2_MODE_COVER_MARQUEE    = 0x04,     /* Cover marquee mode           */
    HUE_2_MODE_ALTERNATING      = 0x05,     /* Alternating mode             */
    HUE_2_MODE_PULSING          = 0x06,     /* Pulsing mode                 */
    HUE_2_MODE_BREATHING        = 0x07,     /* Breathing mode               */
    HUE_2_NUM_MODES                         /* Number of Hue 2 modes        */
};

class Hue2Controller
{
public:
    Hue2Controller(libusb_device_handle* dev_handle);
    ~Hue2Controller();
    
    char*           GetLEDString();
    unsigned int    GetStripsOnChannel(unsigned int channel);
    void            SetChannelLEDs(unsigned int channel, std::vector<RGBColor> colors);

    unsigned int    channel_leds[HUE_2_NUM_CHANNELS];

private:
    libusb_device_handle*   dev;
};
