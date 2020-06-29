## ![OpenRGB](https://gitlab.com/CalcProgrammer1/OpenRGB/-/wikis/uploads/5b7e633ac9f63b00c8a4c72686206c3f/OpenRGB.png) (formerly OpenAuraSDK)

One of the biggest complaints about RGB is the software ecosystem surrounding it.  Every manufacturer has their own app, their own brand, their own style.  If you want to mix and match devices, you end up with a ton of conflicting, functionally identical apps competing for your background resources.  On top of that, these apps are proprietary and Windows-only.  Some even require online accounts.  What if there was a way to control all of your RGB devices from a single app, on both Windows and Linux, without any nonsense?  That is what OpenRGB sets out to achieve.  One app to rule them all.

## Features

* Set colors and select effect modes for a wide variety of RGB hardware
* Save and load profiles
* Control lighting from third party software using the OpenRGB SDK
* Command line interface
* Connect multiple instances of OpenRGB to synchronize lighting across multiple PCs
* Can operate standalone or in a client/headless server configuration
* View device information
* No official/manufacturer software required

## Supported Devices

See the [Project Wiki](https://gitlab.com/CalcProgrammer1/OpenRGB/-/wikis/home) for the current list of supported devices.

## WARNING!

This project provides a tool to probe the SMBus.  This is a potentially dangerous operation if you don't know what you're doing.  Exercise caution when clicking the Detect Devices or Dump Device buttons.  There have been reports of Gigabyte motherboards having serious issues (bricking the RGB or bricking the entire board) when dumping certain devices.  On the same lines, exercise the same caution when using the i2cdump and i2cdetect commands on Linux, as they perform the same functionality.  OpenRGB is not liable for damage caused by improper SMBus access.

As of now, only Gigabyte RGB Fusion 2.0 boards have been reported to have issues.

![OpenRGB_0.11](https://gitlab.com/CalcProgrammer1/OpenRGB/-/wikis/uploads/0ef8eb3936fe715217e7e3430c0aae18/OpenRGB_new_icons.PNG)

## Windows

  *  Pre-built binaries are available under the Releases section on GitLab.
  *  If you wish to build the application yourself:
      1. Download the latest Visual Studio Community Edition and Qt Creator.
      2. Open the OpenRGB.pro project in Qt Creator.
      3. Use the MSVC compiler kit, either 32- or 64-bit, to build the application.
      4. Run the project from Qt Creator.  If you want to use your custom build standalone, download the latest matching Release package and replace the OpenRGB.exe in it with your new build.
  *  **You must run the application as Administrator the first time to allow InpOut32 to set up.  It can be run as a normal user afterwards**

### USB Access

  *  Some USB devices (especially keyboards and mice) require installation of the WinUSB driver.  You can do this with Zadig:
      - Download Zadig:  https://zadig.akeo.ie/
      - Select "list all devices" from the menu
      - Select the last interface (highest number) of your device
      - With "WinUSB" selected, click Install

## Linux

  *  Pre-built binaries are not currently available for Linux
  *  You can build the project using Qt Creator or on the command line.
      1.  Install build dependencies
          - Debian: sudo apt install build-essential qtcreator qt5-default libusb-1.0-0-dev libhidapi-dev pkgconf
          - Fedora: sudo dnf install git make automake gcc gcc-c++ qt-creator qt5-devel hidapi-devel libusb-devel
      2.  git clone https://gitlab.com/CalcProgrammer1/OpenRGB
      3.  cd OpenRGB
      4.  qmake OpenRGB.pro
      5.  make -j8
  *  Run the application with ./OpenRGB
     
### SMBus Access

  *  SMBus access is necessary for controlling RGB RAM and certain motherboard on-board LEDs.
  *  If you are not trying to use OpenRGB to control RGB RAM or motherboard LEDs, you may skip this section.
  *  ASUS and ASRock motherboards have their RGB controller on an SMBus interface that is not accessible by an unmodified Linux kernel (for now).  I am working on getting patches submitted upstream, but for now you must patch your kernel with the provided OpenRGB.patch file.
  *  Allowing access to SMBus:
      1. Load the i2c-dev module: `sudo modprobe i2c-dev`
      2. Load the i2c driver for your chipset:
          -  Intel:
              - `sudo modprobe i2c-i801`
              - `sudo modprobe i2c-nct6775` - Secondary controller for motherboard LEDs (requires patch)
          -  AMD:
              - `modprobe i2c-piix4` 
              - Unmodified kernel will have one interface, patched kernel will have two.  The first at 0x0B00 and the second at 0x0B20.  The 0x0B20 interface is for motherboard LEDs.
  *  Instructions on patching the kernel:
      - https://gitlab.com/CalcProgrammer1/OpenRGB/-/wikis/OpenRGB-Kernel-Patch
  *  Some Gigabyte/Aorus motherboards have an ACPI conflict with the SMBus controller.
      - Add `acpi_enforce_resources=lax` to your kernel command line and reboot.  The controller should now show up.
  *  You'll have to enable user access to your SMBus if you don't run as root.
      - List all SMBus controllers: `sudo i2cdetect -l`
      - Note the number for PIIX4, I801, and NCT6775 controllers.
      - Give user access to those controllers, for instance: `sudo chmod 777 /dev/i2c-0`

### USB Access

  *  USB devices require udev rules to access as a normal user.
  *  You can run OpenRGB as root to detect all USB devices.
  *  Udev rules are included in this repo:
      - Copy the 60-openrgb.rules file to /etc/udev/rules.d/
      - Reload rules with `sudo udevadm control --reload-rules && sudo udevadm trigger`

## OpenRGB SDK

OpenRGB provides a network interface for controlling supported RGB devices from other software.  These projects implement the OpenRGB SDK and provide additional ways for you to control your lighting setup.

  * Keyboard Visualizer (by me): https://gitlab.com/CalcProgrammer1/KeyboardVisualizer
  * OpenRGB Python Client (by bahorn): https://github.com/bahorn/OpenRGB-PyClient
  * OpenRGB Python Client (by jath03): https://github.com/jath03/openrgb-python
  * OpenRGB Node.js Client (by vlakreeh): https://github.com/vlakreeh/openrgb
  * D-Bus Connector for OpenRGB (by Vinno97): https://github.com/Vinno97/OpenRGB-DBus-Connector
  * OpenRGB E1.31 Receiver (by me): https://gitlab.com/CalcProgrammer1/OpenRGBE131Receiver
  * OpenRGB.NET - C# OpenRGB Client (by diogotr7): https://github.com/diogotr7/OpenRGB.NET

## Projects Used

  * InpOutx64: http://www.highrez.co.uk/downloads/inpout32/
  * libusb: https://github.com/libusb/libusb
  * hidapi: https://github.com/libusb/hidapi
  * libe131: https://github.com/hhromic/libe131
  * NVFC: https://github.com/graphitemaster/NVFC
  * OpenRazer: https://github.com/openrazer/openrazer
  * OpenRazer-Win32: https://github.com/CalcProgrammer1/openrazer-win32
  * Qt-Plus (ColorWheel): https://github.com/liuyanghejerry/Qt-Plus
  * AMD ADL Libraries: https://github.com/GPUOpen-LibrariesAndSDKs/display-library
  
## Projects Researched

While no code from these projects directly made its way into OpenRGB, these projects have been invaluable resources for protocol information.

  * ckb-next: https://github.com/ckb-next/ckb-next
  * linux_thermaltake_riing: https://github.com/chestm007/linux_thermaltake_riing
  * Aura Addressable Header Controller: https://gitlab.com/cneil02/aura-addressable-header-controller
  * OpenPyAURA: https://gitlab.com/thelastguardian/openpyaura
  * AsrLed: https://github.com/EUA/AsrLed
  * asrock-leds: https://github.com/RattyDAVE/asrock-leds
  * hue-plus: https://github.com/kusti8/hue-plus
  * rogauracore: https://github.com/wroberts/rogauracore
  * msi-rgb: https://github.com/nagisa/msi-rgb
  * OpenCorsairLink: https://github.com/audiohacked/OpenCorsairLink
  * msi-keyboard: https://github.com/bparker06/msi-keyboard
  * rivalcfg: https://github.com/flozz/rivalcfg
  * VRMTool: https://github.com/rbrune/VRMtool
  * g810-led: https://github.com/MatMoul/g810-led
  * liquidctl: https://github.com/jonasmalacofilho/liquidctl
