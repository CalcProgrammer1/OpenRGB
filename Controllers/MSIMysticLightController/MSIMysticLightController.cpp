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
        loc = path;

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

unsigned int MSIMysticLightController::GetZoneMinLedCount(ZONE /*zone*/)
{
    return 1;
}

unsigned int MSIMysticLightController::GetZoneMaxLedCount(ZONE zone)
{
    switch (zone)
    {
    case J_RAINBOW_1:
    case J_RAINBOW_2:
    case J_CORSAIR:
        return 240;
    default:
        return 1;
    }
}

unsigned int MSIMysticLightController::GetZoneLedCount(ZONE zone)
{
    RainbowZoneData *requestedZone = GetRainbowZoneData(zone);

    if (!requestedZone)
    {
        return GetZoneMaxLedCount(zone);
    }

    return requestedZone->led_count;
}

void MSIMysticLightController::SetZoneLedCount(ZONE zone, unsigned int led_count)
{
    RainbowZoneData *requestedZone = GetRainbowZoneData(zone);

    if (!requestedZone)
    {
        return;
    }

    requestedZone->led_count = std::min(GetZoneMaxLedCount(zone), std::max(GetZoneMinLedCount(zone), led_count));
}

void MSIMysticLightController::SetMode(ZONE zone, EFFECT mode, SPEED speed, BRIGHTNESS brightness, bool rainbow_color)
{
    ZoneData* zoneData = GetZoneData(zone);
    if(!zoneData)
    {
        return;
    }

    zoneData->effect = mode;
    zoneData->speedAndBrightnessFlags = (zoneData->speedAndBrightnessFlags & 128) | brightness << 2 | speed;
    zoneData->colorFlags = static_cast<unsigned char>(std::bitset<8>(zoneData->colorFlags).set(7, !rainbow_color).to_ulong());
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
    return loc;
}

std::string MSIMysticLightController::GetSerial()
{
    return chip_id;
}

bool MSIMysticLightController::ReadSettings()
{
    return hid_get_feature_report(dev, reinterpret_cast<unsigned char *>(&data), sizeof data) == sizeof data;
}

bool MSIMysticLightController::Update()
{
    return hid_send_feature_report(dev, reinterpret_cast<unsigned char *>(&data), sizeof data) == sizeof data;
}

void MSIMysticLightController::SaveOnUpdate(bool save)
{
    data.save_data = save;
}

void MSIMysticLightController::SetZoneColor(ZONE zone, unsigned char r1, unsigned char g1, unsigned char b1, unsigned char r2, unsigned char g2, unsigned char b2)
{
    ZoneData* zoneData = GetZoneData(zone);
    if(!zoneData)
    {
        return;
    }

    zoneData->color.R = r1;
    zoneData->color.G = g1;
    zoneData->color.B = b1;

    zoneData->color2.R = r2;
    zoneData->color2.G = g2;
    zoneData->color2.B = b2;
}

std::pair<Color, Color> MSIMysticLightController::GetZoneColor(ZONE zone)
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

ZoneData *MSIMysticLightController::GetZoneData(ZONE zone)
{
    switch (zone)
    {
    case J_RGB_1:
        return &data.j_rgb_1;
    case J_RGB_2:
        return &data.j_rgb_2;
    case J_RAINBOW_1:
        return &data.j_rainbow_1;
    case J_RAINBOW_2:
        return &data.j_rainbow_2;
    case J_PIPE_1:
        return &data.j_pipe_1;
    case J_PIPE_2:
        return &data.j_pipe_2;
    case ON_BOARD_LED:
        return &data.on_board_led;
    case ON_BOARD_LED_1:
        return &data.on_board_led_1;
    case ON_BOARD_LED_2:
        return &data.on_board_led_2;
    case ON_BOARD_LED_3:
        return &data.on_board_led_3;
    case ON_BOARD_LED_4:
        return &data.on_board_led_4;
    case ON_BOARD_LED_5:
        return &data.on_board_led_5;
    case ON_BOARD_LED_6:
        return &data.on_board_led_6;
    case ON_BOARD_LED_7:
        return &data.on_board_led_7;
    case ON_BOARD_LED_8:
        return &data.on_board_led_8;
    case ON_BOARD_LED_9:
        return &data.on_board_led_9;
    case J_CORSAIR_OUTERLL120:
        return &data.j_corsair_outerll120;
    default:
    case J_CORSAIR:
        break;
    }

    return nullptr;
}
RainbowZoneData *MSIMysticLightController::GetRainbowZoneData(ZONE zone)
{
    switch (zone)
    {
    case J_RAINBOW_1:
        return &data.j_rainbow_1;
    case J_RAINBOW_2:
        return &data.j_rainbow_2;
    case J_CORSAIR:
    default:
        return nullptr;
    }
}

bool MSIMysticLightController::ReadFwVersion()
{
    // First read the APROM
    // Checksum also available at report ID 180, with highCheksum stored at index 8, and low at 9
    int num = 1;
    unsigned char request[64] = {1, 176};
    unsigned char response[64];

    std::fill_n(request + 2, sizeof request - 2, 204);
    num &= hid_write(dev, request, 64);
    num &= hid_read(dev, response, 64);

    unsigned char highValue = response[2] >> 4;
    unsigned char lowValue  = response[2] & 15;

    version_APROM = std::to_string(static_cast<int>(highValue)).append(".").append(std::to_string(static_cast<int>(lowValue)));

    // Now read the LDROM
    // Checksum also available at report ID 184, with highCheksum stored at index 8, and low at 9
    request[1] = 182;
    num &= hid_write(dev, request, 64);
    num &= hid_read(dev, response, 64);

    highValue = response[2] >> 4;
    lowValue = response[2] & 15;

    version_LDROM = std::to_string(static_cast<int>(highValue)).append(".").append(std::to_string(static_cast<int>(lowValue)));

    return num == 1;
}

void MSIMysticLightController::ReadSerial()
{
    wchar_t serial[256];
    hid_get_serial_number_string(dev, serial, 256);
    std::wstring wserial = std::wstring(serial);
    chip_id = std::string(wserial.begin(), wserial.end());
}

void MSIMysticLightController::ReadName()
{
    wchar_t tname[256];
    hid_get_manufacturer_string(dev, tname, 256);
    std::wstring wname = std::wstring(tname);
    name = std::string(wname.begin(), wname.end());

    hid_get_product_string(dev, tname, 256);
    wname = std::wstring(tname);
    name.append(" ").append(std::string(wname.begin(), wname.end()));
}

void MSIMysticLightController::SetDeviceSettings(bool is_fan, FAN_TYPE fan_type,
                                                 unsigned char corsair_device_quantity,
                                                 bool is_LL120Outer_individual) { // If is_fan false, it is a stripe
    CorsairZoneData &settingsZone = data.j_corsair;
    settingsZone.fan_flags = (settingsZone.fan_flags & 128) | fan_type << 1 | is_fan;
    settingsZone.corsair_quantity = corsair_device_quantity << 2;
    settingsZone.is_individual = static_cast<unsigned char>(std::bitset<8>(settingsZone.padding).set(0, is_LL120Outer_individual).to_ulong());
}

bool MSIMysticLightController::SetVolume(unsigned char main, unsigned char left, unsigned char right) {
    unsigned char packet[64];
    std::fill_n(packet, sizeof packet, 204);

    if(main > 100)
    {
        main = 100;
    }

    if(left > 100)
    {
        left = 100;
    }

    if(right > 100)
    {
        right = 100;
    }

    packet[0] = 1;
    packet[1] = 192;
    packet[3] = main;
    packet[4] = left;
    packet[5] = right;

    return hid_write(dev, packet, sizeof packet);
}
