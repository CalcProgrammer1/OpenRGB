/*---------------------------------------------------------*\
| RGBController_GigabyteSuperIORGB.cpp                      |
|                                                           |
|   RGBController for Gigabyte Aorus Super IO motherboard   |
|                                                           |
|   Ryan Frankcombe (422gRdHuX5uk)              11 Sep 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_GigabyteSuperIORGB.h"

/**------------------------------------------------------------------*\
    @name Gigabyte SuperIO RGB
    @category Motherboard
    @type SuperIO
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectGigabyteSuperIORGBControllers
    @comment
        Testing was done on an ITE8688 Chipset, and adds the support for motherboards that can only be controlled with Gigabyte’s “Ambient Led” NOT EITHER RGB Fusion 1.0/2.0.

        You should first check to confirm that you DO NOT HAVE EITHER A USB OR SMBUS controllable chipset, use the following powershell command to confirm if you have a USB device:
        ```powershell
        gwmi Win32_USBControllerDevice |%{[wmi]($_.Dependent)} | Sort Manufacturer,Description,DeviceID | Ft -GroupBy Manufacturer Description,Service,DeviceID
        ```

        If you see anything with a VID of 048D or 8297, DO NOT PROCEED, the software is likely supportable by OpenRGB's Fusion2 USB controller.

        After that check output on the SMBus: Using the OpenRGB I2C Sniffer to confirm that no data is output on either bus while changing colors in “Ambient LED”, if either bus outputs data, DO NOT PROCEED, the software is likely supportable in Gigabyte RGB Fusion.

        The following chipsets are likely supported:
        | Chipset ID |
        | :---:      |
        | ITE8620E   |
        | ITE8626    |
        | ITE8686E   |
        | ITE8688    |
        | ITE8689    |
        | ITE8728F   |
        | ITE8790F   |
        | ITE8791E   |

        To confirm your chipset, you can open CPU-Z, the MCU chipset model is found under Mainboard like below:

        To confirm that your RGB is compatible with this Controller in OpenRGB, you will need to add the motherboard DMI Model name, which is the output of the following powershell command:

        ```powershell
        wmic baseboard get product
        ```

        To the struct array that is found in GigabyteSuperIORGBControllerDetect.cpp, like below

        ```c++
        gig_device compatible_devices[] =
        {
            {"X570 UD"},
            {"EXAMPLEBASEBOARDPRODUCTNAME"}
        };
        ```

         If your chipset is also NOT an ITE8688, you will also need to add the chipset, to get this is a bit harder, for myself I added a breakpoint, on this line, in GigabyteSuperIORGBControllerDetect.cpp:
         ```c++
                switch (val & SIO_ID_MASK)
        ```

        For an ITE8688, the value was 0x8688 in Hexidecimal, it is likely the case with other models as well that the code matches the MCU model.

        To add this chipset you will need to first add it to the SuperIO definitions file, which is named super_io.h like below
        ```c++
        #define SIO_ITE8688_ID              0x8688  // Device ID for ITE8688  (8688)
        #define SIO_NEWCHIPSETMODEL_ID      0xFOUNDHEXIDECIMALVALUE  // Device ID for NEWCHIPSETMODEL  (FOUNDHEXIDECIMALVALUE)
        ```

        And lastly you will need to add the chip to the

        GigabyteSuperIORGBControllerDetect.cpp like below:
        ```c++
                switch (val & SIO_ID_MASK)
                {
                    case SIO_ITE8688_ID:
                    case SIO_NEWCHIPSETMODEL_ID:
        ```
\*-------------------------------------------------------------------*/

RGBController_GigabyteSuperIORGB::RGBController_GigabyteSuperIORGB(GigabyteSuperIORGBController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetDeviceName();
    vendor                  = "Gigabyte";
    type                    = DEVICE_TYPE_MOTHERBOARD;
    description             = "Gigabyte SuperIO RGB Device";
    location                = controller->GetDeviceLocation();

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = 0;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name             = "Static";
    Static.value            = GIGABYTE_MODE1_STATIC;
    Static.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Rainbow;
    Rainbow.name            = "Rainbow";
    Rainbow.value           = GIGABYTE_MODE1_RAINBOW;
    Rainbow.flags           = MODE_FLAG_HAS_PER_LED_COLOR;
    Rainbow.color_mode      = MODE_COLORS_PER_LED;
    modes.push_back(Rainbow);

    mode Breathing;
    Breathing.name          = "Breathing";
    Breathing.value         = GIGABYTE_MODE1_BREATHING;
    Breathing.flags         = MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.color_mode    = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode Flashing;
    Flashing.name           = "Flashing";
    Flashing.value          = GIGABYTE_MODE1_FLASHING;
    Flashing.flags          = MODE_FLAG_HAS_PER_LED_COLOR;
    Flashing.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(Flashing);

    SetupZones();
}

RGBController_GigabyteSuperIORGB::~RGBController_GigabyteSuperIORGB()
{
    delete controller;
}

void RGBController_GigabyteSuperIORGB::SetupZones()
{
    zone gig_zone;
    gig_zone.name           = "Gigabyte Zone";
    gig_zone.type           = ZONE_TYPE_SINGLE;
    gig_zone.leds_min       = 1;
    gig_zone.leds_max       = 1;
    gig_zone.leds_count     = 1;
    gig_zone.matrix_map     = NULL;
    zones.push_back(gig_zone);

    led gig_led;
    gig_led.name            = "LED_C1";
    leds.push_back(gig_led);

    SetupColors();
}

void RGBController_GigabyteSuperIORGB::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_GigabyteSuperIORGB::DeviceUpdateLEDs()
{
    RGBColor      color = colors[0];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    controller->SetColor(red, grn, blu);
}

void RGBController_GigabyteSuperIORGB::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_GigabyteSuperIORGB::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_GigabyteSuperIORGB::DeviceUpdateMode()
{
    controller->SetMode(modes[active_mode].value);
}
