OpenAuraSDK

This project is an open-source implementation of the Asus Aura RGB controller software.  It currently only supports a limited number of motherboard and RAM module lighting controllers.

It supports both Windows and Linux.  To build on Windows, download the latest Visual Studio Community edition and open the .sln.  You may have to retarget the project to the latest version of the Windows SDK and build tools and stuff.  Build for x86, the InpOut32 library I use does not support x64.

To build on Linux, you can either open the project using QT Creator or build it using qmake.

To build with qmake:

cd OpenAuraSDK

qmake OpenAuraSDK.pro

make

You'll have to enable user access to your SMbus if you don't run as root, for instance:

sudo chmod 777 /dev/i2c-0

You can figure out which SMBus is your motherboard's by using i2cdetect -l. For AMD boards, the second SMBus isn't currently picked up by the kernel driver and that seems to be where Asus wired the Aura controllers so I have a kernel patch here:

https://gitlab.com/CalcProgrammer1/OpenAuraSDK/issues/9

that will allow the kernel to use the second bus (at 0x0b20). Additionally, the AMD SMBus driver adds duplicate entries for the first bus (at 0x0b00) so I've just been giving access to only one of them, otherwise devices will show up multiple times in the list.