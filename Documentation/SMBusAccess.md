# SMBus Access

This document details the process to set up SMBus/I2C access on supported operating systems.

SMBus, or [System Management Bus](https://en.wikipedia.org/wiki/System_Management_Bus), is a low-level interface present on most PC motherboards.  Some RGB control devices are attached via SMBus.  These include all DDR4 and DDR5 RAM modules with integrated RGB lighting as well as the onboard lighting on several motherboards, mostly from the X370/Z270 and X470/Z370 generations.

If you are not using RGB RAM and you are not using a motherboard from the X370/Z270 or X470/Z370 generation you can skip these steps and ignore the SMBus warning if it appears.

SMBus is generally not meant to be accessed by user applications, but RGB software creates an exception to this rule.  This means that some steps may be necessary to allow OpenRGB permission to access the SMBus interface.  These steps are listed below.

## Windows

  * On Windows, OpenRGB uses the [WinRing0](https://github.com/GermanAizek/WinRing0) driver to access the SMBus interface.
  *  **You must run the application as Administrator the first time to allow WinRing0 to set up.  It can be run as a normal user afterwards**
  * Early versions of OpenRGB used [InpOut32](https://www.highrez.co.uk/downloads/inpout32/). This is no longer needed and should be removed to avoid warnings by some anti-cheat software. You can uninstall Inpout32 by following the instructions [here](https://gitlab.com/CalcProgrammer1/OpenRGB/-/issues/669#note_461054255).

## Linux

  1. Install the `i2c-tools` package.
  2. Load the i2c-dev module: `sudo modprobe i2c-dev`
  3. Load the i2c driver for your chipset:
      * Intel
        * `sudo modprobe i2c-i801`
      * AMD
        * `sudo modprobe i2c-piix4`
      * Nuvoton
        * This interface is used alongside `i2c-i801` on some older ASUS Intel motherboards for the on-board lighting.
        * `sudo modprobe i2c-nct6793`
        * Note: The i2c-nct6793 driver must be installed separately, see [i2c-nct6793-dkms](https://gitlab.com/CalcProgrammer1/i2c-nct6793-dkms)

  * If you want the i2c modules to load automatically at boot, run the following:
      1. `sudo touch /etc/modules-load.d/i2c.conf`
      2. `sudo sh -c 'echo "i2c-dev" >> /etc/modules-load.d/i2c.conf'`
      3. Run the following based on which i2c drivers you loaded in the previous section:
          * `sudo sh -c 'echo "i2c-i801" >> /etc/modules-load.d/i2c.conf'`
          * `sudo sh -c 'echo "i2c-piix4" >> /etc/modules-load.d/i2c.conf'`
  
  * You will have to enable user access to the i2c devices if you don't run OpenRGB as root.
      1. List all SMBus controllers: `sudo i2cdetect -l`
      2. Note the number(s) for piix4 or i801 controllers.
      3. Give user access to those controllers. If you have not installed OpenRGB from a distribution package then most likely you need to install the udev rules manually.

  * Some Gigabyte/Aorus motherboards have an ACPI conflict with the SMBus controller.  You can bypass this conflict by adding the `acpi_enforce_resources=lax` kernel parameter to your kernel command line.  See the [Kernel Parameters](Documentation/KernelParameters.md) page for more information.

  * The [spd5118 kernel driver](https://docs.kernel.org/hwmon/spd5118.html) can claim certain I2C addresses for Kingston Fury DDR5 memory and thus prevent other kernel modules from accessing them. This is the case if the `i2cdetect` command prints the character string `UU` on the I2C bus responsible for the DRAM. A solution to this problem is to unload the `spd5118` kernel driver using `rmmod spd5118`.

## MacOS

  * For Intel devices using a controller in the i801 family you have to download and install the [macUSPCIO driver](https://github.com/ShadyNawara/macUSPCIO/releases)
