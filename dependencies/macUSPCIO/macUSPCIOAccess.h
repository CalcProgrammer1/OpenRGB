//
//  macUSPCIOAccess.h
//  Access methods for macUSPCIO
//
//  Created by Shady Nawara
//  MacUSPCIO: https://github.com/ShadyNawara/macUSPCIO
//

#ifndef macUSPCIOAccess_h
#define macUSPCIOAccess_h

#include <IOKit/IOKitLib.h>

extern io_connect_t macUSPCIO_driver_connection;

inline bool InitMacUSPCIODriver()
{
    if(macUSPCIO_driver_connection)
    {
        return true;
    }
    
    io_service_t dev = IOServiceGetMatchingService(kIOMasterPortDefault, IOServiceMatching("macUSPCIO"));
    if (dev)
    {
        kern_return_t ret = IOServiceOpen(dev, mach_task_self(), 0, &macUSPCIO_driver_connection);
        IOObjectRelease(dev);
        return ret == kIOReturnSuccess;
    }
    return false;
}

inline bool GetMacUSPCIODriverStatus() {
    if(macUSPCIO_driver_connection)
    {
        return true;
    }
    return false;
}

inline void CloseMacUSPCIODriver() {
    if(macUSPCIO_driver_connection) {
        IOServiceClose(macUSPCIO_driver_connection);
        macUSPCIO_driver_connection = 0;
    }
}

inline uint8_t ReadIoPortByte(uint16_t address)
{
    if(!macUSPCIO_driver_connection)
    {
        return 0;
    }

    uint32_t outputCount = 1;
    uint64_t output = 0;
    uint64_t input[1] = {address};
    
    IOConnectCallScalarMethod(macUSPCIO_driver_connection, 0, input, 1, &output, &outputCount);
        
    return (uint8_t)output;
}

inline void WriteIoPortByte(uint16_t address, uint8_t value)
{
    if(!macUSPCIO_driver_connection)
    {
        return;
    }

    uint32_t outputCount = 1;
    uint64_t output = 0;
    uint64_t input[2] = {address, value};
    
    IOConnectCallScalarMethod(macUSPCIO_driver_connection, 1, input, 2, &output, &outputCount);
}

inline uint8_t ReadConfigPortByte(uint16_t address) {
    if(!macUSPCIO_driver_connection)
    {
        return 0;
    }

    uint32_t outputCount = 1;
    uint64_t output = 0;
    uint64_t input[1] = {address};
    
    IOConnectCallScalarMethod(macUSPCIO_driver_connection, 2, input, 1, &output, &outputCount);
        
    return (uint8_t)output;
}

inline uint16_t ReadConfigPortWord(uint16_t address)
{
    if(!macUSPCIO_driver_connection)
    {
        return 0;
    }

    uint32_t outputCount = 1;
    uint64_t output = 0;
    uint64_t input[1] = {address};
    
    IOConnectCallScalarMethod(macUSPCIO_driver_connection, 3, input, 1, &output, &outputCount);
        
    return (uint16_t)output;
}


#endif /* macUSPCIOAccess_h */
