/*-----------------------------------------*\
|  color32.h                                |
|                                           |
|  Simple structure to allow passing of     |
|  32-bit color data between classes        |
|                                           |
|  David Lee (RAMChYLD) 15/11/2020          |
\*-----------------------------------------*/

#ifndef COLOR32_H
#define COLOR32_H

typedef struct{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
} color32;

#endif // COLOR32_H
