/*---------------------------------------------------------*\
| OKSKeyboardController.cpp                                 |
|                                                           |
|   Driver for OKS keyboard                                 |
|                                                           |
|   Merafour (OKS)                              24 Feb 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "OKSKeyboardController.h"
#include "StringUtils.h"

OKSKeyboardController::OKSKeyboardController(hid_device* dev_handle, const char* path, const unsigned short pid, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
    usb_pid     = pid;

    SendInitialize();
}

OKSKeyboardController::~OKSKeyboardController()
{
    hid_close(dev);
}

std::string OKSKeyboardController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string OKSKeyboardController::GetNameString()
{
    return(name);
}

std::string OKSKeyboardController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

unsigned short OKSKeyboardController::GetUSBPID()
{
    return(usb_pid);
}

void OKSKeyboardController::SendColors(unsigned char* color_data, unsigned int /*color_data_size*/)
{
    char usb_buf[65];
    union kb2_port_t Pack;
    uint8_t cnt;
    uint8_t red,green,blue;
    uint16_t color_idx;
    uint32_t irgb[14];
    uint16_t pos=0;
    for(color_idx=0; color_idx<(6*21); color_idx += 14)
    {
        for(cnt=0; cnt<14; cnt++)
        {
            pos = color_idx+cnt;
            red   = color_data[pos*3+0];
            green = color_data[pos*3+1];
            blue  = color_data[pos*3+2];
            irgb[cnt] = blue&0xFF;
            irgb[cnt] <<= 8;
            irgb[cnt] |= green&0xFF;
            irgb[cnt] <<= 8;
            irgb[cnt] |= red&0xFF;
            irgb[cnt] <<= 8;
            irgb[cnt] |= pos&0xFF;
        }
        kb2M_wled(&Pack, irgb);
        usb_buf[0] = 0x04;
        for(uint8_t i=0; i<64; i++) usb_buf[i+1] = Pack.bin[i];
        /*-----------------------------------------------------*\
        | Send packet                                           |
        \*-----------------------------------------------------*/
        hid_write(dev, (unsigned char *)usb_buf, 65);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
}

void OKSKeyboardController::SendKeyboardModeEx(const mode &m, unsigned char /*red*/, unsigned char /*green*/, unsigned char /*blue*/)
{
    union kb2_port_t Pack;
    kb2M_wrgb(&Pack, m.brightness, m.value, m.speed, m.direction);
    Send(Pack.bin, Pack.length+KB2_HEAD_SIZE);
}

void OKSKeyboardController::Send(const uint8_t bin[], const uint16_t len)
{
    char usb_buf[65];
    uint16_t Len;
    uint16_t pos;
    pos=0;
    while(pos<len)
    {
        /*-----------------------------------------------------*\
        | Zero out buffer                                       |
        \*-----------------------------------------------------*/
        memset(usb_buf, 0x00, sizeof(usb_buf));
        /*-----------------------------------------------------*\
        | Set send data packet                                  |
        \*-----------------------------------------------------*/
        usb_buf[0] = 0x04;
        Len = len-pos;
        if(Len>64) Len=64;
        for(uint8_t i=0; i<Len; i++) usb_buf[i+1] = bin[i+pos];
        pos += Len;
        /*-----------------------------------------------------*\
        | Send packet                                           |
        \*-----------------------------------------------------*/
        hid_write(dev, (unsigned char *)usb_buf, 65);
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}

void OKSKeyboardController::SendInitialize()
{
    union kb2_port_t Pack;
    kb2M_wrgb(&Pack, 0, 0, 2, 1);
    Send(Pack.bin, Pack.length+KB2_HEAD_SIZE);
}

uint8_t OKSKeyboardController::kb2_ComputeChecksum(const union kb2_port_t* const Pack)
{
    uint8_t checksum = 0x35;
    const uint8_t len = Pack->length;

    checksum += Pack->head;
    checksum += len;
    checksum += Pack->cmd;
    if((len>0) && (len<=KB2_DATA_SIZE)) checksum += Pack->data[len-1];
    return checksum;
}
int OKSKeyboardController::kb2_add_32b(union kb2_port_t* const Pack, const uint32_t value)
{
    union uint32_kb2 Value;
    if((Pack->length+4)>KB2_DATA_SIZE) return -1;
    Value.data = value;
    Pack->data[Pack->length++] = Value.Byte0;
    Pack->data[Pack->length++] = Value.Byte1;
    Pack->data[Pack->length++] = Value.Byte2;
    Pack->data[Pack->length++] = Value.Byte3;
    return 4;
}
void OKSKeyboardController::kb2M_init(union kb2_port_t* const Pack, const enum kb2_cmd cmd)
{
    Pack->head = KB2_PACK_HEAD;
    Pack->length = 0;
    Pack->cmd = cmd;
    Pack->checksum = 0;
}
void OKSKeyboardController::kb2M_wrgb(union kb2_port_t* const Pack, const uint8_t bright, const uint8_t mode, const uint8_t speed, const uint8_t dir)
{
    kb2M_init(Pack, KB2_CMD_WRGB);
    kb2_add_32b(Pack, bright);
    Pack->data[Pack->length++] = mode;
    Pack->data[Pack->length++] = speed;
    Pack->data[Pack->length++] = dir;
    Pack->data[Pack->length++] = 0xFF;
    Pack->data[Pack->length++] = 0xFF;
    Pack->checksum = kb2_ComputeChecksum(Pack);
}
void OKSKeyboardController::kb2M_wled(union kb2_port_t* const Pack, const uint32_t irgb[14])
{
    kb2M_init(Pack, KB2_CMD_WLED);
    for(uint8_t i=0; i<14; i++) kb2_add_32b(Pack, irgb[i]);
    Pack->data[Pack->length++] = 0xFF;
    Pack->data[Pack->length++] = 0xFF;
    Pack->checksum = kb2_ComputeChecksum(Pack);
}
