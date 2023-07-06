#ifndef FANCONTROLLER_ASUSTUFLAPTOPWMI_H
#define FANCONTROLLER_ASUSTUFLAPTOPWMI_H

#include "FanController.h"
#include "AsusTUFLaptopController_Windows.h"

class FanController_AsusTUFLaptopWMI : public FanController
{
private:
    AsusTUFLaptopController* controller;

public:
    FanController_AsusTUFLaptopWMI(AsusTUFLaptopController* dev);
    void UpdateControl() override;
    void UpdateReading() override;
};

#endif // FANCONTROLLER_ASUSTUFLAPTOPWMI_H
