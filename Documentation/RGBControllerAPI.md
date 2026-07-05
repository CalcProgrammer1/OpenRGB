# RGBController API

Device support in OpenRGB can be broken down into three major components.

* Controller
* Detector
* RGBController

## **Controller**

A device's Controller class is a free-form class that provides whatever functionality is necessary to communicate with a device.  This class should implement functions to send control packets to a device and receive information packets from a device.  It should provide the capability to set device colors and modes.  The Controller header file should provide defined constants for mode, speed, and other control values specific to the device's protocol.  If possible, this class should provide the capability to retrieve firmware version and serial number information from the device.  This class can also provide additional device protocol functionality even if it goes unused in OpenRGB currently.  For instance, you may provide functions for controlling mouse DPI, polling rate, fan speed, or any other device-specific capability you want.  If OpenRGB ever implements these extra functions in the future, having them implemented already in the Controller will make that easier.

The Controller class files are kept in the Controllers/ folder.

## **Detector**

A device's Detector function scans the system's interfaces to see if a particular device (Controller/RGBController) exists.  Several types of detectors exist and are listed below.  Each detector type is passed different arguments based on the interface it is detecting.  The REGISTER_DETECTOR macros are used to register a detector function with the ResourceManager which is responsible for calling detector functions at detection time.  Detector functions are then responsible for creating instances of Controllers and RGBControllers and registering them with the ResourceManager by calling the `ResourceManager::RegisterRGBController` interface.

HID Detectors

HID (Human Interface Device) is the most common interface for USB devices with RGB capabilities, especially for peripherals such as keyboards and mice.  While it is usually used over USB, HID can also be used over Bluetooth.  The `hidapi` library is used for interfacing with HID devices.  The following detector formats can be registered:

```C++
REGISTER_HID_DETECTOR("HID Detector Name", DetectHIDDevicesFunction, HID_VID, HID_PID);
REGISTER_HID_DETECTOR_I("HID Detector Name", DetectHIDDevicesFunction, HID_VID, HID_PID, HID_INTERFACE);
REGISTER_HID_DETECTOR_IP("HID Detector Name", DetectHIDDevicesFunction, HID_VID, HID_PID, HID_INTERFACE, HID_PAGE);
REGISTER_HID_DETECTOR_IPU("HID Detector Name", DetectHIDDevicesFunction, HID_VID, HID_PID, HID_INTERFACE, HID_PAGE, HID_USAGE);
REGISTER_HID_DETECTOR_P("HID Detector Name", DetectHIDDevicesFunction, HID_VID, HID_PID, HID_PAGE);
REGISTER_HID_DETECTOR_PU("HID Detector Name", DetectHIDDevicesFunction, HID_VID, HID_PID, HID_PAGE, HID_USAGE);
```

The I/IP/IPU/P/PU variants add filtering for specific HID interfaces, pages, and usages as many HID devices expose multiple interfaces and not all are used for RGB control.

I2C/SMBus Detectors

I2C (Inter-Integrated Circuit), or SMBus (System Management Bus, a compatible subset of I2C), is the second most common interface used by RGB devices and is used for on-board RGB on certrain motherboards, most graphics cards, and all RAM modules.  Each I2C device has a 7-bit address.  As I2C does not offer a standardized means of identifying a device on the bus, we have several different options for detecting I2C devices that can narrow down the search to a specific I2C bus.

```C++
REGISTER_I2C_DETECTOR("I2C Detector Name", DetectI2CDevicesFunction);
REGISTER_I2C_DIMM_DETECTOR("I2C Detector Name", DetectI2CDevicesFunction, JEDEC_ID, DIMM_TYPE);
REGISTER_I2C_PCI_DETECTOR("I2C Detector Name", DetectI2CDevicesFunction, PCI_VEN, PCI_DEV, PCI_SUBVEN, PCI_SUBDEV, I2C_ADDR);
```

The standard version of the I2C detector calls the detector function with a vector of all available I2C buses.  The detector can then perform any chip specific detection necessary to determine if the device exists on any of the given buses.  Only use this version of the detector if the DIMM or PCI variants are not suitable for your device.  There are additional macros that can be used to narrow down I2C bus detection such as `IF_MOBO_SMBUS` for motherboard buses and `IF_DRAM_SMBUS` for DRAM buses.

The DIMM version of the detector can be used to filter for specific DRAM modules using SPD information.  Only the I2C bus for the DRAM will be provided and the detector will only be called if the JEDEC ID and DIMM type match.

The PCI version of the detector can be used to filter for I2C devices on specific PCI cards, usually graphics cards.  The detector will only be called for I2C buses with matching PCI IDs.  The detector can also provide a specific address, though it is possible for the detector function to ignore this address if more complex address determination is needed.

Generic Detectors

The generic detector type is used for any device that doesn't fit into one of the previous detection types.  This detector is frequently used for manually configured devices such as network and serial port devices.  It is also used for USB devices that cannot be accessed via `hidapi` or serial and instead requiring direct USB access via `libusb`.

```C++
REGISTER_DETECTOR("Generic Detector Name", DetectDevicesFunction);
```

The Detector files are kept in the Controllers/ folder.

## **RGBController**

OpenRGB uses an internal API called RGBController to standardize the interface to RGB devices from multiple vendors and categories.  This API uses vectors to describe each device.  This API is implemented as an RGBController class that is inherited by each implementation, for example the RGBController_CorsairPeripheral is defined like so:

```C++
#include "RGBController.h"

class RGBController_CorsairPeripheral : public RGBController
{
```

The RGBController files for a controller implementation are kept in the Controllers/ folder alongside the Controller and Detector files.

The RGBController class inherits from the RGBControllerInterface abstract base class, which defines the public API.  The RGBController class specification contains the following:

* Device Name
* Device Vendor
* Device Description
* Device Version
* Device Serial
* Device Location
* Device Display Name
* Vector of LEDs
* Vector of Zones
* Vector of Modes
* Vector of Colors (32-bit 0x00BBGGRR format)
* Device Type (enum)
* Active mode index
* Vector of LED Display Names
* Controller Flags
* Device-Specific Configuration (JSON string)

### Device Types

| Value | Description   |
| ----- | ------------- |
| 0     | Motherboard   |
| 1     | DRAM          |
| 2     | GPU           |
| 3     | Cooler        |
| 4     | LED Strip     |
| 5     | Keyboard      |
| 6     | Mouse         |
| 7     | Mousemat      |
| 8     | Headset       |
| 9     | Headset Stand |
| 10    | Gamepad       |
| 11    | Light         |
| 12    | Speaker       |
| 13    | Virtual       |
| 14    | Storage       |
| 15    | Case          |
| 16    | Microphone    |
| 17    | Accessory     |
| 18    | Keypad        |
| 19    | Laptop        |
| 20    | Monitor       |
| 21    | Unknown       |

Additional device types may be added in the future.  They are added after the last known device type.  Anything out of range should be considered Unknown.

### Controller Flags

| Controller Flags Bit | Name                                  | Description                                                       |
| -------------------- | ------------------------------------- | ----------------------------------------------------------------- |
| 0                    | Local                                 | Controller is provided by this OpenRGB instance                   |
| 1                    | Remote                                | Controller is provided by a remote OpenRGB instance               |
| 2                    | Virtual                               | Controller is virtual (not a physical device)                     |
| 3                    | Hidden                                | Controller is hidden from the user interface                      |
| 8                    | Reset Before Update                   | Device resets update flag before calling update function          |
| 16                   | Manually Configurable Name            | Device name is manually configurable                              |
| 17                   | Manually Configurable Device Specific | Device device-specific config is manually configurable            |
| 24                   | Manually Configured Name              | Device name has been manually configured                          |
| 25                   | Manually Configured Device Specific   | Device device-specific config has been manually configured        |

### LED Display Names

The LED Display Names vector (`led_display_names`) can override the base name of an LED.  The intended use case for this field is providing regional key names for non-English keyboard layouts.  The base key names should always be provided in English QWERTY layout for positional mapping to work on certain SDK applications, so the display names field can override the base name to provide the correct key name for the localized layout without disrupting SDK application mapping.  If not overriding any LED names, this vector can be left empty.  If only overriding certain LED names, those not being overridden can be empty strings.  If used, the length of this vector must equal the length of the LEDs vector.

## LEDs

The LED structure contains information about an LED.

* LED Name
* LED Value

The Value has no defined functionality in the RGBController API and is provided for implementation-specific use.  You can use this field to associate implementation-specific data with an LED.

## Zones

The Zone structure contains information about a zone.  A zone is a logical grouping of LEDs defined by the RGBController implementation.  LEDs in a zone must be contiguous in the RGBController's LEDs/Colors vectors.

* Zone Name
* Zone Display Name
* Zone Type
* LED pointer
* Color pointer
* Start Index
* LED Count
* Minimum number of LEDs
* Maximum number of LEDs
* Matrix map pointer
* Vector of segments
* Zone Flags
* Vector of zone-specific modes
* Active zone-specific mode index

The LED pointer and Color pointer point to the first LED/Color in the RGBController's LEDs/Colors vector associated with this zone.  The Start Index is the index to the same LED/Color in the vectors.

The LED count is the number of LEDs in the zone.  For zones with a fixed number of LEDs, the count, min, and max values should all be equal.  For zones with a manually configurable number of LEDs, the count should be between the min and max values, inclusively.  The `ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE` flag must be set to allow the user to adjust the number of LEDs.  Zones with manually configurable size are most commonly used to represent addressable RGB (ARGB) controllers as the number of LEDs depends on what strips/devices are attached to the ARGB headers.  The ConfigureZone function in the RGBController API is used to resize the number of LEDs in the zone.  The initial value should be zero for ARGB zones if the device does not provide a means to automatically determine the number of connected LEDs.

If the zone has the `ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE_EFFECTS_ONLY` flag is set, the zone allows the user to manually configure an LED count between the min and max size, but for the purposes of per-LED modes, the zone is treated as only 0 or 1 LEDs.  This flag is used on certain devices that allow configuring the number of LEDs on an ARGB header but do not allow direct control over each LED's color.

The Zone Display Name can override the base name of a zone.  If the zone has the `ZONE_FLAG_MANUALLY_CONFIGURED_NAME` flag set, the user can manually configure the display name and the display name is used instead of the base name.

Zones can also have their own set of modes and an active mode index.  If a zone has modes defined, the device supports per-zone mode control, allowing different effects to run on different zones of the same device.  The active mode is set to -1 when the zone should use the device-level mode instead.

### Zone Types

The zone type enum defines the zone type.  This describes the physical layout of the zone and can be used by software to generate appropriate effects for the zone.

| Zone Type Value | Description      |
| --------------- | ---------------- |
| 0               | Single           |
| 1               | Linear (1D)      |
| 2               | Matrix (2D)      |
| 3               | Linear Loop      |
| 4               | Matrix Loop X    |
| 5               | Matrix Loop Y    |
| 6               | Segmented        |

If the zone has the `ZONE_FLAG_MANUALLY_CONFIGURABLE_TYPE` flag set, the user can manually configure the zone type.
### Matrix Map

Each zone has a matrix map pointer which allows an optional matrix map to be associated with the zone.  The matrix map is used to provide positioning information about LEDs in a 2D grid.  If a matrix map is not provided for a zone, the zone's matrix map pointer must be set to NULL.

A matrix map has the following:

* Height
* Width
* Map data pointer

The height and width determine the size of the map data.  The map data pointer should point to a data block of (Height * Width) unsigned 32-bit integers.  This data can be accessed as if it were a Map[Y][X] 2D array.  The values of the map are LED index values in the zone (so offset by Start Index from the RGBController's LEDs vector).  If a spot in the matrix is unused and does not map to an LED, it should be set to 0xFFFFFFFF.

If the zone has the `ZONE_FLAG_MANUALLY_CONFIGURABLE_MATRIX_MAP` flag set, the user can manually configure the zone's matrix map.

### Segments

Each Zone contains a vector of Segments.  Segments can be used to divide a physical zone (such as an ARGB header) into multiple logical sub-zones, or segments.  This is mainly used for ARGB zones with multiple components daisy-chained together.  For example, segments can be used to group multiple rings on an ARGB fan or multiple daisy-chained fans connected to one header.  If the device is capable of automatically detecting multiple components connected to a single output, the RGBController may create segments automatically during zone creation.  Otherwise, leaving this vector empty will indicate that the zone contains no segments, though resizable zones allow the user to define their own segments.

A segment contains the following:

* Segment Name
* Segment Type (See Zone Type values)
* Start Index
* LED Count
* Matrix Map
* Segment Flags

The Start Index is the index within the Zone where the Segment starts.  The LED Count is the number of LEDs in the Segment.  Care should be taken to ensure that the total number of LEDs across all segments equals the number of LEDs in the Zone and the start indices do not overlap.

The Matrix Map provides per-segment positioning information in a 2D grid, similar to the zone-level matrix map.

If the zone has the `ZONE_FLAG_MANUALLY_CONFIGURABLE_SEGMENTS` flag set, the user can manually configure the zone's segments.

### Segment Flags

| Segment Flags Bit | Name         | Description                    |
| ----------------- | ------------ | ------------------------------ |
| 0                 | Group Start  | Start of a segment group       |
| 1                 | Group Member | Segment is part of a group     |

Segments can be grouped by setting the Group Start flag on an empty segment and then setting the Group Member flag on subsequent segments.

### Zone Flags

The Zone Flags field is a bitfield with informational flags related to the Zone.

| Zone Flags Bit | Name                                    | Description                                                                                                                                          |
| -------------- | --------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------- |
| 0              | Manually Configurable Size Effects Only | Zone size is manually configurable, but only for hardware effects modes.  The zone is treated as a single LED in the Colors vector for per-LED modes |
| 1              | Manually Configurable Size              | Zone size is manually configurable                                                                                                                   |
| 2              | Manually Configurable Name              | Zone name is manually configurable                                                                                                                   |
| 3              | Manually Configurable Type              | Zone type is manually configurable                                                                                                                   |
| 4              | Manually Configurable Matrix Map        | Zone matrix map is manually configurable                                                                                                             |
| 5              | Manually Configurable Segments          | Zone segments are manually configurable                                                                                                              |
| 6              | Manually Configurable Device Specific   | Zone device-specific config is manually configurable                                                                                                 |
| 12             | Manually Configured Size                | Zone size has been manually configured                                                                                                               |
| 13             | Manually Configured Name                | Zone name has been manually configured                                                                                                               |
| 14             | Manually Configured Type                | Zone type has been manually configured                                                                                                               |
| 15             | Manually Configured Matrix Map          | Zone matrix map has been manually configured                                                                                                         |
| 16             | Manually Configured Segments            | Zone segments have been manually configured                                                                                                          |
| 17             | Manually Configured Device Specific     | Zone device-specific config has been manually configured                                                                                             |
| 24             | Zone Geometry May Change                | Zone geometry may change (e.g. hotplug devices)                                                                                                      |

## Modes

Modes represent internal effects and have a name field that describes the effect.  The mode's index in the vector is its ID.  The Active Mode variable in the RGBController class specifies which mode is currently selected.  A mode contains the following:

* Mode Name
* Mode Value
* Mode Flags
* Minimum Speed
* Maximum Speed
* Minimum Brightness
* Maximum Brightness
* Minimum number of colors
* Maximum number of colors
* Speed Value
* Brightness Value
* Direction
* Color Mode
* Colors Vector

### Mode Value

The mode value is field is provided to hold an implementation-defined mode value.  This is usually the mode's value in the hardware protocol.

### Mode Flags

The mode flags field is a bitfield that contains information about what features a mode has.

| Mode Flags Bit | Description                                                    |
| -------------- | -------------------------------------------------------------- |
| 0              | Mode has speed parameter                                       |
| 1              | Mode has left/right direction parameter                        |
| 2              | Mode has up/down direction parameter                           |
| 3              | Mode has horizontal/vertical direction parameter               |
| 4              | Mode has brightness parameter                                  |
| 5              | Mode has per-LED color settings                                |
| 6              | Mode has mode specific color settings                          |
| 7              | Mode has random color option                                   |
| 8              | Mode can be manually saved                                     |
| 9              | Mode automatically saves                                       |
| 10             | Mode always applies to entire device, overrides per-zone modes |

### Mode Speed

The mode minimum and maximum speed fields should be set to the implementation-specific minimum and maximum speed values for the given mode if the mode supports speed control.  The mode speed value field will be set between the minimum and maximum value, inclusively.  The minimum speed may be a greater numerical value than the maximum speed if your device's speed adjustment is inverted (usually because the device takes a delay period rather than a speed value).

### Mode Brightness

The mode minimum and maximum brightness fields should be set to the implementation-specific minimum and maximum brightness values for the given mode if the mode supports brightness control.  The mode brightness value field will be set between the minimum and maximum value, inclusively.

### Mode Colors

The mode minimum and maximum number of colors fields should be used if the mode supports mode-specific color settings.  These determine the size range of the mode's Colors vector.  If the mode has a fixed number of colors, the minimum and maximum should be equal.  Mode-specific colors are used when a mode has one or more configurable colors but these colors do not apply directly to individual LEDs.  Example would be a breathing mode that cycles between one or more colors each breath pulse.  A mode may have multiple color options available, for instance a breathing mode that can either use one or more defined colors or just cycle through random colors.  The available color modes for a given mode are set with the flags.  The selected color mode is set using the color mode field, which can be one of the following values.

### Mode Directions

| Direction Value | Name        |
| --------------- | ----------- |
| 0               | Left        |
| 1               | Right       |
| 2               | Up          |
| 3               | Down        |
| 4               | Horizontal  |
| 5               | Vertical    |

### Color Mode Values

| Color Mode Value | Description                                                                                       |
| ---------------- | ------------------------------------------------------------------------------------------------- |
| 0                | None - this mode does not have configurable colors                                                |
| 1                | Per-LED - this mode uses the RGBController's colors vector to set each LED to its specified color |
| 2                | Mode Specific - this mode has one or more configurable colors, but not individual LED control     |
| 3                | Random - this mode can be switched to a random or cycling color palette                           |

## Update Reason Codes

The SignalUpdate function uses reason codes to indicate what type of change occurred.  These are used by callbacks to determine what action to take.

| Reason Code | Name                                                          | Description                                    |
| ----------- | ------------------------------------------------------------- | ---------------------------------------------- |
| 0           | RGBCONTROLLER_UPDATE_REASON_UPDATELEDS                        | UpdateLEDs() was called                        |
| 1           | RGBCONTROLLER_UPDATE_REASON_UPDATEMODE                        | UpdateMode() was called                        |
| 2           | RGBCONTROLLER_UPDATE_REASON_SAVEMODE                          | SaveMode() was called                          |
| 3           | RGBCONTROLLER_UPDATE_REASON_CONFIGUREZONE                     | ConfigureZone() was called                     |
| 4           | RGBCONTROLLER_UPDATE_REASON_CLEARSEGMENTS                     | ClearSegments() was called                     |
| 5           | RGBCONTROLLER_UPDATE_REASON_ADDSEGMENT                        | AddSegment() was called                        |
| 6           | RGBCONTROLLER_UPDATE_REASON_HIDDEN                            | Hidden flag was set                            |
| 7           | RGBCONTROLLER_UPDATE_REASON_UNHIDDEN                          | Hidden flag was cleared                        |
| 8           | RGBCONTROLLER_UPDATE_REASON_SETDEVICESPECIFICCONFIGURATION    | SetDeviceSpecificConfiguration() was called    |
| 9           | RGBCONTROLLER_UPDATE_REASON_SETDEVICESPECIFICZONECONFIGURATION| SetDeviceSpecificZoneConfiguration() was called|
| 10          | RGBCONTROLLER_UPDATE_REASON_CONFIGUREDEVICE                   | ConfigureDevice() was called                   |

## Functions

### Controller Information Functions

#### `std::string GetName()`

Returns the `name` string of the device.

#### `std::string GetVendor()`

Returns the `vendor` string of the device.

#### `std::string GetDescription()`

Returns the `description` string of the device.

#### `std::string GetVersion()`

Returns the `version` string of the device.

#### `std::string GetSerial()`

Returns the `serial` string of the device.

#### `std::string GetLocation()`

Returns the `location` string of the device.

#### `std::string GetDisplayName()`

Returns the display name of the device.  If the device has a manually configured name (`CONTROLLER_FLAG_MANUALLY_CONFIGURED_NAME`), the display name is returned.  Otherwise, the base name is returned.

#### `device_type GetDeviceType()`

Returns the device type enum value.

#### `controller_flags GetFlags()`

Returns the controller flags bitfield.

### Hidden Flag Functions

#### `bool GetHidden()`

Returns true if the device is hidden (`CONTROLLER_FLAG_HIDDEN` is set).

#### `void SetHidden(bool hidden)`

Sets or clears the hidden flag on the device.  Signals an update with `RGBCONTROLLER_UPDATE_REASON_HIDDEN` or `RGBCONTROLLER_UPDATE_REASON_UNHIDDEN`.

### Zone Functions

#### `zone GetZone(unsigned int zone_idx)`

Returns a copy of the zone at the given index.

#### `int GetZoneActiveMode(unsigned int zone)`

Returns the active mode index for the given zone, or -1 if the zone uses the device-level mode.

#### `RGBColor GetZoneColor(unsigned int zone, unsigned int color_index)`

Returns the color value of the given color index within the given zone.

#### `RGBColor* GetZoneColorsPointer(unsigned int zone)`

Returns a pointer to the colors array for the given zone.  The caller is responsible for locking and unlocking the access mutex.

#### `unsigned int GetZoneCount()`

Returns the number of zones in the `zones` vector.

#### `std::string GetZoneDisplayName(unsigned int zone)`

Returns the display name of the given zone.  If the zone has `ZONE_FLAG_MANUALLY_CONFIGURED_NAME` set, the display name is returned.  Otherwise, the base name is returned.

#### `zone_flags GetZoneFlags(unsigned int zone)`

Returns the zone flags bitfield for the given zone.

#### `unsigned int GetZoneLEDsCount(unsigned int zone)`

Returns the number of LEDs in the given zone.

#### `unsigned int GetZoneLEDsMax(unsigned int zone)`

Returns the maximum number of LEDs allowed in the given zone.

#### `unsigned int GetZoneLEDsMin(unsigned int zone)`

Returns the minimum number of LEDs allowed in the given zone.

#### `matrix_map_type GetZoneMatrixMap(unsigned int zone)`

Returns the matrix map for the given zone.

#### `const unsigned int* GetZoneMatrixMapData(unsigned int zone)`

Returns a pointer to the matrix map data array for the given zone.

#### `unsigned int GetZoneMatrixMapHeight(unsigned int zone)`

Returns the height of the matrix map for the given zone.

#### `unsigned int GetZoneMatrixMapWidth(unsigned int zone)`

Returns the width of the matrix map for the given zone.

#### `unsigned int GetZoneModeCount(unsigned int zone)`

Returns the number of zone-specific modes for the given zone.

#### `unsigned int GetZoneModeBrightness(unsigned int zone, unsigned int mode)`

Returns the brightness value of the given mode in the given zone.

#### `unsigned int GetZoneModeBrightnessMax(unsigned int zone, unsigned int mode)`

Returns the maximum brightness value of the given mode in the given zone.

#### `unsigned int GetZoneModeBrightnessMin(unsigned int zone, unsigned int mode)`

Returns the minimum brightness value of the given mode in the given zone.

#### `RGBColor GetZoneModeColor(unsigned int zone, unsigned int mode, unsigned int color_index)`

Returns the color at the given index for the given mode in the given zone.

#### `unsigned int GetZoneModeColorMode(unsigned int zone, unsigned int mode)`

Returns the color mode of the given mode in the given zone.

#### `unsigned int GetZoneModeColorsCount(unsigned int zone, unsigned int mode)`

Returns the number of colors in the given mode in the given zone.

#### `unsigned int GetZoneModeColorsMax(unsigned int zone, unsigned int mode)`

Returns the maximum number of colors for the given mode in the given zone.

#### `unsigned int GetZoneModeColorsMin(unsigned int zone, unsigned int mode)`

Returns the minimum number of colors for the given mode in the given zone.

#### `unsigned int GetZoneModeDirection(unsigned int zone, unsigned int mode)`

Returns the direction value of the given mode in the given zone.

#### `unsigned int GetZoneModeFlags(unsigned int zone, unsigned int mode)`

Returns the flags bitfield of the given mode in the given zone.

#### `std::string GetZoneModeName(unsigned int zone, unsigned int mode)`

Returns the name of the given mode in the given zone.

#### `unsigned int GetZoneModeSpeed(unsigned int zone, unsigned int mode)`

Returns the speed value of the given mode in the given zone.

#### `unsigned int GetZoneModeSpeedMax(unsigned int zone, unsigned int mode)`

Returns the maximum speed value of the given mode in the given zone.

#### `unsigned int GetZoneModeSpeedMin(unsigned int zone, unsigned int mode)`

Returns the minimum speed value of the given mode in the given zone.

#### `std::string GetZoneName(unsigned int zone)`

Returns the name of the given zone.

#### `unsigned int GetZoneSegmentCount(unsigned int zone)`

Returns the number of segments in the given zone.

#### `segment_flags GetZoneSegmentFlags(unsigned int zone, unsigned int segment)`

Returns the flags of the given segment in the given zone.

#### `unsigned int GetZoneSegmentLEDsCount(unsigned int zone, unsigned int segment)`

Returns the number of LEDs in the given segment of the given zone.

#### `matrix_map_type GetZoneSegmentMatrixMap(unsigned int zone, unsigned int segment)`

Returns the matrix map of the given segment in the given zone.

#### `const unsigned int * GetZoneSegmentMatrixMapData(unsigned int zone, unsigned int segment)`

Returns a pointer to the matrix map data of the given segment in the given zone.

#### `unsigned int GetZoneSegmentMatrixMapHeight(unsigned int zone, unsigned int segment)`

Returns the matrix map height of the given segment in the given zone.

#### `unsigned int GetZoneSegmentMatrixMapWidth(unsigned int zone, unsigned int segment)`

Returns the matrix map width of the given segment in the given zone.

#### `std::string GetZoneSegmentName(unsigned int zone, unsigned int segment)`

Returns the name of the given segment in the given zone.

#### `unsigned int GetZoneSegmentStartIndex(unsigned int zone, unsigned int segment)`

Returns the start index of the given segment within the given zone.

#### `unsigned int GetZoneSegmentType(unsigned int zone, unsigned int segment)`

Returns the type of the given segment in the given zone.

#### `unsigned int GetZoneStartIndex(unsigned int zone)`

Returns the start index of the given zone within the device's LED/color vectors.

#### `zone_type GetZoneType(unsigned int zone)`

Returns the type of the given zone.

#### `unsigned int GetLEDsInZone(unsigned int zone)`

Returns the number of LEDs in the given zone, accounting for the effects-only flag.  If the zone has `ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE_EFFECTS_ONLY` set and the LED count is greater than 1, this function returns 1.

#### `void SetZoneActiveMode(unsigned int zone, int mode)`

Sets the active mode for the given zone.  Set to -1 to use the device-level mode.  Calls `UpdateZoneMode()` after setting.

#### `void SetZoneColor(unsigned int zone, unsigned int color_index, RGBColor color)`

Sets the color at the given index within the given zone.

#### `void SetZoneModeBrightness(unsigned int zone, unsigned int mode, unsigned int brightness)`

Sets the brightness value for the given mode in the given zone.

#### `void SetZoneModeColor(unsigned int zone, unsigned int mode, unsigned int color_index, RGBColor color)`

Sets the color at the given index for the given mode in the given zone.

#### `void SetZoneModeColorMode(unsigned int zone, unsigned int mode, unsigned int color_mode)`

Sets the color mode for the given mode in the given zone.

#### `void SetZoneModeColorsCount(unsigned int zone, unsigned int mode, unsigned int count)`

Sets the number of colors for the given mode in the given zone.  The count must be within the mode's colors_min and colors_max range.

#### `void SetZoneModeDirection(unsigned int zone, unsigned int mode, unsigned int direction)`

Sets the direction for the given mode in the given zone.  The direction must be valid for the mode's flags.

#### `void SetZoneModeSpeed(unsigned int zone, unsigned int mode, unsigned int speed)`

Sets the speed for the given mode in the given zone.  The speed must be within the mode's speed_min and speed_max range.

#### `bool SupportsPerZoneModes()`

Returns true if any zone has zone-specific modes defined.

### Mode Functions

#### `unsigned int GetModeCount()`

Returns the number of modes in the `modes` vector.

#### `unsigned int GetModeBrightness(unsigned int mode)`

Returns the brightness value of the given mode.

#### `unsigned int GetModeBrightnessMax(unsigned int mode)`

Returns the maximum brightness value of the given mode.

#### `unsigned int GetModeBrightnessMin(unsigned int mode)`

Returns the minimum brightness value of the given mode.

#### `RGBColor GetModeColor(unsigned int mode, unsigned int color_index)`

Returns the color at the given index for the given mode.

#### `unsigned int GetModeColorMode(unsigned int mode)`

Returns the color mode of the given mode.

#### `unsigned int GetModeColorsCount(unsigned int mode)`

Returns the number of colors in the given mode.

#### `unsigned int GetModeColorsMax(unsigned int mode)`

Returns the maximum number of colors for the given mode.

#### `unsigned int GetModeColorsMin(unsigned int mode)`

Returns the minimum number of colors for the given mode.

#### `unsigned int GetModeDirection(unsigned int mode)`

Returns the direction value of the given mode.

#### `unsigned int GetModeFlags(unsigned int mode)`

Returns the flags bitfield of the given mode.

#### `std::string GetModeName(unsigned int mode)`

Returns the name of the given mode.

#### `unsigned int GetModeSpeed(unsigned int mode)`

Returns the speed value of the given mode.

#### `unsigned int GetModeSpeedMax(unsigned int mode)`

Returns the maximum speed value of the given mode.

#### `unsigned int GetModeSpeedMin(unsigned int mode)`

Returns the minimum speed value of the given mode.

#### `void SetModeBrightness(unsigned int mode, unsigned int brightness)`

Sets the brightness value for the given mode.  The brightness must be within the mode's brightness_min and brightness_max range.

#### `void SetModeColor(unsigned int mode, unsigned int color_index, RGBColor color)`

Sets the color at the given index for the given mode.

#### `void SetModeColorMode(unsigned int mode, unsigned int color_mode)`

Sets the color mode for the given mode.

#### `void SetModeColorsCount(unsigned int mode, unsigned int count)`

Sets the number of colors for the given mode.  The count must be within the mode's colors_min and colors_max range.

#### `void SetModeDirection(unsigned int mode, unsigned int direction)`

Sets the direction for the given mode.  The direction must be valid for the mode's flags.

#### `void SetModeSpeed(unsigned int mode, unsigned int speed)`

Sets the speed for the given mode.  The speed must be within the mode's speed_min and speed_max range.

#### `int GetActiveMode()`

Returns the active mode index of the device.  The returned int should line up with the `modes` vector.

#### `void SetActiveMode(int mode)`

Sets the active mode index of the device.  The mode should be the index in the `modes` vector of the mode you wish to set.  If the mode has `MODE_FLAG_REQUIRES_ENTIRE_DEVICE` set, all per-zone active modes are set to -1.  Calls `UpdateMode()` after setting.

#### `void SetCustomMode()`

When called, the device should be put into its software-controlled mode.  This differs between devices, but generally devices have a direct control or static effect mode.  Ideally, this mode should not save to the device's internal Flash.  This function sets up a device for software effect control.  It searches the mode list for "Direct", then "Custom", then "Static" modes with per-LED or mode-specific color support.

### LED Functions

#### `unsigned int GetLEDCount()`

Returns the number of LEDs in the `leds` vector.

#### `std::string GetLEDName(unsigned int led)`

Returns the name of the given LED.

#### `std::string GetLEDDisplayName(unsigned int led)`

Returns the display name of the given LED.  If the LED has a display name set in the `led_display_names` vector, that is returned.  Otherwise, the base LED name is returned.

### Color Functions

#### `RGBColor GetColor(unsigned int led)`

Returns the color value of the given LED in the `colors` vector.

#### `RGBColor* GetColorsPointer()`

Returns a pointer to the beginning of the `colors` vector.

#### `void SetColor(unsigned int led, RGBColor color)`

Sets the color value of the given LED in the `colors` vector.

#### `void SetAllColors(RGBColor color)`

Sets the color value of all LEDs in the `colors` vector by calling `SetAllZoneColors` for each zone.

#### `void SetAllZoneColors(int zone, RGBColor color)`

Sets the color value of all LEDs in the given zone in the `colors` vector.

### Device-Specific Configuration Functions

#### `nlohmann::json GetDeviceSpecificConfigurationSchema()`

Returns the JSON schema for the device-specific configuration.  The schema is extracted from the device's configuration JSON string.

#### `nlohmann::json GetDeviceSpecificConfiguration()`

Returns the current device-specific configuration values as a JSON object.

#### `void SetDeviceSpecificConfiguration(nlohmann::json configuration_json)`

Sets the device-specific configuration values.  Validates the provided keys against the schema before updating.  Calls `DeviceUpdateDeviceSpecificConfiguration()` and signals an update with `RGBCONTROLLER_UPDATE_REASON_SETDEVICESPECIFICCONFIGURATION`.

#### `nlohmann::json GetDeviceSpecificZoneConfigurationSchema(int zone)`

Returns the JSON schema for the device-specific zone configuration for the given zone.

#### `nlohmann::json GetDeviceSpecificZoneConfiguration(int zone)`

Returns the current device-specific zone configuration values for the given zone as a JSON object.

#### `void SetDeviceSpecificZoneConfiguration(int zone, nlohmann::json configuration_json)`

Sets the device-specific zone configuration values for the given zone.  Validates the provided keys against the schema before updating.  Calls `DeviceUpdateDeviceSpecificZoneConfiguration(zone)` and signals an update with `RGBCONTROLLER_UPDATE_REASON_SETDEVICESPECIFICZONECONFIGURATION`.

### Update Callback Functions

#### `void RegisterUpdateCallback(RGBControllerCallback new_callback, void * new_callback_arg)`

Registers a callback function to be called when the device state changes.  The callback is called with the callback argument, an update reason code, and a pointer to the RGBController.  Duplicate registrations are ignored.

#### `void UnregisterUpdateCallback(void * callback_arg)`

Unregisters all callbacks with the given callback argument.

#### `void ClearCallbacks()`

Removes all registered callbacks.

#### `void SignalUpdate(unsigned int update_reason)`

Calls all registered callbacks with the given update reason code.

### Device Update Functions

#### `void Shutdown()`

Stops the device thread and locks the access and signal mutexes.  This should be called from the inherited class destructor before the base class destructor runs.

#### `void UpdateLEDs()`

Sets the update LEDs flag and signals an update with `RGBCONTROLLER_UPDATE_REASON_UPDATELEDS`.  The actual device update is performed by the device thread.

#### `void UpdateZoneLEDs(int zone)`

Immediately updates the LEDs in the given zone by calling `DeviceUpdateZoneLEDs()`.  Signals an update with `RGBCONTROLLER_UPDATE_REASON_UPDATELEDS`.

#### `void UpdateSingleLED(int led)`

Immediately updates a single LED by calling `DeviceUpdateSingleLED()`.  Signals an update with `RGBCONTROLLER_UPDATE_REASON_UPDATELEDS`.

#### `void UpdateMode()`

Sets the update mode flag and signals an update with `RGBCONTROLLER_UPDATE_REASON_UPDATEMODE`.  The actual device mode update is performed by the device thread.

#### `void UpdateZoneMode(int zone)`

Immediately updates the mode for the given zone by calling `DeviceUpdateZoneMode()`.  Signals an update with `RGBCONTROLLER_UPDATE_REASON_UPDATEMODE`.

#### `void SaveMode()`

Saves the current mode to the device by calling `DeviceSaveMode()`.  Signals an update with `RGBCONTROLLER_UPDATE_REASON_SAVEMODE`.

#### `void ClearSegments(int zone)`

Clears all segments from the given zone.  Clears the `ZONE_FLAG_MANUALLY_CONFIGURED_SEGMENTS` flag.  Signals an update with `RGBCONTROLLER_UPDATE_REASON_CLEARSEGMENTS`.

#### `void AddSegment(int zone, segment new_segment)`

Adds a segment to the given zone.  Sets the `ZONE_FLAG_MANUALLY_CONFIGURED_SEGMENTS` flag.  Signals an update with `RGBCONTROLLER_UPDATE_REASON_ADDSEGMENT`.

#### `void ConfigureZone(int zone_idx, zone new_zone)`

Configures a zone with the given properties.  The new_zone's flags determine which properties are updated (size, name, type, matrix map, segments, device-specific).  Calls `DeviceConfigureZone()` and signals an update with `RGBCONTROLLER_UPDATE_REASON_CONFIGUREZONE`.

#### `void ResizeZone(int zone, int new_size)`

Resizes the given zone to the new size.  Only works if the zone has `ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE` or `ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE_EFFECTS_ONLY` set.  Calls `ConfigureZone()` internally.

#### `void ConfigureDevice(controller_flags new_flags, std::string new_name)`

Configures device-level properties.  If the device has `CONTROLLER_FLAG_MANUALLY_CONFIGURABLE_NAME`, the display name can be set.  If the device has `CONTROLLER_FLAG_MANUALLY_CONFIGURABLE_DEVICE_SPECIFIC`, the device-specific configuration flag can be set.  Calls `DeviceConfigureDevice()` and signals an update with `RGBCONTROLLER_UPDATE_REASON_CONFIGUREDEVICE`.

### Virtual Functions (to be implemented in device implementation)

These functions should be overridden by device-specific RGBController implementations to perform the actual hardware communication.

#### `virtual void DeviceConfigureDevice()`

Called when device-level configuration changes.  Default implementation does nothing.

#### `virtual void DeviceConfigureZone(int zone_idx)`

Called when a zone is configured.  Default implementation does nothing.

#### `virtual void DeviceUpdateLEDs()`

Called by the device thread to update all LEDs on the device.

#### `virtual void DeviceUpdateZoneLEDs(int zone)`

Called to update LEDs in a specific zone on the device.

#### `virtual void DeviceUpdateSingleLED(int led)`

Called to update a single LED on the device.

#### `virtual void DeviceUpdateMode()`

Called by the device thread to update the device's active mode.

#### `virtual void DeviceUpdateZoneMode(int zone)`

Called to update the mode for a specific zone on the device.

#### `virtual void DeviceSaveMode()`

Called to save the current mode to the device's non-volatile storage.

#### `virtual void DeviceUpdateDeviceSpecificConfiguration()`

Called when the device-specific configuration changes.

#### `virtual void DeviceUpdateDeviceSpecificZoneConfiguration(int zone)`

Called when the device-specific zone configuration changes for the given zone.

### Static Serialized Description Functions

These functions are used for serializing and deserializing device descriptions for network communication.

#### `static unsigned char * GetColorDescriptionData(unsigned char* data_ptr, RGBController* controller, unsigned int protocol_version)`

Serializes color data into a buffer.

#### `static unsigned int GetColorDescriptionSize(RGBController* controller, unsigned int protocol_version)`

Returns the size of the serialized color data.

#### `static unsigned char * GetDeviceDescriptionData(unsigned char* data_ptr, RGBController* controller, unsigned int protocol_version)`

Serializes the full device description (type, name, vendor, description, version, serial, location, modes, zones, LEDs, colors, LED display names, flags, display name, configuration) into a buffer.

#### `static unsigned int GetDeviceDescriptionSize(RGBController* controller, unsigned int protocol_version)`

Returns the size of the serialized device description.

#### `static unsigned char * GetLEDDescriptionData(unsigned char* data_ptr, led led, unsigned int protocol_version)`

Serializes an LED description into a buffer.

#### `static unsigned int GetLEDDescriptionSize(led led, unsigned int protocol_version)`

Returns the size of the serialized LED description.

#### `static unsigned char * GetMatrixMapDescriptionData(unsigned char* data_ptr, matrix_map_type matrix_map, unsigned int protocol_version)`

Serializes a matrix map into a buffer.

#### `static unsigned int GetMatrixMapDescriptionSize(matrix_map_type matrix_map, unsigned int protocol_version)`

Returns the size of the serialized matrix map.

#### `static unsigned char * GetModeDescriptionData(unsigned char* data_ptr, mode mode, unsigned int protocol_version)`

Serializes a mode description into a buffer.

#### `static unsigned int GetModeDescriptionSize(mode mode, unsigned int protocol_version)`

Returns the size of the serialized mode description.

#### `static unsigned char * GetSegmentDescriptionData(unsigned char* data_ptr, segment segment, unsigned int protocol_version)`

Serializes a segment description into a buffer.

#### `static unsigned int GetSegmentDescriptionSize(segment segment, unsigned int protocol_version)`

Returns the size of the serialized segment description.

#### `static unsigned char * GetZoneDescriptionData(unsigned char* data_ptr, zone zone, unsigned int protocol_version)`

Serializes a zone description into a buffer.

#### `static unsigned int GetZoneDescriptionSize(zone zone, unsigned int protocol_version)`

Returns the size of the serialized zone description.

#### `static unsigned char* SetColorDescription(unsigned char* data_ptr, unsigned int data_size, RGBController* controller, unsigned int protocol_version, bool resize = false)`

Deserializes color data from a buffer into the controller.

#### `static unsigned char* SetDeviceDescription(unsigned char* data_ptr, unsigned int data_size, RGBController* controller, unsigned int protocol_version)`

Deserializes a full device description from a buffer into the controller.

#### `static unsigned char* SetLEDDescription(unsigned char* data_ptr, unsigned int data_size, led* led, unsigned int protocol_version)`

Deserializes an LED description from a buffer.

#### `static unsigned char* SetMatrixMapDescription(unsigned char* data_ptr, unsigned int data_size, matrix_map_type* matrix_map, unsigned int protocol_version)`

Deserializes a matrix map from a buffer.

#### `static unsigned char* SetModeDescription(unsigned char* data_ptr, unsigned int data_size, mode* mode, unsigned int protocol_version)`

Deserializes a mode description from a buffer.

#### `static unsigned char* SetSegmentDescription(unsigned char* data_ptr, unsigned int data_size, segment* segment, unsigned int protocol_version)`

Deserializes a segment description from a buffer.

#### `static unsigned char* SetZoneDescription(unsigned char* data_ptr, unsigned int data_size, zone* zone, unsigned int protocol_version)`

Deserializes a zone description from a buffer.

### Static JSON Description Functions

These functions are used for serializing and deserializing device descriptions in JSON format.

#### `static nlohmann::json GetDeviceDescriptionJSON(RGBController* controller)`

Returns a JSON representation of the full device description.

#### `static nlohmann::json GetLEDDescriptionJSON(led led)`

Returns a JSON representation of an LED.

#### `static nlohmann::json GetMatrixMapDescriptionJSON(matrix_map_type matrix_map)`

Returns a JSON representation of a matrix map.

#### `static nlohmann::json GetModeDescriptionJSON(mode mode)`

Returns a JSON representation of a mode.

#### `static nlohmann::json GetSegmentDescriptionJSON(segment segment)`

Returns a JSON representation of a segment.

#### `static nlohmann::json GetZoneDescriptionJSON(zone zone)`

Returns a JSON representation of a zone.

#### `static void SetDeviceDescriptionJSON(nlohmann::json controller_json, RGBController* controller)`

Deserializes a device description from JSON into the controller.

#### `static led SetLEDDescriptionJSON(nlohmann::json led_json)`

Deserializes an LED from JSON.

#### `static matrix_map_type SetMatrixMapDescriptionJSON(nlohmann::json matrix_map_json)`

Deserializes a matrix map from JSON.

#### `static mode SetModeDescriptionJSON(nlohmann::json mode_json)`

Deserializes a mode from JSON.

#### `static segment SetSegmentDescriptionJSON(nlohmann::json segment_json)`

Deserializes a segment from JSON.

#### `static zone SetZoneDescriptionJSON(nlohmann::json zone_json)`

Deserializes a zone from JSON.

### Static Utility Functions

#### `static bool CompareControllers(RGBController* controller_1, RGBController* controller_2)`

Compares two controllers for equality based on their name, vendor, description, version, serial, and location.

#### `static std::string DeviceTypeToString(device_type type)`

Converts a device type enum value to its string representation.

#### `static bool SetModeValuesFromMode(mode& destination, mode& source)`

Copies mode values (speed, brightness, direction, color_mode, colors) from source to destination.
