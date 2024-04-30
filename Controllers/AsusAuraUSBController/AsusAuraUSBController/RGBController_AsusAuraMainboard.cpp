/*-----------------------------------------*\
|  RGBController_AsusAuraMainboard.cpp      |
|                                           |
|  RGB controller for Asus Aura USB         |
|  mainboards                               |
|                                           |
|  rytypete 8/30/2023                       |
|  based on RGBController_AsusAuraUSB.cpp   |
|  code                                     |
\*-----------------------------------------*/

#include "RGBController_AsusAuraMainboard.h"

/**------------------------------------------------------------------*\
    @name Asus Aura USB Mainboard
    @category Motherboard
    @type USB
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectAsusAuraUSBMotherboards
    @comment The Asus Aura USB Mainboard controller applies to most
        AMD and Intel mainboards from the x570 chipset onwards.
\*-------------------------------------------------------------------*/

RGBController_AuraMainboard::RGBController_AuraMainboard(AuraMainboardController* controller_ptr) :
    RGBController_AuraUSB(controller_ptr)
{
    name        = "ASUS Aura USB Mainboard";
    description = "ASUS Aura USB Mainboard Device";

    /*-------------------------------------------------------*\
    | Add manual save flag to all modes except direct mode    |
    \*-------------------------------------------------------*/
    for(unsigned int mode_idx = 0; mode_idx < modes.size(); mode_idx++)
    {
        mode Mode = modes[mode_idx];
        if(Mode.value != AURA_MODE_DIRECT)
        {
            Mode.flags |= MODE_FLAG_MANUAL_SAVE;
            modes[mode_idx] = Mode;
        }
    }
}

void RGBController_AuraMainboard::DeviceUpdateShutdownEffect()
{
    unsigned char red = 0;
    unsigned char grn = 0;
    unsigned char blu = 0;

    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        red = RGBGetRValue(modes[active_mode].colors[0]);
        grn = RGBGetGValue(modes[active_mode].colors[0]);
        blu = RGBGetBValue(modes[active_mode].colors[0]);
    }

    for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        AuraDeviceInfo device_info = controller->GetAuraDevices()[zone_idx];

        /*---------------------------------------------------*\
        | Shutdown effect only works with onboard lighting    |
        \*---------------------------------------------------*/
        if(device_info.device_type == AuraDeviceType::FIXED && zones[zone_idx].leds_count > 0)
        {
            ((AuraMainboardController*) controller)->SetMode(zone_idx, modes[active_mode].value, red, grn, blu, true);
        }
    }
}

void RGBController_AuraMainboard::DeviceSaveMode()
{
    DeviceUpdateMode();
    DeviceUpdateShutdownEffect();
    ((AuraMainboardController*) controller)->SendCommit();
}
