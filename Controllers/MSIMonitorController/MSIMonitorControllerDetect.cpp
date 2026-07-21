/*---------------------------------------------------------*\
| MSIMonitorControllerDetect.cpp                            |
|                                                           |
|   Detector for MSI  monitor (MSI Gaming Controller)       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "MSIMonitorController.h"
#include "RGBController_MSIMonitor.h"
#include "RGBController_MSIMonitor72.h"

#define MSI_USB_VID         0x1462
#define MSI_USB_PID         0x3FA4

/*---------------------------------------------------------*\
| Vendor layout table (from the version -> layout map).     |
| The firmware reports its version to the 0xB0 query but    |
| not a LED count, so count/direction/block-count come      |
| from here.  Fields: version, LED count, fill direction    |
| (MSI_MONITOR_72_DIR_*), dual_block.                       |
|                                                           |
| Every entry here is a single linear run driven through    |
| the 168 byte 0x72 buffer.  Direction mirrors the fill so  |
| OpenRGB LED 0 is the left/start on every model.  Circle   |
| and Ctype have no simple line mapping - they are driven   |
| as a plain line here and should get a custom per model    |
| layout once an owner maps the physical shape.             |
|                                                           |
| Only version 34 is hardware verified (MPG 322URX).  The   |
| rest are derived from the vendor table: same buffer, LED  |
| count and direction are the only differences, so effect   |
| and single colour modes are correct by construction.      |
|                                                           |
| Left out on purpose:                                      |
|  - dual_block versions 11/13/15 are stubbed but disabled  |
|    (see the commented block); their two control blocks    |
|    drive two runs and the per LED split is unconfirmed.   |
|  - Advanced/0x82 versions 31/47 need the 0x30 bulk report |
|    this controller does not implement.                    |
| A version not matched here falls through to the legacy    |
| 0x71 controller, home of the MAG272 class.                |
\*---------------------------------------------------------*/
struct MSIMonitor72Layout
{
    uint8_t         version;
    unsigned int    leds;
    unsigned int    direction;
    bool            dual_block;
};

static const MSIMonitor72Layout msi_monitor_72_layouts[] =
{
    { 34,  9, MSI_MONITOR_72_DIR_LEFT_TO_RIGHT, false },    /* MPG271QRXQD_OLED (MPG 322URX reports this) - verified */
    {  5, 12, MSI_MONITOR_72_DIR_RIGHT_TO_LEFT, false },    /* MAG272R */
    {  8, 24, MSI_MONITOR_72_DIR_CIRCLE,        false },    /* MAG301CR - Circle, wants custom layout */
    { 10,  9, MSI_MONITOR_72_DIR_LEFT_TO_RIGHT, false },    /* MAG273R */
    { 12,  9, MSI_MONITOR_72_DIR_LEFT_TO_RIGHT, false },    /* MAG271VCR */
    { 14, 14, MSI_MONITOR_72_DIR_LEFT_TO_RIGHT, false },    /* NXG253R */
    { 17,  8, MSI_MONITOR_72_DIR_RIGHT_TO_LEFT, false },    /* MAG2x5R */
    { 20, 12, MSI_MONITOR_72_DIR_LEFT_TO_RIGHT, false },    /* MAG301RF */
    { 21, 18, MSI_MONITOR_72_DIR_LEFT_TO_RIGHT, false },    /* MPG341QR */
    { 22, 24, MSI_MONITOR_72_DIR_LEFT_TO_RIGHT, false },    /* MPG321QRF */
    { 24, 12, MSI_MONITOR_72_DIR_LEFT_TO_RIGHT, false },    /* MAG321QR */
    { 25, 24, MSI_MONITOR_72_DIR_LEFT_TO_RIGHT, false },    /* MPG321UR */
    { 28,  9, MSI_MONITOR_72_DIR_LEFT_TO_RIGHT, false },    /* MAG273CQR */
    { 29, 24, MSI_MONITOR_72_DIR_RIGHT_TO_LEFT, false },    /* MAG401QR */
    { 30, 21, MSI_MONITOR_72_DIR_RIGHT_TO_LEFT, false },    /* MAG325CQRF */
    { 32,  7, MSI_MONITOR_72_DIR_LEFT_TO_RIGHT, false },    /* MAG274QRF */
    { 33,  7, MSI_MONITOR_72_DIR_LEFT_TO_RIGHT, false },    /* MPG274URFQD */
    { 36,  9, MSI_MONITOR_72_DIR_LEFT_TO_RIGHT, false },    /* MPG275CQRXF */
    { 37, 22, MSI_MONITOR_72_DIR_LEFT_TO_RIGHT, false },    /* MPG345CQRD */
    { 38, 20, MSI_MONITOR_72_DIR_LEFT_TO_RIGHT, false },    /* MAG325CQRFQDE2 */
    { 39,  8, MSI_MONITOR_72_DIR_RIGHT_TO_LEFT, false },    /* MPG321CURX_QD_OLED */
    { 40, 12, MSI_MONITOR_72_DIR_LEFT_TO_RIGHT, false },    /* MAG322URDFE16 */
    { 41, 14, MSI_MONITOR_72_DIR_RIGHT_TO_LEFT, false },    /* MPG242RX60N */
    { 42, 22, MSI_MONITOR_72_DIR_CTYPE,         false },    /* MPG274URDFWE16 - Ctype, wants custom layout */
    { 43, 21, MSI_MONITOR_72_DIR_LEFT_TO_RIGHT, false },    /* MPG346CQRFX24 */
    { 44,  9, MSI_MONITOR_72_DIR_LEFT_TO_RIGHT, false },    /* MPG271QRQD_OLEDX50 */
    { 46,  9, MSI_MONITOR_72_DIR_LEFT_TO_RIGHT, false },    /* MPG341CQRQD_OLEDX36 */

    /*-----------------------------------------------------------------*\
    | Dual block - stubbed but DISABLED.  The dual_block path fills     |
    | both colour arrays so uniform colour modes are correct, but each  |
    | block drives its own run and the per LED split is unconfirmed, so |
    | Direct is a guess.  These versions also map colour index above 15 |
    | to a separate DragonShield emblem region (unhandled).  An owner   |
    | should enable, test, and ideally give it a custom layout:         |
    |   { 11, 16, MSI_MONITOR_72_DIR_RIGHT_TO_LEFT, true },  MPG273CQR  |
    |   { 13, 21, MSI_MONITOR_72_DIR_LEFT_TO_RIGHT, true },  MPG343CQR  |
    |   { 15, 18, MSI_MONITOR_72_DIR_RIGHT_TO_LEFT, true },  MPG323CQR  |
    |                                                                   |
    | Advanced/0x82 - not addable here, need the 0x30 bulk report:      |
    |   version 31, 40 LEDs, MEG342CQD_OLED                             |
    |   version 47, 83 LEDs, 4 areas, MEG_X                             |
    \*-----------------------------------------------------------------*/
};

static const MSIMonitor72Layout* MSIMonitorLayout(uint8_t version)
{
    for(unsigned int i = 0; i < (sizeof(msi_monitor_72_layouts) / sizeof(msi_monitor_72_layouts[0])); i++)
    {
        if(msi_monitor_72_layouts[i].version == version)
        {
            return(&msi_monitor_72_layouts[i]);
        }
    }

    return(nullptr);
}

DetectedControllers DetectMSIMonitorController(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        /*-------------------------------------------------*\
        | One VID/PID covers several buffer layouts, so ask |
        | the MCU which one it speaks.  A version in the    |
        | 0x72 table selects the 168 byte layout and its    |
        | LED count; anything else keeps the legacy 0x71    |
        | path, which is what the older monitors answer to. |
        \*-------------------------------------------------*/
        MSIMonitorController* controller = new MSIMonitorController(dev, *info, name);

        uint8_t                     version = controller->GetLayoutVersion();
        const MSIMonitor72Layout*   layout  = MSIMonitorLayout(version);

        if(layout != nullptr)
        {
            detected_controllers.push_back(new RGBController_MSIMonitor72(controller, layout->leds, layout->direction, layout->dual_block));
        }
        else
        {
            /*-------------------------------------------------*\
            | 0x71 family (78 byte buffer), handled by the      |
            | legacy controller as a plain line.  For an owner  |
            | who wants to bring one onto this controller: same |
            | operative block 12, but the second colour array   |
            | is at index 50 (not 95) and store-flash at 77.    |
            | RightToLeft mirroring applies.  version, LEDs,    |
            | direction:                                        |
            |   1,  9, RightToLeft   (early/generic)            |
            |   2,  7, LeftToRight   (malformed stub, skip)     |
            |   3,  9, RightToLeft   (early/generic)            |
            |   4,  9, LeftToRight   (early/generic)            |
            |   6,  9, UpToDown      MAG270CR                   |
            |   7,  7, LeftToRight   MAG251RX                   |
            \*-------------------------------------------------*/
            detected_controllers.push_back(new RGBController_MSIMonitor(controller));
        }
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IPU("MSI Monitor (Gaming Controller)", DetectMSIMonitorController, MSI_USB_VID, MSI_USB_PID, 0, 0x01, 0);
