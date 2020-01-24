/*-----------------------------------------*\
|  AMDWraithPrismController.h               |
|                                           |
|  Driver for AMD Wraith Prism RGB lighting |
|  controller                               |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/6/2019   |
\*-----------------------------------------*/

#include "AMDWraithPrismController.h"
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

AMDWraithPrismController::AMDWraithPrismController(libusb_device_handle* dev_handle)
{
    dev = dev_handle;

    strcpy(device_name, "AMD Wraith Prism");

    current_fan_mode = AMD_WRAITH_PRISM_FAN_LOGO_MODE_STATIC;
    current_fan_speed = 0x00;

    current_logo_mode = AMD_WRAITH_PRISM_FAN_LOGO_MODE_STATIC;
    current_logo_speed = 0x00;

    current_ring_mode = AMD_WRAITH_PRISM_EFFECT_CHANNEL_STATIC;
    current_ring_speed = 0x00;
    current_ring_direction = false;

    SendEnableCommand();
    SetRingEffectChannel(0x00);
    SendApplyCommand();
}

AMDWraithPrismController::~AMDWraithPrismController()
{

}

char* AMDWraithPrismController::GetDeviceName()
{
    return device_name;
}

std::string AMDWraithPrismController::GetEffectChannelString(unsigned char channel)
{
    std::string ret_string = "";

    unsigned char usb_buf[] =
    {
        0x40, 0x21, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
    };

    int             actual;

    usb_buf[0x02] = channel;

    libusb_interrupt_transfer(dev, 0x04, usb_buf, 64, &actual, 0);
    libusb_interrupt_transfer(dev, 0x83, usb_buf, 64, &actual, 0);

    ret_string.append((char *)&usb_buf[0x08]);

    return(ret_string);
}

std::string AMDWraithPrismController::GetFirmwareVersionString()
{
    std::string ret_string = "";

    unsigned char usb_buf[] =
    {
        0x12, 0x20, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
    };

    int             actual;
    unsigned char   fw_buf[16] = {0x00};

    libusb_interrupt_transfer(dev, 0x04, usb_buf, 64, &actual, 0);
    libusb_interrupt_transfer(dev, 0x83, usb_buf, 64, &actual, 0);

    for(int char_idx = 0; char_idx < 16; char_idx+=2)
    {
        if(usb_buf[char_idx + 0x08] != 0)
        {
            fw_buf[char_idx / 2] = usb_buf[char_idx + 0x08];
        }
        else
        {
            break;
        }
    }

    ret_string.append((char *)fw_buf);

    return(ret_string);
}

void AMDWraithPrismController::SetFanMode(unsigned char mode, unsigned char speed)
{
    current_fan_mode        = mode;
    current_fan_speed       = speed;
}

void AMDWraithPrismController::SetFanColor(unsigned char red, unsigned char green, unsigned char blue)
{
    SendEffectChannelUpdate
    (
        AMD_WRAITH_PRISM_EFFECT_CHANNEL_FAN_LED,
        current_fan_speed,
        0x00,
        current_fan_mode,
        0xFF,
        red,
        green,
        blue
    );
}

void AMDWraithPrismController::SetLogoMode(unsigned char mode, unsigned char speed)
{
    current_logo_mode       = mode;
    current_logo_speed      = speed;
}

void AMDWraithPrismController::SetLogoColor(unsigned char red, unsigned char green, unsigned char blue)
{
    SendEffectChannelUpdate
    (
        AMD_WRAITH_PRISM_EFFECT_CHANNEL_LOGO_LED,
        current_logo_speed,
        0x00,
        current_logo_mode,
        0xFF,
        red,
        green,
        blue
    );
}

void AMDWraithPrismController::SetRingMode(unsigned char mode, unsigned char speed, bool direction)
{
    current_ring_mode       = mode;
    current_ring_speed      = speed;
    current_ring_direction  = direction;
}

void AMDWraithPrismController::SetRingColor(unsigned char red, unsigned char green, unsigned char blue)
{
    SendEffectChannelUpdate
    (
        AMD_WRAITH_PRISM_EFFECT_CHANNEL_STATIC,
        0xFF,
        0x00,
        AMD_WRAITH_PRISM_FAN_LOGO_MODE_STATIC,
        0xFF,
        red,
        green,
        blue
    );
}

void AMDWraithPrismController::SetRingEffectChannel(unsigned char channel)
{
    SendChannelRemap(channel, AMD_WRAITH_PRISM_EFFECT_CHANNEL_LOGO_LED, AMD_WRAITH_PRISM_EFFECT_CHANNEL_FAN_LED);
}

void AMDWraithPrismController::SendEnableCommand()
{
    unsigned char usb_buf[] =
    {
        0x41, 0x80, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
    };

    int             actual;

    libusb_interrupt_transfer(dev, 0x04, usb_buf, 64, &actual, 0);
    libusb_interrupt_transfer(dev, 0x83, usb_buf, 64, &actual, 0);
}

void AMDWraithPrismController::SendApplyCommand()
{
    unsigned char usb_buf[] =
    {
        0x51, 0x28, 0x00, 0x00,
        0xE0, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
    };

    int             actual;

    libusb_interrupt_transfer(dev, 0x04, usb_buf, 64, &actual, 0);
    libusb_interrupt_transfer(dev, 0x83, usb_buf, 64, &actual, 0);
}

void AMDWraithPrismController::SendEffectChannelUpdate
    (
    unsigned char effect_channel,
    unsigned char speed,
    unsigned char direction,
    unsigned char mode,
    unsigned char brightness,
    unsigned char red,
    unsigned char green,
    unsigned char blue
    )
{
    unsigned char usb_buf[] =
    {
        0x51, 0x2C, 0x01, 0x00,
        0x05, 0xFF, 0x00, 0x01,
        0xFF, 0xFF, 0x00, 0xFF,
        0x00, 0x00, 0x00, 0x00,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF
    };

    int             actual;

    usb_buf[0x04]   = effect_channel;
    usb_buf[0x05]   = speed;
    usb_buf[0x06]   = direction ? 0x01 : 0x00;
    usb_buf[0x07]   = mode;

    usb_buf[0x09]   = brightness;

    usb_buf[0x0A]   = red;
    usb_buf[0x0B]   = green;
    usb_buf[0x0C]   = blue;

    libusb_interrupt_transfer(dev, 0x04, usb_buf, 64, &actual, 0);
    libusb_interrupt_transfer(dev, 0x83, usb_buf, 64, &actual, 0);
}

void AMDWraithPrismController::SendChannelRemap(unsigned char ring_channel, unsigned char logo_channel, unsigned char fan_channel)
{
    unsigned char usb_buf[] =
    {
        0x51, 0xA0, 0x01, 0x00,
        0x00, 0x03, 0x00, 0x00,
        0x05, 0x06, 0x07, 0x07,
        0x07, 0x07, 0x07, 0x07,
        0x07, 0x07, 0x07, 0x07,
        0x07, 0x07, 0x07, 0x07,
        0x07, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
    };

    int             actual;

    usb_buf[0x08] = logo_channel;
    usb_buf[0x09] = fan_channel;

    for(int led = 0x0A; led <= 0x18; led++)
    {
        usb_buf[led] = ring_channel;
    }

    libusb_interrupt_transfer(dev, 0x04, usb_buf, 64, &actual, 0);
    libusb_interrupt_transfer(dev, 0x83, usb_buf, 64, &actual, 0);
}