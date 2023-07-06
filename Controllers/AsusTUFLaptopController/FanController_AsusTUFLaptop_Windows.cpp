#include "FanController_AsusTUFLaptop_Windows.h"


FanController_AsusTUFLaptopWMI::FanController_AsusTUFLaptopWMI(AsusTUFLaptopController* dev)
{
    controller = dev;
    name = "ASUS TUF Laptop";
    description = "WMI Device";
    location    = "\\\\.\\ATKACPI";

    // Coolers RPM reading & current mode retrieval is still WIP
    fans.resize(2);
    fans[0].name = "CPU Cooler";
    fans[0].speed_min = 0;
    fans[0].speed_max = 2;
    fans[0].speed_cmd = 1;
    fans[0].prev_speed_cmd = 1;
    fans[0].rpm_rdg = 0; // We don't have it, but we don't want an unitialized value either
}

void FanController_AsusTUFLaptopWMI::UpdateControl()
{
    int mode = fans[0].speed_cmd;
    switch(mode)
    {
    case 0:
        mode = ASUS_WMI_FAN_SPEED_SILENT;
        break;
    case 1:
        mode = ASUS_WMI_FAN_SPEED_NORMAL;
        break;
    case 2:
        mode = ASUS_WMI_FAN_SPEED_TURBO;
        break;
    }

    controller->setFanMode(mode);
}

void FanController_AsusTUFLaptopWMI::UpdateReading()
{
    ;
}

