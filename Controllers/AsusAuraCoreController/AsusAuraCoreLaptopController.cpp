/*---------------------------------------------------------------------*\
|  AsusAuraCoreLaptopController.cpp                                     |
|                                                                       |
|  Driver for Asus Aura Core Laptop USB Controller                      |
|                                                                       |
|  Chris M (Dr_No)          28 Jul 2022                                 |
|                                                                       |
\*---------------------------------------------------------------------*/

#include "AsusAuraCoreLaptopController.h"

static uint8_t packet_map[ASUSAURACORELAPTOP_KEYCOUNT       +
                          ASUSAURACORELAPTOP_LIGHTBARCOUNT  +
                          ASUSAURACORELAPTOP_LIDCOUNT       ] =
{
/*00        ESC  F1   F2   F3   F4   F5   F6   F7   F8   F9  */
            21,  23,  24,  25,  26,  28,  29,  30,  31,  33,

/*10        F10  F11  F12  DEL   `    1    2    3    4    5  */
            34,  35,  36,  37,  42,  43,  44,  45,  46,  47,

/*20         6    7    8    9    0    -    =   BSP  BSP  BSP */
            48,  49,  50,  51,  52,  53,  54,  55,  56,  57,

/*30        PLY  TAB   Q    W    E    R    T    Y    U    I  */
            58,  63,  64,  65,  66,  67,  68,  69,  70,  71,

/*40         O    P    [    ]    \   STP  CAP   A    S    D  */
            72,  73,  74,  75,  76,  79,  84,  85,  86,  87,

/*50         F    G    H    J    K    L    ;    '   ENT  PRV */
            88,  89,  90,  91,  92,  93,  94,  95,  98, 100,

/*60        LSH   Z    X    C    V    B    N    M    ,    .  */
           105, 107, 108, 109, 110, 111, 112, 113, 114, 115,

/*70         /   RSH  UP   NXT LCTL  LFN LWIN LALT  SPC RALT */
           116, 119, 139, 121, 126, 127, 128, 129, 131, 135,

/*80       RCTL  LFT  DWN  RGT  PRT KSTN  VDN  VUP MICM HPFN */
           137, 159, 160, 161, 142, 175,   2,   3,   4,   5,

/*90       ARMC  LB1  LB2  LB3  LB4  LB5  LB6 LOGO LIDL LIDR */
             6, 174, 173, 172, 171, 170, 169, 167, 176, 177,

};

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
    const uint8_t sz    = HID_MAX_STR;
    wchar_t       tmp[sz];

    dev                 = dev_handle;
    location            = path;

    hid_get_manufacturer_string(dev, tmp, sz);
    std::wstring wName = std::wstring(tmp);
    device_name = std::string(wName.begin(), wName.end());

    hid_get_product_string(dev, tmp, sz);
    wName = std::wstring(tmp);
    device_name.append(" ").append(std::string(wName.begin(), wName.end()));

    SetPowerConfigFromJSON();
}

AsusAuraCoreLaptopController::~AsusAuraCoreLaptopController()
{
    hid_close(dev);
}

std::string AsusAuraCoreLaptopController::GetDeviceName()
{
    return device_name;
}

std::string AsusAuraCoreLaptopController::GetSerial()
{
    const uint8_t sz    = HID_MAX_STR;
    wchar_t       tmp[sz];

    int ret             = hid_get_serial_number_string(dev, tmp, sz);

    if (ret != 0)
    {
        LOG_DEBUG("[%s] Get HID Serial string failed", device_name.c_str());
        return("");
    }

    std::wstring w_tmp  = std::wstring(tmp);
    std::string serial  = std::string(w_tmp.begin(), w_tmp.end());

    return serial;
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

        SendUpdate();
        SendBrightness();
    }
}

void AsusAuraCoreLaptopController::SetLedsDirect(std::vector<RGBColor> colors)
{
    /*---------------------------------------------------------*\
    | The keyboard zone is a set of 168 keys (indexed from 0)   |
    |   sent in 11 packets of 16 triplets. The Lid and Lightbar |
    |   zones are sent in one final packet afterwards.          |
    \*---------------------------------------------------------*/
    const uint8_t  key_set                                  = 167;
    const uint8_t  led_count                                = 178;
    const uint16_t map_size                                 = 3 * led_count;
    const uint8_t leds_per_packet                           = 16;
    uint8_t buffer[ASUSAURACORELAPTOP_WRITE_PACKET_SIZE]    = { ASUSAURACORELAPTOP_REPORT_ID, ASUSAURACORELAPTOP_CMD_DIRECT,
                                                                0x00, 0x01, 0x01, 0x01, 0x00, leds_per_packet, 0x00 };
    uint8_t key_buf[map_size];

    memset(key_buf, 0, map_size);

    for(size_t led_index = 0; led_index < colors.size(); led_index++)
    {
        uint16_t offset         = 3 * packet_map[led_index];

        key_buf[offset]         = RGBGetRValue(colors[led_index]);
        key_buf[offset + 1]     = RGBGetGValue(colors[led_index]);
        key_buf[offset + 2]     = RGBGetBValue(colors[led_index]);
    }

    for(size_t i = 0; i < key_set; i+=leds_per_packet)
    {
        uint8_t leds_remaining  = key_set - i;

        if(leds_remaining < leds_per_packet)
        {
            buffer[07]          = leds_remaining;
        }

        buffer[06]              = i;
        memcpy(&buffer[ASUSAURACORELAPTOP_DATA_BYTE], &key_buf[3 * i], (3 * buffer[07]));

        hid_send_feature_report(dev, buffer, ASUSAURACORELAPTOP_WRITE_PACKET_SIZE);
    }

    buffer[4] = 0x04;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    memcpy(&buffer[ASUSAURACORELAPTOP_DATA_BYTE], &key_buf[3 * key_set], (3 * (led_count - key_set)));

    hid_send_feature_report(dev, buffer, ASUSAURACORELAPTOP_WRITE_PACKET_SIZE);
}

void AsusAuraCoreLaptopController::SendBrightness()
{
    const uint8_t index                                     = 2;
    uint8_t buffer[ASUSAURACORELAPTOP_WRITE_PACKET_SIZE]    = { ASUSAURACORELAPTOP_REPORT_ID, ASUSAURACORELAPTOP_CMD_BRIGHTNESS };

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
    }
    else
    {
        for(size_t i = 0; i < power_config.size(); i++)
        {
            std::string key_name        = power_config[i].zone;

            if(device_settings[section_power].contains(key_name))
            {
                power_config[i].state   = device_settings[section_power][key_name];
                LOG_DEBUG("[%s] Reading power config for %s: %s", device_name.c_str(), key_name.c_str(), ((power_config[i].state) ? "On" : "Off"));
            }
        }
    }

    /*-----------------------------------------------------------------------------*\
    | Power state flags are packed in zones but the order is inconsistent.          |
    |   With thanks to AsusCtl for helping to decipher the packet captures          |
    |   https://gitlab.com/asus-linux/asusctl/-/blob/main/rog-aura/src/usb.rs#L150  |
    \*-----------------------------------------------------------------------------*/
    bool flag_array[] =
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
    LOG_DEBUG("[%s] Sending power config Logo+KB: %02X Lightbar: %02X Lid Edges: %02X Raw: %08X", device_name.c_str(), (flags & 0xFF), ((flags >> 8)  & 0xFF), ((flags >> 16)  & 0xFF), flags);
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
