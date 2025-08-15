/*---------------------------------------------------------*\
| OKSKeyboardController.h                                   |
|                                                           |
|   Driver for OKS keyboard                                 |
|                                                           |
|   Merafour (OKS)                              24 Feb 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <stdint.h>
#include <string>
#include <hidapi.h>
#include "RGBController.h"

/*-----------------------------------------------------*\
| OKS vendor ID                                         |
\*-----------------------------------------------------*/
#define OKS_VID                                 0x1C4F

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define OKS_OPTICAL_RGB_PID                     0xEE88

/*-----------------------------------------------------*\
| Communication protocol                                |
\*-----------------------------------------------------*/
#define KB2_PACK_HEAD       (0x5C&0xFF)
#define KB2_HEAD_SIZE       4
#define KB2_PORT_SIZE       (64*4)
#define KB2_DATA_SIZE       (KB2_PORT_SIZE-KB2_HEAD_SIZE)
union kb2_port_t
{
    uint8_t bin[KB2_PORT_SIZE];
    struct
    {
        uint8_t head;
        uint8_t length;
        uint8_t cmd;
        uint8_t checksum;
        uint8_t data[KB2_DATA_SIZE];
    };
};
enum kb2_cmd
{
    KB2_CMD_RRGB      = 0x14,
    KB2_CMD_WRGB      = 0x15,
    KB2_CMD_RLED      = 0x16,
    KB2_CMD_WLED      = 0x17,
};
union uint32_kb2
{
    uint32_t data;
    struct
    {
        uint8_t Byte0;
        uint8_t Byte1;
        uint8_t Byte2;
        uint8_t Byte3;
    };
};

class OKSKeyboardController
{
public:
    OKSKeyboardController(hid_device* dev_handle, const char* path, const unsigned short pid, std::string dev_name);
    ~OKSKeyboardController();

    std::string     GetDeviceLocation();
    std::string     GetNameString();
    std::string     GetSerialString();
    unsigned short  GetUSBPID();

    void            SendColors(unsigned char* color_data, unsigned int color_data_size);
    void            SendKeyboardModeEx(const mode &m, unsigned char red, unsigned char green, unsigned char blue);

private:
    hid_device*     dev;
    std::string     location;
    std::string     name;
    unsigned short  usb_pid;

    void            Send(const uint8_t bin[64], const uint16_t len);
    void            SendInitialize();
    uint8_t         kb2_ComputeChecksum(const union kb2_port_t* const Pack);
    int             kb2_add_32b(union kb2_port_t* const Pack, const uint32_t value);
    void            kb2M_init(union kb2_port_t* const Pack, const enum kb2_cmd cmd);
    void            kb2M_wrgb(union kb2_port_t* const Pack, const uint8_t bright, const uint8_t mode, const uint8_t speed, const uint8_t dir);
    void            kb2M_wled(union kb2_port_t* const Pack, const uint32_t irgb[14]);
};
