/*-----------------------------------------*\
|  HyperXPulsefireSurgeController.cpp       |
|                                           |
|  Driver for HyperX Pulsefire Surge        |
|  lighting controller                      |
|                                           |
|  Adam Honse (CalcProgrammer1) 7/25/2020   |
\*-----------------------------------------*/

#include "HyperXPulsefireSurgeController.h"

#include <cstring>

HyperXPulsefireSurgeController::HyperXPulsefireSurgeController(hid_device* dev_handle)
{
    dev = dev_handle;
}

HyperXPulsefireSurgeController::~HyperXPulsefireSurgeController()
{

}