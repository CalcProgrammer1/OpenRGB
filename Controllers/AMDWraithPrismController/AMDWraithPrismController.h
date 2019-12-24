/*-----------------------------------------*\
|  AMDWraithPrismController.h               |
|                                           |
|  Definitions and types for AMD Wraith     |
|  Prism lighting controller                |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/6/2019   |
\*-----------------------------------------*/

#include <libusb-1.0/libusb.h>

class AMDWraithPrismController
{
public:
    AMDWraithPrismController(libusb_device_handle* dev_handle);
    ~AMDWraithPrismController();

    char* GetDeviceName();

    void SendEnableCommand();
    void SendRemapCommand();
    void SendEffectCommand();

private:
    char                    device_name[32];
    libusb_device_handle*   dev;
};
