/*---------------------------------------------------------*\
| MountainKeyboardController.cpp                            |
|                                                           |
|   Driver for Mountain keyboard                            |
|                                                           |
|   Wojciech Lazarski                              Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <chrono>
#include <cstring>
#include <thread>
#include "MountainKeyboardController.h"
#include "StringUtils.h"

using namespace std::chrono_literals;

MountainKeyboardController::MountainKeyboardController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

MountainKeyboardController::~MountainKeyboardController()
{
    hid_close(dev);
}

std::string MountainKeyboardController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string MountainKeyboardController::GetNameString()
{
    return(name);
}

std::string MountainKeyboardController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void MountainKeyboardController::SelectMode(unsigned char mode_idx)
{
    unsigned char usb_buf[MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE];

    memset(usb_buf, 0x00, MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE);
    usb_buf[0x01] = MOUNTAIN_KEYBOARD_SEND_CMD;
    usb_buf[0x02] = MOUNTAIN_KEYBOARD_SELECT_MODE_CMD;
    usb_buf[0x05] = 0x01; //constant data
    usb_buf[0x06] = mode_idx;
    hid_write(dev, usb_buf, MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE);
    std::this_thread::sleep_for(200ms);
}

void MountainKeyboardController::SaveData(unsigned char mode_idx)
{
    unsigned char usb_buf[MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE];

    memset(usb_buf, 0x00, MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE);
    usb_buf[0x01] = MOUNTAIN_KEYBOARD_SAVE_CMD;
    usb_buf[0x02] = MOUNTAIN_KEYBOARD_SAVE_MAGIC1;
    usb_buf[0x05] = mode_idx;
    hid_write(dev, usb_buf, MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE);
    std::this_thread::sleep_for(200ms);
}

void MountainKeyboardController::SendOffCmd()
{
    unsigned char usb_buf[MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE];

    memset(usb_buf, 0x00, MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE);
    usb_buf[0x01] = MOUNTAIN_KEYBOARD_SEND_CMD;
    usb_buf[0x02] = MOUNTAIN_KEYBOARD_SEND_COLOR_DATA_CMD;
    usb_buf[0x03] = MOUNTAIN_KEYBOARD_OFF_MSG;

    usb_buf[0x05] = 0xFF; //constant data
    usb_buf[0x06] = 0x64; // constant data

    usb_buf[0x07] = 0xFF; // constant data
    usb_buf[0x08] = 0xFF; // constant data
    usb_buf[0x09] = 0xFF; // constant data

    hid_write(dev, usb_buf, MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE);
    std::this_thread::sleep_for(10ms);
}


void MountainKeyboardController::SendColorStaticCmd(color_setup setup)
{
    unsigned char usb_buf[MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE];

    memset(usb_buf, 0x00, MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE);
    usb_buf[0x01] = MOUNTAIN_KEYBOARD_SEND_CMD;
    usb_buf[0x02] = MOUNTAIN_KEYBOARD_SEND_COLOR_DATA_CMD;
    usb_buf[0x03] = MOUNTAIN_KEYBOARD_STATIC_MSG;
    usb_buf[0x05] = 0xFF; // constant data
    usb_buf[0x06] = setup.brightness;

    usb_buf[0x07] = 0x00; // constant data
    usb_buf[0x08] = 0xFF; // constant data
    usb_buf[0x09] = 0xFF; // constant data

    usb_buf[0x0A] = setup.mode.one_color.r;
    usb_buf[0x0B] = setup.mode.one_color.g;
    usb_buf[0x0C] = setup.mode.one_color.b;

    hid_write(dev, usb_buf, MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE);
    std::this_thread::sleep_for(10ms);
}

void MountainKeyboardController::SendColorWaveCmd(color_setup setup)
{
    unsigned char usb_buf[MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE];

    memset(usb_buf, 0x00, MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE);
    usb_buf[0x01] = MOUNTAIN_KEYBOARD_SEND_CMD;
    usb_buf[0x02] = MOUNTAIN_KEYBOARD_SEND_COLOR_DATA_CMD;
    usb_buf[0x03] = MOUNTAIN_KEYBOARD_COLOR_WAVE_MSG;

    usb_buf[0x05] = setup.speed;
    usb_buf[0x06] = setup.brightness;
    usb_buf[0x08] = setup.direction;

    switch(setup.color_mode)
    {
        case MOUNTAIN_KEYBOARD_COLOR_MODE_DUAL:
        {
            usb_buf[0x07] = 0x00; // constant data
            usb_buf[0x09] = 0x02; // constant data

            usb_buf[0x0A] = 0x04; // constant data
            usb_buf[0x0B] = 0x19; // constant data

            usb_buf[0x0C] = setup.mode.two_colors.r1;
            usb_buf[0x0D] = setup.mode.two_colors.g1;
            usb_buf[0x0E] = setup.mode.two_colors.b1;
            usb_buf[0x0F] = 0x32; // constant data
            usb_buf[0x10] = setup.mode.two_colors.r2;
            usb_buf[0x11] = setup.mode.two_colors.g2;
            usb_buf[0x12] = setup.mode.two_colors.b2;
            usb_buf[0x13] = 0x4B; // constant data
            usb_buf[0x14] = setup.mode.two_colors.r1;
            usb_buf[0x15] = setup.mode.two_colors.g1;
            usb_buf[0x16] = setup.mode.two_colors.b1;
            usb_buf[0x17] = 0x64; // constant data
            usb_buf[0x18] = setup.mode.two_colors.r2;
            usb_buf[0x19] = setup.mode.two_colors.g2;
            usb_buf[0x1A] = setup.mode.two_colors.b2;
        }
        break;

        case MOUNTAIN_KEYBOARD_COLOR_MODE_SINGLE:
        {
            usb_buf[0x07] = 0x00; // constant data
            usb_buf[0x09] = 0x00; // constant data

            usb_buf[0x0A] = 0x01; // constant data
            usb_buf[0x0B] = 0x64; // constant data

            usb_buf[0x0C] = setup.mode.one_color.r;
            usb_buf[0x0D] = setup.mode.one_color.g;
            usb_buf[0x0E] = setup.mode.one_color.b;
            usb_buf[0x0F] = 0xFF; // constant data
        }
        break;

        case MOUNTAIN_KEYBOARD_COLOR_MODE_RAINBOW:
        {
            usb_buf[0x07] = 0x02; // constant data
            usb_buf[0x09] = 0x02; // constant data

            usb_buf[0x0B] = 0xFF; // constant data
            usb_buf[0x0F] = 0xFF; // constant data
        }
        break;

        default:
        break;
    }

    hid_write(dev, usb_buf, MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE);
    std::this_thread::sleep_for(10ms);
}

void MountainKeyboardController::SendColorTornadoCmd(color_setup setup)
{
    unsigned char usb_buf[MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE];

    memset(usb_buf, 0x00, MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE);
    usb_buf[0x01] = MOUNTAIN_KEYBOARD_SEND_CMD;
    usb_buf[0x02] = MOUNTAIN_KEYBOARD_SEND_COLOR_DATA_CMD;
    usb_buf[0x03] = MOUNTAIN_KEYBOARD_TORNADO_MSG;

    usb_buf[0x05] = setup.speed;
    usb_buf[0x06] = setup.brightness;
    usb_buf[0x08] = setup.direction;

    switch(setup.color_mode)
    {
        case MOUNTAIN_KEYBOARD_COLOR_MODE_SINGLE:
        {
            usb_buf[0x07] = 0x00; // constant data
            usb_buf[0x09] = 0x00; // constant data

            usb_buf[0x0A] = 0x01; // constant data
            usb_buf[0x0B] = 0x64; // constant data

            usb_buf[0x0C] = setup.mode.one_color.r;
            usb_buf[0x0D] = setup.mode.one_color.g;
            usb_buf[0x0E] = setup.mode.one_color.b;
            usb_buf[0x0F] = 0xFF; // constant data
        }
        break;

        case MOUNTAIN_KEYBOARD_COLOR_MODE_RAINBOW:
        {
            usb_buf[0x07] = 0x02; // constant data
            usb_buf[0x09] = 0x02; // constant data

            usb_buf[0x0B] = 0xFF; // constant data
            usb_buf[0x0F] = 0xFF; // constant data
        }
        break;

        default:
        break;
    }
    hid_write(dev, usb_buf, MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE);
    std::this_thread::sleep_for(10ms);
}

void MountainKeyboardController::SendColorBreathingCmd( color_setup setup)
{
    unsigned char usb_buf[MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE];

    memset(usb_buf, 0x00, MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE);
    usb_buf[0x01] = MOUNTAIN_KEYBOARD_SEND_CMD;
    usb_buf[0x02] = MOUNTAIN_KEYBOARD_SEND_COLOR_DATA_CMD;
    usb_buf[0x03] = MOUNTAIN_KEYBOARD_BREATHING_MSG;
    usb_buf[0x05] = setup.speed;
    usb_buf[0x06] = setup.brightness;

    switch(setup.color_mode)
    {
        case MOUNTAIN_KEYBOARD_COLOR_MODE_DUAL:
        {
            usb_buf[0x07] = 0x10; // constant data
            usb_buf[0x08] = 0xFF; // constant data
            usb_buf[0x09] = 0xFF; // constant data

            usb_buf[0x0A] = setup.mode.two_colors.r1;
            usb_buf[0x0B] = setup.mode.two_colors.g1;
            usb_buf[0x0C] = setup.mode.two_colors.b1;
            usb_buf[0x0D] = setup.mode.two_colors.r2;
            usb_buf[0x0E] = setup.mode.two_colors.g2;
            usb_buf[0x0F] = setup.mode.two_colors.b2;
        }
        break;

        case MOUNTAIN_KEYBOARD_COLOR_MODE_SINGLE:
        {
            usb_buf[0x07] = 0x00; // constant data
            usb_buf[0x08] = 0xFF; // constant data
            usb_buf[0x09] = 0xFF; // constant data
            usb_buf[0x0A] = setup.mode.one_color.r;
            usb_buf[0x0B] = setup.mode.one_color.g;
            usb_buf[0x0C] = setup.mode.one_color.b;
        }
        break;

        case MOUNTAIN_KEYBOARD_COLOR_MODE_RAINBOW:
        {
            usb_buf[0x07] = 0x02; // constant data
            usb_buf[0x08] = 0xFF; // constant data
            usb_buf[0x09] = 0xFF; // constant data
        }
        break;

        default:
        break;
    }
    hid_write(dev, usb_buf, MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE);
    std::this_thread::sleep_for(10ms);
}

void MountainKeyboardController::SendColorMatrixCmd(color_setup setup)
{
    unsigned char usb_buf[MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE];

    memset(usb_buf, 0x00, MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE);
    usb_buf[0x01] = MOUNTAIN_KEYBOARD_SEND_CMD;
    usb_buf[0x02] = MOUNTAIN_KEYBOARD_SEND_COLOR_DATA_CMD;
    usb_buf[0x03] = MOUNTAIN_KEYBOARD_MATRIX_MSG;
    usb_buf[0x05] = setup.speed;
    usb_buf[0x06] = setup.brightness;

    usb_buf[0x07] = 0x00; // constant data
    usb_buf[0x08] = 0xFF; // constant data
    usb_buf[0x09] = 0xFF; // constant data

    usb_buf[0x0A] = setup.mode.two_colors.r1;
    usb_buf[0x0B] = setup.mode.two_colors.g1;
    usb_buf[0x0C] = setup.mode.two_colors.b1;

    usb_buf[0x13] = setup.mode.two_colors.r2;
    usb_buf[0x14] = setup.mode.two_colors.g2;
    usb_buf[0x15] = setup.mode.two_colors.b2;

    hid_write(dev, usb_buf, MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE);
    std::this_thread::sleep_for(10ms);
}

void MountainKeyboardController::SendColorReactiveCmd(color_setup setup)
{
    unsigned char usb_buf[MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE];

    memset(usb_buf, 0x00, MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE);
    usb_buf[0x01] = MOUNTAIN_KEYBOARD_SEND_CMD;
    usb_buf[0x02] = MOUNTAIN_KEYBOARD_SEND_COLOR_DATA_CMD;
    usb_buf[0x03] = MOUNTAIN_KEYBOARD_REACTIVE_MSG;
    usb_buf[0x05] = setup.speed;
    usb_buf[0x06] = setup.brightness;

    usb_buf[0x07] = 0x00; // constant data
    usb_buf[0x08] = 0xFF; // constant data
    usb_buf[0x09] = 0xFF; // constant data

    usb_buf[0x0A] = setup.mode.two_colors.r1;
    usb_buf[0x0B] = setup.mode.two_colors.g1;
    usb_buf[0x0C] = setup.mode.two_colors.b1;

    usb_buf[0x13] = setup.mode.two_colors.r2;
    usb_buf[0x14] = setup.mode.two_colors.g2;
    usb_buf[0x15] = setup.mode.two_colors.b2;

    hid_write(dev, usb_buf, MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE);
    std::this_thread::sleep_for(10ms);
}

void MountainKeyboardController::SendColorStartPacketCmd(unsigned char brightness)
{
    unsigned char usb_buf[MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE];

    memset(usb_buf, 0xFF, MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE);
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = MOUNTAIN_KEYBOARD_SEND_CMD;
    usb_buf[0x02] = MOUNTAIN_KEYBOARD_SEND_COLOR_DATA_CMD;
    usb_buf[0x03] = MOUNTAIN_KEYBOARD_CUSTOM_MSG; // constant data
    usb_buf[0x04] = 0x00; // constant data
    usb_buf[0x06] = brightness;
    hid_write(dev, usb_buf, MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE);
    std::this_thread::sleep_for(10ms);
}

void MountainKeyboardController::SendColorPacketCmd(unsigned char pkt_no,unsigned char brightness, unsigned char *data, unsigned int data_size)
{
    unsigned char usb_buf[MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE];

    memset(usb_buf, 0x00, MOUNTAIN_KEYBOARD_USB_BUFFER_HEADER_SIZE);
    usb_buf[0x01] = MOUNTAIN_KEYBOARD_SEND_CMD;
    usb_buf[0x02] = MOUNTAIN_KEYBOARD_SEND_COLOR_DATA_CMD;
    usb_buf[0x04] = 0x01; // constant data
    usb_buf[0x05] = pkt_no;
    usb_buf[0x06] = brightness;

    if(data_size <= MOUNTAIN_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE)
    {
        memcpy(&usb_buf[MOUNTAIN_KEYBOARD_USB_BUFFER_HEADER_SIZE],data,data_size);
        if(data_size < MOUNTAIN_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE)
        {
            memset(&usb_buf[MOUNTAIN_KEYBOARD_USB_BUFFER_HEADER_SIZE + data_size],0x00,MOUNTAIN_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE-data_size);
        }
        hid_write(dev, usb_buf, MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE);
        std::this_thread::sleep_for(5ms);
    }
}

void MountainKeyboardController::SendColorEdgePacketCmd(unsigned char pkt_no, unsigned char *data, unsigned int data_size)
{
    unsigned char usb_buf[MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE];

    memset(usb_buf, 0x00, MOUNTAIN_KEYBOARD_USB_BUFFER_HEADER_SIZE);
    usb_buf[0x01] = MOUNTAIN_KEYBOARD_SEND_CMD;
    usb_buf[0x02] = MOUNTAIN_KEYBOARD_SEND_COLOR_EDGE_CMD;
    usb_buf[0x03] = 0x0A; // constant data
    usb_buf[0x05] = pkt_no;
    usb_buf[0x06] = 0xFF;

    if(data_size <= MOUNTAIN_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE)
    {
        memcpy(&usb_buf[MOUNTAIN_KEYBOARD_USB_BUFFER_HEADER_SIZE],data,data_size);
        if(data_size < MOUNTAIN_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE)
        {
            memset(&usb_buf[MOUNTAIN_KEYBOARD_USB_BUFFER_HEADER_SIZE + data_size],0x00,MOUNTAIN_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE-data_size);
        }
        hid_write(dev, usb_buf, MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE);
        std::this_thread::sleep_for(5ms);
    }
}

void MountainKeyboardController::SendColorPacketFinishCmd()
{
    unsigned char usb_buf[MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE];

    memset(usb_buf, 0x00, MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE);
    usb_buf[0x01] = MOUNTAIN_KEYBOARD_SEND_CMD;
    usb_buf[0x02] = MOUNTAIN_KEYBOARD_CONFIRM_CMD;

    for(unsigned char i=0;i<3;i++)
    {
        usb_buf[0x03] = i;
        hid_write(dev, usb_buf, MOUNTAIN_KEYBOARD_USB_BUFFER_SIZE);
        std::this_thread::sleep_for(10ms);
    }
}

void MountainKeyboardController::SendWheelColorChange(unsigned char color_data [3])
{

    wheel_config * usb_buf = GetWheelConfig();
    if (usb_buf != nullptr)
    {
        usb_buf->fixed_byte_1 = 0x01;
        usb_buf->fixed_byte_2 = 0x02;
        usb_buf->zero_byte    = 0x00;
        usb_buf->r = color_data[0];
        usb_buf->g = color_data[1];
        usb_buf->b = color_data[2];
        hid_write(dev, (unsigned char *) usb_buf, MOUNTAIN_KEYBOARD_WHEEL_CONFIG_BUFFER_SIZE);
    }

}

wheel_config * MountainKeyboardController::GetWheelConfig()
{

    unsigned char * usb_buf = new unsigned char [MOUNTAIN_KEYBOARD_WHEEL_CONFIG_BUFFER_SIZE+1];
    unsigned char * recv_buf = &usb_buf[1];


    for (int i =0; i < 1000; i++)
    {

        memset(usb_buf, 0x00, MOUNTAIN_KEYBOARD_WHEEL_CONFIG_BUFFER_SIZE+1);
        wheel_config * usb_conf = (wheel_config *) usb_buf;
        usb_conf->config_start_first = 0x11;
        usb_conf->config_start_second = 0x14;
        hid_write(dev, usb_buf, MOUNTAIN_KEYBOARD_WHEEL_CONFIG_BUFFER_SIZE);
        memset(usb_buf, 0x00, MOUNTAIN_KEYBOARD_WHEEL_CONFIG_BUFFER_SIZE+1);
        hid_read_timeout(dev, recv_buf, MOUNTAIN_KEYBOARD_WHEEL_CONFIG_BUFFER_SIZE, 1);
        if (usb_conf->config_start_first == MOUNTAIN_KEYBOARD_WHEEL_CONFIG_FIRST_BYTE &&
            usb_conf->config_start_second == MOUNTAIN_KEYBOARD_WHEEL_CONFIG_SECOND_BYTE)
        {
            return usb_conf;
        }

    }

    delete[] usb_buf;
    return nullptr;

}


void MountainKeyboardController::SendDirectColorEdgeCmd(bool quick_mode, unsigned char brightness, unsigned char *color_data, unsigned int data_size)
{
    static bool first_call = true;
    static unsigned char prv_buffer[MOUNTAIN_KEYBOARD_TRANSFER_EDGE_BUFFER_SIZE] = {0};

    unsigned char pkt_no        = 0;
    unsigned char *data_ptr     = color_data;
    unsigned char *prv_data_ptr = prv_buffer;
    unsigned int  data_len      = data_size;

    if(MOUNTAIN_KEYBOARD_TRANSFER_EDGE_BUFFER_SIZE >= data_len)
    {
        if(!quick_mode)
        {
            SendColorStartPacketCmd(brightness);
        }
        while(data_len>0)
        {
            if(data_len >= MOUNTAIN_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE)
            {
                if(first_call || !quick_mode || memcmp(data_ptr,prv_data_ptr,MOUNTAIN_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE))
                {
                    MountainKeyboardController::SendColorEdgePacketCmd(pkt_no,data_ptr,MOUNTAIN_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE);
                    memcpy(prv_data_ptr,data_ptr,MOUNTAIN_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE);
                }
                pkt_no++;
                data_ptr        += MOUNTAIN_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE;
                prv_data_ptr    += MOUNTAIN_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE;
                data_len        -= MOUNTAIN_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE;
            }
            else
            {
                if(first_call || !quick_mode || memcmp(data_ptr,prv_data_ptr,data_len))
                {
                    MountainKeyboardController::SendColorEdgePacketCmd(pkt_no,data_ptr,data_len);
                    memcpy(prv_data_ptr,data_ptr,data_len);
                }
                data_len = 0;
            }
        }
        if(!quick_mode)
        {
            SendColorPacketFinishCmd();
        }

        if (first_call)
        {
            first_call = false;
        }
    }
}

void MountainKeyboardController::SendDirectColorCmd(bool quick_mode, unsigned char brightness, unsigned char *color_data, unsigned int data_size)
{
    static bool first_call = true;
    static unsigned char prv_buffer[MOUNTAIN_KEYBOARD_TRANSFER_BUFFER_SIZE] = {0};

    unsigned char pkt_no        = 0;
    unsigned char *data_ptr     = color_data;
    unsigned char *prv_data_ptr = prv_buffer;
    unsigned int  data_len      = data_size;

    if(MOUNTAIN_KEYBOARD_TRANSFER_BUFFER_SIZE >= data_len)
    {
        if(!quick_mode)
        {
            SendColorStartPacketCmd(brightness);
        }
        while(data_len>0)
        {
            if(data_len >= MOUNTAIN_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE)
            {
                if(first_call || !quick_mode || memcmp(data_ptr,prv_data_ptr,MOUNTAIN_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE))
                {
                    MountainKeyboardController::SendColorPacketCmd(pkt_no,brightness,data_ptr,MOUNTAIN_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE);
                    memcpy(prv_data_ptr,data_ptr,MOUNTAIN_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE);
                }
                pkt_no++;
                data_ptr        += MOUNTAIN_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE;
                prv_data_ptr    += MOUNTAIN_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE;
                data_len        -= MOUNTAIN_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE;
            }
            else
            {
                if(first_call || !quick_mode || memcmp(data_ptr,prv_data_ptr,data_len))
                {
                    MountainKeyboardController::SendColorPacketCmd(pkt_no,brightness,data_ptr,data_len);
                    memcpy(prv_data_ptr,data_ptr,data_len);
                }
                data_len = 0;
            }
        }
        if(!quick_mode)
        {
            SendColorPacketFinishCmd();
        }

        if (first_call)
        {
            first_call = false;
        }
    }
}
