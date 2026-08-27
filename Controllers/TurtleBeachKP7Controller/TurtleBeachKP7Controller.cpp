/*---------------------------------------------------------*\
| TurtleBeachKP7Controller.cpp                              |
|                                                           |
|   Driver for Turtle Beach Command Series KP7 keypad       |
|                                                           |
|   Joe Peterson (joevpeterson@gmail.com)       05 Aug 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <chrono>
#include <cstring>
#include "StringUtils.h"
#include "TurtleBeachKP7Controller.h"

using namespace std::chrono_literals;

TurtleBeachKP7Controller::TurtleBeachKP7Controller(hid_device* dev_handle, const char* path)
{
    dev                     = dev_handle;
    location                = path;
    last_led_data_size      = 0;

    memset(last_led_data, 0x00, sizeof(last_led_data));

    EnterDirectMode();

    keepalive_running       = true;
    keepalive_thread        = std::thread(&TurtleBeachKP7Controller::KeepAliveThreadFunction, this);
}

TurtleBeachKP7Controller::~TurtleBeachKP7Controller()
{
    keepalive_running = false;

    if(keepalive_thread.joinable())
    {
        keepalive_thread.join();
    }

    hid_close(dev);
}

std::string TurtleBeachKP7Controller::GetLocation()
{
    return("HID: " + location);
}

std::string TurtleBeachKP7Controller::GetSerialString()
{
    wchar_t serial_string[128];
    int     ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void TurtleBeachKP7Controller::EnterDirectMode()
{
    unsigned char buf[5];

    /*-----------------------------------------------------*\
    | Flip the device out of its onboard preset effects and |
    | into host controlled mode.  Until this is sent the    |
    | keypad ignores everything written to EP 0x06.         |
    \*-----------------------------------------------------*/
    buf[0] = TURTLE_BEACH_KP7_ENTER_DIRECT_REPORT_ID;
    buf[1] = 0x00;
    buf[2] = 0x01;
    buf[3] = 0x00;
    buf[4] = 0x00;

    hid_send_feature_report(dev, buf, sizeof(buf));
}

void TurtleBeachKP7Controller::SendDirect(unsigned char* led_data, unsigned int led_data_size)
{
    unsigned int copy_size = led_data_size;

    if(copy_size > sizeof(last_led_data))
    {
        copy_size = sizeof(last_led_data);
    }

    /*-----------------------------------------------------*\
    | Cache the frame so the keep alive thread has          |
    | something to re-send while the host is idle           |
    \*-----------------------------------------------------*/
    {
        std::lock_guard<std::mutex> lock(led_data_mutex);

        memcpy(last_led_data, led_data, copy_size);
        last_led_data_size = copy_size;
    }

    SendFrame(led_data, led_data_size);
}

void TurtleBeachKP7Controller::SendFrame(unsigned char* led_data, unsigned int led_data_size)
{
    unsigned char   buf[TURTLE_BEACH_KP7_REPORT_SIZE];
    unsigned int    page0_bytes = led_data_size;
    unsigned int    page1_bytes = 0;

    /*-----------------------------------------------------*\
    | Page 0 holds the first TURTLE_BEACH_KP7_PAGE0_LED_    |
    | BYTES bytes of the concatenated [R, G, B] LED buffer  |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    buf[0] = TURTLE_BEACH_KP7_REPORT_ID;
    buf[1] = 0x00;

    if(page0_bytes > TURTLE_BEACH_KP7_PAGE0_LED_BYTES)
    {
        page0_bytes = TURTLE_BEACH_KP7_PAGE0_LED_BYTES;
    }

    memcpy(&buf[2], led_data, page0_bytes);

    hid_write(dev, buf, TURTLE_BEACH_KP7_REPORT_SIZE);

    /*-----------------------------------------------------*\
    | Page 1 holds the remaining bytes, zero padded         |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    buf[0] = TURTLE_BEACH_KP7_REPORT_ID;
    buf[1] = 0x01;

    if(led_data_size > TURTLE_BEACH_KP7_PAGE0_LED_BYTES)
    {
        page1_bytes = led_data_size - TURTLE_BEACH_KP7_PAGE0_LED_BYTES;

        if(page1_bytes > TURTLE_BEACH_KP7_PAGE1_LED_BYTES)
        {
            page1_bytes = TURTLE_BEACH_KP7_PAGE1_LED_BYTES;
        }

        memcpy(&buf[2], &led_data[TURTLE_BEACH_KP7_PAGE0_LED_BYTES], page1_bytes);
    }

    hid_write(dev, buf, TURTLE_BEACH_KP7_REPORT_SIZE);
}

void TurtleBeachKP7Controller::KeepAliveThreadFunction()
{
    while(keepalive_running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(TURTLE_BEACH_KP7_KEEPALIVE_DELAY_MS));

        if(!keepalive_running)
        {
            break;
        }

        unsigned char   frame[TURTLE_BEACH_KP7_LED_COUNT * 3];
        unsigned int    frame_size;

        {
            std::lock_guard<std::mutex> lock(led_data_mutex);

            frame_size = last_led_data_size;

            if(frame_size > 0)
            {
                memcpy(frame, last_led_data, frame_size);
            }
        }

        if(frame_size > 0)
        {
            SendFrame(frame, frame_size);
        }
    }
}
