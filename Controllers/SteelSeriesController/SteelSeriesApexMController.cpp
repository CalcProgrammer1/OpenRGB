/*-----------------------------------------*\
|  SteelSeriesApexMController.cpp           |
|                                           |
|  Definitions and types for SteelSeries    |
|  Apex M750 Keyboard lighting controller   |
|                                           |
|  Florian Heilmann (FHeilmann) 12/10/2020  |
\*-----------------------------------------*/

#include "SteelSeriesApexMController.h"

#include <cstring>

#define NA 0xFF
static unsigned int keys_m[] = { 96, 99, 98, NA, 40, NA, NA, NA, NA, 102, 103, 104, 100, NA, NA , 75, 76, 74, NA, 93, NA, 94,
                                 97, 25, 23, 2 , 21, 1 , 13, 12, 49, 50 , 51 , NA , 101, NA, NA , NA, 77, NA, 84, 85, 86, 83,
                                 52, 0 , 18, 3 , 5 , 6 , 7 , 9 , 10, 11 , 46 , 47 , NA , 36, NA , NA, NA, NA, 87, 88, 89, NA,
                                 39, 16, 22, 4 , 17, 19, 24, 20, 8 , 14 , 15 , 43 , 44 , NA, 105, 71, 72, 73, 90, 91, 92, 82,
                                 48, 26, 27, 28, 29, 30, 31, 32, 33, 34 , 35 , 41 , 42 , NA, 38 , 68, 69, 70, 78, 79, 80, 81,
                                 37, 53, 54, 55, 56, NA, 57, 58, 59, 60 , NA , 61 , 62 , 63, 64 , 65, 66, 67, NA, NA, NA, NA };

SteelSeriesApexMController::SteelSeriesApexMController(hid_device* dev_handle, steelseries_type type, const char* path)
{
    dev         = dev_handle;
    location    = path;
    proto_type  = type;
    EnableLEDControl();
}

SteelSeriesApexMController::~SteelSeriesApexMController()
{
    hid_close(dev);
}

void SteelSeriesApexMController::EnableLEDControl()
{
    unsigned char buf[513] = { 0x00 };
    buf[0x00] = 0x00;
    buf[0x01] = 0x00;
    buf[0x02] = 0x00;
    buf[0x03] = 0x00;
    buf[0x04] = 0x01;
    buf[0x05] = 0x00;
    buf[0x06] = 0x85;
    hid_send_feature_report(dev, buf, 513);
    buf[0x00] = 0x00;
    buf[0x01] = 0x00;
    buf[0x02] = 0x00;
    buf[0x03] = 0x00;
    buf[0x04] = 0x03;
    buf[0x05] = 0x01;
    buf[0x06] = 0x00;
    buf[0x07] = 0xff;
    hid_send_feature_report(dev, buf, 513);
    buf[0x00] = 0x00;
    buf[0x01] = 0x00;
    buf[0x02] = 0x00;
    buf[0x03] = 0x00;
    buf[0x04] = 0x01;
    buf[0x05] = 0x00;
    buf[0x06] = 0x85;
    hid_send_feature_report(dev, buf, 513);
}

void SteelSeriesApexMController::SetMode(unsigned char /*mode*/, std::vector<RGBColor> /*colors*/)
{
}

void SteelSeriesApexMController::SetLEDsDirect(std::vector<RGBColor> colors)
{
    unsigned char buf[513] = { 0x00 };
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
            buf[i * 3 + 9] = 0xFF;
            buf[i * 3 + 10] = 0x32;
            buf[i * 3 + 11] = 0x00;
        }
        else
        {
            buf[(i * 3) + 9] = RGBGetRValue(colors[keys_m[i]]);
            buf[(i * 3) + 10] = RGBGetGValue(colors[keys_m[i]]);
            buf[(i * 3) + 11] = RGBGetBValue(colors[keys_m[i]]);
        }
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 513);

}
