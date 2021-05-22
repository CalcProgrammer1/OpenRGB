/*------------------------------------------*\
|  HyperXAlloyOriginsCoreController.cpp      |
|                                            |
|  Driver for HyperX Alloy Origins Core      |
|  RGB Keyboard lighting controller          |
|                                            |
|  Volodymyr Nazarchuk (Vavooon) 4/28/2021   |
\*------------------------------------------*/

#include "HyperXAlloyOriginsCoreController.h"

#include <cstring>

// Skip these indices in the color output
static unsigned int skip_idx[] = {6,  7, 14, 15, 22, 23, 30, 31, 38, 39, 44, 46, 47, 54, 55, 58, 60, 61, 62, 63, 70, 71, 78, 79, 86, 87, 94, 95, 101, 102, 103, 109, 110, 111, 118, 119};

HyperXAlloyOriginsCoreController::HyperXAlloyOriginsCoreController(hid_device* dev_handle, const char* path)
{
    dev      = dev_handle;
    location = path;
}

HyperXAlloyOriginsCoreController::~HyperXAlloyOriginsCoreController()
{
    hid_close(dev);
}

std::string HyperXAlloyOriginsCoreController::GetDeviceLocation()
{
    return("HID " + location);
}

void HyperXAlloyOriginsCoreController::SetLEDsDirect(std::vector<RGBColor> colors)
{
    for(unsigned int skip_cnt = 0; skip_cnt < (sizeof(skip_idx) / sizeof(skip_idx[0])); skip_cnt++)
    {
        colors.insert(colors.begin() + skip_idx[skip_cnt], 0x00000000);
    }

    unsigned char buf[380];
    memset(buf, 0x00, sizeof(buf));

    int offset = 0;
    int rowPos = 0;

    for(unsigned int color_idx = 0; color_idx < colors.size(); color_idx++)
    {
        if (color_idx > 0 && color_idx % 16 == 0)
        {
            offset += 48;
            rowPos = 0;
        }

        buf[rowPos + offset]      = RGBGetGValue(colors[color_idx]);
        buf[rowPos + offset + 16] = RGBGetRValue(colors[color_idx]);
        buf[rowPos + offset + 32] = RGBGetBValue(colors[color_idx]);

        rowPos++;
    }

    unsigned int sentBytes   = 0;
    unsigned int bytesToSend = sizeof(buf);
    unsigned int payloadSize = 60;
    unsigned int seq         = 0;

    while(sentBytes < bytesToSend)
    {
        if (bytesToSend - sentBytes < payloadSize)
        {
            payloadSize = bytesToSend - sentBytes;
        }

        unsigned char packet[65];
        memset(packet, 0x00, sizeof(packet));
        
        packet[0] = 0xA2;
        packet[1] = seq++;
        packet[3] = payloadSize;

        memcpy(&packet[4], &buf[sentBytes], payloadSize);

        hid_write(dev, packet, 65);

        sentBytes += payloadSize;
    }
}
