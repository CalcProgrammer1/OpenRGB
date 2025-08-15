/*---------------------------------------------------------*\
| CorsairPeripheralController.cpp                           |
|                                                           |
|   Driver for Corsair peripherals                          |
|                                                           |
|   Adam Honse (CalcProgrammer1)                09 Jan 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "CorsairPeripheralController.h"
#include "LogManager.h"
#include "StringUtils.h"

using namespace std::chrono_literals;

static unsigned int keys[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0C, 0x0D, 0x0E, 0x0F, 0x11, 0x12,
                              0x14, 0x15, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x24, 0x25, 0x26,
                              0x27, 0x28, 0x2A, 0x2B, 0x2C, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
                              0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x42, 0x43, 0x44, 0x45, 0x48, 73,   74,   75,   76,   78,
                              79,   80,   81,   84,   85,   86,   87,   88,   89,   90,   91,   92,   93,   96,   97,
                              98,   99,   100,  101,  102,  103,  104,  105,  108,  109,  110,  111,  112,  113,  115,
                              116,  117,  120,  121,  122,  123,  124,  126,  127,  128,  129,  132,  133,  134,  135,
                              136,  137,  139,  140,  141, 0x10, 114};

static unsigned int keys_k70_mk2[] =   {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0C, 0x0D, 0x0E, 0x0F, 0x11, 0x12,
                                        0x14, 0x15, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x24, 0x25, 0x26,
                                        0x27, 0x28, 0x2A, 0x2B, 0x2C, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
                                        0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x42, 0x43, 0x44, 0x45, 0x48, 73,   74,   75,   76,   78,
                                        79,   80,   81,   84,   85,   86,   87,   88,   89,   90,   91,   92,   93,   96,   97,
                                        98,   99,   100,  101,  102,  103,  104,  105,  108,  109,  110,  111,  112,  113,  115,
                                        116,  117,  120,  121,  122,  123,  124,  126,  127,  128,  129,  132,  133,  134,  135,
                                        136,  137,  139,  140,  141,  16,   114,  47,   59,   125 };


static unsigned int keys_k95_plat[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0C, 0x0D, 0x0E, 0x0F, 0x11, 0x12,
                                       0x14, 0x15, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x24, 0x25, 0x26,
                                       0x27, 0x28, 0x2A, 0x2B, 0x2C, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
                                       0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x42, 0x43, 0x44, 0x45, 0x48, 73,   74,   75,   76,   78,
                                       79,   80,   81,   84,   85,   86,   87,   88,   89,   90,   91,   92,   93,   96,   97,
                                       98,   99,   100,  101,  102,  103,  104,  105,  108,  109,  110,  111,  112,  113,  115,
                                       116,  117,  120,  121,  122,  123,  124,  126,  127,  128,  129,  132,  133,  134,  135,
                                       136,  137,  139,  140,  141,
                                       0x10, 114, 0x0a, 0x16, 0x22, 0x2e, 0x3a, 0x46,  125,
                                       144,  145,  146,  158,  160,  147,  148,  149,  150,  151,  152,  153,
                                       154,  155,  159,  162,  161,  156,  157};

static unsigned int keys_k95[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0C, 0x0D, 0x0E, 0x0F, 0x11, 0x12,
                                  0x14, 0x15, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x24, 0x25, 0x26,
                                  0x27, 0x28, 0x2A, 0x2B, 0x2C, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
                                  0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x42, 0x43, 0x44, 0x45, 0x48, 73,   74,   75,   76,   78,
                                  79,   80,   81,   84,   85,   86,   87,   88,   89,   90,   91,   92,   93,   96,   97,
                                  98,   99,   100,  101,  102,  103,  104,  105,  108,  109,  110,  111,  112,  113,  115,
                                  116,  117,  120,  121,  122,  123,  124,  126,  127,  128,  129,  132,  133,  134,  135,
                                  136,  137,  139,  140,  141,
                                  0x10, 114, 0x0a, 0x16, 0x22, 0x2e, 0x3a, 0x46, 0x52, 0x5e, 0x6a, 0x76, 0x3b, 0x47, 0x53,
                                  0x5f, 0x6b, 0x77, 0x83, 0x8f, 0x0b, 0x17, 0x23, 0x2f};

static unsigned int st100[] = { 0x00, 0x01, 0x02, 0x03, 0x05, 0x06, 0x07, 0x08, 0x04 };

static unsigned int key_mapping_k95_plat_ansi[] = { 0x31, 0x3f, 0x41, 0x42, 0x51, 0x53, 0x55, 0x6f, 0x7e, 0x7f, 0x80, 0x81 };
static unsigned int key_mapping_k95_plat_iso[] =  { 0x3f, 0x41, 0x42, 0x50, 0x53, 0x55, 0x6f, 0x78, 0x7e, 0x7f, 0x80, 0x81 };
static unsigned int key_mapping_k70_mk2_plat_iso[] = { 0x3f, 0x41, 0x42, 0x50, 0x53, 0x55, 0x6f, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 0x80, 0x81 };

#define CORSAIR_PERIPHERAL_CONTROLLER_NAME "Corsair peripheral"

CorsairPeripheralController::CorsairPeripheralController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;

    ReadFirmwareInfo();

    /*-----------------------------------------------------*\
    | K55 and K95 Platinum require additional steps         |
    \*-----------------------------------------------------*/
    if (logical_layout == CORSAIR_TYPE_K55 || logical_layout == CORSAIR_TYPE_K95_PLAT || logical_layout == CORSAIR_TYPE_K70_MK2 || logical_layout == CORSAIR_TYPE_K68)
    {
        SpecialFunctionControl();
    }

    LightingControl();

    if (logical_layout == CORSAIR_TYPE_K55 || logical_layout == CORSAIR_TYPE_K95_PLAT || logical_layout == CORSAIR_TYPE_K70_MK2 || logical_layout == CORSAIR_TYPE_K68)
    {
        SetupK55AndK95LightingControl();
    }
}

CorsairPeripheralController::~CorsairPeripheralController()
{
    hid_close(dev);
}

device_type CorsairPeripheralController::GetDeviceType()
{
    return type;
}

std::string CorsairPeripheralController::GetDeviceLocation()
{
    return("HID: " + location);
}

int CorsairPeripheralController::GetPhysicalLayout()
{
    return physical_layout;
}

int CorsairPeripheralController::GetLogicalLayout()
{
    return logical_layout;
}

std::string CorsairPeripheralController::GetFirmwareString()
{
    return firmware_version;
}

std::string CorsairPeripheralController::GetName()
{
    return name;
}

std::string CorsairPeripheralController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void CorsairPeripheralController::SetLEDs(std::vector<RGBColor>colors)
{
    switch(type)
    {
        case DEVICE_TYPE_KEYBOARD:
            if (logical_layout == CORSAIR_TYPE_K55)
            {
                SubmitKeyboardZonesColors(colors[0], colors[1], colors[2]);
            }
            else
            {
                SetLEDsKeyboardFull(colors);
            }
            break;

        case DEVICE_TYPE_MOUSE:
            SetLEDsMouse(colors);
            break;

        case DEVICE_TYPE_MOUSEMAT:
            SetLEDsMousemat(colors);
            break;

        case DEVICE_TYPE_HEADSET_STAND:
            /*-----------------------------------------------------*\
            | The logo zone of the ST100 is in the middle of the    |
            | base LED strip, so remap the colors so that the logo  |
            | is the last LED in the sequence.                      |
            \*-----------------------------------------------------*/
            std::vector<RGBColor> remap_colors;
            remap_colors.resize(colors.size());

            for(int i = 0; i < 9; i++)
            {
                remap_colors[st100[i]] = colors[i];
            }

            /*-----------------------------------------------------*\
            | The ST100 uses the mousemat protocol                  |
            \*-----------------------------------------------------*/
            SetLEDsMousemat(remap_colors);
            break;
    }
}

void CorsairPeripheralController::SetLEDsKeyboardFull(std::vector<RGBColor> colors)
{
    unsigned char red_val[168];
    unsigned char grn_val[168];
    unsigned char blu_val[168];
    unsigned char data_sz = 24;

    /*-----------------------------------------------------*\
    | Zero out buffers                                      |
    \*-----------------------------------------------------*/
    memset(red_val, 0x00, sizeof( red_val ));
    memset(grn_val, 0x00, sizeof( grn_val ));
    memset(blu_val, 0x00, sizeof( blu_val ));

    /*-----------------------------------------------------*\
    | Copy red, green, and blue components into buffers     |
    \*-----------------------------------------------------*/
    for(std::size_t color_idx = 0; color_idx < colors.size(); color_idx++)
    {
        RGBColor           color = colors[color_idx];
        if (logical_layout == CORSAIR_TYPE_K95_PLAT)
        {
            red_val[keys_k95_plat[color_idx]] = RGBGetRValue(color);
            grn_val[keys_k95_plat[color_idx]] = RGBGetGValue(color);
            blu_val[keys_k95_plat[color_idx]] = RGBGetBValue(color);
            data_sz = 48;
        }
        else if (logical_layout == CORSAIR_TYPE_K95)
        {
            red_val[keys_k95[color_idx]] = RGBGetRValue(color);
            grn_val[keys_k95[color_idx]] = RGBGetGValue(color);
            blu_val[keys_k95[color_idx]] = RGBGetBValue(color);
            data_sz = 48; //untested
        }
        else if (logical_layout == CORSAIR_TYPE_K70_MK2)
        {
            red_val[keys_k70_mk2[color_idx]] = RGBGetRValue(color);
            grn_val[keys_k70_mk2[color_idx]] = RGBGetGValue(color);
            blu_val[keys_k70_mk2[color_idx]] = RGBGetBValue(color);
        }
        else
        {
            red_val[keys[color_idx]] = RGBGetRValue(color);
            grn_val[keys[color_idx]] = RGBGetGValue(color);
            blu_val[keys[color_idx]] = RGBGetBValue(color);
        }
    }

    /*-----------------------------------------------------*\
    | Send red bytes                                        |
    \*-----------------------------------------------------*/
    StreamPacket(1, 60, &red_val[0]);
    StreamPacket(2, 60, &red_val[60]);
    StreamPacket(3, data_sz, &red_val[120]);
    SubmitKeyboardFullColors(1, 3, 1);

    /*-----------------------------------------------------*\
    | Send green bytes                                      |
    \*-----------------------------------------------------*/
    StreamPacket(1, 60, &grn_val[0]);
    StreamPacket(2, 60, &grn_val[60]);
    StreamPacket(3, data_sz, &grn_val[120]);
    SubmitKeyboardFullColors(2, 3, 1);

    /*-----------------------------------------------------*\
    | Send blue bytes                                       |
    \*-----------------------------------------------------*/
    StreamPacket(1, 60, &blu_val[0]);
    StreamPacket(2, 60, &blu_val[60]);
    StreamPacket(3, data_sz, &blu_val[120]);
    SubmitKeyboardFullColors(3, 3, 2);
}

void CorsairPeripheralController::SetLEDsMouse(std::vector<RGBColor> colors)
{
    SubmitMouseColors((unsigned char)colors.size(), &colors[0]);
}

void CorsairPeripheralController::SetLEDsMousemat(std::vector<RGBColor> colors)
{
    SubmitMousematColors((unsigned char)colors.size(), &colors[0]);
}

void CorsairPeripheralController::SetLEDsKeyboardLimited(std::vector<RGBColor> colors)
{
    unsigned char data_pkt[216];
    unsigned char red_val[144];
    unsigned char grn_val[144];
    unsigned char blu_val[144];

    /*-----------------------------------------------------*\
    | Zero out buffers                                      |
    \*-----------------------------------------------------*/
    memset(data_pkt, 0x00, sizeof( data_pkt ));
    memset(red_val, 0x00, sizeof( red_val ));
    memset(grn_val, 0x00, sizeof( grn_val ));
    memset(blu_val, 0x00, sizeof( blu_val ));

    /*-----------------------------------------------------*\
    | Scale color values to 9-bit                           |
    \*-----------------------------------------------------*/
    for(std::size_t color_idx = 0; color_idx < colors.size(); color_idx++)
    {
        RGBColor      color = colors[color_idx];
        unsigned char red   = RGBGetRValue(color);
        unsigned char grn   = RGBGetGValue(color);
        unsigned char blu   = RGBGetBValue(color);

        if( red > 7 ) red = 7;
        if( grn > 7 ) grn = 7;
        if( blu > 7 ) blu = 7;

        red = 7 - red;
        grn = 7 - grn;
        blu = 7 - blu;

        red_val[keys[color_idx]] = red;
        grn_val[keys[color_idx]] = grn;
        blu_val[keys[color_idx]] = blu;
    }

    /*-----------------------------------------------------*\
    | Pack the color values, 2 values per byte              |
    \*-----------------------------------------------------*/
    for(int red_idx = 0; red_idx < 72; red_idx++)
    {
        data_pkt[red_idx] = red_val[(red_idx * 2) + 1] << 4 | red_val[red_idx * 2];
    }

    for(int grn_idx = 0; grn_idx < 72; grn_idx++)
    {
        data_pkt[grn_idx + 72] = grn_val[(grn_idx * 2) + 1] << 4 | grn_val[grn_idx * 2];
    }

    for(int blu_idx = 0; blu_idx < 72; blu_idx++)
    {
        data_pkt[blu_idx + 144] = blu_val[(blu_idx * 2) + 1] << 4 | blu_val[blu_idx * 2];
    }

    /*-----------------------------------------------------*\
    | Send the packets                                      |
    \*-----------------------------------------------------*/
    StreamPacket(1, 60, &data_pkt[0]);
    StreamPacket(2, 60, &data_pkt[60]);
    StreamPacket(3, 60, &data_pkt[120]);
    StreamPacket(4, 36, &data_pkt[180]);

    SubmitKeyboardLimitedColors(216);
}

void CorsairPeripheralController::SwitchMode(bool software)
{
    if(software)
    {
        if (logical_layout == CORSAIR_TYPE_K55 || logical_layout == CORSAIR_TYPE_K95_PLAT || logical_layout == CORSAIR_TYPE_K70_MK2 || logical_layout == CORSAIR_TYPE_K68)
        {
            SpecialFunctionControl();
        }

        LightingControl();

        if (logical_layout == CORSAIR_TYPE_K55 || logical_layout == CORSAIR_TYPE_K95_PLAT || logical_layout == CORSAIR_TYPE_K70_MK2 || logical_layout == CORSAIR_TYPE_K68)
        {
            SetupK55AndK95LightingControl();
        }
    }
    else
    {
        unsigned char usb_buf[CORSAIR_PERIPHERAL_PACKET_LENGTH];

        memset(usb_buf, 0x00, CORSAIR_PERIPHERAL_PACKET_LENGTH);

        usb_buf[1] = CORSAIR_COMMAND_WRITE;
        usb_buf[2] = CORSAIR_PROPERTY_SPECIAL_FUNCTION;
        usb_buf[3] = CORSAIR_LIGHTING_CONTROL_HARDWARE;

        hid_write(dev, usb_buf, CORSAIR_PERIPHERAL_PACKET_LENGTH);
    }
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void CorsairPeripheralController::LightingControl()
{
    unsigned char usb_buf[CORSAIR_PERIPHERAL_PACKET_LENGTH];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, CORSAIR_PERIPHERAL_PACKET_LENGTH);

    /*-----------------------------------------------------*\
    | Set up Lighting Control packet                        |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x00;
    usb_buf[0x01]           = CORSAIR_COMMAND_WRITE;
    usb_buf[0x02]           = CORSAIR_PROPERTY_LIGHTING_CONTROL;
    usb_buf[0x03]           = CORSAIR_LIGHTING_CONTROL_SOFTWARE;

    /*-----------------------------------------------------*\
    | Lighting control byte needs to be 3 for keyboards and |
    | headset stand, 1 for mice and mousepads               |
    \*-----------------------------------------------------*/
    switch(type)
    {
        default:
        case DEVICE_TYPE_KEYBOARD:
            usb_buf[0x05]   = 0x03; // On K95 Platinum, this controls keyboard brightness
            break;

        case DEVICE_TYPE_MOUSE:
            usb_buf[0x05]   = 0x01;
            break;

        case DEVICE_TYPE_MOUSEMAT:
            usb_buf[0x05]   = 0x04;
            break;

        case DEVICE_TYPE_HEADSET_STAND:
            usb_buf[0x05]   = 0x03;
            break;
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, CORSAIR_PERIPHERAL_PACKET_LENGTH);
}

/*-----------------------------------------------------*\
| Probably a key mapping packet?                        |
\*-----------------------------------------------------*/

void CorsairPeripheralController::SetupK55AndK95LightingControl()
{
    unsigned char usb_buf[CORSAIR_PERIPHERAL_PACKET_LENGTH];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, CORSAIR_PERIPHERAL_PACKET_LENGTH);

    /*-----------------------------------------------------*\
    | Set up a packet                                       |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x00;
    usb_buf[0x01]           = CORSAIR_COMMAND_WRITE;
    usb_buf[0x02]           = CORSAIR_PROPERTY_LIGHTING_CONTROL;
    usb_buf[0x03]           = 0x08;

    usb_buf[0x05]           = 0x01;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, CORSAIR_PERIPHERAL_PACKET_LENGTH);

    unsigned int* skipped_identifiers = key_mapping_k95_plat_ansi;
    int skipped_identifiers_count = sizeof(key_mapping_k95_plat_ansi) / sizeof(key_mapping_k95_plat_ansi[0]);

    if (physical_layout == CORSAIR_LAYOUT_ISO)
    {
        if(logical_layout == CORSAIR_TYPE_K70_MK2)
        {
            skipped_identifiers = key_mapping_k70_mk2_plat_iso;
            skipped_identifiers_count = sizeof(key_mapping_k70_mk2_plat_iso) / sizeof(key_mapping_k70_mk2_plat_iso[0]);
        }
        else
        {
            skipped_identifiers = key_mapping_k95_plat_iso;
            skipped_identifiers_count = sizeof(key_mapping_k95_plat_iso) / sizeof(key_mapping_k95_plat_iso[0]);
        }
    }

    unsigned int identifier = 0;
    for (int i = 0; i < 4; i++)
    {
        /*-----------------------------------------------------*\
        | Zero out buffer                                       |
        \*-----------------------------------------------------*/
        memset(usb_buf, 0x00, CORSAIR_PERIPHERAL_PACKET_LENGTH);

        /*-----------------------------------------------------*\
        | Set up a packet - a sequence of 120 ids               |
        \*-----------------------------------------------------*/
        usb_buf[0x00]           = 0x00;
        usb_buf[0x01]           = CORSAIR_COMMAND_WRITE;
        usb_buf[0x02]           = 0x40;
        usb_buf[0x03]           = 0x1E;

        for (int j = 0; j < 30; j++)
        {
            for (int j = 0; j < skipped_identifiers_count; j++)
            {
                if (identifier == skipped_identifiers[j])
                {
                    identifier++;
                }
            }

            usb_buf[5 + 2 * j]      = identifier++;
            usb_buf[5 + 2 * j + 1]  = 0xC0;
        }

        /*-----------------------------------------------------*\
        | Send packet                                           |
        \*-----------------------------------------------------*/
        hid_write(dev, usb_buf, CORSAIR_PERIPHERAL_PACKET_LENGTH);
    }
}

void CorsairPeripheralController::SpecialFunctionControl()
{
    unsigned char usb_buf[CORSAIR_PERIPHERAL_PACKET_LENGTH];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, CORSAIR_PERIPHERAL_PACKET_LENGTH);

    /*-----------------------------------------------------*\
    | Set up Lighting Control packet                        |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x00;
    usb_buf[0x01]           = CORSAIR_COMMAND_WRITE;
    usb_buf[0x02]           = CORSAIR_PROPERTY_SPECIAL_FUNCTION;
    usb_buf[0x03]           = CORSAIR_LIGHTING_CONTROL_SOFTWARE;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, CORSAIR_PERIPHERAL_PACKET_LENGTH);
}

void CorsairPeripheralController::ReadFirmwareInfo()
{
    int  actual;
    unsigned char usb_buf[CORSAIR_PERIPHERAL_PACKET_LENGTH];
    char offset = 0;

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, CORSAIR_PERIPHERAL_PACKET_LENGTH);

    /*-----------------------------------------------------*\
    | Set up Read Firmware Info packet                      |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = CORSAIR_COMMAND_READ;
    usb_buf[0x02]   = CORSAIR_PROPERTY_FIRMWARE_INFO;

    /*-----------------------------------------------------*\
    | Send packet and try reading it using an HID read      |
    | If that fails, repeat the send and read the reply as  |
    | a feature report.                                     |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, CORSAIR_PERIPHERAL_PACKET_LENGTH);
    actual = hid_read_timeout(dev, usb_buf, CORSAIR_PERIPHERAL_PACKET_LENGTH, 1000);

    if(actual == 0)
    {
        /*-------------------------------------------------*\
        | Zero out buffer                                   |
        \*-------------------------------------------------*/
        memset(usb_buf, 0x00, CORSAIR_PERIPHERAL_PACKET_LENGTH);

        /*-------------------------------------------------*\
        | Set up Read Firmware Info packet                  |
        \*-------------------------------------------------*/
        usb_buf[0x00]   = 0x00;
        usb_buf[0x01]   = CORSAIR_COMMAND_READ;
        usb_buf[0x02]   = CORSAIR_PROPERTY_FIRMWARE_INFO;

        hid_send_feature_report(dev, usb_buf, CORSAIR_PERIPHERAL_PACKET_LENGTH);
        actual = hid_get_feature_report(dev, usb_buf, CORSAIR_PERIPHERAL_PACKET_LENGTH);
        offset = 1;
    }

    /*-----------------------------------------------------*\
    | Get device type                                       |
    |   0xC0    Device is a keyboard                        |
    |   0xC1    Device is a mouse                           |
    |   0xC2    Device is a mousepad or headset stand       |
    \*-----------------------------------------------------*/
    LOG_DEBUG("[%s] Device type %02X", CORSAIR_PERIPHERAL_CONTROLLER_NAME, usb_buf[0x14 + offset]);

    switch(usb_buf[0x14 + offset])
    {
        case 0xC0:
            {
                unsigned short pid = (unsigned short)(usb_buf[0x0E] << 8) + (unsigned short)(usb_buf[0x0F]);

                /*-----------------------------------------------------*\
                | Get the correct Keyboard Type                         |
                \*-----------------------------------------------------*/
                switch(pid)
                {
                    case 0x1B2D:
                    logical_layout = CORSAIR_TYPE_K95_PLAT;
                    break;

                    case 0x1B11:
                    logical_layout = CORSAIR_TYPE_K95;
                    break;

                    case 0x1B3D:
                    logical_layout = CORSAIR_TYPE_K55;
                    break;

                    case 0x1B38:
                    case 0x1B49:
                    case 0x1B6B:
                    case 0x1B55:
                    logical_layout = CORSAIR_TYPE_K70_MK2;
                    break;

                    case 0x1B4F:
                    logical_layout = CORSAIR_TYPE_K68;
                    break;

                    default:
                    logical_layout = CORSAIR_TYPE_NORMAL;
                }

                /*-----------------------------------------------------*\
                | Get the correct Keyboard Layout.                      |
                | Currently unused but can be implemented in the future.|
                \*-----------------------------------------------------*/
                switch(usb_buf[0x17 + offset])
                {
                    case CORSAIR_LAYOUT_ANSI:
                        physical_layout = CORSAIR_LAYOUT_ANSI;
                        break;
                    case CORSAIR_LAYOUT_ISO:
                        physical_layout = CORSAIR_LAYOUT_ISO;
                        break;
                    case CORSAIR_LAYOUT_ABNT:
                        physical_layout = CORSAIR_LAYOUT_ABNT;
                        break;
                    case CORSAIR_LAYOUT_JIS:
                        physical_layout = CORSAIR_LAYOUT_JIS;
                        break;
                    case CORSAIR_LAYOUT_DUBEOLSIK:
                        physical_layout = CORSAIR_LAYOUT_DUBEOLSIK;
                        break;
                    default:
                        physical_layout = CORSAIR_LAYOUT_ANSI;
                }

            }
            type = DEVICE_TYPE_KEYBOARD;
            break;

        case 0xC1:
            type = DEVICE_TYPE_MOUSE;
            SpecialFunctionControl();
            break;

        case 0xC2:
            {
                unsigned short pid = (unsigned short)(usb_buf[0x0F] << 8) + (unsigned short)(usb_buf[0x0E]);

                switch(pid)
                {
                    case 0x0A34:
                        type = DEVICE_TYPE_HEADSET_STAND;
                        SpecialFunctionControl();
                        break;

                    default:
                        type = DEVICE_TYPE_MOUSEMAT;
                        SpecialFunctionControl();
                        break;
                }
            }
            break;

        default:
            type = DEVICE_TYPE_UNKNOWN;
            break;
    }

    /*-----------------------------------------------------*\
    | Format firmware version string if device type is valid|
    \*-----------------------------------------------------*/
    if(type != DEVICE_TYPE_UNKNOWN)
    {
        firmware_version = std::to_string(usb_buf[0x09 + offset]) + "." + std::to_string(usb_buf[0x08 + offset]);
    }
}

void CorsairPeripheralController::StreamPacket
    (
    unsigned char   packet_id,
    unsigned char   data_sz,
    unsigned char*  data_ptr
    )
{
    unsigned char usb_buf[CORSAIR_PERIPHERAL_PACKET_LENGTH];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, CORSAIR_PERIPHERAL_PACKET_LENGTH);

    /*-----------------------------------------------------*\
    | Set up Stream packet                                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = CORSAIR_COMMAND_STREAM;
    usb_buf[0x02]   = packet_id;
    usb_buf[0x03]   = data_sz;

    /*-----------------------------------------------------*\
    | Copy in data bytes                                    |
    \*-----------------------------------------------------*/
    memcpy(&usb_buf[0x05], data_ptr, data_sz);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, CORSAIR_PERIPHERAL_PACKET_LENGTH);
}

void CorsairPeripheralController::SetHardwareMode
    (
        int mode_value,
        unsigned int color_mode,
        std::vector<RGBColor> colors,
        unsigned int speed,
        unsigned int direction,
        unsigned char brightness
    )
{
    unsigned char usb_buf[CORSAIR_PERIPHERAL_PACKET_LENGTH];
    memset(usb_buf, 0x00, CORSAIR_PERIPHERAL_PACKET_LENGTH);

    /*-----------------------------------------------------*\
    | Set the brightness                                    |
    \*-----------------------------------------------------*/
    usb_buf[1] = CORSAIR_COMMAND_WRITE;
    usb_buf[2] = 0x05;
    usb_buf[3] = 0x02;
    usb_buf[5] = brightness;

    hid_write(dev, usb_buf, CORSAIR_PERIPHERAL_PACKET_LENGTH);

    /*-----------------------------------------------------*\
    | Send "lght_00.d"                                      |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, CORSAIR_PERIPHERAL_PACKET_LENGTH);

    usb_buf[1]  = CORSAIR_COMMAND_WRITE;
    usb_buf[2]  = 0x17;
    usb_buf[3]  = 0x05;
    usb_buf[5]  = 0x6c;
    usb_buf[6]  = 0x67;
    usb_buf[7]  = 0x68;
    usb_buf[8]  = 0x74;
    usb_buf[9]  = 0x5F;
    usb_buf[10] = 0x30;
    usb_buf[11] = 0x30;
    usb_buf[12] = 0x2E;
    usb_buf[13] = 0x64;

    hid_write(dev, usb_buf, CORSAIR_PERIPHERAL_PACKET_LENGTH);

    /*-----------------------------------------------------*\
    | Stream the mode data                                  |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, CORSAIR_PERIPHERAL_PACKET_LENGTH);

    usb_buf[1] = CORSAIR_COMMAND_STREAM;
    usb_buf[2] = 0x01;
    usb_buf[3] = 0x0D;
    usb_buf[5] = mode_value;
    usb_buf[8] = direction;

    if(mode_value == CORSAIR_HW_MODE_TYPE_KEY_VALUE)
    {
        usb_buf[9] = speed;
    }
    else
    {
        usb_buf[6] = speed;
    }

    if(color_mode == MODE_COLORS_RANDOM)
    {
        usb_buf[7] = 0x01;
    }
    else if (color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        usb_buf[7] = 0x03;

        usb_buf[10] = RGBGetRValue(colors[0]);
        usb_buf[11] = RGBGetGValue(colors[0]);
        usb_buf[12] = RGBGetBValue(colors[0]);
        usb_buf[13] = 0xFF;

        usb_buf[14] = RGBGetRValue(colors[1]);
        usb_buf[15] = RGBGetGValue(colors[1]);
        usb_buf[16] = RGBGetBValue(colors[1]);
        usb_buf[17] = 0xFF;
    }

    hid_write(dev, usb_buf, CORSAIR_PERIPHERAL_PACKET_LENGTH);

    /*-----------------------------------------------------*\
    | Stop stream and commit                                |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, CORSAIR_PERIPHERAL_PACKET_LENGTH);

    usb_buf[1]  = CORSAIR_COMMAND_WRITE;
    usb_buf[2]  = 0x17;
    usb_buf[3]  = 0x09;

    hid_write(dev, usb_buf, CORSAIR_PERIPHERAL_PACKET_LENGTH);

    usb_buf[3]  = 0x08;

    hid_write(dev, usb_buf, CORSAIR_PERIPHERAL_PACKET_LENGTH);
}

void CorsairPeripheralController::SubmitKeyboardFullColors
    (
    unsigned char   color_channel,
    unsigned char   packet_count,
    unsigned char   finish_val
    )
{
    unsigned char usb_buf[CORSAIR_PERIPHERAL_PACKET_LENGTH];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, CORSAIR_PERIPHERAL_PACKET_LENGTH);

    /*-----------------------------------------------------*\
    | Set up Submit Keyboard 24-Bit Colors packet           |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = CORSAIR_COMMAND_WRITE;
    usb_buf[0x02]   = CORSAIR_PROPERTY_SUBMIT_KEYBOARD_COLOR_24;
    usb_buf[0x03]   = color_channel;
    usb_buf[0x04]   = packet_count;
    usb_buf[0x05]   = finish_val;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, CORSAIR_PERIPHERAL_PACKET_LENGTH);
}

void CorsairPeripheralController::SubmitKeyboardZonesColors
    (
    RGBColor left,
    RGBColor mid,
    RGBColor right
    )
{
    unsigned char usb_buf[CORSAIR_PERIPHERAL_PACKET_LENGTH];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Submit Keyboard 24-Bit Colors packet           |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = CORSAIR_COMMAND_WRITE;
    usb_buf[0x02]   = CORSAIR_PROPERTY_SUBMIT_KBZONES_COLOR_24;
    usb_buf[0x03]   = 0x00;
    usb_buf[0x04]   = 0x00;
    usb_buf[0x05]   = RGBGetRValue(left);
    usb_buf[0x06]   = RGBGetGValue(left);
    usb_buf[0x07]   = RGBGetBValue(left);
    usb_buf[0x08]   = RGBGetRValue(mid);
    usb_buf[0x09]   = RGBGetGValue(mid);
    usb_buf[0x0A]   = RGBGetBValue(mid);
    usb_buf[0x0B]   = RGBGetRValue(right);
    usb_buf[0x0C]   = RGBGetGValue(right);
    usb_buf[0x0D]   = RGBGetBValue(right);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, CORSAIR_PERIPHERAL_PACKET_LENGTH);
}

void CorsairPeripheralController::SubmitKeyboardLimitedColors
    (
    unsigned char   byte_count
    )
{
    unsigned char usb_buf[CORSAIR_PERIPHERAL_PACKET_LENGTH];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Submit Keyboard 9-Bit Colors packet            |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = CORSAIR_COMMAND_WRITE;
    usb_buf[0x02]   = CORSAIR_PROPERTY_SUBMIT_KEYBOARD_COLOR_9;
    usb_buf[0x05]   = byte_count;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, CORSAIR_PERIPHERAL_PACKET_LENGTH);
}

void CorsairPeripheralController::SubmitMouseColors
    (
    unsigned char   num_zones,
    RGBColor *      color_data
    )
{
    unsigned char usb_buf[CORSAIR_PERIPHERAL_PACKET_LENGTH];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, CORSAIR_PERIPHERAL_PACKET_LENGTH);

    /*-----------------------------------------------------*\
    | Set up Submit Mouse Colors packet                     |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = CORSAIR_COMMAND_WRITE;
    usb_buf[0x02]   = CORSAIR_PROPERTY_SUBMIT_MOUSE_COLOR;
    usb_buf[0x03]   = num_zones;
    usb_buf[0x04]   = 0x00;

    /*-----------------------------------------------------*\
    | Copy in colors in <ZONE> <RED> <GREEN> <BLUE> order   |
    \*-----------------------------------------------------*/
    for(unsigned int zone_idx = 0; zone_idx < num_zones; zone_idx++)
    {
        usb_buf[(zone_idx * 4) + 5] = zone_idx;
        usb_buf[(zone_idx * 4) + 6] = RGBGetRValue(color_data[zone_idx]);
        usb_buf[(zone_idx * 4) + 7] = RGBGetGValue(color_data[zone_idx]);
        usb_buf[(zone_idx * 4) + 8] = RGBGetBValue(color_data[zone_idx]);
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, CORSAIR_PERIPHERAL_PACKET_LENGTH);
}

void CorsairPeripheralController::SubmitMousematColors
    (
    unsigned char   num_zones,
    RGBColor *      color_data
    )
{
    unsigned char usb_buf[CORSAIR_PERIPHERAL_PACKET_LENGTH];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, CORSAIR_PERIPHERAL_PACKET_LENGTH);

    /*-----------------------------------------------------*\
    | Set up Submit Mouse Colors packet                     |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = CORSAIR_COMMAND_WRITE;
    usb_buf[0x02]   = CORSAIR_PROPERTY_SUBMIT_MOUSE_COLOR;
    usb_buf[0x03]   = num_zones;
    usb_buf[0x04]   = 0x00;

    /*-----------------------------------------------------*\
    | Copy in colors in <RED> <GREEN> <BLUE> order          |
    \*-----------------------------------------------------*/
    for(unsigned int zone_idx = 0; zone_idx < num_zones; zone_idx++)
    {
        usb_buf[(zone_idx * 3) + 5] = RGBGetRValue(color_data[zone_idx]);
        usb_buf[(zone_idx * 3) + 6] = RGBGetGValue(color_data[zone_idx]);
        usb_buf[(zone_idx * 3) + 7] = RGBGetBValue(color_data[zone_idx]);
    }

    /*-----------------------------------------------------*\
    | Send packet using feature reports, as headset stand   |
    | seems to not update completely using HID writes       |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, CORSAIR_PERIPHERAL_PACKET_LENGTH);
}
