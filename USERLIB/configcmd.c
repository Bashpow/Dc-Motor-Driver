#include "configcmd.h"

uint8_t Config_Cmd_Len_Check(uint8_t len)
{
	if (len == CONFIG_CMD_LENGTH)
	{
		return 1;
	}
	return 0;
}

uint8_t Config_Cmd_Rx_Format_Check(uint8_t *arg)
{
	// 帧头校验
	if ((arg[0] & 0xFE) != 0xB4)
	{
		return 0;
	}

	// 帧尾校验
	if ((arg[6] & 0x0F) != 0x09)
	{
		return 0;
	}

	return 1;
}

uint8_t Config_Cmd_Crc4_Itu_Check(uint8_t *arg)
{
	return 1;
}

uint8_t Config_Cmd_Cmd_Check(uint8_t *arg)
{
	return 1;
}

uint8_t Config_Cmd_Get_Control_Bit(uint8_t *arg)
{
	return (arg[1] & 0x03);
}

uint8_t Config_Cmd_Implement(uint8_t *arg)
{
	switch (arg[1])
	{
	case 0x00:
		return 1;
	case 0x01:
		return 1;
	case 0x02:
		return 1;
	}
	return 0;
}
