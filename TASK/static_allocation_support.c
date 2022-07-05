/**
 * @file static_allocation_support.c
 * @author Bashpow (B31296637828@hotmail.com)
 * @brief FreeRTOS开启configSUPPORT_STATIC_ALLOCATION宏定义，所需的函数支持
 *        使用静态方法创建任务的时候需要将宏 configSUPPORT_STATIC_ALLOCATION 设置为 1，
 *        将这个宏配置为1 后进行编译会提示有两个函数未定义，这两个函数就是需要我们自己去重写的。
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
