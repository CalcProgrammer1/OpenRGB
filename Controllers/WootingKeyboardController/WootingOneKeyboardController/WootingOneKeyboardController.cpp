/*---------------------------------------------------------*\
| WootingOneKeyboardController.cpp                          |
|                                                           |
|   Driver for Wooting One keyboard                         |
|                                                           |
|   Diogo Trindade (diogotr7)                   04 Mar 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "StringUtils.h"
#include "WootingOneKeyboardController.h"

#undef  WOOTING_CONTROLLER_NAME
#define WOOTING_CONTROLLER_NAME         "[WootingONE] "

//0xFFFFFFFF indicates an unused entry in matrix
#define NA 0xFFFFFFFF

static const unsigned int rgb_led_index[WOOTING_RGB_ROWS][WOOTING_RGB_COLUMNS] =
{
    {   0,  NA,  11,  12,  23,  24,  36,  47,  85,  84,  49,  48,  59,  61,  73,  81,  80, 113, 114, 115, 116 },
    {   2,   1,  14,  13,  26,  25,  35,  38,  37,  87,  86,  95,  51,  63,  75,  72,  74,  96,  97,  98,  99 },
    {   3,   4,  15,  16,  27,  28,  39,  42,  40,  88,  89,  52,  53,  71,  76,  83,  77, 102, 103, 104, 100 },
    {   5,   6,  17,  18,  29,  30,  41,  46,  44,  90,  93,  54,  57,  65,  NA,  NA,  NA, 105, 106, 107,  NA },
    {   9,   8,  19,  20,  31,  34,  32,  45,  43,  91,  92,  55,  NA,  66,  NA,  78,  NA, 108, 109, 110, 101 },
    {  10,  22,  21,  NA,  NA,  NA,  33,  NA,  NA,  NA,  94,  58,  67,  68,  70,  79,  82,  NA, 111, 112,  NA }
};

static uint16_t getCrc16ccitt(const uint8_t* buffer, uint16_t size)
{
    uint16_t crc = 0;

    while(size--)
    {
        crc ^= (*buffer++ << 8);

        for(uint8_t i = 0; i < 8; ++i)
        {
            if(crc & 0x8000)
            {
                crc = (crc << 1) ^ 0x1021;
            }
            else
            {
                crc = crc << 1;
            }
        }
    }

    return crc;
}

WootingOneKeyboardController::WootingOneKeyboardController(hid_device* dev_handle, const char *path, uint8_t wooting_type, std::string dev_name)
{
    dev                 = dev_handle;
    location            = path;
    name                = dev_name;
    this->wooting_type  = wooting_type;
    key_code_limit      = (wooting_type == WOOTING_KB_TKL) ? WOOTING_ONE_KEY_CODE_LIMIT : WOOTING_TWO_KEY_CODE_LIMIT;

    /*---------------------------------------------------------*\
    | Get device HID manufacturer and product strings           |
    \*---------------------------------------------------------*/
    const int szTemp = 256;
    wchar_t tmpName[szTemp];

    hid_get_manufacturer_string(dev, tmpName, szTemp);
    vendor = std::string(StringUtils::wstring_to_string(tmpName));

    hid_get_product_string(dev, tmpName, szTemp);
    description = std::string(StringUtils::wstring_to_string(tmpName));

    SendInitialize();
}

WootingOneKeyboardController::~WootingOneKeyboardController()
{

}

void WootingOneKeyboardController::SendDirect(RGBColor* colors, uint8_t colour_count)
{
    const uint8_t pwm_mem_map[48] =
    {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,
        0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D,
        0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D
    };

    unsigned char buffer0[RGB_RAW_BUFFER_SIZE] = {0};
    unsigned char buffer1[RGB_RAW_BUFFER_SIZE] = {0};
    unsigned char buffer2[RGB_RAW_BUFFER_SIZE] = {0};
    unsigned char buffer3[RGB_RAW_BUFFER_SIZE] = {0};
    unsigned char buffer4[RGB_RAW_BUFFER_SIZE] = {0};

    for(std::size_t color_idx = 0; color_idx < colour_count; color_idx++)
    {
        unsigned char led_index = rgb_led_index[color_idx % 6][color_idx / 6];

        if(led_index > key_code_limit)
        {
            continue;
        }

        unsigned char *buffer_pointer = buffer0;

        if(led_index >= 96)
        {
            buffer_pointer = buffer4;
        }
        else if(led_index >= 72)
        {
            buffer_pointer = buffer3;
        }
        else if(led_index >= 48)
        {
            buffer_pointer = buffer2;
        }
        else if(led_index >= 24)
        {
            buffer_pointer = buffer1;
        }
        else
        {
            buffer_pointer = buffer0;
        }

        unsigned char buffer_index          = pwm_mem_map[led_index % 24];
        buffer_pointer[buffer_index + 0x00] = RGBGetRValue(colors[color_idx]);
        buffer_pointer[buffer_index + 0x10] = RGBGetGValue(colors[color_idx]);
        buffer_pointer[buffer_index + 0x20] = RGBGetBValue(colors[color_idx]);
    }

    wooting_usb_send_buffer(RGB_PARTS::PART0, buffer0);
    wooting_usb_send_buffer(RGB_PARTS::PART1, buffer1);
    wooting_usb_send_buffer(RGB_PARTS::PART2, buffer2);
    wooting_usb_send_buffer(RGB_PARTS::PART3, buffer3);
    if(key_code_limit > WOOTING_ONE_KEY_CODE_LIMIT)
    {
        wooting_usb_send_buffer(RGB_PARTS::PART4, buffer4);
    }
}

void WootingOneKeyboardController::SendInitialize()
{
    wooting_usb_send_feature(WOOTING_COLOR_INIT_COMMAND, 0,0,0,0);

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

bool WootingOneKeyboardController::wooting_usb_send_buffer(RGB_PARTS part_number, uint8_t* rgb_buffer)
{
    unsigned char report_buffer[WOOTING_REPORT_SIZE] = {0};

    /*---------------------------------------------------------*\
    | Set up the Send Buffer packet                             |
    \*---------------------------------------------------------*/
    report_buffer[0] = 0;                           // HID report index (unused)
    report_buffer[1] = 0xD0;                        // Magic word
    report_buffer[2] = 0xDA;                        // Magic word
    report_buffer[3] = WOOTING_RAW_COLORS_REPORT;   // Report ID

    switch(part_number)
    {
        case PART0:
            report_buffer[4] = 0;                   // Slave nr
            report_buffer[5] = 0;                   // Reg start address
            break;

        case PART1:
            report_buffer[4] = 0;                   // Slave nr
            report_buffer[5] = RGB_RAW_BUFFER_SIZE; // Reg start address
            break;

        case PART2:
            report_buffer[4] = 1;                   // Slave nr
            report_buffer[5] = 0;                   // Reg start address
            break;

        case PART3:
            report_buffer[4] = 1;                   // Slave nr
            report_buffer[5] = RGB_RAW_BUFFER_SIZE; // Reg start address
            break;

        case PART4:
            report_buffer[4] = 2;                   // Slave nr
            report_buffer[5] = 0;                   // Reg start address
            break;

        default:
            return false;
            break;
    }

    /*---------------------------------------------------------*\
    | Copy in the buffer data                                   |
    \*---------------------------------------------------------*/
    memcpy(&report_buffer[6], rgb_buffer, RGB_RAW_BUFFER_SIZE);

    /*---------------------------------------------------------*\
    | Calculate the CRC and append it to the packet             |
    \*---------------------------------------------------------*/
    unsigned short crc = getCrc16ccitt((unsigned char*)&report_buffer, WOOTING_REPORT_SIZE - 2);
    report_buffer[127] = (unsigned char)crc;
    report_buffer[128] = crc >> 8;

    /*---------------------------------------------------------*\
    | Send packet                                               |
    \*---------------------------------------------------------*/
    hid_write(dev, report_buffer, WOOTING_REPORT_SIZE);

    return true;
}
