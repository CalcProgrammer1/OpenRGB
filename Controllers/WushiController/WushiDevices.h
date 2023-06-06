 #include <string>
 #include "RGBController.h"

#define NA 0xFFFFFFFF
#ifndef wushiDEVICES_H
#define wushiDEVICES_H
/*-----------------------------------------------------*\
| WUSHI product IDs                                  |
\*-----------------------------------------------------*/
#define WF_S60                             0x5678

struct Wushi_led
{
    uint8_t led_num;
    std::string name;
};

struct Wushi_zone
{
    std::string name;
    zone_type type;
    unsigned char id;
    unsigned int height;
    unsigned int width;
    const unsigned int* matrix_map;
    const Wushi_led* leds;
    unsigned int start; //index to start reading the list of leds
    unsigned int end; //end index
};

/*--------------------------------------------------------*\
| Additional LEDs for wf S60                          |
\*--------------------------------------------------------*/
const Wushi_led wf_wf_S60_additional_leds[]
{
    {0x86, "WUSHI_L50"}
};

static const Wushi_zone WUSHI_L50 =
{
        "LED Strip",
        ZONE_TYPE_LINEAR,
        1,
        1,
        1,
        NULL,
        wf_wf_S60_additional_leds,
        3,
        3,
};

#endif
