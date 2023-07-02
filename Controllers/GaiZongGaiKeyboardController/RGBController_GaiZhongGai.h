/*---------------------------------------------------------------*\
|  RGBController_GaiZhongGai.h                                    |
|                                                                 |
| https://oshwlab.com/yangdsada/GaiZhongGai-Keyboard-68-4PRO      |
| https://oshwhub.com/myng/42-jian-pan                            |
| https://oshwhub.com/hivisme/17jian-shuo-zi-xiao-jian-pan        |
| https://oshwhub.com/yangzen/xing-huo-2-qi-guang-ban-qu-dong-    |
| https://oshwhub.com/morempty/CH552gyin-liang-xuan-niu           |
|                                                                 |
| An Yang     2023/6/24                                           |
\*---------------------------------------------------------------*/

#pragma once
#include "RGBController.h"
#include "GaiZhongGaiController.h"

class RGBController_GaiZhongGaiKeyboard : public RGBController
{
public:
    RGBController_GaiZhongGaiKeyboard(GaiZhongGaiKeyboardController* controller_ptr);
    ~RGBController_GaiZhongGaiKeyboard();

    void SetupZones();
    void ResizeZone(int zone, int new_size);
    
    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    GaiZhongGaiKeyboardController*    controller;
};
