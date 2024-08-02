/*---------------------------------------------------------*\
| LianLiUniHubController.cpp                                |
|                                                           |
|   Driver for Lian Li Uni Hub                              |
|                                                           |
|   Luca Lovisa                                 20 Feb 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "LianLiUniHubController.h"

using namespace std::chrono_literals;

/*----------------------------------------------------------------------------*\
| The Uni Hub is controlled by sending control transfers to various wIndex     |
| addresses, allthough it announces some kind of hid interface. Hence it       |
| requires libusb as hidapi provides no wIndex customization.                  |
\*----------------------------------------------------------------------------*/

LianLiUniHubController::LianLiUniHubController
    (
    libusb_device*              device,
    libusb_device_descriptor*   descriptor
    )
{
    int ret;

    /*--------------------------------------------------------------------*\
    | Open the libusb device.                                              |
    \*--------------------------------------------------------------------*/
    ret = libusb_open(device, &handle);

    if(ret < 0)
    {
        return;
    }

    /*--------------------------------------------------------------------*\
    | Fill in the location string from USB port numbers.                   |
    \*--------------------------------------------------------------------*/
    uint8_t ports[7];

    ret = libusb_get_port_numbers(device, ports, sizeof(ports));

    if(ret > 0)
    {
        location = "USB: ";

        for (int i = 0; i < ret; i ++)
        {
            location += std::to_string(ports[i]);
            location.push_back(':');
        }

        location.pop_back();
    }

    /*--------------------------------------------------------------------*\
    | Fill in the serial string from the string descriptor                 |
    \*--------------------------------------------------------------------*/
    char serialStr[64];

    ret = libusb_get_string_descriptor_ascii(handle, descriptor->iSerialNumber, reinterpret_cast<unsigned char*>(serialStr), sizeof(serialStr));

    if(ret > 0)
    {
        serial = std::string(serialStr, ret);
    }

    /*--------------------------------------------------------------------*\
    | Fill in the version string by reading version from device.           |
    \*--------------------------------------------------------------------*/
    version = ReadVersion();

    /*--------------------------------------------------------------------*\
    | Create channels with their static configuration and "sane" defaults. |
    \*--------------------------------------------------------------------*/
    Channel channel1;
    channel1.index                = 0;
    channel1.anyFanCountOffset    = UNIHUB_ANY_C1_FAN_COUNT_OFFSET;
    channel1.anyFanCount          = UNIHUB_ANY_FAN_COUNT_001;
    channel1.ledActionAddress     = UNIHUB_LED_C1_ACTION_ADDRESS;
    channel1.ledCommitAddress     = UNIHUB_LED_C1_COMMIT_ADDRESS;
    channel1.ledModeAddress       = UNIHUB_LED_C1_MODE_ADDRESS;
    channel1.ledSpeedAddress      = UNIHUB_LED_C1_SPEED_ADDRESS;
    channel1.ledDirectionAddress  = UNIHUB_LED_C1_DIRECTION_ADDRESS;
    channel1.ledBrightnessAddress = UNIHUB_LED_C1_BRIGHTNESS_ADDRESS;
    channel1.ledMode              = UNIHUB_LED_MODE_RAINBOW;
    channel1.ledSpeed             = UNIHUB_LED_SPEED_100;
    channel1.ledDirection         = UNIHUB_LED_DIRECTION_LTR;
    channel1.ledBrightness        = UNIHUB_LED_BRIGHTNESS_100;
    channel1.fanHubActionAddress  = UNIHUB_FAN_C1_HUB_ACTION_ADDRESS;
    channel1.fanHubCommitAddress  = UNIHUB_FAN_C1_HUB_COMMIT_ADDRESS;
    channel1.fanPwmActionAddress  = UNIHUB_FAN_C1_PWM_ACTION_ADDRESS;
    channel1.fanPwmCommitAddress  = UNIHUB_FAN_C1_PWM_COMMIT_ADDRESS;
    channel1.fanRpmActionAddress  = UNIHUB_FAN_C1_RPM_ACTION_ADDRESS;
    channel1.fanSpeed             = UNIHUB_FAN_SPEED_QUIET;
    channels[0] = channel1;

    Channel channel2;
    channel2.index                = 1;
    channel2.anyFanCountOffset    = UNIHUB_ANY_C2_FAN_COUNT_OFFSET;
    channel2.anyFanCount          = UNIHUB_ANY_FAN_COUNT_001;
    channel2.ledActionAddress     = UNIHUB_LED_C2_ACTION_ADDRESS;
    channel2.ledCommitAddress     = UNIHUB_LED_C2_COMMIT_ADDRESS;
    channel2.ledModeAddress       = UNIHUB_LED_C2_MODE_ADDRESS;
    channel2.ledSpeedAddress      = UNIHUB_LED_C2_SPEED_ADDRESS;
    channel2.ledDirectionAddress  = UNIHUB_LED_C2_DIRECTION_ADDRESS;
    channel2.ledBrightnessAddress = UNIHUB_LED_C2_BRIGHTNESS_ADDRESS;
    channel2.ledMode              = UNIHUB_LED_MODE_RAINBOW;
    channel2.ledSpeed             = UNIHUB_LED_SPEED_100;
    channel2.ledDirection         = UNIHUB_LED_DIRECTION_LTR;
    channel2.ledBrightness        = UNIHUB_LED_BRIGHTNESS_100;
    channel2.fanHubActionAddress  = UNIHUB_FAN_C2_HUB_ACTION_ADDRESS;
    channel2.fanHubCommitAddress  = UNIHUB_FAN_C2_HUB_COMMIT_ADDRESS;
    channel2.fanPwmActionAddress  = UNIHUB_FAN_C2_PWM_ACTION_ADDRESS;
    channel2.fanPwmCommitAddress  = UNIHUB_FAN_C2_PWM_COMMIT_ADDRESS;
    channel2.fanRpmActionAddress  = UNIHUB_FAN_C2_RPM_ACTION_ADDRESS;
    channel2.fanSpeed             = UNIHUB_FAN_SPEED_QUIET;
    channels[1] = channel2;

    Channel channel3;
    channel3.index                = 2;
    channel3.anyFanCountOffset    = UNIHUB_ANY_C3_FAN_COUNT_OFFSET;
    channel3.anyFanCount          = UNIHUB_ANY_FAN_COUNT_001;
    channel3.ledActionAddress     = UNIHUB_LED_C3_ACTION_ADDRESS;
    channel3.ledCommitAddress     = UNIHUB_LED_C3_COMMIT_ADDRESS;
    channel3.ledModeAddress       = UNIHUB_LED_C3_MODE_ADDRESS;
    channel3.ledSpeedAddress      = UNIHUB_LED_C3_SPEED_ADDRESS;
    channel3.ledDirectionAddress  = UNIHUB_LED_C3_DIRECTION_ADDRESS;
    channel3.ledBrightnessAddress = UNIHUB_LED_C3_BRIGHTNESS_ADDRESS;
    channel3.ledMode              = UNIHUB_LED_MODE_RAINBOW;
    channel3.ledSpeed             = UNIHUB_LED_SPEED_100;
    channel3.ledDirection         = UNIHUB_LED_DIRECTION_LTR;
    channel3.ledBrightness        = UNIHUB_LED_BRIGHTNESS_100;
    channel3.fanHubActionAddress  = UNIHUB_FAN_C3_HUB_ACTION_ADDRESS;
    channel3.fanHubCommitAddress  = UNIHUB_FAN_C3_HUB_COMMIT_ADDRESS;
    channel3.fanPwmActionAddress  = UNIHUB_FAN_C3_PWM_ACTION_ADDRESS;
    channel3.fanPwmCommitAddress  = UNIHUB_FAN_C3_PWM_COMMIT_ADDRESS;
    channel3.fanRpmActionAddress  = UNIHUB_FAN_C3_RPM_ACTION_ADDRESS;
    channel3.fanSpeed             = UNIHUB_FAN_SPEED_QUIET;
    channels[2] = channel3;

    Channel channel4;
    channel4.index                = 3;
    channel4.anyFanCountOffset    = UNIHUB_ANY_C4_FAN_COUNT_OFFSET;
    channel4.anyFanCount          = UNIHUB_ANY_FAN_COUNT_001;
    channel4.ledActionAddress     = UNIHUB_LED_C4_ACTION_ADDRESS;
    channel4.ledCommitAddress     = UNIHUB_LED_C4_COMMIT_ADDRESS;
    channel4.ledModeAddress       = UNIHUB_LED_C4_MODE_ADDRESS;
    channel4.ledSpeedAddress      = UNIHUB_LED_C4_SPEED_ADDRESS;
    channel4.ledDirectionAddress  = UNIHUB_LED_C4_DIRECTION_ADDRESS;
    channel4.ledBrightnessAddress = UNIHUB_LED_C4_BRIGHTNESS_ADDRESS;
    channel4.ledMode              = UNIHUB_LED_MODE_RAINBOW;
    channel4.ledSpeed             = UNIHUB_LED_SPEED_100;
    channel4.ledDirection         = UNIHUB_LED_DIRECTION_LTR;
    channel4.ledBrightness        = UNIHUB_LED_BRIGHTNESS_100;
    channel4.fanHubActionAddress  = UNIHUB_FAN_C4_HUB_ACTION_ADDRESS;
    channel4.fanHubCommitAddress  = UNIHUB_FAN_C4_HUB_COMMIT_ADDRESS;
    channel4.fanPwmActionAddress  = UNIHUB_FAN_C4_PWM_ACTION_ADDRESS;
    channel4.fanPwmCommitAddress  = UNIHUB_FAN_C4_PWM_COMMIT_ADDRESS;
    channel4.fanRpmActionAddress  = UNIHUB_FAN_C4_RPM_ACTION_ADDRESS;
    channel4.fanSpeed             = UNIHUB_FAN_SPEED_QUIET;
    channels[3] = channel4;
}

LianLiUniHubController::~LianLiUniHubController()
{
    CloseLibusb();
}

std::string LianLiUniHubController::GetVersion()
{
    return version;
}

std::string LianLiUniHubController::GetLocation()
{
    return location;
}

std::string LianLiUniHubController::GetSerial()
{
    return serial;
}

void LianLiUniHubController::SetAnyFanCount(size_t channel, uint8_t count)
{
    /*-------------------------------------*\
    | Check for invalid channel             |
    \*-------------------------------------*/
    if(channel >= UNIHUB_CHANNEL_COUNT)
    {
        return;
    }

    channels[channel].anyFanCount = count;
}

void LianLiUniHubController::SetLedColors(size_t channel, RGBColor* colors, size_t count)
{
    /*-------------------------------------*\
    | Check for invalid channel             |
    \*-------------------------------------*/
    if(channel >= UNIHUB_CHANNEL_COUNT)
    {
        return;
    }

    /*-------------------------------------*\
    | Check for invalid count               |
    \*-------------------------------------*/
    if(count > UNIHUB_CHANLED_COUNT)
    {
        count = UNIHUB_CHANLED_COUNT;
    }

    size_t i = 0;
    for(; i < count; i++)
    {
        channels[channel].colors[i].r = RGBGetRValue(colors[i]);
        channels[channel].colors[i].b = RGBGetBValue(colors[i]);
        channels[channel].colors[i].g = RGBGetGValue(colors[i]);
    }

    /* Set all remaining leds to black */
    for(; i < UNIHUB_CHANLED_COUNT; i++)
    {
        channels[channel].colors[i].r = 0x00;
        channels[channel].colors[i].b = 0x00;
        channels[channel].colors[i].g = 0x00;
    }
}

void LianLiUniHubController::SetLedMode(size_t channel, uint8_t mode)
{
    /*-------------------------------------*\
    | Check for invalid channel             |
    \*-------------------------------------*/
    if(channel >= UNIHUB_CHANNEL_COUNT)
    {
        return;
    }

    channels[channel].ledMode = mode;
}

void LianLiUniHubController::SetLedSpeed(size_t channel, uint8_t speed)
{
    /*-------------------------------------*\
    | Check for invalid channel             |
    \*-------------------------------------*/
    if(channel >= UNIHUB_CHANNEL_COUNT)
    {
        return;
    }

    channels[channel].ledSpeed = speed;
}

void LianLiUniHubController::SetLedDirection(size_t channel, uint8_t direction)
{
    /*-------------------------------------*\
    | Check for invalid channel             |
    \*-------------------------------------*/
    if(channel >= UNIHUB_CHANNEL_COUNT)
    {
        return;
    }

    channels[channel].ledDirection = direction;
}

void LianLiUniHubController::SetLedBrightness(size_t channel, uint8_t brightness)
{
    /*-------------------------------------*\
    | Check for invalid channel             |
    \*-------------------------------------*/
    if(channel >= UNIHUB_CHANNEL_COUNT)
    {
        return;
    }

    channels[channel].ledBrightness = brightness;
}

uint16_t LianLiUniHubController::GetFanSpeed(size_t channel)
{
    /*-------------------------------------*\
    | Check for invalid channel             |
    \*-------------------------------------*/
    if(channel >= UNIHUB_CHANNEL_COUNT)
    {
        return 0;
    }

    return channels[channel].fanSpeed;
}

void LianLiUniHubController::SetFanSpeed(size_t channel, uint16_t speed)
{
    /*-------------------------------------*\
    | Check for invalid channel             |
    \*-------------------------------------*/
    if(channel >= UNIHUB_CHANNEL_COUNT)
    {
        return;
    }

    channels[channel].fanSpeed = speed;
}

void LianLiUniHubController::EnableRgbhMode()
{
    rgbhModeEnabled = true;
}

void LianLiUniHubController::DisableRgbhMode()
{
    rgbhModeEnabled = false;
}

void LianLiUniHubController::EnableSyncMode()
{
    syncModeEnabled = true;
}

void LianLiUniHubController::DisableSyncMode()
{
    syncModeEnabled = false;
}

/*----------------------------------------------------------------------------*\
| The Uni Hub is a PWM and LED controller designed specifically for the Lian   |
| Li Uni Fans. It can control them by itself using the built-in effect engine  |
| can also be connected to the mainboard via 4-pin PWM and 3-pin RGB cables    |
| and forward these signals. The protocol implementation below was build as    |
| close a possible to the Lian Li L-Connect software.                          |
|                                                                              |
| The commands to control the fan speeds and to switch between controller and  |
| mainboard control is already included, but currently deactivated as OpenRGB  |
| had no fan control module or controller specific configuration at the time   |
| of writing.                                                                  |
\*----------------------------------------------------------------------------*/
void LianLiUniHubController::Synchronize()
{
    /*---------------------------------------------------------------------*\
    | Configure common settings.                                            |
    \*---------------------------------------------------------------------*/

    /*---------------------------------------------------------------------*\
    | Still unsure about this. Probably some sort of configuration          |
    | initialization                                                        |
    \*---------------------------------------------------------------------*/
    uint8_t config_initialization[1] = { 0x34 };

    SendConfig(UNIHUB_ACTION_ADDRESS, config_initialization, sizeof(config_initialization));
    SendCommit(UNIHUB_COMMIT_ADDRESS);

    for(const Channel& channel : channels)
    {
        /*-------------------------------------*\
        | The Uni Hub doesn't know zero fans    |
        \*-------------------------------------*/
        uint8_t anyFanCount = channel.anyFanCount;

        if(anyFanCount == UNIHUB_ANY_FAN_COUNT_000)
        {
            anyFanCount =  UNIHUB_ANY_FAN_COUNT_001;
        }

        /*-------------------------------------*\
        | Configure the physical fan count      |
        \*-------------------------------------*/
        uint8_t config_fan_count[2] = { 0x32, (uint8_t)(channel.anyFanCountOffset | anyFanCount) };

        SendConfig(UNIHUB_ACTION_ADDRESS, config_fan_count, sizeof(config_fan_count));
        SendCommit(UNIHUB_COMMIT_ADDRESS);
    }

    /*--------------------------------------------------------------------*\
    | Configure channels for sync effects                                  |
    \*--------------------------------------------------------------------*/
    if(syncModeEnabled)
    {
        uint8_t config_sync[6];
        uint8_t config_sync_index = 0;

        config_sync[config_sync_index++] = 0x33;

        for(const Channel& channel : channels)
        {
            if(channel.anyFanCount != UNIHUB_ANY_FAN_COUNT_000)
            {
                config_sync[config_sync_index++] = channel.index;
            }
        }

        config_sync[config_sync_index++] = 0x08;

        SendConfig(UNIHUB_ACTION_ADDRESS, config_sync, config_sync_index);
        SendCommit(UNIHUB_COMMIT_ADDRESS);
    }

    /*--------------------------------------------------------------------*\
    | Configure led settings.                                              |
    \*--------------------------------------------------------------------*/
    for(const Channel& channel : channels)
    {
        if(channel.anyFanCount != UNIHUB_ANY_FAN_COUNT_000)
        {
            /*-----------------------------*\
            | Configure led colors          |
            \*-----------------------------*/
            uint8_t config_colors[192];
            memcpy(config_colors, channel.colors, sizeof(config_colors));

            /*-----------------------------*\
            | No idea what this does ...    |
            \*-----------------------------*/
            if (syncModeEnabled)
            {
                config_colors[0x06] = 0x66;
                config_colors[0x07] = 0x33;
                config_colors[0x08] = 0xCC;

                memset(config_colors + 0x09, 0x00, sizeof(config_colors) - 0x09);
            }

            SendConfig(channel.ledActionAddress, config_colors, sizeof(config_colors));

            /*-----------------------------*\
            | Configure led mode            |
            \*-----------------------------*/
            uint8_t config_mode[1] = { channel.ledMode };

            SendConfig(channel.ledModeAddress, config_mode, sizeof(config_mode));
        }
        /*-----------------------------------------------------------------*\
        | The Uni Hub doesn't know zero fans so we set them to black        |
        \*-----------------------------------------------------------------*/
        else
        {
            /*-----------------------------*\
            | Configure led colors          |
            \*-----------------------------*/
            uint8_t config_colors[192];
            memset(config_colors, 0x00, sizeof(config_colors));

            SendConfig(channel.ledActionAddress, config_colors, sizeof(config_colors));

            /*-----------------------------*\
            | Configure led mode            |
            \*-----------------------------*/
            uint8_t config_mode[1] = { UNIHUB_LED_MODE_STATIC_COLOR };

            SendConfig(channel.ledModeAddress, config_mode, sizeof(config_mode));
        }

        /*---------------------------------*\
        | Configure led speed               |
        \*---------------------------------*/
        uint8_t config_speed[1] = { channel.ledSpeed };

        SendConfig(channel.ledSpeedAddress, config_speed, sizeof(config_speed));

        /*---------------------------------*\
        | Configure led direction           |
        \*---------------------------------*/
        uint8_t config_direction[1] = { channel.ledDirection };

        SendConfig(channel.ledDirectionAddress, config_direction, sizeof(config_direction));

        /*---------------------------------*\
        | Configure led brightness          |
        \*---------------------------------*/
        uint8_t config_brightness[1] = { channel.ledBrightness };

        SendConfig(channel.ledBrightnessAddress, config_brightness, sizeof(config_brightness));

        /*---------------------------------*\
        | Commit only once for all led      |
        | settings                          |
        \*---------------------------------*/
        SendCommit(channel.ledCommitAddress);
    }

    /*--------------------------------------------------------------------*\
    | Configure fan settings. Comment out until enabling fan control       |
    \*--------------------------------------------------------------------*/
//    uint8_t control = 0;

      /*-------------------------------------*\
      | Configure fan settings                |
      \*-------------------------------------*/
//    for(const Channel& channel : channels)
//    {
//        if(channel.fanSpeed == UNIHUB_FAN_SPEED_PWM)
//        {
              /*-----------------------------*\
              | Configure the fan to pwm      |
              | control                       |
              \*-----------------------------*/
//            uint8_t config_pwm[1] = { 0x00 };

//            control |= (0x01 << channel.index);

//            SendConfig(channel.fanPwmActionAddress, config_pwm, sizeof(config_pwm));
//            SendCommit(channel.fanPwmCommitAddress);
//        }
//        else
//        {
              /*-----------------------------*\
              | Configure the fan to hub      |
              | control and set speed         |
              \*-----------------------------*/
//            uint8_t config_hub[2] = { (uint8_t)(channel.fanSpeed >> 0x08), (uint8_t)(channel.fanSpeed &  0xFF) };

//            SendConfig(channel.fanHubActionAddress, config_hub, sizeof(config_hub));
//            SendCommit(channel.fanHubCommitAddress);
//        }
//    }

      /*-------------------------------------*\
      | Configure fan control modes           |
      \*-------------------------------------*/
//    uint8_t config_fan_mode[2] = { 0x31, (uint8_t)(0xF0 | control) };

//    SendConfig(UNIHUB_ACTION_ADDRESS, config_fan_mode, sizeof(config_fan_mode));
//    SendCommit(UNIHUB_COMMIT_ADDRESS);

    /*--------------------------------------------------------------------*\
    | Configure led settings.                                              |
    \*--------------------------------------------------------------------*/
    if(rgbhModeEnabled)
    {
        /*-------------------------------------*\
        | Configure the leds to hdr control.    |
        \*-------------------------------------*/
        uint8_t config_hdr[2] = { 0x30, 0x01 };

        SendConfig(UNIHUB_ACTION_ADDRESS, config_hdr, sizeof(config_hdr));
        SendCommit(UNIHUB_COMMIT_ADDRESS);
    }
    else
    {
        /*-------------------------------------*\
        | Configure the leds to hub control     |
        \*-------------------------------------*/
        uint8_t config_hub[2] = { 0x30, 0x00 };

        SendConfig(UNIHUB_ACTION_ADDRESS, config_hub, sizeof(config_hub));
        SendCommit(UNIHUB_COMMIT_ADDRESS);
    }
}

uint16_t LianLiUniHubController::ReadFanSpeed(size_t channel)
{
    /*-------------------------------------*\
    | Check for invalid handle              |
    \*-------------------------------------*/
    if(handle == nullptr)
    {
        return(0);
    }

    /*-------------------------------------*\
    | Check for invalid channel             |
    \*-------------------------------------*/
    if(channel > UNIHUB_CHANNEL_COUNT)
    {
        return(0);
    }

    uint8_t buffer[2];
    uint8_t length = sizeof(buffer);

    uint16_t wIndex = channels[channel].fanRpmActionAddress;

    /*-------------------------------------*\
    | Send packet                           |
    \*-------------------------------------*/
    size_t ret = libusb_control_transfer(handle,    /* dev_handle       */
                                         0xC0,      /* bmRequestType    */
                                         0x81,      /* bRequest         */
                                         0x00,      /* wValue           */
                                         wIndex,    /* wIndex           */
                                         buffer,    /* data             */
                                         length,    /* wLength          */
                                         1000);     /* timeout          */

    /*-------------------------------------*\
    | Check for communication error         |
    \*-------------------------------------*/
    if(ret != length)
    {
        return(0);
    }

    return(*(uint16_t*)buffer);
}

void LianLiUniHubController::CloseLibusb()
{
    if (handle != nullptr)
    {
        libusb_close(handle);
        handle = nullptr;
    }
}

std::string LianLiUniHubController::ReadVersion()
{
    /*-------------------------------------*\
    | Check for invalid handle              |
    \*-------------------------------------*/
    if(handle == nullptr)
    {
        return("");
    }

    uint8_t buffer[5];
    uint8_t length = sizeof(buffer);

    /*-------------------------------------*\
    | Send packet                           |
    \*-------------------------------------*/
    size_t ret = libusb_control_transfer(handle,    /* dev_handle       */
                                         0xC0,      /* bmRequestType    */
                                         0x81,      /* bRequest         */
                                         0x00,      /* wValue           */
                                         0xB500,    /* wIndex           */
                                         buffer,    /* data             */
                                         length,    /* wLength          */
                                         1000);     /* timeout          */

    /*-------------------------------------*\
    | Check for communication error         |
    \*-------------------------------------*/
    if(ret != length)
    {
        return("");
    }

    /*-------------------------------------*\
    | Format version string                 |
    \*-------------------------------------*/
    char version[15];
    int  vlength = std::snprintf(version, sizeof(version), "%x.%x.%x.%x.%x", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]);

    return(std::string(version, vlength));
}

void LianLiUniHubController::SendConfig(uint16_t wIndex, uint8_t  *config, size_t length)
{
    /*-------------------------------------*\
    | Check for invalid handle              |
    \*-------------------------------------*/
    if(handle == nullptr)
    {
        return;
    }

    /*-------------------------------------*\
    | Send packet                           |
    \*-------------------------------------*/
    size_t ret = libusb_control_transfer(handle,            /* dev_handle       */
                                         0x40,              /* bmRequestType    */
                                         0x80,              /* bRequest         */
                                         0x00,              /* wValue           */
                                         wIndex,            /* wIndex           */
                                         config,            /* data             */
                                         (uint16_t)length,  /* wLength          */
                                         1000);             /* timeout          */

    /*-------------------------------------*\
    | Check for communication error         |
    \*-------------------------------------*/
    if(ret != length)
    {
        return;
    }
}

void LianLiUniHubController::SendCommit(uint16_t wIndex)
{
    /*-------------------------------------*\
    | Set up config packet                  |
    \*-------------------------------------*/
    uint8_t config[1] = { 0x01 };

    /*-------------------------------------*\
    | Send packet                           |
    \*-------------------------------------*/
    SendConfig(wIndex, config, sizeof(config));

    std::this_thread::sleep_for(5ms);
}
