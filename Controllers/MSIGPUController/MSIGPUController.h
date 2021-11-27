/*-----------------------------------------*\
|  MSIGPUController.h                       |
|                                           |
|  Definitions for MSI GPUs                 |
|                                           |
\*-----------------------------------------*/

#include <string>
#include "i2c_smbus.h"

#pragma once

typedef unsigned char msi_gpu_dev_id;

enum
{
    MSI_GPU_REG_BRIGHTNESS                 = 0x36,    /* MSI GPU Brightness Register     */
    MSI_GPU_REG_SPEED                      = 0x38,    /* MSI GPU Speed Register          */
    MSI_GPU_REG_UNKNOWN                    = 0x26,    /* MSI GPU Unknown Register        */
    MSI_GPU_REG_R1                         = 0x30,    /* MSI GPU R1 Register             */
    MSI_GPU_REG_G1                         = 0x31,    /* MSI GPU G1 Register             */
    MSI_GPU_REG_B1                         = 0x32,    /* MSI GPU B1 Register             */
    MSI_GPU_REG_R2                         = 0x27,    /* MSI GPU R2 Register             */
    MSI_GPU_REG_G2                         = 0x28,    /* MSI GPU G2 Register             */
    MSI_GPU_REG_B2                         = 0x29,    /* MSI GPU B2 Register             */
    MSI_GPU_REG_R3                         = 0x2a,    /* MSI GPU R3 Register             */
    MSI_GPU_REG_G3                         = 0x2b,    /* MSI GPU G3 Register             */
    MSI_GPU_REG_B3                         = 0x2c,    /* MSI GPU B3 Register             */
    MSI_GPU_REG_MODE                       = 0x22,    /* MSI GPU Mode Selection Register */
    MSI_GPU_REG_SAVE                       = 0x3f,    /* MSI GPU Save Changes Register  */
};

enum
{
    MSI_GPU_MODE_OFF                       = 0x01,        /* OFF mode                                            */
    MSI_GPU_MODE_RAINBOW                   = 0x08,        /* Rainbow effect mode                                 */
    MSI_GPU_MODE_STATIC                    = 0x13,        /* Static color mode                                   */
    MSI_GPU_MODE_RAINDROP                  = 0x1a,        /* Raindrop effect mode                                */
    MSI_GPU_MODE_MAGIC                     = 0x07,        /* Magic effect mode                                   */
    MSI_GPU_MODE_PATROLLING                = 0x05,        /* Patrolling effect mode                              */
    MSI_GPU_MODE_STREAMING                 = 0x06,        /* Streaming effect mode                               */
    MSI_GPU_MODE_LIGHTNING                 = 0x15,        /* Lightning effect mode                               */
    MSI_GPU_MODE_WAVE                      = 0x1f,        /* Wave effect mode                                    */
    MSI_GPU_MODE_METEOR                    = 0x16,        /* Meteor effect mode                                  */
    MSI_GPU_MODE_STACK                     = 0x0d,       /* Stack effect mode                                   */
    MSI_GPU_MODE_RHYTHM                    = 0x0b,       /* Rhythm effect mode                                  */
    MSI_GPU_MODE_FLOWING                   = 0x09,       /* Flowing effect mode                                 */
    MSI_GPU_MODE_WHIRLING                  = 0x0f,       /* Whirling effect mode                                */
    MSI_GPU_MODE_TWISTING                  = 0x11,       /* Twisting effect mode                                */
    MSI_GPU_MODE_LAMINATING                = 0x1d,       /* Laminating effect mode                              */
    MSI_GPU_MODE_FADEIN                    = 0x14,       /* Fadein effect mode                                  */
    MSI_GPU_MODE_BREATHING                 = 0x04,       /* Breathing effect mode                               */
    MSI_GPU_MODE_FLASHING                  = 0x02,       /* Flashing effect mode                                */
    MSI_GPU_MODE_DOUBLEFLASHING            = 0x03,       /* Doubleflashing effect mode                          */
};

class MSIGPUController
{
public:
    MSIGPUController(i2c_smbus_interface* bus);
    ~MSIGPUController();

    std::string   GetDeviceLocation();

    void          SetRGB1(unsigned char red, unsigned char green, unsigned char blue);
    void          SetRGB2(unsigned char red, unsigned char green, unsigned char blue);
    void          SetRGB3(unsigned char red, unsigned char green, unsigned char blue);

    void          SetMode(unsigned char mode);
    void          Save();

    unsigned char MSIGPURegisterRead(unsigned char reg);
    void          MSIGPURegisterWrite(unsigned char reg, unsigned char val);

private:
    i2c_smbus_interface *   bus;
    msi_gpu_dev_id          dev;
};
