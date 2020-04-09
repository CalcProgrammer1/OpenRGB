## ![OpenRGB](https://gitlab.com/CalcProgrammer1/OpenRGB/-/wikis/uploads/5b7e633ac9f63b00c8a4c72686206c3f/OpenRGB.png) (formerly OpenAuraSDK)

One of the biggest complaints about RGB is the software ecosystem surrounding it.  Every manufacturer has their own app, their own brand, their own style.  If you want to mix and match devices, you end up with a ton of conflicting, functionally identical apps competing for your background resources.  On top of that, these apps are proprietary and Windows-only.  Some even require online accounts.  What if there was a way to control all of your RGB devices from a single app, on both Windows and Linux, without any nonsense?  That is what OpenRGB sets out to achieve.  One app to rule them all.
OpenRGB is still in its early stages and already supports quite a few products.  I'm always on the lookout for good deals on more popular RGB devices to add support for.

## Supported Devices

See the [Project Wiki](https://gitlab.com/CalcProgrammer1/OpenRGB/-/wikis/home) for the current list of supported devices.

![OpenRGB_0.11](https://gitlab.com/CalcProgrammer1/OpenRGB/-/wikis/uploads/2a913ece50cfa1ab2210f63e4846df4f/OpenRGB_0.11.PNG)

## Installation
#### Windows
  1. Download the latest Visual Studio Community Edition and Qt Creator.
  2. Open the OpenRGB_Win.pro project.
  3. Build the project for either the `x86` or `x64` architecture.
  4. Run the project from Qt Creator.  If you want to use your custom build standalone, download the latest matching Release package and replace the OpenRGB.exe in it with your new build.

**You must run the application as Administrator the first time to allow InpOut32 to set up.  It can be run as a normal user afterwards**

#### Linux:
  1. Either open the project using QT Creator or build it using qmake.  You will need the libusb-1.0-0 (or equivalent) development package from your distribution's package manager installed.
    
    * sudo apt install build-esential qtcreator qt5-default libusb-1.0-0-dev

    * git clone https://gitlab.com/CalcProgrammer1/OpenRGB

    * cd OpenRGB

    * qmake OpenRGB.pro

    * make -j8
    
    * ./OpenRGB
     

  2. Allow access to SMBus:<br>

     - This can be identified by your motherboard
         ##### Intel
          - `modprobe i2c-dev i2c-i801`
          - Asus used the SMBus controller on the Super IO chip for on-board Aura chips on Intel motherboards.  I have a kernel patch to add a driver for this chip (OpenRGB.patch).  After patching the kernel, enable the Nuvoton NCT67xx SMBus driver in your kernel configuration.  The driver may be loaded with `modprobe i2c-nct6775`
         ##### AMD
          - `modprobe i2c-dev i2c-piix4` 
          - The second SMBus isn't currently picked up by the kernel driver and that seems to be where Asus wired the Aura controllers so I have a kernel patch (OpenRGB.patch) that will allow the kernel to use the second bus (at `0x0b20`).

     - Instructions on patching the kernel:
       - https://gitlab.com/CalcProgrammer1/OpenRGB/-/wikis/OpenRGB-Kernel-Patch
       
     - Some Gigabyte/Aorus motherboards have an ACPI conflict with the SMBus controller.
       - Add `acpi_enforce_resources=lax` to your kernel command line and reboot.  The controller should now show up.

     - You'll have to enable user access to your SMbus if you don't run as root.
       - List all SMBus controllers: `sudo i2cdetect -l`
       - Find out which control your Aura devices (PIIX4, I801, and NCT67xx)
       - Give user access to those controllers, for instance: `sudo chmod 777 /dev/i2c-0`

## Projects Used

  * InpOutx64: http://www.highrez.co.uk/downloads/inpout32/

  * libusb: https://github.com/libusb/libusb

  * hidapi: https://github.com/libusb/hidapi

  * libe131: https://github.com/hhromic/libe131

  * NVFC: https://github.com/graphitemaster/NVFC

  * OpenRazer: https://github.com/openrazer/openrazer

  * OpenRazer-Win32: https://github.com/CalcProgrammer1/openrazer-win32

  * Qt-Plus (ColorWheel): https://github.com/liuyanghejerry/Qt-Plus

