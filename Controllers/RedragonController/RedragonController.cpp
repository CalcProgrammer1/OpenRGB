#include "RedragonController.h"

#include <cstring>

static void send_usb_msg(hid_device* dev, char * data_pkt, unsigned int size)
{
    char* usb_pkt = new char[size + 1];
    
    usb_pkt[0] = 0x04;
    for(int i = 1; i < size + 1; i++)
    {
        usb_pkt[i] = data_pkt[i-1];
    }
    
    hid_write(dev, (unsigned char *)usb_pkt, size + 1);
    
    delete usb_pkt;
}

static void get_usb_msg(hid_device* dev, char* data_pkt, unsigned int size)
{
    char usb_pkt[65];
    usb_pkt[0] = 0x00;
    for(int i = 1; i < 65; i++)
    {
        usb_pkt[i] = data_pkt[i-1];
    }
    int bytes = hid_get_feature_report(dev, (unsigned char*)data_pkt, size);
}

RedragonController::RedragonController(hid_device* dev_handle)
{
    dev = dev_handle;

    //SendMouseMode(REDRAGON_MODE_STATIC, 0x01, 0x00, 0xFF, 0xFF);
    //SendMouseApply();

    unsigned char color_data[0x36];

    for(int i = 0; i < 0x36; i += 3)
    {
        color_data[i] = 0xFF;
        color_data[i+1] = 0x00;
        color_data[i+2] = 0x00;
    }

    color_data[51] = 0xFF;
    color_data[52] = 0xFF;
    color_data[53] = 0xFF;

    SendKeyboardBegin();
    SendKeyboardMode(REDRAGON_K556_MODE_CUSTOM);

    SendKeyboardData
        (
        color_data,
        0x36,
        0x36 + 0x36 + 0x36 + 0x36 + 0x36 + 0x36
        );
    SendKeyboardEnd();
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------*\
| Keyboard functions                                                                        |
\*-----------------------------------------------------------------------------------------*/

void RedragonController::SendKeyboardBegin()
{
    char usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Keyboard Begin packet                          |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x04;
    usb_buf[0x01]           = 0x01;
    usb_buf[0x02]           = 0x00;
    usb_buf[0x03]           = 0x01;
    
    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, (unsigned char *)usb_buf, 64);
    hid_read(dev, (unsigned char *)usb_buf, 64);
}

void RedragonController::SendKeyboardData
    (
    unsigned char *     data,
    unsigned char       data_size,
    unsigned short      data_offset
    )
{
    char usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Keyboard End packet                            |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x04;
    usb_buf[0x01]           = 0x11 + data_offset;
    usb_buf[0x02]           = data_size;
    usb_buf[0x03]           = ( data_size / 3 ) - 1;
    usb_buf[0x04]           = data_size;
    usb_buf[0x05]           = data_offset & 0x00FF;
    usb_buf[0x06]           = data_offset >> 8;

    /*-----------------------------------------------------*\
    | Copy in data bytes                                    |
    \*-----------------------------------------------------*/
    memcpy(&usb_buf[0x08], data, data_size);
    
    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, (unsigned char *)usb_buf, 64);
    hid_read(dev, (unsigned char *)usb_buf, 64);
}

void RedragonController::SendKeyboardMode
    (
    unsigned char       mode
    )
{
    char usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Keyboard End packet                            |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x04;
    usb_buf[0x01]           = 0x08 + mode;
    usb_buf[0x02]           = 0x00;
    usb_buf[0x03]           = 0x06;
    usb_buf[0x04]           = 0x01;

    usb_buf[0x08]           = mode;
    
    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, (unsigned char *)usb_buf, 64);
    hid_read(dev, (unsigned char *)usb_buf, 64);
}

void RedragonController::SendKeyboardEnd()
{
    char usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Keyboard End packet                            |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x04;
    usb_buf[0x01]           = 0x02;
    usb_buf[0x02]           = 0x00;
    usb_buf[0x03]           = 0x02;
    
    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, (unsigned char *)usb_buf, 64);
    hid_read(dev, (unsigned char *)usb_buf, 64);
}

/*-----------------------------------------------------------------------------------------*\
| Mouse functions                                                                           |
\*-----------------------------------------------------------------------------------------*/

void RedragonController::SendMouseApply()
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

void RedragonController::SendMouseMode
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
