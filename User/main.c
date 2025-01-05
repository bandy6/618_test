/**
  ******************************************************************************
  * @file    GPIO/IOToggle/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    05-1-2025
  * @brief   Main program body.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "bsp_led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "uart.h"

TaskHandle_t StartTask_Handler;			// 任务句柄
TaskHandle_t UartTask_Handler;			// 任务句柄


/* 任务函数 */
void start_task(void *pvParameters)
{
  (void)pvParameters;
	int i = 1;
	
	while (1)
	{
    LED1_ON;			  // 亮
		vTaskDelay(1000);
		LED1_OFF;		   // 灭
    vTaskDelay(1000);
	}
}

void uart_task(void *pvParameters)
{
  (void)pvParameters;

	while (1)
	{
    	Usart_SendString( USART1,"串口发送test\n");
      vTaskDelay(2000);
	}
}

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{	
	/* 嵌套向量中断控制器组选择 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	/* LED 端口初始化 */
	LED_GPIO_Config();	 
	
  /* 串口初始化 */
  /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
  uart_init();

	/* 创建任务 */
    xTaskCreate((TaskFunction_t )start_task,            // 任务函数
                (const char*    )"start_task",          // 任务名称
                (uint16_t       )128,        			      // 任务堆栈大小
                (void*          )NULL,                  // 传递给任务函数的参数
                (UBaseType_t    )1,       				      // 任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   // 任务句柄

	/* 创建串口任务 */
    xTaskCreate((TaskFunction_t )uart_task,            // 任务函数
                (const char*    )"uart_task",          // 任务名称
                (uint16_t       )128,        			      // 任务堆栈大小
                (void*          )NULL,                  // 传递给任务函数的参数
                (UBaseType_t    )1,       				      // 任务优先级
                (TaskHandle_t*  )&UartTask_Handler);   // 任务句柄

				
    vTaskStartScheduler();          					          // 开启任务调度
}
