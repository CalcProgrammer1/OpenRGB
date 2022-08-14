#ifndef RGBCONTROLLER_FAUSTUS_H
#define RGBCONTROLLER_FAUSTUS_H

#include "RGBController.h"
#include <fstream>

enum
{
    FAUSTUS_MODE_STATIC      = 0,
    FAUSTUS_MODE_BREATHING   = 1,
    FAUSTUS_MODE_COLOR_CYCLE = 2,
    FAUSTUS_MODE_STROBE      = 3
};
enum
{
    FAUSTUS_SPEED_SLOWEST = 0,
    FAUSTUS_SPEED_NORMAL  = 1,
    FAUSTUS_SPEED_FASTEST = 2,
};

class RGBController_Faustus : public RGBController
{
    private:
        std::string r_path;
        std::string g_path;
        std::string b_path;
        std::string mode_path;
        std::string flags_path;
        std::string set_path;

    public:
        RGBController_Faustus(const std::string& dev_path);

        void        SetupZones();

        void        ResizeZone(int zone, int new_size);
        
        void        DeviceUpdateLEDs();
        void        UpdateZoneLEDs(int zone);
        void        UpdateSingleLED(int led);

        void        DeviceUpdateMode();
};

#endif // RGBCONTROLLER_FAUSTUS_H
