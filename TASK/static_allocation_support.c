/**
 * @file static_allocation_support.c
 * @author Bashpow (B31296637828@hotmail.com)
 * @brief FreeRTOS����configSUPPORT_STATIC_ALLOCATION�궨�壬����ĺ���֧��
 *        ʹ�þ�̬�������������ʱ����Ҫ���� configSUPPORT_STATIC_ALLOCATION ����Ϊ 1��
 *        �����������Ϊ1 ����б������ʾ����������δ���壬����������������Ҫ�����Լ�ȥ��д�ġ�
 * @version 1.0
 * @date 2022-07-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "FreeRTOS.h"
#include "task.h"

#if ( configSUPPORT_STATIC_ALLOCATION == 1 )

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
								   StackType_t **ppxIdleTaskStackBuffer,
								   uint32_t *pulIdleTaskStackSize)
{
	static StaticTask_t TldeTaskTCP;
	static StackType_t IdleTaskkStack[configMINIMAL_STACK_SIZE];
	*ppxIdleTaskTCBBuffer = &TldeTaskTCP;
	*ppxIdleTaskStackBuffer = IdleTaskkStack;
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

#endif
