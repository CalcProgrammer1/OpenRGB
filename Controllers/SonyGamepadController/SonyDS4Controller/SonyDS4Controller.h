/*-----------------------------------------*\
|  SonyDS4Controller.h                      |
|                                           |
|  Controller for Sony Dualshock 4          |
|                                           |
|  Pol Rius (alpemwarrior) 24/09/2020       |
\*-----------------------------------------*/

#include <hidapi/hidapi.h>

#include <string>

class SonyDS4Controller
{
public:
    SonyDS4Controller(hid_device * device_handle, const char * device_path);
    ~SonyDS4Controller();

    std::string GetLocation();
    std::string GetSerialString();

    void SetColors(unsigned char red, unsigned char green, unsigned char blue);

private:
    hid_device*     device_handle;
    bool            is_bluetooth = false;
    std::string     location;

    void sendReportUSB(unsigned char red, unsigned char green, unsigned char blue);
    void sendReportBT(unsigned char red, unsigned char green, unsigned char blue);
};
