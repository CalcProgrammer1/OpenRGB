/*-----------------------------------------*\
|  AuraUSBController.cpp                    |
|                                           |
|  Driver for ASUS Aura RGB USB             |
|  lighting controller                      |
|                                           |
|  Martin Hartl (inlart) 4/25/2020          |
\*-----------------------------------------*/

#include "AuraUSBController.h"
#include <cstring>

AuraUSBController::AuraUSBController(hid_device* dev_handle)
{
    dev = dev_handle;

    GetFirmwareVersion();
    GetConfigTable();
}

AuraUSBController::~AuraUSBController()
{

}

unsigned int AuraUSBController::GetChannelCount()
{
    return( 5 );
}

std::string AuraUSBController::GetDeviceName()
{
    return(device_name);
}

void AuraUSBController::GetConfigTable()
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0xEC;
    usb_buf[0x01]   = AURA_REQUEST_CONFIG_TABLE;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 65);

    /*-----------------------------------------------------*\
    | Copy the firmware string if the reply ID is correct   |
    \*-----------------------------------------------------*/
    if(usb_buf[1] == 0x30)
    {
        memcpy(config_table, &usb_buf[4], 60);

        for(int i = 0; i < 60; i+=6)
        {
            printf("%02X %02X %02X %02X %02X %02X\r\n", config_table[i + 0],
                                                        config_table[i + 1],
                                                        config_table[i + 2],
                                                        config_table[i + 3],
                                                        config_table[i + 4],
                                                        config_table[i + 5]);
        }
    }
}

void AuraUSBController::GetFirmwareVersion()
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up firmware version request packet                |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0xEC;
    usb_buf[0x01]   = AURA_REQUEST_FIRMWARE_VERSION;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 65);

    /*-----------------------------------------------------*\
    | Copy the firmware string if the reply ID is correct   |
    \*-----------------------------------------------------*/
    if(usb_buf[1] == 0x02)
    {
        memcpy(device_name, &usb_buf[2], 16);
    }
}
