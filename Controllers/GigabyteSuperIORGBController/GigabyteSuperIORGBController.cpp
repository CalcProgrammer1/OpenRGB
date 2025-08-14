/*---------------------------------------------------------*\
| GigabyteSuperIORGBController.cpp                          |
|                                                           |
|   Driver for Gigabyte Aorus Super IO motherboard          |
|                                                           |
|   Ryan Frankcombe (422gRdHuX5uk)              11 Sep 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "GigabyteSuperIORGBController.h"
#include "super_io.h"

GigabyteSuperIORGBController::GigabyteSuperIORGBController(int sioaddr, std::string dev_name)
{
    gig_sioaddr = sioaddr;
    name        = dev_name;
}

GigabyteSuperIORGBController::~GigabyteSuperIORGBController()
{

}

std::string GigabyteSuperIORGBController::GetDeviceLocation()
{
    char hex[12];
    snprintf(hex, sizeof(hex), "0x%X", gig_sioaddr);
    return("SIO: " + std::string(hex));
}

std::string GigabyteSuperIORGBController::GetDeviceName()
{
    return(name);
}

void GigabyteSuperIORGBController::ChipEntry()
{
    /*--------------------------------*\
    | Chip Entry Command               |
    \*_-------------------------------*/
    superio_enter(gig_sioaddr);
    superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_CHIPENTRY_REGISTER_1, GIGABYTE_SUPERIO_CHIPENTRY_VALUE_1);
    superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_CHIPENTRY_REGISTER_2, GIGABYTE_SUPERIO_CHIPENTRY_VALUE_2);
    superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_CHIPENTRY_REGISTER_2, GIGABYTE_SUPERIO_CHIPENTRY_VALUE_2);

    /*--------------------------------*\
    | Chip Select Command              |
    \*_-------------------------------*/
    superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_CHIPSELECT_REGISTER_1, GIGABYTE_SUPERIO_CHIPSELECT_VALUE_1);
}

void GigabyteSuperIORGBController::ChipExit()
{
    /*-----------------------------------------------------------------------------------*\
    | Chip Exit Command                                                                   |
    | Per https://pdf1.alldatasheetde.com/datasheet-pdf/download/1132513/ITE/IT8712F.html |
    \*_----------------------------------------------------------------------------------*/
    superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_CHIPEXIT_REGISTER_1, GIGABYTE_SUPERIO_CHIPEXIT_VALUE_1);
}

void GigabyteSuperIORGBController::SetColor(unsigned int red, unsigned int green, unsigned int blue)
{
    /*--------------------------------*\
    | Chip Entry Command               |
    \*_-------------------------------*/
    ChipEntry();

    /*--------------------------------*\
    | Set Colors                       |
    \*_-------------------------------*/
    superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_RED_REGISTER_1, red);
    superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_GREEN_REGISTER_1, green);
    superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_BLUE_REGISTER_1, blue);

    superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_RED_REGISTER_2, red);
    superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_GREEN_REGISTER_2, green);
    superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_BLUE_REGISTER_2, blue);

    /*--------------------------------*\
    | Chip Exit Command                |
    \*_-------------------------------*/
    ChipExit();
}

void GigabyteSuperIORGBController::SetMode(int new_mode)
{
    if(new_mode>=GIGABYTE_MODE1_STATIC && new_mode<=GIGABYTE_MODE1_FLASHING)
    {
        ChipEntry();
    }

    /*-----------------------------------------------------*\
    | Write the colors to the color sequence registers      |
    \*-----------------------------------------------------*/
    switch (new_mode)
    {
       case GIGABYTE_MODE1_STATIC:
            superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_STATIC_REGISTER_1, GIGABYTE_SUPERIO_STATIC_VALUE_1);
            superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_STATIC_REGISTER_2, GIGABYTE_SUPERIO_STATIC_VALUE_2);
            superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_STATIC_REGISTER_3, GIGABYTE_SUPERIO_STATIC_VALUE_3);
            superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_STATIC_REGISTER_4, GIGABYTE_SUPERIO_STATIC_VALUE_4);
            superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_STATIC_REGISTER_5, GIGABYTE_SUPERIO_STATIC_VALUE_5);
            superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_STATIC_REGISTER_6, GIGABYTE_SUPERIO_STATIC_VALUE_6);
            break;

       case GIGABYTE_MODE1_RAINBOW:
            superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_RAINBOW_REGISTER_1, GIGABYTE_SUPERIO_RAINBOW_VALUE_1);
            superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_RAINBOW_REGISTER_2, GIGABYTE_SUPERIO_RAINBOW_VALUE_2);
            superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_RAINBOW_REGISTER_3, GIGABYTE_SUPERIO_RAINBOW_VALUE_3);
            superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_RAINBOW_REGISTER_4, GIGABYTE_SUPERIO_RAINBOW_VALUE_4);
            superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_RAINBOW_REGISTER_5, GIGABYTE_SUPERIO_RAINBOW_VALUE_5);
            superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_RAINBOW_REGISTER_6, GIGABYTE_SUPERIO_RAINBOW_VALUE_6);
            superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_RAINBOW_REGISTER_7, GIGABYTE_SUPERIO_RAINBOW_VALUE_7);
            superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_RAINBOW_REGISTER_8, GIGABYTE_SUPERIO_RAINBOW_VALUE_8);
            break;

       case GIGABYTE_MODE1_BREATHING:
            superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_BREATHING_REGISTER_1, GIGABYTE_SUPERIO_BREATHING_VALUE_1);
            superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_BREATHING_REGISTER_2, GIGABYTE_SUPERIO_BREATHING_VALUE_2);
            superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_BREATHING_REGISTER_3, GIGABYTE_SUPERIO_BREATHING_VALUE_3);
            superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_BREATHING_REGISTER_4, GIGABYTE_SUPERIO_BREATHING_VALUE_4);
            superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_BREATHING_REGISTER_5, GIGABYTE_SUPERIO_BREATHING_VALUE_5);
            superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_BREATHING_REGISTER_6, GIGABYTE_SUPERIO_BREATHING_VALUE_6);
            break;

       case GIGABYTE_MODE1_FLASHING:
            superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_FLASHING_REGISTER_1, GIGABYTE_SUPERIO_FLASHING_VALUE_1);
            superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_FLASHING_REGISTER_2, GIGABYTE_SUPERIO_FLASHING_VALUE_2);
            superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_FLASHING_REGISTER_3, GIGABYTE_SUPERIO_FLASHING_VALUE_3);
            superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_FLASHING_REGISTER_4, GIGABYTE_SUPERIO_FLASHING_VALUE_4);
            superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_FLASHING_REGISTER_5, GIGABYTE_SUPERIO_FLASHING_VALUE_5);
            superio_outb(gig_sioaddr, GIGABYTE_SUPERIO_FLASHING_REGISTER_6, GIGABYTE_SUPERIO_FLASHING_VALUE_6);
            break;
    }

    if(new_mode>=GIGABYTE_MODE1_STATIC && new_mode<=GIGABYTE_MODE1_FLASHING)
    {
        ChipExit();
    }
}
