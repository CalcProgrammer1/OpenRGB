/*-----------------------------------------*\
|  JginYueInternalUSBController.cpp         |
|                                           |
|  Driver for JginYue internal USB          |
|  lighting controller                      |
|                                           |
|                                           |
|                                           |
\*-----------------------------------------*/
#include <cstring>
#include <stdio.h>

#include "RGBController.h"
#include "ResourceManager.h"
#include "SettingsManager.h"
#include "JginYueInternalUSBController.h"
#include "dependencies/dmiinfo.h"

#define JGINYUE_USB_GENERAL_COMMAND_HEADER              0x01
#define JGINYUE_USB_LED_STRIPE_SET_COMMAND_HEADER       0x02
#define JGINYUE_USB_MODE_SET_COMMAND_HEADER             0x03
#define JGINYUE_USB_PER_LED_SET_COMMAND_HEADER          0x04

#define JGINYUE_USB_GET_FW_VERSION                      0xA0
#define JGINYUE_USB_GET_FW_REPLY                        0x5A

#define JGINYUE_MAX_ZONES               2
#define JGINYUE_ADDRESSABLE_MAX_LEDS    100
