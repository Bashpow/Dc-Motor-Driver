#ifndef __STM32FLASH_H_
#define __STM32FLASH_H_

#include "stm32f0xx.h"

#define STM32_FLASH_SIZE_KB   32     	 	//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_PAGE_SIZE_B     1024          //��ѡSTM32��FLASHһҳ�ֽ�
#define STM32_FLASH_BASE_ADDR 0x08000000 	//STM32 FLASH����ʼ��ַ

void Stm32flash_Write(uint32_t WriteAddr, uint16_t *pBuffer, uint16_t NumToWrite);
void Stm32flash_Read(uint32_t ReadAddr, uint16_t *pBuffer, uint16_t NumToRead);

#endif // __STM32FLASH_H_
