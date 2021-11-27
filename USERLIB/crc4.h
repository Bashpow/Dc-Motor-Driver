#ifndef _CRC4_H_
#define _CRC4_H_

#include "stdio.h"

#define CRC4_INIT_VALUE 0x00

unsigned char CRC4_Itu(unsigned char crc, unsigned char *data, unsigned char len);

#endif //_CRC4_H_
