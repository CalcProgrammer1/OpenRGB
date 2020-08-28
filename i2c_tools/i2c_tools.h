#include <string>
#include "i2c_smbus.h"

#define MODE_AUTO   0
#define MODE_QUICK  1
#define MODE_READ   2
#define MODE_FUNC   3

std::string i2c_detect(i2c_smbus_interface * bus, int mode);

std::string i2c_dump(i2c_smbus_interface * bus, unsigned char address);

std::string i2c_read(i2c_smbus_interface * bus, unsigned char address, unsigned char regaddr, unsigned char size);
