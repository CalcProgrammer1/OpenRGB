/*---------------------------------------------------------*\
| LianLiUniHubSLInfinityController.cpp                      |
|                                                           |
|   Driver for Lian Li SL Infinity Uni Hub                  |
|                                                           |
|   Simon McKenna                               21 Oct 2023 |
|   Will Kennedy                                17 Jan 2023 |
|   Oliver P                                    26 Apr 2022 |
|   Credit to Luca Lovisa for original work.                |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string.h>
#include "LianLiUniHubSLInfinityController.h"
#include "StringUtils.h"

using namespace std::chrono_literals;

LianLiUniHubSLInfinityController::LianLiUniHubSLInfinityController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev             = dev_handle;
    location        = path;
    name            = dev_name;
}

LianLiUniHubSLInfinityController::~LianLiUniHubSLInfinityController()
{
    hid_close(dev);
}

std::string LianLiUniHubSLInfinityController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string LianLiUniHubSLInfinityController::GetFirmwareVersionString()
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

std::string LianLiUniHubSLInfinityController::GetName()
{
    return(name);
}

std::string LianLiUniHubSLInfinityController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

float infinityBrightnessLimit(RGBColor color)
{
    /*---------------------------------------------------------*\
    | Limiter to protect LEDs                                   |
    \*---------------------------------------------------------*/
    if(UNIHUB_SLINF_LED_LIMITER && (RGBGetRValue(color) + RGBGetBValue(color) + RGBGetGValue(color) > 460))
    {
        return 460.f / (RGBGetRValue(color) + RGBGetBValue(color) + RGBGetGValue(color));
    }
    return 1;
}

void LianLiUniHubSLInfinityController::SetChannelLEDs(unsigned char channel, RGBColor * colors, unsigned int num_colors, float brightness)
{
    unsigned char   led_data[16 * 6 * 3];
    int             fan_idx = 0;
    int             mod_led_idx;
    int             cur_led_idx;

    if(num_colors == 0)
    {
        return;     // Do nothing, channel isn't in use
    }

    for(unsigned int led_idx = 0; led_idx < num_colors; led_idx++)
    {
        mod_led_idx = (led_idx % 16);

        if((mod_led_idx == 0) && (led_idx != 0))
        {
            fan_idx++;
        }

        float brightness_scale = brightness * infinityBrightnessLimit(colors[led_idx]);

        //Determine current position of led_data array from colors array
        cur_led_idx = ((mod_led_idx + (fan_idx * 16)) * 3);

        led_data[cur_led_idx + 0] = (unsigned char)(RGBGetRValue(colors[led_idx]) * brightness_scale);
        led_data[cur_led_idx + 1] = (unsigned char)(RGBGetBValue(colors[led_idx]) * brightness_scale);
        led_data[cur_led_idx + 2] = (unsigned char)(RGBGetGValue(colors[led_idx]) * brightness_scale);
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
        (fan_idx + 1)*16,
        led_data
    );

    SendCommitAction
    (
        channel,                                   // Channel
        UNIHUB_SLINF_LED_MODE_STATIC_COLOR,        // Effect
        UNIHUB_SLINF_LED_SPEED_000,                // Speed
        UNIHUB_SLINF_LED_DIRECTION_LTR,            // Direction
        UNIHUB_SLINF_LED_BRIGHTNESS_100            // Brightness
    );

}

void LianLiUniHubSLInfinityController::SetChannelMode(unsigned char channel, const mode active_mode, unsigned int num_fans)
{
    static unsigned int brightness_code[5] =
    {
        UNIHUB_SLINF_LED_BRIGHTNESS_000,
        UNIHUB_SLINF_LED_BRIGHTNESS_025,
        UNIHUB_SLINF_LED_BRIGHTNESS_050,
        UNIHUB_SLINF_LED_BRIGHTNESS_075,
        UNIHUB_SLINF_LED_BRIGHTNESS_100
    };

    static unsigned int speed_code[5] =
    {
        UNIHUB_SLINF_LED_SPEED_000,
        UNIHUB_SLINF_LED_SPEED_025,
        UNIHUB_SLINF_LED_SPEED_050,
        UNIHUB_SLINF_LED_SPEED_075,
        UNIHUB_SLINF_LED_SPEED_100
    };

    unsigned char   fan_led_data[16 * 6 * 3];
    int             cur_led_idx;
    float           brightness;

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(fan_led_data, 0x00, sizeof(fan_led_data));

    std::vector<RGBColor> colors = active_mode.colors;
    unsigned int num_colors = (unsigned int)colors.size();

    if(!colors.empty())                                          // Update led_data if there's colors
    {
        brightness = static_cast<float>(active_mode.brightness)/4;
        if(num_colors == 6)
        {
            for(unsigned int i = 0; i < 6; i++)
            {
                float brightness_scale = brightness * infinityBrightnessLimit(colors[i]);
                for(unsigned int led_idx = 0; led_idx < 16 * 3; led_idx += 3)
                {
                    cur_led_idx = (i * 16 * 3) + led_idx;
                    fan_led_data[cur_led_idx + 0] = (unsigned char)(RGBGetRValue(colors[i]) * brightness_scale);
                    fan_led_data[cur_led_idx + 1] = (unsigned char)(RGBGetBValue(colors[i]) * brightness_scale);
                    fan_led_data[cur_led_idx + 2] = (unsigned char)(RGBGetGValue(colors[i]) * brightness_scale);
                }
            }
        }
        else
        {
            colors.resize(4);
            for(unsigned int i = num_colors; i < 4; i++)
            {
                colors[i] = 0x00;
            }

            // needs a 72 length array of 4 colors, even if less are defined
            for(unsigned int j = 0; j < 4; j++)
            {
                float brightness_scale = brightness * infinityBrightnessLimit(colors[j]);
                for(unsigned int i = 0; i < 6; i++)
                {
                    cur_led_idx = (i * 12) + (j * 3);
                    fan_led_data[cur_led_idx + 0] = (unsigned char)(RGBGetRValue(colors[j]) * brightness_scale);
                    fan_led_data[cur_led_idx + 1] = (unsigned char)(RGBGetBValue(colors[j]) * brightness_scale);
                    fan_led_data[cur_led_idx + 2] = (unsigned char)(RGBGetGValue(colors[j]) * brightness_scale);
                }
            }
        }

    }

    SendStartAction
    (
        channel,                                // Current channel
        (num_fans + 1)                          // Number of fans
    );

    SendColorData
    (
        channel,                                // Channel
        (num_fans + 1)*16,
        fan_led_data                            // Data
    );

    SendCommitAction
    (
        channel,                                // Channel
        active_mode.value,                      // Effect
        speed_code[active_mode.speed],          // Speed
        active_mode.direction,                  // Direction
        brightness_code[active_mode.brightness] // Brightness
    );
}

void LianLiUniHubSLInfinityController::SendStartAction(unsigned char channel, unsigned int /*num_fans*/)
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet                                 |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = UNIHUB_SLINF_TRANSACTION_ID;
    usb_buf[0x01]   = 0x10;
    usb_buf[0x02]   = 0x60;
    usb_buf[0x03]   = 1 + (channel / 2); // every fan-array uses two channels (one for the spinner and one for the led-band on the side)
    usb_buf[0x04]   = 0x04; // TODO: number of fans (1-4) on this channel, hardcoding this to 4 for now

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, sizeof(usb_buf));
    std::this_thread::sleep_for(5ms);

}

void LianLiUniHubSLInfinityController::SendColorData(unsigned char   channel, unsigned int    num_leds, unsigned char*  led_data)
{
    /*---------------------------------------------------------*\
    | Send LED data                                        |
    \*---------------------------------------------------------*/

    unsigned char usb_buf[353];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet                                 |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = UNIHUB_SLINF_TRANSACTION_ID;
    usb_buf[0x01]   = 0x30 + channel; // action + channel(30 = channel 1, 31 = channel 2, etc.)

    /*-----------------------------------------------------*\
    | Copy in color data bytes                              |
    \*-----------------------------------------------------*/
    memcpy(&usb_buf[0x02], led_data, num_leds * 3);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, sizeof(usb_buf));
    std::this_thread::sleep_for(5ms);
}

void LianLiUniHubSLInfinityController::SendCommitAction(unsigned char channel, unsigned char effect, unsigned char speed, unsigned int direction, unsigned int brightness)
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet                                 |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = UNIHUB_SLINF_TRANSACTION_ID;
    usb_buf[0x01]   = 0x10 + channel; // Channel+device (10 = channel 1, 11 = channel 2, etc.)
    usb_buf[0x02]   = effect;                           // Effect
    usb_buf[0x03]   = speed;                            // Speed, 02=0%, 01=25%, 00=50%, ff=75%, fe=100%
    usb_buf[0x04]   = direction;                        // Direction, right=00, left=01
    usb_buf[0x05]   = brightness;                       // Brightness, 0=100%, 1= 75%, 2 = 50%, 3 = 25%, 8 = 0%

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, sizeof(usb_buf));
    std::this_thread::sleep_for(5ms);
}
