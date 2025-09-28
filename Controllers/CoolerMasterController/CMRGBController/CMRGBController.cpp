/*---------------------------------------------------------*\
| CMRGBController.cpp                                       |
|                                                           |
|   Driver for Cooler Master RGB controller                 |
|                                                           |
|   Nic W (midgetspy)                           13 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include "RGBController_CMRGBController.h"
#include "CMRGBController.h"
#include "StringUtils.h"

CMRGBController::CMRGBController(hid_device* dev_handle, char* path)
{
    dev         = dev_handle;
    location    = path;

    /*---------------------------------------------------------*\
    | Get device name from HID manufacturer and product strings |
    \*---------------------------------------------------------*/
    wchar_t name_string[HID_MAX_STR];

    hid_get_manufacturer_string(dev, name_string, HID_MAX_STR);
    device_name = StringUtils::wstring_to_string(name_string);

    hid_get_product_string(dev, name_string, HID_MAX_STR);
    device_name.append(" ").append(StringUtils::wstring_to_string(name_string));

    ReadCurrentMode();
}

void CMRGBController::SendFlowControl(unsigned char byte_flag)
{
    const unsigned char buffer_size     = CM_RGBC_PACKET_SIZE;
    unsigned char buffer[buffer_size]   = { 0x00, CM_RGBC_OPCODE_OP_FLOW_CONTROL }; //Packets on Windows need a 0x00 if they don't use ReportIDs

    buffer[0x02] = byte_flag;

    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_RGBC_INTERRUPT_TIMEOUT);
}

void CMRGBController::SendApply()
{
    const unsigned char buffer_size     = CM_RGBC_PACKET_SIZE;
    unsigned char buffer[buffer_size]   = { 0x00, CM_RGBC_OPCODE_OP_UNKNOWN_50, CM_RGBC_OPCODE_TYPE_UNKNOWN_55 }; //Packets on Windows need a 0x00 if they don't use ReportIDs

    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_RGBC_INTERRUPT_TIMEOUT);
}

void CMRGBController::SendReadMode()
{
    const unsigned char buffer_size     = CM_RGBC_PACKET_SIZE;
    unsigned char buffer[buffer_size]   = { };

    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_OP]     = CM_RGBC_OPCODE_OP_READ;
    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_TYPE]   = CM_RGBC_OPCODE_TYPE_MODE;

    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_RGBC_INTERRUPT_TIMEOUT);

    current_mode = buffer[CM_RGBC_PACKET_OFFSET_MODE];
}

void CMRGBController::SendSetMode(unsigned char mode)
{
    const unsigned char buffer_size     = CM_RGBC_PACKET_SIZE;
    unsigned char buffer[buffer_size]   = { };

    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_OP]     = CM_RGBC_OPCODE_OP_WRITE;
    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_TYPE]   = CM_RGBC_OPCODE_TYPE_MODE;
    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_MODE]   = mode;

    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_RGBC_INTERRUPT_TIMEOUT);
}

void CMRGBController::SendSetCustomColors(RGBColor color_1, RGBColor color_2, RGBColor color_3, RGBColor color_4)
{
    const unsigned char buffer_size     = CM_RGBC_PACKET_SIZE;
    unsigned char buffer[buffer_size]   = { };

    current_port1_color = color_1;
    current_port2_color = color_2;
    current_port3_color = color_3;
    current_port4_color = color_4;

    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_OP]                     = CM_RGBC_OPCODE_OP_WRITE;
    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_TYPE]                   = CM_RGBC_OPCODE_TYPE_LED_INFO;

    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_MULTIPLE_COLOR_1]       = RGBGetRValue(color_1);
    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_MULTIPLE_COLOR_1 + 1]   = RGBGetGValue(color_1);
    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_MULTIPLE_COLOR_1 + 2]   = RGBGetBValue(color_1);

    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_MULTIPLE_COLOR_2]       = RGBGetRValue(color_2);
    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_MULTIPLE_COLOR_2 + 1]   = RGBGetGValue(color_2);
    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_MULTIPLE_COLOR_2 + 2]   = RGBGetBValue(color_2);

    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_MULTIPLE_COLOR_3]       = RGBGetRValue(color_3);
    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_MULTIPLE_COLOR_3 + 1]   = RGBGetGValue(color_3);
    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_MULTIPLE_COLOR_3 + 2]   = RGBGetBValue(color_3);

    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_MULTIPLE_COLOR_4]       = RGBGetRValue(color_4);
    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_MULTIPLE_COLOR_4 + 1]   = RGBGetGValue(color_4);
    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_MULTIPLE_COLOR_4 + 2]   = RGBGetBValue(color_4);

    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_RGBC_INTERRUPT_TIMEOUT);
}

void CMRGBController::SendReadCustomColors()
{
    const unsigned char buffer_size     = CM_RGBC_PACKET_SIZE;
    unsigned char buffer[buffer_size]   = { };

    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_OP]     = CM_RGBC_OPCODE_OP_READ;
    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_TYPE]   = CM_RGBC_OPCODE_TYPE_LED_INFO;

    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_RGBC_INTERRUPT_TIMEOUT);

    current_port1_color = ToRGBColor(
        buffer[CM_RGBC_PACKET_OFFSET_MULTIPLE_COLOR_1],
        buffer[CM_RGBC_PACKET_OFFSET_MULTIPLE_COLOR_1 + 1],
        buffer[CM_RGBC_PACKET_OFFSET_MULTIPLE_COLOR_1 + 2]);
    current_port2_color = ToRGBColor(
        buffer[CM_RGBC_PACKET_OFFSET_MULTIPLE_COLOR_2],
        buffer[CM_RGBC_PACKET_OFFSET_MULTIPLE_COLOR_2 + 1],
        buffer[CM_RGBC_PACKET_OFFSET_MULTIPLE_COLOR_2 + 2]);
    current_port3_color = ToRGBColor(
        buffer[CM_RGBC_PACKET_OFFSET_MULTIPLE_COLOR_3],
        buffer[CM_RGBC_PACKET_OFFSET_MULTIPLE_COLOR_3 + 1],
        buffer[CM_RGBC_PACKET_OFFSET_MULTIPLE_COLOR_3 + 2]);
    current_port4_color = ToRGBColor(
        buffer[CM_RGBC_PACKET_OFFSET_MULTIPLE_COLOR_4],
        buffer[CM_RGBC_PACKET_OFFSET_MULTIPLE_COLOR_4 + 1],
        buffer[CM_RGBC_PACKET_OFFSET_MULTIPLE_COLOR_4 + 2]);
}

void CMRGBController::SendSetConfig(unsigned char mode, unsigned char speed, unsigned char brightness, RGBColor color_1, RGBColor color_2, bool simplified=false, bool multilayer=false)
{
    const unsigned char buffer_size     = CM_RGBC_PACKET_SIZE;
    unsigned char buffer[buffer_size]   = { };

    current_mode            = mode;
    current_speed           = speed;
    current_brightness      = brightness;
    current_mode_color_1    = color_1;
    current_mode_color_2    = color_2;

    /*---------------------------------------------*\
    | Handle special cases                          |
    \*---------------------------------------------*/
    switch(mode)
    {
        case CM_RGBC_MODE_COLOR_CYCLE:
            brightness  = 0xDF;
            color_1     = 0xFFFFFF;
            color_2     = 0x000000;
            break;

        case CM_RGBC_MODE_OFF:
            brightness  = 0x03;
            break;
    }

    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_OP]                 = CM_RGBC_OPCODE_OP_WRITE;
    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_TYPE]               = simplified ? CM_RGBC_OPCODE_TYPE_CONFIG_SIMPLIFIED : CM_RGBC_OPCODE_TYPE_CONFIG_FULL;
    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_MODE]               = mode;
    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_SPEED]              = speed;
    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_BRIGHTNESS]         = brightness;
    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_COLOR_1]            = RGBGetRValue(color_1);
    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_COLOR_1 + 1]        = RGBGetGValue(color_1);
    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_COLOR_1 + 2]        = RGBGetBValue(color_1);

    /*---------------------------------------------*\
    | Magic values, meaning unknown                 |
    \*---------------------------------------------*/
    buffer[REPORT_ID_OFFSET + 0x06] = (mode == CM_RGBC_MODE_BREATHING)  ? 0x20 : 0x00;
    buffer[REPORT_ID_OFFSET + 0x07] = (mode == CM_RGBC_MODE_STAR)       ? 0x19 : 0xFF;
    buffer[REPORT_ID_OFFSET + 0x08]                                     = 0xFF;

    if(!simplified)
    {
        buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_MULTILAYER]     = multilayer ? 0x01 : 0x00;
        buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_COLOR_2]        = RGBGetRValue(color_2);
        buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_COLOR_2 + 1]    = RGBGetGValue(color_2);
        buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_COLOR_2 + 2]    = RGBGetBValue(color_2);

        for(int i = REPORT_ID_OFFSET + 16; i < CM_RGBC_PACKET_SIZE; i++)
        {
            buffer[i] = 0xFF;
        }
    }

    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_RGBC_INTERRUPT_TIMEOUT);
}

void CMRGBController::SendReadConfig(unsigned char mode)
{
    const unsigned char buffer_size     = CM_RGBC_PACKET_SIZE;
    unsigned char buffer[buffer_size]   = { };

    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_OP]             = CM_RGBC_OPCODE_OP_READ;
    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_TYPE]           = CM_RGBC_OPCODE_TYPE_CONFIG_FULL;
    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_MODE]           = mode;

    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_RGBC_INTERRUPT_TIMEOUT);

    current_mode            = mode;
    current_speed           = buffer[CM_RGBC_PACKET_OFFSET_SPEED];
    current_brightness      = buffer[CM_RGBC_PACKET_OFFSET_BRIGHTNESS];

    current_mode_color_1    = ToRGBColor(
        buffer[CM_RGBC_PACKET_OFFSET_COLOR_1],
        buffer[CM_RGBC_PACKET_OFFSET_COLOR_1 + 1],
        buffer[CM_RGBC_PACKET_OFFSET_COLOR_1 + 2]);
    current_mode_color_2    = ToRGBColor(
        buffer[CM_RGBC_PACKET_OFFSET_COLOR_2],
        buffer[CM_RGBC_PACKET_OFFSET_COLOR_2 + 1],
        buffer[CM_RGBC_PACKET_OFFSET_COLOR_2 + 2]);
}

void CMRGBController::SendCustomColorStart()
{
    const unsigned char buffer_size     = CM_RGBC_PACKET_SIZE;
    unsigned char buffer[buffer_size]   = { };

    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_OP]     = CM_RGBC_OPCODE_OP_WRITE;
    buffer[REPORT_ID_OFFSET + CM_RGBC_PACKET_OFFSET_TYPE]   = CM_RGBC_OPCODE_TYPE_UNKNOWN_30;

    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_RGBC_INTERRUPT_TIMEOUT);
}

void CMRGBController::ReadCurrentMode()
{
    SendFlowControl(CM_RGBC_OPCODE_FLOW_01);

    SendReadMode();
}

void CMRGBController::ReadModeConfig(unsigned char mode)
{
    SendFlowControl(CM_RGBC_OPCODE_FLOW_00);

    SendReadConfig(mode);

    if(mode == CM_RGBC_MODE_MULTIPLE)
    {
        SendReadCustomColors();
    }
}

void CMRGBController::SetMode(unsigned char mode, unsigned char speed, unsigned char brightness, RGBColor color_1, RGBColor color_2)
{
    SendFlowControl(CM_RGBC_OPCODE_FLOW_01);

    SendSetConfig(mode, speed, brightness, color_1, color_2, false);

    SendSetMode(mode);

    SendApply();

    SendFlowControl(CM_RGBC_OPCODE_FLOW_00);
}

void CMRGBController::SetLedsDirect(RGBColor color_1, RGBColor color_2, RGBColor color_3, RGBColor color_4)
{
    SendFlowControl(CM_RGBC_OPCODE_FLOW_80);

    SendCustomColorStart();

    SendSetCustomColors(color_1, color_2, color_3, color_4);

    SendSetMode(CM_RGBC_MODE_MULTIPLE);

    SendCustomColorStart();

    SendSetConfig(CM_RGBC_MODE_MULTIPLE, 0x00, 0xFF, color_1, 0x000000, false);

    SendApply();

    SendFlowControl(CM_RGBC_OPCODE_FLOW_00);
}

std::string CMRGBController::GetDeviceName()
{
    return(device_name);
}

std::string CMRGBController::GetSerial()
{
    wchar_t serial_string[HID_MAX_STR];
    int ret = hid_get_serial_number_string(dev, serial_string, HID_MAX_STR);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string CMRGBController::GetLocation()
{
    return("HID: " + location);
}

unsigned char CMRGBController::GetMode()
{
    return(current_mode);
}

unsigned char CMRGBController::GetSpeed()
{
    return(current_speed);
}

unsigned char CMRGBController::GetBrightness()
{
    return(current_brightness);
}

RGBColor CMRGBController::GetModeColor(int color_number)
{
    switch(color_number)
    {
        case 0:
            return(current_mode_color_1);

        case 1:
            return(current_mode_color_2);

        default:
            return(ToRGBColor(0, 0, 0));
    }
}

RGBColor CMRGBController::GetPortColor(int port_number)
{
    switch(port_number)
    {
        case 0:
            return(current_port1_color);

        case 1:
            return(current_port2_color);

        case 2:
            return(current_port3_color);

        case 3:
            return(current_port4_color);

        default:
            return(ToRGBColor(0, 0, 0));
    }
}

CMRGBController::~CMRGBController()
{
    if(dev)
    {
        hid_close(dev);
    }
}
