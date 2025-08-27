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

The RGBController class specification contains the following:

* Device Name
* Device Vendor
* Device Description
* Device Version
* Device Serial
* Device Location
* Vector of LEDs
* Vector of Zones
* Vector of Modes
* Vector of Colors (32-bit 0x00BBGGRR format)
* Device Type (enum)
* Active mode index
* Vector of LED Alternate Names
* Controller Flags

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

| Controller Flags Bit | Name    | Description                                         |
| -------------------- | ------- | --------------------------------------------------- |
| 0                    | Local   | Controller is provided by this OpenRGB instance     |
| 1                    | Remote  | Controller is provided by a remote OpenRGB instance |
| 2                    | Virtual | Controller is virtual (not a physical device)       |

### LED Alternate Names

The LED Altrernate Names vector can override the base name of an LED.  The intended use case for this field is providing regional key names for non-English keyboard layouts.  The base key names should always be provided in English QWERYY layout for positional mapping to work on certain SDK applications, so the alternate names field can override the base name to provide the correct key name for the localized layout without disrupting SDK application mapping.  If not overriding any LED names, this vector can be left empty.  If only overriding certain LED names, those not being overridden can be empty strings.  If used, the length of this vector must equal the length of the LEDs vector.

## LEDs

The LED structure contains information about an LED.

* LED Name
* LED Value

The Value has no defined functionality in the RGBController API and is provided for implementation-specific use.  You can use this field to associate implementation-specific data with an LED.

## Zones

The Zone structure contains information about a zone.  A zone is a logical grouping of LEDs defined by the RGBController implementation.  LEDs in a zone must be contiguous in the RGBController's LEDs/Colors vectors.

* Zone Name
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

The LED pointer and Color pointer point to the first LED/Color in the RGBController's LEDs/Colors vector associated with this zone.  The Start Index is the index to the same LED/Color in the vectors.

The LED count is the number of LEDs in the zone.  For zones with a fixed number of LEDs, the count, min, and max values should all be equal.  For zones with a user-adjustable number of LEDs, the count should be between the min and max values, inclusively.  User-adjustable zones are most commonly used to represent addressable RGB (ARGB) controllers as the number of LEDs depends on what strips/devices are attached to the ARGB headers.  The ResizeZone function in the RGBController API is used to resize the number of LEDs in the zone.  The initial value should be zero for ARGB zones if the device does not provide a means to automatically determint the number of connected LEDs.

### Zone Types

The zone type enum defines the zone type.  This describes the physical layout of the zone and can be used by software to generate appropriate effects for the zone.

| Zone Type Value | Description |
| --------------- | ----------- |
| 0               | Single      |
| 1               | Linear (1D) |
| 2               | Matrix (2D) |

### Matrix Map

Each zone has a matrix map pointer which allows an optional matrix map to be associated with the zone.  The matrix map is used to provide positioning information about LEDs in a 2D grid.  If a matrix map is not provided for a zone, the zone's matrix map pointer must be set to NULL.

A matrix map has the following:

* Height
* Width
* Map data pointer

The height and width determine the size of the map data.  The map data pointer should point to a data block of (Height * Width) unsigned 32-bit integers.  This data can be accessed as if it were a Map[Y][X] 2D array.  The values of the map are LED index values in the zone (so offset by Start Index from the RGBController's LEDs vector).  If a spot in the matrix is unused and does not map to an LED, it should be set to 0xFFFFFFFF.

### Segments

Each Zone contains a vector of Segments.  Segments can be used to divide a physical zone (such as an ARGB header) into multiple logical sub-zones, or segments.  This is mainly used for ARGB zones with multiple components daisy-chained together.  For example, segments can be used to group multiple rings on an ARGB fan or multiple daisy-chained fans connected to one header.  If the device is capable of automatically detecting multiple components connected to a single output, the RGBController may create segments automatically during zone creation.  Otherwise, leaving this vector empty will indicate that the zone contains no segments, though resizable zones allow the user to define their own segments.

A segment contains the following:

* Segment Name
* Segment Type (See Zone Type values)
* Start Index
* LED Count

The Start Index is the index within the Zone where the Segment starts.  The LED Count is the number of LEDs in the Segment.  Care should be taken to ensure that the total number of LEDs across all segments equals the number of LEDs in the Zone and the start indices do not overlap.

### Zone Flags

The Zone Flags field is a bitfield with informational flags related to the Zone.

| Zone Flags Bit | Name                | Description                                                                                                                                      |
| -------------- | ------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------ |
| 0              | Resize Effects Only | This zone is resizable, but the size is only used for effects modes.  The zone is treated as a single LED in the Colors vector for per-LED modes |

## Modes

Modes represent internal effects and have a name field that describes the effect.  The mode's index in the vector is its ID.  The Active Mode variable in the RGBController class specifies which mode is currently selected.  A mode contains the following:

* Mode Name
* Mode Value
* Mode Flags
* Minimum Speed
* Maximum Speed
* Minimum number of colors
* Maximum number of colors
* Speed Value
* Direction
* Color Mode
* Colors Vector

The mode value is field is provided to hold an implementation-defined mode value.  This is usually the mode's value in the hardware protocol.

The mode flags field is a bitfield that contains information about what features a mode has.

| Mode Flags Bit | Description                                      |
| -------------- | ------------------------------------------------ |
| 0              | Mode has speed parameter                         |
| 1              | Mode has left/right direction parameter          |
| 2              | Mode has up/down direction parameter             |
| 3              | Mode has horizontal/vertical direction parameter |
| 4              | Mode has brightness parameter                    |
| 5              | Mode has per-LED color settings                  |
| 6              | Mode has mode specific color settings            |
| 7              | Mode has random color option                     |

The mode minimum and maximum speed fields should be set to the implementation-specific minimum and maximum speed values for the given mode if the mode supports speed control.  The mode speed value field will be set between the minimum and maximum value, inclusively.  The minimum speed may be a greater numerical value than the maximum speed if your device's speed adjustment is inverted (usually because the device takes a delay period rather than a speed value).

The mode minimum and maximum number of colors fields should be used if the mode supports mode-specific color settings.  These determine the size range of the mode's Colors vector.  If the mode has a fixed number of colors, the minimum and maximum should be equal.  Mode-specific colors are used when a mode has one or more configurable colors but these colors do not apply directly to individual LEDs.  Example would be a breathing mode that cycles between one or more colors each breath pulse.  A mode may have multiple color options available, for instance a breathing mode that can either use one or more defined colors or just cycle through random colors.  The available color modes for a given mode are set with the flags.  The selected color mode is set using the color mode field, which can be one of the following values.

| Color Mode Value | Description                                                                                       |
| ---------------- | ------------------------------------------------------------------------------------------------- |
| 0                | None - this mode does not have configurable colors                                                |
| 1                | Per-LED - this mode uses the RGBController's colors vector to set each LED to its specified color |
| 2                | Mode Specific - this mode has one or more configurable colors, but not individual LED control     |
| 3                | Random - this mode can be switched to a random or cycling color palette                           |

## Functions

### `std::string GetName()`

Returns the `name` string of the device.

### `std::string GetVendor()`

Returns the `vendor` string of the device.

### `std::string GetDescription()`

Returns the `description` string of the device.

### `std::string GetVersion()`

Returns the `version` string of the device.

### `std::string GetSerial()`

Returns the `serial` string of the device.

### `std::string GetLocation()`

Returns the `location` string of the device.

### `std::string GetModeName(int mode)`

Returns the `name` string of the given mode in the `modes` vector.

### `std::string GetZoneName(int zone)`

Returns the `name` string of the given zone in the `zones` vector.

### `std::string GetLEDName(int led)`

Returns the `name` string of the given LED in the `leds` vector.

### `RGBColor GetLED(unsigned int led)`

Returns the color value of the given LED in the `colors` vector.

### `void SetLED(unsigned int led, RGBColor color)`

Sets the color value of the given LED in the `colors` vector.

### `void SetAllLEDs(RGBColor color)`

Sets the color value of all LEDs in the `colors` vector.

### `void SetAllZoneLEDs(int zone, RGBColor color)`

Sets the color value of all LEDs in the given zone in the `colors` vector.

### `int GetMode()`

Returns the active mode index of the device.  The returned int should line up with the `modes` vector.

### `void SetMode(int mode)`

Sets the active mode index of the device.  The mode should be the index in the `modes` vector of the mode you wish to set.

### `void SetCustomMode()`

When called, the device should be put into its software-controlled mode.  This differs between devices, but generally devices have a direct control or static effect mode.  Ideally, this mode should not save to the device's internal Flash.  This function sets up a device for software effect control.

### `void UpdateLEDs()`

Update all LEDs based on the `colors` vector.

### `void UpdateZoneLEDs(int zone)`

Update all LEDs in the given zone based on the `colors` vector.

### `void UpdateSingleLED(int led)`

Update a single LED based on the `colors` vector.

### `void UpdateMode()`

Update the mode based on the active mode index and the `modes` vector.
