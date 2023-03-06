#ifdef _WIN32

#include "AsusTUFLaptopController.h"
#include "acpiwmi.h"

AsusTUFLaptopController::AsusTUFLaptopController()
{
    /*---------------------------------------------------*\
    | Call AsWMI_Open at least once during initialization.|
    | Since it's part of the firmware it's guaranteed to  |
    | always be there. This is necessary to issue further |
    | commands.                                           |
    \*---------------------------------------------------*/
    AsWMI_Open();
}

AsusTUFLaptopController::~AsusTUFLaptopController()
{

}

void AsusTUFLaptopController::SetMode(unsigned char red,
                                      unsigned char green,
                                      unsigned char blue,
                                      unsigned char mode,
                                      unsigned char speed,
                                      bool save)
{

    /*--------------------------------------------------------*\
    | Use switch case since our speed values are magic numbers |
    | Default to Medium/Normal speed                           |
    \*--------------------------------------------------------*/
    unsigned char speed_val;

    switch(speed)
    {
    case(1):
        speed_val = ASUS_WMI_KEYBOARD_SPEED_SLOW;
        break;
    default:
    case(2):
        speed_val = ASUS_WMI_KEYBOARD_SPEED_NORMAL;
        break;
    case(3):
        speed_val = ASUS_WMI_KEYBOARD_SPEED_FAST;
        break;
    }


    /*----------------------------------------------------------*\
    | We need to use a magic value to save to firmware in order  |
    | To persist reboots. Save is normal op with different magic |
    \*---------------------------------------------------------*/
    unsigned char save_val = ASUS_WMI_KEYBOARD_MAGIC_USE;

    if (save)
    {
        save_val = ASUS_WMI_KEYBOARD_MAGIC_SAVE;
    }

    // B3 is store value
    unsigned int high = save_val | (mode<<8) | (red<<16) | (green<<24);
    unsigned int low  = blue | (speed_val<<8);

    AsWMI_NB_DeviceControl_2arg(ASUS_WMI_DEVID_TUF_RGB_MODE, high, low);

}

unsigned char AsusTUFLaptopController::GetBrightness()
{
    int backlight_state = 0;
    AsWMI_NB_GetDeviceStatus(ASUS_WMI_DEVID_KBD_BACKLIGHT, &backlight_state);

    /*----------------------------------------------*\
    | Only lowest two bits indicate brightness level |
    \*----------------------------------------------*/
    return backlight_state & 0x7F;
}

void AsusTUFLaptopController::SetBrightness(unsigned char brightness)
{
    /*-----------------------------------------------------*\
    | Only calls in this format persistently set brightness |
    \*-----------------------------------------------------*/
    int ctrl_param = 0x80 | (brightness & 0x7F);
    AsWMI_NB_DeviceControl(ASUS_WMI_DEVID_KBD_BACKLIGHT, ctrl_param);
}

/*-----------------------------------------------------------*\
| These settings will not persist a reboot unless save is set |
\*-----------------------------------------------------------*/
void AsusTUFLaptopController::SetPowerState(bool boot,
                                            bool awake,
                                            bool sleep,
                                            bool shutdown,
                                            bool save)
{
    unsigned int state = 0xBD;

    if (boot)     state = state | ASUS_WMI_KEYBOARD_POWER_BOOT;
    if (awake)    state = state | ASUS_WMI_KEYBOARD_POWER_AWAKE;
    if (sleep)    state = state | ASUS_WMI_KEYBOARD_POWER_SLEEP;
    if (shutdown) state = state | ASUS_WMI_KEYBOARD_POWER_SHUTDOWN;

    if (save)     state = state | ASUS_WMI_KEYBOARD_POWER_SAVE;

    AsWMI_NB_DeviceControl(ASUS_WMI_DEVID_TUF_RGB_STATE, state);
}

#endif // _WIN32
