/*---------------------------------------------------------*\
| SteelSeriesApexBaseController.cpp                         |
|                                                           |
|   Driver base for SteelSeries Apex                        |
|                                                           |
|   Florian Heilmann (FHeilmann)                19 Oct 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "SteelSeriesApexBaseController.h"

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
| The serial number of the keyboard is acquired by sending  |
| an output report to address 0xFF and reading the result.  |
| The HID capability table is not used.  The serial number  |
| also contains the model number which can be used to       |
| determine the physical layout of different region         |
| keyboards throughout the product stack.                   |
\*---------------------------------------------------------*/
std::string SteelSeriesApexBaseController::GetSerial()
{
    std::string return_string = "";
    if(proto_type == APEX)
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

std::string SteelSeriesApexBaseController::GetVersion()
{
    std::string return_string = "Unsupported protocol";

    if(proto_type == APEX)
    {
        /*-------------------------------------------------*\
        | For the Apex Pro there are two firmware versions  |
        | which can be acquired, KBD and LED.  We know      |
        | where both are located, we do not know which is   |
        | what.  For now we'll make an assumption and fix   |
        | if proven wrong.                                  |
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
            fwver = fwver.c_str();

            /*---------------------------------------------*\
            | Find 2 periods in string, if found we can     |
            | form a X.Y.Z revision.                        |
            \*---------------------------------------------*/
            std::size_t majorp = fwver.find('.');
            if(majorp != std::string::npos)
            {
                std::size_t minorp = fwver.find('.', majorp+1);
                if(minorp != std::string::npos)
                {
                    std::string major = fwver.substr(0, majorp);
                    std::string minor = fwver.substr(majorp+1, (minorp-majorp-1));
                    std::string build = fwver.substr(minorp+1);
                    return_string = "KBD: " + major + "." + minor + "." + build;
                }
            }
        }

        /*-------------------------------------------------*\
        | Clear and reuse buffer                            |
        \*-------------------------------------------------*/
        memset(ibuf, 0x00, sizeof(ibuf));
        obuf[0x02] = 0x01;
        hid_write(dev, obuf, STEELSERIES_PACKET_OUT_SIZE);
        result = hid_read_timeout(dev, ibuf, STEELSERIES_PACKET_IN_SIZE, 10);

        if(result > 0)
        {
            std::string fwver(ibuf, ibuf+STEELSERIES_PACKET_IN_SIZE);
            fwver = fwver.c_str();

            std::size_t majorp = fwver.find('.');
            if(majorp != std::string::npos)
            {
                std::size_t minorp = fwver.find('.', majorp+1);
                if(minorp != std::string::npos)
                {
                    std::string major = fwver.substr(0, majorp);
                    std::string minor = fwver.substr(majorp+1, (minorp-majorp-1));
                    std::string build = fwver.substr(minorp+1);
                    return_string = return_string + " / LED: " + major + "." + minor + "." + build;
                }
            }
        }
    }

    return(return_string);
}
