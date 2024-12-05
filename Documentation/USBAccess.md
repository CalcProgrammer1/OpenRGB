# USB Access

This document details the process to set up USB access on supported operating systems.

USB, or [Universal Serial Bus](https://en.wikipedia.org/wiki/USB) is the most common interface used to connect RGB devices to a PC.  It can be used both externally, where a device has a cable which plugs into a USB port or motherboard header, or internally, where a device such as an RGB controller chip built into a motherboard is wired directly to the processor or chipset's USB interface.

USB access permissions vary based on the type of device and the operating system.  Some steps may be necessary to allow OpenRGB permission to access these devices.

## Windows

  * Windows should not need any special setup to access USB devices.
  * If a device does not get detected, try running OpenRGB as Administrator.
  * Early versions of OpenRGB used the WinUSB driver, installed using Zadig.  This is no longer required, and you need to uninstall the WinUSB driver if you previously installed it.  You can uninstall the WinUSB driver by following [this guide](https://gitlab.com/CalcProgrammer1/OpenRGB/-/wikis/Frequently-Asked-Questions#i-installed-the-winusb-driver-for-a-device-and-i-wish-to-uninstall-it).

## Linux

  * USB devices require [udev rules](/Documentation/UdevRules.md) to access as a normal user.
  * Alternatively you can run OpenRGB as root to detect all USB devices. (Not recommended)
  * USB based Gigabyte AORUS motherboards may also have an ACPI conflict. Please [add a kernel parameter](#kernel-parameters) to resolve this conflict.

## MacOS

  * USB devices may require the Input Monitoring permission.  You can add OpenRGB in System Preferences > Security & Privacy > Privacy.
