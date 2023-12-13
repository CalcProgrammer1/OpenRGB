/*-------------------------------------------------------------------*\
|  CMKeyboardV1Controller.h                                           |
|                                                                     |
|  Driver for Coolermaster MasterKeys keyboards (V1)                  |
|                                                                     |
|  Tam D (too.manyhobbies)      30th Nov 2023                         |
|                                                                     |
\*-------------------------------------------------------------------*/
#include "CMKeyboardAbstractController.h"

class CMKeyboardV1Controller : public CMKeyboardAbstractController
{
public:
    CMKeyboardV1Controller(hid_device* dev_handle, hid_device_info* dev_info);
    ~CMKeyboardV1Controller();

    /*---------------------------------------------------------*\
    | Protocol specific funtions to be implmented               |
    \*---------------------------------------------------------*/
    void                        SetLeds(std::vector<led> leds, std::vector<RGBColor> colors);
    void                        SetSingleLED(uint8_t in_led, RGBColor in_color);
    void                        Initialize();
    void                        Shutdown();
    void                        SetLEDControl(bool bManual);

    void                        SetActiveEffect(uint8_t effectId);
    uint8_t                     GetActiveEffect();
    void                        SetEffect(uint8_t effectId, uint8_t p1, uint8_t p2, uint8_t p3, RGBColor color1, RGBColor color2);
    struct cm_keyboard_effect   GetEffect(uint8_t effectId);
    void                        SetCustomMode();
    void                        SetMode(mode selectedMode);
    std::vector<uint8_t>        GetEnabledEffects();
    void                        InitializeModes(std::vector<mode> &modes);
    KEYBOARD_LAYOUT             GetKeyboardLayout();

private:
    std::string                 _GetFirmwareVersion();

};
