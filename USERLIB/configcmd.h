#ifndef _CONFIGCMD_H_
#define _CONFIGCMD_H_

#include "stm32f0xx.h"
#include "crc4.h"

#define CONFIG_CMD_LENGTH 7u
#define READ_PERMISSION 0
#define WRITE_PERMISSION 1
#define NO_RW_PERMISSION 2
#define LIST_NUM 10

typedef struct
{
    uint8_t cmd_id;
    uint8_t read_wirte_permissions;
    uint8_t date_length;
    char cmd_reference[32];
} CMD_Data_t;

typedef enum
{
    Cmd_Data_None = 0u,
    Cmd_Data_uint8 = 1,
    Cmd_Data_int8 = 2,
    Cmd_Data_uint16 = 3,
    Cmd_Data_int16 = 4,
    Cmd_Data_uint32 = 5,
    Cmd_Data_int32 = 6,
    Cmd_Data_float32 = 7,
} CMD_Data_Len;

#endif //_CONFIGCMD_H_
