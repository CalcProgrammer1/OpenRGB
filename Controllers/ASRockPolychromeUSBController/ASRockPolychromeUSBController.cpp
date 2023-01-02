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
#define POLYCHROME_USB_INITIAL_CHUNK    0xE3
#define POLYCHROME_USB_SEND_CHUNK       0xE4
#define POLYCHROME_USB_INIT             0xA4
#define POLYCHROME_USB_COMMIT           0x12

const char* polychrome_USB_zone_names[] =
{
    "RGB LED 1 Header",
    "RGB LED 2 Header",
    "Addressable Header 1",
    "Addressable Header 2",
    "PCH",
    "IO Cover",
    "PCB",
    "Addressable Header 3/Audio",
};

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

    /*--------------------------------------------------*\
    | Disable RGSwap as it causes flashing on each update|
    \*--------------------------------------------------*/
    WriteRGSwap(0,0,0,0,0,0,0,0);
    ReadConfigTables();

    for (unsigned int zonecnt = 0; zonecnt < POLYCHROME_USB_ZONE_MAX_NUM; zonecnt++)
    {
        if(configtable[zonecnt] == 0x1E || !((configtable[9] >> zonecnt) & 1))
        {
			/*-----------------------------------------------------------------------*\
			| If we don't have this device type (0x1E) we will skip it and continue.  |
            | Or if the device is not available we skip it and continue               |
			\*-----------------------------------------------------------------------*/
            continue;
        }

        newdev_info.num_leds = configtable[zonecnt];
        newdev_info.rgswap = ((configtable[8] >> zonecnt) & 1);

		/*--------------------------------------------------------------------------------------------------*\
		| We will need to know what zone type this is, so that we can look up the name and make calls later. |
		\*--------------------------------------------------------------------------------------------------*/
        newdev_info.zone_type = zonecnt;

        switch (zonecnt)
        {
            /*-----------------------------------------*\
            | Type: Addressable, configurable           |
            \*-----------------------------------------*/
            case POLYCHROME_USB_ZONE_ARGB1:
            case POLYCHROME_USB_ZONE_ARGB2:
            /*---------------------------------------------*\
            | The last led channel is allocated to a       |
            | third ARGB header on some newer motherboards |
            \*---------------------------------------------*/
            case POLYCHROME_USB_ZONE_AUDIO:
                newdev_info.device_type = PolychromeDeviceType::ADDRESSABLE;
                break;

            /*-----------------------------------------*\
            | Type: Addressable, not configurable       |
            \*-----------------------------------------*/
            case POLYCHROME_USB_ZONE_PCH:
            case POLYCHROME_USB_ZONE_IOCOVER:
            case POLYCHROME_USB_ZONE_PCB:
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

void PolychromeUSBController::ResizeZone(int zone, int new_size)
{
    unsigned char zonecfg[POLYCHROME_USB_ZONE_MAX_NUM];

    memset(zonecfg, POLYCHROME_USB_ZONE_UNAVAILABLE, POLYCHROME_USB_ZONE_MAX_NUM);

    configtable[zone] = new_size;

    for(unsigned int i = 0; i < POLYCHROME_USB_ZONE_MAX_NUM; i++)
    {
        zonecfg[i] = configtable[i];
    }

    unsigned char zonecmd = POLYCHROME_USB_LEDCOUNT_CFG;
    WriteHeader(zonecmd, zonecfg, sizeof(zonecfg));
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

	/*----------------------------------------------------*\
    | Get the device info so we can look up the zone type. | 
	\*----------------------------------------------------*/
    PolychromeDeviceInfo device_info = GetPolychromeDevices()[zone];

    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet with leading 00                  |
    \*-----------------------------------------------------*/
	usb_buf[0x01] = POLYCHROME_USB_SET_ZONE;
    usb_buf[0x03] = device_info.zone_type; 
	usb_buf[0x04] = mode;
    usb_buf[0x05] = RGBGetGValue(rgb);
    usb_buf[0x06] = RGBGetRValue(rgb);
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

void PolychromeUSBController::WriteAllZones
    (
    const std::vector<PolychromeZoneInfo>&  zones_info,
    const std::vector<zone>&                zones
    )
{
    std::vector<unsigned char> combined_leds_rgb;
    std::size_t max_led_count = 0;
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++){
        PolychromeDeviceInfo device_info = GetPolychromeDevices()[zone_idx];

        for(std::size_t led_idx = 0; led_idx < zones[zone_idx].leds_count; led_idx++)
        {
            if(device_info.rgswap)
            {
                combined_leds_rgb.push_back(RGBGetRValue(zones[zone_idx].colors[led_idx]));
                combined_leds_rgb.push_back(RGBGetGValue(zones[zone_idx].colors[led_idx]));
            }
            else
            {
                combined_leds_rgb.push_back(RGBGetGValue(zones[zone_idx].colors[led_idx]));
                combined_leds_rgb.push_back(RGBGetRValue(zones[zone_idx].colors[led_idx]));
            }
            combined_leds_rgb.push_back(RGBGetBValue(zones[zone_idx].colors[led_idx]));
        }
        max_led_count += zones[zone_idx].leds_max;
    }
    combined_leds_rgb.resize((max_led_count + 8) * 3, 0);

    unsigned char usb_buf[65];
    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up initial message packet with leading 00         |
    \*-----------------------------------------------------*/
    usb_buf[0x01] = POLYCHROME_USB_SET_ZONE;
    usb_buf[0x03] = 0xFF;
    usb_buf[0x04] = POLYCHROME_USB_INITIAL_CHUNK;
    usb_buf[0x07] = 0xFF;
    usb_buf[0x40] = 0x65;

    std::size_t initial_led_offset = 9;
    std::size_t message_byte_capacity = 54;           // 18 leds * 3 bytes

    std::size_t led_byte_idx = 0;

    while(led_byte_idx < combined_leds_rgb.size())
    {
        for(std::size_t byte_idx = 0; byte_idx < message_byte_capacity; byte_idx++)
        {
            usb_buf[initial_led_offset + byte_idx] = combined_leds_rgb[led_byte_idx++];
            if(led_byte_idx >= combined_leds_rgb.size())
            {
                break;
            }
        }

        /*-----------------------------------------------------*\
        | Send packet                                           |
        \*-----------------------------------------------------*/
        hid_write(dev, usb_buf, 65);

        /*-----------------------------------------------------*\
        | Zero out buffer                                       |
        \*-----------------------------------------------------*/
        memset(usb_buf, 0x00, sizeof(usb_buf));

        /*-----------------------------------------------------*\
        | Set up message packet with leading 00                  |
        \*-----------------------------------------------------*/
        usb_buf[0x01] = POLYCHROME_USB_SET_ZONE;
        usb_buf[0x03] = 0xFF;
        usb_buf[0x04] = POLYCHROME_USB_SEND_CHUNK;
        usb_buf[0x40] = 0x65;

        initial_led_offset = 5;
        message_byte_capacity = 57;                 // 19 leds * 3 bytes
    }
}

void PolychromeUSBController::WriteRGSwap
    (
    bool ahdr1,
    bool ahdr0,
    bool hdr1,
    bool hdr0,
    bool pch,
    bool io,
    bool pcb,
    bool chnl8
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

    usb_buf[0x03] = POLYCHROME_USB_RGSWAP_CFG;
    usb_buf[0x04] = ((chnl8 << 7) & (pcb << 6) & (io << 5) & (pch << 4) & (ahdr1 << 3) & (ahdr0 << 2) & (hdr1 << 1) & hdr0);

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
    /*-----------------------------------------------------*\
    | Get the device info so we can look up the zone type   |
	\*-----------------------------------------------------*/
    PolychromeDeviceInfo device_info = GetPolychromeDevices()[zone];

    unsigned char       usb_buf[65];
    PolychromeZoneInfo  zoneinfo;
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
    usb_buf[0x03]   = device_info.zone_type; 

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

    /*------------------------------------------------------*\
    | Set Chroma mode (0xE2) & Per-Led mode (0xE3) as Direct |
    \*------------------------------------------------------*/
    zoneinfo.mode   = usb_buf[0x04] != 0xE2 && usb_buf[0x04] != 0xE3 && usb_buf[0x04] < 0x0F ? usb_buf[0x04] : 0x0F;

    /*------------------------------------------------------*\
    | G & R are swapped since RGSwap is disabled on init     |
    \*------------------------------------------------------*/
    zoneinfo.color  = ToRGBColor(g,r,b);
    zoneinfo.speed  = usb_buf[0x08];
    zoneinfo.zone   = usb_buf[0x03];

    return(zoneinfo);
}

void PolychromeUSBController::ReadConfigTables()
{
    unsigned char usb_buf[65];
    unsigned char maxzoneleds[8];
    unsigned char rgswap;
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
    | PCB, AUDIO/ARGB3                                      |
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
    | Reads bitwise in format: AUDIO/ARGB3, PCB, IO, PCH,   |
    | ARGB2,ARGB1, RGB2, RGB1 if available                  |
    \*-----------------------------------------------------*/
    rgswap=usb_buf[4];

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

    /*-----------------------------------------------------*\
    | Reads bitwise in format: AUDIO/ARGB3, PCB, IO, PCH,   |
    | ARGB2,ARGB1, RGB2, RGB1 if available                  |
    \*-----------------------------------------------------*/
    header1=usb_buf[4];

    memcpy(configtable,&maxzoneleds,8);
    configtable[8]=rgswap;
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
