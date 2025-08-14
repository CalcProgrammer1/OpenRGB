/*---------------------------------------------------------*\
| MSIRGBController.cpp                                      |
|                                                           |
|   Driver for MSI-RGB motherboard                          |
|                                                           |
|   Logic adapted from https://github.com/nagisa/msi-rgb    |
|                                                           |
|   Adam Honse (CalcProgrammer1)                11 Feb 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "MSIRGBController.h"
#include "dmiinfo.h"
#include "super_io.h"

MSIRGBController::MSIRGBController(int sioaddr, bool invert, std::string dev_name)
{
    msi_sioaddr = sioaddr;
    name        = dev_name;

    /*-----------------------------------------------------*\
    | This setup step isn't well documented                 |
    | Without this, pulsing does not work                   |
    \*-----------------------------------------------------*/
    superio_outb(msi_sioaddr, SIO_REG_LOGDEV, 0x09);

    int val_at_2c = superio_inb(msi_sioaddr, 0x2C);

    val_at_2c &= 0b11110111;
    val_at_2c |= 0b00010000;

    superio_outb(msi_sioaddr, 0x2C, val_at_2c);

    /*-----------------------------------------------------*\
    | Set logical device register to RGB controller         |
    \*-----------------------------------------------------*/
    superio_outb(msi_sioaddr, SIO_REG_LOGDEV, MSI_SIO_LOGDEV_RGB);

    /*-----------------------------------------------------*\
    | Test if RGB is enabled.  If it is not, enable it.     |
    \*-----------------------------------------------------*/
    int enable = superio_inb(msi_sioaddr, MSI_SIO_RGB_REG_ENABLE);

    if((enable & MSI_SIO_RGB_ENABLE_MASK) != MSI_SIO_RGB_ENABLE_MASK)
    {
        superio_outb(msi_sioaddr, MSI_SIO_RGB_REG_ENABLE, 0xE0);
    }

    /*-----------------------------------------------------*\
    | Lighting enabled, no pulsing or blinking              |
    \*-----------------------------------------------------*/
    superio_outb(msi_sioaddr, MSI_SIO_RGB_REG_CFG_1, 0x00);

    /*--------------------------------------------------------------*\
    | Header on, pulse deactivated, colors inverted depending on DMI |
    \*--------------------------------------------------------------*/
    unsigned char ff_val = 0b11100010;

    if (invert)
    {
        ff_val |= 0b00011100;
    }

    superio_outb(msi_sioaddr, MSI_SIO_RGB_REG_CFG_3, ff_val);
    /*-----------------------------------------------------------*\
    | This seems to be related to some rainbow mode. Deactivated  |
    \*-----------------------------------------------------------*/
    superio_outb(msi_sioaddr, 0xFD, 0x00);
}

MSIRGBController::~MSIRGBController()
{

}

std::string MSIRGBController::GetDeviceLocation()
{
    char hex[12];
    snprintf(hex, sizeof(hex), "0x%X", msi_sioaddr);
    return("SIO: " + std::string(hex));
}

std::string MSIRGBController::GetDeviceName()
{
    return(name);
}

void MSIRGBController::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    /*-----------------------------------------------------*\
    | The MSI RGB controller uses 4 bits per color rather   |
    | than 8.  Shift the values by 4 so that the 4 most     |
    | significant bits of each color are the new color value|
    \*-----------------------------------------------------*/
    red     = red >> 4;
    green   = green >> 4;
    blue    = blue >> 4;

    /*-----------------------------------------------------*\
    | Set logical device register to RGB controller         |
    \*-----------------------------------------------------*/
    superio_outb(msi_sioaddr, SIO_REG_LOGDEV, MSI_SIO_LOGDEV_RGB);

    /*-----------------------------------------------------*\
    | Write the colors to the color sequence registers      |
    | Only static mode is supported right now - all colors  |
    | are the same.                                         |
    \*-----------------------------------------------------*/
    superio_outb(msi_sioaddr, MSI_SIO_RGB_REG_RED_1_0, (red | (red << 4)));
    superio_outb(msi_sioaddr, MSI_SIO_RGB_REG_RED_3_2, (red | (red << 4)));
    superio_outb(msi_sioaddr, MSI_SIO_RGB_REG_RED_5_4, (red | (red << 4)));
    superio_outb(msi_sioaddr, MSI_SIO_RGB_REG_RED_7_6, (red | (red << 4)));

    superio_outb(msi_sioaddr, MSI_SIO_RGB_REG_GREEN_1_0, (green | (green << 4)));
    superio_outb(msi_sioaddr, MSI_SIO_RGB_REG_GREEN_3_2, (green | (green << 4)));
    superio_outb(msi_sioaddr, MSI_SIO_RGB_REG_GREEN_5_4, (green | (green << 4)));
    superio_outb(msi_sioaddr, MSI_SIO_RGB_REG_GREEN_7_6, (green | (green << 4)));

    superio_outb(msi_sioaddr, MSI_SIO_RGB_REG_BLUE_1_0, (blue | (blue << 4)));
    superio_outb(msi_sioaddr, MSI_SIO_RGB_REG_BLUE_3_2, (blue | (blue << 4)));
    superio_outb(msi_sioaddr, MSI_SIO_RGB_REG_BLUE_5_4, (blue | (blue << 4)));
    superio_outb(msi_sioaddr, MSI_SIO_RGB_REG_BLUE_7_6, (blue | (blue << 4)));
}
