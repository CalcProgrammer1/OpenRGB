/*---------------------------------------------------------*\
| MSIGPUv2Controller.h                                      |
|                                                           |
|   Driver for MSI V2 GPU (ITE9)                            |
|                                                           |
|   Wojciech Lazarski                           03 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"

typedef unsigned char msi_gpu_dev_id;

#define MSI_GPU_V2_SPEED_MIN                0
#define MSI_GPU_V2_SPEED_MID                1
#define MSI_GPU_V2_SPEED_MAX                2
#define MSI_GPU_V2_BRIGHTNESS_MIN           1
#define MSI_GPU_V2_BRIGHTNESS_MAX           5
#define MSI_GPU_V2_BRIGHTNESS_MULTI         20

enum
{
    MSI_GPU_V2_REG_BRIGHTNESS               = 0x36,     /* MSI GPU Brightness Register     */
    MSI_GPU_V2_REG_SPEED                    = 0x38,     /* MSI GPU Speed Register          */
    MSI_GPU_V2_REG_UNKNOWN                  = 0x2E,     /* MSI GPU Unknown Register        */
    MSI_GPU_V2_REG_R1                       = 0x30,     /* MSI GPU R1 Register             */
    MSI_GPU_V2_REG_G1                       = 0x31,     /* MSI GPU G1 Register             */
    MSI_GPU_V2_REG_B1                       = 0x32,     /* MSI GPU B1 Register             */
    MSI_GPU_V2_REG_COLOR_BLOCK1_BASE        = 0x27,     /* MSI GPU 1 Color block register  */
    MSI_GPU_V2_REG_COLOR_BLOCK2_BASE        = 0x28,     /* MSI GPU 2 Color block register  */
    MSI_GPU_V2_REG_COLOR_BLOCK3_BASE        = 0x29,     /* MSI GPU 3 Color block register  */
    MSI_GPU_V2_REG_MODE                     = 0x22,     /* MSI GPU Mode Selection Register */
    MSI_GPU_V2_REG_SAVE                     = 0x3F,     /* MSI GPU Commit Changes Register */
    MSI_GPU_V2_REG_CONTROL                  = 0x46,     /* MSI GPU Direction Register      */
};

enum
{
    MSI_GPU_V2_CONTROL_DIRECTION_RIGHT      = 0x00,     /* Right direction light effects */
    MSI_GPU_V2_CONTROL_DIRECTION_LEFT       = 0x02,     /* Left direction light effects */
    MSI_GPU_V2_CONTROL_NON_RGBMODE          = 0x01,     /* Non RGB Mode - programming colors */
};

enum
{
    MSI_GPU_V2_MODE_IDLE                    = 0x1C,     /* Idle mode for programing ?                          */
    MSI_GPU_V2_MODE_OFF                     = 0x01,     /* OFF mode                                            */
    MSI_GPU_V2_MODE_RAINBOW                 = 0x08,     /* Rainbow effect mode                                 */
    MSI_GPU_V2_MODE_STATIC                  = 0x13,     /* Static color mode                                   */
    MSI_GPU_V2_MODE_RAINDROP                = 0x1A,     /* Raindrop effect mode                                */
    MSI_GPU_V2_MODE_MAGIC                   = 0x07,     /* Magic effect mode                                   */
    MSI_GPU_V2_MODE_PATROLLING              = 0x05,     /* Patrolling effect mode                              */
    MSI_GPU_V2_MODE_STREAMING               = 0x06,     /* Streaming effect mode                               */
    MSI_GPU_V2_MODE_LIGHTNING               = 0x15,     /* Lightning effect mode                               */
    MSI_GPU_V2_MODE_WAVE                    = 0x1F,     /* Wave effect mode                                    */
    MSI_GPU_V2_MODE_METEOR                  = 0x16,     /* Meteor effect mode                                  */
    MSI_GPU_V2_MODE_STACK                   = 0x0D,     /* Stack effect mode                                   */
    MSI_GPU_V2_MODE_RHYTHM                  = 0x0C,     /* Rhythm effect mode                                  */
    MSI_GPU_V2_MODE_FLOWING                 = 0x09,     /* Flowing effect mode                                 */
    MSI_GPU_V2_MODE_FLOWING2                = 0x0A,     /* Flowing effect mode V2                              */
    MSI_GPU_V2_MODE_WHIRLING                = 0x0F,     /* Whirling effect mode                                */
    MSI_GPU_V2_MODE_TWISTING                = 0x11,     /* Twisting effect mode                                */
    MSI_GPU_V2_MODE_LAMINATING              = 0x1D,     /* Laminating effect mode                              */
    MSI_GPU_V2_MODE_FADEIN                  = 0x14,     /* Fadein effect mode                                  */
    MSI_GPU_V2_MODE_BREATHING               = 0x04,     /* Breathing effect mode                               */
    MSI_GPU_V2_MODE_FLASHING                = 0x02,     /* Flashing effect mode                                */
    MSI_GPU_V2_MODE_DOUBLEFLASHING          = 0x03,     /* Doubleflashing effect mode                          */
};

class MSIGPUv2Controller
{
public:
    MSIGPUv2Controller(i2c_smbus_interface* bus, msi_gpu_dev_id dev, std::string dev_name);
    ~MSIGPUv2Controller();

    std::string   GetDeviceLocation();
    std::string   GetDeviceName();

    void          SetRGB1(unsigned char red, unsigned char green, unsigned char blue);
    void          SetRGB1V2(unsigned char red, unsigned char green, unsigned char blue);
    void          SetRGB2V2(unsigned char red, unsigned char green, unsigned char blue);
    void          SetRGB3V2(unsigned char red, unsigned char green, unsigned char blue);

    void          SetMode(unsigned char mode);
    void          Save();

    unsigned char MSIGPURegisterRead(unsigned char reg);
    void          MSIGPURegisterWrite(unsigned char reg, unsigned char val);
    void          MSIGPUBlockWrite(unsigned char reg,unsigned char *val, unsigned char len);

private:
    i2c_smbus_interface *   bus;
    msi_gpu_dev_id          dev;
    std::string             name;
};
