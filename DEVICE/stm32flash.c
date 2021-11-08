#include "stm32flash.h"

/**
 * @brief stm32内部flash读一个半字
 * @param {uint32_t} faddr
 * @return {uint16_t} data
 */
uint16_t Stm32flash_Read_Half_Word(uint32_t faddr)
{
	return *(volatile uint16_t*)faddr; 
}

/**
 * @brief stm32flash写入一串数据（无擦除检查）
 * @param {uint32_t} WriteAddr 
 * @param {uint16_t} *pBuffer
 * @param {uint16_t} NumToWrite
 * @return {*}
 */
void Stm32flash_Write_Nocheck(uint32_t WriteAddr, uint16_t *pBuffer, uint16_t NumToWrite)
{
	for (uint16_t i = 0; i < NumToWrite; i++)
	{
		FLASH_ProgramHalfWord(WriteAddr, pBuffer[i]);
		WriteAddr += 2; //地址增加2.
	}
}

//从指定地址开始写入指定长度的数据
//特别注意:因为STM32F4的扇区实在太大,没办法本地保存扇区数据,所以本函数
//         写地址如果非0XFF,那么会先擦除整个扇区且不保存扇区数据.所以
//         写非0XFF的地址,将导致整个扇区数据丢失.建议写之前确保扇区里
//         没有重要数据,最好是整个扇区先擦除了,然后慢慢往后写. 
//该函数对OTP区域也有效!可以用来写OTP区!
//OTP区域地址范围:0X1FFF7800~0X1FFF7A0F
//WriteAddr:起始地址(此地址必须为4的倍数!!)
//pBuffer:数据指针
//NumToWrite:字(32位)数(就是要写入的32位数据的个数.)
void Stm32flash_Write(uint32_t WriteAddr, uint16_t *pBuffer, uint16_t NumToWrite)
{
	FLASH_Status flash_status = FLASH_COMPLETE;
	uint32_t page_start_addr = 0;
	uint32_t addrx = 0;
	uint32_t endaddr = 0;

	if (WriteAddr % 2 != 0)
	{
		return; //写入地址不是2的倍数
	}
	if (WriteAddr < STM32_FLASH_BASE_ADDR || (WriteAddr >= (STM32_FLASH_BASE_ADDR + 1024 * STM32_FLASH_SIZE_KB)))
	{
		return; //非法地址
	}

	FLASH_Unlock(); //解锁

	addrx = WriteAddr;					  //写入的起始地址
	endaddr = WriteAddr + NumToWrite * 2; //写入的结束地址

	while (addrx < endaddr) //扫清一切障碍.(对非FFFFFFFF的地方,先擦除)
	{
		if (Stm32flash_Read_Half_Word(addrx) != 0XFFFF) //有非0XFFFF的地方,要擦除这个扇区
		{
			page_start_addr = (uint16_t)((WriteAddr - STM32_FLASH_BASE_ADDR) / STM32_PAGE_SIZE_B) * STM32_PAGE_SIZE_B + STM32_FLASH_BASE_ADDR;
			//page_start_addr = WriteAddr - (WriteAddr%1024);
			flash_status = FLASH_ErasePage(page_start_addr);
			if (flash_status != FLASH_COMPLETE)
			{
				break; //发生错误了
			}
		}
		else
		{
			addrx += 2;
		}
	}

	if (flash_status == FLASH_COMPLETE)
	{
		while (WriteAddr < endaddr) //写数据
		{
			flash_status = FLASH_ProgramHalfWord(WriteAddr, *pBuffer);
			if (flash_status != FLASH_COMPLETE) //写入数据
			{
				break; //写入异常
			}
			WriteAddr += 2;
			pBuffer++;
		}
	}
	FLASH_Lock(); //上锁
}

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
void Stm32flash_Read(uint32_t ReadAddr, uint16_t *pBuffer, uint16_t NumToRead)
{
	uint16_t i;
	for (i = 0; i < NumToRead; i++)
	{
		pBuffer[i] = Stm32flash_Read_Half_Word(ReadAddr); //读取2个字节.
		ReadAddr += 2;								      //偏移2个字节.
	}
}
