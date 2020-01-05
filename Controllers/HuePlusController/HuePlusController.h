/*---------------------------------------------------------*\
|  Definitions for NZXT Hue+                                |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 12/11/2016       |
\*---------------------------------------------------------*/

#ifndef LED_STRIP_H
#define LED_STRIP_H

#include "RGBController.h"
#include "serial_port.h"
#include <vector>


#ifndef TRUE
#define TRUE true
#define FALSE false
#endif

#ifndef WIN32
#define LPSTR           char *
#define strtok_s        strtok_r
#endif

#define HUE_PLUS_BAUD           256000
#define HUE_PLUS_PACKET_SIZE    125

enum
{
    HUE_PLUS_CHANNEL_BOTH   = 0x00,     /* Both channels                */
    HUE_PLUS_CHANNEL_1      = 0x01,     /* Channel 1                    */
    HUE_PLUS_CHANNEL_2      = 0x02,     /* Channel 2                    */
    HUE_PLUS_NUM_CHANNELS   = 0x02      /* Number of channels           */
};

enum
{
    HUE_PLUS_CHANNEL_1_IDX  = 0x00,     /* Channel 1 array index        */
    HUE_PLUS_CHANNEL_2_IDX  = 0x01,     /* Channel 2 array index        */
};

enum
{
    HUE_PLUS_MODE_FIXED     = 0x00,     /* Fixed colors mode            */
    HUE_PLUS_MODE_FADING    = 0x01,     /* Fading mode                  */
    HUE_PLUS_MODE_SPECTRUM  = 0x02,     /* Spectrum cycle mode          */
    HUE_PLUS_NUM_MODES                  /* Number of Hue Plus modes     */
};

class HuePlusController
{
public:
    HuePlusController();
    ~HuePlusController();

    void            Initialize(char* port_name);
    char*           GetLEDString();
    unsigned int    GetStripsOnChannel(unsigned int channel);
    void            SetChannelEffect(unsigned int channel, unsigned int mode, std::vector<RGBColor> colors);
    void            SetChannelLEDs(unsigned int channel, std::vector<RGBColor> colors);

    unsigned int    channel_leds[HUE_PLUS_NUM_CHANNELS];

private:
    char led_string[1024];
    char port_name[128];
    serial_port *serialport;
};

#endif
