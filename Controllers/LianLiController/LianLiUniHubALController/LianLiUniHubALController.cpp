/*---------------------------------------------------------*\
| LianLiUniHubALController.cpp                              |
|                                                           |
|   Driver for Lian Li AL Uni Hub                           |
|                                                           |
|   Oliver P                                    26 Apr 2022 |
|   Credit to Luca Lovisa for original work                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string.h>
#include "LianLiUniHubALController.h"
#include "StringUtils.h"

using namespace std::chrono_literals;

LianLiUniHubALController::LianLiUniHubALController(hid_device* dev_handle, const char* path, unsigned short pid, std::string dev_name)
{
    dev             = dev_handle;
    dev_pid         = pid;
    location        = path;
    name            = dev_name;
}

LianLiUniHubALController::~LianLiUniHubALController()
{
    hid_close(dev);
}

std::string LianLiUniHubALController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string LianLiUniHubALController::GetFirmwareVersionString()
{
    wchar_t product_string[40];
    int ret = hid_get_product_string(dev, product_string, 40);

    if (ret != 0)
    {
        return ("");
    }

    std::string return_string = StringUtils::wstring_to_string(product_string);

    return(return_string.substr(return_string.find_last_of("-")+1,4).c_str());
}

std::string LianLiUniHubALController::GetName()
{
    return(name);
}

std::string LianLiUniHubALController::GetSerialString()
{
    wchar_t serial_string[20];
    int ret = hid_get_serial_number_string(dev, serial_string, 20);

    if (ret != 0)
    {
        return ("");
    }

    std::string return_string = StringUtils::wstring_to_string(serial_string);

    return(return_string);

}

void LianLiUniHubALController::SetChannelLEDs(unsigned char channel, RGBColor * colors, unsigned int num_colors, float brightness)
{
    unsigned char   fan_led_data[96];
    unsigned char   edge_led_data[144];
    int             fan_idx = 0;
    int             mod_led_idx;
    int             cur_led_idx;

    if(num_colors == 0)
    {
        return;     // Do nothing, channel isn't in use
    }

    for(unsigned int led_idx = 0; led_idx < num_colors; led_idx++)
    {
        mod_led_idx = (led_idx % 20);

        if((mod_led_idx == 0) && (led_idx != 0))
        {
            fan_idx++;
        }

        /*---------------------------------------------------------*\
        | Limiter to protect LEDs                                   |
        \*---------------------------------------------------------*/
        if(UNIHUB_AL_LED_LIMITER && RGBGetRValue(colors[led_idx]) > 153 && (RGBGetRValue(colors[led_idx]) == RGBGetBValue(colors[led_idx])) && (RGBGetRValue(colors[led_idx]) == RGBGetGValue(colors[led_idx])) )
        {
            colors[led_idx] = ToRGBColor(153,153,153);
        }

        if(mod_led_idx < 8)         // Fan LEDs, 8 LEDs per fan
        {
            //Determine current position of led_data array from colors array
            cur_led_idx = ((mod_led_idx + (fan_idx * 8)) * 3);

            fan_led_data[cur_led_idx + 0] = (unsigned char)(RGBGetRValue(colors[led_idx]) * brightness);
            fan_led_data[cur_led_idx + 1] = (unsigned char)(RGBGetBValue(colors[led_idx]) * brightness);
            fan_led_data[cur_led_idx + 2] = (unsigned char)(RGBGetGValue(colors[led_idx]) * brightness);
        }
        else                        // Edge LEDs, 12 LEDs per fan
        {
            //Determine current position of led_data array from colors array
            cur_led_idx = (((mod_led_idx - 8) + (fan_idx * 12)) * 3);

            edge_led_data[cur_led_idx + 0]  = (unsigned char)(RGBGetRValue(colors[led_idx]) * brightness);
            edge_led_data[cur_led_idx + 1]  = (unsigned char)(RGBGetBValue(colors[led_idx]) * brightness);
            edge_led_data[cur_led_idx + 2]  = (unsigned char)(RGBGetGValue(colors[led_idx]) * brightness);
        }
    }

    /*---------------------------------------------------------*\
    | Send fan LED data                                         |
    \*---------------------------------------------------------*/

    SendStartAction
    (
        channel,            // Current channel
        (fan_idx + 1)       // Number of fans
    );

    SendColorData
    (
        channel,            // Channel
        0,                  // 0 = Fan, 1 = Edge
        (fan_idx + 1)*8,
        fan_led_data
    );

    SendCommitAction
    (
        channel,                                // Channel
        0,                                      // 0 = Fan, 1 = Edge
        UNIHUB_AL_LED_MODE_STATIC_COLOR,        // Effect
        UNIHUB_AL_LED_SPEED_000,                // Speed
        UNIHUB_AL_LED_DIRECTION_LTR,            // Direction
        UNIHUB_AL_LED_BRIGHTNESS_100            // Brightness
    );

    /*---------------------------------------------------------*\
    | Send edge LED data                                        |
    \*---------------------------------------------------------*/
    SendStartAction
    (
        channel,            // Current channel
        (fan_idx + 1)       // Number of fans
    );

    SendColorData
    (
        channel,            // Channel
        1,                  // 0 = Fan, 1 = Edge
        (fan_idx + 1)*12,
        edge_led_data
    );

    SendCommitAction
    (
        channel,                                // Channel
        1,                                      // 0 = Fan, 1 = Edge
        UNIHUB_AL_LED_MODE_STATIC_COLOR,        // Effect
        UNIHUB_AL_LED_SPEED_000,                // Speed
        UNIHUB_AL_LED_DIRECTION_LTR,            // Direction
        UNIHUB_AL_LED_BRIGHTNESS_100            // Brightness
    );

}

void LianLiUniHubALController::SetChannelMode(unsigned char channel, unsigned int mode_value, std::vector<RGBColor> colors, unsigned int num_colors, unsigned int num_fans, bool upd_both_fan_edge, unsigned int brightness, unsigned int speed, unsigned int direction)
{
    static unsigned int brightness_code[5] =
    {
        UNIHUB_AL_LED_BRIGHTNESS_000,
        UNIHUB_AL_LED_BRIGHTNESS_025,
        UNIHUB_AL_LED_BRIGHTNESS_050,
        UNIHUB_AL_LED_BRIGHTNESS_075,
        UNIHUB_AL_LED_BRIGHTNESS_100
    };

    static unsigned int speed_code[5] =
    {
        UNIHUB_AL_LED_SPEED_000,
        UNIHUB_AL_LED_SPEED_025,
        UNIHUB_AL_LED_SPEED_050,
        UNIHUB_AL_LED_SPEED_075,
        UNIHUB_AL_LED_SPEED_100
    };

    unsigned char   fan_led_data[96];
    unsigned char   edge_led_data[144];
    int             cur_led_idx;
    float           brightness_scale;

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(fan_led_data, 0x00, sizeof(fan_led_data));
    memset(edge_led_data, 0x00, sizeof(edge_led_data));

    if(num_colors)                                          // Update led_data if there's colors
    {
        switch(mode_value)
        {
            case UNIHUB_AL_LED_MODE_STATIC_COLOR:           // Static mode requires a full data array
            case UNIHUB_AL_LED_MODE_BREATHING:
                brightness_scale = static_cast<float>(brightness)/4;
                for(unsigned int i = 0; i < 4; i++)
                {
                    /*---------------------------------------------------------*\
                    | Limiter to protect LEDs                                   |
                    \*---------------------------------------------------------*/
                    if(UNIHUB_AL_LED_LIMITER && RGBGetRValue(colors[i]) > 153 && (RGBGetRValue(colors[i]) == RGBGetBValue(colors[i])) && (RGBGetRValue(colors[i]) == RGBGetGValue(colors[i])) )
                    {
                        colors[i] = ToRGBColor(153,153,153);
                    }

                    for(unsigned int led_idx = 0; led_idx < 22; led_idx += 3)
                    {
                        cur_led_idx = (i * 8 * 3) + led_idx;
                        fan_led_data[cur_led_idx + 0] = (unsigned char)(RGBGetRValue(colors[i]) * brightness_scale);
                        fan_led_data[cur_led_idx + 1] = (unsigned char)(RGBGetBValue(colors[i]) * brightness_scale);
                        fan_led_data[cur_led_idx + 2] = (unsigned char)(RGBGetGValue(colors[i]) * brightness_scale);
                    }

                    for(unsigned int led_idx = 0; led_idx < 34; led_idx += 3)
                    {
                        cur_led_idx = (i * 12 * 3) + led_idx;
                        edge_led_data[cur_led_idx + 0] = (unsigned char)(RGBGetRValue(colors[i]) * brightness_scale);
                        edge_led_data[cur_led_idx + 1] = (unsigned char)(RGBGetBValue(colors[i]) * brightness_scale);
                        edge_led_data[cur_led_idx + 2] = (unsigned char)(RGBGetGValue(colors[i]) * brightness_scale);
                    }
                }
                break;

            default:
                colors.resize(4);
                for(unsigned int i = num_colors; i < 4; i++)
                {
                    colors[i] = 0x00;
                }

                // needs a 48 length array of 4 colors, even if less are defined
                for(unsigned int i = 0; i < 4; i++)
                {
                    for(unsigned int j = 0; j < 4; j++)
                    {
                        cur_led_idx = (i * 12) + (j * 3);
                        fan_led_data[cur_led_idx + 0] = RGBGetRValue(colors[j]);
                        fan_led_data[cur_led_idx + 1] = RGBGetBValue(colors[j]);
                        fan_led_data[cur_led_idx + 2] = RGBGetGValue(colors[j]);
                    }
                }
                break;
       }

    }

    SendStartAction
    (
        channel,                        // Current channel
        (num_fans + 1)                  // Number of fans
    );

    SendColorData
    (
        channel,                        // Channel
        0,                              // 0 = Fan, 1 = Edge
        (num_fans + 1)*8,
        fan_led_data                        // Data
    );

    SendCommitAction
    (
        channel,                        // Channel
        0,                              // 0 = Fan, 1 = Edge
        mode_value,                     // Effect
        speed_code[speed],              // Speed
        direction,                      // Direction
        brightness_code[brightness]     // Brightness
    );

    if(upd_both_fan_edge)
    {
        SendStartAction
        (
            channel,                    // Current channel
            (num_fans + 1)              // Number of fans
        );

        SendColorData
        (
            channel,                    // Channel
            1,                          // 0 = Fan, 1 = Edge
            (num_fans + 1)*12,
            edge_led_data
        );

        SendCommitAction
        (
            channel,                    // Channel
            1,                          // 0 = Fan, 1 = Edge
            mode_value,                 // Effect
            speed_code[speed],          // Speed
            direction,                  // Direction
            brightness_code[brightness] // Brightness
        );
    }
}

void LianLiUniHubALController::SendStartAction(unsigned char channel, unsigned int num_fans)
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet                                 |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = UNIHUB_AL_TRANSACTION_ID;
    usb_buf[0x01]   = 0x10;
    usb_buf[0x02]   = 0x40;
    usb_buf[0x03]   = channel + 1;
    usb_buf[0x04]   = num_fans;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    std::this_thread::sleep_for(5ms);

}

void LianLiUniHubALController::SendColorData(unsigned char   channel, unsigned int fan_or_edge, unsigned int    num_leds, unsigned char*  led_data)
{
    /*---------------------------------------------------------*\
    | Send edge LED data                                        |
    \*---------------------------------------------------------*/

    unsigned char usb_buf[146];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet                                 |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = UNIHUB_AL_TRANSACTION_ID;
    usb_buf[0x01]   = 0x30 + fan_or_edge + (channel * 2); // Channel+device (30 = channel 1 edge, 31 = channel 1 edge, 32 = channel 2 fan, 33 = channel 2 edge, etc.)

    /*-----------------------------------------------------*\
    | Copy in color data bytes                              |
    \*-----------------------------------------------------*/
    memcpy(&usb_buf[0x02], led_data, num_leds * 3);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 146);
    std::this_thread::sleep_for(5ms);
}

void LianLiUniHubALController::SendCommitAction(unsigned char channel, unsigned int fan_or_edge, unsigned char effect, unsigned char speed, unsigned int direction, unsigned int brightness)
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet                                 |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = UNIHUB_AL_TRANSACTION_ID;
    usb_buf[0x01]   = 0x10 + fan_or_edge + (channel*2); // Channel+device (10 = channel 1 fan, 11 = channel 1 edge, 12 = channel 2 fan, 13 = chanell 2 edge, etc.)
    usb_buf[0x02]   = effect;                           // Effect
    usb_buf[0x03]   = speed;                            // Speed, 02=0%, 01=25%, 00=50%, ff=75%, fe=100%
    usb_buf[0x04]   = direction;                        // Direction, right=00, left=01
    usb_buf[0x05]   = brightness;                       // Brightness, 0=100%, 1= 75%, 2 = 50%, 3 = 25%, 8 = 0%

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    std::this_thread::sleep_for(5ms);
}
