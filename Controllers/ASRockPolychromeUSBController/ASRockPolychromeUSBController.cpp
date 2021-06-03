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
#define POLYCHROME_USB_SET_ZONE         0x10
#define POLYCHROME_USB_INIT             0xA4
#define POLYCHROME_USB_COMMIT           0x12

PolychromeUSBController::PolychromeUSBController(hid_device* dev_handle, const char* path)
{
    DMIInfo       dmi;

    dev         = dev_handle;
    device_name = "ASRock " + dmi.getMainboard();
    location    = path;

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
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }
    
    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());

    return(return_string);
}

void PolychromeUSBController::SetDeviceInfo()
{
    PolychromeDeviceInfo newdev_info;
    ReadConfigTables();

    for (unsigned int zonecnt = 0; zonecnt < POLYCHROME_USB_ZONE_MAX_NUM; zonecnt++)
    {
        if(configtable[zonecnt] == 0x1E)
        {
            break;
        }

        newdev_info.num_leds = configtable[zonecnt];

        switch (zonecnt)
        {
            /*-----------------------------------------*\
            | Type: Addressable, configurable           |
            \*-----------------------------------------*/
            case POLYCHROME_USB_ZONE_ARGB1:
            case POLYCHROME_USB_ZONE_ARGB2:
                newdev_info.device_type = PolychromeDeviceType::ADDRESSABLE;
                break;

            /*-----------------------------------------*\
            | Type: Addressable, not configurable       |
            \*-----------------------------------------*/
            case POLYCHROME_USB_ZONE_PCH:
            case POLYCHROME_USB_ZONE_IOCOVER:
            case POLYCHROME_USB_ZONE_PCB:
            case POLYCHROME_USB_ZONE_AUDIO:
                newdev_info.device_type = PolychromeDeviceType::FIXED;
                break;
            
            /*-----------------------------------------*\
            | Type: Fixed                               |
            \*-----------------------------------------*/
            case POLYCHROME_USB_ZONE_RGB1:
            case POLYCHROME_USB_ZONE_RGB2:
            default:
                newdev_info.device_type = PolychromeDeviceType::FIXED;
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
    | Set up message packet with leading 00                  |
    \*-----------------------------------------------------*/
	usb_buf[0x01] = POLYCHROME_USB_SET_ZONE;
	usb_buf[0x03] = zone;
	usb_buf[0x04] = mode;
	usb_buf[0x05] = RGBGetRValue(rgb);
	usb_buf[0x06] = RGBGetGValue(rgb);
	usb_buf[0x07] = RGBGetBValue(rgb);
	usb_buf[0x08] = speed;
	usb_buf[0x09] = 0xFF;
	usb_buf[0x10] = allzone;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 64);
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
    | Set up message packet with leading 00                 |
    \*-----------------------------------------------------*/
	usb_buf[0x01] = POLYCHROME_USB_WRITE_HEADER;

    usb_buf[0x04] = POLYCHROME_USB_RGSWAP_CFG;
	usb_buf[0x05] = ((ahdr1 << 3) & (ahdr0 << 2) & (hdr1 << 1) & hdr0) & 0x40;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 64);
}

void PolychromeUSBController::WriteHeader
    (
    unsigned char   cfg,
    unsigned char*  configstring,
    unsigned int    configsize
    )
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet with leading 00                 |
    \*-----------------------------------------------------*/
    usb_buf[0x01] = POLYCHROME_USB_WRITE_HEADER;
	usb_buf[0x03] = cfg;
    memcpy(&usb_buf[4], configstring, configsize);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 64);
}

PolychromeZoneInfo PolychromeUSBController::GetZoneConfig(unsigned char zone)
{
    unsigned char       usb_buf[65];
    PolychromeZoneInfo  zoneinfo;
    //unsigned char       all;
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
    usb_buf[0x01]   = POLYCHROME_USB_READ_ZONE_CONFIG;
    usb_buf[0x03]   = zone; 

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);

    /*-----------------------------------------------------*\
    | Read response                                         |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));
    hid_read(dev, usb_buf, 64);
    
    r   = usb_buf[0x05];
    g   = usb_buf[0x06];
    b   = usb_buf[0x07];

    zoneinfo.mode   = usb_buf[0x04];
    zoneinfo.color  = ToRGBColor(r,g,b);  
    zoneinfo.speed  = usb_buf[0x08];
    zoneinfo.zone   = usb_buf[0x03];
    //all             = usb_buf[0x10];

    return(zoneinfo);
}

void PolychromeUSBController::ReadConfigTables()
{
    unsigned char usb_buf[65];
    unsigned char maxzoneleds[8];
    unsigned char rgbswap;
    unsigned char header1;

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up max led config table request packet            |
    \*-----------------------------------------------------*/
    usb_buf[0x01]   = POLYCHROME_USB_READ_HEADER;
    usb_buf[0x03]   = POLYCHROME_USB_LEDCOUNT_CFG;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);

    /*-----------------------------------------------------*\
    | Read response                                         |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));
    hid_read(dev, usb_buf, 64);

    /*-----------------------------------------------------*\
    | Reads in format: RGB1, RGB2, ARGB1, ARGB2, PCH, IO,   |
    | PCB, AUDIO                                            |
    \*-----------------------------------------------------*/
    memcpy(&maxzoneleds,&usb_buf[0x04],8);

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up RGB Swap table request packet                  |
    \*-----------------------------------------------------*/
    usb_buf[0x01]   = POLYCHROME_USB_READ_HEADER;
    usb_buf[0x03]   = POLYCHROME_USB_RGSWAP_CFG;

    hid_write(dev, usb_buf, 65);
    memset(usb_buf, 0x00, sizeof(usb_buf));
    hid_read(dev, usb_buf, 64);

    /*-----------------------------------------------------*\
    | Reads bitwise in format: AUDIO, PCB, IO, PCH, ARGB2,  |
    | ARGB1, RGB2, RGB1 if available                        |
    \*-----------------------------------------------------*/
    rgbswap=usb_buf[4];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Header1 config table request packet            |
    \*-----------------------------------------------------*/
    usb_buf[0x01]   = POLYCHROME_USB_READ_HEADER;
    usb_buf[0x03]   = 0x01;

    hid_write(dev, usb_buf, 64);
    memset(usb_buf, 0x00, sizeof(usb_buf));
    hid_read(dev, usb_buf, 64);
    header1=usb_buf[4];

    memcpy(configtable,&maxzoneleds,8);
    configtable[8]=rgbswap;
    configtable[9]=header1;
    return;
}

void PolychromeUSBController::Commit()
{
    /*-----------------------------------------------------*\
    | Saves all Writes in Device -  Will keep after powerup |
    \*-----------------------------------------------------*/
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet with leading 00                  |
    \*-----------------------------------------------------*/
	usb_buf[0x01] = POLYCHROME_USB_COMMIT;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 64);
};

const std::vector<PolychromeDeviceInfo>& PolychromeUSBController::GetPolychromeDevices() const
{
    return(device_info);
};
