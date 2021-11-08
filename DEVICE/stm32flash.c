#include "stm32flash.h"

/**
 * @brief stm32�ڲ�flash��һ������
 * @param {uint32_t} faddr
 * @return {uint16_t} data
 */
uint16_t Stm32flash_Read_Half_Word(uint32_t faddr)
{
	return *(volatile uint16_t*)faddr; 
}

/**
 * @brief stm32flashд��һ�����ݣ��޲�����飩
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
		WriteAddr += 2; //��ַ����2.
	}
}

//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ر�ע��:��ΪSTM32F4������ʵ��̫��,û�취���ر�����������,���Ա�����
//         д��ַ�����0XFF,��ô���Ȳ������������Ҳ�������������.����
//         д��0XFF�ĵ�ַ,�����������������ݶ�ʧ.����д֮ǰȷ��������
//         û����Ҫ����,��������������Ȳ�����,Ȼ����������д. 
//�ú�����OTP����Ҳ��Ч!��������дOTP��!
//OTP�����ַ��Χ:0X1FFF7800~0X1FFF7A0F
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ4�ı���!!)
//pBuffer:����ָ��
//NumToWrite:��(32λ)��(����Ҫд���32λ���ݵĸ���.)
void Stm32flash_Write(uint32_t WriteAddr, uint16_t *pBuffer, uint16_t NumToWrite)
{
	FLASH_Status flash_status = FLASH_COMPLETE;
	uint32_t page_start_addr = 0;
	uint32_t addrx = 0;
	uint32_t endaddr = 0;

	if (WriteAddr % 2 != 0)
	{
		return; //д���ַ����2�ı���
	}
	if (WriteAddr < STM32_FLASH_BASE_ADDR || (WriteAddr >= (STM32_FLASH_BASE_ADDR + 1024 * STM32_FLASH_SIZE_KB)))
	{
		return; //�Ƿ���ַ
	}

	FLASH_Unlock(); //����

	addrx = WriteAddr;					  //д�����ʼ��ַ
	endaddr = WriteAddr + NumToWrite * 2; //д��Ľ�����ַ

	while (addrx < endaddr) //ɨ��һ���ϰ�.(�Է�FFFFFFFF�ĵط�,�Ȳ���)
	{
		if (Stm32flash_Read_Half_Word(addrx) != 0XFFFF) //�з�0XFFFF�ĵط�,Ҫ�����������
		{
			page_start_addr = (uint16_t)((WriteAddr - STM32_FLASH_BASE_ADDR) / STM32_PAGE_SIZE_B) * STM32_PAGE_SIZE_B + STM32_FLASH_BASE_ADDR;
			//page_start_addr = WriteAddr - (WriteAddr%1024);
			flash_status = FLASH_ErasePage(page_start_addr);
			if (flash_status != FLASH_COMPLETE)
			{
				break; //����������
			}
		}
		else
		{
			addrx += 2;
		}
	}

	if (flash_status == FLASH_COMPLETE)
	{
		while (WriteAddr < endaddr) //д����
		{
			flash_status = FLASH_ProgramHalfWord(WriteAddr, *pBuffer);
			if (flash_status != FLASH_COMPLETE) //д������
			{
				break; //д���쳣
			}
			WriteAddr += 2;
			pBuffer++;
		}
	}
	FLASH_Lock(); //����
}

//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void Stm32flash_Read(uint32_t ReadAddr, uint16_t *pBuffer, uint16_t NumToRead)
{
	uint16_t i;
	for (i = 0; i < NumToRead; i++)
	{
		pBuffer[i] = Stm32flash_Read_Half_Word(ReadAddr); //��ȡ2���ֽ�.
		ReadAddr += 2;								      //ƫ��2���ֽ�.
	}
}
