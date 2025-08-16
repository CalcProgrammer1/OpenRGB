/*---------------------------------------------------------*\
| SteelSeriesApexMController.cpp                            |
|                                                           |
|   Driver for SteelSeries Apex M750                        |
|                                                           |
|   Florian Heilmann (FHeilmann)                12 Oct 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "SteelSeriesApexMController.h"

#define SS_APEX_M_PACKET_SIZE 513
#define NA 0xFF

static unsigned int keys_m[] =
{
/*    LCTRL  LWIN  LALT   XXX SPACE   XXX   XXX   XXX   XXX  RALT  RWIN   FNC RCTRL   XXX   XXX  LEFT  DOWN RIGHT   XXX    #0   XXX    #.     */
        79,   82,   81,   NA,   40,   NA,   NA,   NA,   NA,   85,   86,   87,   83,   NA,   NA,   75,   76,   74,   NA,  104,   NA,  105,

/*    LSHFT     Z     X     C     V     B     N     M     ,     .     /   XXX RSHFT   XXX   XXX   XXX    UP   XXX    #1    #2    #3 #ENTR     */
        80,   25,    23,   2,   21,    1,   13,   12,   49,   50,   51,   NA,   84,   NA,   NA,   NA,   77,   NA,   95,   96,   97,   94,

/*    CAPLK     A     S     D     F     G     H     J     K     L     ;     '   XXX ENTER   XXX   XXX   XXX   XXX    #4    #5    #6   XXX     */
        52,    0,   18,    3,    5,    6,    7,    9,   10,   11,   46,   47,   NA,   36,   NA,   NA,   NA,   NA,   98,   99,  100,   NA,

/*      TAB     Q     W     E     R     T     Y     U     I     O     P     [     ]   XXX     \ DELTE   END  PGDN    #7    #8    #9    #+     */
        39,   16,   22,    4,   17,   19,   24,   20,    8,   14,   15,   43,   44,   NA,   88,   71,   72,   73,  101,  102,  103,   93,

/*        `     1     2     3     4     5     6     7     8     9     0     -     =   XXX BKSPC INSRT  HOME  PGUP NUMLK    #/    #*    #-     */
        48,   26,   27,   28,   29,   30,   31,   32,   33,   34,   35,   41,   42,   NA,   38,   68,   69,   70,   89,   90,   91,   92,

/*      ESC    F1    F2    F3    F4   XXX    F5    F6    F7    F8   XXX    F9   F10   F11   F12 PRTSC SCRLK PAUSE   XXX   XXX   XXX   XXX     */
        37,   53,   54,   55,   56,   NA,   57,   58,   59,   60,   NA,   61,   62,   63,   64,   65,   66,   67,   NA,   NA,   NA,   NA
};

SteelSeriesApexMController::SteelSeriesApexMController(hid_device* dev_handle, steelseries_type type, const char* path, std::string dev_name) : SteelSeriesApexBaseController(dev_handle, path, dev_name)
{
    proto_type  = type;
    EnableLEDControl();
}

SteelSeriesApexMController::~SteelSeriesApexMController()
{
    hid_close(dev);
}

void SteelSeriesApexMController::EnableLEDControl()
{
    unsigned char buf[SS_APEX_M_PACKET_SIZE] = { 0x00 };

    buf[0x00] = 0x00;
    buf[0x01] = 0x00;
    buf[0x02] = 0x00;
    buf[0x03] = 0x00;
    buf[0x04] = 0x01;
    buf[0x05] = 0x00;
    buf[0x06] = 0x85;
    hid_send_feature_report(dev, buf, SS_APEX_M_PACKET_SIZE);

    buf[0x00] = 0x00;
    buf[0x01] = 0x00;
    buf[0x02] = 0x00;
    buf[0x03] = 0x00;
    buf[0x04] = 0x03;
    buf[0x05] = 0x01;
    buf[0x06] = 0x00;
    buf[0x07] = 0xff;
    hid_send_feature_report(dev, buf, SS_APEX_M_PACKET_SIZE);

    buf[0x00] = 0x00;
    buf[0x01] = 0x00;
    buf[0x02] = 0x00;
    buf[0x03] = 0x00;
    buf[0x04] = 0x01;
    buf[0x05] = 0x00;
    buf[0x06] = 0x85;
    hid_send_feature_report(dev, buf, SS_APEX_M_PACKET_SIZE);
}

void SteelSeriesApexMController::SetMode(unsigned char /*mode*/, std::vector<RGBColor> /*colors*/)
{
}

void SteelSeriesApexMController::SetLEDsDirect(std::vector<RGBColor> colors)
{
    unsigned char buf[SS_APEX_M_PACKET_SIZE] = { 0x00 };
    int num_keys = sizeof(keys_m) / sizeof(*keys_m);

    /*-----------------------------------------------------*\
    | Set up Direct packet                                  |
    \*-----------------------------------------------------*/
    buf[0x00] = 0x00;
    buf[0x01] = 0x00;
    buf[0x02] = 0x00;
    buf[0x03] = 0x01;
    buf[0x04] = 0x8e;
    buf[0x05] = 0x01;
    buf[0x06] = 0x03;
    buf[0x07] = 0x06;
    buf[0x08] = 0x16;

    /*-----------------------------------------------------*\
    | Fill in color data                                    |
    \*-----------------------------------------------------*/
    for (int i = 0; i < num_keys; i++)
    {
        if (keys_m[i] == NA)
        {
            buf[i * 3 + 9]      = 0xFF;
            buf[i * 3 + 10]     = 0x32;
            buf[i * 3 + 11]     = 0x00;
        }
        else
        {
            buf[(i * 3) + 9]    = RGBGetRValue(colors[keys_m[i]]);
            buf[(i * 3) + 10]   = RGBGetGValue(colors[keys_m[i]]);
            buf[(i * 3) + 11]   = RGBGetBValue(colors[keys_m[i]]);
        }
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, SS_APEX_M_PACKET_SIZE);
}
