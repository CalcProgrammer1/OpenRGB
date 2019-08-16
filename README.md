# OpenAuraSDK
##### This project is an open-source implementation of the Asus Aura RGB controller software.  It currently only supports a limited number of motherboard and RAM module lighting controllers.

## Supported Devices 

Confirmed supported motherboards (only AMD boards working in Linux at the moment):
  - ASUS PRIME X370 Pro
  - ASUS PRIME Z270-A
  - ASUS ROG Strix Z370-E
    
Confirmed supported RGB RAM (except on X299):
  - G.Skill Trident Z RGB
  - Geil Super Luce


## Installation
#### Windows
  1. Download the latest Visual Studio Community Edition and open the `.sln`.<br>
  2. Build the project for `x86` Architecture. The InpOut32 library I use does not support x64.<br>
**You may have to retarget the project to the latest version of the Windows SDK and build tools and stuff.**<br>


#### Linux:
  1. Either open the project using QT Creator or build it using qmake.

    
         cd OpenAuraSDK
         qmake OpenAuraSDK.pro
         make
     

  2. Allow access to SMBus:<br>

     - This can be identified by your motherboard
         ##### Non AMD
          - `i2cdetect -l` 
         ##### AMD 
          - The second SMBus isn't currently picked up by the kernel driver and that seems to be where Asus wired the Aura controllers so I have a kernel patch [here](https://gitlab.com/CalcProgrammer1/OpenAuraSDK/issues/9) that will allow the kernel to use the second bus (at `0x0b20`). Additionally, the AMD SMBus driver adds duplicate entries for the first bus (at `0x0b00`) so I've just been giving access to only one of them, otherwise devices will show up multiple times in the list.

     - You'll have to enable user access to your SMbus if you don't run as root, for instance: `sudo chmod 777 /dev/i2c-0`


