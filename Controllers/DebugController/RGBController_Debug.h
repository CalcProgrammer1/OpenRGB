#ifndef RGBCONTROLLER_DEBUG_H
#define RGBCONTROLLER_DEBUG_H

#include "RGBController_Dummy.h"

// A variation of Dummy controller that allows the zones to be resized

class RGBController_Debug : public RGBController_Dummy
{
public:
    RGBController_Debug();
    void ResizeZone(int zone, int newSize) override;
};

#endif // RGBCONTROLLER_DEBUG_H
