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

    if( dev )
    {
        loc = path;
        
        ReadName();
        ReadSerial();
        ReadFwVersion();
    }
}

MSIMysticLightController::~MSIMysticLightController()
{
    if( dev )
    {
        hid_close(dev);
    }
}

unsigned int MSIMysticLightController::GetZoneMinLedCount(ZONE zone)
{
    return 1;
}

unsigned int MSIMysticLightController::GetZoneMaxLedCount(ZONE zone)
{
    switch (zone)
    {
    case ZONE::J_RAINBOW_1:
    case ZONE::J_RAINBOW_2:
    case ZONE::J_CORSAIR:
        return 1; // TODO: Handle multiple leds
        break;
    default:
        return 1;
    }
}

unsigned int MSIMysticLightController::GetZoneLedCount(ZONE zone)
{
    return GetZoneMaxLedCount(zone); // TODO: Handle multiple leds
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
    return(name);
}

std::string MSIMysticLightController::GetFWVersion()
{
    return std::string("AP/LD ").append(versionAPROM).append(" / ").append(versionLDROM);
}

std::string MSIMysticLightController::GetDeviceLocation()
{
    return(loc);
}

std::string MSIMysticLightController::GetSerial()
{
    return(chip_id);
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

ZoneData *MSIMysticLightController::GetZoneData(ZONE zone)
{
    switch (zone)
    {
    case ZONE::J_RGB_1:
        return &data.j_rgb_1;
    case ZONE::J_RGB_2:
        return &data.j_rgb_2;
    case ZONE::J_RAINBOW_1:
        return &data.j_rainbow_1;
    case ZONE::J_RAINBOW_2:
        return &data.j_rainbow_2;
    case ZONE::J_PIPE_1:
        return &data.j_pipe_1;
    case ZONE::J_PIPE_2:
        return &data.j_pipe_2;
    case ZONE::ON_BOARD_LED:
        return &data.on_board_led;
    case ZONE::ON_BOARD_LED_1:
        return &data.on_board_led_1;
    case ZONE::ON_BOARD_LED_2:
        return &data.on_board_led_2;
    case ZONE::ON_BOARD_LED_3:
        return &data.on_board_led_3;
    case ZONE::ON_BOARD_LED_4:
        return &data.on_board_led_4;
    case ZONE::ON_BOARD_LED_5:
        return &data.on_board_led_5;
    case ZONE::ON_BOARD_LED_6:
        return &data.on_board_led_6;
    case ZONE::ON_BOARD_LED_7:
        return &data.on_board_led_7;
    case ZONE::ON_BOARD_LED_8:
        return &data.on_board_led_8;
    case ZONE::ON_BOARD_LED_9:
        return &data.on_board_led_9;
    }

    return nullptr;
}

bool MSIMysticLightController::ReadFwVersion()
{
    // First read the APROM
    int num = 1;
    unsigned char request[64] = {1, 176};
    unsigned char response[64];

    std::fill_n(request + 2, sizeof request - 2, 204);
    num &= hid_write(dev, request, 64);
    num &= hid_read(dev, response, 64);

    unsigned char highValue = response[2] >> 4;
    unsigned char lowValue  = response[2] & 15;

    versionAPROM = std::to_string(static_cast<int>(highValue)).append(".").append(std::to_string(static_cast<int>(lowValue)));

    // Now read the LDROM
    request[1] = 182;
    num &= hid_write(dev, request, 64);
    num &= hid_read(dev, response, 64);

    highValue = response[2] >> 4;
    lowValue = response[2] & 15;

    versionLDROM = std::to_string(static_cast<int>(highValue)).append(".").append(std::to_string(static_cast<int>(lowValue)));

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