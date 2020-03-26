#include "RedragonM711Controller.h"

#include <cstring>

static void send_usb_msg(hid_device* dev, char * data_pkt, unsigned int size)
{
    char* usb_pkt = new char[size + 1];
    
    usb_pkt[0] = 0x00;
    for(int i = 1; i < size + 1; i++)
    {
        usb_pkt[i] = data_pkt[i-1];
    }
    
    hid_send_feature_report(dev, (unsigned char *)usb_pkt, size + 1);
    
    delete usb_pkt;
}

RedragonM711Controller::RedragonM711Controller(hid_device* dev_handle)
{
    dev = dev_handle;
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void RedragonM711Controller::SendMouseApply()
{
    char usb_buf[16];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Apply packet                                   |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x02;
    usb_buf[0x01]           = 0xF1;
    usb_buf[0x02]           = 0x02;
    usb_buf[0x03]           = 0x04;
    
    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    send_usb_msg(dev, usb_buf, 16);
}

void RedragonM711Controller::SendMouseMode
    (
    unsigned char       mode,
    unsigned char       speed,
    unsigned char       red,
    unsigned char       green,
    unsigned char       blue
    )
{
    char usb_buf[16];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Lighting Control packet                        |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x02;
    usb_buf[0x01]           = 0xF3;
    usb_buf[0x02]           = 0x49;
    usb_buf[0x03]           = 0x04;
    usb_buf[0x04]           = 0x06;

    usb_buf[0x08]           = red;
    usb_buf[0x09]           = green;
    usb_buf[0x0A]           = blue;

    usb_buf[0x0B]           = 0x01; //on

    usb_buf[0x0C]           = speed;
    usb_buf[0x0D]           = mode;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    send_usb_msg(dev, usb_buf, 16);
}
