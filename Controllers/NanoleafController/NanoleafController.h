/*-----------------------------------------*\
|  NanoleafController.h                     |
|                                           |
|  API Interface for Nanoleaf devices       |
|                                           |
|  Nikita Rushmanov 01/13/2022              |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "net_port.h"

#define NANOLEAF_DIRECT_MODE_EFFECT_NAME "*Dynamic*"

#define NANOLEAF_LIGHT_PANELS_MODEL "NL22"
#define NANOLEAF_CANVAS_MODEL "NL29"

class NanoleafController
{
public:
    static std::string Pair(std::string address, int port);
    static void Unpair(std::string address, int port, std::string auth_token);

    NanoleafController(std::string a_address, int a_port, std::string a_auth_token);

    void SelectEffect(std::string effect_name);
    void StartExternalControl();
    void SetBrightness(int a_brightness);
    // Requires External Control to have been started.
    void UpdateLEDs(std::vector<RGBColor>& colors);

    std::string GetAuthToken() { return auth_token; };
    std::string GetName() { return name; };
    std::string GetSerial() { return serial; };
    std::string GetManufacturer() { return manufacturer; };
    std::string GetFirmwareVersion() { return firmware_version; };
    std::string GetModel() { return model; };
    std::vector<std::string>& GetEffects() { return effects; };
    std::vector<int>& GetPanelIds() { return panel_ids; };
    std::string GetSelectedEffect() { return selectedEffect; };
    int GetBrightness() { return brightness; };

private:
    net_port external_control_socket;

    std::string address;
    int port;
    std::string location;
    std::string auth_token;

    std::string name;
    std::string serial;
    std::string manufacturer;
    std::string firmware_version;
    std::string model;

    std::vector<std::string> effects;
    std::vector<int> panel_ids;

    std::string selectedEffect;
    int brightness;
};
