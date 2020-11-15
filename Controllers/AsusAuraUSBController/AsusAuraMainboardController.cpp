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
    /*-----------------------------------------------------*\
    | Add mainboard device                                  |
    \*-----------------------------------------------------*/
    device_info.push_back({0x00, 0x04, config_table[0x1B], AuraDeviceType::FIXED});

    /*-----------------------------------------------------*\
    | Add addressable devices                               |
    \*-----------------------------------------------------*/
    for(int i = 0; i < config_table[0x02]; ++i)
    {
        device_info.push_back({0x01, (unsigned char)i, 0x01, AuraDeviceType::ADDRESSABLE});
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

    SendCommit();
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
        led_data,
        device_info[channel].device_type == AuraDeviceType::FIXED
    );

    SendCommit();
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
    unsigned char   channel,
    unsigned char   start_led,
    unsigned char   led_count,
    unsigned char*  led_data,
    bool            fixed
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
    usb_buf[0x01]   = AURA_MAINBOARD_CONTROL_MODE_EFFECT_COLOR;
    usb_buf[0x02]   = channel;
    usb_buf[0x03]   = fixed ? 0xFF : 0x00;
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
