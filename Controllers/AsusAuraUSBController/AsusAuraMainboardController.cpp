/*-----------------------------------------*\
|  AsusAuraMainboardController.cpp          |
|                                           |
|  Driver for ASUS Aura RGB USB mainboard   |
|  lighting controller                      |
|                                           |
|  Martin Hartl (inlart) 4/25/2020          |
\*-----------------------------------------*/

#include "AsusAuraMainboardController.h"
#include <cstring>

AuraMainboardController::AuraMainboardController(hid_device* dev_handle, const char* path) : AuraUSBController(dev_handle, path), mode(AURA_MODE_DIRECT)
{
    unsigned char num_total_mainboard_leds  = config_table[0x1B];
    unsigned char num_rgb_headers           = config_table[0x1D];
    unsigned char num_addressable_headers   = config_table[0x02];

    if(num_total_mainboard_leds < num_rgb_headers)
    {
        num_rgb_headers = 0;
    }

    /*-----------------------------------------------------*\
    | Add mainboard device                                  |
    \*-----------------------------------------------------*/
    device_info.push_back({0x00, 0x04, num_total_mainboard_leds, num_rgb_headers, AuraDeviceType::FIXED});

    /*-----------------------------------------------------*\
    | Add addressable devices                               |
    \*-----------------------------------------------------*/
    for(int i = 0; i < num_addressable_headers; i++)
    {
        device_info.push_back({0x01, (unsigned char)i, 0x01, 0, AuraDeviceType::ADDRESSABLE});
    }
}

AuraMainboardController::~AuraMainboardController()
{
}

void AuraMainboardController::SetChannelLEDs(unsigned char channel, RGBColor * colors, unsigned int num_colors)
{
    unsigned char   led_data[60];
    unsigned int    leds_sent = 0;

    while(leds_sent < num_colors)
    {
        unsigned int leds_to_send = 20;

        if((num_colors - leds_sent) < leds_to_send)
        {
            leds_to_send = num_colors - leds_sent;
        }

        for(unsigned int led_idx = 0; led_idx < leds_to_send; led_idx++)
        {
            led_data[(led_idx * 3) + 0] = RGBGetRValue(colors[led_idx + leds_sent]);
            led_data[(led_idx * 3) + 1] = RGBGetGValue(colors[led_idx + leds_sent]);
            led_data[(led_idx * 3) + 2] = RGBGetBValue(colors[led_idx + leds_sent]);
        }

        SendDirect
        (
            device_info[channel].direct_channel,
            leds_sent,
            leds_to_send,
            led_data,
            leds_sent + leds_to_send >= num_colors
        );

        leds_sent += leds_to_send;
    }
}

void AuraMainboardController::SetMode
    (
    unsigned char   channel,
    unsigned char   mode,
    unsigned char   red,
    unsigned char   grn,
    unsigned char   blu
    )
{
    this->mode = mode;
    RGBColor color = ToRGBColor(red, grn, blu);

    SendEffect(device_info[channel].effect_channel, mode);
    if(mode == AURA_MODE_DIRECT)
    {
        return;
    }

    unsigned char   led_data[60];
    unsigned char   start_led = 0;

    for(std::size_t i = 0; i < channel; ++i)
    {
        start_led += device_info[i].num_leds;
    }

    for(std::size_t led_idx = 0; led_idx < device_info[channel].num_leds; led_idx++)
    {
        led_data[(led_idx * 3) + 0] = RGBGetRValue(color);
        led_data[(led_idx * 3) + 1] = RGBGetGValue(color);
        led_data[(led_idx * 3) + 2] = RGBGetBValue(color);
    }

    SendColor
    (
        channel,
        start_led,
        device_info[channel].num_leds,
        led_data
    );
}

unsigned short AuraMainboardController::GetMask(int start, int size)
{
    return(((1 << size) - 1) << start);
}

void AuraMainboardController::SendEffect
    (
    unsigned char   channel,
    unsigned char   mode
    )
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet                                 |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0xEC;
    usb_buf[0x01]   = AURA_MAINBOARD_CONTROL_MODE_EFFECT;
    usb_buf[0x02]   = channel;
    usb_buf[0x03]   = 0x00;
    usb_buf[0x04]   = 0x00;
    usb_buf[0x05]   = mode;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
}

void AuraMainboardController::SendColor
    (
    unsigned char   /*channel*/,
    unsigned char   start_led,
    unsigned char   led_count,
    unsigned char*  led_data
    )
{
    unsigned short  mask = GetMask(start_led, led_count);
    unsigned char   usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet                                 |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0xEC;
    usb_buf[0x01]   = AURA_MAINBOARD_CONTROL_MODE_EFFECT_COLOR;
    usb_buf[0x02]   = mask >> 8;
    usb_buf[0x03]   = mask & 0xff;
    usb_buf[0x04]   = 0x00;

    /*-----------------------------------------------------*\
    | Copy in color data bytes                              |
    \*-----------------------------------------------------*/
    memcpy(&usb_buf[0x05 + 3 * start_led], led_data, led_count * 3);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
}

void AuraMainboardController::SendCommit()
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet                                 |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0xEC;
    usb_buf[0x01]   = AURA_MAINBOARD_CONTROL_MODE_COMMIT;
    usb_buf[0x02]   = 0x55;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
}
