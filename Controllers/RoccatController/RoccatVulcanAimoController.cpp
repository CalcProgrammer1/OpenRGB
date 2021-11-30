/*-------------------------------------------------------------------*\
|  RoccatVulcanAimoController.cpp                                     |
|                                                                     |
|  Driver for Roccat Vulcan Aimo Mouse                                |
|                                                                     |
|  Mola19 17/12/2021                                                  |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "RoccatVulcanAimoController.h"

#include <cstring>

RoccatVulcanAimoController::RoccatVulcanAimoController(hid_device* dev_handle, char *path)
{
    dev         = dev_handle;
    location    = path;
}

RoccatVulcanAimoController::~RoccatVulcanAimoController()
{
    hid_close(dev);
}

std::string RoccatVulcanAimoController::GetSerial()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());

    return(return_string);

}

std::string RoccatVulcanAimoController::GetLocation()
{
    return("HID: " + location);
}

void RoccatVulcanAimoController::SendColors(std::vector<RGBColor> colors)
{
    unsigned char bufs[7][65];

    bufs[0][1] = 0xA1;
    bufs[0][2] = 0x01;
    bufs[0][3] = 0x01;
    bufs[0][4] = 0xB4;

    for(int z = 0; z < 11; z++)
    {
        for(int l = 0; l < 12; l++)
        {
            int placeR = (z * 36) + 4 + l +  0;
            bufs[placeR / 64][placeR % 64 + 1] = RGBGetRValue(colors[z * 12 + l]);

            int placeG = (z * 36) + 4 + l + 12;
            bufs[placeG / 64][placeG % 64 + 1] = RGBGetGValue(colors[z * 12 + l]);

            int placeB = (z * 36) + 4 + l + 24;
            bufs[placeB / 64][placeB % 64 + 1] = RGBGetBValue(colors[z * 12 + l]);
        }
    }

    for(int p = 0; p < 7; p++)
    {
        hid_write(dev, bufs[p], 65);
    }
}
