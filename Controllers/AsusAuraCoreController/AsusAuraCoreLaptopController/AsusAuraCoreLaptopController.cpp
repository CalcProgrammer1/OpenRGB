/*---------------------------------------------------------*\
| AsusAuraCoreLaptopController.cpp                          |
|                                                           |
|   Driver for ASUS ROG Aura Core Laptop                    |
|                                                           |
|   Chris M (Dr_No)                             28 Jul 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "AsusAuraCoreLaptopController.h"
#include "dmiinfo.h"
#include "SettingsManager.h"
#include "StringUtils.h"

static std::string power_zones[ASUSAURACORELAPTOP_POWER_ZONES] =
{
    "Logo",
    ZONE_EN_KEYBOARD,
    "Lightbar",
    "Lid Edges"
};

static std::string power_states[ASUSAURACORELAPTOP_POWER_STATES] =
{
    " when booting",
    " when awake",
    " when sleeping",
    " when off",
};

AsusAuraCoreLaptopController::AsusAuraCoreLaptopController(hid_device* dev_handle, const char* path)
{
    dev                     = dev_handle;
    location                = path;

    /*---------------------------------------------------------*\
    | The motherboard name will uniquely ID the laptop to       |
    |   determine the metadata of the device.                   |
    \*---------------------------------------------------------*/
    DMIInfo dmi_info;
    std::string dmi_name    = dmi_info.getMainboard();
    bool not_found          = true;

    for(uint16_t i = 0; i < AURA_CORE_LAPTOP_DEVICE_COUNT; i++)
    {
        if(aura_core_laptop_device_list[i]->dmi_name == dmi_name)
        {
            /*---------------------------------------------------------*\
            | Set device ID                                             |
            \*---------------------------------------------------------*/
            not_found       = false;
            device_index    = i;
            break;
        }
    }

    if(not_found)
    {
        LOG_ERROR("[%s] device capabilities not found. Please creata a new device request.",
                  dmi_name.c_str());
        return;
    }

    /*---------------------------------------------------------*\
    | Only set power config for known devices                   |
    \*---------------------------------------------------------*/
    SetPowerConfigFromJSON();
    SendInitDirectMode();
}

AsusAuraCoreLaptopController::~AsusAuraCoreLaptopController()
{
    hid_close(dev);
}

const aura_core_laptop_device* AsusAuraCoreLaptopController::GetDeviceData()
{
    return aura_core_laptop_device_list[device_index];
}

std::string AsusAuraCoreLaptopController::GetDeviceDescription()
{
    /*---------------------------------------------------------*\
    | Get device name from HID manufacturer and product strings |
    \*---------------------------------------------------------*/
    wchar_t name_string[HID_MAX_STR];

    hid_get_manufacturer_string(dev, name_string, HID_MAX_STR);
    std::string name = StringUtils::wstring_to_string(name_string);

    hid_get_product_string(dev, name_string, HID_MAX_STR);
    name.append(" ").append(StringUtils::wstring_to_string(name_string));
    return name;
}

unsigned int AsusAuraCoreLaptopController::GetKeyboardLayout()
{
    const uint8_t index                                     = 6;
    uint8_t result                                          = 0;
    uint8_t rd_buf[ASUSAURACORELAPTOP_WRITE_PACKET_SIZE]    = { ASUSAURACORELAPTOP_REPORT_ID };
    uint8_t buffer[ASUSAURACORELAPTOP_WRITE_PACKET_SIZE]    = { ASUSAURACORELAPTOP_REPORT_ID,
                                                                ASUSAURACORELAPTOP_CMD_LAYOUT,
                                                                0x20,
                                                                0x31,
                                                                0x00,
                                                                0x10  };

    /*---------------------------------------------------------*\
    | Clear the read buffer to ensure we read the right packet  |
    \*---------------------------------------------------------*/
    do
    {
        result = hid_read_timeout(dev, rd_buf, ASUSAURACORELAPTOP_WRITE_PACKET_SIZE, 10);
    }
    while(result > 0);

    memset(&rd_buf[1],     0, ASUSAURACORELAPTOP_WRITE_PACKET_SIZE - 1);
    memset(&buffer[index], 0, ASUSAURACORELAPTOP_WRITE_PACKET_SIZE - index);

    hid_send_feature_report(dev, buffer, ASUSAURACORELAPTOP_WRITE_PACKET_SIZE);
    result = hid_get_feature_report(dev, rd_buf, ASUSAURACORELAPTOP_WRITE_PACKET_SIZE);

    LOG_DEBUG("[%s] GetKeyboardLayout %02X %02X %02X %02X   %02X %02X %02X {%02X}   %02X %02X %02X %02X",
              aura_core_laptop_device_list[device_index]->dmi_name.c_str(),
              rd_buf[5],  rd_buf[6],  rd_buf[7],  rd_buf[8],  rd_buf[9],  rd_buf[10],
              rd_buf[11], rd_buf[12], rd_buf[13], rd_buf[14], rd_buf[15], rd_buf[16]);

    if(result > 0)
    {
        return rd_buf[12];
    }

    LOG_DEBUG("[%s] GetKeyboardLayout: An error occurred! Setting layout to ANSI",
              aura_core_laptop_device_list[device_index]->dmi_name.c_str());
    return ASUSAURACORELAPTOP_LAYOUT_ANSI;
}

std::string AsusAuraCoreLaptopController::GetSerial()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        LOG_DEBUG("[%s] Get HID Serial string failed",
                  aura_core_laptop_device_list[device_index]->dmi_name.c_str());
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string AsusAuraCoreLaptopController::GetLocation()
{
    return("HID: " + location);
}

void AsusAuraCoreLaptopController::SetMode(uint8_t mode, uint8_t speed, uint8_t brightness, RGBColor color1, RGBColor color2, uint8_t random, uint8_t direction)
{
    bool needs_update       = !( (current_mode          == mode         )   &&
                                 (current_speed         == speed        )   &&
                                 (current_brightness    == brightness   )   &&
                                 (current_c1            == color1       )   &&
                                 (current_c2            == color2       )   &&
                                 (current_random        == random       )   &&
                                 (current_direction     == direction    )   );

    if(needs_update)
    {
        current_mode        = mode;
        current_speed       = speed;
        current_brightness  = brightness;
        current_c1          = color1;
        current_c2          = color2;
        current_random      = random;
        current_direction   = direction;

        if(current_mode == ASUSAURACORELAPTOP_MODE_DIRECT)
        {
            SendBrightness();
            SendInitDirectMode();
            return;
        }

        SendUpdate();
        SendBrightness();
    }
}

void AsusAuraCoreLaptopController::SendInitDirectMode()
{
    uint8_t buffer[ASUSAURACORELAPTOP_WRITE_PACKET_SIZE]    = { ASUSAURACORELAPTOP_REPORT_ID, ASUSAURACORELAPTOP_CMD_DIRECT };
    memset(&buffer[2], 0, ASUSAURACORELAPTOP_WRITE_PACKET_SIZE - 2);

    LOG_DEBUG("[%s] Resetting device for direct control", aura_core_laptop_device_list[device_index]->dmi_name.c_str());
    hid_send_feature_report(dev, buffer, ASUSAURACORELAPTOP_WRITE_PACKET_SIZE);
}

void AsusAuraCoreLaptopController::SetLedsDirect(std::vector<RGBColor *> colors)
{
    /*---------------------------------------------------------*\
    | The keyboard zone is a set of 168 keys (indexed from 0)   |
    |   sent in 11 packets of 16 triplets. The Lid and Lightbar |
    |   zones are sent in one final packet afterwards.          |
    \*---------------------------------------------------------*/
    const uint8_t   key_set                                 = 167;
    const uint8_t   led_count                               = (uint8_t)colors.size();
    const uint16_t  map_size                                = 3 * led_count;
    const uint8_t   leds_per_packet                         = 16;
    uint8_t buffer[ASUSAURACORELAPTOP_WRITE_PACKET_SIZE]    = { ASUSAURACORELAPTOP_REPORT_ID, ASUSAURACORELAPTOP_CMD_DIRECT,
                                                                0x00, 0x01, 0x01, 0x01, 0x00, leds_per_packet, 0x00 };
    uint8_t*        key_buf                                 = new uint8_t[map_size];

    memset(key_buf, 0, map_size);

    for(uint8_t led_index = 0; led_index < led_count; led_index++)
    {
        std::size_t buf_idx     = (led_index * 3);

        key_buf[buf_idx]        = RGBGetRValue(*colors[led_index]);
        key_buf[buf_idx + 1]    = RGBGetGValue(*colors[led_index]);
        key_buf[buf_idx + 2]    = RGBGetBValue(*colors[led_index]);
    }

    for(uint8_t i = 0; i < key_set; i += leds_per_packet)
    {
        uint8_t leds_remaining  = key_set - i;

        if(leds_remaining < leds_per_packet)
        {
            buffer[07]          = leds_remaining;

            memset(&buffer[ASUSAURACORELAPTOP_DATA_BYTE],
                   0,
                   ASUSAURACORELAPTOP_WRITE_PACKET_SIZE - ASUSAURACORELAPTOP_DATA_BYTE);
        }

        buffer[06]              = i;
        memcpy(&buffer[ASUSAURACORELAPTOP_DATA_BYTE], &key_buf[3 * i], (3 * buffer[07]));

        LOG_DEBUG("[%s] Sending buffer @ index %d thru index %d",
                  aura_core_laptop_device_list[device_index]->dmi_name.c_str(),
                  i,
                  i + buffer[07]);

        hid_send_feature_report(dev, buffer, ASUSAURACORELAPTOP_WRITE_PACKET_SIZE);
    }

    buffer[4] = 0x04;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;

    memset(&buffer[ASUSAURACORELAPTOP_DATA_BYTE],
           0,
           ASUSAURACORELAPTOP_WRITE_PACKET_SIZE - ASUSAURACORELAPTOP_DATA_BYTE);

    if(led_count > key_set)
    {
        memcpy(&buffer[ASUSAURACORELAPTOP_DATA_BYTE],
               &key_buf[3 * key_set],
               (3 * (led_count - key_set)));
    }

    LOG_DEBUG("[%s] Sending buffer @ index %d thru index %d",
              aura_core_laptop_device_list[device_index]->dmi_name.c_str(),
              key_set,
              led_count);

    hid_send_feature_report(dev, buffer, ASUSAURACORELAPTOP_WRITE_PACKET_SIZE);
    delete[] key_buf;
}

void AsusAuraCoreLaptopController::SendBrightness()
{
    const uint8_t index                                     = 5;
    uint8_t buffer[ASUSAURACORELAPTOP_WRITE_PACKET_SIZE]    = { ASUSAURACORELAPTOP_REPORT_ID, ASUSAURACORELAPTOP_CMD_BRIGHTNESS, 0xC5, 0xC4};

    memset(&buffer[index], 0, ASUSAURACORELAPTOP_WRITE_PACKET_SIZE - index);
    buffer[4] = current_brightness;

    hid_send_feature_report(dev, buffer, ASUSAURACORELAPTOP_WRITE_PACKET_SIZE);
}

void AsusAuraCoreLaptopController::SendUpdate()
{
    uint8_t buffer[ASUSAURACORELAPTOP_WRITE_PACKET_SIZE]    = { ASUSAURACORELAPTOP_REPORT_ID, ASUSAURACORELAPTOP_CMD_UPDATE };

    buffer[ASUSAURACORELAPTOP_ZONE_BYTE]                    = 0;
    buffer[ASUSAURACORELAPTOP_MODE_BYTE]                    = current_mode;
    buffer[ASUSAURACORELAPTOP_R1_BYTE]                      = RGBGetRValue(current_c1);
    buffer[ASUSAURACORELAPTOP_G1_BYTE]                      = RGBGetGValue(current_c1);
    buffer[ASUSAURACORELAPTOP_B1_BYTE]                      = RGBGetBValue(current_c1);
    buffer[ASUSAURACORELAPTOP_SPEED_BYTE]                   = current_speed;
    buffer[ASUSAURACORELAPTOP_DIRECTION_BYTE]               = current_direction;
    buffer[ASUSAURACORELAPTOP_DATA_BYTE]                    = current_random;
    buffer[ASUSAURACORELAPTOP_R2_BYTE]                      = RGBGetRValue(current_c2);
    buffer[ASUSAURACORELAPTOP_G2_BYTE]                      = RGBGetGValue(current_c2);
    buffer[ASUSAURACORELAPTOP_B2_BYTE]                      = RGBGetBValue(current_c2);

    hid_send_feature_report(dev, buffer, ASUSAURACORELAPTOP_WRITE_PACKET_SIZE);

    SendApply();
}

void AsusAuraCoreLaptopController::SendApply()
{
    const uint8_t index                                     = 2;
    uint8_t buffer[ASUSAURACORELAPTOP_WRITE_PACKET_SIZE]    = { ASUSAURACORELAPTOP_REPORT_ID, ASUSAURACORELAPTOP_CMD_APPLY };

    memset(&buffer[index], 0, ASUSAURACORELAPTOP_WRITE_PACKET_SIZE - index);

    hid_send_feature_report(dev, buffer, ASUSAURACORELAPTOP_WRITE_PACKET_SIZE);
}

void AsusAuraCoreLaptopController::SendSet()
{
    const uint8_t index                                     = 2;
    uint8_t buffer[ASUSAURACORELAPTOP_WRITE_PACKET_SIZE]    = { ASUSAURACORELAPTOP_REPORT_ID, ASUSAURACORELAPTOP_CMD_SET };

    memset(&buffer[index], 0, ASUSAURACORELAPTOP_WRITE_PACKET_SIZE - index);

    hid_send_feature_report(dev, buffer, ASUSAURACORELAPTOP_WRITE_PACKET_SIZE);
}

std::vector<p_state> AsusAuraCoreLaptopController::PowerConfigArray()
{
    std::vector<p_state>    temp;

    for(uint8_t zone_index = 0; zone_index < ASUSAURACORELAPTOP_POWER_ZONES; zone_index++)
    {
        for(uint8_t state_index = 0; state_index < ASUSAURACORELAPTOP_POWER_STATES; state_index++)
        {
            p_state new_state;

            new_state.zone  = power_zones[zone_index] + power_states[state_index];
            new_state.state = true;

            temp.push_back(new_state);
        }
    }

    return temp;
}

void AsusAuraCoreLaptopController::SetPowerConfigFromJSON()
{
    std::vector<p_state> power_config   = PowerConfigArray();
    const std::string section_power     = "PowerConfig";
    const std::string detector_name     = "Asus Aura Core Laptop";
    SettingsManager* settings_manager   = ResourceManager::get()->GetSettingsManager();
    json device_settings                = settings_manager->GetSettings(detector_name);

    /*---------------------------------------------------------*\
    | Get Power state config from the settings manager          |
    |   If PowerConfig is not found then write it to settings   |
    \*---------------------------------------------------------*/
    if(!device_settings.contains(section_power))
    {
        json pcfg;

        for(size_t i = 0; i < power_config.size(); i++)
        {
            pcfg[power_config[i].zone]  = power_config[i].state;
        }

        device_settings[section_power]  = pcfg;
        settings_manager->SetSettings(detector_name, device_settings);
        settings_manager->SaveSettings();
        LOG_DEBUG("[%s] default power config saved to openrgb.json",
                  aura_core_laptop_device_list[device_index]->dmi_name.c_str());
    }
    else
    {
        for(size_t i = 0; i < power_config.size(); i++)
        {
            std::string key_name        = power_config[i].zone;

            if(device_settings[section_power].contains(key_name))
            {
                power_config[i].state   = device_settings[section_power][key_name];
                LOG_DEBUG("[%s] Reading power config for %s: %s",
                          aura_core_laptop_device_list[device_index]->dmi_name.c_str(),
                          key_name.c_str(), ((power_config[i].state) ? "On" : "Off"));
            }
        }
    }

    /*-----------------------------------------------------------------------------*\
    | Power state flags are packed in zones but the order is inconsistent.          |
    |   With thanks to AsusCtl for helping to decipher the packet captures          |
    |   https://gitlab.com/asus-linux/asusctl/-/blob/main/rog-aura/src/usb.rs#L150  |
    \*-----------------------------------------------------------------------------*/
    bool flag_array[32] =
    {
        power_config[0].state,      power_config[4].state,
        power_config[1].state,      power_config[5].state,
        !power_config[2].state,     !power_config[6].state,
        !power_config[3].state,     !power_config[7].state,

        false,                      power_config[8].state,
        power_config[9].state,      !power_config[10].state,
        !power_config[11].state,    false,
        false,                      false,

        power_config[12].state,     power_config[13].state,
        !power_config[14].state,    !power_config[15].state
    };

    uint32_t flags      = PackPowerFlags(flag_array);
    LOG_DEBUG("[%s] Sending power config Logo+KB: %02X Lightbar: %02X Lid Edges: %02X Raw: %08X",
              aura_core_laptop_device_list[device_index]->dmi_name.c_str(),
              (flags & 0xFF), ((flags >> 8)  & 0xFF), ((flags >> 16)  & 0xFF), flags);
    SendPowerConfig(flags);
}

void AsusAuraCoreLaptopController::SendPowerConfig(uint32_t flags)
{
    const uint8_t index                                     = 6;
    uint8_t buffer[ASUSAURACORELAPTOP_WRITE_PACKET_SIZE]    = { ASUSAURACORELAPTOP_REPORT_ID, ASUSAURACORELAPTOP_CMD_POWER, 0x01, 0x00, 0x00, 0x0F };

    memset(&buffer[index], 0, ASUSAURACORELAPTOP_WRITE_PACKET_SIZE - index);

    buffer[3] = flags           & 0xFF;
    buffer[4] = (flags >> 8)    & 0xFF;
    buffer[5] = (flags >> 16)   & 0xFF;

    hid_send_feature_report(dev, buffer, ASUSAURACORELAPTOP_WRITE_PACKET_SIZE);
}

uint32_t AsusAuraCoreLaptopController::PackPowerFlags(bool flags[])
{
    uint32_t temp = {};
    const uint8_t length = 32;

    for (size_t i = 0; i < length; ++i)
    {
        uint32_t flag = {flags[i]};
        flag <<= i;
        temp |= flag;
    }

    return temp;
}
