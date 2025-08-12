/*---------------------------------------------------------*\
| CorsairICueLinkProtocol.h                                 |
|                                                           |
|   Driver for Corsair iCue Link System Hub                 |
|                                                           |
|   Aiden Vigue (acvigue)                       02 Mar 2025 |
|   Nikola Jurkovic (jurkovic.nikola)           11 Aug 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>

typedef struct CorsairICueLinkDevice
{
    unsigned char   type         = 0x00;
    unsigned char   model        = 0x00;
    std::string     display_name = "Unknown";
    unsigned char   led_channels = 0;
    bool            internal     = false;
} CorsairICueLinkDevice;

static const CorsairICueLinkDevice known_devices[] =
{
    { 0x05, 0x02, "iCUE LINK 5000T RGB",            160 },
    { 0x05, 0x01, "iCUE LINK 9000D RGB AIRFLOW",    22  },
    { 0x05, 0x00, "iCUE LINK ADAPTER",              0   },
    { 0x06, 0x00, "iCUE LINK COOLER PUMP LCD",      24  },
    { 0x11, 0x00, "iCUE LINK TITAN 240",            20  },
    { 0x11, 0x04, "iCUE LINK TITAN 240",            20  },
    { 0x07, 0x00, "iCUE LINK H100i RGB",            20  },
    { 0x07, 0x04, "iCUE LINK H100i RGB",            20  },
    { 0x11, 0x01, "iCUE LINK TITAN 280",            20  },
    { 0x07, 0x01, "iCUE LINK H115i RGB",            20  },
    { 0x11, 0x02, "iCUE LINK TITAN 360",            20  },
    { 0x11, 0x05, "iCUE LINK TITAN 360",            20  },
    { 0x07, 0x02, "iCUE LINK H150i RGB",            20  },
    { 0x07, 0x05, "iCUE LINK H150i RGB",            20  },
    { 0x11, 0x03, "iCUE LINK TITAN 420",            20  },
    { 0x07, 0x03, "iCUE LINK H170i RGB",            20  },
    { 0x10, 0x00, "VRM COOLER MODULE",              0   },
    { 0x02, 0x00, "iCUE LINK LX RGB",               18  },
    { 0x14, 0x00, "ORIGIN OA",                      0   },
    { 0x01, 0x00, "iCUE LINK QX RGB",               34  },
    { 0x13, 0x00, "iCUE LINK RX",                   0   },
    { 0x04, 0x00, "iCUE LINK RX MAX",               0   },
    { 0x0F, 0x00, "iCUE LINK RX RGB",               8   },
    { 0x03, 0x00, "iCUE LINK RX RGB MAX",           8   },
    { 0x09, 0x00, "iCUE LINK XC7 ELITE",            24  },
    { 0x0C, 0x00, "iCUE LINK XD5 ELITE",            22  },
    { 0x0E, 0x00, "iCUE LINK XD5 ELITE LCD",        22, true  },
    { 0x19, 0x00, "iCUE LINK XD6 ELITE",            22  },
    { 0x0A, 0x00, "iCUE LINK XG3 HYBRID",           0   },
    { 0x0D, 0x00, "iCUE LINK XG7 RGB",              16  }
};

//Lengths
#define CORSAIR_ICUE_LINK_BUFFER_WRITE_LENGTH        513
#define CORSAIR_ICUE_LINK_BUFFER_READ_LENGTH         512
#define CORSAIR_ICUE_LINK_READ_HEADER_SIZE           3
#define CORSAIR_ICUE_LINK_WRITE_HEADER_SIZE          4
#define CORSAIR_ICUE_LINK_MAXIMUM_BUFFER_PER_REQUEST 508

//Commands
#define CORSAIR_ICUE_LINK_CMD_OPEN_ENDPOINT          {0x0d, 0x01}
#define CORSAIR_ICUE_LINK_CMD_OPEN_COLOR_ENDPOINT    {0x0d, 0x00}
#define CORSAIR_ICUE_LINK_CMD_CLOSE_ENDPOINT         {0x05, 0x01, 0x01}
#define CORSAIR_ICUE_LINK_CMD_GET_FIRMWARE           {0x02, 0x13}
#define CORSAIR_ICUE_LINK_CMD_SOFTWARE_MODE          {0x01, 0x03, 0x00, 0x02}
#define CORSAIR_ICUE_LINK_CMD_HARDWARE_MODE          {0x01, 0x03, 0x00, 0x01}
#define CORSAIR_ICUE_LINK_CMD_WRITE                  {0x06, 0x01}
#define CORSAIR_ICUE_LINK_CMD_WRITE_COLOR            {0x06, 0x00}
#define CORSAIR_ICUE_LINK_CMD_WRITE_COLOR_NEXT       {0x07, 0x00}
#define CORSAIR_ICUE_LINK_CMD_READ                   {0x08, 0x01}
#define CORSAIR_ICUE_LINK_CMD_GET_DEVICE_MODE        {0x01, 0x08, 0x01}

//Command modes
#define CORSAIR_ICUE_LINK_MODE_GET_DEVICES           {0x36,}
#define CORSAIR_ICUE_LINK_MODE_GET_TEMPERATURES      {0x21,}
#define CORSAIR_ICUE_LINK_MODE_GET_SPEEDS            {0x17,}
#define CORSAIR_ICUE_LINK_MODE_SET_SPEED             {0x18,}
#define CORSAIR_ICUE_LINK_MODE_SET_COLOR             {0x22,}

//Command data types
#define CORSAIR_ICUE_LINK_DATA_TYPE_GET_DEVICES      {0x21, 0x00}
#define CORSAIR_ICUE_LINK_DATA_TYPE_GET_TEMPERATURES {0x10, 0x00}
#define CORSAIR_ICUE_LINK_DATA_TYPE_GET_SPEEDS       {0x25, 0x00}
#define CORSAIR_ICUE_LINK_DATA_TYPE_SET_SPEED        {0x07, 0x00}
#define CORSAIR_ICUE_LINK_DATA_TYPE_SET_COLOR        {0x12, 0x00}

typedef enum CORSAIR_ICUE_ENDPOINT_TYPE
{
    CORSAIR_ICUE_ENDPOINT_TYPE_DEFAULT,
    CORSAIR_ICUE_ENDPOINT_TYPE_COLOR
} CORSAIR_ICUE_ENDPOINT_TYPE;

const CorsairICueLinkDevice* FindCorsairICueLinkDevice(unsigned char type, unsigned char model);
