/*-----------------------------------------*\
|  AsusAuraUSBController.cpp                |
|                                           |
|  Driver for ASUS Aura RGB USB             |
|  lighting controller                      |
|                                           |
|  Martin Hartl (inlart) 4/25/2020          |
\*-----------------------------------------*/

#include "AsusAuraUSBController.h"
#include <cstring>
#include <stdexcept>

AuraUSBController::AuraUSBController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;

    GetFirmwareVersion();
    GetConfigTable();
}

AuraUSBController::~AuraUSBController()
{
    hid_close(dev);
}
unsigned int AuraUSBController::GetChannelCount()
{
    return(device_info.size());
}

std::string AuraUSBController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string AuraUSBController::GetDeviceName()
{
    return(device_name);
}

std::string AuraUSBController::GetSerialString()
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

const std::vector<AuraDeviceInfo>& AuraUSBController::GetAuraDevices() const
{
    return(device_info);
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
    else
    {       
        hid_close(dev);

        throw std::runtime_error("Could not read config table");
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

void AuraUSBController::SendDirect
    (
    unsigned char   device,
    unsigned char   start_led,
    unsigned char   led_count,
    unsigned char*  led_data,
    bool apply /* = false */
    )
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet                                 |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0xEC;
    usb_buf[0x01]   = AURA_CONTROL_MODE_DIRECT;
    usb_buf[0x02]   = apply ? 0x80 : 0x00;
    usb_buf[0x02]  |= device;
    usb_buf[0x03]   = start_led;
    usb_buf[0x04]   = led_count;

    /*-----------------------------------------------------*\
    | Copy in color data bytes                              |
    \*-----------------------------------------------------*/
    memcpy(&usb_buf[0x05], led_data, led_count * 3);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
}
