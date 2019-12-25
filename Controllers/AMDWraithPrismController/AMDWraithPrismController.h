/*-----------------------------------------*\
|  AMDWraithPrismController.h               |
|                                           |
|  Definitions and types for AMD Wraith     |
|  Prism lighting controller                |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/6/2019   |
\*-----------------------------------------*/

#include <string>
#include <libusb-1.0/libusb.h>

class AMDWraithPrismController
{
public:
    AMDWraithPrismController(libusb_device_handle* dev_handle);
    ~AMDWraithPrismController();

    char* GetDeviceName();

    std::string GetEffectChannelString(unsigned char channel);
    std::string GetFirmwareVersionString();

    void SetRingEffectChannel(unsigned char channel);

    void SetFanColor(unsigned char red, unsigned char green, unsigned char blue);
    void SetLogoColor(unsigned char red, unsigned char green, unsigned char blue);

    void SendEnableCommand();
    void SendRemapCommand();
    void SendEffectCommand();

private:
    char                    device_name[32];
    libusb_device_handle*   dev;
};
