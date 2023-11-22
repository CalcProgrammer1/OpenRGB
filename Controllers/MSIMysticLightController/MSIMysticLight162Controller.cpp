/*-----------------------------------------*\
|  MSIMysticLight162Controller.cpp          |
|                                           |
|  Driver for MSI Mystic Light (162-byte)   |
|  USB lighting controller                  |
|                                           |
|  T-bond 3/4/2020                          |
|  Adam Honse 3/6/2021                      |
\*-----------------------------------------*/

#include "MSIMysticLight162Controller.h"
#include <algorithm>
#include <array>
#include <bitset>

MSIMysticLight162Controller::MSIMysticLight162Controller(hid_device* handle, const char *path)
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

    /*-----------------------------------------*\
    | Initialize save flag                      |
    \*-----------------------------------------*/
    data.save_data = 0;
}

MSIMysticLight162Controller::~MSIMysticLight162Controller()
{
    hid_close(dev);
}

void MSIMysticLight162Controller::SetMode
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

    zoneData->effect                    = mode;
    zoneData->speedAndBrightnessFlags   = ( brightness << 2 ) | ( speed & 0x03 );
    zoneData->colorFlags                = 0x00;// BitSet(zoneData->colorFlags, !rainbow_color, 7u);
    zoneData->padding                   = 0x00;
}

std::string MSIMysticLight162Controller::GetDeviceName()
{
    return name;
}

std::string MSIMysticLight162Controller::GetFWVersion()
{
    std::string firmware_version;
    firmware_version = "APROM: " + version_APROM + ", LDROM: " + version_LDROM;
    return firmware_version;
}

std::string MSIMysticLight162Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string MSIMysticLight162Controller::GetSerial()
{
    return chip_id;
}

bool MSIMysticLight162Controller::ReadSettings()
{
    /*-----------------------------------------------------*\
    | Read packet from hardware, return true if successful  |
    \*-----------------------------------------------------*/
    return(hid_get_feature_report(dev, (unsigned char *)&data, sizeof(data)) == sizeof data);
}

bool MSIMysticLight162Controller::Update()
{
    /*-----------------------------------------------------*\
    | Send packet to hardware, return true if successful    |
    \*-----------------------------------------------------*/
    return(hid_send_feature_report(dev, (unsigned char *)&data, sizeof(data)) == sizeof data);
}

void MSIMysticLight162Controller::SetLEDColor
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

ZoneData *MSIMysticLight162Controller::GetZoneData
    (
    MSI_ZONE           zone
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
    case MSI_ZONE_ON_BOARD_LED_0:
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
    case MSI_ZONE_J_CORSAIR:
        return &data.j_corsair_1;
    }

    return nullptr;
}

RainbowZoneData *MSIMysticLight162Controller::GetRainbowZoneData
    (
    MSI_ZONE            zone
    )
{
    switch(zone)
    {
    case MSI_ZONE_J_RAINBOW_1:
        return (RainbowZoneData *)&data.j_rainbow_1;
//    case MSI_ZONE_J_RAINBOW_2:
//        return &data.j_rainbow_2;
    case MSI_ZONE_J_CORSAIR:
    default:
        return nullptr;
    }
}

bool MSIMysticLight162Controller::ReadFwVersion()
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

void MSIMysticLight162Controller::ReadSerial()
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

void MSIMysticLight162Controller::ReadName()
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

void MSIMysticLight162Controller::GetMode
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

unsigned char MSIMysticLight162Controller::BitSet
    (
    unsigned char       value,
    bool                bit,
    unsigned int        position
    )
{
    return static_cast<unsigned char>(std::bitset<8>(value).set(position, bit).to_ulong());
}
