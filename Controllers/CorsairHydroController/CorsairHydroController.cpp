/*---------------------------------------------------------*\
|  Processing Code for Corsair Hydro Series                 |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 8/17/2020        |
\*---------------------------------------------------------*/

#include "CorsairHydroController.h"

CorsairHydroController::CorsairHydroController(libusb_device_handle* dev_handle)
{
    dev = dev_handle;
    
    SendFirmwareRequest();
}

std::string CorsairHydroController::GetFirmwareString()
{
    return(firmware_version);
}

void CorsairHydroController::SendFirmwareRequest()
{
    unsigned char usb_buf[8];
    int           actual;

    usb_buf[0] = 0xAA;

    libusb_bulk_transfer(dev, 0x01, usb_buf, 1, &actual, 1000);
    libusb_bulk_transfer(dev, 0x81, usb_buf, 7, &actual, 1000);

    firmware_version = std::to_string(usb_buf[3]) + "." + std::to_string(usb_buf[4]) + "." + std::to_string(usb_buf[5]) + "." + std::to_string(usb_buf[6]);
}

void CorsairHydroController::SetFixed
    (
        unsigned char   red,
        unsigned char   green,
        unsigned char   blue
    )
{
    unsigned char usb_buf[8];
    int           actual;

    usb_buf[0] = 0x56;
    usb_buf[1] = 0x02;
    usb_buf[2] = red;
    usb_buf[3] = green;
    usb_buf[4] = blue;
    usb_buf[5] = red;
    usb_buf[6] = green;
    usb_buf[7] = blue;

    libusb_bulk_transfer(dev, 0x01, usb_buf, 8, &actual, 1000);
    libusb_bulk_transfer(dev, 0x81, usb_buf, 3, &actual, 1000);

    SendApply();
}

void CorsairHydroController::SendApply()
{
    unsigned char usb_buf[3];
    int           actual;

    usb_buf[0] = 0x55;
    usb_buf[1] = 0x01;

    libusb_bulk_transfer(dev, 0x01, usb_buf, 2, &actual, 1000);
    libusb_bulk_transfer(dev, 0x81, usb_buf, 3, &actual, 1000);
}