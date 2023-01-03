/*-----------------------------------------*\
|  AsusAuraMouseController.cpp              |
|                                           |
|  Driver for ASUS Aura RGB USB             |
|  lighting controller                      |
|                                           |
|  Adam Honse (CalcProgrammer1) 10/23/2020  |
\*-----------------------------------------*/

#include "AsusAuraMouseController.h"

#include <cstring>

AuraMouseController::AuraMouseController(hid_device* dev_handle, const char* path, uint16_t pid)
{
    dev         = dev_handle;
    location    = path;
    device_pid  = pid;
}

AuraMouseController::~AuraMouseController()
{
    hid_close(dev);
}

std::string AuraMouseController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string AuraMouseController::GetSerialString()
{
    wchar_t serial_string[HID_MAX_STR];
    int ret = hid_get_serial_number_string(dev, serial_string, HID_MAX_STR);

    if(ret != 0)
    {
        return("");
    }

    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());

    return(return_string);
}

std::string AuraMouseController::GetVersion(bool wireless, int protocol)
{
    unsigned char usb_buf[ASUS_AURA_MOUSE_PACKET_SIZE];
    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x12;
    hid_write(dev, usb_buf, ASUS_AURA_MOUSE_PACKET_SIZE);

    unsigned char usb_buf_out[ASUS_AURA_MOUSE_PACKET_SIZE];
    hid_read(dev, usb_buf_out, ASUS_AURA_MOUSE_PACKET_SIZE);

    std::string str;

    switch(protocol)
    {
        case 0:
            {
                unsigned char* offset = usb_buf_out + (wireless ? 13 : 4);
                str = std::string(offset, offset + 4);
            }
            break;

        case 1:
        case 2:
            {
                char version[9];
                int wireless_offset = (protocol == 2 ? 14 : 13);
                int offset = (wireless ? wireless_offset : 4);
                snprintf(version, 9, "%2X.%02X.%02X", usb_buf_out[offset + 2], usb_buf_out[offset + 1], usb_buf_out[offset]);
                str = std::string(version);
            }
            break;

        case 3:
            {
                unsigned char* offset = usb_buf_out + (wireless ? 13 : 4);
                str = std::string(offset, offset + 4);
                str = "0." + str.substr(0, 2) + "." + str.substr(2, 2);
            }
            break;

        case 4:
            {
                char version[16];
                int offset = (wireless ? 13 : 4);
                snprintf(version, 16, "%2d.%02d.%02d", usb_buf_out[offset + 1], usb_buf_out[offset + 2], usb_buf_out[offset + 3]);
                str = std::string(version);
            }
            break;
    }

    return str;
}

void AuraMouseController::SaveMode()
{
    unsigned char usb_save_buf[ASUS_AURA_MOUSE_PACKET_SIZE] = { 0x00, 0x50, 0x03 };

    hid_write(dev, usb_save_buf, ASUS_AURA_MOUSE_PACKET_SIZE);
}

void AuraMouseController::SendUpdate
    (
    unsigned char   zone,
    unsigned char   mode,
    unsigned char   red,
    unsigned char   grn,
    unsigned char   blu,
    unsigned char   dir,
    bool            random,
    unsigned char   speed,
    unsigned char   brightness
    )
{
    int bytes_read = 1;
    unsigned char usb_buf_flush[ASUS_AURA_MOUSE_PACKET_SIZE];

    while(bytes_read > 0)
    {
      bytes_read = hid_read_timeout(dev, usb_buf_flush, ASUS_AURA_MOUSE_PACKET_SIZE, 0);
    }

    unsigned char usb_buf[ASUS_AURA_MOUSE_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, ASUS_AURA_MOUSE_PACKET_SIZE);

    /*-----------------------------------------------------*\
    | Set up message packet                                 |
    \*-----------------------------------------------------*/
    
    if (device_pid == AURA_ROG_GLADIUS_II_ORIGIN_PNK_LTD_PID)
    {
        // this device supports 2 color for breathing,
        // but since this mode is per led and openrgb doesn't support 2 colors per led this feature is not implemented
        usb_buf[0x00]   = 0x00;
        usb_buf[0x01]   = 0x51;
        usb_buf[0x02]   = 0x28;
        usb_buf[0x03]   = zone;
        usb_buf[0x04]   = 0x00;
        usb_buf[0x05]   = mode;
        usb_buf[0x06]   = brightness;
        usb_buf[0x07]   = 0x00; // boolean signaling if the 2nd set of colors is in use
        usb_buf[0x08]   = red;
        usb_buf[0x09]   = grn;
        usb_buf[0x0A]   = blu;
        usb_buf[0x0B]   = 0; // 2nd red
        usb_buf[0x0C]   = 0; // 2nd green
        usb_buf[0x0D]   = 0; // 2nd blue
        usb_buf[0x0E]   = dir;
        usb_buf[0x0F]   = random;
        usb_buf[0x10]   = speed;
    }
    else
    {
        usb_buf[0x00]   = 0x00;
        usb_buf[0x01]   = 0x51;
        usb_buf[0x02]   = 0x28;
        usb_buf[0x03]   = zone;
        usb_buf[0x04]   = 0x00;
        usb_buf[0x05]   = mode;
        usb_buf[0x06]   = brightness;
        usb_buf[0x07]   = red;
        usb_buf[0x08]   = grn;
        usb_buf[0x09]   = blu;
        usb_buf[0x0A]   = dir;
        usb_buf[0x0B]   = random;
        usb_buf[0x0C]   = speed;
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, ASUS_AURA_MOUSE_PACKET_SIZE);

    unsigned char usb_buf_out[ASUS_AURA_MOUSE_PACKET_SIZE];
    hid_read_timeout(dev, usb_buf_out, ASUS_AURA_MOUSE_PACKET_SIZE, 10);
}
