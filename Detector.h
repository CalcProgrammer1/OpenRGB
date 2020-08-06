#pragma once

#include "DeviceDetector.h"

#define REGISTER_DETECTOR(name, func) static DeviceDetector device_detector_obj(name, func)
#define REGISTER_I2C_DETECTOR(name, func) static I2CDeviceDetector device_detector_obj(name, func)
#define REGISTER_I2C_BUS_DETECTOR(func) static I2CBusDetector device_detector_obj(func)
