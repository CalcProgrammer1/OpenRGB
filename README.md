# OpenRGB (formerly OpenAuraSDK)

The goal of this project is to create an easy-to-use open source software program and library for accessing and controlling RGB lights on various PC hardware including motherboards, RAM modules, graphics cards, cooling devices, and peripherals.

This project originally focused only on ASUS Aura.  It was spun off of Keyboard Visualizer's AsusAuraWindows branch to learn more about the details behind the Aura protocol and to develop a more flexible, compatible, and reliable driver for Aura.  Our Aura implementation is now quite solid and supports multiple generations of Aura controller across both Intel and AMD platforms.  It also supports Aura-compatible controllers used across multiple manufacturers of RGB memory modules including G.Skill Trident Z RGB and others.  It is still lacking a few capabilities, namely saving settings to persist across reboots, but the core functionality of setting colors and modes is there.

After getting a solid Aura implementation, the project branched out into other manufacturers and categories of RGB devices.  A major focus was to develop a software API that could reliably represent as many RGB devices as possible, exposing their various modes and describing their LED layouts via zones, that was generic enough to write a user application without specifically targeting any one manufacturer.  To this extent, the generic_rgb_interface_test branch was created which became the foundation of OpenRGB.  The goal of OpenRGB is to both develop new drivers for unsupported devices and integrate existing open source drivers for devices that do have some sort of open support.  The goal is to be the one-stop solution to open source RGB lighting control!

## Supported Devices

See the [Project Wiki](https://gitlab.com/CalcProgrammer1/OpenRGB/-/wikis/home) for the current list of supported devices.

## Installation
#### Windows
  1. Download the latest Visual Studio Community Edition and Qt Creator.
  2. Update the submodules: git submodule update --init --recursive
  3. Open the OpenRGB_Win.pro project.
  4. Build the project for `x86` Architecture. The InpOut32 library I use does not support x64.
  5. Copy InpOut32.dll from dependencies to the same path as OpenAuraSDK.exe along with the Qt libraries.

**You must run the application as Administrator the first time to allow InpOut32 to set up.  It can be run as a normal user afterwards**

#### Linux:
  1. Either open the project using QT Creator or build it using qmake.  You will need the libusb-1.0-0 (or equivalent) and libhidapi development packages from your distribution's package manager installed.
    
    * sudo apt install qt5-default libusb-1.0-0-dev libhidapi-dev

    * git clone https://gitlab.com/CalcProgrammer1/OpenRGB

    * cd OpenRGB

    * git submodule update --init --recursive

    * qmake OpenRGB.pro

    * make -j8
    
    * ./OpenRGB
     

  2. Allow access to SMBus:<br>

     - This can be identified by your motherboard
         ##### Intel
          - `modprobe i2c-dev i2c-i801`
          - Asus used the SMBus controller on the Super IO chip for on-board Aura chips on Intel motherboards.  I have a kernel patch to add a driver for this chip [here](https://gitlab.com/CalcProgrammer1/OpenAuraSDK/issues/22).  After patching the kernel, enable the Nuvoton NCT67xx SMBus driver in your kernel configuration.  The driver may be loaded with `modprobe i2c-nct6775`
         ##### AMD
          - `modprobe i2c-dev i2c-piix4` 
          - The second SMBus isn't currently picked up by the kernel driver and that seems to be where Asus wired the Aura controllers so I have a kernel patch [here](https://gitlab.com/CalcProgrammer1/OpenAuraSDK/issues/9) that will allow the kernel to use the second bus (at `0x0b20`).

     - You'll have to enable user access to your SMbus if you don't run as root.
       - List all SMBus controllers: `sudo i2cdetect -l`
       - Find out which control your Aura devices (PIIX4, I801, and NCT67xx)
       - Give user access to those controllers, for instance: `sudo chmod 777 /dev/i2c-0`


