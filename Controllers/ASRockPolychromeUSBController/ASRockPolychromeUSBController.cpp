/*-----------------------------------------*\
|  ASRockPolychromeUSBController.cpp        |
|                                           |
|  Driver for ASRock Polychrome USB         |
|  lighting controller                      |
|                                           |
|  Ed Kambulow (dredvard) 12/20/2020        |
\*-----------------------------------------*/

#include "RGBController.h"
#include "ASRockPolychromeUSBController.h"
#include <cstring>
#include <stdio.h>
#include "dependencies/dmiinfo.h"

#define POLYCHROME_USB_READ_ZONE_CONFIG 0x11
#define POLYCHROME_USB_READ_HEADER      0x14
#define POLYCHROME_USB_WRITE_HEADER     0x15
#define POLYCHROME_USB_SET              0x10
#define POLYCHROME_USB_INIT             0xA4
#define POLYCHROME_USB_EXIT             0x12

#define POLYCHROME_USB_ADDRESS_CFG      02
#define POLYCHROME_USB_RGSWAP_CFG       03

PolychromeUSBController::PolychromeUSBController(hid_device* dev_handle, const char* path)
{
    DMIInfo       dmi;
    unsigned char zone;

    dev         = dev_handle;
    device_name = "ASRock " + dmi.getMainboard();
    location    = path;
    zone        = 1;

    SetDeviceInfo();
}

PolychromeUSBController::~PolychromeUSBController()
{

}

unsigned int PolychromeUSBController::GetChannelCount()
{
    return(device_info.size());
}

std::string PolychromeUSBController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string PolychromeUSBController::GetDeviceName()
{
    return(device_name);
}

std::string PolychromeUSBController::GetSerialString()
{
    wchar_t serial_string[128];
    hid_get_serial_number_string(dev, serial_string, 128);

    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());

    return(return_string);
}

void PolychromeUSBController::SetDeviceInfo()
{
    unsigned int max_chns = 6;

    PolychromeDeviceInfo newdev_info;

    for (unsigned int channel_idx = 0; channel_idx < max_chns; channel_idx++)
    {
        switch(channel_idx)
        {
            // Type: Addressable
            case POLYCHROME_USB_ZONE_ARGB1:
            case POLYCHROME_USB_ZONE_ARGB2:
                newdev_info.num_leds = 1;
                //newdev_info.effect_channel=channel_idx;
                newdev_info.device_type=PolychromeDeviceType::ADDRESSABLE;

                break;

            // Might be Addressable
            case POLYCHROME_USB_ZONE_PCH :
            case POLYCHROME_USB_ZONE_IOCOVER:
            case POLYCHROME_USB_ZONE_PCB :
            case POLYCHROME_USB_ZONE_AUDIO:
                newdev_info.num_leds = 1;
                newdev_info.device_type=PolychromeDeviceType::FIXED;
                break;

            // TYpe: Fixed
            case POLYCHROME_USB_ZONE_RGB1:
            case POLYCHROME_USB_ZONE_RGB2:
            default:
                newdev_info.num_leds = 1;
                newdev_info.device_type=PolychromeDeviceType::FIXED;
                break;
        }

        device_info.push_back(newdev_info);
    }
}

void PolychromeUSBController::WriteZone
    (
        unsigned char   zone,
        unsigned char   mode,
        unsigned char   speed,
        RGBColor        rgb, 
        bool            allzone = false
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
	usb_buf[0x00] = POLYCHROME_USB_SET;
	usb_buf[0x02] = zone;
	usb_buf[0x03] = mode;
	usb_buf[0x04] = RGBGetRValue(rgb);
	usb_buf[0x05] = RGBGetGValue(rgb);
	usb_buf[0x06] = RGBGetBValue(rgb);
	usb_buf[0x07] = speed;
	usb_buf[0x08] = 0xFF;
	usb_buf[0x09] = allzone;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 65);
};

void PolychromeUSBController::WriteRGSwap
    (
        bool ahdr1,
        bool ahdr0,
        bool hdr1,
        bool hdr0
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
	usb_buf[0x00] = POLYCHROME_USB_WRITE_HEADER;

    usb_buf[0x03] = POLYCHROME_USB_RGSWAP_CFG;
	usb_buf[0x04] = ((ahdr1 << 3) & (ahdr0 << 2) & (hdr1 << 1) & hdr0) & 0x40;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 65);
}

void PolychromeUSBController::WriteAddressableHeader
    (
        unsigned char   zone,
        unsigned char   ahdr0,
        unsigned char   hdr1
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
	usb_buf[0x00] = 0x15;
	usb_buf[0x02] = zone;
	usb_buf[0x03] = 0x01;  
    usb_buf[0x03] = 0x01;

	usb_buf[0x04] = ahdr0;
	usb_buf[0x05] = hdr1;
	usb_buf[0x06] = 0x05;
	usb_buf[0x07] = 0x07;
	usb_buf[0x08] = 0x1e;
    usb_buf[0x09] = 0x1e;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 65);
}

PolychromeZoneInfo PolychromeUSBController::GetZoneConfig(unsigned char zone)
{
    unsigned char       usb_buf[65];
    PolychromeZoneInfo  zoneinfo;
    unsigned char       all;
    unsigned char       mode;
    unsigned char       spd;
    unsigned int        rgb;
    unsigned char       r;
    unsigned char       g;
    unsigned char       b;

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = POLYCHROME_USB_READ_ZONE_CONFIG;
    usb_buf[0x02]   = zone; 

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);

    /*-----------------------------------------------------*\
    | Read response                                         |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));
    hid_read(dev, usb_buf, 65);
    
    r   = usb_buf[0x05];
    g   = usb_buf[0x06];
    b   = usb_buf[0x07];

    zoneinfo.mode   = usb_buf[0x04];
    zoneinfo.color  = ToRGBColor(r,g,b);  
    zoneinfo.speed  = usb_buf[0x08];
    zoneinfo.zone   = usb_buf[0x03];
    all             = usb_buf[0x10];

    return(zoneinfo);
}

const std::vector<PolychromeDeviceInfo>& PolychromeUSBController::GetPolychromeDevices() const
{
    return(device_info);
};