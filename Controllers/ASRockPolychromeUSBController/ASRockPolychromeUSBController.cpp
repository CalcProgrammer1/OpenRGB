/*---------------------------------------------------------*\
| ASRockPolychromeUSBController.cpp                         |
|                                                           |
|   Driver for ASRock Polychrome USB motherboards           |
|                                                           |
|   Ed Kambulow (dredvard)                      20 Dec 2020 |
|   Shady Nawara (ShadyNawara)                  16 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include "RGBController.h"
#include "ResourceManager.h"
#include "SettingsManager.h"
#include "StringUtils.h"
#include "ASRockPolychromeUSBController.h"
#include "dmiinfo.h"

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
    return((unsigned int)device_info.size());
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

    return(StringUtils::wstring_to_string(serial_string));
}

void PolychromeUSBController::SetDeviceInfo()
{
    PolychromeDeviceInfo newdev_info;

    ReadConfigTables();

    /*--------------------------------------------------*\
    | Read settings to check for configured RGSwap       |
    \*--------------------------------------------------*/
    const std::string detector_name     = "ASRock Polychrome USB";
    const std::string json_rgswap       = "RGSwap";
    SettingsManager* settings_manager   = ResourceManager::get()->GetSettingsManager();
    json device_settings                = settings_manager->GetSettings(detector_name);

    /*---------------------------------------------------------*\
    | Get RGSwap settings from the settings manager             |
    | If RGSwap settings are not found then write them out      |
    | Onboard leds are set to their existing values,            |
    | Addressable RGB and RGB headers are set to false          |
    \*---------------------------------------------------------*/
    if(!device_settings.contains(json_rgswap))
    {
        device_settings[json_rgswap][polychrome_USB_zone_names[0]] = false;
        device_settings[json_rgswap][polychrome_USB_zone_names[1]] = false;
        device_settings[json_rgswap][polychrome_USB_zone_names[2]] = false;
        device_settings[json_rgswap][polychrome_USB_zone_names[3]] = false;
        device_settings[json_rgswap][polychrome_USB_zone_names[4]] = ((configtable[8] >> 4) & 1) ? true : false;
        device_settings[json_rgswap][polychrome_USB_zone_names[5]] = ((configtable[8] >> 5) & 1) ? true : false;
        device_settings[json_rgswap][polychrome_USB_zone_names[6]] = ((configtable[8] >> 6) & 1) ? true : false;
        device_settings[json_rgswap][polychrome_USB_zone_names[7]] = false;

        settings_manager->SetSettings(detector_name, device_settings);
        settings_manager->SaveSettings();
    }
    else
    {
        for(std::size_t idx = 0; idx < 8; idx++)
        {
            if(device_settings[json_rgswap].contains(polychrome_USB_zone_names[idx]))
            {
                rgswapconfig[idx] = device_settings[json_rgswap][polychrome_USB_zone_names[idx]];
            }
        }
    }

    bool rgswap_final[8] = {0};

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
        newdev_info.rgswap = ((configtable[8] >> zonecnt) & 1) || rgswapconfig[zonecnt];
        rgswap_final[zonecnt] = newdev_info.rgswap;

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

    // set rgswap to match our settings
    WriteRGSwap(rgswap_final[0], rgswap_final[1], rgswap_final[2], rgswap_final[3], rgswap_final[4], rgswap_final[5], rgswap_final[6], rgswap_final[7]);
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

    if(device_info.rgswap)
    {
        usb_buf[0x05] = RGBGetRValue(rgb);
        usb_buf[0x06] = RGBGetGValue(rgb);
    }
    else
    {
        usb_buf[0x05] = RGBGetGValue(rgb);
        usb_buf[0x06] = RGBGetRValue(rgb);
    }

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
    const std::vector<PolychromeZoneInfo>&  /*zones_info*/,
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
        hid_read(dev, usb_buf, 64);

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

/*-----------------------------------------------------*\
| If reset is true, rgswap is set to the values         |
| specified in the settings file, otherwise set to off  |
\*-----------------------------------------------------*/
void PolychromeUSBController::SetRGSwap(bool reset)
{
    if(reset)
    {
        bool rg[8] = {0};
        std::vector<PolychromeDeviceInfo> devices_info = GetPolychromeDevices();
        for (PolychromeDeviceInfo device : devices_info)
        {
            rg[device.zone_type] = device.rgswap;
        }
        WriteRGSwap(rg[0], rg[1], rg[2], rg[3], rg[4], rg[5], rg[6], rg[7]);
    }
    else
    {
        /*-----------------------------------------------------------------*\
        | Disable RGSwap as it causes flashing on each update in direct mode|
        \*-----------------------------------------------------------------*/
        WriteRGSwap(0, 0, 0, 0, 0, 0, 0, 0);
    }
}

void PolychromeUSBController::WriteRGSwap
    (
    bool hdr0,
    bool hdr1,
    bool ahdr0,
    bool ahdr1,
    bool pch,
    bool io,
    bool pcb,
    bool chnl8
    )
{
    unsigned char rgconfig[1] = {static_cast<unsigned char>((((unsigned char)chnl8 << 7) | ((unsigned char)pcb << 6) | ((unsigned char)io << 5) | ((unsigned char)pch << 4) | ((unsigned char)ahdr1 << 3) | ((unsigned char)ahdr0 << 2) | ((unsigned char)hdr1 << 1) | (unsigned char)hdr0))};
    WriteHeader(POLYCHROME_USB_RGSWAP_CFG, rgconfig, 1);
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
    | G & R are swapped since RGSwap is disabled on init,    |
    | Unless overwritten in settings                         |
    \*------------------------------------------------------*/
    if(device_info.rgswap)
    {
        zoneinfo.color  = ToRGBColor(r,g,b);
    }
    else
    {
        zoneinfo.color  = ToRGBColor(g,r,b);
    }
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
