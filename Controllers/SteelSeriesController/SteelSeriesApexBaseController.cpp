/*---------------------------------------------------------*\
| SteelSeriesApexBaseController.cpp                         |
|                                                           |
|   Driver base for SteelSeries Apex                        |
|                                                           |
|   Florian Heilmann (FHeilmann)                19 Oct 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "SteelSeriesApexBaseController.h"
#include <algorithm>

SteelSeriesApexBaseController::SteelSeriesApexBaseController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

SteelSeriesApexBaseController::~SteelSeriesApexBaseController()
{

}

std::string SteelSeriesApexBaseController::GetLocation()
{
    return("HID: " + location);
};

std::string SteelSeriesApexBaseController::GetName()
{
    return(name);
}

/*---------------------------------------------------------*\
| Gen 1 Apex Pro stores the unit serial number in firmware. |
| The first 5 digits determine the region of the keyboard.  |
| This is not the case for Gen 3, call to this function     |
| will be ignored.                                          |
\*---------------------------------------------------------*/
std::string SteelSeriesApexBaseController::GetSerial()
{
    std::string return_string = "";
    if(proto_type == APEX && kbd_quirk == APEX_GEN1)
    {
        unsigned char obuf[STEELSERIES_PACKET_OUT_SIZE];
        unsigned char ibuf[STEELSERIES_PACKET_IN_SIZE];
        int result;

        memset(obuf, 0x00, sizeof(obuf));
        obuf[0x00] = 0;
        obuf[0x01] = 0xFF;
        hid_write(dev, obuf, STEELSERIES_PACKET_OUT_SIZE);

        result = hid_read_timeout(dev, ibuf, STEELSERIES_PACKET_IN_SIZE, 2);

        /*-------------------------------------------------*\
        | Only the first 19 bytes are of value              |
        \*-------------------------------------------------*/
        if(result > 0)
        {
            std::string serialnum(ibuf, ibuf+19);
            return_string = serialnum;
        }
    }

        return(return_string);
}

std::string ExtractVersion(std::string version_string)
{
    /*---------------------------------------------*\
    | Find 2 periods in string, if found we can     |
    | form a X.Y.Z revision.                        |
    \*---------------------------------------------*/
    std::size_t majorp = version_string.find('.');
    if(majorp != std::string::npos)
    {
        std::size_t minorp = version_string.find('.', majorp+1);
        if(minorp != std::string::npos)
        {
            std::string major = version_string.substr(0, majorp);
            std::string minor = version_string.substr(majorp+1, (minorp-majorp-1));
            std::string build = version_string.substr(minorp+1);
            return major + "." + minor + "." + build;
        }
    }
    return "";
}

std::string SteelSeriesApexBaseController::GetVersion()
{
    std::string return_string = "Unsupported protocol";

    if(proto_type == APEX)
    {
        /*-------------------------------------------------*\
        | Gen 1 & 2 Apex Pro report KBD and LED firmware    |
        | Gen 3 only reports the KBD firmware, ignoring     |
        | requests to read the LED version                  |
        \*-------------------------------------------------*/
        unsigned char   obuf[STEELSERIES_PACKET_OUT_SIZE];
        unsigned char   ibuf[STEELSERIES_PACKET_IN_SIZE];
        int             result;

        memset(obuf, 0x00, sizeof(obuf));
        obuf[0x00] = 0;
        obuf[0x01] = 0x90;
        hid_write(dev, obuf, STEELSERIES_PACKET_OUT_SIZE);
        result = hid_read_timeout(dev, ibuf, STEELSERIES_PACKET_IN_SIZE, 2);

        if(result > 0)
        {
            std::string fwver(ibuf, ibuf+STEELSERIES_PACKET_IN_SIZE);
            fwver.erase(std::remove(fwver.begin(), fwver.end(), '\0'), fwver.end());

            /*---------------------------------------------*\
            | Apex Pro Gen 3 needs the first char dropped   |
            \*---------------------------------------------*/
            if(kbd_quirk == APEX_GEN3)
            {
                fwver.erase(0,1);
            }

            return_string = "KBD: " + ExtractVersion(fwver);
        }

        /*-------------------------------------------------*\
        | Clear and reuse buffer                            |
        \*-------------------------------------------------*/
        if(kbd_quirk != APEX_GEN3)
        {
            memset(ibuf, 0x00, sizeof(ibuf));
            obuf[0x02] = 0x01;
            hid_write(dev, obuf, STEELSERIES_PACKET_OUT_SIZE);
            result = hid_read_timeout(dev, ibuf, STEELSERIES_PACKET_IN_SIZE, 10);

            if(result > 0)
            {
                std::string fwver(ibuf, ibuf+STEELSERIES_PACKET_IN_SIZE);
                fwver.erase(std::remove(fwver.begin(), fwver.end(), '\0'), fwver.end());
                fwver = fwver.c_str();

                return_string = return_string + " / LED: " + ExtractVersion(fwver);
            }
        }
    }

    return(return_string);
}
