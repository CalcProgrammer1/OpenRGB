#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string *> find_usb_serial_port(unsigned short vid, unsigned short pid);
