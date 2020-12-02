/*-----------------------------------------*\
|  MSIMysticLightController.cpp             |
|                                           |
|  Driver for MSI Mystic Light USB          |
|  lighting controller                      |
|                                           |
|  T-bond 3/4/2020                          |
\*-----------------------------------------*/

#include "MSIMysticLightController.h"
#include <algorithm>
#include <array>
#include <bitset>

MSIMysticLightController::MSIMysticLightController(hid_device* handle, const char *path)
{
    dev = handle;

    if(dev)
    {
        location = path;

        ReadName();
        ReadSerial();
        ReadFwVersion();
        ReadSettings();
    }
}

MSIMysticLightController::~MSIMysticLightController()
{
    if(dev)
    {
        hid_close(dev);
    }
}

unsigned int MSIMysticLightController::GetZoneMinLedCount
    (
    MSI_ZONE            /*zone*/
    )
{
    return 1;
}

unsigned int MSIMysticLightController::GetZoneMaxLedCount
    (
    MSI_ZONE            zone
    )
{
    switch(zone)
    {
    case MSI_ZONE_J_RAINBOW_1:
    case MSI_ZONE_J_RAINBOW_2:
    case MSI_ZONE_J_CORSAIR:
        return 4; // TODO: It can be different by zone and by mobo
    default:
        return 1;
    }
}

unsigned int MSIMysticLightController::GetZoneLedCount
    (
    MSI_ZONE            zone
    )
{
    RainbowZoneData *requestedZone = GetRainbowZoneData(zone);

    if (!requestedZone)
    {
        return GetZoneMaxLedCount(zone);
    }

    return requestedZone->cycle_or_led_num;
}

void MSIMysticLightController::SetZoneLedCount
    (
    MSI_ZONE            zone,
    unsigned int        led_count
    )
{
    RainbowZoneData *requestedZone = GetRainbowZoneData(zone);

    if (!requestedZone)
    {
        return;
    }

    led_count = std::min(GetZoneMaxLedCount(zone), std::max(GetZoneMinLedCount(zone), led_count));
    requestedZone->cycle_or_led_num = led_count;
}

void MSIMysticLightController::SetMode
    (
    MSI_ZONE            zone,
    MSI_MODE            mode,
    MSI_SPEED           speed,
    MSI_BRIGHTNESS      brightness,
    bool                rainbow_color
    )
{
    ZoneData* zoneData = GetZoneData(zone);
    if(!zoneData)
    {
        return;
    }

    zoneData->effect = mode;
    zoneData->speedAndBrightnessFlags = (zoneData->speedAndBrightnessFlags & 128u) | brightness << 2u | speed;
    zoneData->colorFlags = BitSet(zoneData->colorFlags, !rainbow_color, 7u);
}

std::string MSIMysticLightController::GetDeviceName()
{
    return name;
}

std::string MSIMysticLightController::GetFWVersion()
{
    return std::string("AP/LD ").append(version_APROM).append(" / ").append(version_LDROM);
}

std::string MSIMysticLightController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string MSIMysticLightController::GetSerial()
{
    return chip_id;
}

bool MSIMysticLightController::ReadSettings()
{
    /*-----------------------------------------------------*\
    | Read packet from hardware, return true if successful  |
    \*-----------------------------------------------------*/
    return(hid_get_feature_report(dev, (unsigned char *)&data, sizeof(data)) == sizeof data);
}

bool MSIMysticLightController::Update()
{
    /*-----------------------------------------------------*\
    | Send packet to hardware, return true if successful    |
    \*-----------------------------------------------------*/
    return(hid_send_feature_report(dev, (unsigned char *)&data, sizeof(data)) == sizeof data);
}

void MSIMysticLightController::SaveOnUpdate(bool save)
{
    data.save_data = save;
}

void MSIMysticLightController::SetZoneColor
    (
    MSI_ZONE            zone,
    unsigned char       red1,
    unsigned char       grn1,
    unsigned char       blu1,
    unsigned char       red2,
    unsigned char       grn2,
    unsigned char       blu2
    )
{
    ZoneData* zoneData = GetZoneData(zone);

    if(!zoneData)
    {
        return;
    }

    zoneData->color.R = red1;
    zoneData->color.G = grn1;
    zoneData->color.B = blu1;

    zoneData->color2.R = red2;
    zoneData->color2.G = grn2;
    zoneData->color2.B = blu2;
}

std::pair<Color, Color> MSIMysticLightController::GetZoneColor
    (
    MSI_ZONE            zone
    )
{
    ZoneData *zoneData = GetZoneData(zone);

    if (!zoneData)
    {
        return std::make_pair(Color{}, Color{});
    }

    return std::make_pair(Color{
                              zoneData->color.R,
                              zoneData->color.G,
                              zoneData->color.B},
                          Color{zoneData->color2.R, zoneData->color2.G, zoneData->color2.B});
}

ZoneData *MSIMysticLightController::GetZoneData
    (
    MSI_ZONE            zone
    )
{
    switch(zone)
    {
    case MSI_ZONE_J_RGB_1:
        return &data.j_rgb_1;
    case MSI_ZONE_J_RGB_2:
        return &data.j_rgb_2;
    case MSI_ZONE_J_RAINBOW_1:
        return &data.j_rainbow_1;
    case MSI_ZONE_J_RAINBOW_2:
        return &data.j_rainbow_2;
    case MSI_ZONE_J_PIPE_1:
        return &data.j_pipe_1;
    case MSI_ZONE_J_PIPE_2:
        return &data.j_pipe_2;
    case MSI_ZONE_ON_BOARD_LED:
        return &data.on_board_led;
    case MSI_ZONE_ON_BOARD_LED_1:
        return &data.on_board_led_1;
    case MSI_ZONE_ON_BOARD_LED_2:
        return &data.on_board_led_2;
    case MSI_ZONE_ON_BOARD_LED_3:
        return &data.on_board_led_3;
    case MSI_ZONE_ON_BOARD_LED_4:
        return &data.on_board_led_4;
    case MSI_ZONE_ON_BOARD_LED_5:
        return &data.on_board_led_5;
    case MSI_ZONE_ON_BOARD_LED_6:
        return &data.on_board_led_6;
    case MSI_ZONE_ON_BOARD_LED_7:
        return &data.on_board_led_7;
    case MSI_ZONE_ON_BOARD_LED_8:
        return &data.on_board_led_8;
    case MSI_ZONE_ON_BOARD_LED_9:
        return &data.on_board_led_9;
    case MSI_ZONE_J_CORSAIR_OUTERLL120:
        return &data.j_corsair_outerll120;
    default:
    case MSI_ZONE_J_CORSAIR:
        break;
    }

    return nullptr;
}

RainbowZoneData *MSIMysticLightController::GetRainbowZoneData
    (
    MSI_ZONE            zone
    )
{
    switch(zone)
    {
    case MSI_ZONE_J_RAINBOW_1:
        return &data.j_rainbow_1;
    case MSI_ZONE_J_RAINBOW_2:
        return &data.j_rainbow_2;
    case MSI_ZONE_J_CORSAIR:
    default:
        return nullptr;
    }
}

bool MSIMysticLightController::ReadFwVersion()
{
    unsigned char request[64];
    unsigned char response[64];
    int ret_val = 64;

    /*-----------------------------------------------------*\
    | First read the APROM                                  |
    | Checksum also available at report ID 180, with MSB    |
    | stored at index 0x08 and LSB at 0x09                  |
    \*-----------------------------------------------------*/

    /*-----------------------------------------------------*\
    | Zero out buffers                                      |
    \*-----------------------------------------------------*/
    memset(request, 0x00, sizeof(request));
    memset(response, 0x00, sizeof(response));

    /*-----------------------------------------------------*\
    | Set up APROM Firmware Version Request packet          |
    \*-----------------------------------------------------*/
    request[0x00]   = 0x01;
    request[0x01]   = 0xB0;

    /*-----------------------------------------------------*\
    | Fill request from 0x02 to 0x61 with 0xCC              |
    \*-----------------------------------------------------*/
    memset(&request[0x02], 0xCC, sizeof(request) - 2);

    /*-----------------------------------------------------*\
    | Send request and receive response packets             |
    \*-----------------------------------------------------*/
    ret_val &= hid_write(dev, request, 64);
    ret_val &= hid_read(dev, response, 64);

    /*-----------------------------------------------------*\
    | Extract high and low values from response             |
    \*-----------------------------------------------------*/
    unsigned char highValue = response[2] >> 4;
    unsigned char lowValue  = response[2] & 0x0F;

    /*-----------------------------------------------------*\
    | Build firmware string <high>.<low>                    |
    \*-----------------------------------------------------*/
    version_APROM = std::to_string(static_cast<int>(highValue)).append(".").append(std::to_string(static_cast<int>(lowValue)));

    /*-----------------------------------------------------*\
    | First read the LDROM                                  |
    | Checksum also available at report ID 184, with MSB    |
    | stored at index 0x08 and LSB at 0x09                  |
    \*-----------------------------------------------------*/

    /*-----------------------------------------------------*\
    | Set up LDROM Firmware Version Request packet          |
    \*-----------------------------------------------------*/
    request[0x00]   = 0x01;
    request[0x01]   = 0xB6;

    /*-----------------------------------------------------*\
    | Send request and receive response packets             |
    \*-----------------------------------------------------*/
    ret_val &= hid_write(dev, request, 64);
    ret_val &= hid_read(dev, response, 64);

    /*-----------------------------------------------------*\
    | Extract high and low values from response             |
    \*-----------------------------------------------------*/
    highValue = response[2] >> 4;
    lowValue  = response[2] & 0x0F;

    /*-----------------------------------------------------*\
    | Build firmware string <high>.<low>                    |
    \*-----------------------------------------------------*/
    version_LDROM = std::to_string(static_cast<int>(highValue)).append(".").append(std::to_string(static_cast<int>(lowValue)));

    /*-----------------------------------------------------*\
    | If return value is zero it means an HID transfer      |
    | failed                                                |
    \*-----------------------------------------------------*/
    return(ret_val > 0);
}

void MSIMysticLightController::ReadSerial()
{
    wchar_t serial[256];

    /*-----------------------------------------------------*\
    | Get the serial number string from HID                 |
    \*-----------------------------------------------------*/
    hid_get_serial_number_string(dev, serial, 256);

    /*-----------------------------------------------------*\
    | Convert wchar_t into std::wstring into std::string    |
    \*-----------------------------------------------------*/
    std::wstring wserial = std::wstring(serial);
    chip_id = std::string(wserial.begin(), wserial.end());
}

void MSIMysticLightController::ReadName()
{
    wchar_t tname[256];

    /*-----------------------------------------------------*\
    | Get the manufacturer string from HID                  |
    \*-----------------------------------------------------*/
    hid_get_manufacturer_string(dev, tname, 256);

    /*-----------------------------------------------------*\
    | Convert wchar_t into std::wstring into std::string    |
    \*-----------------------------------------------------*/
    std::wstring wname = std::wstring(tname);
    name = std::string(wname.begin(), wname.end());

    /*-----------------------------------------------------*\
    | Get the product string from HID                       |
    \*-----------------------------------------------------*/
    hid_get_product_string(dev, tname, 256);

    /*-----------------------------------------------------*\
    | Append the product string to the manufacturer string  |
    \*-----------------------------------------------------*/
    wname = std::wstring(tname);
    name.append(" ").append(std::string(wname.begin(), wname.end()));
}

void MSIMysticLightController::SetDeviceSettings
    (
    bool            is_fan,
    MSI_FAN_TYPE    fan_type,
    unsigned char   corsair_device_quantity,
    bool            is_LL120Outer_individual
    )
{
    /*-----------------------------------------------------*\
    | If is_fan is false, it is an LED strip                |
    \*-----------------------------------------------------*/
    CorsairZoneData &settingsZone   = data.j_corsair;
    settingsZone.fan_flags          = (settingsZone.fan_flags & 0x80) | fan_type << 1 | is_fan;
    settingsZone.corsair_quantity   = corsair_device_quantity << 2;
    settingsZone.is_individual      = BitSet(settingsZone.is_individual, is_LL120Outer_individual, 0);
}

bool MSIMysticLightController::SetVolume
    (
    unsigned char       main,
    unsigned char       left,
    unsigned char       right
    )
{
    unsigned char packet[64];
    std::fill_n(packet, sizeof packet, 204u);

    if(main > 100u)
    {
        main = 100u;
    }

    if(left > 100u)
    {
        left = 100u;
    }

    if(right > 100u)
    {
        right = 100u;
    }

    packet[0] = 1u;
    packet[1] = 192u;
    packet[3] = main;
    packet[4] = left;
    packet[5] = right;

    return hid_write(dev, packet, sizeof packet);
}

void MSIMysticLightController::SetBoardSyncSettings
    (
    bool                onboard_sync,
    bool                combine_JRGB,
    bool                combine_JPIPE1,
    bool                combine_JPIPE2,
    bool                combine_JRAINBOW1,
    bool                combine_JRAINBOW2,
    bool                combine_crossair
    )
{
    ZoneData &syncZone = data.on_board_led;

    /*-----------------------------------------------------*\
    | Set sync flags for on-board LED zone                  |
    \*-----------------------------------------------------*/
    syncZone.colorFlags = BitSet(syncZone.colorFlags, onboard_sync,      0);
    syncZone.colorFlags = BitSet(syncZone.colorFlags, combine_JRAINBOW1, 1);
    syncZone.colorFlags = BitSet(syncZone.colorFlags, combine_JRAINBOW2, 2);
    syncZone.colorFlags = BitSet(syncZone.colorFlags, combine_crossair,  3);
    syncZone.colorFlags = BitSet(syncZone.colorFlags, combine_JPIPE1,    4);
    syncZone.colorFlags = BitSet(syncZone.colorFlags, combine_JPIPE2,    5);

    syncZone.speedAndBrightnessFlags = BitSet(syncZone.speedAndBrightnessFlags, combine_JRGB, 7);
}

void MSIMysticLightController::GetMode
    (
    MSI_ZONE            zone,
    MSI_MODE            &mode,
    MSI_SPEED           &speed,
    MSI_BRIGHTNESS      &brightness,
    bool        &rainbow_color
    ) 
{
    /*-----------------------------------------------------*\
    | Get data for given zone                               |
    \*-----------------------------------------------------*/
    ZoneData *zoneData = GetZoneData(zone);

    /*-----------------------------------------------------*\
    | Return if zone is invalid                             |
    \*-----------------------------------------------------*/
    if (!zoneData)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Update pointers with data                             |
    \*-----------------------------------------------------*/
    mode            = (MSI_MODE)(zoneData->effect);
    speed           = (MSI_SPEED)(zoneData->speedAndBrightnessFlags & 0x03);
    brightness      = (MSI_BRIGHTNESS)((zoneData->speedAndBrightnessFlags >> 2) & 0x1F);
    rainbow_color   = (zoneData->colorFlags & 0x80) >> 7;
}

unsigned char MSIMysticLightController::BitSet
    (
    unsigned char       value,
    bool                bit,
    unsigned int        position
    )
{
    return static_cast<unsigned char>(std::bitset<8>(value).set(position, bit).to_ulong());
}

void MSIMysticLightController::SetCycleCount
    (
    MSI_ZONE            zone,
    unsigned char       cycle_num
    )
{
    RainbowZoneData *requestedZone = GetRainbowZoneData(zone);

    if (!requestedZone)
    {
        return;
    }

    requestedZone->cycle_or_led_num = cycle_num;
}

unsigned char MSIMysticLightController::GetCycleCount
    (
    MSI_ZONE            zone
    )
{
    RainbowZoneData *requestedZone = GetRainbowZoneData(zone);

    if (!requestedZone)
    {
        return 0;
    }

    return requestedZone->cycle_or_led_num;
}

void MSIMysticLightController::GetBoardSyncSettings
    (
    bool                &onboard_sync,
    bool                &combine_JRGB,
    bool                &combine_JPIPE1,
    bool                &combine_JPIPE2,
    bool                &combine_JRAINBOW1,
    bool                &combine_JRAINBOW2,
    bool                &combine_crossair
    )
{
    ZoneData &syncZone = data.on_board_led;

    /*-----------------------------------------------------*\
    | Get sync flags for on-board LED zone                  |
    \*-----------------------------------------------------*/
    onboard_sync        = (syncZone.colorFlags >> 0) & 0x01;
    combine_JRAINBOW1   = (syncZone.colorFlags >> 1) & 0x01;
    combine_JRAINBOW2   = (syncZone.colorFlags >> 2) & 0x01;
    combine_crossair    = (syncZone.colorFlags >> 3) & 0x01;
    combine_JPIPE1      = (syncZone.colorFlags >> 4) & 0x01;
    combine_JPIPE2      = (syncZone.colorFlags >> 5) & 0x01;

    combine_JRGB        = (syncZone.speedAndBrightnessFlags & 0x80) >> 7;
}

void MSIMysticLightController::GetDeviceSettings
    (
    bool                &stripe_or_fan,
    MSI_FAN_TYPE        &fan_type,
    unsigned char       &corsair_device_quantity,
    bool                &is_LL120Outer_individual
    )
{
    CorsairZoneData &settingsZone   = data.j_corsair;
    stripe_or_fan                   = settingsZone.fan_flags & 0x01;
    fan_type                        = (MSI_FAN_TYPE )((settingsZone.fan_flags & 14u) >> 1);
    corsair_device_quantity         = (settingsZone.corsair_quantity & 0xFC) >> 2;
    is_LL120Outer_individual        = settingsZone.is_individual & 0x01;
}
