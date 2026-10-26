/*---------------------------------------------------------*\
| KlipschLuminaController.cpp                               |
|                                                           |
|   Driver for Klipsch ProMedia Lumina speakers             |
|                                                           |
|   wlof                                        26 Oct 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include <thread>
#include "KlipschLuminaController.h"

KlipschLuminaController::KlipschLuminaController(hidapi_wrapper hid_wrapper, hid_device* dev_handle, hid_device* lamparray_handle, const char* path)
{
    wrapper         = hid_wrapper;
    dev             = dev_handle;
    lamparray_dev   = lamparray_handle;
    location        = path;
    light_mode      = KLIPSCH_LUMINA_MODE_UNKNOWN;
    brightness      = KLIPSCH_LUMINA_BRIGHTNESS_MAX;
    primary_color   = 0;
    secondary_color = 0;
    direct_unlocked = false;
    last_frame_time = std::chrono::steady_clock::now();

    ReadState();
}

KlipschLuminaController::~KlipschLuminaController()
{
    if(lamparray_dev != NULL)
    {
        wrapper.hid_close(lamparray_dev);
    }

    wrapper.hid_close(dev);
}

std::string KlipschLuminaController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string KlipschLuminaController::GetFirmwareVersion()
{
    return(firmware);
}

std::string KlipschLuminaController::GetSerialString()
{
    return(serial);
}

unsigned char KlipschLuminaController::GetLightMode()
{
    return(light_mode);
}

unsigned char KlipschLuminaController::GetBrightness()
{
    return(brightness);
}

RGBColor KlipschLuminaController::GetPrimaryColor()
{
    return(primary_color);
}

RGBColor KlipschLuminaController::GetSecondaryColor()
{
    return(secondary_color);
}

void KlipschLuminaController::ReadState()
{
    unsigned char                           usb_buf[KLIPSCH_LUMINA_BUFFER_SIZE];
    std::chrono::steady_clock::time_point   start   = std::chrono::steady_clock::now();
    bool                                    found   = false;

    /*-----------------------------------------------------*\
    | Request the state report with a bare frame header,    |
    | then read it back.  Repeat until it arrives, the      |
    | device ignores requests while it is powering up.      |
    \*-----------------------------------------------------*/
    while(!found && ((std::chrono::steady_clock::now() - start) < std::chrono::milliseconds(KLIPSCH_LUMINA_STATE_TIMEOUT_MS)))
    {
        memset(usb_buf, 0x00, sizeof(usb_buf));

        usb_buf[1]      = KLIPSCH_LUMINA_FRAME_HEADER_0;
        usb_buf[2]      = KLIPSCH_LUMINA_FRAME_HEADER_1;
        usb_buf[3]      = KLIPSCH_LUMINA_FRAME_HEADER_2;

        wrapper.hid_write(dev, usb_buf, KLIPSCH_LUMINA_BUFFER_SIZE);

        std::this_thread::sleep_for(std::chrono::milliseconds(KLIPSCH_LUMINA_STATE_DELAY_MS));

        memset(usb_buf, 0x00, sizeof(usb_buf));

        wrapper.hid_get_input_report(dev, usb_buf, KLIPSCH_LUMINA_BUFFER_SIZE);

        found = (usb_buf[1] == KLIPSCH_LUMINA_FRAME_HEADER_0) && (usb_buf[2] == KLIPSCH_LUMINA_FRAME_HEADER_1) && (usb_buf[3] == KLIPSCH_LUMINA_FRAME_HEADER_2);
    }

    if(!found)
    {
        light_mode = KLIPSCH_LUMINA_MODE_UNKNOWN;
        return;
    }

    light_mode = usb_buf[KLIPSCH_LUMINA_STATE_LIGHT_MODE];
    brightness = usb_buf[KLIPSCH_LUMINA_STATE_LED_LEVEL];

    primary_color   = ToRGBColor(usb_buf[KLIPSCH_LUMINA_STATE_CUSTOM_RGB + 0], usb_buf[KLIPSCH_LUMINA_STATE_CUSTOM_RGB + 1], usb_buf[KLIPSCH_LUMINA_STATE_CUSTOM_RGB + 2]);
    secondary_color = ToRGBColor(usb_buf[KLIPSCH_LUMINA_STATE_CUSTOM_RGB + 3], usb_buf[KLIPSCH_LUMINA_STATE_CUSTOM_RGB + 4], usb_buf[KLIPSCH_LUMINA_STATE_CUSTOM_RGB + 5]);

    char string_buf[KLIPSCH_LUMINA_STATE_SERIAL_LEN + 1];

    memcpy(string_buf, &usb_buf[KLIPSCH_LUMINA_STATE_SERIAL], KLIPSCH_LUMINA_STATE_SERIAL_LEN);
    string_buf[KLIPSCH_LUMINA_STATE_SERIAL_LEN] = '\0';
    serial = string_buf;

    memcpy(string_buf, &usb_buf[KLIPSCH_LUMINA_STATE_FIRMWARE], KLIPSCH_LUMINA_STATE_FIRMWARE_LEN);
    string_buf[KLIPSCH_LUMINA_STATE_FIRMWARE_LEN] = '\0';
    firmware = string_buf;
}

void KlipschLuminaController::SetLightMode(unsigned char mode)
{
    /*-----------------------------------------------------*\
    | Read the current mode, it can be changed from the     |
    | speakers or from Klipsch's app                        |
    \*-----------------------------------------------------*/
    ReadState();

    bool mode_changed = (mode != light_mode);

    if(mode_changed)
    {
        SendCommand(KLIPSCH_LUMINA_CMD_LIGHT_MODE, &mode, 1);

        /*-------------------------------------------------*\
        | A mode change clears the stored colors            |
        \*-------------------------------------------------*/
        light_mode      = mode;
        primary_color   = 0;
        secondary_color = 0;

        std::this_thread::sleep_for(std::chrono::milliseconds(KLIPSCH_LUMINA_MODE_DELAY_MS));
    }

    /*-----------------------------------------------------*\
    | Direct mode renders nothing until LampArrayControl    |
    | (AutonomousMode = 1) is written on the LampArray      |
    | interface.  On Windows the OS holds the interface and |
    | writes it itself.                                     |
    \*-----------------------------------------------------*/
    if((lamparray_dev != NULL) && (mode == KLIPSCH_LUMINA_MODE_DIRECT) && (mode_changed || !direct_unlocked))
    {
        unsigned char usb_buf[2];

        usb_buf[0]      = KLIPSCH_LUMINA_LAMPARRAY_CONTROL_ID;
        usb_buf[1]      = 0x01;

        wrapper.hid_send_feature_report(lamparray_dev, usb_buf, sizeof(usb_buf));

        std::this_thread::sleep_for(std::chrono::milliseconds(KLIPSCH_LUMINA_LAMPARRAY_DELAY_MS));

        direct_unlocked = true;
    }
}

void KlipschLuminaController::SetCustomColors(RGBColor primary, RGBColor secondary)
{
    /*-----------------------------------------------------*\
    | The firmware redraws at full brightness when it gets  |
    | colors, only send them when they change               |
    \*-----------------------------------------------------*/
    if((primary == primary_color) && (secondary == secondary_color))
    {
        return;
    }

    unsigned char data[6];

    data[0] = RGBGetRValue(primary);
    data[1] = RGBGetGValue(primary);
    data[2] = RGBGetBValue(primary);
    data[3] = RGBGetRValue(secondary);
    data[4] = RGBGetGValue(secondary);
    data[5] = RGBGetBValue(secondary);

    SendCommand(KLIPSCH_LUMINA_CMD_CUSTOM_RGB, data, sizeof(data));

    primary_color   = primary;
    secondary_color = secondary;
}

void KlipschLuminaController::SetLEDColors(RGBColor* colors)
{
    unsigned char data[3 * KLIPSCH_LUMINA_LED_COUNT];

    /*-----------------------------------------------------*\
    | Limit frame rate to the LampArray MinUpdateInterval   |
    \*-----------------------------------------------------*/
    std::chrono::steady_clock::time_point   now     = std::chrono::steady_clock::now();
    std::chrono::milliseconds               elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_frame_time);

    if(elapsed.count() < KLIPSCH_LUMINA_FRAME_INTERVAL_MS)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(KLIPSCH_LUMINA_FRAME_INTERVAL_MS) - elapsed);
    }

    for(unsigned int led_idx = 0; led_idx < KLIPSCH_LUMINA_LED_COUNT; led_idx++)
    {
        data[(3 * led_idx) + 0] = RGBGetRValue(colors[led_idx]);
        data[(3 * led_idx) + 1] = RGBGetGValue(colors[led_idx]);
        data[(3 * led_idx) + 2] = RGBGetBValue(colors[led_idx]);
    }

    SendCommand(KLIPSCH_LUMINA_CMD_CUSTOM_LIGHTING, data, sizeof(data));

    last_frame_time = std::chrono::steady_clock::now();
}

void KlipschLuminaController::SetBrightness(unsigned char level)
{
    /*-----------------------------------------------------*\
    | The level is kept across power cycles, only send it   |
    | when it changes                                       |
    \*-----------------------------------------------------*/
    if(level == brightness)
    {
        return;
    }

    unsigned char data[2];

    data[0] = KLIPSCH_LUMINA_LED_LEVEL_SELECTOR;
    data[1] = level;

    SendCommand(KLIPSCH_LUMINA_CMD_LED_LEVEL, data, sizeof(data));

    brightness = level;
}

void KlipschLuminaController::SendCommand(unsigned char cmd, const unsigned char* data, unsigned char data_len)
{
    unsigned char   usb_buf[KLIPSCH_LUMINA_BUFFER_SIZE];
    unsigned char   frame_len   = data_len + KLIPSCH_LUMINA_FRAME_OVERHEAD;
    unsigned char   sum         = 0;

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up command frame after the report ID byte         |
    \*-----------------------------------------------------*/
    usb_buf[1]      = KLIPSCH_LUMINA_FRAME_HEADER_0;
    usb_buf[2]      = KLIPSCH_LUMINA_FRAME_HEADER_1;
    usb_buf[3]      = KLIPSCH_LUMINA_FRAME_HEADER_2;
    usb_buf[4]      = cmd;
    usb_buf[5]      = frame_len;
    usb_buf[6]      = 0x00;

    memcpy(&usb_buf[7], data, data_len);

    /*-----------------------------------------------------*\
    | Calculate checksum                                    |
    \*-----------------------------------------------------*/
    for(unsigned int byte_idx = 1; byte_idx < frame_len; byte_idx++)
    {
        sum += usb_buf[byte_idx];
    }

    usb_buf[frame_len] = (unsigned char)(0x100 - sum);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    wrapper.hid_write(dev, usb_buf, KLIPSCH_LUMINA_BUFFER_SIZE);
}
