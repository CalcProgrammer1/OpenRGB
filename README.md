# OpenAuraSDK
##### This project is an open-source implementation of the Asus Aura RGB controller software.  It currently only supports a limited number of motherboard and RAM module lighting controllers.

## Supported Devices 

Confirmed supported motherboards:
  - ASUS PRIME X370 Pro
  - ASUS PRIME X470 Pro
  - ASUS PRIME Z270-A
  - ASUS PRIME Z370-A
  - ASUS ROG Strix Z370-E
    
Confirmed supported RGB RAM (except on X299):
  - G.Skill Trident Z RGB
  - Geil Super Luce
  - TeamGroup Delta RGB

Experimental support in generic RGB testing branch:
  - Corsair Vengeance RGB
  - Corsair Vengeance Pro RGB
  - HyperX Predator RGB

## Installation
#### Windows
  1. Download the latest Visual Studio Community Edition and open the `.sln`.
  2. Build the project for `x86` Architecture. The InpOut32 library I use does not support x64.
  3. Copy InpOut32.dll from dependencies to the same path as OpenAuraSDK.exe.

**You may have to retarget the project to the latest version of the Windows SDK and build tools and stuff.**

**You must run the application as Administrator the first time to allow InpOut32 to set up.  It can be run as a normal user afterwards**

#### Linux:
  1. Either open the project using QT Creator or build it using qmake.
    
         cd OpenAuraSDK
         qmake OpenAuraSDK.pro
         make
     

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


