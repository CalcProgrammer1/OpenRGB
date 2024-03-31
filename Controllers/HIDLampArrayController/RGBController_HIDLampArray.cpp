#include "RGBController_HIDLampArray.h"

RGBController_HIDLampArray::RGBController_HIDLampArray(HIDLampArrayController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "HID LampArray Device";
    vendor      = "Generic";
    type        = DEVICE_TYPE_MOUSEMAT;
    description = "HID LampArray Device";
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_HIDLampArray::~RGBController_HIDLampArray()
{

}

void RGBController_HIDLampArray::SetupZones()
{

}

void RGBController_HIDLampArray::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_HIDLampArray::DeviceUpdateLEDs()
{

}

void RGBController_HIDLampArray::UpdateZoneLEDs(int /*zone*/)
{

}

void RGBController_HIDLampArray::UpdateSingleLED(int /*led*/)
{

}

void RGBController_HIDLampArray::DeviceUpdateMode()
{

}
