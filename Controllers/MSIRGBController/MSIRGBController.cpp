/*-----------------------------------------*\
|  MSIRGBController.cpp                     |
|                                           |
|  Driver for MSI-RGB lighting controller   |
|                                           |
|  Logic adapted from:                      |
|     https://github.com/nagisa/msi-rgb     |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/11/2020   |
\*-----------------------------------------*/

#include "MSIRGBController.h"
#include "super_io.h"

MSIRGBController::MSIRGBController(int sioaddr)
{
    msi_sioaddr = sioaddr;

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
        superio_outb(msi_sioaddr, MSI_SIO_RGB_REG_ENABLE, MSI_SIO_RGB_ENABLE_MASK & (enable & !MSI_SIO_RGB_ENABLE_MASK));
    }

    /*-----------------------------------------------------*\
    | Lighting enabled, no pulsing or blinking              |
    \*-----------------------------------------------------*/
    superio_outb(msi_sioaddr, MSI_SIO_RGB_REG_CFG_1, 0x00);

    /*-----------------------------------------------------*\
    | Lighting enabled, RGB non-inverted, header on         |
    \*-----------------------------------------------------*/
    superio_outb(msi_sioaddr, MSI_SIO_RGB_REG_CFG_3, 0xE2);
}

MSIRGBController::~MSIRGBController()
{

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

    superio_outb(msi_sioaddr, MSI_SIO_RGB_REG_BLUE_1_0, (blue & (blue << 4)));
    superio_outb(msi_sioaddr, MSI_SIO_RGB_REG_BLUE_3_2, (blue & (blue << 4)));
    superio_outb(msi_sioaddr, MSI_SIO_RGB_REG_BLUE_5_4, (blue & (blue << 4)));
    superio_outb(msi_sioaddr, MSI_SIO_RGB_REG_BLUE_7_6, (blue & (blue << 4)));
}