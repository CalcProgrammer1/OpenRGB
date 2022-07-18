/*-----------------------------------------*\
|  HPOmen30LController.cpp                  |
|                                           |
|  Driver for HP Omen 30L RGB lighting      |
|  controller                               |
\*-----------------------------------------*/

#include "HPOmen30LController.h"
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

HPOmen30LController::HPOmen30LController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;

    strcpy(device_name, "HP Omen 30L");

    hp_zone logo;
    logo.value      = HP_OMEN_30L_LOGO_ZONE;
    logo.mode       = HP_OMEN_30L_DIRECT;
    logo.speed      = HP_OMEN_30L_SPEED_MED;
    logo.brightness = 0x64;
    hp_zones.push_back(logo);

    hp_zone bar;
    bar.value      = HP_OMEN_30L_BAR_ZONE;
    bar.mode       = HP_OMEN_30L_DIRECT;
    bar.speed      = HP_OMEN_30L_SPEED_MED;
    bar.brightness = 0x64;
    hp_zones.push_back(bar);

    hp_zone fan;
    fan.value      = HP_OMEN_30L_FAN_ZONE;
    fan.mode       = HP_OMEN_30L_DIRECT;
    fan.speed      = HP_OMEN_30L_SPEED_MED;
    fan.brightness = 0x64;
    hp_zones.push_back(fan);

    hp_zone cpu;
    cpu.value      = HP_OMEN_30L_CPU_ZONE;
    cpu.mode       = HP_OMEN_30L_DIRECT;
    cpu.speed      = HP_OMEN_30L_SPEED_MED;
    cpu.brightness = 0x64;
    hp_zones.push_back(cpu);

}

HPOmen30LController::~HPOmen30LController()
{
    hid_close(dev);
}

std::string HPOmen30LController::GetLocationString()
{
    return("HID: " + location);
}

char* HPOmen30LController::GetDeviceName()
{
    return device_name;
}

std::string HPOmen30LController::GetSerialString()
{
    std::string ret_string = "";
    return(ret_string);
}

std::string HPOmen30LController::GetEffectChannelString(unsigned char /*channel*/)
{
    std::string ret_string = "";
    return(ret_string);
}

std::string HPOmen30LController::GetFirmwareVersionString()
{
    std::string ret_string = "";
    return(ret_string);
}

void HPOmen30LController::SetZoneMode(int zone,unsigned char mode, unsigned char speed,unsigned char brightness)
{
    hp_zones[zone].mode       = mode;
    hp_zones[zone].speed      = speed;
    hp_zones[zone].brightness = brightness;

}

void HPOmen30LController::SetZoneColor(int zone, std::vector<RGBColor> colors)
{
    SendZoneUpdate(zone, colors);
}

void HPOmen30LController::SendZoneUpdate(int zone, std::vector<RGBColor> colors)
{
    unsigned char usb_buf[] =
    {
        0x00, 0x00,             // [0x00-0x01]
        0x12, 0x05, 0x00, 0x00, // [0x02-0x05]
        0x00, 0x00, 0x00, 0x00, // [0x06-0x09]
        0x00, 0x00, 0x00, 0x00, // [0x0A-0x0D]
        0x00, 0x00, 0x00, 0x00, // [0x0E-0x11]
        0x00, 0x00, 0x00, 0x00, // [0x12-0x15]
        0x00, 0x00, 0x00, 0x00, // [0x16-0x19]
        0x00, 0x00, 0x00, 0x00, // [0x1A-0x1D]
        0x00, 0x00, 0x00, 0x00, // [0x1E-0x21]
        0x00, 0x00, 0x00, 0x00, // [0x22-0x25]
        0x00, 0x00, 0x00, 0x00, // [0x26-0x29]
        0x00, 0x00, 0x00, 0x00, // [0x2A-0x2D]
        0x00, 0x00, 0x00, 0x00, // [0x2E-0x31]
        0x00, 0x00, 0x00, 0x00, // [0x32-0x35] Always 0x00*4
        0x00, 0x00, 0x00, 0x00  // [0x36-0x39] zone / 0x01 / theme / speed
    };

    usb_buf[0x36]   = hp_zones[zone].value;

    if(hp_zones[zone].mode != HP_OMEN_30L_DIRECT)
    {
        hid_write(dev, usb_buf, 58);
    }

    usb_buf[0x37]   = 0x01;
    usb_buf[0x39]   = hp_zones[zone].speed;
    usb_buf[0x03]   = hp_zones[zone].mode;
    usb_buf[0x30]   = hp_zones[zone].brightness;

    if(hp_zones[zone].mode == HP_OMEN_30L_DIRECT)
    {
        usb_buf[0x31]   = HP_OMEN_30L_DIRECT;
        usb_buf[0x04]   = 0x01;
    }
    else
    {
        usb_buf[0x31]   = 0x0A;
    }

    if(hp_zones[zone].mode == HP_OMEN_30L_DIRECT)
    {
        usb_buf[0x08] = usb_buf[0x0C] = usb_buf[0x10] = usb_buf[0x14] = 0x64;
        usb_buf[0x09] = usb_buf[0x0D] = usb_buf[0x11] = usb_buf[0x15] = RGBGetRValue(colors[zone]);
        usb_buf[0x0A] = usb_buf[0x0E] = usb_buf[0x12] = usb_buf[0x16] = RGBGetGValue(colors[zone]);
        usb_buf[0x0B] = usb_buf[0x0F] = usb_buf[0x13] = usb_buf[0x17] = RGBGetBValue(colors[zone]);

        hid_write(dev, usb_buf, 58);
    }
    else if(hp_zones[zone].mode == HP_OMEN_30L_STATIC)
    {
        usb_buf[0x08] = usb_buf[0x0B] = usb_buf[0x0E] = usb_buf[0x11] = RGBGetRValue(colors[zone]);
        usb_buf[0x09] = usb_buf[0x0C] = usb_buf[0x0F] = usb_buf[0x12] = RGBGetGValue(colors[zone]);
        usb_buf[0x0A] = usb_buf[0x0D] = usb_buf[0x10] = usb_buf[0x13] = RGBGetBValue(colors[zone]);

        hid_write(dev, usb_buf, 58);
    }
    else
    {
        usb_buf[0x04] = colors.size();

        for(unsigned int i = 0; i < colors.size(); i++)
        {
            usb_buf[0x05] = i + 1;

            usb_buf[0x08] = usb_buf[0x0B] = usb_buf[0x0E] = usb_buf[0x11] = RGBGetRValue(colors[i]);
            usb_buf[0x09] = usb_buf[0x0C] = usb_buf[0x0F] = usb_buf[0x12] = RGBGetGValue(colors[i]);
            usb_buf[0x0A] = usb_buf[0x0D] = usb_buf[0x10] = usb_buf[0x13] = RGBGetBValue(colors[i]);

            hid_write(dev, usb_buf, 58);
        }
    }
}
