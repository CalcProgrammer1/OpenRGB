#include "Detector.h"
#include "RedragonK556Controller.h"
#include "RedragonM711Controller.h"
#include "RGBController.h"
#include "RGBController_RedragonK556.h"
#include "RGBController_RedragonM711.h"
#include <vector>
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define REDRAGON_KEYBOARD_VID           0x0C45
#define REDRAGON_KEYBOARD_USAGE_PAGE    0xFF1C
#define REDRAGON_K550_PID               0x5204
#define REDRAGON_K552_PID               0x5104
#define REDRAGON_K556_PID               0x5004
#define TECWARE_PHANTOM_ELITE_PID       0x652F
#define WARRIOR_KANE_TC235              0x8520

/*-----------------------------------------------------*\
| Mouse product IDs                                     |
\*-----------------------------------------------------*/
#define REDRAGON_MOUSE_VID              0x04D9
#define REDRAGON_MOUSE_USAGE_PAGE       0xFFA0
#define REDRAGON_M711_PID               0xFC30
#define REDRAGON_M715_PID               0xFC39
#define REDRAGON_M908_PID               0xFC4D

/******************************************************************************************\
*                                                                                          *
*   DetectRedragonControllers                                                              *
*                                                                                          *
*       Tests the USB address to see if a Redragon RGB Keyboard controller exists there.   *
*                                                                                          *
\******************************************************************************************/

void DetectRedragonKeyboards(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if( dev )
    {
        RedragonK556Controller* controller = new RedragonK556Controller(dev, info->path);
        RGBController_RedragonK556* rgb_controller = new RGBController_RedragonK556(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectRedragonMice(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if( dev )
    {
        RedragonM711Controller* controller = new RedragonM711Controller(dev, info->path);
        RGBController_RedragonM711* rgb_controller = new RGBController_RedragonM711(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

/*---------------------------------------------------------------------------------------------------------------------------------------------*\
| Keyboards                                                                                                                                     |
\*---------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IP("Redragon K550 Yama",      DetectRedragonKeyboards, REDRAGON_KEYBOARD_VID, REDRAGON_K550_PID,         1, REDRAGON_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Redragon K552 Kumara",    DetectRedragonKeyboards, REDRAGON_KEYBOARD_VID, REDRAGON_K552_PID,         1, REDRAGON_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Redragon K556 Devarajas", DetectRedragonKeyboards, REDRAGON_KEYBOARD_VID, REDRAGON_K556_PID,         1, REDRAGON_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Tecware Phantom Elite",   DetectRedragonKeyboards, REDRAGON_KEYBOARD_VID, TECWARE_PHANTOM_ELITE_PID, 1, REDRAGON_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Warrior Kane TC235",      DetectRedragonKeyboards, REDRAGON_KEYBOARD_VID, WARRIOR_KANE_TC235,        1, REDRAGON_KEYBOARD_USAGE_PAGE);
/*---------------------------------------------------------------------------------------------------------------------------------------------*\
| Mice                                                                                                                                          |
\*---------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IP("Redragon M711 Cobra",     DetectRedragonMice,      REDRAGON_MOUSE_VID,    REDRAGON_M711_PID,         2, REDRAGON_MOUSE_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Redragon M715 Dagger",    DetectRedragonMice,      REDRAGON_MOUSE_VID,    REDRAGON_M715_PID,         2, REDRAGON_MOUSE_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Redragon M908 Impact",    DetectRedragonMice,      REDRAGON_MOUSE_VID,    REDRAGON_M908_PID,         2, REDRAGON_MOUSE_USAGE_PAGE);
/*---------------------------------------------------------------------------------------------------------------------------------------------*\
| Mousemats                                                                                                                                     |
\*---------------------------------------------------------------------------------------------------------------------------------------------*/
