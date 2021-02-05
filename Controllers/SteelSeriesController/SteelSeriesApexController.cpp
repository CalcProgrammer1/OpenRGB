/*-----------------------------------------*\
|  SteelSeriesApexController.cpp            |
|                                           |
|  Driver for SteelSeries Apex 7 RGB        |
|  Keyboard lighting controller             |
|                                           |
|  Eric Samuelson (edbgon) 7/5/2020         |
\*-----------------------------------------*/

#include "SteelSeriesApexController.h"

#include <cstring>

using namespace std::chrono_literals;

static unsigned int keys[] = {0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                              0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d,
                              0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a,
                              0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
                              0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45,
                              0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52,
                              0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 
                              0x60, 0x61, 0x62, 0x63, 0x64, 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
                              0xf0, 0x31 };

static unsigned int keys_tkl[] = {0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                  0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d,
                                  0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a,
                                  0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
                                  0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45,
                                  0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52,
                                  0x64, 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xf0, 0x31 };

SteelSeriesApexController::SteelSeriesApexController(hid_device* dev_handle, steelseries_type type, const char* path)
{
    dev         = dev_handle;
    location    = path;
    proto_type  = type;
}

SteelSeriesApexController::~SteelSeriesApexController()
{
    hid_close(dev);
}

void SteelSeriesApexController::SetMode
    (
    unsigned char mode,
    std::vector<RGBColor> /*colors*/
    )
{
    unsigned char mode_colors[9];

    active_mode = mode;

    memset(mode_colors, 0x00, sizeof(mode_colors));

}

void SteelSeriesApexController::SetLEDsDirect(std::vector<RGBColor> colors)
{
    unsigned char buf[643];
    int num_keys = 0;

    if(proto_type == APEX)
    {
        num_keys = sizeof(keys) / sizeof(*keys);
    }
    else
    {
        num_keys = sizeof(keys_tkl) / sizeof(*keys_tkl);
    }

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Direct packet                                  |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0;
    buf[0x01]   = APEX_PACKET_ID_DIRECT;
    buf[0x02]   = num_keys;

    /*-----------------------------------------------------*\
    | Fill in color data                                    |
    \*-----------------------------------------------------*/
    for(int i = 0; i < num_keys; i++)
    {
        if(proto_type == APEX)
        {
            buf[(i*4)+3] = keys[i];
        }
        else
        {
            buf[(i*4)+3] = keys_tkl[i];   
        }
        buf[(i*4)+4] = RGBGetRValue(colors[i]);
        buf[(i*4)+5] = RGBGetGValue(colors[i]);
        buf[(i*4)+6] = RGBGetBValue(colors[i]);
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 643);
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void SteelSeriesApexController::SelectProfile
    (
    unsigned char   profile
    )
{
    unsigned char buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer, set up packet and send               |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));
    buf[0x00]   = 0;
    buf[0x01]   = 0x89;
    buf[0x02]   = profile;
    hid_send_feature_report(dev, buf, 65);

}
