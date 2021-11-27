#include "configcmd.h"

static CMD_Data_t cmd_list[8] = {
    {0x00, NO_RW_PERMISSION, Cmd_Data_None, "Already keep, no use"},
    {0x01, READ_PERMISSION, Cmd_Data_uint32, "Electrical identification code"},
    {0x02, READ_PERMISSION, Cmd_Data_uint32, "Electrical error code"},
    {0x03, NO_RW_PERMISSION, Cmd_Data_uint8, "Set success"},
};

int8_t Find_Index_From_Cmd_ID(uint8_t cmd_id)
{
    for (uint8_t i = 0; i < CONFIG_CMD_LENGTH; i++)
    {
        if (cmd_list[i].cmd_id == cmd_id)
            return i;
    }
    return -1;
}

/**
 * @brief 对接收到的上位机数据长度进行判断
 * @param len 
 * @return uint8_t 
 */
uint8_t Config_Cmd_Len_Check(uint8_t len)
{
    if (len == CONFIG_CMD_LENGTH)
    {
        return 1;
    }
    return 0;
}

/**
 * @brief 对上位机的数据进行帧头和帧尾的检验
 * @attention 在接收时倒数第二位一定为 0
 * @note  Frame Header 格式
 *        Bit 7:2   固定为 0x2D(0b101101)
 *        Bit 1:0   0b 00：上位机写电调参数
 *                  0b 01：上位机读电调参数
 *                  0b 10：电调返回参数(参数未被修改)
 *                  0b 11：电调返回参数(参数已被修改)
 * @param arg 
 * @return uint8_t 
 */
uint8_t Config_Cmd_Rx_Format_Check(uint8_t *arg)
{
    ///< 对接收到的上位机数据进行帧头校验
    if ((arg[0] & 0xFE) != 0xB4) ///< (0x2d << 2) = 0xB4
    {
        return 0;
    }

    ///< 帧尾校验
    if ((arg[6] & 0x0F) != 0x09)
    {
        return 0;
    }

    return 1;
}

uint8_t Config_Cmd_Crc4_Itu_Check(uint8_t *arg)
{
    return ((arg[6] >> 4) == CRC4_Itu(CRC4_INIT_VALUE, arg, CONFIG_CMD_LENGTH - 1));
}

uint8_t Config_Cmd_CMD_Check(uint8_t *arg)
{
    return 1;
}

uint8_t Config_Cmd_Get_Control_Bit(uint8_t *arg)
{
    return (arg[1] & 0x03);
}

uint8_t Config_Cmd_Implement(uint8_t *arg)
{
    int8_t index = Find_Index_From_Cmd_ID((arg[1]));
    if (!index)
        return 0;
    if (cmd_list[index].read_wirte_permissions == READ_PERMISSION)
    {
        return 0;
    }
    else if (cmd_list[index].read_wirte_permissions == WRITE_PERMISSION)
    {
        return 0;
    }

    return 1;
}
void Parse_Cmd_Data(uint8_t *arg, uint8_t data_len_type, int32_t result)
{
    switch (data_len_type)
    {
    case Cmd_Data_uint8:
    {
        
        break;
    }

    case Cmd_Data_int8:
    {
        break;
    }

    case Cmd_Data_uint16:
    {
        break;
    }

    case Cmd_Data_int16:
    {
        break;
    }

    case Cmd_Data_uint32:
    {
        break;
    }

    case Cmd_Data_int32:
    {
        break;
    }

    case Cmd_Data_float32:
    {
        break;
    }

    default:
        break;
    }
}
