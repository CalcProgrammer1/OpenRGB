/*---------------------------------------------------------*\
| ZotacBlackwellGPUController.h                             |
|                                                           |
|   Driver for ZOTAC Blackwell (RTX 50 series) GPU          |
|                                                           |
|   Eder Sánchez                                27 Mar 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"
#include "RGBController.h"

/*---------------------------------------------------------*\
| ZOTAC Blackwell I2C address                               |
\*---------------------------------------------------------*/
#define ZOTAC_BLACKWELL_GPU_ADDR                0x4B

/*---------------------------------------------------------*\
| Register map (0x20 - 0x2F)                                |
\*---------------------------------------------------------*/
#define ZOTAC_BLACKWELL_GPU_REG_FIXED           0x20
#define ZOTAC_BLACKWELL_GPU_REG_ZONE            0x21
#define ZOTAC_BLACKWELL_GPU_REG_MODE            0x22
#define ZOTAC_BLACKWELL_GPU_REG_RED1            0x23
#define ZOTAC_BLACKWELL_GPU_REG_GREEN1          0x24
#define ZOTAC_BLACKWELL_GPU_REG_BLUE1           0x25
#define ZOTAC_BLACKWELL_GPU_REG_RED2            0x26
#define ZOTAC_BLACKWELL_GPU_REG_GREEN2          0x27
#define ZOTAC_BLACKWELL_GPU_REG_BLUE2           0x28
#define ZOTAC_BLACKWELL_GPU_REG_BRIGHTNESS      0x29
#define ZOTAC_BLACKWELL_GPU_REG_SPEED           0x2A
#define ZOTAC_BLACKWELL_GPU_REG_DIRECTION       0x2B
#define ZOTAC_BLACKWELL_GPU_REG_RESERVED_2C     0x2C
#define ZOTAC_BLACKWELL_GPU_REG_RESERVED_2D     0x2D
#define ZOTAC_BLACKWELL_GPU_REG_RESERVED_2E     0x2E
#define ZOTAC_BLACKWELL_GPU_REG_RESERVED_2F     0x2F

/*---------------------------------------------------------*\
| Control registers                                         |
\*---------------------------------------------------------*/
#define ZOTAC_BLACKWELL_GPU_REG_RELOAD          0x11
#define ZOTAC_BLACKWELL_GPU_REG_COMMIT          0x17

/*---------------------------------------------------------*\
| Zone indices                                              |
\*---------------------------------------------------------*/
#define ZOTAC_BLACKWELL_GPU_ZONE_LOGO           0x00
#define ZOTAC_BLACKWELL_GPU_ZONE_SIDEBAR        0x01
#define ZOTAC_BLACKWELL_GPU_ZONE_INFINITY       0x02
#define ZOTAC_BLACKWELL_GPU_NUM_ZONES           3

/*---------------------------------------------------------*\
| Mode values (from Firestorm V5.0.0.012E reverse eng.)     |
\*---------------------------------------------------------*/
#define ZOTAC_BLACKWELL_GPU_MODE_STATIC         0x01
#define ZOTAC_BLACKWELL_GPU_MODE_BREATHE        0x02
#define ZOTAC_BLACKWELL_GPU_MODE_FADE           0x03
#define ZOTAC_BLACKWELL_GPU_MODE_WINK           0x04
#define ZOTAC_BLACKWELL_GPU_MODE_GLIDE          0x08
#define ZOTAC_BLACKWELL_GPU_MODE_PRISM          0x09
#define ZOTAC_BLACKWELL_GPU_MODE_BOKEH          0x0A
#define ZOTAC_BLACKWELL_GPU_MODE_BEACON         0x0B
#define ZOTAC_BLACKWELL_GPU_MODE_TANDEM         0x18
#define ZOTAC_BLACKWELL_GPU_MODE_TIDAL          0x19
#define ZOTAC_BLACKWELL_GPU_MODE_ASTRA          0x20
#define ZOTAC_BLACKWELL_GPU_MODE_COSMIC         0x21
#define ZOTAC_BLACKWELL_GPU_MODE_VOLTA          0x22

/*---------------------------------------------------------*\
| Direction values                                          |
\*---------------------------------------------------------*/
#define ZOTAC_BLACKWELL_GPU_DIR_LEFT            0x00
#define ZOTAC_BLACKWELL_GPU_DIR_RIGHT           0x01

/*---------------------------------------------------------*\
| I2C transaction delays (microseconds)                     |
\*---------------------------------------------------------*/
#define ZOTAC_BLACKWELL_GPU_DELAY_US            3000
#define ZOTAC_BLACKWELL_GPU_COMMIT_DELAY_US     10000

class ZotacBlackwellGPUController
{
public:
    ZotacBlackwellGPUController(i2c_smbus_interface* bus, u8 dev, std::string dev_name);
    ~ZotacBlackwellGPUController();

    std::string             GetDeviceLocation();
    std::string             GetName();
    std::string             GetVersion();

    void                    SetMode(unsigned int zone, unsigned int mode_val,
                                    RGBColor color1, RGBColor color2,
                                    unsigned int brightness, unsigned int speed,
                                    unsigned int direction);
    void                    Commit();

private:
    i2c_smbus_interface*    bus;
    u8                      dev;
    std::string             name;
    std::string             version;

    void                    ReadVersion();
};
