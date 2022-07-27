#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE. 
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

//��Բ�ͬ�ı��������ò�ͬ��stdint.h�ļ�
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
    #include <stdint.h>
	#include "stdio.h"
    extern uint32_t SystemCoreClock;
#endif

/*******************************/
/*   FreeRTOS������������ѡ��   */
/*******************************/
#define configUSE_PREEMPTION					1                       //1ʹ����ռʽ�ںˣ�0ʹ��Э��
#define configUSE_TIME_SLICING					1						//1ʹ��ʱ��Ƭ����(Ĭ��ʽʹ�ܵ�)
#define configUSE_PORT_OPTIMISED_TASK_SELECTION	1                       //1�������ⷽ����ѡ����һ��Ҫ���е�����
                                                                        //һ����Ӳ������ǰ����ָ������ʹ�õ�
                                                                        //MCUû����ЩӲ��ָ��Ļ��˺�Ӧ������Ϊ0��
#define configUSE_TICKLESS_IDLE					0                       //1���õ͹���ticklessģʽ
#define configUSE_QUEUE_SETS				  	1                       //Ϊ1ʱ���ö��м�
#define configCPU_CLOCK_HZ				  		( SystemCoreClock )     //CPUƵ��
#define configTICK_RATE_HZ				  		( 1000 )                //ʱ�ӽ���Ƶ�ʣ���������Ϊ1000�����ھ���1ms
#define configMAX_PRIORITIES				   	( 8 )                   //��ʹ�õ�������ȼ�
#define configMINIMAL_STACK_SIZE				((unsigned short)128)   //��������ʹ�õĶ�ջ��С
#define configMAX_TASK_NAME_LEN					( 8 )                   //���������ַ�������

#define configUSE_16_BIT_TICKS					0                       //ϵͳ���ļ����������������ͣ�1��ʾΪ16λ�޷������Σ�0��ʾΪ32λ�޷�������
#define configIDLE_SHOULD_YIELD					1                       //Ϊ1ʱ�����������CPUʹ��Ȩ������ͬ���ȼ����û�����
#define configUSE_TASK_NOTIFICATIONS            1                       //Ϊ1ʱ��������֪ͨ���ܣ�Ĭ�Ͽ���
#define configUSE_MUTEXES						1                       //Ϊ1ʱʹ�û����ź���
#define configQUEUE_REGISTRY_SIZE				8                       //��Ϊ0ʱ��ʾ���ö��м�¼�������ֵ�ǿ��ԣ���¼�Ķ��к��ź��������Ŀ��
#define configCHECK_FOR_STACK_OVERFLOW          0                       //����0ʱ���ö�ջ�����⹦�ܣ����ʹ�ô˹���
                                                                        //�û������ṩһ��ջ������Ӻ��������ʹ�õĻ�
                                                                        //��ֵ����Ϊ1����2����Ϊ������ջ�����ⷽ����
#define configUSE_RECURSIVE_MUTEXES				1                       //Ϊ1ʱʹ�õݹ黥���ź���
#define configUSE_MALLOC_FAILED_HOOK			0                       //1ʹ���ڴ�����ʧ�ܹ��Ӻ���
#define configUSE_APPLICATION_TASK_TAG		    0
#define configUSE_COUNTING_SEMAPHORES			0                       //Ϊ1ʱʹ�ü����ź���

/************************************/
/*   FreeRTOS���ڴ������й�����ѡ��   */
/************************************/
#define configAPPLICATION_ALLOCATED_HEAP        0                       //Ĭ������£�FreeRTOS����FreeRTOS�����������������������ڴ��С�������Ӧ�ó����д�������ѣ�������Ӧ�ó����д�����ѷ������ڴ�������λ�á�
#define configSUPPORT_STATIC_ALLOCATION         1                       //����ʹ��Ӧ�ó����д���ṩ��RAM����RTOS����
#define configSUPPORT_DYNAMIC_ALLOCATION        0                       //֧�ֶ�̬�ڴ�����
#define configTOTAL_HEAP_SIZE					((size_t)(17*1024))     //ϵͳ�����ܵĶѴ�С

/**************************************/
/*   FreeRTOS�빳�Ӻ����йص�����ѡ��   */
/**************************************/
#define configUSE_IDLE_HOOK						0                       //1��ʹ�ÿ��й��ӣ�0����ʹ��
#define configUSE_TICK_HOOK						0                       //1��ʹ��ʱ��Ƭ���ӣ�0����ʹ��

/**********************************/
/*   FreeRTOS��Э���йص�����ѡ��   */
/**********************************/
#define configUSE_CO_ROUTINES 			        0                       //Ϊ1ʱ����Э�̣�����Э���Ժ��������ļ�croutine.c
#define configMAX_CO_ROUTINE_PRIORITIES         ( 2 )                   //Э�̵���Ч���ȼ���Ŀ

/****************************************/
/*   FreeRTOS�������ʱ���йص�����ѡ��   */
/****************************************/
#define configUSE_TIMERS                         0                               //Ϊ1ʱ���������ʱ��
#define configTIMER_TASK_PRIORITY                (configMAX_PRIORITIES-1)        //�����ʱ�����ȼ�
#define configTIMER_QUEUE_LENGTH                 5                               //�����ʱ�����г���
#define configTIMER_TASK_STACK_DEPTH             (configMINIMAL_STACK_SIZE*2)    //�����ʱ�������ջ��С

/*******************************/
/*   FreeRTOS��ѡ��������ѡ��   */
/*******************************/
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_vTaskPrioritySet		        1
#define INCLUDE_uxTaskPriorityGet		        0
#define INCLUDE_vTaskDelete				        0
#define INCLUDE_vTaskCleanUpResources	        1
#define INCLUDE_vTaskSuspend			        0
#define INCLUDE_vTaskDelayUntil			        1
#define INCLUDE_vTaskDelay				        1
#define INCLUDE_eTaskGetState			        0
#define INCLUDE_xTimerPendFunctionCall	        0

/**********************************/
/*   FreeRTOS���ж��йص�����ѡ��   */
/**********************************/
#ifdef __NVIC_PRIO_BITS
	#define configPRIO_BITS             __NVIC_PRIO_BITS
#else
	#define configPRIO_BITS             4
#endif
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY           15                      //�ж�������ȼ�
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY      2                       //ϵͳ�ɹ��������ж����ȼ�
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

/******************************************/
/*   FreeRTOS���жϷ������йص�����ѡ��   */
/*****************************************/
#define xPortPendSVHandler 	PendSV_Handler
#define vPortSVCHandler   	SVC_Handler
#define xPortSysTickHandler SysTick_Handler

#endif /* FREERTOS_CONFIG_H */

